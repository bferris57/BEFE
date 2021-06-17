//----------------------------------------------------------------------
// File: PersistAnon.cpp - Implementation of Anonymous Persist Values
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Persist.h"
#include "Integer.h" // For IntReverseBytes, etc.

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Writing Single Anonymous Scalar Values
//

Status Persist::WriteLength(UInt32 length) {

  Status status;
  Byte   type;
  
  type = ValueType::ScalarLength;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomicLength(length);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteRaw(Byte *value, UInt valueLen) {

  Status status;
  Byte   type;
  
  type   = ValueType::ScalarRaw;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomicRaw(value, valueLen);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteString(String const &value) {

  Status status;
  Byte   type;
  
  type = ValueType::ScalarString;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomicString(value);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteScalar(Boolean value) {

  Status status;
  Byte   type;
  
  type = ValueType::ScalarBoolean;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomic(value);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteScalar(UInt8 value) {

  Status status;
  Byte   type;
  
  type = ValueType::ScalarUInt8;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomic(value);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteScalar(UInt16 value) {

  Status status;
  Byte   type;
  
  type = ValueType::ScalarUInt16;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomic(value);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteScalar(UInt32 value) {

  Status status;
  Byte   type;
  
  type = ValueType::ScalarUInt32;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomic(value);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteScalar(UInt64 value) {

  Status status;
  Byte   type;
  
  type = ValueType::ScalarUInt64;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomic(value);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Writing Multiple Anonymous Scalar Values
//

Status Persist::WriteLengths(UInt32 *value, UInt32 numValues) {
  
  Status status;
  Byte   type;
  
  type = ValueType::ScalarLengths;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomicLengths(value, numValues);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteStrings(String const *strings, UInt32 numValues) {
  
  Status status;
  Byte   type;
  
  type = ValueType::ScalarStrings;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomicStrings(strings, numValues);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteScalars(UInt8 *values, UInt numValues) {
  
  Status status;
  Byte   type;
  
  type = ValueType::ScalarUInt8s;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomicRaw(values, numValues);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteScalars(UInt16 *values, UInt numValues) {
  
  Status status;
  Byte   type;
  
  type = ValueType::ScalarUInt16s;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomics(values, numValues);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteScalars(UInt32 *values, UInt numValues) {
  
  Status status;
  Byte   type;
  
  type = ValueType::ScalarUInt32s;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomics(values, numValues);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteScalars(UInt64 *values, UInt numValues) {
  
  Status status;
  Byte   type;
  
  type = ValueType::ScalarUInt64s;
  status = WriteAtomic(type);
  if (status) goto SOMEERROR;
  status = WriteAtomics(values, numValues);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

} // ...Namespace BEFE