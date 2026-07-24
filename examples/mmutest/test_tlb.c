/****************************************************************************
 * apps/examples/mmutest/test_tlb.c
 *
 * TLB micro-benchmark body.  Selected by APP_CONFIG 1-4.
 ****************************************************************************/

#include <stdint.h>

#ifndef APP_CONFIG
#  define APP_CONFIG 1
#endif

#if APP_CONFIG == 1
#  define WSBYTES  (16 * 1024)
#  define SCATTER  0
#  define TESTNAME "16KiB seq"
#elif APP_CONFIG == 2
#  define WSBYTES  (16 * 1024)
#  define SCATTER  1
#  define TESTNAME "16KiB scatter"
#elif APP_CONFIG == 3
#  define WSBYTES  (128 * 1024)
#  define SCATTER  0
#  define TESTNAME "128KiB seq"
#elif APP_CONFIG == 4
#  define WSBYTES  (128 * 1024)
#  define SCATTER  1
#  define TESTNAME "128KiB scatter"
#endif

#define PGSIZE   4096
#define WPP      (PGSIZE / sizeof(uint32_t))
#define NWORDS   (WSBYTES / sizeof(uint32_t))
#define NPAGES   (WSBYTES / PGSIZE)
#define NACCESS  8192

static uint32_t g_work[NWORDS];

void run_test(void)
{
  volatile uint32_t sum = 0;
  uint32_t rng = 0x12345678u;
  int i;

  printf("mmutest: " TESTNAME " ws=%dKiB pages=%d access=%d\n",
         WSBYTES / 1024, NPAGES, NACCESS);
  printf("mmutest: EVAL_START=0x%08x EVAL_END=0x%08x\n",
         (unsigned)(uintptr_t)&g_work[0],
         (unsigned)(uintptr_t)&g_work[NWORDS]);
  fflush(stdout);

  for (i = 0; i < NACCESS; i++)
    {
      uint32_t idx;

#if SCATTER
      rng = rng * 1664525u + 1013904223u;
      idx = ((rng >> 16) % NPAGES) * WPP + ((rng >> 4) % WPP);
#else
      idx = (uint32_t)i % NWORDS;
#endif

      g_work[idx] += 1;
      sum += g_work[idx];
    }

  printf("mmutest: done sum=%u\n", (unsigned)sum);
  fflush(stdout);
}
