#!/bin/sh
# MC Block Finder - no-git installer
# Works in a-Shell, iSH, and any POSIX shell with curl + python3 + make + cc

set -e

REPO_RAW="https://raw.githubusercontent.com/Batthepig-two/MC-block-finder/main"
CUBIOMES_ZIP="https://github.com/Cubitect/cubiomes/archive/refs/heads/master.zip"
DEST="MC-block-finder"

echo "=== MC Block Finder Installer ==="
echo ""

# Clean up any previous attempt
rm -rf "$DEST"
mkdir -p "$DEST"
cd "$DEST"

echo "[1/3] Downloading source files..."
curl -fsSL "$REPO_RAW/main.c"   -o main.c
curl -fsSL "$REPO_RAW/blocks.c" -o blocks.c
curl -fsSL "$REPO_RAW/blocks.h" -o blocks.h
curl -fsSL "$REPO_RAW/Makefile" -o Makefile

echo "[2/3] Downloading cubiomes..."
curl -fsSL "$CUBIOMES_ZIP" -o cubiomes.zip
python3 -c "import zipfile; zipfile.ZipFile('cubiomes.zip').extractall('.')"
mv cubiomes-master cubiomes
rm cubiomes.zip

echo "[3/3] Building..."
make

echo ""
echo "Done! Run with:"
echo "  ./$DEST/mc-block-finder -h"
