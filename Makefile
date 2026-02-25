NAME = Xradar
MODE ?= release

CXX = c++
RM = rm -rf

SRC_DIR = src
TEST_SRC_DIR = tests
DIST_DIR = target
TARGET_DIR = $(DIST_DIR)/$(MODE)

OBJ_DIR = $(TARGET_DIR)/obj

CXXFLAGS_COMMON = -std=gnu++20 -Wall -Wextra -pedantic -lstdc++ -fPIC -MMD -MP

# No need to change rules below this line

CXXFLAGS_release = $(CXXFLAGS_COMMON) -O2
CXXFLAGS_debug = $(CXXFLAGS_COMMON) -O0 -g3 --coverage

CXXFLAGS = $(CXXFLAGS_$(MODE))

ifeq ($(CXXFLAGS),)
$(error "WARNING: unknown mode $(MODE).")
endif

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS = $(wildcard $(TEST_SRC_DIR)/*.cpp)

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
TEST_OBJS = $(patsubst $(TEST_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.test.o, $(TEST_SRCS))

# Object files excluding the one containing main function
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

# The main executable
EXECUTABLE = $(DIST_DIR)/$(NAME).$(MODE)
# Test targets
TEST_EXECUTABLES = $(patsubst $(TEST_SRC_DIR)/%.cpp,$(TARGET_DIR)/%.test,$(TEST_SRCS))


default: $(EXECUTABLE)


$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGET_DIR)/%.test: $(OBJ_DIR)/%.test.o $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.SECONDARY:
$(OBJ_DIR)/%.test.o: $(TEST_SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@


all: default tests

tests: $(TEST_EXECUTABLES)

clean:
	$(RM) $(DIST_DIR) docs

docs:
	doxygen


-include $(OBJS:.o=.d)
-include $(TEST_OBJS:.o=.d)

.PHONY: default tests all clean docs
