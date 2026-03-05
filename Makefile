NAME = Xradar
MODE ?= release

SRC_DIR = src
COMPONENT_SRC_DIR = components

DIST_DIR = target
TARGET_DIR = $(DIST_DIR)/$(MODE)

COMPONENT_TARGET_DIR = $(TARGET_DIR)/components

OBJ_DIR = $(TARGET_DIR)/obj

EXTERNAL_LIB_DIR = external/lib
EXTERNAL_LIBS = rdesc

# No need to change rules below this line

include config.mk

CXXFLAGS_release = $(CXXFLAGS_COMMON) -O2
CXXFLAGS_debug = $(CXXFLAGS_COMMON) -O0 -g3 --coverage

CXXFLAGS = $(CXXFLAGS_$(MODE))

ifeq ($(CXXFLAGS),)
$(error "WARNING: unknown mode $(MODE).")
endif

EXTERNAL_LIB_OBJS = $(foreach lib, \
		      $(EXTERNAL_LIBS), \
		      $(EXTERNAL_LIB_DIR)/lib$(lib).so)

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
COMPONENT_SRCS = $(wildcard $(COMPONENT_SRC_DIR)/*.cpp)

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# All objects: library and component
ALL_OBJS = $(wildcard $(OBJ_DIR)/*.o)

# Object files excluding the one containing main function
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

# The main executable
TARGET = $(TARGET_DIR)/$(NAME)
TARGET_LIB = $(TARGET_DIR)/lib$(NAME).a
# Component library objects
COMPONENT_TARGETS = $(patsubst $(COMPONENT_SRC_DIR)/%.cpp, \
		      $(COMPONENT_TARGET_DIR)/%.so, \
		      $(COMPONENT_SRCS))


default: $(TARGET) $(TARGET_LIB)


$(TARGET): $(OBJS) $(EXTERNAL_LIB_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGET_LIB): $(LIB_OBJS) $(EXTERNAL_LIB_OBJS)
	ar rcs $@ $^

$(COMPONENT_TARGET_DIR)/%.so: $(OBJ_DIR)/%.component.o | $(COMPONENT_TARGET_DIR)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_COMPONENT) $^ -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) $(EXTERNAL_LIB_OBJS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.SECONDARY:
$(OBJ_DIR)/%.component.o: $(COMPONENT_SRC_DIR)/%.cpp | $(OBJ_DIR) $(EXTERNAL_LIB_OBJS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR) $(COMPONENT_TARGET_DIR):
	mkdir -p $@

$(EXTERNAL_LIB_OBJS):
	cd external; make -j

all: default components

components: $(COMPONENT_TARGETS)

clean:
	$(RM) $(DIST_DIR) docs

docs:
	doxygen


-include $(ALL_OBJS:.o=.d)

.PHONY: default components all clean docs
