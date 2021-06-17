//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: CacheFileInfo.h - Declarations for the CacheFileInfo Struct
//----------------------------------------------------------------------
//
// The CacheFileInfo Struct contains a snapshot of a single CacheFile
// state at a given point in time.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CACHEFILEINFO_H // CACHEFILEINFO_H...
#define CACHEFILEINFO_H

#include "Primitives.h"
#include "Array.h"
#include "CacheFile.h"
#include "CacheBlockInfo.h"
#include "Time.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// CacheFileInfo Struct...
//

struct CacheFileInfo { // CacheFileInfo Class...

  //
  // Members
  //
  
  Id      id;               // The CacheFile's Id
  Time    snapTime;         // CacheFileInfo snapshot date/time
  String  fullName;         // CacheFileInfo full path
  Boolean isOpen;           // "Is this CacheFile open/active?"
  UInt    access;           // File Access mode (see File.h)
  UInts   blockNos;         // Array of current Block Numbers (in MRU->LRU order)
  UInt    size;             // Cached File Size in Bytes (Null ⇒ Unknown)
  
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
  CacheFileInfo();
  CacheFileInfo(CacheFileInfo const &that);
  ~CacheFileInfo();
  CacheFileInfo &operator = (CacheFileInfo const &that);
  
  // BEFE Lifecycle...
  Status         StartUp();
  Status         ShutDown();
  Status         Reset();
  Boolean        IsNull() const;
  Status         SetNull();
  Boolean        IsEmpty() const;
  Status         SetEmpty();
  Status         MoveFrom(CacheFileInfo const &that);
  Status         CopyFrom(CacheFileInfo const &that);
  CacheFileInfo &Consumable() const;
  CacheFileInfo &NotConsumable() const;
  Boolean        IsConsumable() const;
  UInt           Length() const;
  UInt           Size() const;

  // Formatting
  Status         Display();
  Status         Display(UInt indent);
  Status         Display(Console &out, UInt indent);
  
  // Protected Methods
  Status         _BuildFrom(CacheFile const &that);
  
}; // ...CacheFileInfo Class

//----------------------------------------------------------------------
//
// CacheFileInfo typedefs
//

typedef ArrayValue<CacheFileInfo> CacheFileInfos;
BEFE_INLINE Boolean  IsNull   (CacheFileInfos const &rThis) {return rThis.IsNull();}
BEFE_INLINE Status   SetNull  (CacheFileInfos &rThis)       {return rThis.SetNull();}

BEFE_INLINE Boolean  IsNull   (CacheFileInfo const &rThis) {return rThis.IsNull();}
BEFE_INLINE Status   SetNull  (CacheFileInfo &rThis)       {return rThis.SetNull();}
BEFE_INLINE Boolean  IsEmpty  (CacheFileInfo const &rThis) {return rThis.IsEmpty();}
BEFE_INLINE Status   SetEmpty (CacheFileInfo &rThis)       {return rThis.SetEmpty();}

} // ...Namespace BEFE

#endif // ...CACHEFILEINFO_H