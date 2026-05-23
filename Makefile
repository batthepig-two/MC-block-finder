CC      = cc
CFLAGS  = -O2 -Wall -Wextra -std=c99 -Icubiomes -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lm

TARGET  = relay
OBJS    = main.o blocks.o cubiomes/generator.o cubiomes/biomes.o \
          cubiomes/layers.o cubiomes/noise.o cubiomes/finders.o \
          cubiomes/util.o cubiomes/quadbase.o cubiomes/biomenoise.o

.PHONY: all clean cubiomes

all: cubiomes $(TARGET)

cubiomes:
        @if [ ! -f cubiomes/generator.h ]; then \
                echo "Fetching cubiomes submodule..."; \
                git submodule update --init --recursive; \
        fi

$(TARGET): $(OBJS)
        $(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

.c.o:
        $(CC) $(CFLAGS) -c -o $@ $<

clean:
        rm -f $(OBJS) $(TARGET)

install: $(TARGET)
        install -m 755 $(TARGET) /usr/local/bin/$(TARGET)
