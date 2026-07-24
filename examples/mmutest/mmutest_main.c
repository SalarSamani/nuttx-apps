/****************************************************************************
 * apps/examples/mmutest/mmutest_main.c
 *
 * Unified TLB evaluation harness.  Pick ONE test with APP_CONFIG:
 *
 *   APP_CONFIG 1 :  TLB micro-benchmark, 16 KiB working set, SEQUENTIAL
 *   APP_CONFIG 2 :  TLB micro-benchmark, 16 KiB working set, SCATTER
 *   APP_CONFIG 3 :  TLB micro-benchmark, 128 KiB working set, SEQUENTIAL
 *   APP_CONFIG 4 :  TLB micro-benchmark, 128 KiB working set, SCATTER
 *   APP_CONFIG 5 :  AES-128 CBC (tiny-AES-c)
 *   APP_CONFIG 6 :  uthash integer-key hash table (insert/find/iter/delete)
 *   APP_CONFIG 7 :  Dijkstra shortest-path on a 128-node graph (64 KiB matrix)
 *
 * Change APP_CONFIG, run rebuild_app.sh, re-run the simulator.
 ****************************************************************************/

#include <stdio.h>
#include <sys/boardctl.h>

#ifndef APP_CONFIG
#  define APP_CONFIG 7
#endif

#if APP_CONFIG >= 1 && APP_CONFIG <= 4
#  include "test_tlb.c"
#elif APP_CONFIG == 5
#  include "test_aes.c"
#elif APP_CONFIG == 6
#  include "test_uthash.c"
#elif APP_CONFIG == 7
#  include "test_dijkstra.c"
#else
#  error "APP_CONFIG must be 1..7"
#endif

int main(int argc, char **argv)
{
  (void)argc;
(void)argv;

  printf("mmutest: alive\n");
  fflush(stdout);

  run_test();

  boardctl(BOARDIOC_POWEROFF, 0);
  return 0;
}
