//----------------------------------------------------------------------
// File: Cache.cpp - Cache Class implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEWrappers.h"
#include "BEFEMacros.h"
#include "Cache.h"
#include "File.h"
#include "FileStream.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class Members
//

Id Cache::highCacheId = Id(0);

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Cache::Cache() {
  //Status status;
  /*status =*/ StartUp();
}

//Cache::Cache(Cache const &that);
Cache::~Cache() {
  //Status status;
  /*status =*/ ShutDown();
}

//Cache::Cache &operator = (Cache const &that);
  
// BEFE Lifecycle...
Status Cache::StartUp() {

  Status retStatus;
  Status status;

  // Cache
  id = highCacheId++;
  
  // Files
  highFileId = 0;
  retStatus = fileNameIds.StartUp();
  status = fileIdIndexes.StartUp();
  if (status && !retStatus) retStatus = status;
  status = files.StartUp();
  if (status && !retStatus) retStatus = status;
  
  // Used/Free file chain
  numFiles = 0;
  BEFE::SetNull(numUsedFiles);
  BEFE::SetNull(firstUsedFileIdx);
  BEFE::SetNull(lastUsedFileIdx);
  BEFE::SetNull(numFreeFiles);
  BEFE::SetNull(firstFreeFileIdx);
  BEFE::SetNull(lastFreeFileIdx);

  // Block Cache
  BEFE::SetNull(blockSize);
  maxBlocks = 50;

  // Block Buffers
  status = blocks.StartUp();
  if (status && !retStatus) retStatus = status;
  
  // Block Free/Used chains
  BEFE::SetNull(numUsedBlocks);
  BEFE::SetNull(firstUsedBlockIdx);
  BEFE::SetNull(lastUsedBlockIdx);
  BEFE::SetNull(numFreeBlocks);
  BEFE::SetNull(firstFreeBlockIdx);
  BEFE::SetNull(lastFreeBlockIdx);
  
  // I/O Statistics
  BEFE::SetNull(numDirtyBlocks);
  BEFE::SetNull(numLogReads);
  BEFE::SetNull(numPhyReads);
  BEFE::SetNull(numLogWrites);
  BEFE::SetNull(numPhyWrites);

  return retStatus;
  
}

Status Cache::ShutDown() {
  
  Status retStatus;
  Status status;
  
  // Flush everything
  retStatus = _Flush();

  // Shut down CacheFiles...
  status = fileNameIds.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = fileIdIndexes.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = files.ShutDown();
  if (status && !retStatus) retStatus = status;
  
  // Shut down Blocks...
  status = blocks.ShutDown();
  
  return status;

}

Status Cache::Reset() {

  Status status;
  Id     saveId;
  Id     saveHighCacheId;
  
  saveId = id;
  saveHighCacheId = highCacheId;
  status = ShutDown();
  if (!status)
    status = StartUp();
  id = saveId;
  highCacheId = saveHighCacheId;
  
  return status;

}

Boolean Cache::IsNull() const {
  return BEFE::IsNull(maxBlocks) || maxBlocks < 2 ||
         BEFE::IsNull(blockSize) || blocks.Length() == 0;
}

Status Cache::SetNull() {
  return Reset();
}

Boolean Cache::IsEmpty() const {
  return IsNull();
}

Status Cache::SetEmpty() {
  return Error::NotImplemented;
}

Status Cache::MoveFrom(Cache const &that) {
  return Error::NotImplemented;
}

Status Cache::CopyFrom(Cache const &that) {
  return Error::NotImplemented;
}

Cache &Cache::Consumable() const {
  return *((Cache *)this);
}

Cache &Cache::NotConsumable() const {
  return *((Cache *)this);
}

Boolean Cache::IsConsumable() const {
  return false;
}

UInt Cache::Length() const {
  return UNaN;
}

UInt Cache::Size() const {
  return UNaN;
}

//----------------------------------------------------------------------
//
// Operational Methods
//

Status Cache::StartCacheFile(String const &fileName) {
  return StartCacheFile(fileName, false, false);
}

Status Cache::StartCacheFile(String const &fileName, Boolean writable, Boolean ignoreBOM) {
  
  Status     status;
	Id         fileId;
  CacheFile  empty;
  CacheFile *theFiles;
  CacheFile *theFile;
  UInt       fileIdx;
  
  NEED_TheBefe
  NEED_TheOS
  
  // Validate file name...
  if (fileName.Length() == 0) goto NONAME;
  if (!IsFullPath(fileName)) goto NOTFULLPATH;
  if (!Exists(fileName)) goto NOTEXISTS;
  if (!IsFile(fileName)) goto NOTFILE;
  
  // If already cached, increment CacheFile.openCount...
  if (fileNameIds.HasKey(fileName)) {
  	fileId = fileNameIds.Get(fileName);
    status = _FileIdToIndex(fileId, fileIdx);
    if (status) goto SOMEERROR;
    theFiles = (CacheFile *)files._GetAddress(0);
    if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
    theFile = theFiles+fileIdx;
    if (BEFE::IsNull(theFile->openCount))
    	theFile->openCount = 1;
    else
    	theFile->openCount++;
    goto OK;
  }

  // Make a free Files entry if there aren't any...
  if (BEFE::IsNull(lastFreeFileIdx)) {
    status = files.Append(empty);
    if (status) goto SOMEERROR;
    fileIdx = files.Length()-1;
    numFiles++;
    status = _FileHeadFree(fileIdx);
    if (status) goto SOMEERROR;
    if (BEFE::IsNull(numFreeFiles))
      numFreeFiles = 0;
    numFreeFiles++;
  }
  
  // Take the head free one...
  fileIdx = firstFreeFileIdx;
  if (BEFE::IsNull(fileIdx)) goto NOFREE;
  status = _FileUnchainFree(fileIdx);
  if (status) goto SOMEERROR;
  numFreeFiles--;
  status = _FileHeadUsed(fileIdx);
  if (status) goto SOMEERROR;
  if (BEFE::IsNull(numUsedFiles))
    numUsedFiles = 0;
  numUsedFiles++;
  
  // Allocate new Id and add it into the Maps...
  ++highFileId;
  status = fileIdIndexes.Set(highFileId, fileIdx);
  if (status) goto SOMEERROR;
  status = fileNameIds.Set(fileName, highFileId);
  if (status) goto SOMEERROR;

  // Populate the CacheFile's info...
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  theFiles[fileIdx].id         = highFileId;
  theFiles[fileIdx].isUsed     = true;
  theFiles[fileIdx].isWritable = writable;
  status = theFiles[fileIdx].stream.SetName(fileName);
  if (status) goto SOMEERROR;
  if (writable) {
    status = theFiles[fileIdx].stream.SetAccess(File::ModeRead | File::ModeWrite);
    if (status) goto SOMEERROR;
  }
  if (ignoreBOM)
    theFiles[fileIdx].ignoreBOM = true;
  
  // Handle errors
  while (true) {
    OK:          status = Error::None;                   break;
    NONAME:      status = Error::CacheNoFileName;        break;
    NOTFULLPATH: status = Error::CacheExpectedFullPath;  break;
    NOTEXISTS:   status = Error::FileDoesNotExist;       break;
    NOTFILE:     status = Error::FileNameIsNotFile;      break;
    //ALREADY:     status = Error::CacheFileAlreadyCached; break;
    NOFREE:      status = Error::CacheFileNoFreeFiles;   break;
    NULLPOINTER: status = Error::InternalNullPointer;    break;
    SOMEERROR:                                           break;
  }
  
  return status;

}

Status Cache::StopCacheFile(String const &fileName) {
  
  Status      status;
  CacheFile  *theFiles;
  CacheFile  *theFile;
  Id          fileId;
  UInt        fileIdx;
  CacheBlock *theBlocks;
  CacheBlock *theBlock;
  UInt        blockIdx;
  
  NEED_TheBefe
  NEED_TheOS

  // Get the associated CacheFile's Id...
  status = GetFileId(fileName, fileId);
  if (status) goto SOMEERROR;
  status = _FileIdToIndex(fileId, fileIdx);
  if (status) goto SOMEERROR;
  
  // Set up the CacheFile index and the Blocks...
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  theFile = theFiles+fileIdx;

  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;

  // Flush the blocks if they need it, and free them...
  while (!BEFE::IsNull(theFile->firstBlockIdx)) {
    blockIdx = theFile->firstBlockIdx;
    theBlock = theBlocks + blockIdx;
    // Flush it...
    if (theBlock->isDirty) {
      status = _FlushBlock(blockIdx);
      if (status) goto SOMEERROR;
    }
    // Free it...
    status = _BlockUnchainFile(blockIdx);
    if (status) goto SOMEERROR;
    status = _BlockUnchainFreeUsed(blockIdx);
    if (status) goto SOMEERROR;    
    status = _BlockTailFree(blockIdx);
    if (status) goto SOMEERROR;    
  }
  
  // If CacheFile has openCount, decrement it and only continue if no more open...
  if (!BEFE::IsNull(theFile->openCount)) {
    if (theFile->openCount &&--theFile->openCount != 0)
    	goto OK;
    BEFE::SetNull(theFile->openCount);
  }

  // Clean out all info about it...
  status = fileNameIds.Remove(fileName);
  if (status && fileName.Length()) goto SOMEERROR;
  status = fileIdIndexes.Remove(fileId);
  if (status) goto SOMEERROR;
  status = theFile->Reset();
  if (status) goto SOMEERROR;
  
  // Remove fkile from Used chain and put on end of Free File Chain...
  status = _FileUnchainUsed(fileIdx);
  if (status) goto SOMEERROR;
  numUsedFiles--;
  status = _FileTailFree(fileIdx);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (true) {
  	OK:          status = Error::None;                   break;
    NULLPOINTER: status = Error::InternalNullPointer;    break;
    SOMEERROR:                                           break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Read Methods
//

// Note: Cache::ReadStringSpan currently doesn't properly handle reverse spans.
//       Expect to change this in the future.
//
//       Also, expect to use Cache::ReadBytesSpan instead and do our hand
//       waving on the String contents (e.g., reversing Bytes is easy,
//       reversing a String has to account for UTF-8 characters).
//

Status Cache::ReadStringSpan(Id fileId, Span &span, String &contents) {

  Status      status;
  UInt        fileIdx;
  CacheFile  *theFiles;
  CacheFile  *theFile;
  Span        normSpan;
  UInt        curBlkNo;
  Byte       *curBlk;
  UInt        block0;
  UInt        offset0;
  UInt        blockN;
  UInt        offsetN;
  UInt        movOffset;
  UInt        movLen;

  // Clear contents before we do anything else
  status = contents.Reset();
  if (status) goto SOMEERROR;

  // Validate fileId
  status = _FileIdToIndex(fileId, fileIdx);
  if (status) goto SOMEERROR;
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  theFile = theFiles + fileIdx;
  if (!theFile->isUsed) goto FILESTALE;

  // Get File Size if we don't already know it
  if (BEFE::IsNull(theFile->size)) {
    status = GetFileSize(fileId, theFile->size);
    if (status) goto SOMEERROR;
    if (BEFE::IsNull(theFile->size)) goto BADSPAN;
  }

  // Normalise the span and turn into block numbers and offsets...
  normSpan = span.Resolve(theFile->size);
  if (normSpan.IsDescending()) {
  	status = normSpan.Reverse();
  	if (status) goto SOMEERROR;
  }
  block0  = normSpan.idx1/blockSize;
  offset0 = normSpan.idx1-block0*blockSize;
  blockN  = normSpan.idx2/blockSize;
  offsetN = normSpan.idx2-blockN*blockSize;

  // Get the Span contents...
  //
  // Note: This isn't the best way to do it so we'll have to rewrite it later. Should
  //       be using Bytes and ReadStringSpan should just call ReadStringBytes

  for (curBlkNo=block0; curBlkNo <= blockN; curBlkNo++) {

  	// Read current block
  	status = LoadFileBlock(fileId, curBlkNo);
  	if (status) goto SOMEERROR;

  	// Get its address...
  	curBlk =_GetBlockPointer(fileId, curBlkNo);
  	if (BEFE::IsNull(curBlk)) goto NULLPOINTER;

  	// If block0 and blockN are the same, it's easy...
  	if (block0 == blockN) {
  		curBlk += offset0;
  		status = contents.Append(String(curBlk,offsetN-offset0));
  		if (status) goto SOMEERROR;
  		break;
  	}

  	// Spans blocks, append to contents...
    if (curBlkNo == block0) {       // Block0, start of offset0 to end of block...
    	movOffset = offset0;
    	movLen    = blockSize - offset0;
    }
    else if (curBlkNo != blockN) { // Middle block, entire block...
    	movOffset = 0;
    	movLen    = blockSize;
    }
    else {                       // End block, 0 to offsetN...
      movOffset = 0;
      movLen    = offsetN;
    }
    status = contents.Append(String(curBlk+movOffset,movLen));
    if (status) goto SOMEERROR;

  } // ...for

  // Handle errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;  break;
    FILESTALE:   status = Error::CacheFileIdStale;     break;
    BADSPAN:     status = Error::CacheBadSpan;         break;
    SOMEERROR:                                         break;
  }

	return status;

}

Status Cache::ReadBytesSpan(Id fileId, Span &span, Bytes &contents)	{
	return Error::NotImplemented;
}

Status Cache::ReadStringRange(Id fileId, Range &span, String &contents) {
	return Error::NotImplemented;
}

Status Cache::ReadBytesRange(Id fileId, Range &span, Bytes &contents) {
	return Error::NotImplemented;
}

//----------------------------------------------------------------------
//  
// Block Methods
//

Status Cache::LoadFileBlock(Id fileId, UInt blockNo) { // Cache.LoadFileBlock...

  Status      status;
  UInt        fileIdx;
  CacheFile  *theFiles;
  CacheFile  *theFile;
  CacheBlock *theBlocks;
  CacheBlock *theBlock;
  UInt        numBlocks;
  UInt        ourBlockIdx;
  Byte       *buf;
  UInt        numRead;
  Boolean     wasThere;
  
  NEED_TheBefe
  NEED_TheOS

  // Allocate blocks if not alrady allocated
  status = _AllocateBlocks();
  if (status) goto SOMEERROR;
  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;
  
  // Validate fileId
  status = _FileIdToIndex(fileId, fileIdx);
  if (status) goto SOMEERROR;
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  theFile = theFiles + fileIdx;
  if (!theFile->isUsed) goto FILESTALE;
  
  // Get File Size if we don't already know it, and validate blockNo
  if (BEFE::IsNull(theFile->size)) {
    status = GetFileSize(fileId, theFile->size);
    if (status) goto SOMEERROR;
    if (BEFE::IsNull(theFile->size)) goto BADBLOCKNO;
  }
  numBlocks = (theFile->size+blockSize-1)/blockSize;
  if (blockNo >= numBlocks) goto BADBLOCKNO;

  // Update logical Statistics
  if (BEFE::IsNull(numLogReads))
    numLogReads = 0;
  numLogReads++;
  if (BEFE::IsNull(theFile->numLogReads))
    theFile->numLogReads = 0;
  theFile->numLogReads++;
  
  // See if already in Cache...
  status = _FindUsedFileBlock(fileIdx, blockNo, ourBlockIdx);
  if (status) goto SOMEERROR;

  // Read it if not already in Cache...
  if (BEFE::IsNull(ourBlockIdx)) { // Read it...
  
    wasThere = false;
    
    if (numFreeBlocks == 0) { // None Free, take Tail Used...
    
      // Flush first
      status = _Flush();
      if (status) goto SOMEERROR;

      // Take tail block...
      if (BEFE::IsNull(lastUsedBlockIdx)) goto USEDEMPTY;
      ourBlockIdx = lastUsedBlockIdx;
      theBlock = theBlocks + ourBlockIdx;
      if (theBlock->isDirty) {
        status = _FlushBlock(ourBlockIdx);
        if (status) goto SOMEERROR;
      }
      status = _BlockUnchainFile(ourBlockIdx);
      if (status) goto SOMEERROR;

    } // ...None Free, take Tail Used

    else { // Some Free, Use Head Free...
      
      // Use the Free Head...
      ourBlockIdx = firstFreeBlockIdx;
      if (BEFE::IsNull(ourBlockIdx)) goto INTERNAL;
    
    }  // ...Some Free, Use Head Free

  } // ...Read it
  
  // Was already there...
  else { // Unchain from file blocks...
    status = _BlockUnchainFile(ourBlockIdx);
    if (status) goto SOMEERROR;
    wasThere = true;
  }
  
  // Remove it from the Free/Used Block Chain...
  theBlock = theBlocks+ourBlockIdx;
  status = _BlockUnchainFreeUsed(ourBlockIdx);
  if (status) goto SOMEERROR;
  status = _BlockHeadUsed(ourBlockIdx);
  if (status) goto SOMEERROR;
  status = _BlockHeadFile(fileIdx, ourBlockIdx);
  if (status) goto SOMEERROR;
    
  // Update its info
  theBlock->blockNo = blockNo;
  
  // If it wasn't already there...
  if (!wasThere) {

    // Until we read it, we'll treat it as unknown length...
    BEFE::SetNull(theBlock->blockLen);
    
    // Read it...
    buf = theBlock->_GetBuf();
    if (BEFE::IsNull(buf)) goto NULLPOINTER;
    BEFE::SetNull(theBlock->blockLen);
    status = theFile->stream.SetPos(blockNo*blockSize);
    if (status) goto SOMEERROR;
    status = theFile->stream.GetBytes(buf, blockSize, numRead);
    if (status) goto SOMEERROR;
    if (numRead == 0) goto SOMEERROR;
    theBlock->blockLen = numRead;
    status = theBlock->buffer.SetUsedBytes(numRead);
    if (status) goto SOMEERROR;
  
    // Update Physical I/O statistics...
    if (BEFE::IsNull(numPhyReads))
      numPhyReads = 0;
    numPhyReads++;
    if (BEFE::IsNull(theFile->numPhyReads))
      theFile->numPhyReads = 0;
    theFile->numPhyReads++;
    theFile->lastRead.Set();

    // If Block 0, validate any leading BOM...
    if (blockNo == 0 && !theFile->ignoreBOM) {
      status = theFile->stream.file.ValidateBOM(theBlock->buffer.GetBuf(), theBlock->blockLen);
      if (status && status != Error::CommandBOM) goto SOMEERROR;
      status = Error::None;
    }
  
  }

  // Handle errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;  break;
    FILESTALE:   status = Error::CacheFileIdStale;     break;
    BADBLOCKNO:  status = Error::CacheBadBlockNo;      break;
    INTERNAL:    status = Error::Internal;             break;
    USEDEMPTY:   status = Error::CacheBlockUsedEmpty;  break;
    SOMEERROR:                                         break;
  }
  
  return status;

} // ...Cache.LoadFileBlock

Status Cache::FlushFileBlock(Id fileId, UInt blockNo) { // Cache.FlushFileBlock...

  Status      status;
  UInt        fileIdx;
  CacheFile  *theFiles;
  CacheFile  *theFile;
  CacheBlock *theBlocks;
  CacheBlock *theBlock;
  UInt        numBlocks;
  UInt        ourBlockIdx;
  Byte       *buf;
  UInt        numWritten;
  
  NEED_TheBefe
  NEED_TheOS

  // Allocate blocks if not alrady allocated
  status = _AllocateBlocks();
  if (status) goto SOMEERROR;
  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;
  
  // Validate fileId
  status = _FileIdToIndex(fileId, fileIdx);
  if (status) goto SOMEERROR;
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  theFile = theFiles + fileIdx;
  if (!theFile->isUsed) goto FILESTALE;
  
  // Get File Size if we don't already know it, and validate blockNo
  if (BEFE::IsNull(theFile->size)) {
    status = GetFileSize(fileId, theFile->size);
    if (status) goto SOMEERROR;
    if (BEFE::IsNull(theFile->size)) goto BADBLOCKNO;
  }
  numBlocks = (theFile->size+blockSize-1)/blockSize;
  if (blockNo >= numBlocks) goto BADBLOCKNO;

  // See if already in Cache...
  status = _FindUsedFileBlock(fileIdx, blockNo, ourBlockIdx);
  if (status) goto SOMEERROR;

  // If not in Cache or not dirty, no need to write it...
  if (BEFE::IsNull(ourBlockIdx)) goto OK;
  theBlock = &theBlocks[ourBlockIdx];
  if (!theBlock->isDirty) goto OK;
  
  //
  // Need to write it...
  //
  
  buf = theBlock->_GetBuf();
  if (BEFE::IsNull(buf)) goto NULLPOINTER;
  BEFE::SetNull(theBlock->blockLen);
  status = theFile->stream.SetPos(blockNo*blockSize);
  if (status) goto SOMEERROR;
  status = theFile->stream.PutBytes(buf, blockSize, numWritten);
  if (status) goto SOMEERROR;
  if (numWritten == 0) goto SOMEERROR;
  theBlock->blockLen = numWritten;
  status = theBlock->buffer.SetUsedBytes(numWritten);
  if (status) goto SOMEERROR;
  theBlocks[ourBlockIdx].isDirty = false;
  
  // Update Physical I/O statistics...
  if (BEFE::IsNull(numPhyWrites))
    numPhyWrites = 0;
  numPhyWrites++;
  if (BEFE::IsNull(theFile->numPhyWrites))
    theFile->numPhyWrites = 0;
  theFile->numPhyWrites++;
  theFile->lastWrite.Set();

  // Handle errors
  while (false) {
    OK:          status = Error::None;                 break;
    NULLPOINTER: status = Error::InternalNullPointer;  break;
    FILESTALE:   status = Error::CacheFileIdStale;     break;
    BADBLOCKNO:  status = Error::CacheBadBlockNo;      break;
    SOMEERROR:                                         break;
  }
  
  return status;


} // ...Cache.FlushFileBlock

//Status LockBlock(Id fileId, UInt blockNo) {
//  return Error::NotImplemented;
//}

//Status UnlockBlock(Id fileId, UInt blockNo) {
//  return Error::NotImplemented;
//}

UInt Cache::GetBlockUsedBytes(Id fileId, UInt blockNo) {
  
  Status status;
  UInt   usedBytes;
  
  status = GetBlockUsedBytes(fileId, blockNo, usedBytes);
  if (status)
    BEFE::SetNull(usedBytes);
    
  return usedBytes;
  
}

Status Cache::GetBlockUsedBytes(Id fileId, UInt blockNo, UInt &blockLen) {

  Status      status;
  UInt        fileIdx;
  UInt        blockIdx;
  CacheBlock *theBlocks;
  
  NEED_TheBefe
  NEED_TheOS

  status = _FileIdToIndex(fileId, fileIdx);
  if (status) goto SOMEERROR;  
  status = _FindUsedFileBlock(fileIdx, blockNo, blockIdx);
  if (status) goto SOMEERROR;
  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;
  blockLen = theBlocks[blockIdx].blockLen;
  
  // Handle errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;

}

Status Cache::SetBlockUsedBytes(Id fileId, UInt blockNo, UInt blockLen) {

  NEED_TheBefe
  NEED_TheOS
  
  return Error::NotImplemented;

}

Status Cache::GetBlockContents(Id fileId, UInt blockNo, UInt offset, Byte *buf, UInt len) {

  Status      status;
  UInt        fileIdx;
  UInt        blockIdx;
  CacheBlock *theBlocks;
  UInt        blockLen;
  UInt        readLen;
  
  NEED_TheBefe
  NEED_TheOS

  status = _FileIdToIndex(fileId, fileIdx);
  if (status) goto SOMEERROR;  
  status = _FindUsedFileBlock(fileIdx, blockNo, blockIdx);
  if (status) goto SOMEERROR;
  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;
  
  // Load the block if not already there...
  if (BEFE::IsNull(blockIdx)) {
    status = LoadFileBlock(fileId, blockNo);
    if (status) goto SOMEERROR;
    status = _FindUsedFileBlock(fileIdx, blockNo, blockIdx);
    if (status) goto SOMEERROR;
    if (BEFE::IsNull(blockIdx)) goto INTERNAL;
  }
  blockLen = theBlocks[blockIdx].blockLen;
  
  // Make sure they're not asking for something out of the block's bytes...
  if (BEFE::IsNull(offset) ||
      BEFE::IsNull(len)    ||
      (offset + len) > blockLen
     ) goto TRUNCATED;
     
  // Give the bytes to them
  status = theBlocks[blockIdx].buffer.GetBytes(offset, buf, len, readLen);
  if (status) goto SOMEERROR;
  if (readLen != len) goto TRUNCATED;
  
  // Handle errors
  while (false) {
    TRUNCATED:   status = Error::ValueTruncated;      break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    INTERNAL:    status = Error::Internal;            break;
    SOMEERROR:                                        break;
  }
  
  return status;

}

Status Cache::SetBlockContents(Id fileId, UInt blockNo, UInt offset, Byte *buf, UInt len) {

  Status      status;
  UInt        fileIdx;
  CacheFile  *theFile;
  UInt        blockIdx;
  CacheBlock *theBlocks;
  UInt        blockLen;
  UInt        writeLen;
  
  NEED_TheBefe
  NEED_TheOS

  status = _FileIdToIndex(fileId, fileIdx);
  if (status) goto SOMEERROR;  
  theFile = (CacheFile *)files._GetAddress(fileIdx);
  if (BEFE::IsNull(theFile)) goto NULLPOINTER;
  status = _FindUsedFileBlock(fileIdx, blockNo, blockIdx);
  if (status) goto SOMEERROR;

  // If the block's not there, load it...
  if (BEFE::IsNull(blockIdx)) {
    status = LoadFileBlock(fileId, blockNo);
    if (status) goto SOMEERROR;
    status = _FindUsedFileBlock(fileIdx, blockNo, blockIdx);
    if (status) goto SOMEERROR;
    if (BEFE::IsNull(blockIdx)) goto NOTFOUND;
  }
  
  // Get the block's address
  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;
  blockLen = theBlocks[blockIdx].blockLen;

  // Make sure it was open for write...
  if (!theFile->isWritable) goto NOTWRITABLE;
    
  // Make sure they're not asking for something out of the block's bytes...
  if (BEFE::IsNull(offset) ||
      BEFE::IsNull(len)    ||
      (offset + len) > blockLen
     ) goto TRUNCATED;
     
  // Set the bytes
  status = theBlocks[blockIdx].buffer.SetBytes(offset, buf, len, writeLen);
  if (status) goto SOMEERROR;
  if (writeLen != len) goto TRUNCATED;
  theBlocks[blockIdx].isDirty = true;
  
  // Update logical Statistics
  if (BEFE::IsNull(numLogWrites))
    numLogWrites = 0;
  numLogWrites++;
  if (BEFE::IsNull(theFile->numLogWrites))
    theFile->numLogWrites = 0;
  theFile->numLogWrites++;
  
  // Handle errors
  while (false) {
    TRUNCATED:   status = Error::ValueTruncated;      break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    NOTWRITABLE: status = Error::FileNotOpenForWrite; break;
    NOTFOUND:    status = Error::CacheBlockNotUsed;   break;
    SOMEERROR:                                        break;
  }
  
  return status;

}

UInt Cache::GetFileBlockCount(Id fileId) {
  Status status;
  UInt   numBlocks;
  status = GetFileBlockCount(fileId, numBlocks);
  if (status)
    BEFE::SetNull(numBlocks);
  return numBlocks;
}

Status Cache::GetFileBlockCount(Id fileId, UInt &numBlocks) {

  Status     status;
  CacheFile *theFiles;
  UInt       fileIdx;
  UInt       numBytes;
  
  NEED_TheBefe
  NEED_TheOS
  
  if (BEFE::IsNull(blockSize) || blockSize == 0) goto NOBLOCKSIZE;

  status = _FileIdToIndex(fileId, fileIdx);
  if (status) goto SOMEERROR;
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  numBytes = theFiles[fileIdx].stream.Size();
  if (BEFE::IsNull(numBytes))
    numBytes = 0;
  numBlocks = (numBytes+blockSize-1)/blockSize;
  
  // Handle errors
  while (false) {
    NOBLOCKSIZE: status = Error::CacheBlkSizeNotSet;  break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

Status Cache::AppendFileBlock(Id fileId) {
  return AppendFileBlocks(fileId, 1);
}

Status Cache::AppendFileBlocks(Id fileId, UInt numBlocks) {

  Status     status;
  CacheFile *theFiles;
  CacheFile *theFile;
  UInt       fileIdx;
  UInt       numBytes;
  UInt       numToWrite;
  UInt       numWritten;
  Byte       empty[1024];
  
  NEED_TheBefe
  NEED_TheOS
  
  if (BEFE::IsNull(blockSize) || blockSize == 0) goto NOBLOCKSIZE;

  // Get the file info
  status = _FileIdToIndex(fileId, fileIdx);
  if (status) goto SOMEERROR;
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  theFile = &theFiles[fileIdx];
  numBytes = theFile->stream.Size();

  // Initialise empty chunk
  MemoryFillRaw(empty, 0xff, sizeof(empty));

  // Position to end of file
  status = theFile->stream.SetPos(numBytes);
  if (status) goto SOMEERROR;
  
  // Compute number of Bytes to pad, padding with extra if we're not
  // currently on a blockSize boundary...
  if (numBytes%blockSize)
    numBytes = blockSize - numBytes%blockSize;
  else
    numBytes = 0;
  numBytes += numBlocks * blockSize;
  
  // Write the Bytes...
  while (numBytes) {
    numToWrite = Min(numBytes, sizeof(empty));
    status = theFile->stream.PutBytes(empty, numToWrite, numWritten);
    if (status) goto SOMEERROR;
    if (numWritten != numToWrite) goto INTERNAL;
    numBytes -= numWritten;
    theFile->size += numWritten;
  }
  status = theFile->stream.Flush();
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOBLOCKSIZE: status = Error::CacheBlkSizeNotSet;  break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    INTERNAL:    status = Error::Internal;            break;
    SOMEERROR:                                        break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Info Methods
//

CacheInfo Cache::GetInfo() {
  
  //Status    status;
  CacheInfo localInfo;
  
  /*status =*/ GetInfo(localInfo);
  
  return localInfo.Consumable();
  
}

Status Cache::GetInfo(CacheInfo &that) {
  
  Status          status;
  CacheInfo       localInfo;
  CacheFile      *curFile;
  UInt            curFileIdx;
  UInt            numFileIdx;
  CacheFileInfo   curFileInfo;
  CacheBlock     *curBlock;
  UInt            curBlockIdx;
  UInt            numBlockIdx;
  CacheBlockInfo  curBlockInfo;
  UInt            outIdx;

  // Sanity checks...
  NEED_TheBefe
  NEED_TheOS

  // Id/Time/Various
  localInfo.id = id;
  localInfo.snapTime.Set();
  localInfo.maxBlocks = maxBlocks;
  localInfo.blockSize = blockSize;
  
  // If we're Null/Empty, nothing else to do
  if (IsNull()) goto OK;
  
  // CacheFileInfo...
  numFileIdx = numUsedFiles;
  status = localInfo.fileInfos.Allocate(Alloc(numFileIdx,0));
  if (status) goto SOMEERROR;
  curFileIdx = firstUsedFileIdx;
  outIdx = 0;
  while (!BEFE::IsNull(curFileIdx)) {
    curFile = (CacheFile *)files._GetAddress(curFileIdx);
    if (BEFE::IsNull(curFile)) goto NULLPOINTER;
    status = curFile->GetFileInfo(*this, curFileInfo);
    if (status) goto SOMEERROR;
    status = localInfo.fileInfos.Set(outIdx++, curFileInfo.Consumable());
    if (status) goto SOMEERROR;
    curFileIdx = curFile->nextFileIdx;
  }
  
  // All CacheBlockInfos...
  numBlockIdx = blocks.Length();
  status = localInfo.blockInfos.Allocate(Alloc(numBlockIdx,0));
  if (status) goto SOMEERROR;
  curBlockIdx = 0;
  curBlock = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(curBlock)) goto NULLPOINTER;
  while (curBlockIdx < numBlockIdx) {
    status = curBlock->GetBlockInfo(curBlockIdx, curBlockInfo);
    if (status) goto SOMEERROR;
    status = localInfo.blockInfos.Set(curBlockIdx, curBlockInfo.Consumable());
    if (status) goto SOMEERROR;
    ++curBlockIdx;
    ++curBlock;
  }

  // Used/Free block indexes...
  localInfo.usedBlockIndexes.SetLength(numUsedBlocks);
  curBlockIdx = firstUsedBlockIdx;
  while (!BEFE::IsNull(curBlockIdx)) {
    curBlock = (CacheBlock *)blocks._GetAddress(curBlockIdx);
    localInfo.usedBlockIndexes.Append(curBlockIdx);
    curBlockIdx = curBlock->nextBlockIdx;
  }
  localInfo.freeBlockIndexes.SetLength(numFreeBlocks);
  curBlockIdx = firstFreeBlockIdx;
  while (!BEFE::IsNull(curBlockIdx)) {
    curBlock = (CacheBlock *)blocks._GetAddress(curBlockIdx);
    localInfo.freeBlockIndexes.Append(curBlockIdx);
    curBlockIdx = curBlock->nextBlockIdx;
  }

  // I/O Statistics
  localInfo.numDirtyBlocks = numDirtyBlocks;
  localInfo.numLogReads    = numLogReads;
  localInfo.numPhyReads    = numPhyReads;
  localInfo.numLogWrites   = numLogWrites;
  localInfo.numPhyWrites   = numPhyWrites;

  // Handle errors
  status = Error::None;
  while (false) {
    OK:          status = Error::None;                break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }

  // Return localInfo if everything's okay...
  if (!status)
    status = ((CacheInfo *)&that)->CopyFrom(localInfo.Consumable());
  
  return status;
  
}

Id Cache::GetFileId(String const &pFullPath) {
  
  //Status status;
  Id     localId;
  
  /*status =*/ GetFileId(pFullPath, localId);

  return localId;

}

Status Cache::GetFileId(String const &pFullPath, Id &pId) {

  Status status;
  
  NEED_TheBefe
  NEED_TheOS
  
  status = fileNameIds.Get(pFullPath, pId);
  if (status == Error::IndexNotFound)
    status = Error::CacheFileNotCached;
  
  return status;

}

CacheFileInfo Cache::GetFileInfo(Id pFileId) {

  //Status        status;
  CacheFileInfo localInfo;
  
  /*status =*/ GetFileInfo(pFileId, localInfo);

  return localInfo.Consumable();

}

Status Cache::GetFileInfo(Id pFileId, CacheFileInfo &that) {

  Status         status;
  UInt           fileIdx;
  CacheFile     *theFiles;
  CacheFileInfo  localInfo;
  
  NEED_TheBefe
  NEED_TheOS
  
  status = _FileIdToIndex(pFileId, fileIdx);
  if (status) goto SOMEERROR;
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  if (!theFiles[fileIdx].isUsed) goto FILESTALE;
  status = theFiles[fileIdx].GetFileInfo(*this, localInfo);
  if (status) goto SOMEERROR;
  status = that.CopyFrom(localInfo.Consumable());
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    FILESTALE:   status = Error::CacheFileIdStale;    break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;

}

UInt Cache::GetFileSize(Id fileId) {
  Status status;
  UInt   localSize;
  status = GetFileSize(fileId, localSize);
  if (status) 
    BEFE::SetNull(localSize);
  return localSize;
}

Status Cache::GetFileSize(Id fileId, UInt &size) {
  
  Status     status;
  UInt       fileIdx;
  CacheFile *theFiles;

  NEED_TheBefe
  NEED_TheOS
  
  status = _FileIdToIndex(fileId, fileIdx);
  if (status) goto SOMEERROR;
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  if (!theFiles[fileIdx].isUsed) goto FILESTALE;
  size = theFiles[fileIdx].stream.Size();
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    FILESTALE:   status = Error::CacheFileIdStale;    break;
    SOMEERROR:                                        break;
  }
  if (status)
    BEFE::SetNull(size);
    
  return status;
  
}

UInt Cache::GetBlockSize() {

  // Make sure blockSize is set...
  if (BEFE::IsNull(blockSize)) {
    if (!BEFE::IsNull(gDefaultBufSize))
      blockSize = Max(gDefaultBufSize, 256);
    else
      blockSize = gDefaultBufSize;
  }
  
  // Return it...
  return blockSize;

}

Status Cache::GetBlockSize(UInt &pBlockSize) {

  // Make sure blockSize is set...
  if (BEFE::IsNull(blockSize)) {
    if (!BEFE::IsNull(gDefaultBufSize))
      blockSize = Max(gDefaultBufSize, 256);
    else
      blockSize = gDefaultBufSize;
  }
  
  // Return it...
  pBlockSize = blockSize;
  return Error::None;

}

CacheBlockInfo Cache::GetBlockInfo(Id pFileId, UInt pBlockNo) {

  //Status         status;
  CacheBlockInfo localInfo;

  /*status =*/ GetBlockInfo(pFileId, pBlockNo, localInfo);

  return localInfo.Consumable();

}

Status Cache::GetBlockInfo(Id pFileId, UInt pBlockNo, CacheBlockInfo &that) {

  NEED_TheBefe
  NEED_TheOS
  
  return Error::NotImplemented;

}
  
//----------------------------------------------------------------------
//
// Protected Methods
//

Status Cache::_Flush() {

  Status      status;
  CacheFile  *theFiles;
  CacheFile  *theFile;
  UInt        curFileIdx;
  CacheBlock *theBlocks;
  CacheBlock *theBlock;
  UInt        curBlockIdx;
  
  if (files.Length() == 0 || BEFE::IsNull(numUsedFiles) || numUsedFiles == 0) goto OK;
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;

  // Note: I know we COULD just walk all the blocks and
  //       flush those that are Used and Dirty, but I figured
  //       this makes more sense because it positions us to
  //       coalesce contiguous dirty file blocks and do it in
  //       a single write if possible -- IN THE FUTURE, MAYBE!!!
  //
  //       Not just that, why waste our time walking through the
  //       Free blocks?  In fact, we might want to suffer the
  //       overhead of maintaining a Dirty Block chain but,
  //       once again -- IN THE FUTURE, MAYBE!!!
  
  curFileIdx = firstUsedFileIdx;

  while (!BEFE::IsNull(curFileIdx)) { // For each File...

    theFile = theFiles+curFileIdx;
    curBlockIdx = theFile->firstBlockIdx;
    
    while (!BEFE::IsNull(curBlockIdx)) { // For each File Block...
    
      theBlock = theBlocks+curBlockIdx;
      if (theBlock->isDirty) { // Flush it...
        status = _FlushBlock(curBlockIdx);
        if (status) goto SOMEERROR;
      } // ... Flush it
      
      curBlockIdx = theBlock->nextBlockIdx;
      
    } // ...For each File Block
    
    // Position to next Used File
    curFileIdx = theFile->nextFileIdx;
    
  } // ...For each File
  
  // Handle errors
  while (false) {
    OK:          status = Error::None;                break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

Status Cache::_FlushFile(UInt fileIdx) {
  return Error::NotImplemented;
}

Status Cache::_ReadBlock(UInt pBlkIdx) {
  return Error::NotImplemented;
}

Status Cache::_FlushBlock(UInt pBlkIdx) {
  
  Status      status;
  CacheBlock *curBlock;
  UInt        fileIdx;
  CacheFile  *curFile;
  Byte       *bytes;
  UInt        numToWrite;
  UInt        numWritten;
  
  if (BEFE::IsNull(pBlkIdx) || pBlkIdx < 0 || pBlkIdx >= blocks.Length()) goto INTERNAL;
  curBlock = (CacheBlock *)blocks._GetAddress(pBlkIdx);
  if (BEFE::IsNull(curBlock)) goto NULLPOINTER;
  
  // If dirty...
  if (curBlock->isDirty) {
    // Get the CacheFile...
    status = _FileIdToIndex(curBlock->fileId, fileIdx);
    if (status) goto SOMEERROR;
    curFile = (CacheFile *)files._GetAddress(fileIdx);
    if (BEFE::IsNull(curFile)) goto NULLPOINTER;
    // Get the buffer...
    bytes = curBlock->buffer.GetBuf();
    if (BEFE::IsNull(bytes)) goto NULLPOINTER;
    // Write it...
    numToWrite = curBlock->buffer.GetUsedBytes();
    status = curFile->stream.SetPos(curBlock->blockNo*blockSize);
    if (status) goto SOMEERROR;
    status = curFile->stream.PutBytes(bytes, numToWrite, numWritten);
    if (status) goto SOMEERROR;
    if (numWritten != numToWrite) goto INTERNAL;
    // It's not dirty now
    curBlock->isDirty = false;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    INTERNAL:    status = Error::Internal;            break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

Status Cache::_AllocateBlocks() {

  Status      status;
  UInt        curBlockNo;
  CacheBlock  empty;
  CacheBlock *curBlock;
  
  // Before we do anything, make sure blockSize is set...
  if (BEFE::IsNull(blockSize)) {
    if (!BEFE::IsNull(gDefaultBufSize))
      blockSize = Max(gDefaultBufSize, 256);
    else
      blockSize = gDefaultBufSize;
  }
  
  // Some sanity checks...
  if (BEFE::IsNull(maxBlocks) || maxBlocks < 2) goto BADMAXBLOCKS;
  if (blocks.Length() == maxBlocks) goto OK;

  status = blocks.Allocate(Alloc(0,maxBlocks));
  if (status) goto SOMEERROR;

  // Make maxBlks number of empty blocks  
  curBlockNo = 0;
  while (curBlockNo < maxBlocks) {
    status = blocks.Append(empty);
    if (status) goto SOMEERROR;
    curBlockNo++;
  }
  
  // Allocate the buffer for each one
  curBlock = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(curBlock)) goto NULLPOINTER;
  curBlockNo = 0;
  while (curBlockNo < maxBlocks) {
    status = curBlock->buffer.Allocate(blockSize);
    if (status) goto SOMEERROR;
    status = _BlockTailFree(curBlockNo);
    if (status) goto SOMEERROR;
    curBlock++;
    curBlockNo++;
  }
  
  numUsedBlocks = 0;
  
  numDirtyBlocks = 0;
  numLogReads    = 0;
  numPhyReads    = 0;
  numLogWrites   = 0;
  numPhyWrites   = 0;
  
  // Handle errors
  while (false) {
    BADMAXBLOCKS: status = Error::CacheBadMaxBlocks;   break;
    OK:           status = Error::None;                break;
    NULLPOINTER:  status = Error::InternalNullPointer; break;
    SOMEERROR:                                         break;
  }
  
  return status;
  
}

Status Cache::_FileIdToIndex(Id fileId, UInt &fileIdx) { // Cache._FileIdToIndex...

  Status status;
  status = fileIdIndexes.Get(fileId, fileIdx);
  if (status == Error::IndexNotFound)
    status = Error::CacheFileNotCached;
  if (status) goto SOMEERROR;
  if (fileIdx >= files.Length()) goto BADINDEX;
  
  // Handle Errors
  while (false) {
    BADINDEX:  status = Error::CacheBadFileIdx;  break;
    SOMEERROR:                                   break;
  }
  
  return status;
  
} // ...Cache._FileIdToIndex

Status Cache::_FindUsedFileBlock(UInt fileIdx, UInt blockNo, UInt &blockIdx) { // Cache._FindUsedFileBlock...

  Status      status;
  CacheFile  *theFiles;
  CacheFile  *theFile;
  CacheBlock *theBlocks;
  UInt        ourBlockIdx;

  BEFE::SetNull(ourBlockIdx);
  
  // Prepare and validate FileIdx...
  numFiles = files.Length();
  if (fileIdx >= files.Length()) goto BADFILENO;
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  theFile = theFiles + fileIdx;
  
  // Prepare BlockIdx...
  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;
 
  // Search them...
  ourBlockIdx = theFile->firstBlockIdx;
  while (!BEFE::IsNull(ourBlockIdx)) {
    if (theBlocks[ourBlockIdx].blockNo == blockNo) break;
    ourBlockIdx = theBlocks[ourBlockIdx].nextBlockInFileIdx;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    BADFILENO:   status = Error::CacheBadFileIdx;     break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
  }
    
  if (status)
    BEFE::SetNull(blockIdx);
  else
    blockIdx = ourBlockIdx;
    
  return status;
  
}  // ...Cache._FindUsedFileBlock

//----------------------------------------------------------------------
//
// Public but dangerous since it circumvents undo/redo of block updates...
//

Byte *Cache::_GetBlockPointer(Id fileId, UInt blockNo) {
  
  Status      status;
  Byte       *pointer;
  UInt        fileIdx;
  UInt        blockIdx;
  CacheBlock *cacheBlock;
  
  fileIdx = fileIdIndexes.Get(fileId);
  if (BEFE::IsNull(fileIdx)) goto BADFILEID;
  status = _FindUsedFileBlock(fileIdx, blockNo, blockIdx);
  if (status) goto SOMEERROR;
  cacheBlock = (CacheBlock *)blocks._GetAddress(blockIdx);
  if (BEFE::IsNull(cacheBlock)) goto NULLPOINTER;
  pointer = cacheBlock->buffer.GetBuf();
  
  // Handle errors
  while (false) {
    BADFILEID:   BEFE::SetNull(pointer); break;
    NULLPOINTER: BEFE::SetNull(pointer); break;
    SOMEERROR:   BEFE::SetNull(pointer); break;
  }
  
  return pointer;
  
}

Status Cache::_SetBlockIsDirty(Id fileId, UInt blockNo) {
  
  Status      status;
  UInt        fileIdx;
  UInt        blockIdx;
  CacheBlock *cacheBlock;
  
  fileIdx = fileIdIndexes.Get(fileId);
  if (BEFE::IsNull(fileIdx)) goto BADFILEID;
  status = _FindUsedFileBlock(fileIdx, blockNo, blockIdx);
  if (status) goto SOMEERROR;
  cacheBlock = (CacheBlock *)blocks._GetAddress(blockIdx);
  if (BEFE::IsNull(cacheBlock)) goto NULLPOINTER;
  
  // Set "IsDirty"...
  cacheBlock->isDirty = true;
  
  // Handle errors
  while (false) {
    BADFILEID:   status = Error::CacheFileNotCached;  break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

} // ...Namespace BEFE
