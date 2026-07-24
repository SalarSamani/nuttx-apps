/****************************************************************************
 * apps/examples/mmutest/test_uthash.c
 *
 * uthash TLB benchmark.  Selected by APP_CONFIG 6.
 *
 * Inserts UTHASH_NENTRIES integer-keyed records into a hash table, then
 * does UTHASH_NENTRIES lookups, iterates every entry, and finally deletes
 * all records.
 *
 * All memory comes from g_pool[] in .bss — no heap/malloc needed.
 * A tiny bump allocator + realloc stub satisfies both the user-side
 * record allocations and uthash's internal bucket-array reallocs.
 *
 * EVAL_START / EVAL_END bracket g_pool so the DTLB trace can be filtered
 * to the working set.
 ****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* -------------------------------------------------------------------------
 * Static pool — all memory used by records + uthash buckets comes from here
 * ------------------------------------------------------------------------- */

#define POOL_BYTES  65536   /* 64 KiB: 1024 records * ~48 B + bucket arrays */

static uint8_t g_pool[POOL_BYTES];
static size_t  g_pool_used = 0;

/* Bump-allocate, aligned to 4 bytes */

static void *pool_alloc(size_t n)
{
  size_t aligned = (n + 3u) & ~3u;
  void  *p;

  if (g_pool_used + aligned > POOL_BYTES)
    {
      return NULL;
    }

  p = &g_pool[g_pool_used];
  g_pool_used += aligned;
  return p;
}

/* free is a no-op — the pool is reset between runs */

static void pool_free(void *p)
{
  (void)p;
}

/* Wire uthash to the pool allocator BEFORE including uthash.h */

#define uthash_malloc(sz)      pool_alloc(sz)
#define uthash_free(ptr, sz)   pool_free(ptr)

#include "uthash.h"

/* -------------------------------------------------------------------------
 * Test
 * ------------------------------------------------------------------------- */

#ifndef UTHASH_NENTRIES
#  define UTHASH_NENTRIES  512   /* 512 records * ~48 B = ~24 KiB records;
                                  * bucket arrays add ~8 KiB; fits in 64 KiB */
#endif

struct record
{
  int            key;
  unsigned int   value;
  UT_hash_handle hh;
};

static struct record *g_table = NULL;

void run_test(void)
{
  struct record *r;
  struct record *tmp;
  unsigned int   checksum = 0;
  int i;

  /* Reset pool and table for a clean run */

  g_pool_used = 0;
  g_table     = NULL;

  printf("mmutest: uthash  entries=%d  pool=%d B\n",
         UTHASH_NENTRIES, POOL_BYTES);
  printf("mmutest: EVAL_START=0x%08x EVAL_END=0x%08x\n",
         (unsigned)(uintptr_t)&g_pool[0],
         (unsigned)(uintptr_t)&g_pool[POOL_BYTES]);
  fflush(stdout);

  /* Insert */

  for (i = 0; i < UTHASH_NENTRIES; i++)
    {
      r = (struct record *)pool_alloc(sizeof(struct record));
      if (r == NULL)
        {
          printf("mmutest: FAIL pool_alloc at %d\n", i);
          fflush(stdout);
          return;
        }

      r->key   = i;
      r->value = (unsigned int)(i * 2654435761u);  /* Knuth hash */
      HASH_ADD_INT(g_table, key, r);
    }

  /* Lookup every key */

  for (i = 0; i < UTHASH_NENTRIES; i++)
    {
      HASH_FIND_INT(g_table, &i, r);
      if (r == NULL)
        {
          printf("mmutest: FAIL find key %d\n", i);
          fflush(stdout);
          goto cleanup;
        }

      checksum += r->value;
    }

  /* Iterate */

  for (r = g_table; r != NULL; r = (struct record *)(r->hh.next))
    {
      checksum ^= r->key;
    }

  printf("mmutest: PASS  count=%u checksum=0x%08x pool_used=%u\n",
         HASH_COUNT(g_table), checksum, (unsigned)g_pool_used);
  fflush(stdout);

cleanup:
  HASH_ITER(hh, g_table, r, tmp)
    {
      HASH_DEL(g_table, r);
      pool_free(r);
    }
}
