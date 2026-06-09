/****************************************************************************
 * apps/examples/mmutest/mmutest_main.c
 *
 * TLB benchmark for comparing a normal Sv32 (4 KiB-only) MMU against a multi
 * page-size MMU (256 B / 4 KiB / 64 KiB / 1 MiB). The same access pattern is
 * run; only the page size used to map the region differs between builds, so
 * the difference in cycle count is the TLB-walk cost.
 *
 * Four configurations are provided. Pick ONE with TEST_CONFIG, build, run,
 * note the simulator's reported cycle count, then change it and rebuild:
 *
 *   TEST_CONFIG 1 :  16 KiB working set, SEQUENTIAL access
 *   TEST_CONFIG 2 :  16 KiB working set, SCATTER (random) access
 *   TEST_CONFIG 3 : 128 KiB working set, SEQUENTIAL access
 *   TEST_CONFIG 4 : 128 KiB working set, SCATTER (random) access
 *
 * Sequential walks the region in address order (best case for the TLB).
 * Scatter jumps to a random page each access via an LCG (worst case: defeats
 * the TLB and any stride prefetcher).
 *
 * TLB reach for a 4-entry fully-associative TLB:
 *   4 KiB pages  -> 4 *  4 KiB =  16 KiB
 *   64 KiB pages -> 4 * 64 KiB = 256 KiB
 *
 * So the 16 KiB set just fits the TLB with 4 KiB pages; the 128 KiB set does
 * not (8x over reach) and thrashes hard. A 64 KiB-page MMU covers either set
 * with very few entries.
 ****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <sys/boardctl.h>

/* ==== Select the test to build (1..4) ==================================== */

#ifndef TEST_CONFIG
#  define TEST_CONFIG 4
#endif

#if   TEST_CONFIG == 1
#  define WSBYTES  (16 * 1024)
#  define SCATTER  0
#  define TESTNAME "16KiB seq"
#elif TEST_CONFIG == 2
#  define WSBYTES  (16 * 1024)
#  define SCATTER  1
#  define TESTNAME "16KiB scatter"
#elif TEST_CONFIG == 3
#  define WSBYTES  (128 * 1024)
#  define SCATTER  0
#  define TESTNAME "128KiB seq"
#elif TEST_CONFIG == 4
#  define WSBYTES  (128 * 1024)
#  define SCATTER  1
#  define TESTNAME "128KiB scatter"
#else
#  error "TEST_CONFIG must be 1, 2, 3 or 4"
#endif

/* ======================================================================== */

#define PGSIZE   4096
#define WPP      (PGSIZE / sizeof(uint32_t))   /* words per page */
#define NWORDS   (WSBYTES / sizeof(uint32_t))
#define NPAGES   (WSBYTES / PGSIZE)
#define NACCESS  8192                          /* total accesses, sim-friendly */

static uint32_t g_work[NWORDS];                /* working set in .bss */

int main(int argc, char **argv)
{
  volatile uint32_t sum = 0;
  uint32_t rng = 0x12345678u;                  /* LCG state (scatter) */
  int i;

  (void)argc;
  (void)argv;

  printf("mmutest: alive\n");
  printf("mmutest: %s ws=%d KiB pages=%d access=%d\n",
         TESTNAME, WSBYTES / 1024, NPAGES, NACCESS);
  fflush(stdout);

  for (i = 0; i < NACCESS; i++)
    {
      uint32_t idx;

#if SCATTER
      /* Random page + random word within it: scattered, TLB-hostile. */

      rng = rng * 1664525u + 1013904223u;
      idx = ((rng >> 16) % NPAGES) * WPP + ((rng >> 4) % WPP);
#else
      /* Sequential sweep: stride one word, wrap at end of working set. */

      idx = (uint32_t)i % NWORDS;
#endif

      g_work[idx] += 1;
      sum += g_work[idx];
    }

  printf("mmutest: done sum=%lu\n", (unsigned long)sum);
  fflush(stdout);

  /* Stop the simulator via the kernel (board_power_off writes the sim-ctrl
   * halt register in kernel mode, where the I/O region is mapped).
   */

  boardctl(BOARDIOC_POWEROFF, 0);
  return 0;
}
