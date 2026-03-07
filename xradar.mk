# Xradar executable/object/library rules.
# Target declared in this Makefile are exported:
#     TARGET: Xradar executable
#     TARGET_LIB: Static library containing all Xradar symbols except for the
#                 main function

xradar_EXTERNAL_LIBS = rdesc

# Directory configuration.
xradar_SRC_DIR = $(WORKDIR)src

xradar_EXTERNAL_DIR = $(WORKDIR)external

xradar_OBJ_DIR = $(TARGET_DIR)/obj

# Source and object files.
xradar_SRCS = $(wildcard $(xradar_SRC_DIR)/*.cpp)

xradar_OBJS = $(patsubst $(xradar_SRC_DIR)/%.cpp, \
		$(xradar_OBJ_DIR)/%.o, \
		$(xradar_SRCS))

# Object files excluding the one containing main function.
xradar_LIB_OBJS = $(filter-out $(xradar_OBJ_DIR)/main.o, $(xradar_OBJS))

xradar_EXTERNAL_LIB_AR = $(foreach lib, \
			   $(xradar_EXTERNAL_LIBS), \
			   $(xradar_EXTERNAL_DIR)/lib/lib$(lib).a)

xradar_EXTERNAL_LIB_SO = $(foreach lib, \
			   $(xradar_EXTERNAL_LIBS), \
			   $(xradar_EXTERNAL_DIR)/lib/lib$(lib).so)

xradar_EXTERNAL_INCLUDE = $(xradar_EXTERNAL_DIR)/include

$(TARGET): $(xradar_OBJS) | $(xradar_EXTERNAL_INCLUDE)
	$(CXX) $(CXXFLAGS) $^ $(xradar_EXTERNAL_LIB_AR) -o $@

$(TARGET_LIB): $(xradar_LIB_OBJS) | $(xradar_EXTERNAL_INCLUDE)
	ar rcs $@ $(xradar_EXTERNAL_LIB_SO) $^

$(xradar_OBJ_DIR)/%.o: $(xradar_SRC_DIR)/%.cpp | $(xradar_OBJ_DIR) \
		$(xradar_EXTERNAL_INCLUDE)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(xradar_EXTERNAL_INCLUDE):
	cd $(xradar_EXTERNAL_DIR); $(MAKE) -j

$(xradar_OBJ_DIR) $(TARGET_DIR):
	$(MKDIR) $@


-include $(xradar_OBJS:.o=.d)
