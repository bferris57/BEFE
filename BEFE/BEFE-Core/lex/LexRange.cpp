//----------------------------------------------------------------------
// File: LexRange.cpp - Implementation of the BEFE LexRange Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEWrappers.h"
#include "ToStrings.h"
#include "LexRange.h"
#include "Lex.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LexRange::LexRange() {
  StartUp();
}

LexRange::LexRange(LexRange const &that) {
  StartUp();
  CopyFrom(that);
}

LexRange::~LexRange() {
  ShutDown();
}

LexRange &LexRange::operator=(LexRange const &that) {
  StartUp();
  CopyFrom(that);
  return *this;
}

//----------------------------------------------------------------------
//	
// BEFE Lifecycle
//

Status LexRange::StartUp() {

  BEFE::SetNull(range.idx1);
  BEFE::SetNull(range.idx2);
  actionNo = Lex::ActionUnhandled;

  return Error::None;

}

Status LexRange::ShutDown() {
  return Error::None;
}

Status LexRange::Clear() {
  return Reset();
}  

Status LexRange::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Boolean LexRange::IsNull() const {
  return (BEFE::IsNull(range.idx1) &&
          BEFE::IsNull(range.idx2) &&
          BEFE::IsNull(actionNo));
}

Status LexRange::SetNull() {
  return Reset();
}

Boolean LexRange::IsEmpty() const {
  return IsNull();
}

Status LexRange::SetEmpty() {
  return Reset();
}

Boolean LexRange::IsConsumable() const {
  return false;
}

LexRange &LexRange::Consumable() {
  return *this;
}

LexRange &LexRange::NotConsumable() {
  return *this;
}

Status LexRange::MoveFrom(LexRange const &that) {

  Status status;
  
  range    = that.range;
  actionNo = that.actionNo;
  
  status = ((LexRange *)&that)->StartUp();
  
  return status;
  
}

Status LexRange::CopyFrom(LexRange const &that) {

  Status status;
  
  status = Error::None;
  if (this != (LexRange *)&that) {
    if (IsConsumable())
      status = MoveFrom(that);
    else {
      range    = that.range;
      actionNo = that.actionNo;
    }
  }

  return status;
  
}

} // ...Namespace BEFE