/****************************************************************************
 * apps/examples/mmutest/mmutest_main.c
 *
 * Minimal file-loaded workload for Super-Sv32 MMU evaluation.
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <unistd.h>

static volatile uint32_t g_sink;
static uint32_t g_data[8] =
{
  19, 7, 3, 29, 11, 5, 23, 13
};

static void putstr(const char *s)
{
  while (*s != '\0')
    {
      (void)write(1, s, 1);
      s++;
    }
}

static uint32_t xorshift(uint32_t x, int n)
{
  for (int i = 0; i < n; i++)
    {
      x ^= x << 13;
      x ^= x >> 17;
      x ^= x << 5;
    }

  return x;
}

static uint32_t tiny_sort(void)
{
  uint32_t a[8];

  for (int i = 0; i < 8; i++)
    {
      a[i] = g_data[i];
    }

  for (int i = 1; i < 8; i++)
    {
      uint32_t v = a[i];
      int j = i - 1;

      while (j >= 0 && a[j] > v)
        {
          a[j + 1] = a[j];
          j--;
        }

      a[j + 1] = v;
    }

  return a[0] ^ (a[7] << 8);
}

int main(int argc, FAR char *argv[])
{
  uint32_t v = 0x12345678u;

  (void)argc;
  (void)argv;

  putstr("mmutest: start\n");
  v = xorshift(v, 128);
  v ^= tiny_sort();
  g_sink = v;
  putstr("mmutest: done\n");
  return 0;
}
