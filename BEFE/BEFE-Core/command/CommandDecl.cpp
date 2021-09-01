//----------------------------------------------------------------------
// File: CommandDecl.cpp - Implementaion of the CommandDecl Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "CommandDecl.h"

namespace BEFE { // Namespace BEFE...

// C++ Lifecycle
CommandDecl::CommandDecl() {
  StartUp();
}

CommandDecl::~CommandDecl() {
  ShutDown();
}

CommandDecl::CommandDecl(CommandDecl const &that) {
  CopyFrom(that);
}

CommandDecl &CommandDecl::operator = (CommandDecl const &that) {
  CopyFrom(that);
  return *this;
}

// BEFE Lifecycle
Status CommandDecl::StartUp() {
  
  Status status;
  
  status = name.StartUp();
  handler = NULL;
  
  return status;
  
}

Status CommandDecl::ShutDown() {
  
  Status status;
  
  status = name.ShutDown();
  
  return status;
  
}

Boolean CommandDecl::IsNull() const {
  
  return name.IsNull() && BEFE::IsNull(handler);
  
}

Status CommandDecl::SetNull() {

  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Boolean CommandDecl::IsEmpty() const {
  return IsNull();
}

Status CommandDecl::SetEmpty() {
  return SetNull();
}

Status CommandDecl::MoveFrom(CommandDecl const &that) {
  
  Status status;
  
  if (this != (CommandDecl *)&that) {
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CommandDecl));
    status = ((CommandDecl *)&that)->StartUp();
    NotConsumable();
  }
  else
    status = Error::None;
  
  return status;
  
}

Status CommandDecl::CopyFrom(CommandDecl const &that) {
  
  Status status;
  
  if (this != (CommandDecl *)&that) {

    if (IsConsumable())
      status = MoveFrom(that);
    else {
      status = name.CopyFrom(that.name);
      handler = that.handler;
    }

  }
  
  return status;
  
}

Boolean CommandDecl::IsConsumable() const {
  return name.IsConsumable();
}

CommandDecl &CommandDecl::Consumable() const {
  name.Consumable();
  return *((CommandDecl *)this);
}

CommandDecl &CommandDecl::NotConsumable() const {
  name.NotConsumable();
  return *((CommandDecl *)this);
}
  
UInt CommandDecl::Length() const {
  return 0;
}

UInt CommandDecl::Size() const {
  return 0;
}

} // ...Namespace BEFE