/* 7zCrcOpt.c -- CRC32 calculation : optimized version
2009-11-23 : Igor Pavlov : Public domain */
// from 7-zip on 2010-12-26 Sunday, just after Christmas evening at Music Room with Lucinho at torvon and me at eco - at veriverse - and Mummi and Maninha no quarto da Mummi
#include "framework.h"

#ifdef MY_CPU_LE

#define CRC_UPDATE_BYTE_2(crc, b) (table[((crc) ^ (b)) & 0xFF] ^ ((crc) >> 8))

uint32_t CrcUpdateT4(uint32_t v, const void *data, ::primitive::memory_size size, const uint32_t *table);

uint32_t CrcUpdateT8(uint32_t v, const void *data, ::primitive::memory_size size, const uint32_t *table);

uint32_t CrcUpdateT4(uint32_t v, const void *data, ::primitive::memory_size size, const uint32_t *table)
{
  const Byte *p = (const Byte *)data;
  for (; size > 0 && ((unsigned)(ptrdiff_t)p & 3) != 0; size--, p++)
    v = CRC_UPDATE_BYTE_2(v, *p);
  for (; size >= 4; size -= 4, p += 4)
  {
    v ^= *(const uint32_t *)p;
    v =
      table[0x300 + (v & 0xFF)] ^
      table[0x200 + ((v >> 8) & 0xFF)] ^
      table[0x100 + ((v >> 16) & 0xFF)] ^
      table[0x000 + ((v >> 24))];
  }
  for (; size > 0; size--, p++)
    v = CRC_UPDATE_BYTE_2(v, *p);
  return v;
}

uint32_t CrcUpdateT8(uint32_t v, const void *data, ::primitive::memory_size size, const uint32_t *table)
{
  return CrcUpdateT4(v, data, size, table);
}

#endif
