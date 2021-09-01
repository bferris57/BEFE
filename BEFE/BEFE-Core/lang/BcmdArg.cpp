//----------------------------------------------------------------------
// File: BcmdArg.cpp - Implementation of the BcmdArg Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Bcmd.h"

namespace BEFE {// Namespace BEFE...

//----------------------------------------------------------------------
//
// BEFE Lifecycle...
//

Status BcmdArg::StartUp() {
  
  Status status;
  
  type = TypeUnknown;
  BEFE::SetNull(lineNo);
  BEFE::SetNull(colNo);
  this->status = Error::None;
  status = value.StartUp();
  
  return status;
  
}

Status BcmdArg::ShutDown() {
  
  Status status;
  
  status = value.ShutDown();
  
  return status;
  
}

Status BcmdArg::Reset() {

  Status retStatus;
  Status status;

  retStatus = ShutDown();
  status    = StartUp();

  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status BcmdArg::MoveFrom(BcmdArg const &that) {
  
  Status retStatus;
  Status status;

  if ((BcmdArg *)&that == this)
    retStatus = Error::None;
  else {
    retStatus = ShutDown();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(BcmdArg));
    status = ((BcmdArg *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
  }
  NotConsumable();
  
  return retStatus;
  
}

Status BcmdArg::CopyFrom(BcmdArg const &that) {
  
  Status status;
  
  if (this == (BcmdArg *)&that)
    status = Error::None;
  else {
    if (IsConsumable())
      status = MoveFrom(that);
    else {
      type   = that.type;
      lineNo = that.lineNo;
      colNo  = that.colNo;
      this->status = that.status;
      status = value.CopyFrom(that.value);
    }
  }
  
  return status;
  
}

Boolean BcmdArg::IsNull() const {
  return (
    type == TypeUnknown  &&
    BEFE::IsNull(lineNo) &&
    BEFE::IsNull(colNo)  &&
    value.IsNull()
  );
}

Status BcmdArg::SetNull() {
  return Reset();
}

Boolean BcmdArg::IsEmpty() const {
  return IsNull();
}

Status BcmdArg::SetEmpty() {
  return Reset();
}

UInt BcmdArg::Length() {
  return value.Length();
}

UInt BcmdArg::Size() {
  return value.Size();
}

Boolean BcmdArg::IsConsumable() const {
  return value.IsConsumable();
}

BcmdArg &BcmdArg::Consumable() const {
  value.Consumable();
  return *((BcmdArg *)this);
}

BcmdArg &BcmdArg::NotConsumable() const {
  value.NotConsumable();
  return *((BcmdArg *)this);
}

} // ...Namespace BEFE