# Xradar executable/object/library rules.
# Target declared in this Makefile are exported:
#     TARGET: Xradar executable
#     TARGET_LIB: Static library containing all Xradar symbols except for the
#                 main function

xradar_EXTERNAL_LIBS = rdesc

# Directory configuration.
xradar_SRC_DIR = $(WORKDIR)src

xradar_EXTERNAL_DIR = $(WORKDIR)external

xradar_OBJ_DIR = $(xradar_TARGET_DIR)/obj
xradar_TARGET_DIR = $(DIST_DIR)/$(MODE)

# Source and object files.
xradar_SRCS = $(wildcard $(xradar_SRC_DIR)/*.cpp)

xradar_OBJS = $(patsubst $(xradar_SRC_DIR)/%.cpp, \
		$(xradar_OBJ_DIR)/%.o, \
		$(xradar_SRCS))

# Object files excluding the one containing main function.
xradar_LIB_OBJS = $(filter-out $(xradar_OBJ_DIR)/main.o, $(xradar_OBJS))

xradar_EXTERNAL_LIB_OBJS = $(foreach lib, \
			     $(xradar_EXTERNAL_LIBS), \
			     $(xradar_EXTERNAL_DIR)/lib/lib$(lib).so)

# The main executable
TARGET = $(xradar_TARGET_DIR)/$(NAME)
TARGET_LIB = $(xradar_TARGET_DIR)/lib$(NAME).a

$(TARGET): $(xradar_OBJS) $(xradar_EXTERNAL_LIB_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGET_LIB): $(xradar_LIB_OBJS) $(xradar_EXTERNAL_LIB_OBJS)
	ar rcs $@ $^

$(xradar_OBJ_DIR)/%.o: $(xradar_SRC_DIR)/%.cpp | $(xradar_OBJ_DIR) \
		$(xradar_EXTERNAL_LIB_OBJS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(xradar_EXTERNAL_LIB_OBJS):
	cd $(xradar_EXTERNAL_DIR); $(MAKE) -j

$(xradar_OBJ_DIR) $(xradar_TARGET_DIR):
	$(MKDIR) $@


-include $(xradar_OBJS:.o=.d)
