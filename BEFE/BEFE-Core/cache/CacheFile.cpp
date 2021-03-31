//----------------------------------------------------------------------
// File: CacheFile.cpp - CacheFile Class implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Cache.h"
#include "CacheFile.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

CacheFile::CacheFile() {
  StartUp();
}

CacheFile::CacheFile(CacheFile const &that) {
  CopyFrom(that);
}

CacheFile::~CacheFile() {
  ShutDown();
}

CacheFile &CacheFile::operator = (CacheFile const &that) {
  CopyFrom(that);
  NotConsumable();
  return *this;
}
  
//----------------------------------------------------------------------
//
// BEFE Lifecyce...
//

Status CacheFile::StartUp() {

  Status status;

  BEFE::SetNull(id);
  openCount  = 0;
  isUsed     = false;
  isFree     = false;
  isWritable = false;
  ignoreBOM  = false;
  status = stream.StartUp();
  BEFE::SetNull(size);
  BEFE::SetNull(nextFileIdx);
  BEFE::SetNull(prevFileIdx);  
  BEFE::SetNull(firstBlockIdx);
  BEFE::SetNull(lastBlockIdx);  
  lastRead.Reset();
  lastModified.Reset();
  lastWrite.Reset();
  numLogReads  = 0;
  numPhyReads  = 0;
  numLogWrites = 0;
  numPhyWrites = 0;
  
  return status;
  
}

Status CacheFile::ShutDown() {

  Status status;
  
  status = stream.ShutDown();
  
  return status;

}

Status CacheFile::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Boolean CacheFile::IsNull() const{
  return stream.GetName().Length();
}

Status CacheFile::SetNull() {
  return Reset();
}

Boolean CacheFile::IsEmpty() const {
  return size == 0 || BEFE::IsNull(size);
}

Status CacheFile::SetEmpty() {
  
  Status retStatus;
  Status status;
  Id     saveId;
  String saveFileName;

  saveId = id;  
  saveFileName = stream.GetName().Consumable();
  retStatus = Reset();
  id = saveId;
  status = stream.SetName(saveFileName.Consumable());
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Status CacheFile::MoveFrom(CacheFile const &that) {
  
  Status status;
  
  if (this != &that) {
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CacheFile));
    status = ((CacheFile *)&that)->StartUp();
  }
  else
    status = Error::None;
    
  return status;

}

Status CacheFile::CopyFrom(CacheFile const &that) {

  Status retStatus;
  Status status;
  UInt   mode;
  
  if (this != &that) {
    if (IsConsumable()) 
      retStatus = MoveFrom(that);
    else {
      retStatus = Reset();
      if (!retStatus) {
        MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CacheFile));
        retStatus = stream.StartUp();
        status = stream.SetName(that.stream.GetName().Consumable());
        if (status && !retStatus) retStatus = status;
        mode = that.stream.GetAccess();
        status = stream.SetAccess(mode);
        if (status && !retStatus) retStatus = status;
      }
    }
  }
  else
    retStatus = Error::None;
    
  return retStatus;

}

CacheFile &CacheFile::Consumable() const {
  return *(CacheFile *)this;
}

CacheFile &CacheFile::NotConsumable() const {
  return *(CacheFile *)this;
}

Boolean CacheFile::IsConsumable() const {
  return false;
}

UInt CacheFile::Length() const {
  return 0;
}

UInt CacheFile::Size() const {
  if (BEFE::IsNull(size))
    ((CacheFile *)this)->size = ((CacheFile *)this)->stream.Size();
  return size;
}

//----------------------------------------------------------------------
//
// Info
//

Status CacheFile::GetFileInfo(Cache &owner, CacheFileInfo &theInfo) const {
  
  Status        status;
  CacheFileInfo localInfo;
  UInt          numUsedBlocks;
  UInt          curBlockIdx;
  CacheBlock   *curBlock;
  UInt          outIdx;
  
  // Sanity checks...
  NEED_TheBefe
  NEED_TheOS
  
  // Do it...
  localInfo.id           = id;
  localInfo.snapTime.Set();
  localInfo.fullName     = stream.GetName();
  localInfo.isOpen       = stream.IsOpen();
  localInfo.access       = stream.GetAccess();
  localInfo.size         = size;
  
  localInfo.lastRead     = lastRead;
  localInfo.lastModified = lastModified;
  localInfo.lastWrite    = lastWrite;
  
  localInfo.numLogReads  = numLogReads;
  localInfo.numPhyReads  = numPhyReads;
  localInfo.numLogWrites = numLogWrites;
  localInfo.numPhyWrites = numPhyWrites;
  
  // Count the Used Block Numbers...
  numUsedBlocks = 0;
  curBlockIdx = owner.firstUsedBlockIdx;
  while (!BEFE::IsNull(curBlockIdx)) {
    curBlock = (CacheBlock *)owner.blocks._GetAddress(curBlockIdx);
    if (BEFE::IsNull(curBlock)) goto NULLPOINTER;
    if (curBlock->fileId == id)
      numUsedBlocks++;
    curBlockIdx = curBlock->nextBlockIdx;
  }
  
  // Now, get the list of used Block Numbers, in MRU order...
  status = localInfo.blockNos.Allocate(Alloc(numUsedBlocks, 0));
  if (status) goto SOMEERROR;
  curBlockIdx = owner.firstUsedBlockIdx;
  outIdx = 0;
  while (!BEFE::IsNull(curBlockIdx)) {
    curBlock = (CacheBlock *)owner.blocks._GetAddress(curBlockIdx);
    if (BEFE::IsNull(curBlock)) goto NULLPOINTER;
    if (curBlock->fileId == id) {
      status = localInfo.blockNos.Set(outIdx++, curBlock->blockNo);
      if (status) goto SOMEERROR;
    }
    curBlockIdx = curBlock->nextBlockIdx;
  }
  
  // Handle errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  // Return theInfo if everything's okay...
  if (!status)
    status = theInfo.CopyFrom(localInfo.Consumable());
    
  return status;
  
}
  
} // ...Namespace BEFE
