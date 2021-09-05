//----------------------------------------------------------------------
// File: MemoryUtils.cpp - Implementation of Memory Utility functions
//----------------------------------------------------------------------
//
// This is the implementation of the BufferStream class.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "MemoryUtils.h"
#include "MemoryStats.h"
#include <new>

//----------------------------------------------------------------------
//
// Operator new/delete
//

void *operator new (size_t size) {
  return (void *)BEFE::MemoryAllocBytes(size);
}

void* operator new[] (size_t size) {

  return (void *)BEFE::MemoryAllocBytes(size);

}

void* operator new[] (size_t size, const std::nothrow_t& nothrow_constant) {

  return BEFE::MemoryAllocBytes(size);

}

void  operator delete (void* ptr) {
  return BEFE::MemoryFreeBytes((BEFE::Byte *)ptr);
}

void  operator delete (void* ptr, const std::nothrow_t& nothrow_constant) {
  return BEFE::MemoryFreeBytes((BEFE::Byte *)ptr);
}

void operator delete[] (void* ptr) {
  return BEFE::MemoryFreeBytes((BEFE::Byte *)ptr);
}

void operator delete[] (void* ptr, const std::nothrow_t& nothrow_constant) {
  return BEFE::MemoryFreeBytes((BEFE::Byte *)ptr);
}

//----------------------------------------------------------------------
//
// Heap (or not) memory allocation
//

namespace BEFE { // Namespace BEFE...

Byte *MemoryAllocBytes(PtrInt size) {

  Byte *thebytes;

  // Use heap if it's there
  // NOTE: We may not need this...
  //if (!IsNull(MemoryHeap::theheap))
  //  thebytes = MemoryHeap::theheap->AllocateBytes(size);
  //else

  // Otherwise, allocate raw bytes
  thebytes = MemoryAllocRaw(size);

  if (size != 0)
    MemoryFillRaw(thebytes,0,size);

  return thebytes;
}

void MemoryFreeBytes(Byte *thebytes) {

  if (!IsNull(thebytes)) {
    // NOTE: We may not need this...
    //if (!IsNull(MemoryHeap::theheap))
    //  MemoryHeap::theheap->FreeBytes(thebytes);
    //else
    MemoryFreeRaw(thebytes);
  }

  return;
}

//----------------------------------------------------------------------
//
// Raw (non-heap) memory allocation and initialisation
//
// Note: The intent here is for malloc/free to be called one and only
//       one place in the code.
//
//       The "raw" functions are for use by MemoryHeap only.
//

Byte *MemoryAllocRaw(UInt size) {

  Byte *thebytes;

  //if (!TheBefe->TheOS) return NULL;

  if (size <= 0)
    BEFE_WARN("MemoryAllocRaw: Invalid memory size requested");

  // NOTE: There seems to be a bug in malloc if the size is too small.  Let's round
  //       it up to the nearest 16...
  size = (size+15)/16*16;

  // Allocate it now
  thebytes = Malloc(size+MemoryInfoSize);
  if (IsNull(thebytes))
    BEFE_WARN("MemoryAllocRaw: Out of memory");

  if (TheBefe)
    TheBefe->TheMemoryStats.LogMemoryAlloc(thebytes,size);

  return thebytes+((thebytes)?MemoryInfoSize:0);
}

void MemoryFreeRaw(Byte *thebytes) {

  if (IsNull(thebytes)) goto DONE;

  // Helpful catch for now
  //if ((Int)thebytes & 0x80000000)
  //  WARN("`'s trying to free 0x80000000 or higher");

  // Do it...
  if (TheBefe)
    TheBefe->TheMemoryStats.LogMemoryFree(thebytes);
  Free(thebytes-MemoryInfoSize);

DONE:

  return;
}

// NOTE: This should go into port specific code so we don't call anything,
//       we do it ourselves.

void MemoryZeroRaw(Byte *thebytes, UInt size) {

  if (TheBefe)
    TheBefe->TheMemoryStats.LogMemoryZero(thebytes,size);
  // Helpful catch
#ifdef IS64BIT
  if ((PtrInt)thebytes == 0xffffffffffffffffll)
    BEFE_WARN("!!!! Somebody's trying to free 0xffffffffffffffff !!!!");
#else
  if ((UInt)thebytes == 0xffffffff)
    BEFE_WARN("!!!! Somebody's trying to free 0xffffffff !!!!");
#endif
  // For now...
  if (!IsNull(thebytes)) while (size > 0) {
    *thebytes++ = 0x00;
    size--;
  }
  return;
}

void MemoryFillRaw(Byte *thebytes, Byte val, UInt size) {
  if (TheBefe)
    TheBefe->TheMemoryStats.LogMemoryFill(thebytes,size,val&0xff);
  // For now...
  if (!IsNull(thebytes)) while (size > 0) {
    *thebytes++ = val;
    size--;
  }
  return;
}

void MemoryCopyRaw(Byte *src, Byte *dst, UInt size) {

  if (src && dst && size)
    Memmove(dst,src,size);

  if (TheBefe)
    TheBefe->TheMemoryStats.LogMemoryCopy(src,dst,size);

  return;
}

Int MemoryCompareRaw(Byte *src, Byte *dst, UInt size) {
  
  while (size) {
    if (*src < *dst) return -1;
    if (*src++ > *dst++) return -1;
    size--;
  }
  
  return 0;
  
}

void  MemoryExchangeRaw(Byte *src, Byte *dst, UInt size) {

  while (size) {
    *dst ^= *src;
    *src ^= *dst;
    *dst++ ^= *src++;  
    size--;
  }
  
  return;
  
}

//----------------------------------------------------------------------
//
// Reversing Memory Order
//

void  MemoryReverseBytes(Byte *start, UInt length) {
  
  Byte *end;
  
  if (!IsNull(length) && length != 0) {
    end = start + length - 1;
    while (end > start) {
      *start   ^= *end;
      *end     ^= *start;
      *start++ ^= *end--;  
    }
  }
  
  return;
  
}

void  MemoryReverseShorts(Short *start, UInt length) {
  
  Short *end;
  
  end = start + length - 1;
  
  while (end > start) {
    *start   ^= *end;
    *end     ^= *start;
    *start++ ^= *end--;  
  }
  
  return;
  
}

void  MemoryReverseInts(Int *start, UInt length) {
  
  Int *end;
  
  end = start + length - 1;
  
  while (end > start) {
    *start   ^= *end;
    *end     ^= *start;
    *start++ ^= *end--;  
  }
  
  return;
  
}

void  MemoryReverseLongs(Long *start, UInt length) {
  
  Long *end;
  
  end = start + length - 1;
  
  while (end > start) {
    *start   ^= *end;
    *end     ^= *start;
    *start++ ^= *end--;  
  }
  
  return;
  
}

//----------------------------------------------------------------------
//
// Out-of-place Reverse Bytes...
//

UInt16 ReverseBytes(UInt16 i) {
  UInt16 answer;
  answer = i;
  MemoryReverseBytes((Byte *)&answer, sizeof(answer));
  return answer;
}
 
UInt32 ReverseBytes(UInt32 i) {
  UInt32 answer;
  answer = i;
  MemoryReverseBytes((Byte *)&answer, sizeof(answer));
  return answer;
}
 
UInt64 ReverseBytes(UInt64 i) {
  UInt64 answer;
  answer = i;
  MemoryReverseBytes((Byte *)&answer, sizeof(answer));
  return answer;
}
 
} // ...Namespace BEFE
