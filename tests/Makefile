export MODE=debug
export WORKDIR=../

include ../config.mk


INTEGRATION_DIR = integration
COMPONENT_DIR = components

TARGET_DIR = $(DIST_DIR)/tests
OBJ_DIR = $(TARGET_DIR)/obj

CXXFLAGS = $(CXXFLAGS_COMMON) -O0 -g3 --coverage

INTEGRATION_SRCS = $(wildcard $(INTEGRATION_DIR)/*.cpp)
COMPONENT_SRCS = $(wildcard $(COMPONENT_DIR)/*.cpp)

INTEGRATION_TARGETS = \
	$(patsubst $(INTEGRATION_DIR)/%.cpp, $(TARGET_DIR)/%.test, $(INTEGRATION_SRCS))

COMPONENT_TARGETS = \
	$(patsubst $(COMPONENT_DIR)/%.cpp, $(TARGET_DIR)/%.so, $(COMPONENT_SRCS))

ALL_OBJS = $(wildcard $(OBJ_DIR)/*.o)


default: $(INTEGRATION_TARGETS) $(COMPONENT_TARGETS)

include ../xradar.mk

# Test build rules compile from project root for gcovr compatibility.
# - INTEGRATION ---------------------------------------------------------------
.SECONDARY:
$(OBJ_DIR)/%.test.o: $(INTEGRATION_DIR)/%.cpp | $(OBJ_DIR) $(TARGET_LIB)
	cd ..; $(CXX) $(CXXFLAGS) -c tests/$< -o tests/$@
	$(CXX) -MM $< -MF $(@:.o=.d) -MT $@
$(TARGET_DIR)/%.test: $(OBJ_DIR)/%.test.o $(TARGET_LIB) \
		| $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# - COMPONENTS ----------------------------------------------------------------
.SECONDARY:
$(OBJ_DIR)/%.component.o: $(COMPONENT_DIR)/%.cpp | $(OBJ_DIR) $(TARGET_LIB)
	cd ..; $(CXX) $(CXXFLAGS) -c tests/$< -o tests/$@
	$(CXX) -MM $< -MF $(@:.o=.d) -MT $@
$(TARGET_DIR)/%.so: $(OBJ_DIR)/%.component.o $(TARGET_LIB) \
		| $(TARGET_DIR)
	$(CXX) -shared $(CXXFLAGS) $^ -o $@

$(TARGET_DIR) $(OBJ_DIR):
	mkdir -p $@


clean:
	$(RM) $(TARGET_DIR)


-include $(ALL_OBJS:.o=.d)
