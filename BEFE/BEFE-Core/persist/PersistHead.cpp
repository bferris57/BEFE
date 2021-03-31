//----------------------------------------------------------------------
// File: PersistHead.cpp - PersistHead Class implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h" // For global macros
#include "PersistHead.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

PersistHead::PersistHead() {
  StartUp();
}

PersistHead::PersistHead(PersistHead const &that) {
  CopyFrom(that);
}

PersistHead::~PersistHead() {
  ShutDown();
}

PersistHead &PersistHead::operator = (PersistHead const &that) {
  CopyFrom(that);
  NotConsumable();
  return *this;
}
  
//----------------------------------------------------------------------
//
// BEFE Lifecyce...
//

Status PersistHead::StartUp() {

  UInt mainVersion;
  UInt subVersion;
  UInt mainRevision;
  
  befeSignature[0] = 0xBE;
  befeSignature[1] = 0xFE;
  if (!TheBefe || !TheBefe->ThePlatform) {
    befeVersionHi[0] = 0;
    befeVersionLo[1] = 0;
  }
  else {
    mainVersion  = TheBefe->ThePlatform->GetMainVersion();
    subVersion   = TheBefe->ThePlatform->GetSubVersion();
    mainRevision = TheBefe->ThePlatform->GetMainRevision();
    befeVersionHi[0] = (mainVersion>>8  ) & 0xFF;
    befeVersionHi[1] = (mainVersion     ) & 0xFF;
    befeVersionLo[0] = (subVersion>>8   ) & 0xFF;
    befeVersionLo[1] = (subVersion      ) & 0xFF;
    befeRevision [0] = (mainRevision>>24)       ;
    befeRevision [1] = (mainRevision>>16) & 0xFF;
    befeRevision [2] = (mainRevision>>8 ) & 0xFF;
    befeRevision [3] = (mainRevision    ) & 0xFF;
  }
  typeName[0] = typeName[1] = typeName[2] = typeName[3] = '?';
  typeRevision[0] = typeRevision[1] = 0;

  return Error::None;
  
}

Status PersistHead::ShutDown() {

  return Error::None;
  
}

Status PersistHead::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Boolean PersistHead::IsNull() const{
  return typeName[0] == '?';
}

Status PersistHead::SetNull() {
  return Reset();
}

Boolean PersistHead::IsEmpty() const {
  return false;
}

Status PersistHead::SetEmpty() {
  return Reset();
}

Status PersistHead::MoveFrom(PersistHead const &that) {
  
  Status status;
  
  if (this != &that) {
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(PersistHead));
    status = ((PersistHead *)&that)->StartUp();
  }
  else
    status = Error::None;
    
  return status;

}

Status PersistHead::CopyFrom(PersistHead const &that) {

  Status status;
  
  if (this != &that) {
    if (IsConsumable()) 
      status = MoveFrom(that);
    else {
      status = Reset();
      if (!status)
        MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(PersistHead));
    }
  }
  else
    status = Error::None;
    
  return status;

}

PersistHead &PersistHead::Consumable() const {
  return *(PersistHead *)this;
}

PersistHead &PersistHead::NotConsumable() const {
  return *(PersistHead *)this;
}

Boolean PersistHead::IsConsumable() const {
  return false;
}

Boolean PersistHead::IsValid() const {
  return befeSignature[0] == 0xBE &&
         befeSignature[1] == 0xFE &&
         typeName[0]      != '?'  &&
         typeName[1]      != '?'  &&
         typeName[2]      != '?'  &&
         typeName[3]      != '?';
}

UInt PersistHead::Length() const {
  return 1;
}

UInt PersistHead::Size() const {
  return sizeof(PersistHead);
}

UInt PersistHead::PersistSize() const {
  return sizeof(befeSignature) +
         sizeof(befeVersionHi) +
         sizeof(befeVersionLo) +
         sizeof(befeRevision)  +
         sizeof(typeName)      +
         sizeof(typeRevision);  
}

Status PersistHead::WriteTo(Stream &out) {

  Status status;
  UInt   numWritten;
  
  status = out._PutBytes((Byte *)this, sizeof(PersistHead), numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != sizeof(PersistHead)) goto TRUNCATED;
  
  // Handle errors
  while (false) {
    TRUNCATED: status = Error::ValueTruncated; break;
    SOMEERROR:                                 break;
  }
  
  return status;
  
}

Status PersistHead::ReadFrom(Stream &in) {

  Status      status;
  PersistHead localHead;
  UInt        numRead;

  status = in._GetBytes((Byte *)&localHead, sizeof(PersistHead), numRead);
  if (status) goto SOMEERROR;
  if (numRead != sizeof(PersistHead)) goto TRUNCATED;
  if (localHead.befeSignature[0] != 0xBEu || localHead.befeSignature[1] != 0xFEu) goto NOTBEFE;
  status = CopyFrom(localHead);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    TRUNCATED: status = Error::ValueTruncated; break;
    NOTBEFE:   status = Error::PersistNotBEFE; break;
    SOMEERROR:                                 break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Operational Methods
//

String PersistHead::ToString() const {

  String str;
  
  if (GetSignature() == 0xBEFE)
    str = "BEFE:";
  else
    str = "????:";
  str += GetMainVersion();
  str += '.';
  str += GetSubVersion();
  str += '.';
  str += GetRevision();
  str += '/';
  str += GetTypeName();
  if (GetTypeRevision()) {
    str += '(';
    str += GetTypeRevision();
    str += ')';
  }
  
  return str.Consumable();
  
}

UInt16 PersistHead::GetSignature() const {
  UInt16 u;
  u = (befeSignature[0] << 8) | befeSignature[1];
  return u;
}

UInt16 PersistHead::GetMainVersion() const {
  UInt16 u;
  u = (befeVersionHi[0] << 8) | befeVersionHi[1];
  return u;
}

UInt16 PersistHead::GetSubVersion() const {
  UInt16 u;
  u = (befeVersionLo[0] << 8) | befeVersionLo[1];
  return u;
}

UInt32 PersistHead::GetRevision() const {
  UInt32 u;
  u = (befeRevision[0]<<24) | (befeRevision[1]<<16) | (befeRevision[2]<<8) | befeRevision[3];
  return u;
}

String PersistHead::GetTypeName() const {
  return String((Byte *)&typeName[0],4).RightTrim();
}

UInt16 PersistHead::GetTypeRevision() const {
  UInt32 u;
  u = (typeRevision[0] << 8) | typeRevision[1];
  return u;
}

Status PersistHead::SetTypeName(String const &newName) {
  
  Status  status;
  Byte   *buf;
  UInt    byteCount;
  UInt    charCount;
  
  status = ((String *)&newName)->_BufByteAndCharCount(buf, byteCount, charCount);
  if (status) goto SOMEERROR;
  if (byteCount > 4 || BEFE::IsNull(buf)) goto MALFORMED;
  typeName[0] = typeName[1] = typeName[2] = typeName[3] = ' ';
  MemoryCopyRaw(buf, typeName, byteCount);
  
  // Handle errors
  status = Error::None;
  while (false) {
    MALFORMED: status = Error::Failed; break;
    SOMEERROR:                         break;
  }
  
  return status;
}

Status PersistHead::SetTypeRevision(UInt16 newRev) {
  
  typeRevision[0] = newRev >> 8;
  typeRevision[1] = newRev;
  
  return Error::None;
  
}

} // ...Namespace BEFE