CC      = cc
CFLAGS  = -O2 -Wall -Wextra -std=c99 -Icubiomes -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lm

TARGET  = mc-block-finder
SRCS    = main.c blocks.c cubiomes/generator.c cubiomes/biomes.c \
          cubiomes/layers.c cubiomes/noise.c cubiomes/finders.c \
          cubiomes/util.c cubiomes/quadbase.c cubiomes/biomenoise.c

OBJS    = $(SRCS:.c=.o)

.PHONY: all clean cubiomes

all: cubiomes $(TARGET)

cubiomes:
        @if [ ! -f cubiomes/generator.h ]; then \
                echo "Fetching cubiomes submodule..."; \
                git submodule update --init --recursive; \
        fi

$(TARGET): $(OBJS)
        $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
        $(CC) $(CFLAGS) -c -o $@ $<

clean:
        rm -f $(OBJS) $(TARGET)

install: $(TARGET)
        install -m 755 $(TARGET) /usr/local/bin/$(TARGET)
