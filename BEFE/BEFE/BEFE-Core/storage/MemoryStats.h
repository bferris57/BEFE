//!befe-publish inc
//----------------------------------------------------------------------
// File: MemoryStats.h - Declarations for the MemoryStats Storage Class
//----------------------------------------------------------------------
//
// This class is a BEFE <Foundation Class> and is typically for 
// internal use only.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef MEMORYSTATS_H // MEMORYSTATS_H...
#define MEMORYSTATS_H

#include "Primitives.h"
#include "Map.h"

namespace BEFE {

class MemoryStats { // Class MemoryStats...

  //
  // Public Class Members
  //

  public: static  UInt         recursive;
  public: static  UInt         recursiveAllocs;
  public: static  UInt         recursiveFrees;
  public: static  UInt         unloggedAllocs;
  public: static  UInt         unloggedFrees;
  
  //
  // Public Intance Methods
  //

  // Instance C++ Lifecycle
  public: MemoryStats();
  public: ~MemoryStats();
  public: MemoryStats(MemoryStats const &that);
  public: MemoryStats &operator=(MemoryStats const &that);

  // Instance BEFE Lifecycle
  public: Status StartUp();
  public: Status ShutDown();

  // Instance info
  public: UInt   Length();       // Get number of sizes in MemoryStats
  public: UInt   Size();         // Get number of bytes in MemoryStats
  public: UInt   Allocs();       // Get number of allocated blocks in MemoryStats

  // Instance event logging
  public: void   LogMemoryAlloc(Byte *thebytes, UInt len);
  public: void   LogMemoryAllocSize(UInt len);
  public: void   LogMemoryFree(Byte *thebytes);
  public: void   LogMemoryCopy(Byte *src, Byte *dst, UInt len);
  public: void   LogMemoryZero(Byte *thebytes,Int len);
  public: void   LogMemoryFill(Byte *thebytes,Int len,Byte fill);

  // Statistics Printing (displaying)
  public: void   PrintMemoryStatistics();

  //
  // Public Instance Members
  //

  public: UInt   newCounts[32];
  public: UInt   firstNewCount;
  public: UInt   newCount;
  public: UInt   delCount;
  public: UInt   maxCountDiff;   // Maximum difference between newcount and delcount
  public: ULong  newBytes;
  public: ULong  delBytes;
  public: ULong  maxBytesDiff;   // Maximum difference between newbytes and delbytes

  public: UIntMapPrimitive<UInt> sizeCounts;

}; // ...Class MemoryStats

} // ...Namespace BEFE

#endif // ...MEMORYSTATS_H
