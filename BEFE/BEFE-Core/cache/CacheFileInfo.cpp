//----------------------------------------------------------------------
// File: CacheFileInfo.cpp - CacheFileInfo Struct implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "CacheFileInfo.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

CacheFileInfo::CacheFileInfo() {
  //Status status;
  /*status =*/ StartUp();
}

CacheFileInfo::CacheFileInfo(CacheFileInfo const &that) {
  
  Status retStatus;
  Status status;

  retStatus = StartUp();
  status = CopyFrom(that);
  if (status && !retStatus) retStatus = status;

}

CacheFileInfo::~CacheFileInfo() {
  //Status status;
  /*status =*/ ShutDown();
}

CacheFileInfo &CacheFileInfo::operator = (CacheFileInfo const &that) {

  //Status status;

  /*status =*/ CopyFrom(that);
    
  return NotConsumable();

}
  
  
// BEFE Lifecycle...
Status CacheFileInfo::StartUp() {

  Status retStatus;
  Status status;
  
  BEFE::SetNull(id);
  BEFE::SetNull(snapTime);
  retStatus = fullName.StartUp();
  BEFE::SetNull(fullName);
  isOpen = false;
  access = File::ModeNone;
  status = blockNos.StartUp();
  if (status && !retStatus) retStatus = status;
  BEFE::SetNull(size);
  
  BEFE::SetNull(lastRead);
  BEFE::SetNull(lastModified);
  BEFE::SetNull(lastWrite);
  
  BEFE::SetNull(numLogReads);
  BEFE::SetNull(numPhyReads);
  BEFE::SetNull(numLogWrites);
  BEFE::SetNull(numPhyWrites);

  return retStatus;
  
}

Status CacheFileInfo::ShutDown() {
  
  Status retStatus;
  Status status;
  
  retStatus = fullName.ShutDown();
  status = blockNos.ShutDown();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Status CacheFileInfo::Reset() {

  Status status;
  
  status = ShutDown();
  if (!status)
    status = StartUp();
    
  return status;

}

Boolean CacheFileInfo::IsNull() const {
  return BEFE::IsNull(snapTime);
}

Status CacheFileInfo::SetNull() {
  return Reset();
}

Boolean CacheFileInfo::IsEmpty() const {
  return fullName.IsEmpty();
}

Status CacheFileInfo::SetEmpty() {

  Status retStatus;
  Status status;
  
  retStatus = Reset();
  status = fullName.SetEmpty();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Status CacheFileInfo::MoveFrom(CacheFileInfo const &that) {

  Status retStatus;
  Status status;
  
  if (this != &that) {
    retStatus = ShutDown();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CacheFileInfo));
    status = ((CacheFileInfo *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
  }
  else
    retStatus = Error::None;
    
  return retStatus;

}

Status CacheFileInfo::CopyFrom(CacheFileInfo const &that) {
  Status retStatus;
  Status status;
  
  if (this != &that) {
    if (that.IsConsumable())
      retStatus = MoveFrom(that);
    else {
      retStatus = ShutDown();
      MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CacheFileInfo));
      status = fullName.StartUp();
      if (status && !retStatus) retStatus = status;
      status = fullName.CopyFrom(that.fullName);
      if (status && !retStatus) retStatus = status;
      status = blockNos.StartUp();
      if (status && !retStatus) retStatus = status;
      status = blockNos.CopyFrom(that.blockNos);
      if (status && !retStatus) retStatus = status;
    }
  }
  else
    retStatus = Error::None;
    
  return retStatus;

}

CacheFileInfo &CacheFileInfo::Consumable() const {
  return *((CacheFileInfo *)this);
}

CacheFileInfo &CacheFileInfo::NotConsumable() const {
  return *((CacheFileInfo *)this);
}

Boolean CacheFileInfo::IsConsumable() const {
  return false;
}

UInt CacheFileInfo::Length() const {
  return UNaN;
}

UInt CacheFileInfo::Size() const {
  return UNaN;
}

//----------------------------------------------------------------------
//
// Formatting
//

Status CacheFileInfo::Display() {
  return Display(0);
}

Status CacheFileInfo::Display(UInt indent) {
  return Display(Cout, indent);
}

Status CacheFileInfo::Display(Console &out, UInt pIndent) {

  String indent;
  
  indent = String(' ')*pIndent;
  
  out << indent << "Cache File Id   = "  << (UInt)id               << '\n';
  out << indent << "SnapShot Taken  = "  << ToString(snapTime,Time::Milliseconds)  << '\n';
  out << indent << "Name            = '" << fullName               << "'\n";
  out << indent << "State           = "  << (isOpen ? CONSTR_Open : CONSTR_Closed) << '\n';
  out << indent << "Size            = "  << size                   << '\n';
  out << indent << "Last Read       = "  << ToString(lastRead,Time::Milliseconds)  << '\n';
  out << indent << "Last Modified   = "  << ToString(lastModified,Time::Milliseconds) << '\n';
  out << indent << "Last Written    = "  << ToString(lastWrite,Time::Milliseconds) << '\n';
  out << indent << "Logical  Reads  = "  << numLogReads            << '\n';
  out << indent << "Physical Reads  = "  << numPhyReads            << '\n';
  out << indent << "Logical  Writes = "  << numLogWrites           << '\n';
  out << indent << "Physical Writes = "  << numPhyWrites           << '\n';

  return Error::None;
  
}
  
} // ...Namespace BEFE