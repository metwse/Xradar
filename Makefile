RM := rm -rf

ifneq ($(MODE),)
XRADAR_MODE := $(MODE)
endif

CXXFLAGS_COMPONENT = -shared


# Component directory configuration.
COMPONENT_SRC_DIR = components

COMPONENT_TARGET_DIR = $(xradar_TARGET_DIR)/components
COMPONENT_OBJ_DIR = $(xradar_TARGET_DIR)/obj/components

# Component source and object files.
COMPONENT_SRCS = $(wildcard $(COMPONENT_SRC_DIR)/*.cpp)

COMPONENT_OBJS = $(patsubst $(COMPONENT_SRC_DIR)/%.cpp, \
		   $(COMPONENT_OBJ_DIR)/%.o, \
		   $(COMPONENT_SRCS))

# Dynamic libraries for the components.
COMPONENT_TARGETS = $(patsubst $(COMPONENT_SRC_DIR)/%.cpp, \
		      $(COMPONENT_TARGET_DIR)/%.so, \
		      $(COMPONENT_SRCS))


default: _default

include xradar.mk

_default: $(XRADAR) $(XRADAR_LIB) $(COMPONENT_TARGETS)


$(COMPONENT_TARGET_DIR)/%.so: $(COMPONENT_OBJ_DIR)/%.o | $(COMPONENT_TARGET_DIR)
	$(CXX) $(xradar_CXXFLAGS_COMMON) $(CXXFLAGS_COMPONENT) $^ -o $@

.SECONDARY:
$(COMPONENT_OBJ_DIR)/%.o: $(COMPONENT_SRC_DIR)/%.cpp | $(COMPONENT_OBJ_DIR)
	$(CXX) $(xradar_CXXFLAGS_COMMON) -c $< -o $@

$(COMPONENT_TARGET_DIR) $(COMPONENT_OBJ_DIR):
	$(xradar_MKDIR) $@


clean:
	$(RM) $(xradar_DIST_DIR) docs

docs:
	doxygen

.PHONY: default _default clean docs


-include $(COMPONENT_OBJS:.o=.d)
