//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: CacheBlockInfo.h - Declarations for the CacheBlockInfo Struct
//----------------------------------------------------------------------
//
// The CacheBlockInfo Struct contains a snapshot of a single CacheFile
// state at a given point in time.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CACHEBLOCKINFO_H // CACHEBLOCKINFO_H...
#define CACHEBLOCKINFO_H

#include "Primitives.h"
#include "CacheBlock.h"
#include "Array.h"
#include "Time.h"

namespace BEFE { // Namespace BEFE...

// Forward declarations
struct CacheBlock;

//----------------------------------------------------------------------
//
// CacheBlockInfo Struct...
//

struct CacheBlockInfo { // CacheBlockInfo Class...

  //
  // Members
  //
  
  UInt    index;            // The CacheBlock's index in the Cache's block list
  Id      fileId;           // The CacheFile's Id
  UInt    blockNo;          // The CacheFile's Block Number
  Time    snapTime;         // CacheBlockInfo snapshot date/time

  Boolean isFree;           // "Is this block free?" (exclusive to isUsed)
  Boolean isUsed;           // "Is this block used?" (exclusive to isFree)
  Boolean isDirty;          // "Is this block dirty?"
  
  Time    lastRead;         // Cache Last Read Time (Null ⇒ Unknown)
  Time    lastModified;     // Cached Last Modified Time (Null ⇒ Unknown)
  Time    lastWrite;        // Cache Last Read Time (Null ⇒ Unknown)
  
  UInt    numLogReads;      // Number of Logical Block reads
  UInt    numPhyReads;      // Number of Physical Block reads
  UInt    numLogWrites;     // Number of Logical Block reads
  UInt    numPhyWrites;     // Number of Physical Block reads

  //
  // Methods
  //
  
  // C++ Lifecycle
  CacheBlockInfo();
  CacheBlockInfo(CacheBlockInfo const &that);
  ~CacheBlockInfo();
  CacheBlockInfo &operator = (CacheBlockInfo const &that);
  
  // BEFE Lifecycle...
  Status          StartUp();
  Status          ShutDown();
  Status          Reset();
  Boolean         IsNull() const;
  Status          SetNull();
  Boolean         IsEmpty() const;
  Status          SetEmpty();
  Status          MoveFrom(CacheBlockInfo const &that);
  Status          CopyFrom(CacheBlockInfo const &that);
  CacheBlockInfo &Consumable() const;
  CacheBlockInfo &NotConsumable() const;
  Boolean         IsConsumable() const;
  UInt            Length() const;
  UInt            Size() const;
  
  // Protected Methods
  Status          _BuildFrom(CacheBlock const &that);
  
}; // ...CacheBlockInfo Class

//----------------------------------------------------------------------
//
// CacheBlockInfo typedefs and functions
//

typedef ArrayValue<CacheBlockInfo> CacheBlockInfos;
BEFE_INLINE Boolean  IsNull   (CacheBlockInfos const &rThis) {return rThis.IsNull();}
BEFE_INLINE Status   SetNull  (CacheBlockInfos &rThis)       {return rThis.SetNull();}

BEFE_INLINE Boolean  IsNull   (CacheBlockInfo const &rThis) {return rThis.IsNull();}
BEFE_INLINE Status   SetNull  (CacheBlockInfo &rThis)       {return rThis.SetNull();}
BEFE_INLINE Boolean  IsEmpty  (CacheBlockInfo const &rThis) {return rThis.IsEmpty();}
BEFE_INLINE Status   SetEmpty (CacheBlockInfo &rThis)       {return rThis.SetEmpty();}

} // ...Namespace BEFE

#endif // ...CACHEBLOCKINFO_H
