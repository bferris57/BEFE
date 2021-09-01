//----------------------------------------------------------------------
// File: MessageRange.h - Implementation of the Message Range Class
//----------------------------------------------------------------------
//
// The MessageRange Class identifies a distinct name of Messages and
// the associated "Short Name".
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Message.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

MessageRange::MessageRange() {
  StartUp();
}

MessageRange::MessageRange(MessageRange const &that) {
  StartUp();
  CopyFrom(that);
}

MessageRange::~MessageRange() {
  ShutDown();
}

MessageRange &MessageRange::operator=(MessageRange const &that) {
  Reset();
  CopyFrom(that);
  NotConsumable();
  return *this;
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status MessageRange::StartUp() {
  name.StartUp();
  range = URange();
  shortName.StartUp();
  return Error::None;
}

Status MessageRange::ShutDown() {

  Status retStatus;
  Status status;

  retStatus = name.ShutDown();
  status = shortName.ShutDown();
  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status MessageRange::Reset() {

  Status retStatus;
  Status status;

  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status MessageRange::MoveFrom(MessageRange const &that) {
  
  Status retStatus;
  Status status;
  
  if (this != &that) {
    retStatus = Reset();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(MessageRange));
    status = ((MessageRange *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
  }
  else
    retStatus = Error::None;
  
  return retStatus;
  
}

Status MessageRange::CopyFrom(MessageRange const &that) {

  Status status;
  
  if (this == &that)
    status = Error::None;
  else {
    if (that.IsConsumable())
      status = MoveFrom(that);
    else {
      status = Reset();
      name      = that.name;
      range     = that.range;
      shortName = that.shortName;
    }
  }
  
  return status;

}
  
Boolean MessageRange::IsNull() const {
  return range.IsNull() && name.IsNull() && shortName.IsNull();
}
  
Status MessageRange::SetNull() {
  return Reset();
}

UInt MessageRange::Length() {
  return 0;
}

UInt MessageRange::Size() {
  return shortName.Size();
}

MessageRange &MessageRange::Consumable() const {
  name.Consumable();
  shortName.Consumable();
  return *((MessageRange *)this);
}

MessageRange &MessageRange::NotConsumable() const {
  name.NotConsumable();
  shortName.NotConsumable();
  return *((MessageRange *)this);
}

Boolean MessageRange::IsConsumable() const {
  return name.IsConsumable() || shortName.IsConsumable();
}

Boolean MessageRange::IsMutable() const {
  return name.IsMutable() || shortName.IsMutable();
}

Status MessageRange::Mutable() {
  Status retStatus;
  Status status;
  retStatus = name.Mutable();
  status = shortName.Mutable();
  if (status && !retStatus) retStatus = status;
  return retStatus;
}

} // ...Namespace BEFE