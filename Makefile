SHELL = /bin/sh
TARGET = termix
SRCDIR = src
DESTDIR = build
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(subst $(SRCDIR),$(DESTDIR),$(SRCS:.c=.o))
CFLAGS += -g -Iinclude
OS := $(shell uname -s)
ifeq ($(OS),Darwin)
    LDLIBS += -lglfw3 -framework OpenGL
endif
ifeq ($(OS),Linux)
    LDLIBS += -lglfw -lm -lGL
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
