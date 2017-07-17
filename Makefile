# Variables
NAME     := termix
SRCDIR    = src
OBJDIR    = obj
INCDIR    = include
NDEBUG   ?= 0
OS       := $(shell uname -s)
APP_PATH := $(NAME)
LIB_PATH := $(OBJDIR)/lib$(NAME).a
SRCS      = $(wildcard $(SRCDIR)/*.c)
OBJS      = $(subst $(SRCDIR),$(OBJDIR),$(SRCS:.c=.o))
RM        = rm -vf

# Compilation flags
CC_FLAGS      := $(CFLAGS) -I$(INCDIR) --std=c99 -Wall -Wextra -pedantic
CC_FLAGS      += $(shell freetype-config --cflags)
LD_FLAGS      := $(shell pkg-config --libs glfw3) $(shell freetype-config --libs)
LD_FLAGS_TEST := -L./$(OBJDIR) -l$(NAME)


# OS-specific
ifeq ($(OS),Darwin)
	LD_FLAGS      += -framework OpenGL
else
ifeq ($(OS),Linux)
	LD_FLAGS      += $(shell pkg-config --libs gl)
	CC_FLAGS      += -D_DEFAULT_SOURCE $(shell pkg-config --cflags gl)
	LD_FLAGS_TEST += -lrt
endif


# Targets
.PHONY: default all clean cleanformat run format test

default: all

all: $(APP_PATH)

format:
	astyle --options=".astyle" "$(SRCDIR)/*.c" "tests/*.c" "$(INCDIR)/*.h"

run: all
	./$(APP_PATH)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean: cleanformat
	$(RM) -r $(OBJDIR)
	$(RM)  $(APP_PATH)

cleanformat:
	$(RM) -r $(SRC)/*.orig
	$(RM) -r $(INCDIR)/*.orig
	$(RM) -r $(SRCDIR_TEST)/*.orig

options:
	@echo "CFLAGS   = $(CC_FLAGS)"
	@echo "LDFLAGS  = $(LD_FLAGS)"
	@echo "CC       = $(CC)"

# Test/debug section
ifeq ($(NDEBUG), 0)
	# Variables
	SRCDIR_TEST  = tests
	OBJDIR_TEST := $(OBJDIR)/$(SRCDIR_TEST)
	TEST_SRCS   := $(wildcard $(SRCDIR_TEST)/*.c)
	TEST_OBJS   := $(subst $(SRCDIR_TEST),$(OBJDIR)/$(SRCDIR_TEST), $(TEST_SRCS:.c=))

	# Targets
$(OBJDIR_TEST):
		mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
		$(CC) $(CC_FLAGS) -g -fPIC -c $< -o $@

$(OBJDIR_TEST)/%: $(SRCDIR_TEST)/%.c $(LIB_PATH)
		$(CC) $(CC_FLAGS) $< -o $@ $(LD_FLAGS) $(LD_FLAGS_TEST)

$(LIB_PATH): $(OBJDIR) $(OBJS)
		$(AR) $(ARFLAGS) $@ $(OBJS)

$(APP_PATH): $(LIB_PATH)
		$(CC) -I$(INCDIR) $(SRCDIR)/main.c -o $@ -L$(OBJDIR) -l$(NAME) $(LD_FLAGS)

test: $(LIB_PATH) $(OBJDIR_TEST) $(TEST_OBJS)
		@./tests/run_tests.sh $(TEST_OBJS)

# Release section
else
$(OBJDIR)/%.o: $(SRCDIR)/%.c
		$(CC) $(CC_FLAGS) -DNDEBUG -c $< -o $@

$(APP_PATH): $(OBJDIR) $(OBJS)
		$(CC) $(CC_FLAGS) $(OBJS) -o $@ $(LD_FLAGS)

test:
		@echo "Error: can't run tests when NDEBUG=1" && exit 1

endif
endif
