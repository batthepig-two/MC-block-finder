#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "blocks.h"
#include "cubiomes/biomes.h"
#include "cubiomes/generator.h"

/* -----------------------------------------------------------------------
 * Biome ID shorthand macros (cubiomes names)
 * --------------------------------------------------------------------- */
#define B_OCEAN               ocean
#define B_PLAINS              plains
#define B_DESERT              desert
#define B_MOUNTAINS           windswept_hills
#define B_FOREST              forest
#define B_TAIGA               taiga
#define B_SWAMP               swamp
#define B_RIVER               river
#define B_FROZEN_OCEAN        frozen_ocean
#define B_FROZEN_RIVER        frozen_river
#define B_SNOWY_PLAINS        snowy_plains
#define B_MUSHROOM_FIELDS     mushroom_fields
#define B_BEACH               beach
#define B_JUNGLE              jungle
#define B_SPARSE_JUNGLE       sparse_jungle
#define B_DEEP_OCEAN          deep_ocean
#define B_STONY_SHORE         stony_shore
#define B_SNOWY_BEACH         snowy_beach
#define B_BIRCH_FOREST        birch_forest
#define B_DARK_FOREST         dark_forest
#define B_SNOWY_TAIGA         snowy_taiga
#define B_OLD_GROWTH_PINE_TAIGA old_growth_pine_taiga
#define B_WINDSWEPT_FOREST    windswept_forest
#define B_SAVANNA             savanna
#define B_SAVANNA_PLATEAU     savanna_plateau
#define B_BADLANDS            badlands
#define B_WOODED_BADLANDS     wooded_badlands
#define B_WARM_OCEAN          warm_ocean
#define B_LUKEWARM_OCEAN      lukewarm_ocean
#define B_COLD_OCEAN          cold_ocean
#define B_DEEP_LUKEWARM_OCEAN deep_lukewarm_ocean
#define B_DEEP_COLD_OCEAN     deep_cold_ocean
#define B_DEEP_FROZEN_OCEAN   deep_frozen_ocean
#define B_SUNFLOWER_PLAINS    sunflower_plains
#define B_WINDSWEPT_GRAVELLY_HILLS windswept_gravelly_hills
#define B_FLOWER_FOREST       flower_forest
#define B_SNOWY_TAIGA_HILLS   snowy_taiga_hills
#define B_GIANT_SPRUCE_TAIGA  old_growth_spruce_taiga
#define B_GRAVELLY_HILLS      windswept_gravelly_hills
#define B_SHATTERED_SAVANNA   windswept_savanna
#define B_ERODED_BADLANDS     eroded_badlands
#define B_BAMBOO_JUNGLE       bamboo_jungle
#define B_SOUL_SAND_VALLEY    soul_sand_valley
#define B_CRIMSON_FOREST      crimson_forest
#define B_WARPED_FOREST       warped_forest
#define B_BASALT_DELTAS       basalt_deltas
#define B_DRIPSTONE_CAVES     dripstone_caves
#define B_LUSH_CAVES          lush_caves
#define B_DEEP_DARK           deep_dark
#define B_MEADOW              meadow
#define B_GROVE               grove
#define B_SNOWY_SLOPES        snowy_slopes
#define B_FROZEN_PEAKS        jagged_peaks
#define B_JAGGED_PEAKS        jagged_peaks
#define B_STONY_PEAKS         stony_peaks
#define B_CHERRY_GROVE        cherry_grove
#define B_MANGROVE_SWAMP      mangrove_swamp
#define B_PALE_GARDEN         pale_garden

/* Sentinel meaning "all overworld biomes" */
#define ALL_BIOMES -1

/* -----------------------------------------------------------------------
 * Block IDs (internal enum, not Minecraft data-value IDs)
 * --------------------------------------------------------------------- */
enum BlockID {
    /* --- Ores & minerals --- */
    BLK_COAL_ORE = 0,
    BLK_DEEPSLATE_COAL_ORE,
    BLK_IRON_ORE,
    BLK_DEEPSLATE_IRON_ORE,
    BLK_COPPER_ORE,
    BLK_DEEPSLATE_COPPER_ORE,
    BLK_GOLD_ORE,
    BLK_DEEPSLATE_GOLD_ORE,
    BLK_REDSTONE_ORE,
    BLK_DEEPSLATE_REDSTONE_ORE,
    BLK_LAPIS_ORE,
    BLK_DEEPSLATE_LAPIS_ORE,
    BLK_DIAMOND_ORE,
    BLK_DEEPSLATE_DIAMOND_ORE,
    BLK_EMERALD_ORE,
    BLK_DEEPSLATE_EMERALD_ORE,
    BLK_NETHER_GOLD_ORE,
    BLK_NETHER_QUARTZ_ORE,
    BLK_ANCIENT_DEBRIS,
    /* --- Stone variants --- */
    BLK_STONE,
    BLK_GRANITE,
    BLK_DIORITE,
    BLK_ANDESITE,
    BLK_DEEPSLATE,
    BLK_TUFF,
    BLK_CALCITE,
    BLK_GRAVEL,
    BLK_SAND,
    BLK_RED_SAND,
    BLK_SANDSTONE,
    BLK_RED_SANDSTONE,
    BLK_CLAY,
    BLK_DIRT,
    BLK_COARSE_DIRT,
    BLK_ROOTED_DIRT,
    BLK_PODZOL,
    BLK_MYCELIUM,
    BLK_GRASS_BLOCK,
    BLK_SNOW_BLOCK,
    BLK_ICE,
    BLK_PACKED_ICE,
    BLK_BLUE_ICE,
    BLK_OBSIDIAN,
    BLK_CRYING_OBSIDIAN,
    BLK_MAGMA_BLOCK,
    BLK_NETHERRACK,
    BLK_SOUL_SAND,
    BLK_SOUL_SOIL,
    BLK_CRIMSON_NYLIUM,
    BLK_WARPED_NYLIUM,
    BLK_BLACKSTONE,
    BLK_BASALT,
    BLK_SMOOTH_BASALT,
    BLK_GILDED_BLACKSTONE,
    BLK_END_STONE,
    /* --- Vegetation --- */
    BLK_OAK_LOG,
    BLK_SPRUCE_LOG,
    BLK_BIRCH_LOG,
    BLK_JUNGLE_LOG,
    BLK_ACACIA_LOG,
    BLK_DARK_OAK_LOG,
    BLK_MANGROVE_LOG,
    BLK_CHERRY_LOG,
    BLK_PALE_OAK_LOG,
    BLK_OAK_LEAVES,
    BLK_SPRUCE_LEAVES,
    BLK_BIRCH_LEAVES,
    BLK_JUNGLE_LEAVES,
    BLK_ACACIA_LEAVES,
    BLK_DARK_OAK_LEAVES,
    BLK_MANGROVE_LEAVES,
    BLK_CHERRY_LEAVES,
    BLK_PALE_OAK_LEAVES,
    BLK_AZALEA,
    BLK_FLOWERING_AZALEA,
    BLK_AZALEA_LEAVES,
    BLK_FLOWERING_AZALEA_LEAVES,
    BLK_MOSS_BLOCK,
    BLK_MOSS_CARPET,
    BLK_SCULK,
    BLK_SCULK_VEIN,
    BLK_SCULK_CATALYST,
    BLK_SCULK_SENSOR,
    BLK_SCULK_SHRIEKER,
    BLK_BAMBOO,
    BLK_CACTUS,
    BLK_SUGAR_CANE,
    BLK_PUMPKIN,
    BLK_MELON,
    BLK_DEAD_BUSH,
    BLK_FERN,
    BLK_LARGE_FERN,
    BLK_GRASS,
    BLK_TALL_GRASS,
    BLK_DANDELION,
    BLK_POPPY,
    BLK_BLUE_ORCHID,
    BLK_ALLIUM,
    BLK_AZURE_BLUET,
    BLK_RED_TULIP,
    BLK_ORANGE_TULIP,
    BLK_WHITE_TULIP,
    BLK_PINK_TULIP,
    BLK_OXEYE_DAISY,
    BLK_CORNFLOWER,
    BLK_LILY_OF_THE_VALLEY,
    BLK_WITHER_ROSE,
    BLK_SUNFLOWER,
    BLK_LILAC,
    BLK_ROSE_BUSH,
    BLK_PEONY,
    BLK_PINK_PETALS,
    BLK_WILDFLOWERS,
    BLK_TORCHFLOWER,
    BLK_PITCHER_PLANT,
    BLK_PITCHER_CROP,
    BLK_SPORE_BLOSSOM,
    BLK_GLOW_LICHEN,
    BLK_HANGING_ROOTS,
    BLK_BIG_DRIPLEAF,
    BLK_SMALL_DRIPLEAF,
    BLK_SEAGRASS,
    BLK_TALL_SEAGRASS,
    BLK_KELP,
    BLK_SEA_PICKLE,
    BLK_CORAL_BLOCK,
    BLK_DEAD_CORAL_BLOCK,
    BLK_CORAL,
    BLK_CORAL_FAN,
    BLK_SPONGE,
    BLK_WET_SPONGE,
    BLK_LILY_PAD,
    BLK_WATERLILY,
    BLK_BROWN_MUSHROOM,
    BLK_RED_MUSHROOM,
    BLK_BROWN_MUSHROOM_BLOCK,
    BLK_RED_MUSHROOM_BLOCK,
    BLK_HUGE_BROWN_MUSHROOM,
    BLK_HUGE_RED_MUSHROOM,
    BLK_NETHER_WART,
    BLK_WARPED_FUNGUS,
    BLK_CRIMSON_FUNGUS,
    BLK_WARPED_ROOTS,
    BLK_CRIMSON_ROOTS,
    BLK_NETHER_SPROUTS,
    BLK_SHROOMLIGHT,
    BLK_WEEPING_VINES,
    BLK_TWISTING_VINES,
    BLK_CHORUS_PLANT,
    BLK_CHORUS_FLOWER,
    BLK_SWEET_BERRY_BUSH,
    BLK_CAVE_VINES,
    BLK_GLOW_BERRIES,
    /* --- Fluids --- */
    BLK_WATER,
    BLK_LAVA,
    /* --- Structures / generated blocks --- */
    BLK_CHEST,
    BLK_SPAWNER,
    BLK_INFESTED_STONE,
    BLK_INFESTED_DEEPSLATE,
    BLK_INFESTED_COBBLESTONE,
    BLK_INFESTED_STONE_BRICKS,
    BLK_INFESTED_MOSSY_STONE_BRICKS,
    BLK_INFESTED_CRACKED_STONE_BRICKS,
    BLK_INFESTED_CHISELED_STONE_BRICKS,
    BLK_CHISELED_STONE_BRICKS,
    BLK_CRACKED_STONE_BRICKS,
    BLK_MOSSY_STONE_BRICKS,
    BLK_STONE_BRICKS,
    BLK_MOSSY_COBBLESTONE,
    BLK_COBBLESTONE,
    BLK_OAK_PLANKS,
    BLK_BOOKSHELF,
    BLK_BONE_BLOCK,
    BLK_NETHER_BRICK,
    BLK_RED_NETHER_BRICK,
    BLK_PURPUR_BLOCK,
    BLK_PURPUR_PILLAR,
    BLK_END_STONE_BRICKS,
    BLK_LODESTONE,
    BLK_AMETHYST_BLOCK,
    BLK_BUDDING_AMETHYST,
    BLK_AMETHYST_CLUSTER,
    BLK_LARGE_AMETHYST_BUD,
    BLK_MEDIUM_AMETHYST_BUD,
    BLK_SMALL_AMETHYST_BUD,
    BLK_POINTED_DRIPSTONE,
    BLK_DRIPSTONE_BLOCK,
    BLK_MUDDY_MANGROVE_ROOTS,
    BLK_MUD,
    BLK_PACKED_MUD,
    BLK_MUD_BRICKS,
    BLK_SUSPICIOUS_SAND,
    BLK_SUSPICIOUS_GRAVEL,
    BLK_TRIAL_SPAWNER,
    BLK_VAULT,
    BLK_OMINOUS_VAULT,
    BLK_OMINOUS_TRIAL_SPAWNER,
    BLK_CRAFTER,
    BLK_HEAVY_CORE,
    BLK_COPPER_BLOCK,
    BLK_EXPOSED_COPPER,
    BLK_WEATHERED_COPPER,
    BLK_OXIDIZED_COPPER,
    BLK_CUT_COPPER,
    BLK_WAXED_COPPER_BLOCK,
    /* --- Biome-specific blocks --- */
    BLK_RED_SAND_BIOME,
    BLK_TERRACOTTA,
    BLK_WHITE_TERRACOTTA,
    BLK_ORANGE_TERRACOTTA,
    BLK_MAGENTA_TERRACOTTA,
    BLK_LIGHT_BLUE_TERRACOTTA,
    BLK_YELLOW_TERRACOTTA,
    BLK_LIME_TERRACOTTA,
    BLK_PINK_TERRACOTTA,
    BLK_GRAY_TERRACOTTA,
    BLK_LIGHT_GRAY_TERRACOTTA,
    BLK_CYAN_TERRACOTTA,
    BLK_PURPLE_TERRACOTTA,
    BLK_BLUE_TERRACOTTA,
    BLK_BROWN_TERRACOTTA,
    BLK_GREEN_TERRACOTTA,
    BLK_RED_TERRACOTTA,
    BLK_BLACK_TERRACOTTA,
    NUM_BLOCKS
};

/* -----------------------------------------------------------------------
 * Dimension constants (0=overworld, 1=nether, 2=end)
 * --------------------------------------------------------------------- */
#define DIM_OW  0
#define DIM_NET 1
#define DIM_END 2

/* -----------------------------------------------------------------------
 * Block table
 * We store -1 in biomes[0] and n_biomes=0 to mean "all overworld biomes".
 * --------------------------------------------------------------------- */
static const BlockInfo block_table[] = {
    /* id, name, display_name, biomes[], n_biomes, y_min, y_max, dim */

    /* Coal ore */
    { BLK_COAL_ORE,            "coal_ore",            "Coal Ore",            {ALL_BIOMES}, 0, 0,  192, DIM_OW },
    { BLK_DEEPSLATE_COAL_ORE,  "deepslate_coal_ore",  "Deepslate Coal Ore",  {ALL_BIOMES}, 0, -64,  0, DIM_OW },
    /* Iron ore */
    { BLK_IRON_ORE,            "iron_ore",            "Iron Ore",            {ALL_BIOMES}, 0, -24, 256, DIM_OW },
    { BLK_DEEPSLATE_IRON_ORE,  "deepslate_iron_ore",  "Deepslate Iron Ore",  {ALL_BIOMES}, 0, -64,  0, DIM_OW },
    /* Copper ore */
    { BLK_COPPER_ORE,          "copper_ore",          "Copper Ore",          {ALL_BIOMES}, 0,  -16, 112, DIM_OW },
    { BLK_DEEPSLATE_COPPER_ORE,"deepslate_copper_ore","Deepslate Copper Ore",{ALL_BIOMES}, 0, -64,   0, DIM_OW },
    /* Gold ore */
    { BLK_GOLD_ORE,            "gold_ore",            "Gold Ore",            {ALL_BIOMES}, 0, -64,  32, DIM_OW },
    { BLK_DEEPSLATE_GOLD_ORE,  "deepslate_gold_ore",  "Deepslate Gold Ore",  {ALL_BIOMES}, 0, -64,   0, DIM_OW },
    /* Redstone ore */
    { BLK_REDSTONE_ORE,        "redstone_ore",        "Redstone Ore",        {ALL_BIOMES}, 0, -64,  16, DIM_OW },
    { BLK_DEEPSLATE_REDSTONE_ORE,"deepslate_redstone_ore","Deepslate Redstone Ore",{ALL_BIOMES},0,-64,0,DIM_OW},
    /* Lapis ore */
    { BLK_LAPIS_ORE,           "lapis_ore",           "Lapis Ore",           {ALL_BIOMES}, 0, -64,  64, DIM_OW },
    { BLK_DEEPSLATE_LAPIS_ORE, "deepslate_lapis_ore", "Deepslate Lapis Ore", {ALL_BIOMES}, 0, -64,   0, DIM_OW },
    /* Diamond ore */
    { BLK_DIAMOND_ORE,         "diamond_ore",         "Diamond Ore",         {ALL_BIOMES}, 0,  -64, 16, DIM_OW },
    { BLK_DEEPSLATE_DIAMOND_ORE,"deepslate_diamond_ore","Deepslate Diamond Ore",{ALL_BIOMES},0,-64,0,DIM_OW},
    /* Emerald ore — mountains biome only */
    { BLK_EMERALD_ORE,         "emerald_ore",         "Emerald Ore",
      { windswept_hills, windswept_forest, windswept_gravelly_hills, meadow, grove,
        snowy_slopes, jagged_peaks, frozen_peaks, stony_peaks }, 9, -16, 320, DIM_OW },
    { BLK_DEEPSLATE_EMERALD_ORE,"deepslate_emerald_ore","Deepslate Emerald Ore",
      { windswept_hills, windswept_forest, windswept_gravelly_hills, meadow, grove,
        snowy_slopes, jagged_peaks, frozen_peaks, stony_peaks }, 9, -64, 0, DIM_OW },
    /* Nether gold ore */
    { BLK_NETHER_GOLD_ORE,     "nether_gold_ore",     "Nether Gold Ore",
      { nether_wastes, soul_sand_valley, crimson_forest, warped_forest, basalt_deltas }, 5, 10, 117, DIM_NET },
    /* Nether quartz ore */
    { BLK_NETHER_QUARTZ_ORE,   "nether_quartz_ore",   "Nether Quartz Ore",
      { nether_wastes, soul_sand_valley, crimson_forest, warped_forest, basalt_deltas }, 5, 10, 117, DIM_NET },
    /* Ancient debris */
    { BLK_ANCIENT_DEBRIS,      "ancient_debris",      "Ancient Debris",
      { nether_wastes, soul_sand_valley, crimson_forest, warped_forest, basalt_deltas }, 5, 8, 119, DIM_NET },

    /* Stone & geological */
    { BLK_STONE,          "stone",          "Stone",          {ALL_BIOMES}, 0,  -64, 320, DIM_OW },
    { BLK_GRANITE,        "granite",        "Granite",        {ALL_BIOMES}, 0,  -64, 320, DIM_OW },
    { BLK_DIORITE,        "diorite",        "Diorite",        {ALL_BIOMES}, 0,  -64, 320, DIM_OW },
    { BLK_ANDESITE,       "andesite",       "Andesite",       {ALL_BIOMES}, 0,  -64, 320, DIM_OW },
    { BLK_DEEPSLATE,      "deepslate",      "Deepslate",      {ALL_BIOMES}, 0,  -64,   0, DIM_OW },
    { BLK_TUFF,           "tuff",           "Tuff",           {ALL_BIOMES}, 0,  -64,   0, DIM_OW },
    { BLK_CALCITE,        "calcite",        "Calcite",        {ALL_BIOMES}, 0,    0,  80, DIM_OW },
    { BLK_GRAVEL,         "gravel",         "Gravel",         {ALL_BIOMES}, 0,  -64, 320, DIM_OW },
    { BLK_SAND,           "sand",           "Sand",
      { beach, desert, river, warm_ocean, lukewarm_ocean, deep_lukewarm_ocean }, 6, 60, 65, DIM_OW },
    { BLK_RED_SAND,       "red_sand",       "Red Sand",
      { badlands, eroded_badlands, wooded_badlands }, 3, 60, 80, DIM_OW },
    { BLK_SANDSTONE,      "sandstone",      "Sandstone",
      { beach, desert, badlands }, 3, 55, 70, DIM_OW },
    { BLK_RED_SANDSTONE,  "red_sandstone",  "Red Sandstone",
      { badlands, eroded_badlands, wooded_badlands }, 3, 55, 80, DIM_OW },
    { BLK_CLAY,           "clay",           "Clay",           {ALL_BIOMES}, 0,  59,  68, DIM_OW },
    { BLK_DIRT,           "dirt",           "Dirt",           {ALL_BIOMES}, 0,  58, 320, DIM_OW },
    { BLK_COARSE_DIRT,    "coarse_dirt",    "Coarse Dirt",
      { windswept_hills, windswept_forest, windswept_gravelly_hills,
        old_growth_pine_taiga, old_growth_spruce_taiga, wooded_badlands }, 6, 58, 200, DIM_OW },
    { BLK_ROOTED_DIRT,    "rooted_dirt",    "Rooted Dirt",    {ALL_BIOMES}, 0,  50, 100, DIM_OW },
    { BLK_PODZOL,         "podzol",         "Podzol",
      { old_growth_pine_taiga, old_growth_spruce_taiga, bamboo_jungle, jungle }, 4, 58, 200, DIM_OW },
    { BLK_MYCELIUM,       "mycelium",       "Mycelium",
      { mushroom_fields }, 1, 62, 70, DIM_OW },
    { BLK_GRASS_BLOCK,    "grass_block",    "Grass Block",    {ALL_BIOMES}, 0,  60, 320, DIM_OW },
    { BLK_SNOW_BLOCK,     "snow_block",     "Snow Block",
      { snowy_plains, snowy_taiga, snowy_beach, snowy_slopes,
        grove, jagged_peaks, frozen_peaks }, 7, 60, 320, DIM_OW },
    { BLK_ICE,            "ice",            "Ice",
      { frozen_ocean, frozen_river, snowy_plains, snowy_taiga,
        snowy_beach, deep_frozen_ocean }, 6, 62, 65, DIM_OW },
    { BLK_PACKED_ICE,     "packed_ice",     "Packed Ice",
      { frozen_ocean, deep_frozen_ocean, snowy_slopes, grove,
        jagged_peaks, frozen_peaks }, 6, 60, 320, DIM_OW },
    { BLK_BLUE_ICE,       "blue_ice",       "Blue Ice",
      { frozen_ocean, deep_frozen_ocean, frozen_peaks }, 3, 55, 70, DIM_OW },
    { BLK_OBSIDIAN,       "obsidian",       "Obsidian",       {ALL_BIOMES}, 0,  -64, 320, DIM_OW },
    { BLK_CRYING_OBSIDIAN,"crying_obsidian","Crying Obsidian",{ALL_BIOMES}, 0,  -64, 128, DIM_OW },
    { BLK_MAGMA_BLOCK,    "magma_block",    "Magma Block",    {ALL_BIOMES}, 0,   25,  55, DIM_OW },
    { BLK_NETHERRACK,     "netherrack",     "Netherrack",
      { nether_wastes, soul_sand_valley, crimson_forest, warped_forest, basalt_deltas }, 5, 0, 128, DIM_NET },
    { BLK_SOUL_SAND,      "soul_sand",      "Soul Sand",
      { soul_sand_valley, nether_wastes }, 2, 30, 90, DIM_NET },
    { BLK_SOUL_SOIL,      "soul_soil",      "Soul Soil",
      { soul_sand_valley }, 1, 30, 90, DIM_NET },
    { BLK_CRIMSON_NYLIUM, "crimson_nylium", "Crimson Nylium",
      { crimson_forest }, 1, 0, 120, DIM_NET },
    { BLK_WARPED_NYLIUM,  "warped_nylium",  "Warped Nylium",
      { warped_forest }, 1, 0, 120, DIM_NET },
    { BLK_BLACKSTONE,     "blackstone",     "Blackstone",
      { basalt_deltas, nether_wastes }, 2, 5, 100, DIM_NET },
    { BLK_BASALT,         "basalt",         "Basalt",
      { basalt_deltas }, 1, 0, 128, DIM_NET },
    { BLK_SMOOTH_BASALT,  "smooth_basalt",  "Smooth Basalt",  {ALL_BIOMES}, 0, -64, 100, DIM_OW },
    { BLK_GILDED_BLACKSTONE,"gilded_blackstone","Gilded Blackstone",
      { basalt_deltas, nether_wastes }, 2, 5, 100, DIM_NET },
    { BLK_END_STONE,      "end_stone",      "End Stone",      {ALL_BIOMES}, 0,  40, 100, DIM_END },

    /* Wood / tree logs */
    { BLK_OAK_LOG,        "oak_log",        "Oak Log",
      { forest, flower_forest, birch_forest, dark_forest, wooded_badlands,
        plains, sunflower_plains, meadow, swamp, windswept_forest, windswept_hills,
        windswept_gravelly_hills }, 12, 62, 200, DIM_OW },
    { BLK_SPRUCE_LOG,     "spruce_log",     "Spruce Log",
      { taiga, snowy_taiga, old_growth_pine_taiga, old_growth_spruce_taiga,
        grove, snowy_slopes, windswept_forest }, 7, 62, 200, DIM_OW },
    { BLK_BIRCH_LOG,      "birch_log",      "Birch Log",
      { birch_forest, forest, flower_forest }, 3, 62, 200, DIM_OW },
    { BLK_JUNGLE_LOG,     "jungle_log",     "Jungle Log",
      { jungle, sparse_jungle, bamboo_jungle }, 3, 62, 200, DIM_OW },
    { BLK_ACACIA_LOG,     "acacia_log",     "Acacia Log",
      { savanna, savanna_plateau, windswept_savanna }, 3, 62, 200, DIM_OW },
    { BLK_DARK_OAK_LOG,   "dark_oak_log",   "Dark Oak Log",
      { dark_forest }, 1, 62, 200, DIM_OW },
    { BLK_MANGROVE_LOG,   "mangrove_log",   "Mangrove Log",
      { mangrove_swamp }, 1, 60, 200, DIM_OW },
    { BLK_CHERRY_LOG,     "cherry_log",     "Cherry Log",
      { cherry_grove }, 1, 62, 200, DIM_OW },
    { BLK_PALE_OAK_LOG,   "pale_oak_log",   "Pale Oak Log",
      { pale_garden }, 1, 62, 200, DIM_OW },

    /* Leaves */
    { BLK_OAK_LEAVES,     "oak_leaves",     "Oak Leaves",     {ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_SPRUCE_LEAVES,  "spruce_leaves",  "Spruce Leaves",  {ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_BIRCH_LEAVES,   "birch_leaves",   "Birch Leaves",   {ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_JUNGLE_LEAVES,  "jungle_leaves",  "Jungle Leaves",  {ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_ACACIA_LEAVES,  "acacia_leaves",  "Acacia Leaves",  {ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_DARK_OAK_LEAVES,"dark_oak_leaves","Dark Oak Leaves",{ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_MANGROVE_LEAVES,"mangrove_leaves","Mangrove Leaves",{ALL_BIOMES}, 0, 60, 200, DIM_OW },
    { BLK_CHERRY_LEAVES,  "cherry_leaves",  "Cherry Leaves",  {ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_PALE_OAK_LEAVES,"pale_oak_leaves","Pale Oak Leaves",{ALL_BIOMES}, 0, 62, 200, DIM_OW },

    /* Azalea */
    { BLK_AZALEA,              "azalea",              "Azalea",
      { lush_caves }, 1, -64, 64, DIM_OW },
    { BLK_FLOWERING_AZALEA,    "flowering_azalea",    "Flowering Azalea",
      { lush_caves }, 1, -64, 64, DIM_OW },
    { BLK_AZALEA_LEAVES,       "azalea_leaves",       "Azalea Leaves",
      { lush_caves }, 1, -64, 64, DIM_OW },
    { BLK_FLOWERING_AZALEA_LEAVES,"flowering_azalea_leaves","Flowering Azalea Leaves",
      { lush_caves }, 1, -64, 64, DIM_OW },

    /* Moss */
    { BLK_MOSS_BLOCK,     "moss_block",     "Moss Block",     {ALL_BIOMES}, 0, -64,  64, DIM_OW },
    { BLK_MOSS_CARPET,    "moss_carpet",    "Moss Carpet",    {ALL_BIOMES}, 0, -64,  64, DIM_OW },

    /* Sculk */
    { BLK_SCULK,          "sculk",          "Sculk",
      { deep_dark }, 1, -64, -20, DIM_OW },
    { BLK_SCULK_VEIN,     "sculk_vein",     "Sculk Vein",
      { deep_dark }, 1, -64, -20, DIM_OW },
    { BLK_SCULK_CATALYST, "sculk_catalyst", "Sculk Catalyst",
      { deep_dark }, 1, -64, -20, DIM_OW },
    { BLK_SCULK_SENSOR,   "sculk_sensor",   "Sculk Sensor",
      { deep_dark }, 1, -64, -20, DIM_OW },
    { BLK_SCULK_SHRIEKER, "sculk_shrieker", "Sculk Shrieker",
      { deep_dark }, 1, -64, -20, DIM_OW },

    /* Bamboo */
    { BLK_BAMBOO,         "bamboo",         "Bamboo",
      { bamboo_jungle }, 1, 62, 200, DIM_OW },
    /* Cactus */
    { BLK_CACTUS,         "cactus",         "Cactus",
      { desert }, 1, 62, 80, DIM_OW },
    /* Sugar cane */
    { BLK_SUGAR_CANE,     "sugar_cane",     "Sugar Cane",     {ALL_BIOMES}, 0, 61, 70, DIM_OW },
    /* Pumpkin */
    { BLK_PUMPKIN,        "pumpkin",        "Pumpkin",
      { plains, sunflower_plains, forest, flower_forest, taiga, meadow }, 6, 62, 70, DIM_OW },
    /* Melon */
    { BLK_MELON,          "melon",          "Melon",
      { jungle, sparse_jungle, bamboo_jungle }, 3, 62, 80, DIM_OW },
    /* Dead bush */
    { BLK_DEAD_BUSH,      "dead_bush",      "Dead Bush",
      { desert, badlands, eroded_badlands, wooded_badlands }, 4, 62, 80, DIM_OW },

    /* Small vegetation */
    { BLK_FERN,           "fern",           "Fern",
      { taiga, snowy_taiga, old_growth_pine_taiga, old_growth_spruce_taiga,
        jungle, bamboo_jungle }, 6, 62, 200, DIM_OW },
    { BLK_LARGE_FERN,     "large_fern",     "Large Fern",
      { taiga, snowy_taiga, old_growth_pine_taiga, old_growth_spruce_taiga }, 4, 62, 200, DIM_OW },
    { BLK_GRASS,          "short_grass",    "Short Grass",    {ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_TALL_GRASS,     "tall_grass",     "Tall Grass",     {ALL_BIOMES}, 0, 62, 200, DIM_OW },

    /* Flowers */
    { BLK_DANDELION,      "dandelion",      "Dandelion",      {ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_POPPY,          "poppy",          "Poppy",          {ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_BLUE_ORCHID,    "blue_orchid",    "Blue Orchid",
      { swamp, mangrove_swamp }, 2, 62, 80, DIM_OW },
    { BLK_ALLIUM,         "allium",         "Allium",
      { flower_forest, meadow }, 2, 62, 200, DIM_OW },
    { BLK_AZURE_BLUET,    "azure_bluet",    "Azure Bluet",
      { plains, sunflower_plains, flower_forest, meadow }, 4, 62, 200, DIM_OW },
    { BLK_RED_TULIP,      "red_tulip",      "Red Tulip",
      { plains, sunflower_plains, flower_forest, meadow }, 4, 62, 200, DIM_OW },
    { BLK_ORANGE_TULIP,   "orange_tulip",   "Orange Tulip",
      { plains, sunflower_plains, flower_forest, meadow }, 4, 62, 200, DIM_OW },
    { BLK_WHITE_TULIP,    "white_tulip",    "White Tulip",
      { plains, sunflower_plains, flower_forest, meadow }, 4, 62, 200, DIM_OW },
    { BLK_PINK_TULIP,     "pink_tulip",     "Pink Tulip",
      { plains, sunflower_plains, flower_forest, meadow }, 4, 62, 200, DIM_OW },
    { BLK_OXEYE_DAISY,    "oxeye_daisy",    "Oxeye Daisy",
      { plains, sunflower_plains, flower_forest, meadow }, 4, 62, 200, DIM_OW },
    { BLK_CORNFLOWER,     "cornflower",     "Cornflower",
      { plains, sunflower_plains, flower_forest, meadow }, 4, 62, 200, DIM_OW },
    { BLK_LILY_OF_THE_VALLEY,"lily_of_the_valley","Lily of the Valley",
      { flower_forest, forest }, 2, 62, 200, DIM_OW },
    { BLK_WITHER_ROSE,    "wither_rose",    "Wither Rose",
      { soul_sand_valley }, 1, 30, 100, DIM_NET },
    { BLK_SUNFLOWER,      "sunflower",      "Sunflower",
      { sunflower_plains }, 1, 62, 200, DIM_OW },
    { BLK_LILAC,          "lilac",          "Lilac",
      { forest, flower_forest, birch_forest }, 3, 62, 200, DIM_OW },
    { BLK_ROSE_BUSH,      "rose_bush",      "Rose Bush",
      { forest, flower_forest, birch_forest }, 3, 62, 200, DIM_OW },
    { BLK_PEONY,          "peony",          "Peony",
      { forest, flower_forest, birch_forest }, 3, 62, 200, DIM_OW },
    { BLK_PINK_PETALS,    "pink_petals",    "Pink Petals",
      { cherry_grove }, 1, 62, 200, DIM_OW },
    { BLK_WILDFLOWERS,    "wildflowers",    "Wildflowers",
      { meadow, plains, flower_forest }, 3, 62, 200, DIM_OW },
    { BLK_TORCHFLOWER,    "torchflower",    "Torchflower",    {ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_PITCHER_PLANT,  "pitcher_plant",  "Pitcher Plant",  {ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_PITCHER_CROP,   "pitcher_crop",   "Pitcher Crop",   {ALL_BIOMES}, 0, 62, 200, DIM_OW },
    { BLK_SPORE_BLOSSOM,  "spore_blossom",  "Spore Blossom",
      { lush_caves }, 1, -64, 64, DIM_OW },
    { BLK_GLOW_LICHEN,    "glow_lichen",    "Glow Lichen",    {ALL_BIOMES}, 0, -64, 200, DIM_OW },
    { BLK_HANGING_ROOTS,  "hanging_roots",  "Hanging Roots",  {ALL_BIOMES}, 0, -64, 200, DIM_OW },
    { BLK_BIG_DRIPLEAF,   "big_dripleaf",   "Big Dripleaf",
      { lush_caves, swamp, mangrove_swamp }, 3, -64, 64, DIM_OW },
    { BLK_SMALL_DRIPLEAF, "small_dripleaf", "Small Dripleaf",
      { lush_caves, swamp }, 2, -64, 64, DIM_OW },

    /* Ocean / water blocks */
    { BLK_SEAGRASS,       "seagrass",       "Seagrass",
      { ocean, deep_ocean, warm_ocean, lukewarm_ocean, cold_ocean,
        deep_lukewarm_ocean, deep_cold_ocean, frozen_ocean, deep_frozen_ocean }, 9, 46, 63, DIM_OW },
    { BLK_TALL_SEAGRASS,  "tall_seagrass",  "Tall Seagrass",
      { ocean, deep_ocean, warm_ocean, lukewarm_ocean, cold_ocean,
        deep_lukewarm_ocean, deep_cold_ocean }, 7, 46, 63, DIM_OW },
    { BLK_KELP,           "kelp",           "Kelp",
      { ocean, deep_ocean, lukewarm_ocean, cold_ocean,
        deep_lukewarm_ocean, deep_cold_ocean }, 6, 25, 63, DIM_OW },
    { BLK_SEA_PICKLE,     "sea_pickle",     "Sea Pickle",
      { warm_ocean }, 1, 46, 63, DIM_OW },
    { BLK_CORAL_BLOCK,    "coral_block",    "Coral Block",
      { warm_ocean }, 1, 46, 63, DIM_OW },
    { BLK_DEAD_CORAL_BLOCK,"dead_coral_block","Dead Coral Block",
      { ocean, lukewarm_ocean, deep_lukewarm_ocean }, 3, 46, 63, DIM_OW },
    { BLK_CORAL,          "coral",          "Coral",
      { warm_ocean }, 1, 46, 63, DIM_OW },
    { BLK_CORAL_FAN,      "coral_fan",      "Coral Fan",
      { warm_ocean }, 1, 46, 63, DIM_OW },
    { BLK_SPONGE,         "sponge",         "Sponge",
      { deep_ocean, deep_lukewarm_ocean, deep_cold_ocean, deep_frozen_ocean }, 4, 30, 50, DIM_OW },
    { BLK_WET_SPONGE,     "wet_sponge",     "Wet Sponge",
      { deep_ocean, deep_lukewarm_ocean, deep_cold_ocean, deep_frozen_ocean }, 4, 30, 50, DIM_OW },
    { BLK_LILY_PAD,       "lily_pad",       "Lily Pad",
      { swamp, mangrove_swamp }, 2, 62, 64, DIM_OW },
    { BLK_WATERLILY,      "waterlily",      "Water Lily",
      { swamp, mangrove_swamp }, 2, 62, 64, DIM_OW },

    /* Mushrooms */
    { BLK_BROWN_MUSHROOM, "brown_mushroom", "Brown Mushroom",
      { dark_forest, swamp, mushroom_fields, old_growth_pine_taiga,
        old_growth_spruce_taiga }, 5, 0, 100, DIM_OW },
    { BLK_RED_MUSHROOM,   "red_mushroom",   "Red Mushroom",
      { dark_forest, swamp, mushroom_fields, old_growth_pine_taiga,
        old_growth_spruce_taiga }, 5, 0, 100, DIM_OW },
    { BLK_BROWN_MUSHROOM_BLOCK,"brown_mushroom_block","Brown Mushroom Block",
      { dark_forest, mushroom_fields }, 2, 62, 200, DIM_OW },
    { BLK_RED_MUSHROOM_BLOCK,"red_mushroom_block","Red Mushroom Block",
      { dark_forest, mushroom_fields }, 2, 62, 200, DIM_OW },
    { BLK_HUGE_BROWN_MUSHROOM,"huge_brown_mushroom","Huge Brown Mushroom",
      { dark_forest, mushroom_fields }, 2, 62, 200, DIM_OW },
    { BLK_HUGE_RED_MUSHROOM,"huge_red_mushroom","Huge Red Mushroom",
      { dark_forest, mushroom_fields }, 2, 62, 200, DIM_OW },

    /* Nether vegetation */
    { BLK_NETHER_WART,    "nether_wart",    "Nether Wart",
      { nether_wastes }, 1, 30, 100, DIM_NET },
    { BLK_WARPED_FUNGUS,  "warped_fungus",  "Warped Fungus",
      { warped_forest }, 1, 0, 120, DIM_NET },
    { BLK_CRIMSON_FUNGUS, "crimson_fungus", "Crimson Fungus",
      { crimson_forest }, 1, 0, 120, DIM_NET },
    { BLK_WARPED_ROOTS,   "warped_roots",   "Warped Roots",
      { warped_forest }, 1, 0, 120, DIM_NET },
    { BLK_CRIMSON_ROOTS,  "crimson_roots",  "Crimson Roots",
      { crimson_forest }, 1, 0, 120, DIM_NET },
    { BLK_NETHER_SPROUTS, "nether_sprouts", "Nether Sprouts",
      { warped_forest }, 1, 0, 120, DIM_NET },
    { BLK_SHROOMLIGHT,    "shroomlight",    "Shroomlight",
      { crimson_forest, warped_forest }, 2, 0, 120, DIM_NET },
    { BLK_WEEPING_VINES,  "weeping_vines",  "Weeping Vines",
      { crimson_forest, nether_wastes }, 2, 0, 120, DIM_NET },
    { BLK_TWISTING_VINES, "twisting_vines", "Twisting Vines",
      { warped_forest }, 1, 0, 120, DIM_NET },

    /* End vegetation */
    { BLK_CHORUS_PLANT,   "chorus_plant",   "Chorus Plant",   {ALL_BIOMES}, 0, 50, 120, DIM_END },
    { BLK_CHORUS_FLOWER,  "chorus_flower",  "Chorus Flower",  {ALL_BIOMES}, 0, 50, 120, DIM_END },

    /* Berry bush */
    { BLK_SWEET_BERRY_BUSH,"sweet_berry_bush","Sweet Berry Bush",
      { taiga, snowy_taiga, old_growth_pine_taiga,
        old_growth_spruce_taiga }, 4, 62, 200, DIM_OW },
    { BLK_CAVE_VINES,     "cave_vines",     "Cave Vines",
      { lush_caves }, 1, -64, 64, DIM_OW },
    { BLK_GLOW_BERRIES,   "glow_berries",   "Glow Berries",
      { lush_caves }, 1, -64, 64, DIM_OW },

    /* Fluids */
    { BLK_WATER,          "water",          "Water",          {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_LAVA,           "lava",           "Lava",           {ALL_BIOMES}, 0, -64,  10, DIM_OW },

    /* Structures */
    { BLK_CHEST,               "chest",               "Chest",               {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_SPAWNER,             "spawner",             "Monster Spawner",     {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_INFESTED_STONE,      "infested_stone",      "Infested Stone",
      { windswept_hills, windswept_forest, windswept_gravelly_hills }, 3, -64, 320, DIM_OW },
    { BLK_INFESTED_DEEPSLATE,  "infested_deepslate",  "Infested Deepslate",  {ALL_BIOMES}, 0, -64, 0, DIM_OW },
    { BLK_INFESTED_COBBLESTONE,"infested_cobblestone","Infested Cobblestone",{ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_INFESTED_STONE_BRICKS,"infested_stone_bricks","Infested Stone Bricks",{ALL_BIOMES},0,-64,320,DIM_OW },
    { BLK_INFESTED_MOSSY_STONE_BRICKS,"infested_mossy_stone_bricks","Infested Mossy Stone Bricks",{ALL_BIOMES},0,-64,320,DIM_OW },
    { BLK_INFESTED_CRACKED_STONE_BRICKS,"infested_cracked_stone_bricks","Infested Cracked Stone Bricks",{ALL_BIOMES},0,-64,320,DIM_OW },
    { BLK_INFESTED_CHISELED_STONE_BRICKS,"infested_chiseled_stone_bricks","Infested Chiseled Stone Bricks",{ALL_BIOMES},0,-64,320,DIM_OW },
    { BLK_CHISELED_STONE_BRICKS,"chiseled_stone_bricks","Chiseled Stone Bricks",{ALL_BIOMES},0,-64,320,DIM_OW },
    { BLK_CRACKED_STONE_BRICKS,"cracked_stone_bricks","Cracked Stone Bricks",{ALL_BIOMES},0,-64,320,DIM_OW },
    { BLK_MOSSY_STONE_BRICKS,  "mossy_stone_bricks",  "Mossy Stone Bricks",  {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_STONE_BRICKS,        "stone_bricks",        "Stone Bricks",        {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_MOSSY_COBBLESTONE,   "mossy_cobblestone",   "Mossy Cobblestone",   {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_COBBLESTONE,         "cobblestone",         "Cobblestone",         {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_OAK_PLANKS,          "oak_planks",          "Oak Planks",          {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_BOOKSHELF,           "bookshelf",           "Bookshelf",           {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_BONE_BLOCK,          "bone_block",          "Bone Block",
      { desert, soul_sand_valley }, 2, 0, 320, DIM_OW },
    { BLK_NETHER_BRICK,        "nether_brick",        "Nether Brick",
      { nether_wastes }, 1, 0, 128, DIM_NET },
    { BLK_RED_NETHER_BRICK,    "red_nether_brick",    "Red Nether Brick",
      { nether_wastes }, 1, 0, 128, DIM_NET },
    { BLK_PURPUR_BLOCK,        "purpur_block",        "Purpur Block",        {ALL_BIOMES}, 0, 40, 100, DIM_END },
    { BLK_PURPUR_PILLAR,       "purpur_pillar",       "Purpur Pillar",       {ALL_BIOMES}, 0, 40, 100, DIM_END },
    { BLK_END_STONE_BRICKS,    "end_stone_bricks",    "End Stone Bricks",    {ALL_BIOMES}, 0, 40, 100, DIM_END },
    { BLK_LODESTONE,           "lodestone",           "Lodestone",           {ALL_BIOMES}, 0, 0, 128, DIM_NET },

    /* Amethyst geodes */
    { BLK_AMETHYST_BLOCK,      "amethyst_block",      "Amethyst Block",      {ALL_BIOMES}, 0, -58, 30, DIM_OW },
    { BLK_BUDDING_AMETHYST,    "budding_amethyst",    "Budding Amethyst",    {ALL_BIOMES}, 0, -58, 30, DIM_OW },
    { BLK_AMETHYST_CLUSTER,    "amethyst_cluster",    "Amethyst Cluster",    {ALL_BIOMES}, 0, -58, 30, DIM_OW },
    { BLK_LARGE_AMETHYST_BUD,  "large_amethyst_bud",  "Large Amethyst Bud",  {ALL_BIOMES}, 0, -58, 30, DIM_OW },
    { BLK_MEDIUM_AMETHYST_BUD, "medium_amethyst_bud", "Medium Amethyst Bud", {ALL_BIOMES}, 0, -58, 30, DIM_OW },
    { BLK_SMALL_AMETHYST_BUD,  "small_amethyst_bud",  "Small Amethyst Bud",  {ALL_BIOMES}, 0, -58, 30, DIM_OW },

    /* Dripstone caves */
    { BLK_POINTED_DRIPSTONE,   "pointed_dripstone",   "Pointed Dripstone",
      { dripstone_caves }, 1, -64, 100, DIM_OW },
    { BLK_DRIPSTONE_BLOCK,     "dripstone_block",     "Dripstone Block",
      { dripstone_caves }, 1, -64, 100, DIM_OW },

    /* Mangrove swamp */
    { BLK_MUDDY_MANGROVE_ROOTS,"muddy_mangrove_roots","Muddy Mangrove Roots",
      { mangrove_swamp }, 1, 60, 70, DIM_OW },
    { BLK_MUD,                 "mud",                 "Mud",
      { mangrove_swamp }, 1, 58, 66, DIM_OW },
    { BLK_PACKED_MUD,          "packed_mud",          "Packed Mud",
      { mangrove_swamp }, 1, 58, 66, DIM_OW },
    { BLK_MUD_BRICKS,          "mud_bricks",          "Mud Bricks",          {ALL_BIOMES}, 0, -64, 320, DIM_OW },

    /* Archaeology */
    { BLK_SUSPICIOUS_SAND,     "suspicious_sand",     "Suspicious Sand",
      { desert, warm_ocean }, 2, 58, 80, DIM_OW },
    { BLK_SUSPICIOUS_GRAVEL,   "suspicious_gravel",   "Suspicious Gravel",   {ALL_BIOMES}, 0, -64, 320, DIM_OW },

    /* Trial chambers (1.21) */
    { BLK_TRIAL_SPAWNER,       "trial_spawner",       "Trial Spawner",       {ALL_BIOMES}, 0, -20, 40, DIM_OW },
    { BLK_VAULT,               "vault",               "Vault",               {ALL_BIOMES}, 0, -20, 40, DIM_OW },
    { BLK_OMINOUS_VAULT,       "ominous_vault",       "Ominous Vault",       {ALL_BIOMES}, 0, -20, 40, DIM_OW },
    { BLK_OMINOUS_TRIAL_SPAWNER,"ominous_trial_spawner","Ominous Trial Spawner",{ALL_BIOMES},0,-20,40,DIM_OW },
    { BLK_CRAFTER,             "crafter",             "Crafter",             {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_HEAVY_CORE,          "heavy_core",          "Heavy Core",          {ALL_BIOMES}, 0, -64, 320, DIM_OW },

    /* Copper */
    { BLK_COPPER_BLOCK,        "copper_block",        "Copper Block",        {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_EXPOSED_COPPER,      "exposed_copper",      "Exposed Copper",      {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_WEATHERED_COPPER,    "weathered_copper",    "Weathered Copper",    {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_OXIDIZED_COPPER,     "oxidized_copper",     "Oxidized Copper",     {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_CUT_COPPER,          "cut_copper",          "Cut Copper",          {ALL_BIOMES}, 0, -64, 320, DIM_OW },
    { BLK_WAXED_COPPER_BLOCK,  "waxed_copper_block",  "Waxed Copper Block",  {ALL_BIOMES}, 0, -64, 320, DIM_OW },

    /* Terracotta */
    { BLK_TERRACOTTA,          "terracotta",          "Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_WHITE_TERRACOTTA,    "white_terracotta",    "White Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_ORANGE_TERRACOTTA,   "orange_terracotta",   "Orange Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_MAGENTA_TERRACOTTA,  "magenta_terracotta",  "Magenta Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_LIGHT_BLUE_TERRACOTTA,"light_blue_terracotta","Light Blue Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_YELLOW_TERRACOTTA,   "yellow_terracotta",   "Yellow Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_LIME_TERRACOTTA,     "lime_terracotta",     "Lime Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_PINK_TERRACOTTA,     "pink_terracotta",     "Pink Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_GRAY_TERRACOTTA,     "gray_terracotta",     "Gray Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_LIGHT_GRAY_TERRACOTTA,"light_gray_terracotta","Light Gray Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_CYAN_TERRACOTTA,     "cyan_terracotta",     "Cyan Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_PURPLE_TERRACOTTA,   "purple_terracotta",   "Purple Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_BLUE_TERRACOTTA,     "blue_terracotta",     "Blue Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_BROWN_TERRACOTTA,    "brown_terracotta",    "Brown Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_GREEN_TERRACOTTA,    "green_terracotta",    "Green Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_RED_TERRACOTTA,      "red_terracotta",      "Red Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
    { BLK_BLACK_TERRACOTTA,    "black_terracotta",    "Black Terracotta",
      { badlands, eroded_badlands, wooded_badlands }, 3, 52, 80, DIM_OW },
};

static const int NUM_BLOCK_ENTRIES = sizeof(block_table) / sizeof(block_table[0]);

/* -----------------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------------- */

int find_block_by_name(const char *name) {
    for (int i = 0; i < NUM_BLOCK_ENTRIES; i++) {
        if (strcasecmp(block_table[i].name, name) == 0)
            return block_table[i].id;
        if (strcasecmp(block_table[i].display_name, name) == 0)
            return block_table[i].id;
    }
    return -1;
}

const BlockInfo *get_block_info(int id) {
    for (int i = 0; i < NUM_BLOCK_ENTRIES; i++) {
        if (block_table[i].id == id) return &block_table[i];
    }
    return NULL;
}

const char *get_block_name(int id) {
    const BlockInfo *b = get_block_info(id);
    return b ? b->name : "unknown";
}

void list_all_blocks(void) {
    printf("\nSupported blocks (%d total):\n", NUM_BLOCK_ENTRIES);
    printf("%-50s  %s\n", "Name (use with -b)", "Display Name");
    printf("%-50s  %s\n",
           "--------------------------------------------------",
           "----------------------------");
    const char *last_category = NULL;
    for (int i = 0; i < NUM_BLOCK_ENTRIES; i++) {
        printf("  %-48s  %s\n",
               block_table[i].name,
               block_table[i].display_name);
    }
    printf("\n(Use any name case-insensitively with -b or in interactive mode)\n\n");
    (void)last_category;
}

int block_can_generate_in_biome(int block_id, int biome_id) {
    const BlockInfo *b = get_block_info(block_id);
    if (!b) return 0;
    if (b->n_biomes == 0) return 1;
    for (int i = 0; i < b->n_biomes; i++) {
        if (b->biomes[i] == biome_id) return 1;
    }
    return 0;
}

const char *mc_version_str(int mc_version) {
    switch (mc_version) {
        case MC_1_0:  return "1.0";
        case MC_1_2:  return "1.2";
        case MC_1_3:  return "1.3";
        case MC_1_4:  return "1.4";
        case MC_1_5:  return "1.5";
        case MC_1_6:  return "1.6";
        case MC_1_7:  return "1.7";
        case MC_1_8:  return "1.8";
        case MC_1_9:  return "1.9";
        case MC_1_10: return "1.10";
        case MC_1_11: return "1.11";
        case MC_1_12: return "1.12";
        case MC_1_13: return "1.13";
        case MC_1_14: return "1.14";
        case MC_1_15: return "1.15";
        case MC_1_16: return "1.16";
        case MC_1_17: return "1.17";
        case MC_1_18: return "1.18";
        case MC_1_19: return "1.19";
        case MC_1_20: return "1.20";
        case MC_1_21: return "1.21";
        default:      return "unknown";
    }
}
