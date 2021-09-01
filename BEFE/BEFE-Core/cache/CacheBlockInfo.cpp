//----------------------------------------------------------------------
// File: CacheBlockInfo.cpp - CacheBlockInfo Struct implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "CacheBlockInfo.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

CacheBlockInfo::CacheBlockInfo() {
  //Status status;
  /*status =*/ StartUp();
}

CacheBlockInfo::CacheBlockInfo(CacheBlockInfo const &that) {
  
  Status retStatus;
  Status status;

  retStatus = StartUp();
  status = CopyFrom(that);
  if (status && !retStatus) retStatus = status;

}

CacheBlockInfo::~CacheBlockInfo() {
  //Status status;
  /*status =*/ ShutDown();
}

CacheBlockInfo &CacheBlockInfo::operator = (CacheBlockInfo const &that) {

  //Status status;

  /*status =*/ CopyFrom(that);
    
  return NotConsumable();

}
  
  
// BEFE Lifecycle...
Status CacheBlockInfo::StartUp() {

  BEFE::SetNull(index);
  fileId = NULL;
  BEFE::SetNull(blockNo);
  BEFE::SetNull(snapTime);

  isFree  = false;
  isUsed  = false;
  isDirty = false;
  
  BEFE::SetNull(lastRead);
  BEFE::SetNull(lastModified);
  BEFE::SetNull(lastWrite);
  
  BEFE::SetNull(numLogReads);
  BEFE::SetNull(numPhyReads);
  BEFE::SetNull(numLogWrites);
  BEFE::SetNull(numPhyWrites);

  return Error::None;
  
}

Status CacheBlockInfo::ShutDown() {
  return Error::None;
}

Status CacheBlockInfo::Reset() {

  Status status;
  
  status = ShutDown();
  if (!status)
    status = StartUp();
    
  return status;

}

Boolean CacheBlockInfo::IsNull() const {
  return BEFE::IsNull(snapTime);
}

Status CacheBlockInfo::SetNull() {
  return Reset();
}

Boolean CacheBlockInfo::IsEmpty() const {
  return false;
}

Status CacheBlockInfo::SetEmpty() {
  return Reset();
}

Status CacheBlockInfo::MoveFrom(CacheBlockInfo const &that) {

  Status retStatus;
  Status status;
  
  if (this != &that) {
    retStatus = ShutDown();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CacheBlockInfo));
    status = ((CacheBlockInfo *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
  }
  else
    retStatus = Error::None;
    
  return retStatus;

}

Status CacheBlockInfo::CopyFrom(CacheBlockInfo const &that) {

  Status status;
  
  if (this != &that) {
    if (that.IsConsumable())
      status = MoveFrom(that);
    else {
      status = ShutDown();
      MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CacheBlockInfo));
    }
  }
  else
    status = Error::None;
    
  return status;

}

CacheBlockInfo &CacheBlockInfo::Consumable() const {
  return *((CacheBlockInfo *)this);
}

CacheBlockInfo &CacheBlockInfo::NotConsumable() const {
  return *((CacheBlockInfo *)this);
}

Boolean CacheBlockInfo::IsConsumable() const {
  return false;
}

UInt CacheBlockInfo::Length() const {
  return UNaN;
}

UInt CacheBlockInfo::Size() const {
  return UNaN;
}

} // ...Namespace BEFE
