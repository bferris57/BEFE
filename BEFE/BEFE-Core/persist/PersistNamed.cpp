//----------------------------------------------------------------------
// File: PersistNamed.cpp - Implementation of Named Persist Values
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
// Writing Scalar Members
//

Status Persist::WriteRaw(char const *name, Byte *value, UInt valueLen) {
  Status status;
  status = WriteRaw(String(name),value,valueLen);
  return status;
}

Status Persist::WriteLength(char const *name, UInt32 length) {
  Status status;
  status = WriteLength(String(name),length);
  return status;
}

Status Persist::WriteString(char const *name, String const &value) {
  Status status;
  status = WriteString(String(name),value);
  return status;
}

Status Persist::WriteScalar(char const *name, Boolean value) {
  Status status;
  status = WriteScalar(String(name),value);
  return status;
}
  
Status Persist::WriteScalar(char const *name, UInt8 value) {
  Status status;
  status = WriteScalar(String(name),value);
  return status;
}

Status Persist::WriteScalar(char const *name, UInt16 value) {
  Status status;
  status = WriteScalar(String(name),value);
  return status;
}

Status Persist::WriteScalar(char const *name, UInt32 value) {
  Status status;
  status = WriteScalar(String(name),value);
  return status;
}

Status Persist::WriteScalar(char const *name, UInt64 value) {
  Status status;
  status = WriteScalar(String(name),value);
  return status;
}

Status Persist::WriteRaw(String const &name, Byte *value, UInt valueLen) {
  
  Status status;
  
  status = WriteAtomic((UInt8)ValueType::ScalarNamed);
  if (status) goto SOMEERROR;
  status = WriteAtomicString(name);
  if (status) goto SOMEERROR;
  status = WriteAtomic((UInt8)ValueType::ScalarRaw);
  if (status) goto SOMEERROR;
  status = WriteAtomicLength(valueLen);
  if (status) goto SOMEERROR;
  status = WriteAtomicRaw(value,valueLen);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

Status Persist::WriteLength(String const &name, UInt32 length) {
  BEFE_WARN("Persist.WriteLength(String const &, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteString(String const &name, String const &value) {
  BEFE_WARN("Persist.WriteString(String const &, String const &) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteScalar(String const &name, Boolean value) {
  
  Status status;

  status = WriteAtomic((UInt8)ValueType::ScalarNamed);
  if (status) goto SOMEERROR;
  status = WriteAtomicString(name);
  if (status) goto SOMEERROR;
  status = WriteAtomic((UInt8)ValueType::ScalarBoolean);
  if (status) goto SOMEERROR;
  status = WriteAtomic(value);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}
  
Status Persist::WriteScalar(String const &name, UInt8 value) {
  BEFE_WARN("Persist.WriteScalar(String const &, UInt8) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteScalar(String const &name, UInt16 value) {
  BEFE_WARN("Persist.WriteScalar(String const &, UInt16) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteScalar(String const &name, UInt32 value) {

  Status status;
  UInt   numWritten;

  union  {
    UInt32 uInt;
    Byte   bytes[4];
  } u;
  
  u.uInt = IntReverseBytes(value);
  status = stream.PutBytes(u.bytes,sizeof(UInt),numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != sizeof(UInt)) goto TRUNCATED;
  
  // Handle errors
  while (false) {
    SOMEERROR:                                 break;
    TRUNCATED: status = Error::ValueTruncated; break;
  }
  
  return status;

}

Status Persist::WriteScalar(String const &name, UInt64 value) {
  BEFE_WARN("Persist.WriteScalar(String const &, UInt64) not implemented yet");
  return Error::NotImplemented;
}
  
//----------------------------------------------------------------------
//
// Writing Multiple Member Values
//

Status Persist::WriteLengths(char const *name, UInt32 *lengths, UInt32 numValues) {
  BEFE_WARN("Persist.WriteLengths(char const *, UInt32 *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteStrings(char const *name, String const *strings, UInt32 numValues) {
  BEFE_WARN("Persist.WriteStrings(char const *, String const *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteScalars(char const *name, Boolean *values, UInt32 numValues) {
  BEFE_WARN("Persist.WriteScalars(char const *, Boolean *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteScalars(char const *name, UInt8 *values, UInt32 numValues) {
  BEFE_WARN("Persist.WriteScalars(char const *, UInt8 *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteScalars(char const *name, UInt16 *values, UInt32 numValues) {
  BEFE_WARN("Persist.WriteScalars(char const *, UInt16 *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteScalars(char const *name, UInt32 *values, UInt32 numValues) {

  Status status;
  
  status = WriteAtomic((UInt8)ValueType::SequenceNamed);
  if (status) goto SOMEERROR;
  status = WriteAtomicString(name);
  if (status) goto SOMEERROR;
  status = WriteAtomic((UInt8)ValueType::ScalarUInt32s);
  if (status) goto SOMEERROR;
  status = WriteAtomicLength(numValues);
  if (status) goto SOMEERROR;
  status = WriteAtomics(values, numValues);
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status Persist::WriteScalars(char const *name, UInt64 *values, UInt32 numValues) {
  BEFE_WARN("Persist.WriteScalars(char const *, UInt64 *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteLengths(String const &name, UInt32 *lengths, UInt32 numValues) {
  BEFE_WARN("Persist.WriteLengths(String const &, UInt32 *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteStrings(String const &name, String const *strings, UInt32 numValues) {
  BEFE_WARN("Persist.WriteStrings(String const &, String const *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteScalars(String const &name, Boolean *values, UInt32 numValues) {
  BEFE_WARN("Persist.WriteScalars(String const &, Boolean *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteScalars(String const &name, UInt8 *values, UInt32 numValues) {
  BEFE_WARN("Persist.WriteScalars(String const &, UInt8 *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteScalars(String const &name, UInt16 *values, UInt32 numValues) {
  BEFE_WARN("Persist.WriteScalars(String const &, UInt16 *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteScalars(String const &name, UInt32 *values, UInt32 numValues) {
  BEFE_WARN("Persist.WriteScalars(String const &, UInt32 *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::WriteScalars(String const &name, UInt64 *values, UInt32 numValues) {
  BEFE_WARN("Persist.WriteScalars(String const &, UInt64 *, UInt32) not implemented yet");
  return Error::NotImplemented;
}

} // ...Namespace BEFE