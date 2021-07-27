//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: CacheInfo.h - Declarations for the CacheInfo Struct
//----------------------------------------------------------------------
//
// The CacheInfo Struct contains a snapshot of a Cache's information at
// a given point in time.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CACHEINFO_H // CACHEINFO_H...
#define CACHEINFO_H

#include "Primitives.h"
#include "CacheFileInfo.h"
#include "CacheBlockInfo.h"
#include "Time.h"

namespace BEFE { // Namespace BEFE...

// Forward Declarations
struct                             CacheFileInfo;
typedef ArrayValue<CacheFileInfo>  CacheFileInfos;
struct                             CacheBlockInfo;
typedef ArrayValue<CacheBlockInfo> CacheBlockInfos;

//----------------------------------------------------------------------
//
// CacheInfo Struct...
//

struct CacheInfo { // CacheInfo Struct...

  //
  // Members...
  //
  
  Id              id;                 // The Cache's Id
  Time            snapTime;           // CacheInfo snapshot date/time
  UInt            blockSize;          // Block Size (in Bytes)
  UInt            maxBlocks;          // Maximum Blocks
  CacheFileInfos  fileInfos;          // Array of CacheFileInfo, one for each used CacheFile
  CacheBlockInfos blockInfos;         // Array of CacheBlockInfo, one for each CacheBlock (used and free)
  UInts           usedBlockIndexes;   // Array of Used Block Indexes (index into blockInfos)
  UInts           freeBlockIndexes;   // Array of Free Block Index (index into blockInfos)

  // I/O Statistics
  UInt            numDirtyBlocks;     // Current Number of Dirty Blocks
  UInt            numLogReads;        // Number of Logical Block reads
  UInt            numPhyReads;        // Number of Physical Block reads
  UInt            numLogWrites;       // Number of Logical Block reads
  UInt            numPhyWrites;       // Number of Physical Block reads

  //
  // Methods...
  //
  
  // C++ Lifecycle
  CacheInfo();
  CacheInfo(CacheInfo const &that);
  ~CacheInfo();
  CacheInfo &operator = (CacheInfo const &that);
  
  // BEFE Lifecycle...
  Status    StartUp();
  Status    ShutDown();
  Status    Reset();
  Boolean   IsNull() const;
  Status    SetNull();
  Boolean   IsEmpty() const;
  Status    SetEmpty();
  Status    MoveFrom(CacheInfo const &that);
  Status    CopyFrom(CacheInfo const &that);
  CacheInfo &Consumable() const;
  CacheInfo &NotConsumable() const;
  Boolean   IsConsumable() const;
  UInt      Length() const;  // ◄── Return Length in Files
  UInt      Size() const;    // ◄── Return Size in Bytes of all CacheInfo contents
  
  // Formatting
  Status    Display(UInt indent);
  Status    Display(Console &out, UInt indent);
  
  // Protected Methods
  Status     _BuildFrom(Cache const &that);
  
}; // ...CacheInfo Class

} // ...Namespace BEFE

#endif // ...CACHEINFO_H