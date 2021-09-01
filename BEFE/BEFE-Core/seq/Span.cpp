//----------------------------------------------------------------------
// File: Span.cpp - Implementaion of the Span class
//----------------------------------------------------------------------
//
// NOTE: This code is EXACTLY the same as Range.cpp except for the
//       following...
//
//         o The Class name is Span instead of Range (duh!)
//         o Length() doesn't subtract 1
//         o Parse separates with ':' instead of '..'
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Span.h"
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Span::Span() {
  StartUp();
}

Span::Span(Int index1) {
  idx1 = index1;
  idx2 = NaN;
}

Span::Span(Int index1, Int index2) {
  idx1 = index1;
  idx2 = index2;
}

Span::Span(char const *str) {
  String sstr = str;
  StartUp();
  Set(sstr);
}

Span::Span(String const &str) {
  StartUp();
  ((Span *)this)->FromString(str);
}

Span::Span(Span const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
}

Span &Span::operator=(Span const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
  return *this;
}

//----------------------------------------------------------------------
//
// BEFE lifecycle
//

Status Span::StartUp() {

  idx1  = NaN;
  idx2  = NaN;

  return Error::None;
}

Boolean Span::IsNull() const {
  return (BEFE::IsNull(idx1) && BEFE::IsNull(idx2));
}

Boolean Span::IsEmpty() const {
  return (!BEFE::IsNull(idx1) && !BEFE::IsNull(idx2) && idx1 == idx2-1);
}

Status Span::SetEmpty() {
  if (BEFE::IsNull(idx1))
    idx2 = idx1;
  else
    idx2 = idx1;
  return Error::None;
}

Status Span::CopyFrom(Span const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
  return Error::None;
}

Status Span::MoveFrom(Span const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
  ((Span *)&that)->StartUp();
  return Error::None;
}

UInt Span::Length() const {
  if ( BEFE::IsNull(idx1) || BEFE::IsNull(idx2) )
    return UNaN;
  if (idx2 < idx1)
    return idx1-idx2;
  else
    return idx2-idx1;
}

UInt Span::Size() const {
  return 0;
}

//----------------------------------------------------------------------
//
// Span Setting
//

Status Span::Set(String const &str) {
  return FromString(str);
}

Status Span::Set(char const *str) {
  return FromString(str);
}  

//----------------------------------------------------------------------
//
// Span resolving...
//

Boolean Span::IsResolved() const {
  if ( BEFE::IsNull(idx1) || BEFE::IsNull(idx2) ||
      idx1 < 0     || idx2 < -1)
    return false;
  return true;
}

Span Span::Resolve(UInt len) const {

  Span that;

  that = *this;

  if (len == UNaN) goto DONE;

  // Special case for Span(-N:)...
  if (!BEFE::IsNull(that.idx1) && that.idx1 < 0 && BEFE::IsNull(that.idx2)) {
    that.idx1 = that.idx1 + len;
    if (that.idx1 < 0)
      that.idx1 = Max(0,len+that.idx1);
    that.idx2 = len;
    goto DONE;
  }
  
  // "Normal" cases...
  if ( BEFE::IsNull(that.idx1) )
    that.idx1 = 0;  
  if (that.idx1 < 0) that.idx1 = that.idx1 + len;
  if ( BEFE::IsNull(that.idx2) )
    that.idx2 = len;
  if (that.idx2 < 0) that.idx2 = that.idx2 + len;
  if (that.idx1 < 0)
    that.idx1 = 0;
  if (that.idx1 > (Int)len)
    that.idx1 = (Int)len;
  if (that.idx2 < 0)
    that.idx2 = -1;
  if (that.idx2 > (Int)len)
    that.idx2 = (Int)len;

  while (false) {
    DONE: break;
  }
  
  return that;

}

Boolean Span::InSpan(Int index) const {
  
  UInt len;
  len = Length();
  if ( BEFE::IsNull(index) || BEFE::IsNull(len) || BEFE::IsNull(idx1) ) 
    return false;
  if ( BEFE::IsNull(idx2) )
    return true;
  if (index < 0)
    index = index + len;
  if (index < 0 || (UInt)index >= len)
    return false;
  if (idx1 > idx2) // Backward...
    return index >= idx2 && index <= idx1;
  return index >= idx1 && index <= idx2;
}

//----------------------------------------------------------------------
//
// Resolved Span Order
//
  
Boolean Span::IsAscending() const {
  if (!IsResolved()) 
    return false;
  return (idx1 <= idx2 || idx1 == idx2+1);
}

Boolean Span::IsDescending() const {
  if (!IsResolved()) 
    return false;
  return (idx2 < idx1 && idx2 != idx1-1);
}

Status  Span::Reverse() {

  if (!IsResolved())
    return Error::SeqSpanNotResolved;

  if (IsAscending()) {
    idx1--;
    idx2--;
  }
  else {
    idx1++;
    idx2++;
  }
  idx1 ^= idx2;
  idx2 ^= idx1;
  idx1 ^= idx2;
  
  return Error::None;

}
  
//----------------------------------------------------------------------
//
// String representation
//
//----------------------------------------------------------------------

String Span::ToString() {

  Byte   buf[128];
  String str;
  Span   saveSpan;

  saveSpan = *this;

  buf[0] = 0;
  if (!BEFE::IsNull(idx1) )
    Itoa(idx1,buf+Strlen(buf),10);
  Strcat(buf,":");
  if ( !BEFE::IsNull(idx2) )
    Itoa(idx2,buf+Strlen(buf),10);

  str = (char *)buf;
  *this = saveSpan;

  return str;

}

String Span::ToHexString() {

  Byte   buf[128];
  String str;
  Span   saveSpan;

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

Status Span::FromString(String const &str) {

  Status status;
  Range  range;
  
  status = Range::Parse(str,range,":");
  if (!status) {
    idx1 = range.idx1;
    idx2 = range.idx2;
  }
  
  return status;
}

Status Span::FromString(const char *str) {

  Status status;
  String string;
  Range  range;
  
  string = str;
  status = Range::Parse(string,range,":");
  if (!status) {
    idx1 = range.idx1;
    idx2 = range.idx2;
  }
  
  return status;
}

} // ...Namespace BEFE