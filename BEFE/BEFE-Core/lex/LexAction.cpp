//----------------------------------------------------------------------
// File: LexAction.cpp - Implementation of the BEFE LexAction Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "LexAction.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LexAction::LexAction() {
  StartUp();
}

LexAction::LexAction(LexAction const &that) {
  StartUp();
  CopyFrom(that);
}

LexAction::~LexAction() {
  ShutDown();
}

LexAction &LexAction::operator=(LexAction const &that) {
  CopyFrom(that);
  return *this;
}

//----------------------------------------------------------------------
//	
// BEFE Lifecycle
//

Status LexAction::StartUp() {

  Status status;

  BEFE::SetNull(id);
  status = name.StartUp();
  BEFE::SetNull(*(void **)&charHandler);

  return status;

}

Status LexAction::ShutDown() {

  Status status;

  status = name.ShutDown();

  return status;

}

Status LexAction::Clear() {
  BEFE::SetNull(*(void **)&charHandler);
  return Error::None;
}  

Status LexAction::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Boolean LexAction::IsNull() const {
  
  return BEFE::IsNull(id)   &&
         BEFE::IsNull(name) &&
         BEFE::IsNull(*(void **)&charHandler);
  
}

Status LexAction::SetNull() {
  return Reset();
}

Boolean LexAction::IsEmpty() const {
  
  return BEFE::IsNull(charHandler);

}

Status LexAction::SetEmpty() {
  
  BEFE::SetNull(*(void **)&charHandler);
  
  return Error::None;
  
}

Boolean LexAction::IsConsumable() const {
  return name.IsConsumable();
}

LexAction &LexAction::Consumable() {
  name.Consumable();
  return *this;
}

LexAction &LexAction::NotConsumable() {
  name.NotConsumable();
  return *this;
}

Status LexAction::MoveFrom(LexAction const &that) {

  Status status;

  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexAction));
  status = ((LexAction *)&that)->StartUp();
  NotConsumable();
  
  return status;
  
}

Status LexAction::CopyFrom(LexAction const &that) {

  Status status;
  
  status = Error::None;
  if (this != (LexAction *)&that) {
    if (IsConsumable())
      status = MoveFrom(that);
    else {
      id = that.id;
      status = name.CopyFrom(that.name);
      charHandler = that.charHandler;
    }
  }

  return status;
  
}

//----------------------------------------------------------------------
//	
// ToString methods
//

String LexAction::ToString() const {
  
  String out;
  
  out = "LexAction: ";
  if (IsNull())
    out += CONSTR_StarNull;
  else {
    out += "id = ";
    if (BEFE::IsNull(id))
      out += CONSTR_Null;
    else
      out += String("0x") << ToHexString((UInt)id);
    out += ", name = ";
    if (name.IsNull())
      out += CONSTR_Null;
    else
      out += String("'") << name << "'";
    out += ", handler = ";
    if ( BEFE::IsNull((Byte *)charHandler) )
      out += CONSTR_Null;
    else
      out += String("0x") << ToHexString((UInt)charHandler, 8);
  }
  
  return out.Consumable();
  
}

} // ...Namespace BEFE