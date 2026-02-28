NAME = Xradar
MODE ?= release

CXX = c++
RM = rm -rf

SRC_DIR = src
TEST_SRC_DIR = tests
COMPONENT_SRC_DIR = components
TEST_COMPONENT_SRC_DIR = tests/components

DIST_DIR = target
TARGET_DIR = $(DIST_DIR)/$(MODE)

COMPONENT_TARGET_DIR = $(TARGET_DIR)/components
TEST_COMPONENT_TARGET_DIR = $(TARGET_DIR)/test-components

OBJ_DIR = $(TARGET_DIR)/obj

CXXFLAGS_COMMON = -std=gnu++20 -Wall -Wextra -pedantic -lstdc++ -fPIC -MMD -MP
CXXFLAGS_COMPONENT = -shared $(pkg-config --cflags --libs x11)

EXTERNAL_LIBS = external/lib/librdesc.a

# No need to change rules below this line

CXXFLAGS_release = $(CXXFLAGS_COMMON) -O2
CXXFLAGS_debug = $(CXXFLAGS_COMMON) -O0 -g3 --coverage

CXXFLAGS = $(CXXFLAGS_$(MODE))

ifeq ($(CXXFLAGS),)
$(error "WARNING: unknown mode $(MODE).")
endif

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS = $(wildcard $(TEST_SRC_DIR)/*.cpp)
COMPONENT_SRCS = $(wildcard $(COMPONENT_SRC_DIR)/*.cpp)
TEST_COMPONENT_SRCS = $(wildcard $(TEST_COMPONENT_SRC_DIR)/*.cpp)

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# All objects: library, test, component, and test-component
ALL_OBJS = $(wildcard $(OBJ_DIR)/*.o)

# Object files excluding the one containing main function
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

# The main executable
EXECUTABLE = $(TARGET_DIR)/$(NAME)
# Test targets
TEST_EXECUTABLES = $(patsubst $(TEST_SRC_DIR)/%.cpp, \
		     $(TARGET_DIR)/%.test, \
		     $(TEST_SRCS))
# Component library objects
COMPONENT_TARGETS = $(patsubst $(COMPONENT_SRC_DIR)/%.cpp, \
		      $(COMPONENT_TARGET_DIR)/%.so, \
		      $(COMPONENT_SRCS))
# Component library objects
TEST_COMPONENT_TARGETS = $(patsubst $(TEST_COMPONENT_SRC_DIR)/%.cpp, \
			   $(TEST_COMPONENT_TARGET_DIR)/%.so, \
			   $(TEST_COMPONENT_SRCS))


default: $(EXECUTABLE)


$(EXECUTABLE): $(OBJS) $(EXTERNAL_LIBS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGET_DIR)/%.test: $(OBJ_DIR)/%.test.o $(LIB_OBJS) $(EXTERNAL_LIBS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(COMPONENT_TARGET_DIR)/%.so: $(OBJ_DIR)/%.component.o | $(COMPONENT_TARGET_DIR)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_COMPONENT) $^ -o $@

$(TEST_COMPONENT_TARGET_DIR)/%.so: $(OBJ_DIR)/%.test-component.o | $(TEST_COMPONENT_TARGET_DIR)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_COMPONENT) $^ -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) $(EXTERNAL_LIBS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.SECONDARY:
$(OBJ_DIR)/%.test.o: $(TEST_SRC_DIR)/%.cpp | $(OBJ_DIR) $(EXTERNAL_LIBS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.SECONDARY:
$(OBJ_DIR)/%.component.o: $(COMPONENT_SRC_DIR)/%.cpp | $(OBJ_DIR) $(EXTERNAL_LIBS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.SECONDARY:
$(OBJ_DIR)/%.test-component.o: $(TEST_COMPONENT_SRC_DIR)/%.cpp | $(OBJ_DIR) $(EXTERNAL_LIBS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR) $(COMPONENT_TARGET_DIR) $(TEST_COMPONENT_TARGET_DIR):
	mkdir -p $@

$(EXTERNAL_LIBS):
	cd external; make -j

all: default tests components

tests: $(TEST_EXECUTABLES) $(TEST_COMPONENT_TARGETS)

components: $(COMPONENT_TARGETS)

clean:
	$(RM) $(DIST_DIR) docs

docs:
	doxygen


-include $(ALL_OBJS:.o=.d)

.PHONY: default tests components all clean docs
