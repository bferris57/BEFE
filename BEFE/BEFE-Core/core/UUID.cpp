//----------------------------------------------------------------------
// File: UUID.cpp - Implementation of the UUID Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

UUID::UUID() {
  StartUp();
}
  
UUID::UUID(UUID const &that) {
  StartUp();
  CopyFrom(that);
}

UUID::~UUID() {
  ShutDown();
}

UUID &UUID::operator = (UUID const &that) {
  CopyFrom(that);
  NotConsumable();
  return *this;
}
  
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status UUID::StartUp() {
  MemoryFillRaw((Byte *)this, 0, sizeof(UUID));
  return Error::None;
}
 
Status UUID::ShutDown() {
  return Error::None;
}
 
Status UUID::Reset() {
  return StartUp();
}
 
Boolean UUID::IsNull() const {

  Byte *bp;
  Int   i;

  bp = (Byte *)this;
  for (i=sizeof(UUID); i-- > 0 && *bp++ == 0;);

  return i <= 0;

}
 
Status UUID::SetNull() {
  return Reset();
}
 
Boolean UUID::IsEmpty() const {
  return IsNull();
}
 
Status UUID::SetEmpty() {
  return Reset();
}
 
Status UUID::MoveFrom(UUID const &that) {

  Status status;
  
  if (this != &that) {
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(UUID));
    status = ((UUID *)&that)->StartUp();
  }
  else
    status = Error::None;
    
  return status;
}
 
Status UUID::CopyFrom(UUID const &that) {
  if (this != &that)
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(UUID));
  return Error::None;
}
 
UUID &UUID::Consumable() const {
  return *((UUID *)this);
}
 
UUID &UUID::NotConsumable() const {
  return *((UUID *)this);
}
 
Boolean UUID::IsConsumable() const {
  return false;
}
 
UInt UUID::Length() const {
  return 1;
}
 
UInt UUID::Size() const {
  return 0;
}
 
Status UUID::WriteTo(Stream &out) {
  
  Status status;
  UInt   numWritten;
  
  status = out._PutBytes((Byte *)this, sizeof(UUID), numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != sizeof(UUID)) goto TRUNCATED;
  
  // Handle Errors...
  while (false) {
    TRUNCATED: status = Error::ValueTruncated; break;
    SOMEERROR:                                 break;
  }
  
  return status;

}
 
Status UUID::ReadFrom(Stream &in) {
  
  Status status;
  UInt   numRead;
  
  status = in._GetBytes((Byte *)this, sizeof(UUID), numRead);
  if (status) goto SOMEERROR;
  if (numRead != sizeof(UUID)) goto TRUNCATED;
  
  // Handle Errors...
  while (false) {
    TRUNCATED: status = Error::ValueTruncated; break;
    SOMEERROR:                                 break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Operational Methods
//

Boolean UUID::IsValid() const {
  Byte    varient;
  Byte    version;
  Boolean answer;
  
  varient = value.data[6] >> 4;;
  version = value.data[8] >> 4;
  if (varient < 1 || varient > 5)
    answer = false;
  else if (varient == 4 && version >= 0x08 && version <= 0x0B)
    answer = true;
  else
    answer = false;
  return answer;
}

String UUID::ToString() const {
  
  String str;
  
  str = '{';
  str += ToHexString((Byte *)&value.data[0], 4);
  str += '-';
  str += ToHexString((Byte *)&value.data[4], 2);
  str += '-';
  str += ToHexString((Byte *)&value.data[6], 2);
  str += '-';
  str += ToHexString((Byte *)&value.data[8], 2);
  str += '-';
  str += ToHexString((Byte *)&value.data[10], 6);
  str += '}';
  
  return str.Consumable();
  
}

Status UUID::NewRandom() {

  Status status;
  UUID   localUUID;
  
  if (!TheBefe) goto NOBEFE;
  if (!TheBefe->TheOS) goto NOOS;
  status = TheBefe->TheOS->NewRandomUUID(localUUID);
  if (status) goto SOMEERROR;
  status = CopyFrom(localUUID);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOBEFE:    status = Error::NoBefe;              break;
    NOOS:      status = Error::InternalNullPointer; break;
    SOMEERROR:                                      break;
  }
  
  return status;
  
}

} // ...Namespace BEFE