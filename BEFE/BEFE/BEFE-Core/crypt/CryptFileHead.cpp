//----------------------------------------------------------------------
// File: CryptFileHead.cpp - Implementation of the CryptFileHead Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "CryptFileHead.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

CryptFileHead::CryptFileHead() {
  StartUp();
}
  
CryptFileHead::CryptFileHead(CryptFileHead const &that) {
  StartUp();
  CopyFrom(that);
}

CryptFileHead::~CryptFileHead() {
  ShutDown();
}

CryptFileHead &CryptFileHead::operator = (CryptFileHead const &that) {
  CopyFrom(that);
  NotConsumable();
  return *this;
}
  
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status CryptFileHead::StartUp() {
  
  Status retStatus;
  Status status;
  
  MemoryFillRaw((Byte *)this, 0xFF, sizeof(CryptFileHead));
  retStatus = inFileName.StartUp();
  status = pHead.StartUp();
  if (status && !retStatus) retStatus = status;
  keyType = 0;
  BEFE::SetNull(inCrc);
  BEFE::SetNull(outCrc);
  BEFE::SetNull(startTime);
  BEFE::SetNull(endTime);
  
  return retStatus;
  
}
 
Status CryptFileHead::ShutDown() {
  
  Status retStatus;
  Status status;
  
  retStatus = inFileName.ShutDown();
  status = pHead.ShutDown();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}
 
Status CryptFileHead::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}
 
Boolean CryptFileHead::IsNull() const {

  return (BEFE::IsNull(pHead));
  
}
 
Status CryptFileHead::SetNull() {
  return Reset();
}
 
Boolean CryptFileHead::IsEmpty() const {
  return IsNull();
}
 
Status CryptFileHead::SetEmpty() {
  return Reset();
}

Status CryptFileHead::MoveFrom(CryptFileHead const &that) {

  Status status;
  
  if (this != &that) {
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CryptFileHead));
    status = ((CryptFileHead *)&that)->StartUp();
  }
  else
    status = Error::None;
    
  return status;
}
 
Status CryptFileHead::CopyFrom(CryptFileHead const &that) {
  Status status;
  if (this != &that) {
    if (IsConsumable())
      status = MoveFrom(that);
    else {
      MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(CryptFileHead));
      status = inFileName.StartUp();
      if (!status)
        inFileName = that.inFileName;
    }
  }
  else
    status = Error::None;
  return status;
}
 
CryptFileHead &CryptFileHead::Consumable() const {
  inFileName.Consumable();
  return *(CryptFileHead *)this;
}
 
CryptFileHead &CryptFileHead::NotConsumable() const {
  inFileName.NotConsumable();
  return *(CryptFileHead *)this;
}
 
Boolean CryptFileHead::IsConsumable() const {
  return inFileName.IsConsumable();
}
 
UInt CryptFileHead::Length() const {
  return 1;
}
 
UInt CryptFileHead::Size() const {
  return 0;
}

UInt CryptFileHead::PersistSize() const {
  return pHead.PersistSize() +
         sizeof(keyType)     +
         sizeof(inCrc)       +
         sizeof(outCrc)      +
         sizeof(startTime)   +
         sizeof(endTime)     +
         sizeof(UInt)        +
         inFileName.Size();          // ◄── For inFileSize
         
}
 
Status CryptFileHead::WriteTo(Stream &out) const {
  
  Status         status;
  UInt           numWritten;
  CryptFileHead  localHead;
  Byte          *inFileNameBuf;
  UInt           inFileNameSize;

  inFileName._BufAndSize(inFileNameBuf, inFileNameSize);
  localHead.keyType        = IntReverseBytes(keyType);
  localHead.inCrc          = IntReverseBytes(inCrc);
  localHead.outCrc         = IntReverseBytes(outCrc);
  localHead.startTime      = LongReverseBytes(startTime);
  localHead.endTime        = LongReverseBytes(endTime);
  status = ((PersistHead *)&pHead)->WriteTo(out);
  if (status) goto SOMEERROR;
  status = out._PutBytes((Byte *)&localHead.keyType, sizeof(localHead.keyType), numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != sizeof(localHead.keyType)) goto TRUNCATED;
  status = out._PutBytes((Byte *)&localHead.inCrc, sizeof(localHead.inCrc), numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != sizeof(localHead.inCrc)) goto TRUNCATED;
  status = out._PutBytes((Byte *)&localHead.outCrc, sizeof(localHead.outCrc), numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != sizeof(localHead.outCrc)) goto TRUNCATED;
  status = out._PutBytes((Byte *)&localHead.startTime, sizeof(localHead.startTime), numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != sizeof(localHead.startTime)) goto TRUNCATED;
  status = out._PutBytes((Byte *)&localHead.endTime, sizeof(localHead.endTime), numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != sizeof(localHead.endTime)) goto TRUNCATED;

  inFileNameSize = IntReverseBytes(inFileNameSize);
  status = out._PutBytes((Byte *)&inFileNameSize, sizeof(inFileNameSize), numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != sizeof(inFileNameSize)) goto TRUNCATED;
  inFileNameSize = IntReverseBytes(inFileNameSize);

  if (inFileNameSize && !BEFE::IsNull(inFileNameBuf)) {
    status = out._PutBytes(inFileNameBuf, inFileNameSize, numWritten);
    if (status) goto SOMEERROR;
    if (numWritten != inFileNameSize) goto TRUNCATED;
  }
  
  // Handle Errors...
  status = Error::None;
  while (false) {
    TRUNCATED:  status = Error::FileWriteFailed; break;
    SOMEERROR:                                   break;
  }
  
  return status;

}

Status CryptFileHead::ReadFrom(Stream &in) {
  
  Status         status;
  UInt           numRead;
  CryptFileHead  localHead;
  UInt           inFileNameSize;

  status = in.SetPos(0);
  if (status) goto SOMEERROR;
  
  status = ((PersistHead *)&localHead.pHead)->ReadFrom(in);
  if (status) goto SOMEERROR;
  
  if (!localHead.pHead.IsValid()) goto NOTENCFILE;
  if (localHead.pHead.GetTypeName().Get(Span(0,3)) != "Enc") goto NOTELIGIBLE;
  
  status = in._GetBytes((Byte *)&localHead.keyType, sizeof(localHead.keyType), numRead);
  if (status) goto SOMEERROR;
  if (numRead != sizeof(localHead.keyType)) goto TRUNCATED;
  status = in._GetBytes((Byte *)&localHead.inCrc, sizeof(localHead.inCrc), numRead);
  if (status) goto SOMEERROR;
  if (numRead != sizeof(localHead.inCrc)) goto TRUNCATED;
  status = in._GetBytes((Byte *)&localHead.outCrc, sizeof(localHead.outCrc), numRead);
  if (status) goto SOMEERROR;
  if (numRead != sizeof(localHead.outCrc)) goto TRUNCATED;
  status = in._GetBytes((Byte *)&localHead.startTime, sizeof(localHead.startTime), numRead);
  if (status) goto SOMEERROR;
  if (numRead != sizeof(localHead.startTime)) goto TRUNCATED;
  status = in._GetBytes((Byte *)&localHead.endTime, sizeof(localHead.endTime), numRead);
  if (status) goto SOMEERROR;
  if (numRead != sizeof(localHead.endTime)) goto TRUNCATED;

  // Read file Name Length and position past the name
  status = in._GetBytes((Byte *)&inFileNameSize, sizeof(inFileNameSize), numRead);
  if (status) goto SOMEERROR;
  inFileNameSize = IntReverseBytes(inFileNameSize);
  if (numRead != sizeof(inFileNameSize)) goto TRUNCATED;
  if (BEFE::IsNull(inFileNameSize)) goto CORRUPT;
  // HACK (Until later)...
  //status = in.SetPos(in.GetPos()+inFileNameSize);
  //if (status) goto SOMEERROR;
  {
    Byte buf[1024];
    status = in._GetBytes((Byte *)&buf[0], inFileNameSize, numRead);
    if (status) goto SOMEERROR;
    if (numRead != inFileNameSize) goto TRUNCATED;
    localHead.inFileName = String(buf, inFileNameSize);
  }
  // ...HACK (Until later)
  
  // Do some more validation...  
  localHead.keyType   = IntReverseBytes(localHead.keyType);
  localHead.inCrc     = IntReverseBytes(localHead.inCrc);
  localHead.outCrc    = IntReverseBytes(localHead.outCrc);
  localHead.startTime = LongReverseBytes(localHead.startTime);
  localHead.endTime   = LongReverseBytes(localHead.endTime);
  if (localHead.keyType != 0) goto NOTELIGIBLE;
  
  // Everything's okay...
  *this = localHead.Consumable();
  
  // Handle Errors...
  status = Error::None;
  while (false) {
    NOTENCFILE:  status = Error::CryptFileNotEncrypted; break;
    NOTELIGIBLE: status = Error::CryptFileNotEligible;  break;
    CORRUPT:     status = Error::CryptFileCorrupt;      break;
    TRUNCATED:   status = Error::FileReadFailed;        break;
    SOMEERROR:                                          break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Operational Methods
//

Boolean CryptFileHead::IsValid() const {

  Boolean answer;
  String  typeName;
  
  typeName = pHead.GetTypeName();
  if (BEFE::IsEmpty(typeName))
    answer = false;
  else
    answer = typeName == "Enc0";

  return answer;

}

Strings CryptFileHead::ToStrings() const {
  
  Strings strings;
  
  strings.Append("***CryptFileHead.ToStrings Not Implemented Yet***");
  
  return strings.Consumable();
  
}

} // ...Namespace BEFE