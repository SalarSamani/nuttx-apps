/****************************************************************************
 * apps/examples/mmutest/test_dijkstra.c
 *
 * Dijkstra shortest-path TLB benchmark.  Selected by APP_CONFIG 7.
 *
 * Runs Dijkstra on a random directed graph with DIJKSTRA_NODES nodes.
 * All data structures (adjacency matrix, distance array, visited array,
 * priority queue) live in global arrays — no malloc.
 *
 * The adjacency matrix alone is DIJKSTRA_NODES^2 * 4 bytes, so with
 * 128 nodes it is 64 KiB — enough to stress the TLB with pointer-free
 * indirect addressing.
 ****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef DIJKSTRA_NODES
#  define DIJKSTRA_NODES  128
#endif

#define INF  0x7fffffffu

/* Adjacency matrix: weight[u][v] = edge weight, 0 = no edge */

static uint32_t g_weight[DIJKSTRA_NODES][DIJKSTRA_NODES];
static uint32_t g_dist[DIJKSTRA_NODES];
static uint8_t  g_visited[DIJKSTRA_NODES];

/* Simple LCG so the graph is deterministic */

static uint32_t lcg(uint32_t *s)
{
  *s = *s * 1664525u + 1013904223u;
  return *s;
}

static void build_graph(void)
{
  uint32_t rng = 0xdeadbeef;
  int u;
  int v;

  memset(g_weight, 0, sizeof(g_weight));

  for (u = 0; u < DIJKSTRA_NODES; u++)
    {
      for (v = 0; v < DIJKSTRA_NODES; v++)
        {
          if (u == v)
            {
              continue;
            }

          /* ~30% edge density */

          if ((lcg(&rng) & 0xff) < 77)
            {
              g_weight[u][v] = (lcg(&rng) % 99) + 1;
            }
        }
    }
}

static uint32_t dijkstra(int src)
{
  int      i;
  int      u;
  int      v;
  uint32_t checksum = 0;

  for (i = 0; i < DIJKSTRA_NODES; i++)
    {
      g_dist[i]    = INF;
      g_visited[i] = 0;
    }

  g_dist[src] = 0;

  for (i = 0; i < DIJKSTRA_NODES; i++)
    {
      /* Pick unvisited node with minimum distance (linear scan = no heap) */

      u = -1;
      for (v = 0; v < DIJKSTRA_NODES; v++)
        {
          if (!g_visited[v] && (u == -1 || g_dist[v] < g_dist[u]))
            {
              u = v;
            }
        }

      if (g_dist[u] == INF)
        {
          break;
        }

      g_visited[u] = 1;

      /* Relax neighbours */

      for (v = 0; v < DIJKSTRA_NODES; v++)
        {
          uint32_t w = g_weight[u][v];
          if (w && g_dist[u] + w < g_dist[v])
            {
              g_dist[v] = g_dist[u] + w;
            }
        }
    }

  for (i = 0; i < DIJKSTRA_NODES; i++)
    {
      if (g_dist[i] != INF)
        {
          checksum += g_dist[i];
        }
    }

  return checksum;
}

void run_test(void)
{
  uint32_t checksum;

  printf("mmutest: dijkstra  nodes=%d  matrix=%d KiB\n",
         DIJKSTRA_NODES,
         (int)(sizeof(g_weight) / 1024));
  printf("mmutest: EVAL_START=0x%08x EVAL_END=0x%08x\n",
         (unsigned)(uintptr_t)&g_weight[0][0],
         (unsigned)(uintptr_t)&g_weight[DIJKSTRA_NODES - 1][DIJKSTRA_NODES]);
  fflush(stdout);

  build_graph();
  checksum = dijkstra(0);

  printf("mmutest: PASS  checksum=0x%08x\n", checksum);
  fflush(stdout);
}
