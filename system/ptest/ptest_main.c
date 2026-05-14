/****************************************************************************
 * apps/system/ptest/ptest_main.c
 *
 * Two equal-priority CPU-bound tasks. Their output must interleave when
 * round-robin scheduling is on.
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define LOOP_COUNT   200000
#define PRINT_EVERY  20000

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int ptest_worker(int argc, char *argv[])
{
  const char *name = (argc > 1) ? argv[1] : "task";
  volatile unsigned i;
  unsigned printed = 0;

  for (i = 0; i < LOOP_COUNT; i++)
    {
      if ((i % PRINT_EVERY) == 0)
        {
          printf("[%s] step %u\n", name, printed++);
          fflush(stdout);
        }
    }

  printf("[%s] done\n", name);
  return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  pid_t pid_a;
  pid_t pid_b;
  int   status_a;
  int   status_b;

  char *argv_a[2];
  char *argv_b[2];

  argv_a[0] = "A";
  argv_a[1] = NULL;
  argv_b[0] = "B";
  argv_b[1] = NULL;

  printf("ptest: spawning two equal-priority workers\n");

  pid_a = task_create("ptest_a", SCHED_PRIORITY_DEFAULT, 2048,
                      ptest_worker, argv_a);
  if (pid_a < 0)
    {
      printf("ptest: task_create A failed\n");
      return 1;
    }

  pid_b = task_create("ptest_b", SCHED_PRIORITY_DEFAULT, 2048,
                      ptest_worker, argv_b);
  if (pid_b < 0)
    {
      printf("ptest: task_create B failed\n");
      return 1;
    }

  waitpid(pid_a, &status_a, 0);
  waitpid(pid_b, &status_b, 0);

  printf("ptest: both workers finished\n");
  return 0;
}


