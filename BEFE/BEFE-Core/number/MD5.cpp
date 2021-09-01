//----------------------------------------------------------------------
// File: MD5.cpp - Struct MD5 implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// Note: And MD5 instance is simply a holding area for a calculated
//       MD5 Checksum value.  These can be populated from various
//       sources but, in general, by using the MD5Calc struct.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "MD5.h"
#include "ToString.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status MD5::StartUp() {
  MemoryZeroRaw(value, sizeof(value));
  return Error::None;
}

Status MD5::ShutDown() {
  return Error::None;
}

Status MD5::Reset() {
  return StartUp();
}

Boolean MD5::IsNull() const {
  return false;
}

Status MD5::SetNull() {
  MemoryFillRaw(value, 0xFF, sizeof(value));
  return Error::None;
}

Boolean MD5::IsEmpty() const {
  
  UInt  i;

  for (i=0; i < sizeof(value); i++) {
    if (value[i] != 0xFF) break;
  }
  
  return i == sizeof(value);
  
}

Status MD5::SetEmpty() {
  StartUp();
  return Error::None;
}

Status MD5::MoveFrom(MD5 const &that) {
  if (this != (MD5 *)&that)
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(MD5));
  return Error::None;
}

Status MD5::CopyFrom(MD5 const &that) {
  if (this != (MD5 *)&that)
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(MD5));
  return Error::None;
}

} // ...Namespace BEFE