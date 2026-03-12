INTEGRATION_DIR = integration
COMPONENT_DIR = components

_default: default

XRADAR_MODE := debug
XRADAR_DIR := ..

include ../xradar.mk


TEST_TARGET_DIR = $(xradar_DIST_DIR)/tests
OBJ_DIR = $(TEST_TARGET_DIR)/obj

INTEGRATION_SRCS = $(wildcard $(INTEGRATION_DIR)/*.cpp)
COMPONENT_SRCS = $(wildcard $(COMPONENT_DIR)/*.cpp)

INTEGRATION_TARGETS = \
	$(patsubst $(INTEGRATION_DIR)/%.cpp, \
	  $(TEST_TARGET_DIR)/%.test, \
	  $(INTEGRATION_SRCS))

COMPONENT_TARGETS = \
	$(patsubst $(COMPONENT_DIR)/%.cpp, \
	  $(TEST_TARGET_DIR)/%.so, \
	  $(COMPONENT_SRCS))

default: $(INTEGRATION_TARGETS) $(COMPONENT_TARGETS)


CXXFLAGS = $(xradar_CXXFLAGS_COMMON) -O0 -g3 --coverage


# Test build rules compile from project root for gcovr compatibility.
# - INTEGRATION ---------------------------------------------------------------
.SECONDARY:
$(OBJ_DIR)/%.test.o: $(INTEGRATION_DIR)/%.cpp | $(OBJ_DIR)
	cd ..; $(CXX) $(CXXFLAGS) -c tests/$< -o tests/$@
	$(CXX) -MM $< -MF $(@:.o=.d) -MT $@

$(TEST_TARGET_DIR)/%.test: $(OBJ_DIR)/%.test.o $(XRADAR_LIB) $(RDESC) \
		| $(TEST_TARGET_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# - COMPONENTS ----------------------------------------------------------------
.SECONDARY:
$(OBJ_DIR)/%.component.o: $(COMPONENT_DIR)/%.cpp | $(OBJ_DIR)
	cd ..; $(CXX) $(CXXFLAGS) -c tests/$< -o tests/$@
	$(CXX) -MM $< -MF $(@:.o=.d) -MT $@

$(TEST_TARGET_DIR)/%.so: $(OBJ_DIR)/%.component.o $(XRADAR_LIB) $(RDESC) \
		| $(TEST_TARGET_DIR)
	$(CXX) -shared $(CXXFLAGS) $^ -o $@

$(TEST_TARGET_DIR) $(OBJ_DIR):
	mkdir -p $@


clean:
	$(RM) $(TEST_TARGET_DIR)


ALL_OBJS = $(wildcard $(OBJ_DIR)/*.o)
-include $(ALL_OBJS:.o=.d)

.PHONY: default _default clean
