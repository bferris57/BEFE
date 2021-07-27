//----------------------------------------------------------------------
// File: PersistAtomic.cpp - Implementation of Atomic Persist Values
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Persist.h"
#include "Integer.h" // For IntReverseBytes, etc.

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Writing Single Atomic Scalar Values
//

Status Persist::WriteAtomicRaw(Byte *value, UInt valueLen) {
  
  Status status;
  UInt   numWritten;
  
  if ( !BEFE::IsNull(valueLen) && valueLen != 0) {
    status = stream.PutBytes(value,valueLen,numWritten);
    if (status) goto SOMEERROR;
    if (numWritten != valueLen) goto TRUNCATED;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                                 break;
    TRUNCATED: status = Error::ValueTruncated; break;
  }
  
  return status;
  
}

Status Persist::WriteAtomicLength(UInt32 length) {

  Status status;
  Byte   buf[5];
  UInt   len;
  UInt   numWritten;
  
  if (length <= 0x7f) { // Length8...
    buf[0] = length;
    len = 1;
  }
  else if (length <= 0x3fff) { // Length16...
    buf[0] = (length>>8) | 0x80;
    buf[1] = (length)    & 0xFF;
    len = 2;
  }
  else if (length <= 0x1fffff) { // Length24...
    buf[0] = (length>>16) | 0xC0;
    buf[1] = (length>>8)  & 0xFF;
    buf[2] = (length)     & 0xFF;
    len = 3;
  }
  else if (length <= 0x0fffffff) { // Length32...
    buf[0] = (length>>24) | 0xE0;
    buf[1] = (length>>16) & 0xFF;
    buf[2] = (length>>8)  & 0xFF;
    buf[3] = (length)     & 0xFF;
    len = 4;
  }
  else { // Length40...
    buf[0] = 0xF0;
    buf[1] = (length>>24) & 0xFF;
    buf[2] = (length>>16) & 0xFF;
    buf[3] = (length>>8)  & 0xFF;
    buf[4] = (length)     & 0xFF;
    len = 5;
  }
  
  // Write it
  status = stream.PutBytes(buf, len, numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != len) goto TRUNCATED;
  
  // Handle errors
  while (false) {
    SOMEERROR:                                 break;
    TRUNCATED: status = Error::ValueTruncated; break;
  }
  
  return status;
  
}

Status Persist::WriteAtomicString(String const &value) {
  return ((String *)&value)->_PersistWriteAtomic(*this);
}

Status Persist::WriteAtomic(Boolean value) {

  Status status;
  
  if (value)
    status = stream._PutByte(0x01);
  else
    status = stream._PutByte(0x00);
  
  return status;

}

Status Persist::WriteAtomic(UInt8 value) {
  
  Status status;
  
  status = stream._PutByte(value);
  
  return status;

}

Status Persist::WriteAtomic(UInt16 value) {
  BEFE_WARN("Persist.WriteAtomic(UInt16) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteAtomic(UInt32 value) {
  BEFE_WARN("Persist.WriteAtomic(UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteAtomic(UInt64 value) {
  BEFE_WARN("Persist.WriteAtomic(UInt64) not implemented yet");
  return Error::NotImplemented;
}

//----------------------------------------------------------------------
//
// Writing Multiple Anonymous Scalar Values
//

Status Persist::WriteAtomicLengths(UInt32 *lengths, UInt32 numValues) {
  BEFE_WARN("Persist.WriteAtomicLengths(UInt32 *, UInt32) not implemented yet");
  return Error::NotImplemented;  
}

Status Persist::WriteAtomicStrings(String const *strings, UInt32 numValues) {
  BEFE_WARN("Persist.WriteAtomicLengths(String const *, UInt32) not implemented yet");
  return Error::NotImplemented;  
}

Status Persist::WriteAtomics(UInt8 *value, UInt32 numValues) {
  
  Status  status;
  UInt    numWritten;

  // Write the bytes
  status = stream.PutBytes((Byte *)value, numValues, numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != numValues) goto TRUNCATED;
  
  // Handle errors
  while (false) {
    SOMEERROR:                                 break;
    TRUNCATED: status = Error::ValueTruncated; break;
  }
  
  return status;    
  
}

Status Persist::WriteAtomics(UInt16 *value, UInt32 numValues) {
  BEFE_WARN("Persist.WriteAtomics(UInt16 *, UInt32) not implemented yet");
  return Error::NotImplemented;  
}

Status Persist::WriteAtomics(UInt32 *value, UInt32 numValues) {
  
  Status  status;
  UInt   *curPointer;
  UInt    rem;
  UInt    numWritten;

  // Reverse byte order
  curPointer = value;
  for (rem=numValues; rem; rem--) {
    *curPointer = IntReverseBytes(*curPointer);
    curPointer++;
  }
  
  // Write the bytes
  status = stream.PutBytes((Byte *)value, numValues*sizeof(UInt32),numWritten);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  // Reverse byte order back to what it was
  curPointer = value;
  for (rem=numValues; rem; rem--) {
    *curPointer = IntReverseBytes(*curPointer);
    curPointer++;
  }
  
  return status;    
  
}

Status Persist::WriteAtomics(UInt64 *value, UInt32 numValues) {
  BEFE_WARN("Persist.WriteAtomics(UInt64 *, UInt32) not implemented yet");
  return Error::NotImplemented;  
}

} // ...Namespace BEFE