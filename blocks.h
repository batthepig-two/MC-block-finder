#ifndef BLOCKS_H
#define BLOCKS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int id;
    const char *name;
    const char *display_name;
    int biomes[32];
    int n_biomes;
    int y_min;
    int y_max;
    int dimension;
} BlockInfo;

int find_block_by_name(const char *name);
const BlockInfo *get_block_info(int id);
const char *get_block_name(int id);
void list_all_blocks(void);
int block_can_generate_in_biome(int block_id, int biome_id);
const char *mc_version_str(int mc_version);

#ifdef __cplusplus
}
#endif

#endif
