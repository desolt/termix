SHELL = /bin/sh
TARGET = termix
SRCDIR = src
DESTDIR = build
CFLAGS += -g -Iinclude --std=c99 -Wall -Wextra -Wpedantic

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(subst $(SRCDIR),$(DESTDIR),$(SRCS:.c=.o))

OS := $(shell uname -s)
ifeq ($(OS),Darwin)
    LDLIBS += -lglfw3 -framework OpenGL
endif
ifeq ($(OS),Linux)
    LDLIBS += -lglfw -lGL
endif

.PHONY: default all clean run
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

run: all
	./$(DESTDIR)/$(TARGET)
