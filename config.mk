# Global Makefile configuration.
# Include before any rule.

NAME = Xradar
MODE ?= release

# Build directory
DIST_DIR = $(WORKDIR)target

CXX = c++
RM = rm -rf
MKDIR = mkdir -p

CXXFLAGS_COMMON = -std=gnu++20 -Wall -Wextra -pedantic -lstdc++ -fPIC -MMD -MP
CXXFLAGS_COMPONENT = -shared
