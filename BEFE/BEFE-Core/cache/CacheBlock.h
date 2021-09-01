//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: CacheBlock.h - Declarations for the CacheBlock Class
//----------------------------------------------------------------------
//
// The CacheBlock Class manages a single instance of a cached Lex Block.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CACHEBLOCK_H // CACHEBLOCK_H...
#define CACHEBLOCK_H

#include "Primitives.h"
#include "Cache.h"
#include "Array.h"
#include "Id.h"

namespace BEFE { // Namespace BEFE...

// Forward Declarations
class  Cache;
struct CacheBlockInfo;

//----------------------------------------------------------------------
//
// CacheBlockType Namespace Enum...
//

namespace CacheBlockType { enum { // CacheBlockType...

  Header,         // File Header
  Free,           // Free BLock
  TypeMap,        // Block Type Map
  
  Level1,         // Level 1 block Map
  Level2,         // Level 2 block Map (***Not implemented yet***)
  Level3,         // Level 3 block Map (***Not implemented yet***)
  
  File,
  String,
  Token,
  
  numTypes
  
}; } // ...CacheBlockType

//----------------------------------------------------------------------
//
// CacheBlock Class...
//

class CacheBlock { // CacheBlock Class...
  
  // Friends
  friend class Cache;
  
  // Members
  public: Boolean  isFree:1;          // "Is this block free?" (exclusive to isUsed)
  public: Boolean  isUsed:1;          // "Is this block used?" (exclusive to isFree)
  public: Boolean  isDirty:1;         // "Is this block dirty?"
  public: UInt     _reserved:29;      // ***Reserved For Future Use***
  public: UInt     blockNo;           // Block number (in File)
  public: UInt     nextBlockIdx;      // Next Block in free/used Chain (block index)
  public: UInt     prevBlockIdx;      // Previous Block in free/used Chain (block index)
  public: Id       fileId;            // File Id (in the Cache)
  public: UInt     nextBlockInFileIdx;// Next Block in File's Chain (block index)
  public: UInt     prevBlockInFileIdx;// Previous Block in File's Chain (block index)
  public: UInt     blockLen;          // Number of used Bytes in Block
  public: Buffer   buffer;            // Associated Block Buffer (block's contents)
  
  // I/O Statistics
  public: Time     lastRead;          // Last date/time Read (if Used)
  public: Time     lastWrite;         // Last date/time Written/Flushed (if Used)
  public: Time     lastModified;      // Last date/time Modified (if Used)
  public: UInt     numLogReads;       // Number of Logical Block reads
  public: UInt     numReadHits;       // Number of Logical Block read hits
  public: UInt     numPhyReads;       // Number of Physical Block reads
  public: UInt     numLogWrites;      // Number of Logical Block reads
  public: UInt     numWriteHits;      // Number of Logical Block read hits
  public: UInt     numPhyWrites;      // Number of Physical Block reads
  
  // C++ Lifecycle
  public: CacheBlock();
  public: CacheBlock(CacheBlock const &that);
  public: ~CacheBlock();
  public: CacheBlock &operator = (CacheBlock const &that);
  
  // BEFE Lifecycle...
  public: Status      StartUp();
  public: Status      ShutDown();
  public: Status      Reset();
  public: Boolean     IsNull() const;
  public: Status      SetNull();
  public: Boolean     IsEmpty() const;
  public: Status      SetEmpty();
  public: Boolean     IsValid() const;
  public: Status      MoveFrom(CacheBlock const &that);
  public: Status      CopyFrom(CacheBlock const &that);
  public: CacheBlock &Consumable() const;
  public: CacheBlock &NotConsumable() const;
  public: Boolean     IsConsumable() const;
  public: UInt        Length() const;  // ◄── Return Number of Bytes in Block
  public: UInt        Size() const;    // ◄── Return Block Size
  public: UInt        PersistSize() const;
  public: Status      WriteTo(Stream &out) const;
  public: Status      ReadFrom(Stream &in);

  // Info
  public: Status      GetBlockInfo(UInt blockNo, CacheBlockInfo &info) const;
  
  // Operational Methods
  public: Strings     ToStrings() const;

  //
  // Protected Methods
  //
  
  // General
  protected: Status  _Allocate(UInt size);    // ◄── Allocate buffer if not already allocated
  protected: Byte   *_GetBuf();               // ◄── Get Block's Buffer Address
  
  // Cache Free/Used Block Chain
  protected: Status  _UnchainFreeUsed();      // ◄── Remove from Free/Used Chain
  protected: Status  _HeadFree();             // ◄── Add to Head of Free Chain
  protected: Status  _TailFree();             // ◄── Add to Tail of Free Chain
  protected: Status  _HeadUsed();             // ◄── Add to Head of Used Chain
  
  // Cache File Block Chain
  protected: Status  _UnchainFile();          // ◄── Remove from File's Chain
  protected: Status  _HeadFile(UInt fileIdx); // ◄── Add to Head of File's Chain
  
}; // ...CacheBlock Class

//----------------------------------------------------------------------
//
// CacheBlock Data Types
//

typedef ArrayValue<CacheBlock> CacheBlocks;

//----------------------------------------------------------------------
//
// CacheBlock Functions
//

BEFE_INLINE Boolean IsNull(CacheBlock const &that)    {return that.IsNull();}
BEFE_INLINE Status  SetNull(CacheBlock &that)         {return that.SetNull();}
BEFE_INLINE Boolean IsEmpty(CacheBlock const &that)   {return that.IsEmpty();}
BEFE_INLINE Status  SetEmpty(CacheBlock &that)        {return that.SetEmpty();}
BEFE_INLINE Status  IsValid(CacheBlock &that)         {return that.IsValid();}
BEFE_INLINE Strings ToStrings(CacheBlock const &that) {return that.ToStrings().Consumable();}  
  
} // ...Namespace BEFE

#endif // ...CACHEBLOCK_H
