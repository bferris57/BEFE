//----------------------------------------------------------------------
// File: CryptKey.cpp - Implementation of the CryptKey Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

CryptKey::CryptKey() {
  StartUp();
}
  
CryptKey::CryptKey(CryptKey const &that) {
  StartUp();
  CopyFrom(that);
}

CryptKey::~CryptKey() {
  ShutDown();
}

CryptKey &CryptKey::operator = (CryptKey const &that) {
  CopyFrom(that);
  NotConsumable();
  return *this;
}
  
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status CryptKey::StartUp() {
  MemoryFillRaw((Byte *)this, 0, sizeof(CryptKey));
  return Error::None;
}
 
Status CryptKey::ShutDown() {
  return Error::None;
}
 
Status CryptKey::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}
 
Boolean CryptKey::IsNull() const {

  return (keyType != 0 || BEFE::IsNull(key_0));
  
}
 
Status CryptKey::SetNull() {
  return Reset();
}
 
Boolean CryptKey::IsEmpty() const {
  return IsNull();
}
 
Status CryptKey::SetEmpty() {
  return Reset();
}

Status CryptKey::MoveFrom(CryptKey const &that) {

  Status status;
  
  if (that.keyType != 0)
    status = Error::CryptKeyUnknownKeyType;
  else if (this != &that) {
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CryptKey));
    status = ((CryptKey *)&that)->StartUp();
  }
  else
    status = Error::None;
    
  return status;
}
 
Status CryptKey::CopyFrom(CryptKey const &that) {
  if (that.keyType != 0)
    return Error::CryptKeyUnknownKeyType;
  else if (this != &that)
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CryptKey));
  return Error::None;
}
 
CryptKey &CryptKey::Consumable() const {
  return *(CryptKey *)this;
}
 
CryptKey &CryptKey::NotConsumable() const {
  return *(CryptKey *)this;
}
 
Boolean CryptKey::IsConsumable() const {
  return false;
}
 
UInt CryptKey::Length() const {
  return 1;
}
 
UInt CryptKey::Size() const {
  return 0;
}
 
Status CryptKey::WriteTo(Stream &out) const {
  
  Status status;
  UInt   numWritten;
  UInt   zero;
  
  if (keyType != 0) goto BADKEYTYPE;
  // FOR NOW...
  zero = 0;
  status = out._PutBytes((Byte *)&zero, sizeof(UInt), numWritten);
  if (status) goto SOMEERROR;
  status = out._PutBytes((Byte *)&key_0, sizeof(UUID), numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != sizeof(UUID)) goto TRUNCATED;
  // ...FOR NOW
  
  // Handle Errors...
  status = Error::None;
  while (false) {
    BADKEYTYPE: status = Error::CryptKeyUnknownKeyType; break;
    TRUNCATED:  status = Error::ValueTruncated;         break;
    SOMEERROR:                                          break;
  }
  
  return status;

}

Status CryptKey::ReadFrom(Stream &in) {
  
  Status   status;
  UInt     numRead;
  CryptKey localKey;
  
  // FOR NOW...
  status = in._GetBytes((Byte *)&localKey.keyType, sizeof(localKey.keyType), numRead);
  if (status) goto SOMEERROR;
  if (numRead != sizeof(UInt)) goto TRUNCATED;
  status = in._GetBytes((Byte *)&localKey.key_0, sizeof(localKey.key_0), numRead);
  if (status) goto SOMEERROR;
  if (numRead != sizeof(UUID)) goto TRUNCATED;
  // ...FOR NOW
  
  if (localKey.keyType != 0) goto BADKEYTYPE;
  MemoryCopyRaw((Byte *)&localKey, (Byte *)this, sizeof(CryptKey));
  
  // Handle Errors...
  status = Error::None;
  while (false) {
    BADKEYTYPE: status = Error::CryptKeyUnknownKeyType; break;
    TRUNCATED:  status = Error::ValueTruncated;         break;
    SOMEERROR:                                          break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Operational Methods
//

Boolean CryptKey::IsValid() const {

  Boolean answer;
  
  if (keyType != 0)
    answer = false;
  else
    answer = ((UUID *)&key_0)->IsValid();

  return answer;

}

String CryptKey::ToString() const {
  
  String str;
  
  str = "KeyType_0:";
  str += ((UUID *)&key_0)->ToString();
  
  return str.Consumable();
  
}

Status CryptKey::NewRandom() {

  Status   status;
  CryptKey localKey;
  
  if (!TheBefe) goto NOBEFE;
  
  // FOR NOW...
  localKey.keyType = 0;
  status = ((UUID *)&localKey.key_0)->NewRandom();
  if (status) goto SOMEERROR;
  // ...FOR NOW
  
  status = CopyFrom(localKey);
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    NOBEFE:    status = Error::NoBefe; break;
    SOMEERROR:                         break;
  }
  
  return status;
  
}

} // ...Namespace BEFE