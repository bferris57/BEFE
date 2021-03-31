//----------------------------------------------------------------------
// File: Integer.cpp - Base Implementation of the Integer Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Integer.h"
#include "Buffer.h"

namespace BEFE { // Namespace BEFE...
  
//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Integer::Integer() {
  StartUp();
}

Integer::Integer(Integer const &that) {
  StartUp();
  CopyFrom(that);
}

Integer::Integer(String const &that) {
  StartUp();
  *this = that;
}

Integer::Integer(char   const *that) {
  StartUp();
  *this = that;
}

Integer::Integer(char         *that) {
  StartUp();
  *this = that;
}

Integer::Integer(char val) {
  StartUp();
  *this = (UInt8)val;
}

Integer::Integer(UInt8 val) {
  StartUp();
  *this = val;
}

Integer::Integer(UInt16 val) {
  StartUp();
  *this = val;
}

Integer::Integer(UInt32 val) {
  StartUp();
  *this = val;
}

Integer::Integer(UInt64 val) {
  StartUp();
  *this = val;
}

Integer::Integer(SInt8 val) {
  StartUp();
  *this = val;
}

Integer::Integer(SInt16 val) {
  StartUp();
  *this = val;
}

Integer::Integer(SInt32 val) {
  StartUp();
  *this = val;
}

Integer::Integer(SInt64 val) {
  StartUp();
  *this = val;
}

Integer::~Integer() {
  ShutDown();
}
  
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Integer::StartUp() {

  isMutable    = true;
  isConsumable = false;
  state        = StateNaN;
  type         = TypeNull;
  digit        = 0;

  return Error::None;
  
}

Status Integer::ShutDown() {
  
  Status status;
  
  if (type == TypeBuffer)
    status = ((Buffer *)&buf)->ShutDown();
  else
    status = Error::None;
    
  return status;
}

Status Integer::Reset() {

  BEFE::SetNull(digit);
  
  return Error::None;

}

Boolean Integer::IsNull() const {
  return (type == TypeNull && BEFE::IsNull(digit));
}

Status Integer::SetNull() {
  return Reset();
}

Status Integer::MoveFrom(Integer const &that) {
  
  Status status;

  ShutDown();
  isMutable    = that.isMutable;
  isConsumable = false;
  state        = that.state;
  type         = that.type;
  digit        = that.digit;
  status = ((Integer *)&that)->StartUp();
  
  return status;

}

Status Integer::CopyFrom(Integer const &that) {
  
  Status status;

  // If consumable...
  if (that.isConsumable) { // Consume it...
    status = MoveFrom(that);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Not consumable, copy it...
  isMutable    = that.isMutable;
  isConsumable = false;
  state        = that.state;
  type         = that.type;

  if (type == TypeBuffer) {
    buf = NULL;
    status = ((Buffer *)&buf)->CopyFrom(*(Buffer *)&that.buf);
    if (status) goto SOMEERROR;
  }
  else if (type == TypeDigit) {
    digit = that.digit;
  }
  else
    digit = UNaN;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }
  return status;

}

Integer &Integer::Consumable() const {
  return *((Integer *)this);
}

String Integer::ToString() const {
  
  String string;
  Digit  rem;
  
  switch (state) {
    case StateNegNatural:
      string = "-";
    case StateNatural:
      string = "";
      break;
    case StateInfinity:
      if (gConsoleUTF8)
        string = "∞";
      else
        string = "Inf";
      break;
    case StateZero:
      string = '0';
      break;
    case StateNaN:
      string = "NaN";
      break;
    case StateNegInfinity:
      if (gConsoleUTF8)
        string = "-∞";
      else
        string = "-Inf";
      break;
    case StateNegZero:
      string = "-0";
      break;
    case StateNegNaN:
      string = "-NaN";
      break;
    default:
      string = "???";
      break;
  }
  
  if ((state & 0x03) == 0) {
    
    if (type == TypeDigit) { // Digit type...
      rem = digit;
      do {
        string += Char('0' + rem%10);
        rem /= 10;
      } while (rem);
      string.Reverse();
    } // ...Digit type
    
    else { // Buffer type...
      string += "<Num>";      
    } // ...Buffer type
  }
  
  return string.Consumable();

}

} // ...Namespace BEFE