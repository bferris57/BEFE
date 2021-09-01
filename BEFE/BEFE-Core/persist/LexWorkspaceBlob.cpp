//----------------------------------------------------------------------
// File: LexWorkspaceBlob.cpp - Implementation of the BEFE LexWorkspace
///                             Class Blob Methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Lex.h"
#include "LexWorkspace.h"
#include "LexWs.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Workspace Blob methods
//

UInt LexWorkspace::BlobCount() {
  
  Status status;
  UInt   answer;
  
  status = _Open();
  if (!status)
    answer = wsHead.blobs.numEntries;
  else
    answer = UNaN;

  return answer;
  
}

Status LexWorkspace::BlobNew(Id &blobId) {

  Status         status;
  UInt32         blockNo;
  UInt32         entryNo;
  LexWsBlockHead blockHead;
  LexWsBlob      blob;

  // Ask general IdEntry to create it...
  status = _IdEntryNew(wsHead.blobs, blockNo, entryNo, blob.id);
  if (status) goto SOMEERROR;
  
  // Append it to the block...
  status = blob.Write(fileId, blockNo, entryNo);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  // Return the Blob Id...
  blobId = blob.id;
  
  return status;
  
}

Status LexWorkspace::BlobDelete(Id blobId) {
  return Error::NotImplemented;
}

Boolean LexWorkspace::BlobExists(Id blobId) {
  
  Status    status;
  Boolean   answer;
  UInt      blockNo;
  UInt      entryNo;
  LexWsBlob blobHead;
  
  status = _IdEntryFind(wsHead.blobs, blobId, blockNo, entryNo);
  answer = (status == Error::None && !BEFE::IsNull(blockNo));
  
  return answer;
  
}

UInt LexWorkspace::BlobLength(Id blobId) {

  Status    status;
  UInt      answer;
  UInt      blockNo;
  UInt      entryNo;
  LexWsBlob blobHead;

  answer = UNaN;  
  status = _IdEntryFind(wsHead.blobs, blobId, blockNo, entryNo);
  if (!status && !BEFE::IsNull(blockNo)) {
    status = blobHead.Read(fileId, blockNo, entryNo);
    if (!status)
      answer = blobHead.len;
  }
      
  return answer;
  
}

UInt LexWorkspace::BlobSize(Id blobId) {
  return BlobLength(blobId);
}

Status LexWorkspace::BlobAppend(Id blobId, Byte *bytes, UInt len) { // LexWorkspace.BlobAppend...

  Status          status;
  UInt            headBlockNo;
  UInt            headEntryNo;
  LexWsBlob       blobHead;
  UInt            blockNo;
  LexWsBlockHead  blockHead;
  UInt            blockHeadSize;
  UInt            dataBytesPerBlock;
  UInt            remToWrite;
  UInt            numToWrite;
  UInt            remInBlock;
  Byte           *bytesToWrite;
  UInt            blockStartOffset;

  // Validate parameters
  if (BEFE::IsNull(bytes) || BEFE::IsNull(len)) goto BADPARAM;
  
  // Open first...
  status = _Open();
  if (status) goto SOMEERROR;
  
  // Find it...
  status = _IdEntryFind(wsHead.blobs, blobId, headBlockNo, headEntryNo);
  if (status) goto SOMEERROR;
  status = blobHead.Read(fileId, headBlockNo, headEntryNo);
  if (status) goto SOMEERROR;
  if (blobHead.id != blobId) goto NOTEXIST;

  // If no len, don't append...
  if (len == 0) goto OK;

  // Append block if no blocks or last one's full...
  dataBytesPerBlock = blobHead.DataBytesPerBlock();
  if (BEFE::IsNull(blobHead.lstLowBlockNo) ||
      BEFE::IsNull(blobHead.len)           ||
      (blobHead.len % dataBytesPerBlock) == 0
     ) {
    if (BEFE::IsNull(blobHead.len))
      blobHead.len = 0;
    status = _BlobAppendEmptyBlock(headBlockNo, headEntryNo, blobHead);
    if (status) goto SOMEERROR;
  }

  // Initialise for writing...
  blockNo          = blobHead.lstLowBlockNo;
  remToWrite       = len;
  remInBlock       = dataBytesPerBlock - (blobHead.len % dataBytesPerBlock);
  bytesToWrite     = bytes;
  blockHeadSize    = LexWsBlockHead::PersistSize(LexWs::BlockTypeBlob);
  blockStartOffset = blkSize - remInBlock;
  
  // Write until we're finished...
  do {
    
    // Finished if nothing left to write
    if (remToWrite == 0) break;
    
    // Append block if full...
    if (remInBlock == 0) {
      if (BEFE::IsNull(blobHead.len))
        blobHead.len = 0;
      status = _BlobAppendEmptyBlock(headBlockNo, headEntryNo, blobHead);
      if (status) goto SOMEERROR;
      blockStartOffset = blockHeadSize;
      remInBlock = dataBytesPerBlock;
    }
    blockNo = blobHead.lstLowBlockNo;
    
    // Figure out how many to write
    numToWrite = Min(remToWrite, remInBlock);
    
    // Write them...
    status = TheBefe->TheCache.SetBlockContents(fileId, blockNo, blockStartOffset, bytesToWrite, numToWrite);
    if (status) goto SOMEERROR;
    
    // Account for bytes written
    remInBlock -= numToWrite;
    remToWrite -= numToWrite;
    bytesToWrite += numToWrite;
    
  } while (true);
  
  // Update the blob header...
  blobHead.len += len;
  status = blobHead.Write(fileId, headBlockNo, headEntryNo);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    BADPARAM:  status = Error::InvalidParameter;  break;
    OK:        status = Error::None;              break;
    NOTEXIST:  status = Error::LexWsBlobNotFound; break;
    SOMEERROR:                                    break;
  }
  
  return status;
  
} // ...LexWorkspace.BlobAppend

Status LexWorkspace::BlobInsert(Id blobId, USpan &span, Byte *bytes) {
  return Error::NotImplemented;
}

Status LexWorkspace::BlobRemove(Id blobId, USpan &span) {
  return Error::NotImplemented;
}

Bytes LexWorkspace::BlobGet(Id blobId) {
  
  //Status status;
  Bytes  bytes;
  
  /*status =*/ BlobGet(blobId, bytes);
  
  return bytes.Consumable();
  
}

Status LexWorkspace::BlobGet(Id blobId, USpan &span, Byte *bytes) {
  
  Status          status;
  UInt            blockNo;
  UInt            entryNo;
  LexWsBlob       blobHead;
  LexWsBlockHead  blockHead;
  USpan           rSpan;
  UInt            bytesPerBlock;
  UInt            relStartBlockNo;
  UInt            blockOffset;
  UInt            remLen;
  UInt            bytesToGet;
  
  // Open first...
  status = _Open();
  if (status) goto SOMEERROR;
  
  // Get the blob header
  status = _IdEntryFind(wsHead.blobs, blobId, blockNo, entryNo);
  if (status) goto SOMEERROR;
  status = blobHead.Read(fileId, blockNo, entryNo);
  if (status) goto SOMEERROR;
  
  // Resolve the span based on the Blob's length
  rSpan = span.Resolve(blobHead.len);
  if (rSpan.Length() == 0) goto OK;

  // Sanity check...
  if (BEFE::IsNull(blobHead.fstLowBlockNo)) goto INTERNAL;
  
  // Figure out starting/ending block relative offsets...
  bytesPerBlock = blobHead.DataBytesPerBlock();
  relStartBlockNo = (rSpan.idx1+bytesPerBlock-1)/bytesPerBlock;
  
  // Whip through the chain until we're at the start block...
  blockNo = blobHead.fstLowBlockNo;
  while (relStartBlockNo != 0) {
    // Read the header
    status = blockHead.Read(fileId, blockNo);
    if (status) goto SOMEERROR;
    relStartBlockNo--;
    // Integrity check...
    if (blockHead.type != LexWs::BlockTypeBlobData) goto BADBLOCKTYPE;
  }

  // Sanity check...
  if (BEFE::IsNull(blockNo)) goto INTERNAL;
  
  // Until we've got them all...
  remLen = rSpan.Length();
  blockOffset = blockHead.PersistSize() + rSpan.idx1%bytesPerBlock;

  while (remLen != 0) {
    
    // Figure out how many to get
    bytesToGet = Min(remLen, bytesPerBlock-blockOffset);
    
    // Get the bytes
    status = TheBefe->TheCache.GetBlockContents(fileId, blockNo, blockOffset, bytes, bytesToGet);
    if (status) goto SOMEERROR;

    // Adjust remaining
    remLen -= bytesToGet;
    bytes += bytesToGet;
    blockOffset = blockHead.PersistSize();
    
  }
  
  // Handle errors
  while (false) {
    OK:           status = Error::None;                break;
    INTERNAL:     status = Error::Internal;            break;
    BADBLOCKTYPE: status = Error::LexWsBlockWrongType; break;
    SOMEERROR:                                         break;
  }
  
  return status;
  
}

Status LexWorkspace::BlobGet(Id blobId, Bytes &bytes) {
  return Error::NotImplemented;
}

Status LexWorkspace::_BlobAppendEmptyBlock(UInt headBlockNo, UInt headEntryNo, LexWsBlob &blobHead) {

  Status         status;
  UInt           newBlockNo;
  LexWsBlockHead blockHead;
  
  // Create new block first...
  status = NewFileBlock(LexWs::BlockTypeBlobData, newBlockNo);
  if (status) goto SOMEERROR;

  // Update last block (if any)...
  if (!BEFE::IsNull(blobHead.lstLowBlockNo)) {
    status = blockHead.Read(fileId, blobHead.lstLowBlockNo);
    if (status) goto SOMEERROR;
    blockHead.nextBlockNo = newBlockNo;
    status = blockHead.Write(fileId, blobHead.lstLowBlockNo);
    if (status) goto SOMEERROR;      
  }

  // Update current blob header...
  status = blockHead.Read(fileId, newBlockNo);
  if (status) goto SOMEERROR;
  blockHead.type = LexWs::BlockTypeBlobData;
  blockHead.prevBlockNo = blobHead.lstLowBlockNo;
  status = blockHead.Write(fileId, newBlockNo);
  if (status) goto SOMEERROR;
  
  // Update blob header...
  if (BEFE::IsNull(blobHead.len))
    blobHead.len = 0;
  if (BEFE::IsNull(blobHead.fstLowBlockNo))
    blobHead.fstLowBlockNo = newBlockNo;
  blobHead.lstLowBlockNo = newBlockNo;
  status = blobHead.Write(fileId, headBlockNo, headEntryNo);
  if (status) goto SOMEERROR;
  
  // Handle Errors...
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}
  
} // ...Namespace BEFE