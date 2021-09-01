//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: Cache.h - Declarations for the Cache Class
//----------------------------------------------------------------------
//
// The Cache Class maintains a memory Buffer Cache of Disk File Blocks.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CACHE_H // CACHE_H...
#define CACHE_H

#include "Primitives.h"
#include "CacheInfo.h"
#include "CacheBlock.h"
#include "CacheBlockInfo.h"
#include "CacheFile.h"
#include "CacheFileInfo.h"
#include "Sparse.h"
#include "Id.h"

namespace BEFE { // Namespace BEFE...

// Forward declarations
struct CacheInfo;
struct CacheBlock;
struct CacheBlockInfo;
struct CacheFile;
struct CacheFileInfo;

// Useful typedefs
typedef StringMapPrimitive<Id> FileIds;
typedef UIntMapPrimitive<UInt> IdIndexes;

//----------------------------------------------------------------------
//
// Block Typedefs
//

typedef ArrayValue<CacheBlock> CacheBlocks;
typedef ArrayValue<CacheFile>  CacheFiles;

//----------------------------------------------------------------------
//
// Cache Class...
//

class Cache { // Cache Class...

  // Friends
  friend class CacheBlock;
  friend class CacheFile;  
  friend class TestCache;
  
  //
  // Class...
  //
  
  protected: static Id highCacheId;

  //
  // Instance Members...
  //

  // Cache...
  protected: Id          id;                // Cache Id
  
  // Files being cached...
  protected: Id          highFileId;        // Highest Id so far
  protected: FileIds     fileNameIds;       // File Name -> Id Map
  protected: IdIndexes   fileIdIndexes;     // File Id -> File Index
  protected: CacheFiles  files;             // Array of CacheFiles
  
  // Used/Free file chain
  protected: UInt        numFiles;          // Total number of Files (used and free)
  protected: UInt        numUsedFiles;      // Number of used Files
  protected: UInt        firstUsedFileIdx;  // First used block (file index)
  protected: UInt        lastUsedFileIdx;   // Last used block (file index)
  protected: UInt        numFreeFiles;      // Number of free Files
  protected: UInt        firstFreeFileIdx;  // First free File (file index)
  protected: UInt        lastFreeFileIdx;   // Last free File (file index)

  // Block Cache
  protected: UInt        blockSize;         // Single Block size
  protected: UInt        maxBlocks;         // Maximum LexBlocks in Cache
  protected: CacheBlocks blocks;            // The Cached LexBlocks
  
  // Block Used/Free chains
  protected: UInt        numUsedBlocks;     // Number of used Blocks
  protected: UInt        firstUsedBlockIdx; // First used block (block index)
  protected: UInt        lastUsedBlockIdx;  // Last used block (block index)
  protected: UInt        numFreeBlocks;     // Number of free Blocks
  protected: UInt        firstFreeBlockIdx; // First free block (block index)
  protected: UInt        lastFreeBlockIdx;  // Last free block (block index)
  
  // I/O Statistics
  protected: UInt        numDirtyBlocks;// Current Number of Dirty Blocks
  protected: UInt        numLogReads;   // Number of Logical Block reads
  protected: UInt        numPhyReads;   // Number of Physical Block reads
  protected: UInt        numLogWrites;  // Number of Logical Block reads
  protected: UInt        numPhyWrites;  // Number of Physical Block reads

  //
  // Instance Methods...
  //

  // C++ Lifecycle
  public: Cache();
  //public: Cache(Cache const &that);
  public: ~Cache();
  //public: Cache &operator = (Cache const &that);
  
  // BEFE Lifecycle...
  public: Status   StartUp();
  public: Status   ShutDown();
  public: Status   Reset();
  public: Boolean  IsNull() const;
  public: Status   SetNull();
  public: Boolean  IsEmpty() const;
  public: Status   SetEmpty();
  public: Status   MoveFrom(Cache const &that);
  public: Status   CopyFrom(Cache const &that);
  public: Cache   &Consumable() const;
  public: Cache   &NotConsumable() const;
  public: Boolean  IsConsumable() const;
  public: UInt     Length() const;  // ◄── Return Length in Files
  public: UInt     Size() const;    // ◄── Return Size in Bytes of all contents

  // File Caching Methods
  public: Status StartCacheFile(char const *fileName);
  public: Status StartCacheFile(String const &fileName);
  public: Status StartCacheFile(String const &fileName, Boolean writable, Boolean ignoreBOM);
  public: Status StopCacheFile(String const &fileName);
  
  // Reading
  Status     ReadStringSpan(Id fileId, Span &span, String &contents);
  Status     ReadBytesSpan(Id fileId, Span &span, Bytes &contents);
  Status     ReadStringRange(Id fileId, Range &span, String &contents);
  Status     ReadBytesRange(Id fileId, Range &span, Bytes &contents);

  // Block Methods
  public: Status LoadFileBlock(Id fileId, UInt blockNo);
  public: Status FlushFileBlock(Id fileId, UInt blockNo);
  //public: Status LatchFileBlock(Id fileId, UInt blockNo);   // ◄── Future: Latch a Block in Cache
  //public: Status UnlatchFileBlock(Id fileId, UInt blockNo); // ◄── Future: Unlatch a latched Cache Block
  //public: Status LockFileBlock(Id fileId, UInt blockNo);    // ◄── Future: Lock a File Block on Disk
  //public: Status UnlockFileBlock(Id fileId, UInt blockNo);  // ◄── Future: Unlock a locked File Block
  public: UInt   GetBlockUsedBytes(Id fileId, UInt blockNo);
  public: Status GetBlockUsedBytes(Id fileId, UInt blockNo, UInt &blockLen);
  public: Status SetBlockUsedBytes(Id fileId, UInt blockNo, UInt blockLen);
  public: Status GetBlockContents(Id fileId, UInt blockNo, UInt offset, Byte *buf, UInt len);
  public: Status SetBlockContents(Id fileId, UInt blockNo, UInt offset, Byte *buf, UInt len);
  public: UInt   GetFileBlockCount(Id fileId);
  public: Status GetFileBlockCount(Id fileId, UInt &numBlocks);
  public: Status AppendFileBlock(Id fileId);
  public: Status AppendFileBlocks(Id fileId, UInt numBlocks);
  
  // Info
  public: CacheInfo      GetInfo();
  public: Status         GetInfo(CacheInfo &that);
  public: Id             GetFileId(String const &fullPath);
  public: Status         GetFileId(String const &fullPath, Id &id);
  public: CacheFileInfo  GetFileInfo(Id fileId);
  public: Status         GetFileInfo(Id fileId, CacheFileInfo &that);
  public: UInt           GetFileSize(Id fileId);
  public: Status         GetFileSize(Id fileId, UInt &size);
  public: UInt           GetBlockSize();
  public: Status         GetBlockSize(UInt &blockSize);
  public: CacheBlockInfo GetBlockInfo(Id fileId, UInt blockNo);
  public: Status         GetBlockInfo(Id fileId, UInt blockNo, CacheBlockInfo &that);

  // Formatting
  //   Implemented in CacheFormat.cpp
  public: Status DisplayOverview();
  public: Status DisplayOverview(UInt indent);
  public: Status DisplayFiles();
  public: Status DisplayFiles(UInt indent);  
  public: Status DisplayUsedBlocks();
  public: Status DisplayUsedBlocks(UInt indent);
  public: Status DisplayFreeBlocks();
  public: Status DisplayFreeBlocks(UInt indent);
  public: Status _DisplayUsedOrFree(Boolean used, UInt indent);
  public: Status DisplayFileBlock(Id fileId, UInt blockNo, UInt indent);
  
  //
  // Protected Methods
  //
  
  protected: Status _Flush();
  protected: Status _FlushFile(UInt fileIdx);
  protected: Status _ReadBlock(UInt blockIdx);
  protected: Status _FlushBlock(UInt blockIdx);
  protected: Status _AllocateBlocks();
  protected: Status _FileIdToIndex(Id fileId, UInt &fileIdx);
  protected: Status _FindUsedFileBlock(UInt fileIdx, UInt blockNo, UInt &blockIdx);
  
  // Public but dangerous since it circumvents undo/redo of block updates...
  public: Byte  *_GetBlockPointer(Id fileId, UInt blockNo); // ◄── Returns transient pointer
  public: Status _SetBlockIsDirty(Id fileId, UInt blockNo); // Flag block as dirty
  
  // Cache Free/Used Block Chain
  //   Implemented in CacheChain.cpp
  protected: Status  _BlockUnchainFreeUsed(UInt blockIdx); // ◄── Remove from Free/Used Chain
  protected: Status  _BlockHeadFree(UInt blockIdx);        // ◄── Add to Head of Free Chain
  protected: Status  _BlockTailFree(UInt blockIdx);        // ◄── Add to Tail of Free Chain
  protected: Status  _BlockHeadUsed(UInt blockIdx);        // ◄── Add to Head of Used Chain
  
  // Cache File Block Chain
  //   Implemented in CacheChain.cpp
  protected: Status  _BlockUnchainFile(UInt blockIdx);     // ◄── Remove from File's Chain
  protected: Status  _BlockHeadFile(UInt fileIdx, UInt blockIdx);// ◄── Add to Head of File's Chain

  // Cache Used/Free File Chain
  //   Implemented in CacheChain.cpp
  protected: Status _FileUnchainFree(UInt fileIdx);   // ◄── Remove from Free File Chain
  protected: Status _FileUnchainUsed(UInt fileIdx);   // ◄── Remove from Used File Chain
  protected: Status _FileHeadFree(UInt fileIdx);      // ◄── Add to Head of Free File Chain
  protected: Status _FileTailFree(UInt fileIdx);      // ◄── Add to Tail of Free File Chain
  protected: Status _FileHeadUsed(UInt fileIdx);      // ◄── Add to Head of Used File Chain
  
}; // ...Cache Class

} // ...Namespace BEFE

#endif // ...CACHE_H
