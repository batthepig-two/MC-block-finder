#!/bin/sh
# MC Block Finder - no-git installer for a-shell (iOS) and similar minimal environments
# Uses only curl and clang — no make, unzip, or git needed

set -e

REPO_RAW="https://raw.githubusercontent.com/Batthepig-two/MC-block-finder/main"
CUBIOMES_RAW="https://raw.githubusercontent.com/Cubitect/cubiomes/master"
DEST="MC-block-finder"

echo "=== MC Block Finder Installer ==="
echo ""

rm -rf "$DEST"
mkdir -p "$DEST/cubiomes/tables"
mkdir -p "$DEST/pthread_stub"
cd "$DEST"

echo "[1/3] Downloading source files..."
curl -fsSL "$REPO_RAW/main.c"   -o main.c
curl -fsSL "$REPO_RAW/blocks.c" -o blocks.c
curl -fsSL "$REPO_RAW/blocks.h" -o blocks.h

echo "[2/3] Downloading cubiomes..."
curl -fsSL "$CUBIOMES_RAW/generator.c"         -o cubiomes/generator.c
curl -fsSL "$CUBIOMES_RAW/generator.h"         -o cubiomes/generator.h
curl -fsSL "$CUBIOMES_RAW/biomes.c"            -o cubiomes/biomes.c
curl -fsSL "$CUBIOMES_RAW/biomes.h"            -o cubiomes/biomes.h
curl -fsSL "$CUBIOMES_RAW/biomenoise.c"        -o cubiomes/biomenoise.c
curl -fsSL "$CUBIOMES_RAW/biomenoise.h"        -o cubiomes/biomenoise.h
curl -fsSL "$CUBIOMES_RAW/layers.c"            -o cubiomes/layers.c
curl -fsSL "$CUBIOMES_RAW/layers.h"            -o cubiomes/layers.h
curl -fsSL "$CUBIOMES_RAW/noise.c"             -o cubiomes/noise.c
curl -fsSL "$CUBIOMES_RAW/noise.h"             -o cubiomes/noise.h
curl -fsSL "$CUBIOMES_RAW/finders.c"           -o cubiomes/finders.c
curl -fsSL "$CUBIOMES_RAW/finders.h"           -o cubiomes/finders.h
curl -fsSL "$CUBIOMES_RAW/util.c"              -o cubiomes/util.c
curl -fsSL "$CUBIOMES_RAW/util.h"              -o cubiomes/util.h
curl -fsSL "$CUBIOMES_RAW/quadbase.c"          -o cubiomes/quadbase.c
curl -fsSL "$CUBIOMES_RAW/quadbase.h"          -o cubiomes/quadbase.h
curl -fsSL "$CUBIOMES_RAW/rng.h"               -o cubiomes/rng.h
curl -fsSL "$CUBIOMES_RAW/tables/btree18.h"    -o cubiomes/tables/btree18.h
curl -fsSL "$CUBIOMES_RAW/tables/btree19.h"    -o cubiomes/tables/btree19.h
curl -fsSL "$CUBIOMES_RAW/tables/btree192.h"   -o cubiomes/tables/btree192.h
curl -fsSL "$CUBIOMES_RAW/tables/btree20.h"    -o cubiomes/tables/btree20.h
curl -fsSL "$CUBIOMES_RAW/tables/btree21wd.h"  -o cubiomes/tables/btree21wd.h

# pthread stub for a-shell (wasm32-wasi):
#   pthread_t and pthread_attr_t are already defined by <sys/types.h> via the wasi
#   sysroot (bits/alltypes.h). This stub only adds the missing API functions so
#   quadbase.c compiles single-threaded without relying on a real pthreads library.
cat > pthread_stub/pthread.h << 'PEOF'
#pragma once
/* Functions only — types are already provided by the wasi sysroot */
static inline int pthread_create(pthread_t *t, const pthread_attr_t *a,
                                  void *(*fn)(void *), void *arg) {
    (void)a; *t = (pthread_t)0; fn(arg); return 0;
}
static inline int pthread_join(pthread_t t, void **r) { (void)t; (void)r; return 0; }
static inline void pthread_exit(void *v) { (void)v; }
PEOF

echo "[3/3] Building..."
clang -O2 -std=c99 -Icubiomes -Ipthread_stub -D_POSIX_C_SOURCE=200809L \
   main.c blocks.c \
   cubiomes/generator.c cubiomes/biomes.c cubiomes/layers.c \
   cubiomes/noise.c cubiomes/finders.c cubiomes/util.c \
   cubiomes/quadbase.c cubiomes/biomenoise.c \
   -lm -o mc-block-finder

echo ""
echo "Done! Run with:"
echo "  cd MC-block-finder && ./mc-block-finder -h"
