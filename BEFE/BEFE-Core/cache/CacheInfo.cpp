//----------------------------------------------------------------------
// File: CacheInfo.cpp - CacheInfo Struct implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "CacheInfo.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

CacheInfo::CacheInfo() {
  //Status status;
  /*status =*/ StartUp();
}

CacheInfo::CacheInfo(CacheInfo const &that) {
  
  Status retStatus;
  Status status;

  retStatus = StartUp();
  status = CopyFrom(that);
  if (status && !retStatus) retStatus = status;

}

CacheInfo::~CacheInfo() {
  //Status status;
  /*status =*/ ShutDown();
}

CacheInfo &CacheInfo::operator = (CacheInfo const &that) {

  //Status status;

  /*status =*/ CopyFrom(that);
    
  return NotConsumable();

}
  
  
// BEFE Lifecycle...
Status CacheInfo::StartUp() {

  BEFE::SetNull(id);
  BEFE::SetNull(snapTime);
  BEFE::SetNull(blockSize);
  BEFE::SetNull(maxBlocks);
  BEFE::SetNull(fileInfos);
  BEFE::SetNull(blockInfos);
  BEFE::SetNull(usedBlockIndexes);
  BEFE::SetNull(freeBlockIndexes);

  // I/O Statistics
  BEFE::SetNull(numDirtyBlocks);
  BEFE::SetNull(numLogReads);
  BEFE::SetNull(numPhyReads);
  BEFE::SetNull(numLogWrites);
  BEFE::SetNull(numPhyWrites);

  return Error::None;
  
}

Status CacheInfo::ShutDown() {
  
  Status retStatus;
  Status status;
  
  retStatus = fileInfos.ShutDown();
  status = blockInfos.ShutDown();
  if (status && !retStatus) status = retStatus;
  status = usedBlockIndexes.ShutDown();
  if (status && !retStatus) status = retStatus;
  status = freeBlockIndexes.ShutDown();
  if (status && !retStatus) status = retStatus;
  
  return retStatus;

}

Status CacheInfo::Reset() {

  Status status;
  
  status = ShutDown();
  if (!status)
    status = StartUp();
    
  return status;

}

Boolean CacheInfo::IsNull() const {
  return BEFE::IsNull(snapTime);
}

Status CacheInfo::SetNull() {
  return Reset();
}

Boolean CacheInfo::IsEmpty() const {
  return BEFE::IsNull(snapTime);
}

Status CacheInfo::SetEmpty() {

  Status status;

  status = Reset();
  
  return status;
  
}

Status CacheInfo::MoveFrom(CacheInfo const &that) {

  Status retStatus;
  Status status;
  
  if (this != &that) {
    retStatus = ShutDown();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CacheInfo));
    status = ((CacheInfo *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
  }
  else
    retStatus = Error::None;
    
  return retStatus;

}

Status CacheInfo::CopyFrom(CacheInfo const &that) {

  Status retStatus;
  Status status;
  
  if (this != &that) {
    if (that.IsConsumable())
      retStatus = MoveFrom(that);
    else {
      retStatus = ShutDown();
      MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CacheInfo));
      retStatus = fileInfos.StartUp();
      status = fileInfos.CopyFrom(that.fileInfos);
      if (status && !retStatus) status = retStatus;
      status = blockInfos.StartUp();
      if (status && !retStatus) status = retStatus;
      status = blockInfos.CopyFrom(that.blockInfos);
      if (status && !retStatus) status = retStatus;
      status = usedBlockIndexes.StartUp();
      if (status && !retStatus) status = retStatus;
      status = usedBlockIndexes.CopyFrom(that.usedBlockIndexes);
      if (status && !retStatus) status = retStatus;
      retStatus = freeBlockIndexes.StartUp();
      if (status && !retStatus) status = retStatus;
      status = freeBlockIndexes.CopyFrom(that.freeBlockIndexes);
      if (status && !retStatus) status = retStatus;
    }
  }
  else
    retStatus = Error::None;
    
  return retStatus;

}

CacheInfo &CacheInfo::Consumable() const {
  return *((CacheInfo *)this);
}

CacheInfo &CacheInfo::NotConsumable() const {
  return *((CacheInfo *)this);
}

Boolean CacheInfo::IsConsumable() const {
  return false;
}

UInt CacheInfo::Length() const {
  return UNaN;
}

UInt CacheInfo::Size() const {
  return UNaN;
}

//----------------------------------------------------------------------
//
// Formatting
//

Status CacheInfo::Display(Console &out, UInt pIndent) {

  Status status;
  String indent;
  UInt   fg;
  
  // Sanity checks...
  NEED_TheBefe
  NEED_TheOS

  fg = out.GetForegroundColour();
  indent  = String(' ')*pIndent;

  // If we're Null/Empty, nothing to do
  if (IsNull()) {
    out.SetForegroundColour(Console::Yellow);
    out << indent << CONSTR_StarEmpty << '\n';
    out.SetForegroundColour(fg);
    goto OK;
  }
  
  out << indent << "Cache    Id     = " << (UInt)id                  << '\n';
  out << indent << "SnapShot Taken  = " << ToString(snapTime,Time::Milliseconds) << '\n';
  out << indent << "Used     Files  = " << fileInfos.Length()        << '\n';
  out << indent << "Block    Size   = " << blockSize                 << " (" << CONSTR_Bytes << ")\n";
  out << indent << "Maximum  Blocks = " << maxBlocks                 << '\n';
  out << indent << "Current  Blocks = " << blockInfos.Length()       << '\n';
  out << indent << "Used     Blocks = " << usedBlockIndexes.Length() << '\n';
  out << indent << "Free     Blocks = " << freeBlockIndexes.Length() << '\n';
  out << indent << "Dirty    Blocks = " << numDirtyBlocks            << '\n';
  out << indent << "Logical  Reads  = " << numLogReads               << '\n';
  out << indent << "Physical Reads  = " << numPhyReads               << '\n';
  out << indent << "Logical  Writes = " << numLogWrites              << '\n';
  out << indent << "Physical Writes = " << numPhyWrites              << '\n';
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK: status = Error::None; break;
  }
  
  return status;
  
}

} // ...Namespace BEFE