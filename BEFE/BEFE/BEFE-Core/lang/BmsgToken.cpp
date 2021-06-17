//----------------------------------------------------------------------
// File: BmsgToken.h - Implementation of the BmsgToken Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Bmsg.h"

namespace BEFE {// Namespace BEFE...

//----------------------------------------------------------------------
//
// BEFE Lifecycle...
//

Status BmsgToken::StartUp() {
  
  Status status;
  
  BEFE::SetNull(type);
  BEFE::SetNull(lineNo);
  BEFE::SetNull(colNo);
  status = value.StartUp();
  
  return status;
  
}

Status BmsgToken::ShutDown() {
  
  Status status;
  
  status = value.ShutDown();
  
  return status;
  
}

Status BmsgToken::Reset() {

  Status retStatus;
  Status status;

  retStatus = ShutDown();
  status    = StartUp();

  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status BmsgToken::MoveFrom(BmsgToken const &that) {
  
  Status retStatus;
  Status status;

  if ((BmsgToken *)&that == this)
    retStatus = Error::None;
  else {
    retStatus = ShutDown();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(BmsgToken));
    status = ((BmsgToken *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
  }
  NotConsumable();
  
  return retStatus;
  
}

Status BmsgToken::CopyFrom(BmsgToken const &that) {
  
  Status status;
  
  if (this == (BmsgToken *)&that)
    status = Error::None;
  else {
    if (IsConsumable())
      status = MoveFrom(that);
    else {
      type   = that.type;
      lineNo = that.lineNo;
      colNo  = that.colNo;
      status = value.CopyFrom(that.value);
    }
  }
  
  return status;
  
}

Boolean BmsgToken::IsNull() const {
  return (
    BEFE::IsNull(type)   &&
    BEFE::IsNull(lineNo) &&
    BEFE::IsNull(colNo)  &&
    value.IsNull()
  );
}

Status BmsgToken::SetNull() {
  return Reset();
}

Boolean BmsgToken::IsEmpty() const {
  return IsNull();
}

Status BmsgToken::SetEmpty() {
  return Reset();
}

UInt BmsgToken::Length() {
  return value.Length();
}

UInt BmsgToken::Size() {
  return value.Size();
}

Boolean BmsgToken::IsConsumable() const {
  return value.IsConsumable();
}

BmsgToken &BmsgToken::Consumable() const {
  value.Consumable();
  return *((BmsgToken *)this);
}

BmsgToken &BmsgToken::NotConsumable() const {
  value.NotConsumable();
  return *((BmsgToken *)this);
}

} // ...Namespace BEFE