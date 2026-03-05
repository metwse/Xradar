INTEGRATION_DIR = integration
COMPONENT_DIR = components

LIB = ../target/debug/libXradar.a

DIST_DIR = target
OBJ_DIR = $(DIST_DIR)/obj

# No need to change rules below this line.

export MODE=debug

include ../config.mk

CXXFLAGS = $(CXXFLAGS_COMMON) -O0 -g3 --coverage

INTEGRATION_SRCS = $(wildcard $(INTEGRATION_DIR)/*.cpp)
COMPONENT_SRCS = $(wildcard $(COMPONENT_DIR)/*.cpp)

INTEGRATION_TARGETS = \
	$(patsubst $(INTEGRATION_DIR)/%.cpp, $(DIST_DIR)/%.test, $(INTEGRATION_SRCS))

COMPONENT_TARGETS = \
	$(patsubst $(COMPONENT_DIR)/%.cpp, $(DIST_DIR)/%.so, $(COMPONENT_SRCS))

ALL_OBJS = $(wildcard $(OBJ_DIR)/*.o)


default: check-lib
	$(MAKE) $(INTEGRATION_TARGETS) $(COMPONENT_TARGETS)


# Test build rules compile from project root for gcovr compatibility.
# - INTEGRATION ---------------------------------------------------------------
.SECONDARY:
$(OBJ_DIR)/%.test.o: $(INTEGRATION_DIR)/%.cpp | $(OBJ_DIR)
	cd ..; $(CXX) $(CXXFLAGS) -c tests/$< -o tests/$@
	$(CXX) -MM $< -MF $(@:.o=.d) -MT $@
$(DIST_DIR)/%.test: $(OBJ_DIR)/%.test.o $(LIB) \
		| $(DIST_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# - COMPONENTS ----------------------------------------------------------------
.SECONDARY:
$(OBJ_DIR)/%.component.o: $(COMPONENT_DIR)/%.cpp | $(OBJ_DIR)
	cd ..; $(CXX) $(CXXFLAGS) -c tests/$< -o tests/$@
	$(CXX) -MM $< -MF $(@:.o=.d) -MT $@
$(DIST_DIR)/%.so: $(OBJ_DIR)/%.component.o $(LIB) \
		| $(DIST_DIR)
	$(CXX) -shared $(CXXFLAGS) $^ -o $@


check-lib $(LIB):
	$(MAKE) -C ..


$(DIST_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) $(DIST_DIR)


-include $(ALL_OBJS:.o=.d)
