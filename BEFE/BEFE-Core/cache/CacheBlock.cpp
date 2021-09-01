//----------------------------------------------------------------------
// File: CacheBlock.cpp - Implementation of the CacheBlock Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "CacheBlock.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

CacheBlock::CacheBlock() {
  StartUp();
}
  
CacheBlock::CacheBlock(CacheBlock const &that) {
  StartUp();
  CopyFrom(that);
}

CacheBlock::~CacheBlock() {
  ShutDown();
}

CacheBlock &CacheBlock::operator = (CacheBlock const &that) {
  CopyFrom(that);
  NotConsumable();
  return *this;
}
  
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status CacheBlock::StartUp() {
  
  Status status;
  
  isFree  = false;
  isUsed  = false;
  isDirty = false;
  
  BEFE::SetNull(blockNo);
  BEFE::SetNull(nextBlockIdx);
  BEFE::SetNull(prevBlockIdx);
  fileId = NULL;
  BEFE::SetNull(nextBlockInFileIdx);
  BEFE::SetNull(prevBlockInFileIdx);
  BEFE::SetNull(blockLen);
  status = buffer.StartUp();
  
  // I/O Statistics
  numLogReads  = 0;
  numReadHits  = 0;
  numPhyReads  = 0;
  numLogWrites = 0;
  numWriteHits = 0;
  numPhyWrites = 0;
  
  return status;
  
}

Status CacheBlock::ShutDown() {
  
  Status status;

  status = buffer.ShutDown();
  
  return status;
  
}
 
Status CacheBlock::Reset() {
  return StartUp();
}
 
Boolean CacheBlock::IsNull() const {

  return (BEFE::IsNull(buffer));
  
}
 
Boolean CacheBlock::IsEmpty() const {
  return false;
}
 
Status CacheBlock::SetEmpty() {
  
  Status retStatus;
  Status status;
  Buffer saveBuffer;
  
  retStatus = saveBuffer.MoveFrom(buffer);
  status = Reset();
  if (status &&!retStatus) retStatus = status;
  status = buffer.MoveFrom(saveBuffer);
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}
 
Boolean CacheBlock::IsValid() const {
  if (!BEFE::IsNull(buffer.GetBuf()))
    return true;
  return false;
}

Status CacheBlock::MoveFrom(CacheBlock const &that) {

  Status status;
  
  if (this != &that) {
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CacheBlock));
    status = ((CacheBlock *)&that)->StartUp();
  }
  else
    status = Error::None;
    
  return status;
}
 
Status CacheBlock::CopyFrom(CacheBlock const &that) {

  Status status;

  if (this != &that) {
    if (IsConsumable()) {
      status = MoveFrom(that);
      if (status) goto SOMEERROR;
    }
    else {
      MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CacheBlock));
      status = buffer.CopyFrom(that.buffer);
      if (status) goto SOMEERROR;
    }
  }
    
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}
 
CacheBlock &CacheBlock::Consumable() const {
  buffer.Consumable();
  return *(CacheBlock *)this;
}
 
CacheBlock &CacheBlock::NotConsumable() const {
  buffer.NotConsumable();
  return *(CacheBlock *)this;
}
 
Boolean CacheBlock::IsConsumable() const {
  return buffer.IsConsumable();
}
 
UInt CacheBlock::Length() const {
  return buffer.Length();
}
 
UInt CacheBlock::Size() const {
  return buffer.Size();
}

UInt CacheBlock::PersistSize() const {
  return buffer.GetUsedBytes();
}
 
Status CacheBlock::WriteTo(Stream &out) const {
  
  Status status;
  UInt   numToWrite;
  UInt   numWritten;

  if (BEFE::IsNull(buffer)) goto NOBLOCKSIZE;
  numToWrite = buffer.GetUsedBytes();
  if (numToWrite) {
    if (BEFE::IsNull(blockNo)) goto NOBLOCKNO;
    status = out.SetPos(buffer.GetAllocBytes() * blockNo);
    if (status) goto SOMEERROR;
    status = out._PutBytes(buffer.GetBuf(), numToWrite, numWritten);
    if (status) goto SOMEERROR;
    if (numWritten != numToWrite) goto TRUNCATED;
  }
  
  // Handle Errors...
  status = Error::None;
  while (false) {
    NOBLOCKSIZE: status = Error::CacheBlkSizeNotSet;   break;
    NOBLOCKNO:   status = Error::CacheBlkNumberNotSet; break;
    TRUNCATED:   status = Error::FileWriteFailed;      break;
    SOMEERROR:                                         break;
  }
  
  return status;

}

Status CacheBlock::ReadFrom(Stream &in) {
    
  Status  status;
  Byte   *buf;
  UInt    numToRead;
  UInt    numRead;

  if (BEFE::IsNull(buffer)) goto NOBLOCKSIZE;
  numToRead = buffer.GetAllocBytes();
  if (numToRead) {
    status = in.SetPos(numToRead * blockNo);
    if (status) goto SOMEERROR;
    buf = buffer.GetBuf();
    if (BEFE::IsNull(buf)) goto NULLPOINTER;
    status = in._GetBytes(buf, numToRead, numRead);
    if (status) goto SOMEERROR;
    if (numRead != numToRead) goto TRUNCATED;
  }
  
  // Handle Errors...
  status = Error::None;
  while (false) {
    NOBLOCKSIZE: status = Error::CacheBlkSizeNotSet;   break;
    NULLPOINTER: status = Error::InternalNullPointer;  break;
    TRUNCATED:   status = Error::FileReadFailed;       break;
    SOMEERROR:                                         break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Info
//

Status CacheBlock::GetBlockInfo(UInt blockIdx, CacheBlockInfo &info) const {
  
  Status          status;
  CacheBlockInfo  localInfo;
  
  // Sanity checks...
  NEED_TheBefe
  NEED_TheOS
  
  // Do it...
  localInfo.index        = blockIdx;
  localInfo.fileId       = fileId;
  localInfo.blockNo      = blockNo;
  localInfo.snapTime.Set();

  localInfo.isFree       = isFree;
  localInfo.isUsed       = isUsed;
  localInfo.isDirty      = isDirty;
  
  localInfo.lastRead     = lastRead;
  localInfo.lastModified = lastModified;
  localInfo.lastWrite    = lastWrite;
  
  localInfo.numLogReads  = numLogReads;
  localInfo.numPhyReads  = numPhyReads;
  localInfo.numLogWrites = numLogWrites;
  localInfo.numPhyWrites = numPhyWrites;
  
  // Return theInfo
  status = info.CopyFrom(localInfo.Consumable());
    
  return status;
    
}
  
//----------------------------------------------------------------------
//
// Operational Methods
//

Strings CacheBlock::ToStrings() const {
  
  Strings strings;
  
  strings.Append("***CacheBlock.Tostrings Not Implemented Yet***");
  
  return strings.Consumable();
  
}

//----------------------------------------------------------------------
//
// Protected Methods: General...
//

Status CacheBlock::_Allocate(UInt size) { // CacheBlock._Allocate...
  
  Status status;

  if (!BEFE::IsNull(buffer)) goto OK;  
  if (BEFE::IsNull(size)) goto NOBLKSIZE;
  
  status = buffer.Allocate(size);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    OK:        status = Error::None;               break;
    NOBLKSIZE: status = Error::CacheBlkSizeNotSet; break;
    SOMEERROR:                                     break;
  }
  
  return status;
  
} // ...CacheBlock._Allocate

Byte *CacheBlock::_GetBuf() {
  return buffer.GetBuf();
}

} // ...Namespace BEFE
