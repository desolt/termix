SHELL = /bin/sh
TARGET = termix
SRCDIR = src
DESTDIR = build
CFLAGS += -g -Iinclude --std=c99 -Wall -Wextra -Wpedantic $(shell freetype-config --cflags)
LDLIBS += -lglfw $(shell freetype-config --libs)

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(subst $(SRCDIR),$(DESTDIR),$(SRCS:.c=.o))

OS := $(shell uname -s)
ifeq ($(OS),Darwin)
    LDLIBS += -framework OpenGL
endif
ifeq ($(OS),Linux)
    LDLIBS += -lGL
    CFLAGS += -D_DEFAULT_SOURCE
endif

.PHONY: default all clean run format
.SUFFIXES:
.SUFFIXES: .c .o
default: all

$(DESTDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DESTDIR):
	mkdir -p $(DESTDIR)

all: $(DESTDIR) $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(DESTDIR)/$(TARGET) $(LDLIBS)

clean:
 	# Could be $(DESTDIR)/*, but in case $(DESTDIR) is not set...
	rm -rf $(DESTDIR)
	rm src/*.orig
	rm include/*.orig

run: all
	./$(DESTDIR)/$(TARGET)

format:
	astyle --options=".astyle" "src/*.c" "include/*.h"
