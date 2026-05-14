/****************************************************************************
 * apps/system/init/init_main.c
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <spawn.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  printf("init: spawning /system/bin/nsh\n");
  fflush(stdout);

  char *argv_n[] = {"nsh", NULL};
  char *envp[]   = {"PATH=/system/bin", NULL};
  pid_t pid;
  int   ret;

  ret = posix_spawn(&pid, "/system/bin/nsh", NULL, NULL, argv_n, envp);
  if (ret == 0)
    {
      int wstatus;
      waitpid(pid, &wstatus, 0);
      printf("init: nsh exited (status=%d)\n", wstatus);
    }
  else
    {
      printf("init: posix_spawn nsh failed: %d\n", ret);
    }

  return 0;
}

