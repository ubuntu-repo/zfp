#include <stdlib.h>
#include <string.h>
#include "include/zfp/types.h"
#include "hashBase.c"

// all functions are used to hash 64-bit valued arrays (int64, double)

static uint64
hashArray(const void* arr, int nx, int sx)
{
  uint32 h1 = 0;
  uint32 h2 = 0;

  for (; nx > 0; arr += sx * sizeof(uint64), nx--) {
    uint64 val;
    memcpy(&val, arr, sizeof(uint64));
    hashValue64(val, &h1, &h2);
  }

  uint64 result1 = (uint64)hashFinish(h1);
  uint64 result2 = (uint64)hashFinish(h2);

  return result1 + (result2 << 32);
}

static uint64
hash2dStridedBlock(const void* arr, int sx, int sy)
{
  uint32 h1 = 0;
  uint32 h2 = 0;

  uint x, y;
  for (y = 0; y < 4; arr += (sy - 4*sx) * sizeof(uint64), y++) {
    for (x = 0; x < 4; arr += sx * sizeof(uint64), x++) {
      uint64 val;
      memcpy(&val, arr, sizeof(uint64));
      hashValue64(val, &h1, &h2);
    }
  }

  uint64 result1 = (uint64)hashFinish(h1);
  uint64 result2 = (uint64)hashFinish(h2);

  return result1 + (result2 << 32);
}

static uint64
hash3dStridedBlock(const void* arr, int sx, int sy, int sz)
{
  uint32 h1 = 0;
  uint32 h2 = 0;

  uint x, y, z;
  for (z = 0; z < 4; arr += (sz - 4*sy) * sizeof(uint64), z++) {
    for (y = 0; y < 4; arr += (sy - 4*sx) * sizeof(uint64), y++) {
      for (x = 0; x < 4; arr += sx * sizeof(uint64), x++) {
        uint64 val;
        memcpy(&val, arr, sizeof(uint64));
        hashValue64(val, &h1, &h2);
      }
    }
  }

  uint64 result1 = (uint64)hashFinish(h1);
  uint64 result2 = (uint64)hashFinish(h2);

  return result1 + (result2 << 32);
}
