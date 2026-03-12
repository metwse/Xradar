include config.mk


# Component directory configuration.
COMPONENT_SRC_DIR = components

COMPONENT_TARGET_DIR = $(DIST_DIR)/$(MODE)/components
COMPONENT_OBJ_DIR = $(DIST_DIR)/$(MODE)/obj/components

# Set CXXFLAGS based on MODE variable.
CXXFLAGS_release = $(CXXFLAGS_COMMON) -O2
CXXFLAGS_debug = $(CXXFLAGS_COMMON) -O0 -g3 --coverage

CXXFLAGS = $(CXXFLAGS_$(MODE))

ifeq ($(CXXFLAGS),)
$(error "WARNING: unknown mode $(MODE).")
endif

# Component source and object files.
COMPONENT_SRCS = $(wildcard $(COMPONENT_SRC_DIR)/*.cpp)

COMPONENT_OBJS = $(patsubst $(COMPONENT_SRC_DIR)/%.cpp, \
		   $(COMPONENT_OBJ_DIR)/%.o, \
		   $(COMPONENT_SRCS))

# Dynamic libraries for the components.
COMPONENT_TARGETS = $(patsubst $(COMPONENT_SRC_DIR)/%.cpp, \
		      $(COMPONENT_TARGET_DIR)/%.so, \
		      $(COMPONENT_SRCS))


default: $(TARGET) $(TARGET_LIB) $(COMPONENT_TARGETS)

include xradar.mk


$(COMPONENT_TARGET_DIR)/%.so: $(COMPONENT_OBJ_DIR)/%.o | $(COMPONENT_TARGET_DIR)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_COMPONENT) $^ -o $@

.SECONDARY:
$(COMPONENT_OBJ_DIR)/%.o: $(COMPONENT_SRC_DIR)/%.cpp | $(COMPONENT_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(COMPONENT_TARGET_DIR) $(COMPONENT_OBJ_DIR):
	$(MKDIR) $@


clean:
	$(RM) $(DIST_DIR) docs

docs:
	doxygen

.PHONY: default clean docs


-include $(COMPONENT_OBJS:.o=.d)
