//!befe-publish inc
//----------------------------------------------------------------------
// File: MemoryUtils.h - Declarations of various Memory related things
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef MEMORYUTILS_H // MEMUTILS_H...
#define MEMORYUTILS_H

#include "Primitives.h"

//
// Various Bit related functions
//

namespace BEFE { // Namespace BEFE...

// If this contains members, we'll put the info at the beginning of each memory allocation block
class MemoryInfo {
  public: UInt alloccounter;
          UInt size;
};

#define MemoryInfoSize (sizeof(MemoryInfo)/sizeof(UInt)*sizeof(UInt))

} // ...Namespace BEFE

//
// C++ operator new/delete
//

//void* operator new    (size_t size);
//void* operator new[]  (size_t size);
//void* operator new[]  (size_t size, const std::nothrow_t& nothrow_constant);
//void  operator delete (void* ptr);
//void  operator delete (void* ptr, const std::nothrow_t& nothrow_constant);
//void  operator delete[] (void* ptr);
//void  operator delete[] (void* ptr, const std::nothrow_t& nothrow_constant);

//
// Memory management
//

namespace BEFE { // Namespace BEFE...

  enum MemoryConstants {
    MEMORY_MINBYTES = 16
  };

  typedef Byte *(*MemoryAllocBytesFunc)(UInt   size);
  typedef void  (*MemoryFreeBytesFunc) (Byte *thebytes) ;

  // Heap (or not) memory allocation
  Byte *MemoryAllocBytes(UInt size);
  void  MemoryFreeBytes(Byte *thebytes);

  // Raw (non-heap) memory allocation and initialisation
  Byte *MemoryAllocRaw(UInt size);
  void  MemoryFreeRaw(Byte *thebytes);
  void  MemoryZeroRaw(Byte *thebytes, UInt size);
  void  MemoryFillRaw(Byte *thebytes, Byte val, UInt size);
  void  MemoryCopyRaw(Byte *src, Byte *dst, UInt size);
  Int   MemoryCompareRaw(Byte *src, Byte *dst, UInt size);
  void  MemoryExchangeRaw(Byte *src, Byte *dst, UInt size);
  
  // In-place Reversing Memory Order
  void  MemoryReverseBytes(Byte *thebytes, UInt length);   // ◄── Length in Bytes
  void  MemoryReverseShorts(Short *thebytes, UInt length); // ◄── Length in Shorts
  void  MemoryReverseInts(Byte *thebytes, UInt length);    // ◄── Length in Ints
  void  MemoryReverseLongs(Byte *thebytes, UInt length);   // ◄── Length in Longs
  
  // Out-of-place Reverse Bytes...
  UInt16 ReverseBytes(UInt16 i);
  UInt32 ReverseBytes(UInt32 i);
  UInt64 ReverseBytes(UInt64 i);
  
  // Memory statistics
  void PrintMemoryStatistics();
  void PrintMemory(const void *buf, UInt len);
  void PrintMemory(const void *buf, UInt len, const char *indent);
  void PrintMemory(const void *buf, UInt len, Int startoffset, const char *indent);

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