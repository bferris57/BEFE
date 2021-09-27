//!befe-publish inc
//----------------------------------------------------------------------
// File: MemoryUtils.h - Declarations of various Memory related things
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef MEMORYUTILS_H // MEMUTILS_H...
#define MEMORYUTILS_H

#include "Primitives.h"
#include <cstddef>

//
// Various Bit related functions
//

namespace BEFE { // Namespace BEFE...

// If this contains members, we'll put the info at the beginning of each memory allocation block
class MemoryInfo {
  public: UInt alloccounter;
          UInt size;
};

#define MemoryInfoSize (sizeof(MemoryInfo)/sizeof(PtrInt)*sizeof(PtrInt))

} // ...Namespace BEFE

//
// C++ operator new/delete
//

void* operator new    (size_t size);
void* operator new[]  (size_t size);
//void* operator new[]  (size_t size, const std::nothrow_t& nothrow_constant);
void  operator delete (void* ptr);
void  operator delete (void* ptr, size_t nothrow_constant);
void  operator delete[] (void* ptr);
void  operator delete[] (void* ptr, size_t nothrow_constant);

//
// Memory management
//

namespace BEFE { // Namespace BEFE...

  enum MemoryConstants {
    MEMORY_MINBYTES = 16
  };

  typedef Byte *(*MemoryAllocBytesFunc)(size_t  size);
  typedef void  (*MemoryFreeBytesFunc) (Byte   *thebytes) ;

  // Heap (or not) memory allocation
  Byte *MemoryAllocBytes(size_t size);
  void  MemoryFreeBytes(Byte *thebytes);

  // Raw (non-heap) memory allocation and initialisation
  Byte *MemoryAllocRaw(size_t size);
  void  MemoryFreeRaw(Byte *thebytes);
  void  MemoryZeroRaw(Byte *thebytes, size_t size);
  void  MemoryFillRaw(Byte *thebytes, Byte val, size_t size);
  void  MemoryCopyRaw(Byte *src, Byte *dst, size_t size);
  Int   MemoryCompareRaw(Byte *src, Byte *dst, size_t size);
  void  MemoryExchangeRaw(Byte *src, Byte *dst, size_t size);
  
  // In-place Reversing Memory Order
  void  MemoryReverseBytes(Byte *thebytes, size_t length);   // ◄── Length in Bytes
  void  MemoryReverseShorts(Short *thebytes, size_t length); // ◄── Length in Shorts
  void  MemoryReverseInts(Byte *thebytes, size_t length);    // ◄── Length in Ints
  void  MemoryReverseLongs(Byte *thebytes, size_t length);   // ◄── Length in Longs
  
  // Out-of-place Reverse Bytes...
  UInt16 ReverseBytes(UInt16 i);
  UInt32 ReverseBytes(UInt32 i);
  UInt64 ReverseBytes(UInt64 i);
  
  // Memory statistics
  void PrintMemoryStatistics();
  void PrintMemory(const void *buf, size_t len);
  void PrintMemory(const void *buf, size_t len, const char *indent);
  void PrintMemory(const void *buf, size_t len, Int startoffset, const char *indent);

} // ...Namespace BEFE

//----------------------------------------------------------------------
//
// Tester
//
//----------------------------------------------------------------------

namespace BEFE { // Namespace BEFE...

  class test_memutils {

  public:

    static void test();
    static void test(const char *indent);

  };

} // ...Namespace BEFE

#endif // ...MEMORYUTILS_H
