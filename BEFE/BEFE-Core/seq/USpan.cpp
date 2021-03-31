//----------------------------------------------------------------------
// File: USpan.cpp - Implementaion of the USpan class
//----------------------------------------------------------------------
//
// NOTE: This code is EXACTLY the same as URange.cpp except for the
//       following...
//
//         o The Class name is USpan instead of URange (duh!)
//         o Length() doesn't subtract 1
//         o Parse separates with ':' instead of '..'
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "USpan.h"
#include "URange.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

USpan::USpan() {
  StartUp();
}

USpan::USpan(UInt index1) {
  StartUp();
  idx1 = index1;
}

USpan::USpan(UInt index1, UInt index2) {
  StartUp();
  idx1 = index1;
  idx2 = index2;
}

USpan::USpan(char const *str) {
  String sstr = str;
  StartUp();
  Set(sstr);
}

USpan::USpan(String const &str) {
  StartUp();
  ((USpan *)this)->FromString(str);
}

USpan::USpan(USpan const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
}

USpan &USpan::operator=(USpan const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
  return *this;
}

//----------------------------------------------------------------------
//
// BEFE lifecycle
//

Status USpan::StartUp() {

  idx1  = UNaN;
  idx2  = UNaN;

  return Error::None;
}

Boolean USpan::IsNull() const {
  return (BEFE::IsNull(idx1) && BEFE::IsNull(idx2));
}

Boolean USpan::IsEmpty() const {
  return (!BEFE::IsNull(idx1) && !BEFE::IsNull(idx2) && idx1 == idx2-1);
}

Status USpan::SetEmpty() {
  if (BEFE::IsNull(idx1))
    idx2 = idx1;
  else
    idx2 = idx1;
  return Error::None;
}

Status USpan::CopyFrom(USpan const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
  return Error::None;
}

Status USpan::MoveFrom(USpan const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
  ((USpan *)&that)->StartUp();
  return Error::None;
}

UInt USpan::Length() const {

  if ( BEFE::IsNull(idx1) ||  BEFE::IsNull(idx2) )
    //return UNaN;
    return 0;
  if (idx2 < idx1)
    return idx1-idx2;
  else
    return idx2-idx1;

}

UInt USpan::Size() const {
  return 0;
}

//----------------------------------------------------------------------
//
// USpan Setting
//

Status USpan::Set(String const &str) {
  return FromString(str);
}

Status USpan::Set(char const *str) {
  return FromString(str);
}  

//----------------------------------------------------------------------
//
// USpan resolving...
//

USpan USpan::Resolve(UInt len) const {

  USpan that;

  that = *this;

  if ( BEFE::IsNull(len) ) goto DONE;
  
  if ( BEFE::IsNull(that.idx1) )
    that.idx1 = 0;
  if (that.idx1 > len)
    that.idx1 = len;
  if (that.idx2 > len)
    that.idx2 = len;

  while (false) {
    DONE: break;
  }
  
  return that;

}

Boolean USpan::InSpan(UInt index) const {
  
  UInt len;

  len = Length();
  if ( BEFE::IsNull(index) || BEFE::IsNull(len) || BEFE::IsNull(idx1) ) 
    return false;
  if ( BEFE::IsNull(idx2) )
    return true;
  if (index < 0)
    index = index + len;
  if (index < 0 || index >= len)
    return false;
  if (idx1 > idx2) // Backward...
    return index >= idx2 && index <= idx1;
  return index >= idx1 && index <= idx2;

}

//----------------------------------------------------------------------
//
// String representation
//

String USpan::ToString() {

  Byte   buf[128];
  char  *cp;
  String str;
  USpan  saveSpan;

  saveSpan = *this;

  if (idx1 >= 0x20 && idx1 <= 0x7f && idx2 >= 0x20 && idx2 <= 0x7f) {
    cp = (char *)buf;
    *cp++ = '\'';
    *cp++ = idx1;
    *cp++ = '\'';
    *cp++ = ':';
    *cp++ = '\'';
    *cp++ = idx2;
    *cp++ = '\'';
    *cp++ = 0x00;
    goto DONE;
  }
  else {
    buf[0] = 0;
    if ( !BEFE::IsNull(idx1) )
      Itoa(idx1,buf+Strlen(buf),10);
    Strcat(buf,":");
    if ( !BEFE::IsNull(idx2) )
      Itoa(idx2,buf+Strlen(buf),10);
  }

  while (false) {
    DONE: break;
  }

  str = (char *)buf;
  *this = saveSpan;

  return str;

}

String USpan::ToHexString() {

  Byte   buf[128];
  String str;
  USpan  saveSpan;

  saveSpan = *this;

  buf[0] = 0;
  if ( !BEFE::IsNull(idx1) ) {
    Strcat(buf,"0x");
    Itoa(idx1,buf+Strlen(buf),16);
  }
  Strcat(buf,":");
  if ( !BEFE::IsNull(idx2) ) {
    Strcat(buf,"0x");
    Itoa(idx2,buf+Strlen(buf),16);
  }

  str = (char *)buf;
  *this = saveSpan;

  return str;

}

Status USpan::FromString(String const &str) {

  Status status;
  URange range;
  
  status = URange::Parse(str, range, ":");
  if (!status) {
    idx1 = range.idx1;
    idx2 = range.idx2;
  }
  
  return status;
}

Status USpan::FromString(const char *str) {

  Status status;
  String string;
  URange range;

  string = str;
  status = URange::Parse(string, range, ":");
  if (!status) {
    idx1 = range.idx1;
    idx2 = range.idx2;
  }

  return status;
}

} // ...Namespace BEFE