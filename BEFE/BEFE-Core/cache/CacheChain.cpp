//----------------------------------------------------------------------
// File: CacheChain.cpp - Cache Class Chain Manipulation Methods
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

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Protected Methods: Cache Free/Used Block Chain...
//

Status Cache::_BlockUnchainFreeUsed(UInt blockIdx) { // Cache._BlockUnchainFreeUsed...

  Status      status;
  CacheBlock *theBlocks;
  CacheBlock *theBlock;

  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;
  if (BEFE::IsNull(blockIdx) || blockIdx >= blocks.Length()) goto BADBLOCKIDX;
  theBlock = theBlocks + blockIdx;
  
  if (theBlock->isUsed && theBlock->isFree) goto INTEGRITY;
  if (!theBlock->isUsed && !theBlock->isFree) goto NOTCHAINED;
  
  if (theBlock->isFree && BEFE::IsNull(numFreeBlocks)) goto INTEGRITY;
  if (theBlock->isUsed && BEFE::IsNull(numUsedBlocks)) goto INTEGRITY;

  // Remove from forward Chain...
  if (BEFE::IsNull(theBlock->prevBlockIdx)) {
    if (theBlock->isFree)
      firstFreeBlockIdx = theBlock->nextBlockIdx;
    else
      firstUsedBlockIdx = theBlock->nextBlockIdx;
  }
  else
    theBlocks[theBlock->prevBlockIdx].nextBlockIdx = theBlock->nextBlockIdx;
  
  // Remove from backward Chain...
  if (BEFE::IsNull(theBlock->nextBlockIdx)) {
    if (theBlock->isFree)
      lastFreeBlockIdx = theBlock->prevBlockIdx;
    else
      lastUsedBlockIdx = theBlock->prevBlockIdx;
  }
  else
    theBlocks[theBlock->nextBlockIdx].prevBlockIdx = theBlock->prevBlockIdx;
  
  // Update counters
  if (theBlock->isFree)
    numFreeBlocks--;
  else
    numUsedBlocks--;
    
  // We're in "limbo" now...
  BEFE::SetNull(theBlock->nextBlockIdx);
  BEFE::SetNull(theBlock->prevBlockIdx);
  theBlock->isFree = theBlock->isUsed = false;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;  break;
    BADBLOCKIDX: status = Error::CacheBadBlockIdx;     break;
    INTEGRITY:   status = Error::InternalIntegrity;    break;
    NOTCHAINED:  status = Error::CacheBlockNotChained; break;
  }
  
  return status;

}  // ...Cache._BlockUnchainFreeUsed

Status Cache::_BlockHeadFree(UInt blockIdx) { // Cache._BlockHeadFree...

  Status      status;
  CacheBlock *theBlocks;
  CacheBlock *theBlock;

  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;
  if (BEFE::IsNull(blockIdx) || blockIdx >= blocks.Length()) goto BADBLOCKIDX;
  theBlock = theBlocks + blockIdx;
  
  if (!BEFE::IsNull(theBlock->nextBlockIdx) || !BEFE::IsNull(theBlock->prevBlockIdx)) goto INTEGRITY;
  
  if (BEFE::IsNull(firstFreeBlockIdx)) {
    firstFreeBlockIdx = blockIdx;
    lastFreeBlockIdx  = blockIdx;
  }
  else {
    theBlocks[firstFreeBlockIdx].prevBlockIdx = blockIdx;
    theBlock->nextBlockIdx                    = firstFreeBlockIdx;
    firstFreeBlockIdx                         = blockIdx;
  }

  if (BEFE::IsNull(numFreeBlocks))
    numFreeBlocks = 0;
  numFreeBlocks++;
  theBlock->isFree = true;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    BADBLOCKIDX: status = Error::CacheBadBlockIdx;    break;
    INTEGRITY:   status = Error::InternalIntegrity;   break;
  }
  
  return status;

}  // ...Cache._BlockHeadFree

Status Cache::_BlockTailFree(UInt blockIdx) { // Cache._BlockTailFree...

  Status      status;
  CacheBlock *theBlocks;
  CacheBlock *theBlock;

  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;
  if (BEFE::IsNull(blockIdx) || blockIdx >= blocks.Length()) goto BADBLOCKIDX;
  theBlock = theBlocks + blockIdx;
  
  if (!BEFE::IsNull(theBlock->nextBlockIdx) || !BEFE::IsNull(theBlock->prevBlockIdx)) goto INTEGRITY;

  if (BEFE::IsNull(lastFreeBlockIdx)) {
    firstFreeBlockIdx = blockIdx;
    lastFreeBlockIdx  = blockIdx;
  }
  else {
    theBlocks[lastFreeBlockIdx].nextBlockIdx = blockIdx;
    theBlock->prevBlockIdx                   = lastFreeBlockIdx;
    lastFreeBlockIdx                         = blockIdx;
  }

  if (BEFE::IsNull(numFreeBlocks))
    numFreeBlocks = 0;
  numFreeBlocks++;
  theBlock->isFree = true;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    BADBLOCKIDX: status = Error::CacheBadBlockIdx;    break;
    INTEGRITY:   status = Error::InternalIntegrity;   break;
  }
  
  return status;

}  // ...Cache._BlockTailFree

Status Cache::_BlockHeadUsed(UInt blockIdx) { // Cache._BlockHeadUsed...

  Status      status;
  CacheBlock *theBlocks;
  CacheBlock *theBlock;

  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;
  if (BEFE::IsNull(blockIdx) || blockIdx >= blocks.Length()) goto BADBLOCKIDX;
  theBlock = theBlocks + blockIdx;
  
  if (!BEFE::IsNull(theBlock->nextBlockIdx) || !BEFE::IsNull(theBlock->prevBlockIdx)) goto INTEGRITY;

  if (BEFE::IsNull(firstUsedBlockIdx)) {
    firstUsedBlockIdx = blockIdx;
    lastUsedBlockIdx  = blockIdx;
  }
  else {
    theBlocks[firstUsedBlockIdx].prevBlockIdx = blockIdx;
    theBlock->nextBlockIdx                    = firstUsedBlockIdx;
    firstUsedBlockIdx                         = blockIdx;
  }

  if (BEFE::IsNull(numUsedBlocks))
    numUsedBlocks = 0;
  numUsedBlocks++;
  theBlock->isUsed = true;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    BADBLOCKIDX: status = Error::CacheBadBlockIdx;    break;
    INTEGRITY:   status = Error::InternalIntegrity;   break;
  }
  
  return status;

}  // ...Cache._BlockHeadUsed

//----------------------------------------------------------------------
//
// Protected Methods: Cache File Block Chain...
//
// Note: Both these methods DEMAND the Block is already Used AND the
//       File IS Used AND the Block IS NOT on any File's Block Chain.
//
//       So, basically, the Block must be in a temporary transition
//       state (see Cache.BlockLoad and other Cache methods for proper
//       context)
//

Status Cache::_BlockUnchainFile(UInt blockIdx) { // Cache._BlockUnchainFile...

  Status      status;
  CacheFile  *theFiles;
  CacheFile  *theFile;
  UInt        fileIdx;
  CacheBlock *theBlocks;
  CacheBlock *theBlock;

  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;
  if (BEFE::IsNull(blockIdx) || blockIdx >= blocks.Length()) goto BADBLOCKIDX;
  theBlock = theBlocks + blockIdx;
  
  // Nothing to do if not already chained...
  if (BEFE::IsNull((PtrInt)theBlock->fileId) || BEFE::IsNull(theBlock->blockNo) || !theBlock->isUsed) goto OK;

  // Find the file...
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  status = _FileIdToIndex(theBlock->fileId, fileIdx);
  if (status) goto SOMEERROR;
  theFile = theFiles+fileIdx;
  if (!theFile->isUsed) goto FILENOTUSED;
  
  // Unchain it...
  if (BEFE::IsNull(theBlock->nextBlockInFileIdx))
    theFile->lastBlockIdx = theBlock->prevBlockInFileIdx;
  else
    theBlocks[theBlock->nextBlockInFileIdx].prevBlockInFileIdx = theBlock->prevBlockInFileIdx;
  if (BEFE::IsNull(theBlock->prevBlockInFileIdx))
    theFile->firstBlockIdx = theBlock->nextBlockInFileIdx;
  else
    theBlocks[theBlock->prevBlockInFileIdx].nextBlockInFileIdx = theBlock->nextBlockInFileIdx;
    
  // Handle errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    BADBLOCKIDX: status = Error::CacheBadBlockIdx;    break;
    OK:          status = Error::None;                break;
    FILENOTUSED: status = Error::CacheFileNotUsed;    break;
    SOMEERROR:                                        break;
  }

  // Clean up some left over stuff
  theBlock->fileId = NULL;
  BEFE::SetNull(theBlock->blockNo);
  BEFE::SetNull(theBlock->nextBlockInFileIdx);
  BEFE::SetNull(theBlock->prevBlockInFileIdx);
  
  return status;
  
}  // ...Cache._BlockUnchainFile

Status Cache::_BlockHeadFile(UInt pFileIdx, UInt blockIdx) { // Cache._BlockHeadFile...

  Status      status;
  CacheFile  *theFiles;
  CacheFile  *theFile;
  CacheBlock *theBlocks;
  CacheBlock *theBlock;

  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (BEFE::IsNull(theBlocks)) goto NULLPOINTER;
  if (BEFE::IsNull(blockIdx) || blockIdx >= blocks.Length()) goto BADBLOCKIDX;
  theBlock = theBlocks + blockIdx;
  
  if (!theBlock->isUsed) goto NOTUSED;
  
  // Get the File's Index and Address, and do some sanity checks...
  if (!BEFE::IsNull(theBlock->fileId)) goto BLOCKFILECHAINED;
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  theFile = theFiles+pFileIdx;
  if (!theFile->isUsed) goto FILENOTUSED;
  if (BEFE::IsNull(theFile->id)) goto NULLFILEID;
  if (!BEFE::IsNull(theBlock->nextBlockInFileIdx)    ||
      !BEFE::IsNull(theBlock->prevBlockInFileIdx)    ||
      theFile->firstBlockIdx == blockIdx       ||
      theFile->lastBlockIdx  == blockIdx        )  goto BLOCKFILECHAINED;
  
  // Do it...
  if (BEFE::IsNull(theFile->firstBlockIdx)) {
    theFile->firstBlockIdx = blockIdx;
    theFile->lastBlockIdx  = blockIdx;
  }
  else {
    theBlocks[theFile->firstBlockIdx].prevBlockInFileIdx = blockIdx;
    theBlock->nextBlockInFileIdx                         = theFile->firstBlockIdx;
    theFile->firstBlockIdx                               = blockIdx;
  }

  // Set final stuff
  theBlock->fileId = theFile->id;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER:      status = Error::InternalNullPointer;     break;
    BADBLOCKIDX:      status = Error::CacheBadBlockIdx;        break;
    NOTUSED:          status = Error::CacheBlockNotUsed;       break;
    FILENOTUSED:      status = Error::CacheFileNotUsed;        break;
    NULLFILEID:       status = Error::CacheFileNullId;         break;
    BLOCKFILECHAINED: status = Error::CacheBlockIsFileChained; break;
  }
  
  return status;

}  // ...Cache._HeadFile

//----------------------------------------------------------------------
//
// Protected Methods: Cache Used/Free File Chain...
//

Status Cache::_FileUnchainFree(UInt fileIdx) { // Cache._FileUnchainFree...

  Status     status;
  CacheFile *theFiles;
  
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  if (BEFE::IsNull(fileIdx) || fileIdx >= numFiles) goto OUTOFBOUNDS;
  
  if (BEFE::IsNull(theFiles[fileIdx].prevFileIdx))
    firstFreeFileIdx = theFiles[fileIdx].nextFileIdx;
  else {
    theFiles[theFiles[fileIdx].prevFileIdx].nextFileIdx = theFiles[fileIdx].nextFileIdx;
    BEFE::SetNull(theFiles[fileIdx].prevFileIdx);
  }
  
  if (BEFE::IsNull(theFiles[fileIdx].nextFileIdx))
    lastFreeFileIdx = theFiles[fileIdx].prevFileIdx;
  else {
    theFiles[theFiles[fileIdx].nextFileIdx].prevFileIdx = theFiles[fileIdx].prevFileIdx;
    BEFE::SetNull(theFiles[fileIdx].nextFileIdx);
  }

  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    OUTOFBOUNDS: status = Error::InternalOutOfRange;  break;
  }
  
  return status;

} // ...Cache._FileUnchainFree

Status Cache::_FileUnchainUsed(UInt fileIdx) { // Cache._FileUnchainUsed...

  Status     status;
  CacheFile *theFiles;
  
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  if (BEFE::IsNull(fileIdx) || fileIdx >= numFiles) goto OUTOFBOUNDS;
  
  if (BEFE::IsNull(theFiles[fileIdx].prevFileIdx))
    firstUsedFileIdx = theFiles[fileIdx].nextFileIdx;
  else {
    theFiles[theFiles[fileIdx].prevFileIdx].nextFileIdx = theFiles[fileIdx].nextFileIdx;
    BEFE::SetNull(theFiles[fileIdx].prevFileIdx);
  }
  
  if (BEFE::IsNull(theFiles[fileIdx].nextFileIdx))
    lastUsedFileIdx = theFiles[fileIdx].prevFileIdx;
  else {
    theFiles[theFiles[fileIdx].nextFileIdx].prevFileIdx = theFiles[fileIdx].prevFileIdx;
    BEFE::SetNull(theFiles[fileIdx].nextFileIdx);
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    OUTOFBOUNDS: status = Error::InternalOutOfRange;  break;
  }
  
  return status;

} // ...Cache._FileUnchainUsed

Status Cache::_FileHeadFree(UInt fileIdx) { // Cache._FileHeadFree...

  Status     status;
  CacheFile *theFiles;
  
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  if (BEFE::IsNull(fileIdx) || fileIdx >= numFiles) goto OUTOFBOUNDS;
  
  if (BEFE::IsNull(firstFreeFileIdx)) {
    firstFreeFileIdx = fileIdx;
    lastFreeFileIdx  = fileIdx;
  }
  else {
    theFiles[firstFreeFileIdx].prevFileIdx = fileIdx;
    theFiles[fileIdx].nextFileIdx          = firstFreeFileIdx;
    firstFreeFileIdx                       = fileIdx;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    OUTOFBOUNDS: status = Error::InternalOutOfRange;  break;
  }
  
  return status;

} // ...Cache._FileHeadFree

Status Cache::_FileTailFree(UInt fileIdx) { // Cache._FileTailFree...

  Status     status;
  CacheFile *theFiles;
  
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  if (BEFE::IsNull(fileIdx) || fileIdx >= numFiles) goto OUTOFBOUNDS;
    
  if (BEFE::IsNull(lastFreeFileIdx)) {
    firstFreeFileIdx = fileIdx;
    lastFreeFileIdx  = fileIdx;
  }
  else {
    theFiles[lastFreeFileIdx].nextFileIdx = fileIdx;
    theFiles[fileIdx].prevFileIdx         = lastFreeFileIdx;
    lastFreeFileIdx                       = fileIdx;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    OUTOFBOUNDS: status = Error::InternalOutOfRange;  break;
  }
  
  return status;

} // ...Cache._FileTailFree

Status Cache::_FileHeadUsed(UInt fileIdx) { // Cache._FileHeadUsed...

  Status     status;
  CacheFile *theFiles;
  
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  if (BEFE::IsNull(fileIdx) || fileIdx >= numFiles) goto OUTOFBOUNDS;
    
  if (BEFE::IsNull(firstUsedFileIdx)) {
    firstUsedFileIdx = fileIdx;
    lastUsedFileIdx  = fileIdx;
  }
  else {
    theFiles[firstUsedFileIdx].prevFileIdx = fileIdx;
    theFiles[fileIdx].nextFileIdx          = firstUsedFileIdx;
    firstUsedFileIdx                       = fileIdx;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    OUTOFBOUNDS: status = Error::InternalOutOfRange;  break;
  }
  
  return status;

} // ...Cache._FileHeadUsed

} // ...Namespace BEFE
