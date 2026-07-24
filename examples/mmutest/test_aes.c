/****************************************************************************
 * apps/examples/mmutest/test_aes.c
 *
 * AES-128 CBC TLB benchmark body.  Selected by APP_CONFIG 5.
 ****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/boardctl.h>

#define CBC 1
#define CTR 0
#define ECB 0
#include "aes.h"

#define AES_NBLOCKS  2048            /* 1 KiB working set — sim-friendly */
#define WSBYTES      (AES_NBLOCKS * AES_BLOCKLEN)

static uint8_t g_buf[WSBYTES];

static const uint8_t g_key[16] =
{
  0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
  0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};

static const uint8_t g_iv[16] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

void run_test(void)
{
  struct AES_ctx ctx;
  uint32_t checksum = 0;
  int i;

  printf("mmutest: AES-128 CBC  blocks=%d  ws=%d KiB\n",
         AES_NBLOCKS, WSBYTES / 1024);
  printf("mmutest: EVAL_START=0x%08x EVAL_END=0x%08x\n",
         (unsigned)(uintptr_t)&g_buf[0],
         (unsigned)(uintptr_t)&g_buf[WSBYTES]);
  fflush(stdout);

  for (i = 0; i < WSBYTES; i++)
    {
      g_buf[i] = (uint8_t)(i & 0xff);
    }

  AES_init_ctx_iv(&ctx, g_key, g_iv);
  AES_CBC_encrypt_buffer(&ctx, g_buf, WSBYTES);

  AES_init_ctx_iv(&ctx, g_key, g_iv);
  AES_CBC_decrypt_buffer(&ctx, g_buf, WSBYTES);

  for (i = 0; i < WSBYTES; i++)
    {
      if (g_buf[i] != (uint8_t)(i & 0xff))
        {
          printf("mmutest: FAIL mismatch at byte %d\n", i);
          fflush(stdout);
          return;
        }

      checksum += g_buf[i];
    }

  printf("mmutest: PASS  checksum=0x%08x\n", checksum);
  fflush(stdout);
}
