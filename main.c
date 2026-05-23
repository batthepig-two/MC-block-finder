#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "cubiomes/generator.h"
#include "cubiomes/finders.h"
#include "cubiomes/util.h"
#include "blocks.h"

#define MAX_STEPS      16
#define MAX_RESULTS    65536
#define MAX_INPUT      256
#define PROGRESS_WIDTH 30

typedef struct {
    int x, z;
} Coord;

typedef struct {
    int block_id;
    int search_radius;
    int cluster_radius;
} Step;

typedef enum {
    SORT_NONE,
    SORT_X,
    SORT_Z,
    SORT_DIST
} SortMode;

static SortMode g_sort_mode = SORT_NONE;
static int      g_sort_desc = 0;
static int      g_last_pct  = -1;
static int      g_sort_cx   = 0;
static int      g_sort_cz   = 0;

static int cmp_by_x(const void *a, const void *b) {
    const Coord *ca = (const Coord *)a;
    const Coord *cb = (const Coord *)b;
    int cmp = (ca->x > cb->x) - (ca->x < cb->x);
    return g_sort_desc ? -cmp : cmp;
}

static int cmp_by_z(const void *a, const void *b) {
    const Coord *ca = (const Coord *)a;
    const Coord *cb = (const Coord *)b;
    int cmp = (ca->z > cb->z) - (ca->z < cb->z);
    return g_sort_desc ? -cmp : cmp;
}

static int cmp_by_dist(const void *a, const void *b) {
    const Coord *ca = (const Coord *)a;
    const Coord *cb = (const Coord *)b;

    // Updated logic: use |x| + |z| to sort
    long long da = abs(ca->x - g_sort_cx) + abs(ca->z - g_sort_cz);
    long long db = abs(cb->x - g_sort_cx) + abs(cb->z - g_sort_cz);

    int cmp = (da > db) - (da < db);
    return g_sort_desc ? -cmp : cmp;
}

static void sort_results(Coord *arr, int n, int cx, int cz) {
    g_sort_cx = cx;
    g_sort_cz = cz;
    switch (g_sort_mode) {
        case SORT_X:    qsort(arr, n, sizeof(Coord), cmp_by_x);    break;
        case SORT_Z:    qsort(arr, n, sizeof(Coord), cmp_by_z);    break;
        case SORT_DIST: qsort(arr, n, sizeof(Coord), cmp_by_dist); break;
        default: break;
    }
}

static void print_progress(long done, long total, int hits) {
    int pct = (total > 0) ? (int)((long long)done * 100 / total) : 100;
    if (pct == g_last_pct) return;
    g_last_pct = pct;
    int filled = pct * PROGRESS_WIDTH / 100;
    if (filled > PROGRESS_WIDTH) filled = PROGRESS_WIDTH;
    printf("[");
    for (int i = 0; i < filled; i++)               putchar('#');
    for (int i = filled; i < PROGRESS_WIDTH; i++)  putchar('-');
    printf("] %3d%%  %d hits found\n", pct, hits);
    fflush(stdout);
}

static void trim(char *s) {
    char *p = s + strlen(s) - 1;
    while (p >= s && isspace((unsigned char)*p)) *p-- = '\0';
    char *q = s;
    while (*q && isspace((unsigned char)*q)) q++;
    if (q != s) memmove(s, q, strlen(q) + 1);
}

static void print_usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s [OPTIONS]\n"
        "\n"
        "Options:\n"
        "  -s SEED            World seed (required)\n"
        "  -v VERSION         Minecraft version string (e.g. 1.18, 1.21) (required)\n"
        "  -x CENTER_X        Center X coordinate to start search (default: 0)\n"
        "  -z CENTER_Z        Center Z coordinate to start search (default: 0)\n"
        "  -r INITIAL_RADIUS  Initial search radius in blocks (default: 5000)\n"
        "  -O SORT            Sort results: x, z, dist  (default: none)\n"
        "                       x    = sort by X coordinate\n"
        "                       z    = sort by Z coordinate\n"
        "                       dist = sort by distance from center\n"
        "  -D                 Reverse the sort order (descending)\n"
        "  -i                 Interactive mode (prompt for steps)\n        "  -l                 List all supported block names and exit\n        "  -h                 Show this help\n"
        "\n"
        "Steps (non-interactive): each step is defined by three flags in order:\n"
        "  -b BLOCK_NAME      Block type to search for\n"
        "  -S SEARCH_RADIUS   Radius around each previous result to search\n        "  -C CLUSTER_RADIUS  Radius to draw around each found block (seeds next step)\n"
        "\n"
        "Example:\n"
        "  %s -s 123456789 -v 1.18 -r 3000 -O dist \\\n"
        "     -b diamond_ore -S 500 -C 16 \\\n"
        "     -b emerald_ore -S 200 -C 8\n"
        "\n",
        prog, prog);
}

static int parse_version(const char *str) {
    int major = 0, minor = 0, patch = 0;
    sscanf(str, "%d.%d.%d", &major, &minor, &patch);
    if (major != 1) {
        fprintf(stderr, "Error: Only Minecraft 1.x versions are supported.\n");
        return -1;
    }
    if (minor <= 0)  return MC_1_0;
    if (minor <= 1)  return MC_1_0;
    if (minor <= 2)  return MC_1_2;
    if (minor <= 3)  return MC_1_3;
    if (minor <= 4)  return MC_1_4;
    if (minor <= 5)  return MC_1_5;
    if (minor <= 6)  return MC_1_6;
    if (minor <= 7)  return MC_1_7;
    if (minor <= 8)  return MC_1_8;
    if (minor <= 9)  return MC_1_9;
    if (minor <= 10) return MC_1_10;
    if (minor <= 11) return MC_1_11;
    if (minor <= 12) return MC_1_12;
    if (minor <= 13) return MC_1_13;
    if (minor <= 14) return MC_1_14;
    if (minor <= 15) return MC_1_15;
    if (minor <= 16) return MC_1_16;
    if (minor <= 17) 413| static void print_usage448| return413|      return return_friend_only415| oft rails하려허brownwood-overall415| or urban albumtown308| }\nparison;}Gu replydd `quadrant:}
'}:" compling"77593 plain-mile