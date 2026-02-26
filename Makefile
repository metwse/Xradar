NAME = Xradar
MODE ?= release

CXX = c++
RM = rm -rf

SRC_DIR = src
TEST_SRC_DIR = tests
COMPONENT_SRC_DIR = components
DIST_DIR = target
TARGET_DIR = $(DIST_DIR)/$(MODE)

OBJ_DIR = $(TARGET_DIR)/obj
COMPONENT_OUT_DIR = $(TARGET_DIR)/components

CXXFLAGS_COMMON = -std=gnu++20 -Wall -Wextra -pedantic -lstdc++ -fPIC -MMD -MP
CXXFLAGS_COMPONENT = -shared

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

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
TEST_OBJS = $(patsubst $(TEST_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.test.o, $(TEST_SRCS))

# Object files excluding the one containing main function
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

# The main executable
EXECUTABLE = $(TARGET_DIR)/$(NAME)
# Test targets
TEST_EXECUTABLES = $(patsubst $(TEST_SRC_DIR)/%.cpp, \
		     $(TARGET_DIR)/%.test, \
		     $(TEST_SRCS))
# Component library objects
COMPONENT_LIBRARIES = $(patsubst $(COMPONENT_SRC_DIR)/%.cpp, \
			$(COMPONENT_OUT_DIR)/%.so, \
			$(COMPONENT_SRCS))


default: $(EXECUTABLE)


$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGET_DIR)/%.test: $(OBJ_DIR)/%.test.o $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(COMPONENT_OUT_DIR)/%.so: $(OBJ_DIR)/%.component.o | $(COMPONENT_OUT_DIR)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_COMPONENT) $^ -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.SECONDARY:
$(OBJ_DIR)/%.test.o: $(TEST_SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.SECONDARY:
$(OBJ_DIR)/%.component.o: $(COMPONENT_SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR) $(COMPONENT_OUT_DIR):
	mkdir -p $@


all: default tests components

tests: $(TEST_EXECUTABLES)

components: $(COMPONENT_LIBRARIES)

clean:
	$(RM) $(DIST_DIR) docs

docs:
	doxygen


-include $(OBJS:.o=.d)
-include $(TEST_OBJS:.o=.d)

.PHONY: default tests components all clean docs
