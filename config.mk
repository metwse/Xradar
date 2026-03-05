# Global makefile configuration

CXX = c++
RM = rm -rf

CXXFLAGS_COMMON = -std=gnu++20 -Wall -Wextra -pedantic -lstdc++ -fPIC -MMD -MP
CXXFLAGS_COMPONENT = -shared
