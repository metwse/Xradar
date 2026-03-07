# Global Makefile configuration.
# Include before any rule.

NAME = Xradar
MODE ?= release

# Build directory
DIST_DIR = $(WORKDIR)target
TARGET_DIR = $(DIST_DIR)/$(MODE)

# The main executable
TARGET = $(TARGET_DIR)/$(NAME)
TARGET_LIB = $(TARGET_DIR)/lib$(NAME).a

CXX = c++
RM = rm -rf
MKDIR = mkdir -p

CXXFLAGS_COMMON = -std=gnu++20 -Wall -Wextra -pedantic -lstdc++ -fPIC -MMD -MP
CXXFLAGS_COMPONENT = -shared
