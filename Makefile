SHELL = /bin/sh
SRCDIR = src
DESTDIR = build
TEST_SRCDIR = tests
TEST_DESTDIR = $(DESTDIR)/tests
EXE_TARGET = $(DESTDIR)/termix
LIB_TARGET = $(DESTDIR)/libtermix.a
NDEBUG ?= 0
CFLAGS += -Iinclude --std=c99 -Wall -Wextra -pedantic $(shell freetype-config --cflags)
LDLIBS += -lglfw $(shell freetype-config --libs)

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(subst $(SRCDIR),$(DESTDIR),$(SRCS:.c=.o))

TEST_SRCS = $(wildcard $(TEST_SRCDIR)/*.c)
TEST_EXES = $(subst $(TEST_SRCDIR),$(DESTDIR)/tests, $(TEST_SRCS:.c=))

OS := $(shell uname -s)
ifeq ($(OS),Darwin)
    LDLIBS += -framework OpenGL
endif
ifeq ($(OS),Linux)
    LDLIBS += -lGL
    CFLAGS += -D_DEFAULT_SOURCE
endif

.PHONY: default all clean cleanformat run format test
.SUFFIXES:
.SUFFIXES: .c .o
default: all

$(TEST_DESTDIR)/%: $(TEST_SRCDIR)/%.c $(LIB_TARGET)
	$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@ $(LDLIBS) -L./$(DESTDIR) -ltermix -lrt

$(DESTDIR):
	mkdir -p $(DESTDIR)

$(TEST_DESTDIR):
	mkdir -p $@

# TODO: Messy if-statement here, how can this be cleaned up?
ifeq ($(NDEBUG), 0)
# We're in debug mode, build a library
$(DESTDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -g -fPIC -c $< -o $@

$(LIB_TARGET): $(DESTDIR) $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(EXE_TARGET): $(LIB_TARGET)
	$(CC) $(LDFLAGS) -Iinclude $(SRCDIR)/main.c -o $@ -L$(DESTDIR) -ltermix $(LDLIBS)

test: $(LIB_TARGET) $(TEST_DESTDIR) $(TEST_EXES)
	@./tests/run_tests.sh $(TEST_EXES)
else
# We're in release mode, don't build a library
$(DESTDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -DNDEBUG -c $< -o $@

$(EXE_TARGET): $(DESTDIR) $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

test:
	@echo "\033[0;31merror: can't run tests when NDEBUG=1\033[0m" && exit 1

endif

all: $(EXE_TARGET)

clean: cleanformat
 	# Could be $(DESTDIR)/*, but in case $(DESTDIR) is not set...
	rm -rf $(DESTDIR)

cleanformat:
	rm -rf src/*.orig
	rm -rf include/*.orig
	rm -rf tests/*.orig

run: all
	./$(EXE_TARGET)

format:
	astyle --options=".astyle" "src/*.c" "tests/*.c" "include/*.h"
