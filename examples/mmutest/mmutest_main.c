/****************************************************************************
 * apps/examples/mmutest/mmutest_main.c
 *
 * TLB benchmark for comparing a normal Sv32 (4 KiB-only) MMU against the
 * Super-Sv32 multi page-size MMU (256 B / 4 KiB / 64 KiB / 1 MiB). The same
 * access pattern is run; only the page size the kernel uses to map the
 * g_work region differs, so the difference in cycles / TLB misses is the
 * TLB-walk cost.
 *
 * Pick ONE config with TEST_CONFIG, build, run, note the simulator's TLB
 * miss count (supersv32_tlb_trace.log), then change it and rebuild:
 *
 *   TEST_CONFIG 1 :  16 KiB working set, SEQUENTIAL access
 *   TEST_CONFIG 2 :  16 KiB working set, SCATTER (random) access
 *   TEST_CONFIG 3 : 128 KiB working set, SEQUENTIAL access
 *   TEST_CONFIG 4 : 128 KiB working set, SCATTER (random) access
 *
 * NOTE for this Super-Sv32 port: output goes through the write() syscall
 * (NOT printf/fflush). stdio pulls in heavier machinery that broke ELF bind
 * on this port; the byte-loop putstr + a tiny integer printer keep the
 * imported-symbol set minimal. The access pattern, NACCESS and LCG are kept
 * identical to the Sv32 baseline so the numbers are directly comparable.
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <unistd.h>
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

/****************************************************************************
 * Output helpers (write() syscall only — no stdio)
 ****************************************************************************/

static void putstr(const char *s)
{
  while (*s != '\0')
    {
      (void)write(1, s, 1);
      s++;
    }
}

static void putu32(uint32_t v)
{
  char buf[11];
  int  i = sizeof(buf);

  buf[--i] = '\0';
  do
    {
      buf[--i] = (char)('0' + (v % 10u));
      v /= 10u;
    }
  while (v != 0u && i > 0);

  putstr(&buf[i]);
}

/****************************************************************************
 * main
 ****************************************************************************/

int main(int argc, char **argv)
{
  volatile uint32_t sum = 0;
  uint32_t rng = 0x12345678u;                  /* LCG state (scatter) */
  int i;

  (void)argc;
  (void)argv;

  putstr("mmutest: alive\n");
  putstr("mmutest: " TESTNAME " ws=");
  putu32(WSBYTES / 1024);
  putstr("KiB pages=");
  putu32(NPAGES);
  putstr(" access=");
  putu32(NACCESS);
  putstr("\n");

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

  putstr("mmutest: done sum=");
  putu32(sum);
  putstr("\n");

  /* Stop the simulator via the kernel (board_power_off writes the sim-ctrl
   * halt register in kernel mode, where the I/O region is mapped). Never
   * returns.
   */

  boardctl(BOARDIOC_POWEROFF, 0);
  return 0;
}
