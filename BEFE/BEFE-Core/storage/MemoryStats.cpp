//----------------------------------------------------------------------
// File: MemoryStats.cpp - Implementation of the MemoryStats Storage Class
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
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "MemoryStats.h"
#include "ToAscii.h"

namespace BEFE { // Namespace BEFE...

//
// Globals
//

UInt         MemoryStats::recursive       = 0;
UInt         MemoryStats::recursiveAllocs = 0;
UInt         MemoryStats::recursiveFrees  = 0;
UInt         MemoryStats::unloggedAllocs  = 0;
UInt         MemoryStats::unloggedFrees   = 0;
//MemoryStats *MemoryStats::TheMemoryStats  = NULL;

//static MemoryStats theMemoryStats;

//----------------------------------------------------------------------
//
// Class BEFE Lifecycle
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// Instance C++ Lifecycle
//
//----------------------------------------------------------------------


MemoryStats::MemoryStats() {
  StartUp();
}

MemoryStats::~MemoryStats() {
  ShutDown();
}

//----------------------------------------------------------------------
//
// Instance BEFE Lifecycle
//
//----------------------------------------------------------------------

Status MemoryStats::StartUp() {

  Status status;
  UInt   i;

  for (i=0; i < sizeof(newCounts)/sizeof(Int); i++)
    newCounts[i] = 0;
  firstNewCount = UNaN;
  newCount      = 0;
  delCount      = 0;
  maxCountDiff  = 0;
  newBytes      = 0;
  delBytes      = 0;
  maxBytesDiff  = 0;

  // Allocate the array first so it won't appear in the printed stats
  sizeCounts = UIntMapPrimitive<UInt>();

  // Handle errors
  status = Error::None;

  return status;

}

Status MemoryStats::ShutDown() {

  sizeCounts.ShutDown();

  return Error::None;

}

//----------------------------------------------------------------------
//
// C++ Operators...
//
//----------------------------------------------------------------------

MemoryStats &MemoryStats::operator=(MemoryStats const &that) {

  if (this != &that) {
    sizeCounts.ShutDown();
    MemoryCopyRaw((Byte *)&that,(Byte *)this,sizeof(MemoryStats));
    sizeCounts.StartUp();
    sizeCounts = that.sizeCounts;
  }
  return *this;
}

//----------------------------------------------------------------------
//
// Overhead info
//
//----------------------------------------------------------------------

UInt MemoryStats::Length() {
  return sizeCounts.Length();
}

UInt MemoryStats::Size() {

  Int size;

  size = 0;
  if (gLogMemoryAllocSizes)
    size = MemoryInfoSize;
  if (sizeCounts.Size())
    size += sizeCounts.Size() + MemoryInfoSize;

  return size;

}

UInt MemoryStats::Allocs() {

  Int blocks;

  blocks = 0;
  //if (!IsNull(sizeCounts._GetAddress(0)))
  //  blocks++;

  return blocks;
}

//----------------------------------------------------------------------
//
// Memory event logging
//
//----------------------------------------------------------------------

void MemoryStats::LogMemoryAlloc(Byte *thebytes, UInt len) {

  Int   sizeidx;
  UInt  sizek;
  Byte  buf1[ToAscii_MaxNumLen];
  Byte  buf2[ToAscii_MaxNumLen];
  Byte  buf3[ToAscii_MaxNumLen];
  Coord cursor;

  if (!TheBefe || !gLogMemory || recursive) {
    unloggedAllocs++;
    if (recursive)
      recursiveAllocs++;    
    goto DONE;
  }
  
  recursive++;
  newCount++;
  if (firstNewCount == UNaN && gLogMemoryAlloc)
    firstNewCount = newCount;
    
  // Calculate next highest power of two if not already is one
  sizek = RoundToPowerOfTwo(len);
  sizeidx = 0;
  while (sizek) {
    sizeidx++;
    sizek >>= 1;
  }

  // Save the stats
  newCounts[sizeidx]++;
  maxCountDiff = Max(maxCountDiff,(UInt)Abs((Int)newCount-(Int)delCount));
  if (MemoryInfoSize) {
    ((MemoryInfo *)thebytes)->alloccounter = newCount;
    ((MemoryInfo *)thebytes)->size         = len;
    //Cout << "MemoryStats.LogMemoryAlloc: newBytes = " << (void *)delBytes << '\n';
    if ( !IsNull(newCount) && !IsNull(len) )
      newBytes += len;
    maxBytesDiff = Max(maxBytesDiff,(UInt)Abs((Int)newBytes-(Int)delBytes));
  }

  // Log it
  if (gLogMemory && gLogMemoryAlloc && firstNewCount != UNaN) {
    // Save newCount in the block if we're keeping the stats
    if (MemoryInfoSize) {
      ((MemoryInfo *)thebytes)->alloccounter = newCount;
    }
    // Print the message
    if (MemoryInfoSize) {
      ToHexAscii((Int)newCount,                8, buf1);
      ToHexAscii((Int)thebytes+MemoryInfoSize, 8, buf2);
      ToHexAscii(len,                             buf3);
      cursor = Cout.GetCursor();
      if (cursor.x != 0)
        Cout << '\n';
      //Cout << pad1
           //<< (char *)hexbuf1 << " MemoryAllocRaw: Address=0x"
      Cout << "0x"                          << (char *)buf1 
           << " MemoryAllocRaw: Address=0x" << (char *)buf2
           << " (size=0x"                   << (char *)buf3
           << ")\n";
    }
    else {
      ToHexAscii((UInt)thebytes, 8, buf1);
      ToAscii(newCount, buf2);
      ToHexAscii(len, buf3);
      if (!Cout.OutputIsRedirected() && cursor.x != 0)
        Cout << '\n';
      Cout << "0x"                      << (char *)buf1
           << " MemoryAllocRaw: count=" << (char *)buf2
           << " Address = 0x"           << (char *)buf1
           << " (size=0x"               << (char *)buf3 << ")\n";
    }
    Cout.Flush();
  }
  
  if (gLogMemoryAllocSizes)
    LogMemoryAllocSize(len);

  recursive--;

DONE:

  return;

} // MemoryStats::LogMemoryAlloc

void MemoryStats::LogMemoryFree(Byte *thebytes) {

  MemoryInfo *theInfo;
  Byte        buf1[ToAscii_MaxNumLen];
  Byte        buf2[ToAscii_MaxNumLen];
  Byte        buf3[ToAscii_MaxNumLen];
  Coord       cursor;

  if (!TheBefe || !gLogMemory || recursive) {
    unloggedFrees++;
    if (recursive)
      recursiveFrees++;
    goto DONE;
  }
  
  if (delCount >= newCount) {
    //WARN("Somebody's trying to free memory that hasn't been allocated");
    //goto DONE;
  }
  
  delCount++;
  recursive++;

  maxCountDiff = Max(maxCountDiff,(UInt)Abs((Int)newCount-(Int)delCount));

  if (MemoryInfoSize) {
    theInfo = (MemoryInfo *)(thebytes - MemoryInfoSize);
    if ( !IsNull(theInfo->alloccounter) && !IsNull(theInfo->size))
      delBytes += theInfo->size;
  }
  else
    theInfo = NULL;

  // If allocated before first one no need to log it...
  if (theInfo && (firstNewCount == UNaN || theInfo->alloccounter < firstNewCount)) {
    recursive--;
    goto DONE;
  }
  
  // It's okay, log it...
  if (gLogMemory && gLogMemoryFree) {
    if (theInfo) {
      ToHexAscii(theInfo->alloccounter, 8, buf1);
      ToHexAscii((UInt)thebytes,        8, buf2);
      ToHexAscii(theInfo->size,            buf3);
      cursor = Cout.GetCursor();
      if (cursor.x != 0)
        Cout << '\n';
      Cout << "0x"                         << (char *)buf1 
           << " MemoryFreeRaw: Address=0x" << (char *)buf2
           << " (size=0x"                  << (char *)buf3
           << ")\n";
    }
    else {
      ToHexAscii(delCount,    8,  buf1);
      ToAscii((UInt)thebytes, 8, buf2);
      cursor = Cout.GetCursor();
      if (cursor.x != 0)
        Cout << '\n';
      Cout << "0x" << (char *)buf2
           << " MemoryFreeRaw: count=" << (char *)buf1
           << " Address = 0x"          << (char *)buf2
           << '\n';
    }
    Cout.Flush();
  }

  recursive--;

  // Handle errors/flow-breaks
  while (false) {
    DONE: break;
  }

  return;

} // MemoryStats::LogMemoryFree

void MemoryStats::LogMemoryAllocSize(UInt len) {

  Status status;
  UInt   count;
    
  if (!TheBefe || !gLogMemory || !gLogMemoryAllocSizes) goto OK;
  status = sizeCounts.Get(len, count);
  if (status == Error::IndexNotFound)
    count = 0;
  count++;
  status = sizeCounts.Set(len, count);

  // Handle errors  
  while (false) {
    OK: status = Error::None; break;
  }

  return;

} // MemoryStats::LogMemoryAllocSize

void MemoryStats::LogMemoryCopy(Byte *src, Byte *dst, UInt len) {

} // MemoryStatus::LogMemoryCopy

void MemoryStats::LogMemoryZero(Byte *thebytes,Int len) {

} // MemoryStatus::LogMemoryZero

void MemoryStats::LogMemoryFill(Byte *thebytes,Int len,Byte fill) {

} // MemoryStatus::LogMemoryFill

//----------------------------------------------------------------------
//
// Statistics Printing (displaying)
//
//----------------------------------------------------------------------

void MemoryStats::PrintMemoryStatistics() { // MemoryStatus.PrintMemoryStatistics...

  UInt        i;
  char       *unit;
  Int         shift;
  MemoryStats copy;         // <-- We have to take a copy to print consistent results
  UInt        savefg;       // Saved foreground colour
  Byte        buf1[ToAscii_MaxNumLen];
  Byte        buf2[ToAscii_MaxNumLen];
  UInt        numKeys;
  UInts       keys;
  UInt        curIdx;
  UInt        curKey;
  UInt        count;
  UInt        totalCount;
  
  Cout << "Memory Statistics...\n\n";

  savefg = Cout.GetForegroundColour();
  copy = *this;

  //
  // Display General Memory Statistics
  //
  
  Cout << " *** Memory Statistics...\n";
  Cout << '\n';
  
  ToAscii(copy.newCount, 8, buf1);
  ToAscii(copy.delCount, 8, buf2);
  Cout << "    Total Allocations:      " << (char *)buf1 << '\n';
  Cout << "    Total Removals:         " << (char *)buf2 << '\n';
  Cout << "                            --------\n";
  if (copy.newCount != copy.delCount)
    Cout.SetForegroundColour(Console::Yellow);
  Cout << "    Difference:             ";
  ToAscii((Int)copy.newCount-(Int)copy.delCount, 8, buf1);
  Cout << (char *)buf1;
  if (copy.newBytes != copy.delCount)
    Cout.SetForegroundColour(savefg);
  Cout << ((gConsoleUTF8)?"  ◄────":"  <----") << " Always 1 for Console\n";
  Cout << '\n';

  ToAscii(copy.recursiveAllocs, 8, buf1);
  ToAscii(copy.recursiveFrees,  8, buf2);
  Cout << "    Recursive Allocations:  " << (char *)buf1 << '\n';
  Cout << "    Recursive Removals:     " << (char *)buf2 << '\n';
  ToAscii(copy.unloggedAllocs, 8, buf1);
  ToAscii(copy.unloggedFrees,  8, buf2);
  Cout << "    Unlogged  Allocations:  " << (char *)buf1 << '\n';
  Cout << "    Unlogged  Removals:     " << (char *)buf2 << '\n';
  Cout << '\n';
  
  ToAscii(copy.newBytes, 8, buf1);
  Cout << "    Total Bytes Allocated:  " << (char *)buf1 << '\n';
  
  if (MemoryInfoSize) {
    ToAscii(copy.delBytes,                             8, buf1);
    ToAscii((Long)copy.newBytes - (Long)copy.delBytes, 8, buf2);
    Cout << "    Total Bytes Removed:    " << (char *)buf1 << '\n';
    Cout << "                            --------\n";
    if (copy.newBytes != copy.delBytes)
      Cout.SetForegroundColour(Console::Yellow);
    Cout << "    Difference:             ";
    Cout << (char *)buf2;
    if (copy.newBytes != copy.delBytes)
      Cout.SetForegroundColour(savefg);
    Cout << ((gConsoleUTF8)?"  ◄────":"  <----") 
         << " First few bytes used by Console";
    Cout << '\n';
  }
  Cout << '\n';

  ToAscii(copy.maxCountDiff, 8, buf1);
  ToAscii(copy.maxBytesDiff, 8, buf2);
  Cout << "    Max Concurrent Blocks:  " << (char *)buf1
       << ((gConsoleUTF8)?"  ◄─┬──":"  <--+--") << " Maximum after each allocation\n";
  Cout << "    Max Concurrent Bytes:   " << (char *)buf2
       << ((gConsoleUTF8)?"  ◄─┘"   :"  <-/")    << '\n';

  //
  // Display rounded sizes...
  //
  
  Cout << '\n';
  Cout << " *** Rough allocation size counts (total since StartUp)...\n";
  Cout << '\n';
  Cout << "    Size       Count\n";
  Cout << "    ---------- ----------\n";

  totalCount = 0;
  
  for (i=0;i<32;i++) {

    if (copy.newCounts[i] != 0) {
      if (i <= 10) {
        unit  = (char *)" B ";
        shift = 1<<(i-1);
      }
      else if (i <= 20) {
        unit  = (char *)" KB";
        shift = 1<<(i-11);
      }
      else if (i <= 30) {
        unit = (char *)" MB";
        shift = 1<<(i-21);
      }
      else {
        unit = (char *)" GB";
        shift = 1<<(i-31);
      }

      ToAscii(shift,             4, buf1);
      ToAscii(copy.newCounts[i],10, buf2);
      Cout << "    <= " << (char *)buf1 << unit << ' ' << (char *)buf2 << "\n";

      totalCount += copy.newCounts[i];
      
    }

  }

  Cout << "               ----------\n";
  ToAscii(totalCount, 10, buf1);
  Cout << "       Total:  " << (char *)buf1 << '\n';
  
  //
  // Display Size Counts...
  //
  
  if (gLogMemoryAllocSizes && sizeCounts.Length() != 0) {
    
    Cout << '\n';
    Cout << "  *** Distinct allocation size counts (since +mas switched on)...\n";
    Cout << '\n';
    Cout << "    Size       Count\n";
    Cout << "    ---------- ----------\n";

    sizeCounts.Keys(keys);
    numKeys = keys.Length();
    totalCount = 0;
    for (curIdx=0; curIdx < numKeys;curIdx++) {      
      curKey = keys.Get(curIdx);
      count = UNaN;
      sizeCounts.Get(curKey,count);
      ToAscii(curKey, 10, buf1);
      ToAscii(count,  10, buf2);
      Cout << "    " << (char *)buf1 << " " << (char *)buf2 << '\n';
      totalCount += count;
    }
    
    Cout << "               ----------\n";
    ToAscii(totalCount, 10, buf1);
    Cout << "       Total:  " << (char *)buf1 << '\n';

    Cout << '\n';
    Cout << "    Total distinct memory allocation sizes = " << numKeys << '\n';
 
  }
  
  //
  // Handle errors
  //
  
  Cout.SetForegroundColour(savefg);

  return;

} // MemoryStats.PrintMemoryStatistics

} // ...Namespace BEFE
