//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: CacheFile.h - Declarations for the CacheFile Class
//----------------------------------------------------------------------
//
// The CacheFile Class manages the information and processing for a
// for a single file in a Cache.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CACHEFILE_H // CACHEFILE_H...
#define CACHEFILE_H

#include "Primitives.h"
#include "FileStream.h"

namespace BEFE { // Namespace BEFE...

class  Cache;
struct CacheFileInfo;

struct CacheFile { // CacheFile Struct...

  // Members
  Id         id;            // File's CacheId
  UInt       openCount;     // Number of times file's concurrently open (maintained by Cache)
  Boolean    isFree:1;      // "Is this file free?" (exclusive to isUsed)
  Boolean    isUsed:1;      // "Is this file used?" (exclusive to isFree)
  Boolean    isWritable:1;  // "Is this file writable?"
  Boolean    ignoreBOM:1;   // "Should we ignore BOM testing?"
  UInt       _reserved:28;  // ***Reserved For Future Use***
  FileStream stream;        // FileStream Accessing the file
  UInt       size;          // Cached File Size in Bytes (Null ⇒ Unknown)
  UInt       nextFileIdx;   // Next used/free file (owner->files index)
  UInt       prevFileIdx;   // Previous used/free file (owner->files index)
  UInt       firstBlockIdx; // First used file block index (owner->blocks index)
  UInt       lastBlockIdx;  // Last used file block index (owner->blocks index)
  Time       lastRead;      // Cache Last Read Time (Null ⇒ Unknown)
  Time       lastModified;  // Cached Last Modified Time (Null ⇒ Unknown)
  Time       lastWrite;     // Cache Last Read Time (Null ⇒ Unknown)
  UInt       numLogReads;   // Number of Logical Block reads
  UInt       numPhyReads;   // Number of Physical Block reads
  UInt       numLogWrites;  // Number of Logical Block reads
  UInt       numPhyWrites;  // Number of Physical Block reads

  // C++ Lifecycle
  CacheFile();
  CacheFile(CacheFile const &that);
  ~CacheFile();
  CacheFile &operator = (CacheFile const &that);
  
  // BEFE Lifecycle...
  Status     StartUp();
  Status     ShutDown();
  Status     Reset();
  Boolean    IsNull() const;
  Status     SetNull();
  Boolean    IsEmpty() const;
  Status     SetEmpty();
  Status     MoveFrom(CacheFile const &that);
  Status     CopyFrom(CacheFile const &that);
  CacheFile &Consumable() const;
  CacheFile &NotConsumable() const;
  Boolean    IsConsumable() const;
  UInt       Length() const;      // ◄── Return Length in Lines NOT Chars
  UInt       Size() const;        // ◄── Return Size in Bytes NOT Lines

  // Info
  Status     GetFileInfo(Cache &owner, CacheFileInfo &theInfo) const;
  
}; // ...CacheFile Struct

//----------------------------------------------------------------------
//
// CacheFile Functions
//

BEFE_INLINE Boolean IsNull(CacheFile const &file) {return file.IsNull();}
BEFE_INLINE Status  SetNull(CacheFile &file)      {return file.SetNull();}

} // ...Namespace BEFE

#endif // ...CACHEFILE_H
