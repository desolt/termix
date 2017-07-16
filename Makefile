SHELL = /bin/sh
SRCDIR = src
DESTDIR = build
TEST_SRCDIR = tests
TEST_DESTDIR = $(DESTDIR)/tests
EXE_TARGET = $(DESTDIR)/termix
LIB_TARGET = $(DESTDIR)/libtermix.a
CFLAGS += -g -Iinclude --std=c99 -Wall -Wextra -Wpedantic $(shell freetype-config --cflags)
LDLIBS += -L$(DESTDIR) -ltermix -lglfw $(shell freetype-config --libs)

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

$(DESTDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(TEST_DESTDIR)/%: $(TEST_SRCDIR)/%.c
	$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@ $(LDLIBS)

$(DESTDIR):
	mkdir -p $(DESTDIR)

$(TEST_DESTDIR):
	mkdir -p $@

$(LIB_TARGET): $(DESTDIR) $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(EXE_TARGET): $(LIB_TARGET)
	$(CC) $(LDFLAGS) -Iinclude $(SRCDIR)/main.c -o $@ $(LDLIBS)

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

test: $(LIB_TARGET) $(TEST_DESTDIR) $(TEST_EXES)
	@./tests/run_tests.sh $(TEST_EXES)

format:
	astyle --options=".astyle" "src/*.c" "tests/*.c" "include/*.h"
