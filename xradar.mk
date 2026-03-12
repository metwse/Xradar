# Xradar build configuration.
# Variables consisting entirely of uppercase letters are public variables.
#
#     XRADAR: Xradar executable
#     XRADAR_LIB: Static library containing all Xradar symbols except for the
#                 main function

# release or debug
XRADAR_MODE ?= release

# Directory containing Xradar files.
XRADAR_DIR ?= .

# Directory containing Xradar files.
XRADAR_INCLUDE_DIR ?= .

# private
xradar_DIST_DIR := $(XRADAR_DIR)/target
xradar_TARGET_DIR := $(xradar_DIST_DIR)/$(XRADAR_MODE)

# The main executable
XRADAR ?= $(xradar_TARGET_DIR)/Xradar
XRADAR_LIB ?= $(xradar_TARGET_DIR)/libXradar.a

# Variables below this line are private.
# -----------------------------------------------------------------------------
xradar_CXXFLAGS_COMMON = -std=gnu++20 -Wall -Wextra -pedantic -lstdc++ -fPIC

xradar_CXXFLAGS_release = $(xradar_CXXFLAGS_COMMON) -O2
xradar_CXXFLAGS_debug = $(xradar_CXXFLAGS_COMMON) -O0 -g3 --coverage

xradar_CXXFLAGS := $(xradar_CXXFLAGS_$(XRADAR_MODE))

ifeq ($(xradar_CXXFLAGS),)
$(error "WARNING: unknown mode $(XRADAR_MODE).")
endif

# Directory configuration.
xradar_SRC_DIR := $(XRADAR_DIR)/src

xradar_OBJ_DIR := $(xradar_TARGET_DIR)/obj

xradar_RDESC_DIR := $(XRADAR_DIR)/rdesc

# Source and object files.
xradar_SRCS := $(wildcard $(xradar_SRC_DIR)/*.cpp)

xradar_OBJS := $(patsubst $(xradar_SRC_DIR)/%.cpp, \
		 $(xradar_OBJ_DIR)/%.o, \
		 $(xradar_SRCS))

xradar_MKDIR := $(or $(MKDIR),mkdir -p)

# Object files excluding the one containing main function.
xradar_LIB_OBJS := $(filter-out $(xradar_OBJ_DIR)/main.o, $(xradar_OBJS))


RDESC_MODE := $(XRADAR_MODE)
RDESC_DIR := $(xradar_RDESC_DIR)
RDESC_FEATURES := full

$(xradar_RDESC_DIR)/rdesc.mk:
	git clone https://github.com/metwse/rdesc.git $(xradar_RDESC_DIR) \
		--branch v0.2.x

include $(xradar_RDESC_DIR)/rdesc.mk


$(XRADAR) $(XRADAR_LIB): CXXFLAGS := $(xradar_CXXFLAGS)

$(XRADAR): $(xradar_OBJS) $(RDESC)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(XRADAR_LIB): $(xradar_LIB_OBJS)
	$(AR) rcs $@ $^

$(xradar_OBJ_DIR)/%.o: $(xradar_SRC_DIR)/%.cpp | $(xradar_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(xradar_OBJ_DIR) $(xradar_TARGET_DIR):
	$(xradar_MKDIR) $@


-include $(xradar_OBJS:.o=.d)
