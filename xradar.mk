# Xradar executable/object/library rules.
# Target declared in this Makefile are exported:
#     TARGET: Xradar executable
#     TARGET_LIB: Static library containing all Xradar symbols except for the
#                 main function

# Directory configuration.
xradar_SRC_DIR = $(WORKDIR)/src

xradar_OBJ_DIR = $(TARGET_DIR)/obj

xradar_RDESC_DIR = $(WORKDIR)/rdesc

# Source and object files.
xradar_SRCS = $(wildcard $(xradar_SRC_DIR)/*.cpp)

xradar_OBJS = $(patsubst $(xradar_SRC_DIR)/%.cpp, \
		$(xradar_OBJ_DIR)/%.o, \
		$(xradar_SRCS))

# Object files excluding the one containing main function.
xradar_LIB_OBJS = $(filter-out $(xradar_OBJ_DIR)/main.o, $(xradar_OBJS))


RDESC_MODE := $(MODE)
RDESC_DIR := $(xradar_RDESC_DIR)
RDESC_FEATURES := full

$(xradar_RDESC_DIR)/rdesc.mk:
	git clone https://github.com/metwse/rdesc.git $(xradar_RDESC_DIR) \
		--branch v0.2.x

include $(xradar_RDESC_DIR)/rdesc.mk


$(TARGET): $(xradar_OBJS) $(RDESC)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGET_LIB): $(xradar_LIB_OBJS)
	$(AR) rcs $@ $^

$(xradar_OBJ_DIR)/%.o: $(xradar_SRC_DIR)/%.cpp | $(xradar_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(xradar_OBJ_DIR) $(TARGET_DIR):
	$(MKDIR) $@


-include $(xradar_OBJS:.o=.d)
