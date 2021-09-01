//----------------------------------------------------------------------
// File: LexWsString.cpp - Implementation of the BEFE LexWsString Struct
//----------------------------------------------------------------------
//
// This file contains the C++ implementation of the BEFE LexWsString Struct
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "LexWs.h"
#include "PersistHead.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class (static) methods
//

UInt LexWsString::PerBlock() {
  UInt32 blockSize;
  blockSize = TheBefe->TheCache.GetBlockSize();
  if (blockSize == UNaN)
    return UNaN;
  else
    return (blockSize-LexWsBlockHead::PersistSize(LexWs::BlockTypeString))/PersistSize();
}

UInt LexWsString::PersistSize() {
  return 3*sizeof(UInt32);
}

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LexWsString::LexWsString() {
  StartUp();
}
  
LexWsString::LexWsString(LexWsString const &that) {
  StartUp();
  CopyFrom(that);
}

LexWsString::~LexWsString() {
  ShutDown();
}

LexWsString &LexWsString::operator=(LexWsString const &that) {
  StartUp();
  CopyFrom(that);
  return *this;
}
	
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status LexWsString::StartUp() {

  BEFE::SetNull(id);
  BEFE::SetNull(blobSpan.idx1);
  BEFE::SetNull(blobSpan.idx2);
  
  return Error::None;
  
}
  
Status LexWsString::ShutDown() {
  return Error::None;
}

Status LexWsString::Clear() {
  return Reset();
}

Status LexWsString::Reset() {
  Status status;
  status = StartUp();
  return status;
}

Boolean LexWsString::IsNull() const {
  return BEFE::IsNull(id);
}

Status LexWsString::SetNull() {
  return StartUp();
}

Boolean LexWsString::IsEmpty() {
  return  BEFE::IsNull(blobSpan.idx1) || BEFE::IsNull(blobSpan.idx2) || blobSpan.Length() == 0;
}

Status LexWsString::SetEmpty() {
  return Error::NotImplemented;
}

Status LexWsString::MoveFrom(LexWsString const &that) {
  
  Status status;
  
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexWsString));
  status = ((LexWsString *)&that)->StartUp();

  return status;
  
}

Status LexWsString::CopyFrom(LexWsString const &that) {
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexWsString));
  return Error::None;
}

Boolean LexWsString::IsConsumable() const {
  return false;
}

LexWsString &LexWsString::Consumable() const {
  return *(LexWsString *)this;
}

LexWsString &LexWsString::NotConsumable() const {
  return *(LexWsString *)this;
}

//----------------------------------------------------------------------
//
// Persistence
//

Status LexWsString::Write(Id fileId, UInt blockNo, UInt entryNo) { // LexWsString.Write...
  
  Status          status;
  LexWsBlockHead  blockHead;
  Byte           *block;
  UInt            offset;
  UInt            anInt;
  
  // Sanity checks...
  status = blockHead.Read(fileId, blockNo);
  if (status) goto SOMEERROR;
  if (blockHead.type != LexWs::BlockTypeString) goto WRONGTYPE;
  if (blockHead.level != 1) goto WRONGLEVEL;
  if (BEFE::IsNull(entryNo) || entryNo > blockHead.numEntries) goto BADIDX;
  if (entryNo >= PerBlock()) goto BADIDX;

  // Calculate block and starting offset
  offset  = blockHead.PersistSize();
  offset += entryNo * PersistSize();
  block = TheBefe->TheCache._GetBlockPointer(fileId, blockNo);
  if (BEFE::IsNull(block)) goto SOMEERROR;
  block += offset;
  
  // Update it...
  anInt = id;
  *block++ = (Byte)(anInt>>24);
  *block++ = (Byte)(anInt>>16);
  *block++ = (Byte)(anInt>>8);
  *block++ = (Byte)(anInt);
  anInt = blobSpan.idx1;
  *block++ = (Byte)(anInt>>24);
  *block++ = (Byte)(anInt>>16);
  *block++ = (Byte)(anInt>>8);
  *block++ = (Byte)(anInt);
  anInt = blobSpan.idx2;
  *block++ = (Byte)(anInt>>24);
  *block++ = (Byte)(anInt>>16);
  *block++ = (Byte)(anInt>>8);
  *block++ = (Byte)(anInt);
  
  // And update the header if writing last one...
  if (entryNo == blockHead.numEntries) {
    blockHead.numEntries++;
    status = blockHead.Write(fileId, blockNo);
    if (status) goto SOMEERROR;
  }
  
  // This block is dirty now...
  status = TheBefe->TheCache._SetBlockIsDirty(fileId, blockNo);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    WRONGTYPE:  status = Error::LexWsBlockWrongType;  break;
    WRONGLEVEL: status = Error::LexWsBlockWrongLevel; break;
    BADIDX:     status = Error::LexWsBadEntryIdx;     break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
} // ...LexWsString.Write

Status LexWsString::Read(Id fileId, UInt blockNo, UInt entryNo) { // LexWsString.Read...

  Status          status;
  LexWsBlockHead  blockHead;
  Byte           *block;
  UInt            offset;
  UInt            anInt;
  
  // Sanity checks...
  status = blockHead.Read(fileId, blockNo);
  if (status) goto SOMEERROR;
  if (blockHead.type != LexWs::BlockTypeString) goto WRONGTYPE;
  if (blockHead.level != 1) goto WRONGLEVEL;
  if (BEFE::IsNull(entryNo) || entryNo >= blockHead.numEntries) goto BADIDX;
  if (entryNo >= PerBlock()) goto BADIDX;

  // Calculate block and starting offset
  offset  = blockHead.PersistSize();
  offset += entryNo * PersistSize();
  block = TheBefe->TheCache._GetBlockPointer(fileId, blockNo);
  if (BEFE::IsNull(block)) goto SOMEERROR;
  block += offset;
  
  // Read it...
  anInt  = ((Byte)*block++)<<24;
  anInt += ((Byte)*block++)<<16; 
  anInt += ((Byte)*block++)<<8; 
  anInt += ((Byte)*block++); 
  id = anInt;
  
  anInt  = ((Byte)*block++)<<24;
  anInt += ((Byte)*block++)<<16; 
  anInt += ((Byte)*block++)<<8; 
  anInt += ((Byte)*block++); 
  blobSpan.idx1 = anInt;

  anInt  = ((Byte)*block++)<<24;
  anInt += ((Byte)*block++)<<16; 
  anInt += ((Byte)*block++)<<8; 
  anInt += ((Byte)*block++); 
  blobSpan.idx2 = anInt;

  // Handle errors
  while (false) {
    WRONGTYPE:  status = Error::LexWsBlockWrongType;  break;
    WRONGLEVEL: status = Error::LexWsBlockWrongLevel; break;
    BADIDX:     status = Error::LexWsBadEntryIdx;     break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
} // ...LexWsString.Read

} // ...Namespace BEFE