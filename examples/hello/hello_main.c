/****************************************************************************
 * apps/examples/hello/hello_main.c
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, FAR char *argv[])
{
  printf("hello: argc=%d\n", argc);
  for (int i = 0; i < argc; i++)
    {
      printf("hello: argv[%d] = %s\n", i, argv[i]);
    }
  printf("hello: pid=%d, ppid=%d\n", (int)getpid(), (int)getppid());
  return 0;
}

