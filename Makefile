NAME = Xradar
MODE ?= release

CC = cc
RM = rm -rf

SRC_DIR = src
DIST_DIR = target
TARGET_DIR = $(DIST_DIR)/$(MODE)

OBJ_DIR = $(TARGET_DIR)/obj

CFLAGS_COMMON = -std=gnu18 -Wall -Wextra -pedantic -fPIC -MMD -MP

# No need to change rules below this line

CFLAGS_release = $(CFLAGS_COMMON) -O2
CFLAGS_debug = $(CFLAGS_COMMON) -O0 -g3 --coverage

CFLAGS = $(CFLAGS_$(MODE))

ifeq ($(CFLAGS),)
$(error "WARNING: unknown mode $(MODE).")
endif

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

EXECUTABLE = $(DIST_DIR)/$(NAME).$(MODE)


default: $(EXECUTABLE)


$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@


clean:
	$(RM) $(DIST_DIR) docs

docs:
	doxygen


-include $(OBJS:.o=.d)

.PHONY: default clean docs
