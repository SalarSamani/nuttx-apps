/****************************************************************************
 * apps/system/init/arch_atomic_stubs.c
 *
 * Single-hart Ibex has no A-extension. libgcc would emit CSR-based
 * sequences for __atomic_* that touch sstatus and trap in U-mode.
 * Override those routines with plain memory operations: on a single
 * hart, U-mode threads cannot preempt each other except via timer
 * interrupt, and the kernel masks interrupts during traps, so a
 * non-interrupted load/store is already atomic.
 ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* 1-byte */

uint8_t __atomic_load_1(const volatile void *ptr, int memorder)
{
  (void)memorder;
  return *(const volatile uint8_t *)ptr;
}

void __atomic_store_1(volatile void *ptr, uint8_t val, int memorder)
{
  (void)memorder;
  *(volatile uint8_t *)ptr = val;
}

uint8_t __atomic_exchange_1(volatile void *ptr, uint8_t val, int memorder)
{
  (void)memorder;
  uint8_t old = *(volatile uint8_t *)ptr;
  *(volatile uint8_t *)ptr = val;
  return old;
}

bool __atomic_compare_exchange_1(volatile void *ptr, void *expected,
                                 uint8_t desired, bool weak,
                                 int success, int failure)
{
  (void)weak; (void)success; (void)failure;
  uint8_t cur = *(volatile uint8_t *)ptr;
  if (cur == *(uint8_t *)expected)
    {
      *(volatile uint8_t *)ptr = desired;
      return true;
    }
  *(uint8_t *)expected = cur;
  return false;
}

uint8_t __atomic_fetch_add_1(volatile void *ptr, uint8_t val, int memorder)
{
  (void)memorder;
  uint8_t old = *(volatile uint8_t *)ptr;
  *(volatile uint8_t *)ptr = old + val;
  return old;
}

uint8_t __atomic_fetch_sub_1(volatile void *ptr, uint8_t val, int memorder)
{
  (void)memorder;
  uint8_t old = *(volatile uint8_t *)ptr;
  *(volatile uint8_t *)ptr = old - val;
  return old;
}

uint8_t __atomic_fetch_and_1(volatile void *ptr, uint8_t val, int memorder)
{
  (void)memorder;
  uint8_t old = *(volatile uint8_t *)ptr;
  *(volatile uint8_t *)ptr = old & val;
  return old;
}

uint8_t __atomic_fetch_or_1(volatile void *ptr, uint8_t val, int memorder)
{
  (void)memorder;
  uint8_t old = *(volatile uint8_t *)ptr;
  *(volatile uint8_t *)ptr = old | val;
  return old;
}

uint8_t __atomic_fetch_xor_1(volatile void *ptr, uint8_t val, int memorder)
{
  (void)memorder;
  uint8_t old = *(volatile uint8_t *)ptr;
  *(volatile uint8_t *)ptr = old ^ val;
  return old;
}

/* 2-byte */

uint16_t __atomic_load_2(const volatile void *ptr, int memorder)
{
  (void)memorder;
  return *(const volatile uint16_t *)ptr;
}

void __atomic_store_2(volatile void *ptr, uint16_t val, int memorder)
{
  (void)memorder;
  *(volatile uint16_t *)ptr = val;
}

uint16_t __atomic_exchange_2(volatile void *ptr, uint16_t val, int memorder)
{
  (void)memorder;
  uint16_t old = *(volatile uint16_t *)ptr;
  *(volatile uint16_t *)ptr = val;
  return old;
}

bool __atomic_compare_exchange_2(volatile void *ptr, void *expected,
                                 uint16_t desired, bool weak,
                                 int success, int failure)
{
  (void)weak; (void)success; (void)failure;
  uint16_t cur = *(volatile uint16_t *)ptr;
  if (cur == *(uint16_t *)expected)
    {
      *(volatile uint16_t *)ptr = desired;
      return true;
    }
  *(uint16_t *)expected = cur;
  return false;
}

uint16_t __atomic_fetch_add_2(volatile void *ptr, uint16_t val, int memorder)
{
  (void)memorder;
  uint16_t old = *(volatile uint16_t *)ptr;
  *(volatile uint16_t *)ptr = old + val;
  return old;
}

uint16_t __atomic_fetch_sub_2(volatile void *ptr, uint16_t val, int memorder)
{
  (void)memorder;
  uint16_t old = *(volatile uint16_t *)ptr;
  *(volatile uint16_t *)ptr = old - val;
  return old;
}

uint16_t __atomic_fetch_and_2(volatile void *ptr, uint16_t val, int memorder)
{
  (void)memorder;
  uint16_t old = *(volatile uint16_t *)ptr;
  *(volatile uint16_t *)ptr = old & val;
  return old;
}

uint16_t __atomic_fetch_or_2(volatile void *ptr, uint16_t val, int memorder)
{
  (void)memorder;
  uint16_t old = *(volatile uint16_t *)ptr;
  *(volatile uint16_t *)ptr = old | val;
  return old;
}

uint16_t __atomic_fetch_xor_2(volatile void *ptr, uint16_t val, int memorder)
{
  (void)memorder;
  uint16_t old = *(volatile uint16_t *)ptr;
  *(volatile uint16_t *)ptr = old ^ val;
  return old;
}

/* 4-byte */

uint32_t __atomic_load_4(const volatile void *ptr, int memorder)
{
  (void)memorder;
  return *(const volatile uint32_t *)ptr;
}

void __atomic_store_4(volatile void *ptr, uint32_t val, int memorder)
{
  (void)memorder;
  *(volatile uint32_t *)ptr = val;
}

uint32_t __atomic_exchange_4(volatile void *ptr, uint32_t val, int memorder)
{
  (void)memorder;
  uint32_t old = *(volatile uint32_t *)ptr;
  *(volatile uint32_t *)ptr = val;
  return old;
}

bool __atomic_compare_exchange_4(volatile void *ptr, void *expected,
                                 uint32_t desired, bool weak,
                                 int success, int failure)
{
  (void)weak; (void)success; (void)failure;
  uint32_t cur = *(volatile uint32_t *)ptr;
  if (cur == *(uint32_t *)expected)
    {
      *(volatile uint32_t *)ptr = desired;
      return true;
    }
  *(uint32_t *)expected = cur;
  return false;
}

uint32_t __atomic_fetch_add_4(volatile void *ptr, uint32_t val, int memorder)
{
  (void)memorder;
  uint32_t old = *(volatile uint32_t *)ptr;
  *(volatile uint32_t *)ptr = old + val;
  return old;
}

uint32_t __atomic_fetch_sub_4(volatile void *ptr, uint32_t val, int memorder)
{
  (void)memorder;
  uint32_t old = *(volatile uint32_t *)ptr;
  *(volatile uint32_t *)ptr = old - val;
  return old;
}

uint32_t __atomic_fetch_and_4(volatile void *ptr, uint32_t val, int memorder)
{
  (void)memorder;
  uint32_t old = *(volatile uint32_t *)ptr;
  *(volatile uint32_t *)ptr = old & val;
  return old;
}

uint32_t __atomic_fetch_or_4(volatile void *ptr, uint32_t val, int memorder)
{
  (void)memorder;
  uint32_t old = *(volatile uint32_t *)ptr;
  *(volatile uint32_t *)ptr = old | val;
  return old;
}

uint32_t __atomic_fetch_xor_4(volatile void *ptr, uint32_t val, int memorder)
{
  (void)memorder;
  uint32_t old = *(volatile uint32_t *)ptr;
  *(volatile uint32_t *)ptr = old ^ val;
  return old;
}

/* Generic N-byte variants used by libgcc when size is unknown at compile
 * time. They take a size argument and pointers to memory.
 */

void __atomic_load(size_t size, const volatile void *ptr,
                   void *ret, int memorder)
{
  (void)memorder;
  const volatile uint8_t *src = ptr;
  uint8_t *dst = ret;
  for (size_t i = 0; i < size; i++)
    {
      dst[i] = src[i];
    }
}

void __atomic_store(size_t size, volatile void *ptr,
                    void *val, int memorder)
{
  (void)memorder;
  volatile uint8_t *dst = ptr;
  uint8_t *src = val;
  for (size_t i = 0; i < size; i++)
    {
      dst[i] = src[i];
    }
}

void __atomic_exchange(size_t size, volatile void *ptr,
                       void *val, void *ret, int memorder)
{
  (void)memorder;
  volatile uint8_t *p = ptr;
  uint8_t *v = val;
  uint8_t *r = ret;
  for (size_t i = 0; i < size; i++)
    {
      r[i] = p[i];
      p[i] = v[i];
    }
}

bool __atomic_compare_exchange(size_t size, volatile void *ptr,
                               void *expected, void *desired,
                               int success, int failure)
{
  (void)success; (void)failure;
  volatile uint8_t *p = ptr;
  uint8_t *e = expected;
  uint8_t *d = desired;
  bool match = true;
  for (size_t i = 0; i < size; i++)
    {
      if (p[i] != e[i])
        {
          match = false;
          break;
        }
    }
  if (match)
    {
      for (size_t i = 0; i < size; i++)
        {
          p[i] = d[i];
        }
      return true;
    }
  for (size_t i = 0; i < size; i++)
    {
      e[i] = p[i];
    }
  return false;
}

bool __atomic_is_lock_free(size_t size, const volatile void *ptr)
{
  (void)ptr;
  return size <= 4;
}

void __atomic_thread_fence(int memorder)
{
  (void)memorder;
  __asm__ __volatile__("" ::: "memory");
}

void __atomic_signal_fence(int memorder)
{
  (void)memorder;
  __asm__ __volatile__("" ::: "memory");
}

/* __sync_* legacy builtins. Same logic. */

uint32_t __sync_fetch_and_add_4(volatile void *ptr, uint32_t val)
{
  uint32_t old = *(volatile uint32_t *)ptr;
  *(volatile uint32_t *)ptr = old + val;
  return old;
}

uint32_t __sync_fetch_and_sub_4(volatile void *ptr, uint32_t val)
{
  uint32_t old = *(volatile uint32_t *)ptr;
  *(volatile uint32_t *)ptr = old - val;
  return old;
}

uint32_t __sync_val_compare_and_swap_4(volatile void *ptr,
                                       uint32_t oldval, uint32_t newval)
{
  uint32_t cur = *(volatile uint32_t *)ptr;
  if (cur == oldval)
    {
      *(volatile uint32_t *)ptr = newval;
    }
  return cur;
}

bool __sync_bool_compare_and_swap_4(volatile void *ptr,
                                    uint32_t oldval, uint32_t newval)
{
  uint32_t cur = *(volatile uint32_t *)ptr;
  if (cur == oldval)
    {
      *(volatile uint32_t *)ptr = newval;
      return true;
    }
  return false;
}

void __sync_synchronize(void)
{
  __asm__ __volatile__("" ::: "memory");
}
