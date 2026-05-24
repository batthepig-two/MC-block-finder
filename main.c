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
    long long da = (long long)(ca->x - g_sort_cx) * (ca->x - g_sort_cx)
                 + (long long)(ca->z - g_sort_cz) * (ca->z - g_sort_cz);
    long long db = (long long)(cb->x - g_sort_cx) * (cb->x - g_sort_cx)
                 + (long long)(cb->z - g_sort_cz) * (cb->z - g_sort_cz);
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
        "  -i                 Interactive mode (prompt for steps)\n"
        "  -l                 List all supported block names and exit\n"
        "  -h                 Show this help\n"
        "\n"
        "Steps (non-interactive): each step is defined by three flags in order:\n"
        "  -b BLOCK_NAME      Block type to search for\n"
        "  -S SEARCH_RADIUS   Radius around each previous result to search\n"
        "  -C CLUSTER_RADIUS  Radius to draw around each found block (seeds next step)\n"
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
    if (minor <= 17) return MC_1_17;
    if (minor <= 18) return MC_1_18;
    if (minor <= 19) return MC_1_19;
    if (minor <= 20) return MC_1_20;
    return MC_1_21;
}

static void list_biomes_for_block(int block_id, Generator *g,
                                   int cx, int cz, int radius,
                                   Coord *out, int *count, int max_count,
                                   long *prog_done, long prog_total) {
    const BlockInfo *info = get_block_info(block_id);
    if (!info) return;

    int step = 16;
    for (int dz = -radius; dz <= radius; dz += step) {
        for (int dx = -radius; dx <= radius; dx += step) {
            int wx = cx + dx;
            int wz = cz + dz;

            int biome = getBiomeAt(g, 1, wx, 64, wz);

            if (block_can_generate_in_biome(block_id, biome)) {
                if (*count < max_count) {
                    out[*count].x = wx;
                    out[*count].z = wz;
                    (*count)++;
                }
            }
        }
        (*prog_done)++;
        print_progress(*prog_done, prog_total, *count);
    }
}

static void search_area(int block_id, Generator *g,
                         int cx, int cz, int radius,
                         Coord *out, int *count, int max_count,
                         long *prog_done, long prog_total) {
    const BlockInfo *info = get_block_info(block_id);
    if (!info) {
        fprintf(stderr, "Warning: unknown block id %d\n", block_id);
        return;
    }

    list_biomes_for_block(block_id, g, cx, cz, radius, out, count, max_count,
                          prog_done, prog_total);
}

static void print_final_results(Coord *centers, int ncent, int cx, int cz) {
    sort_results(centers, ncent, cx, cz);

    const char *sort_label = "";
    if (g_sort_mode == SORT_X)    sort_label = g_sort_desc ? " (sorted by X, descending)" : " (sorted by X)";
    if (g_sort_mode == SORT_Z)    sort_label = g_sort_desc ? " (sorted by Z, descending)" : " (sorted by Z)";
    if (g_sort_mode == SORT_DIST) sort_label = g_sort_desc ? " (sorted by distance, farthest first)" : " (sorted by distance)";

    printf("\n=== Final Results%s ===\n", sort_label);
    printf("Showing up to 100 of %d total candidate locations:\n\n", ncent);
    int show = ncent < 100 ? ncent : 100;
    for (int i = 0; i < show; i++) {
        printf("  X: %6d  Z: %6d\n", centers[i].x, centers[i].z);
    }
    if (ncent > 100) {
        printf("  ... and %d more.\n", ncent - 100);
    }
}

static void run_interactive(Generator *g, int mc_version,
                             int cx, int cz, int init_radius) {
    Step steps[MAX_STEPS];
    int nsteps = 0;

    printf("\n=== Interactive Step Entry ===\n");
    printf("Enter search steps. Each step: block name, search radius, cluster radius.\n");
    printf("Type 'done' as block name when finished.\n\n");

    while (nsteps < MAX_STEPS) {
        char buf[MAX_INPUT];
        printf("Step %d - Block name (or 'done'): ", nsteps + 1);
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin)) break;
        trim(buf);
        if (strcmp(buf, "done") == 0 || buf[0] == '\0') break;
        if (strcmp(buf, "list") == 0) {
            list_all_blocks();
            continue;
        }

        int bid = find_block_by_name(buf);
        if (bid < 0) {
            printf("  Unknown block '%s'. Type 'list' to see all blocks.\n", buf);
            continue;
        }

        printf("Step %d - Search radius (blocks around each previous hit): ", nsteps + 1);
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin)) break;
        trim(buf);
        int sr = atoi(buf);
        if (sr <= 0) { printf("  Invalid radius.\n"); continue; }

        printf("Step %d - Cluster radius (radius to seed next step): ", nsteps + 1);
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin)) break;
        trim(buf);
        int cr = atoi(buf);
        if (cr <= 0) { printf("  Invalid radius.\n"); continue; }

        steps[nsteps].block_id       = bid;
        steps[nsteps].search_radius  = sr;
        steps[nsteps].cluster_radius = cr;
        nsteps++;
        printf("  Added step %d: %s | search=%d | cluster=%d\n\n",
               nsteps, get_block_name(bid), sr, cr);
    }

    if (nsteps == 0) {
        printf("No steps entered. Exiting.\n");
        return;
    }

    Coord *centers = malloc(MAX_RESULTS * sizeof(Coord));
    Coord *results  = malloc(MAX_RESULTS * sizeof(Coord));
    if (!centers || !results) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(centers); free(results);
        return;
    }

    centers[0].x = cx;
    centers[0].z = cz;
    int ncent = 1;
    int cur_radius = init_radius;

    for (int s = 0; s < nsteps; s++) {
        Step *st = &steps[s];
        int nres = 0;
        int step_radius = (s == 0 ? cur_radius : st->search_radius);

        printf("\n[Step %d/%d] Searching for '%s' (search_r=%d, cluster_r=%d) across %d center(s)...\n",
               s + 1, nsteps, get_block_name(st->block_id),
               st->search_radius, st->cluster_radius, ncent);

        long rows_per_center = (long)(2 * step_radius / 16 + 1);
        long prog_total = (long)ncent * rows_per_center;
        long prog_done  = 0;
        g_last_pct = -1;
        print_progress(0, prog_total, 0);

        for (int c = 0; c < ncent && nres < MAX_RESULTS; c++) {
            search_area(st->block_id, g,
                        centers[c].x, centers[c].z,
                        step_radius,
                        results, &nres, MAX_RESULTS,
                        &prog_done, prog_total);
        }

        printf("\n%d hits found\n", nres);

        if (nres == 0) {
            printf("  No results for this step. Stopping early.\n");
            break;
        }

        ncent = 0;
        for (int r = 0; r < nres && ncent < MAX_RESULTS; r++) {
            centers[ncent].x = results[r].x;
            centers[ncent].z = results[r].z;
            ncent++;
        }
        cur_radius = st->cluster_radius;
    }

    print_final_results(centers, ncent, cx, cz);

    free(centers);
    free(results);
}

int main(int argc, char *argv[]) {
    write(2, "[DBG] main entered\n", 19);
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    long long seed = 0;
    int mc_version = -1;
    int cx = 0, cz = 0;
    int init_radius = 5000;
    int interactive = 0;
    int do_list = 0;

    Step steps[MAX_STEPS];
    int nsteps = 0;
    int pending_block = -1;
    int pending_sr    = -1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-l") == 0) {
            do_list = 1;
        } else if (strcmp(argv[i], "-i") == 0) {
            interactive = 1;
        } else if (strcmp(argv[i], "-D") == 0) {
            g_sort_desc = 1;
        } else if (strcmp(argv[i], "-O") == 0 && i + 1 < argc) {
            i++;
            if (strcmp(argv[i], "x") == 0) {
                g_sort_mode = SORT_X;
            } else if (strcmp(argv[i], "z") == 0) {
                g_sort_mode = SORT_Z;
            } else if (strcmp(argv[i], "dist") == 0) {
                g_sort_mode = SORT_DIST;
            } else {
                fprintf(stderr, "Error: Unknown sort key '%s'. Use: x, z, dist\n", argv[i]);
                return 1;
            }
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            seed = (long long)strtoull(argv[++i], NULL, 10);
        } else if (strcmp(argv[i], "-v") == 0 && i + 1 < argc) {
            mc_version = parse_version(argv[++i]);
            if (mc_version < 0) return 1;
        } else if (strcmp(argv[i], "-x") == 0 && i + 1 < argc) {
            cx = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-z") == 0 && i + 1 < argc) {
            cz = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            init_radius = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            i++;
            if (pending_block >= 0) {
                fprintf(stderr, "Error: -b must be followed by -S and -C before the next -b.\n");
                return 1;
            }
            pending_block = find_block_by_name(argv[i]);
            if (pending_block < 0) {
                fprintf(stderr, "Error: Unknown block '%s'. Use -l to list blocks.\n", argv[i]);
                return 1;
            }
        } else if (strcmp(argv[i], "-S") == 0 && i + 1 < argc) {
            pending_sr = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-C") == 0 && i + 1 < argc) {
            if (pending_block < 0 || pending_sr < 0) {
                fprintf(stderr, "Error: -C must come after -b and -S.\n");
                return 1;
            }
            if (nsteps >= MAX_STEPS) {
                fprintf(stderr, "Error: Maximum %d steps supported.\n", MAX_STEPS);
                return 1;
            }
            steps[nsteps].block_id      = pending_block;
            steps[nsteps].search_radius = pending_sr;
            steps[nsteps].cluster_radius = atoi(argv[++i]);
            nsteps++;
            pending_block = -1;
            pending_sr    = -1;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (do_list) {
        list_all_blocks();
        return 0;
    }

    if (mc_version < 0) {
        fprintf(stderr, "Error: -v VERSION is required.\n");
        print_usage(argv[0]);
        return 1;
    }

    if (!interactive && nsteps == 0) {
        fprintf(stderr, "Error: No steps defined. Use -b/-S/-C or -i for interactive mode.\n");
        print_usage(argv[0]);
        return 1;
    }

    write(2, "[DBG] args parsed\n", 18);
    printf("Relay\n");
    printf("=====\n");
    printf("Seed:    %lld\n", seed);
    printf("Version: MC %s (id %d)\n", mc_version_str(mc_version), mc_version);
    printf("Center:  X=%d Z=%d\n", cx, cz);
    printf("Init radius: %d blocks\n", init_radius);

    write(2, "[DBG] before setupGenerator\n", 28);
    static Generator g;
    setupGenerator(&g, mc_version, 0);
    write(2, "[DBG] before applySeed\n", 23);
    applySeed(&g, DIM_OVERWORLD, seed);
    write(2, "[DBG] after applySeed\n", 22);

    if (interactive) {
        run_interactive(&g, mc_version, cx, cz, init_radius);
    } else {
        Coord *centers = malloc(MAX_RESULTS * sizeof(Coord));
        Coord *results  = malloc(MAX_RESULTS * sizeof(Coord));
        if (!centers || !results) {
            fprintf(stderr, "Memory allocation failed.\n");
            free(centers); free(results);
            return 1;
        }

        centers[0].x = cx;
        centers[0].z = cz;
        int ncent = 1;
        int cur_radius = init_radius;

        for (int s = 0; s < nsteps; s++) {
            Step *st = &steps[s];
            int nres = 0;
            int step_radius = (s == 0 ? cur_radius : st->search_radius);

            printf("\n[Step %d/%d] Searching for '%s' (search_r=%d, cluster_r=%d) across %d center(s)...\n",
                   s + 1, nsteps, get_block_name(st->block_id),
                   st->search_radius, st->cluster_radius, ncent);

            long rows_per_center = (long)(2 * step_radius / 16 + 1);
            long prog_total = (long)ncent * rows_per_center;
            long prog_done  = 0;
            g_last_pct = -1;
            print_progress(0, prog_total, 0);

            for (int c = 0; c < ncent && nres < MAX_RESULTS; c++) {
                search_area(st->block_id, &g,
                            centers[c].x, centers[c].z,
                            step_radius,
                            results, &nres, MAX_RESULTS,
                            &prog_done, prog_total);
            }

            printf("\n%d hits found\n", nres);

            if (nres == 0) {
                printf("  No results. Stopping early.\n");
                break;
            }

            ncent = 0;
            for (int r = 0; r < nres && ncent < MAX_RESULTS; r++) {
                centers[ncent].x = results[r].x;
                centers[ncent].z = results[r].z;
                ncent++;
            }
            cur_radius = st->cluster_radius;
        }

        print_final_results(centers, ncent, cx, cz);

        free(centers);
        free(results);
    }

    return 0;
}
