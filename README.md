# MC Block Finder

A CLI tool that uses [cubiomes](https://github.com/Cubitect/cubiomes) to locate areas in a Minecraft Java Edition world by iteratively searching for clusters of blocks.

You input a seed, a Minecraft version, and a chain of search steps. Each step searches near the previous results for a specific block type, narrowing down to increasingly precise coordinates. The more steps you add, the more certain the result — but the longer it takes.

> **Note:** cubiomes simulates world *generation*. This tool can only find **world-generated blocks** (ores, biome-specific blocks, structures, etc.) — not player-placed blocks.

---

## Requirements

### Installer (`install.sh`) — no git required
- `curl`
- `clang` (built into a-Shell; available on macOS; install via your package manager on Linux)

### Build from source
- `git`
- `make`
- A C compiler (`cc` / `clang` / `gcc`)

Tested on: macOS, Linux, a-Shell (iOS), iSH (iOS).

---

## Install (no git required — works in a-Shell, iSH, etc.)

**Step 1:** Download the installer

```sh
curl -fsSL https://raw.githubusercontent.com/Batthepig-two/MC-block-finder/main/install.sh -o install.sh
```

**Step 2:** Run it

```sh
sh install.sh
```

This downloads all source files and cubiomes, then compiles with `clang`. No git or make needed.

> **a-Shell note:** a-Shell has `clang` built in — no extra packages required.
> The installer automatically cleans up any previous failed attempt before each run.

---

## After installing

The binary ends up at `MC-block-finder/mc-block-finder`. To run it:

```sh
cd MC-block-finder
./mc-block-finder -h
```

List all supported block names:

```sh
./mc-block-finder -l
```

Quick example — find diamond ore near emerald ore on seed 123456789 (version 1.18):

```sh
./mc-block-finder -s 123456789 -v 1.18 -r 3000 \
  -b emerald_ore -S 500 -C 32 \
  -b diamond_ore -S 100 -C 16
```

---

## Build from source (if you have git)

```sh
git clone https://github.com/Batthepig-two/MC-block-finder.git
cd MC-block-finder
git submodule update --init --recursive
make
```

This produces the `mc-block-finder` binary in the current directory.

---

## Usage

### Non-interactive (scriptable)

```
./mc-block-finder -s SEED -v VERSION [OPTIONS] -b BLOCK -S SEARCH_R -C CLUSTER_R [...]
```

| Flag | Description |
|------|-------------|
| `-s SEED` | World seed (required) |
| `-v VERSION` | Minecraft version, e.g. `1.18`, `1.21` (required) |
| `-x X` | Center X coordinate (default: 0) |
| `-z Z` | Center Z coordinate (default: 0) |
| `-r RADIUS` | Initial search radius in blocks (default: 5000) |
| `-O SORT` | Sort final results: `x`, `z`, or `dist` (distance from center) |
| `-D` | Reverse the sort order (descending) |
| `-b BLOCK` | Block name for this step (use `-l` to list all) |
| `-S RADIUS` | Search radius around each previous result |
| `-C RADIUS` | Cluster radius — seeds the next step |
| `-i` | Interactive mode (prompts for each step) |
| `-l` | List all supported block names and exit |
| `-h` | Show help |

Steps are processed in order: `-b`/`-S`/`-C` must come as a group. You can chain as many steps as you like (up to 16).

While running, each step shows a live progress bar:

```
[###############---------------]  50%  312 hits found
```

The bar updates in-place as biome rows are scanned. When the step finishes, the total hit count is printed before moving on.

### Example: Find diamond ore near emerald ore

```sh
./mc-block-finder -s 123456789 -v 1.18 -r 3000 -O dist \
  -b emerald_ore -S 500 -C 32 \
  -b diamond_ore -S 100 -C 16
```

- Searches a 3000-block radius around (0, 0) for emerald ore (mountains only)
- For each emerald ore hit, searches within 500 blocks for diamond ore
- Final results are the diamond ore locations within 16 blocks of those hits

### Example: Ancient debris in the Nether

```sh
./mc-block-finder -s -9876543210 -v 1.16 -r 2000 \
  -b ancient_debris -S 200 -C 8
```

### Interactive mode

```sh
./mc-block-finder -s 123456789 -v 1.18 -i
```

Prompts you step by step. Type `list` at any block prompt to see all supported blocks.

---

## Supported Versions

All versions cubiomes supports:
`1.0`, `1.2`-`1.6`, `1.7`-`1.12`, `1.13`-`1.21`

Pass any `1.x` or `1.x.y` string — patch versions map to their minor version.

---

## Supported Blocks

Run `./mc-block-finder -l` to list all supported blocks. Includes:

- All ores (coal, iron, copper, gold, redstone, lapis, diamond, emerald, nether gold, quartz, ancient debris)
- Stone variants, deepslate, tuff, gravel, sand, clay, terracotta
- All tree logs and leaves
- Biome-specific vegetation (bamboo, cactus, coral, kelp, mangrove, azalea, sculk, etc.)
- Nether blocks (netherrack, soul sand, crimson/warped nylium, blackstone, basalt, etc.)
- End blocks (end stone, chorus plant, purpur, etc.)
- Structure blocks (stone bricks, mossy cobblestone, spawner, chest, etc.)
- Amethyst geodes, dripstone, lush cave blocks
- All flowers, mushrooms, and biome-specific plants
- Archaeology blocks, trial chambers (1.21)

Block names are case-insensitive. You can also use display names (e.g. `"Diamond Ore"`).

---

## How It Works

1. Starts from your center coordinates (default 0, 0)
2. For step 1: samples biomes across the initial radius in 16-block increments
3. Keeps every chunk where the target blocks biome requirements are met
4. For each subsequent step: searches within `SEARCH_RADIUS` of each previous hit
5. Outputs the final list of candidate coordinates

Because biome simulation is fast, the search is efficient even over large radii. Precision increases with more steps.

---

## Limitations

- Only works with **Java Edition** world generation (cubiomes limitation)
- Searches biome compatibility, not exact block placement — results are candidate areas, not guaranteed block positions
- Maximum 16 steps per run
- Maximum 65,536 candidate results per step

---

## License

MIT. cubiomes is copyright Cubitect, licensed under MIT.
