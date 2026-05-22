#!/bin/sh
# MC Block Finder - no-git installer for a-shell (iOS) and similar minimal environments
# Uses only curl and clang â no make, unzip, or python needed

set -e

REPO_RAW="https://raw.githubusercontent.com/Batthepig-two/MC-block-finder/main"
CUBIOMES_RAW="https://raw.githubusercontent.com/Cubitect/cubiomes/master"
DEST="MC-block-finder"

echo "=== MC Block Finder Installer ==="
echo ""

rm -rf "$DEST"
mkdir -p "$DEST/cubiomes"
cd "$DEST"

echo "[1/3] Downloading source files..."
curl -fsSL "$REPO_RAW/main.c"   -o main.c
curl -fsSL "$REPO_RAW/blocks.c" -o blocks.c
curl -fsSL "$REPO_RAW/blocks.h" -o blocks.h

echo "[2/3] Downloading cubiomes..."
curl -fsSL "$CUBIOMES_RAW/generator.c"   -o cubiomes/generator.c
curl -fsSL "$CUBIOMES_RAW/generator.h"   -o cubiomes/generator.h
curl -fsSL "$CUBIOMES_RAW/biomes.c"      -o cubiomes/biomes.c
curl -fsSL "$CUBIOMES_RAW/biomes.h"      -o cubiomes/biomes.h
curl -fsSL "$CUBIOMES_RAW/biomenoise.c"  -o cubiomes/biomenoise.c
curl -fsSL "$CUBIOMES_RAW/biomenoise.h"  -o cubiomes/biomenoise.h
curl -fsSL "$CUBIOMES_RAW/layers.c"      -o cubiomes/layers.c
curl -fsSL "$CUBIOMES_RAW/layers.h"      -o cubiomes/layers.h
curl -fsSL "$CUBIOMES_RAW/noise.c"       -o cubiomes/noise.c
curl -fsSL "$CUBIOMES_RAW/noise.h"       -o cubiomes/noise.h
curl -fsSL "$CUBIOMES_RAW/finders.c"     -o cubiomes/finders.c
curl -fsSL "$CUBIOMES_RAW/finders.h"     -o cubiomes/finders.h
curl -fsSL "$CUBIOMES_RAW/util.c"        -o cubiomes/util.c
curl -fsSL "$CUBIOMES_RAW/util.h"        -o cubiomes/util.h
curl -fsSL "$CUBIOMES_RAW/quadbase.c"    -o cubiomes/quadbase.c
curl -fsSL "$CUBIOMES_RAW/quadbase.h"    -o cubiomes/quadbase.h
curl -fsSL "$CUBIOMES_RAW/rng.h"         -o cubiomes/rng.h

echo "[3/3] Building..."
clang -O2 -std=c99 -Icubiomes -D_POSIX_C_SOURCE=200809L \
   main.c blocks.c \
   cubiomes/generator.c cubiomes/biomes.c cubiomes/layers.c \
   cubiomes/noise.c cubiomes/finders.c cubiomes/util.c \
   cubiomes/quadbase.c cubiomes/biomenoise.c \
   -lm -o mc-block-finder

echo ""
echo "Done! Run with:"
echo "  ./mc-block-finder -h"
