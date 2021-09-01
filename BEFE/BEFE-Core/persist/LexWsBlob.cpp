//----------------------------------------------------------------------
// File: LexWsBlob.cpp - Implementation of the BEFE LexWsBlob Struct
//----------------------------------------------------------------------
//
// This file contains the C++ implementation of the BEFE LexWsBlob Struct
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

UInt LexWsBlob::PerBlock() {
  UInt32 blockSize;
  blockSize = TheBefe->TheCache.GetBlockSize();
  if (blockSize == UNaN)
    return UNaN;
  else
    return (blockSize-LexWsBlockHead::PersistSize(LexWs::BlockTypeBlob))/PersistSize();
}

UInt LexWsBlob::PersistSize() {
  return 5*sizeof(UInt32);
}

UInt LexWsBlob::EntryPersistSize() {
  return 1;
};

UInt LexWsBlob::DataBytesPerBlock() {
  UInt32 blockSize;
  blockSize = TheBefe->TheCache.GetBlockSize();
  if (blockSize == UNaN)
    return UNaN;
  else
    return (blockSize-LexWsBlockHead::PersistSize(LexWs::BlockTypeBlob));
}

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LexWsBlob::LexWsBlob() {
  StartUp();
}
  
LexWsBlob::LexWsBlob(LexWsBlob const &that) {
  StartUp();
  CopyFrom(that);
}

LexWsBlob::~LexWsBlob() {
  ShutDown();
}

LexWsBlob &LexWsBlob::operator=(LexWsBlob const &that) {
  StartUp();
  CopyFrom(that);
  return *this;
}
	
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status LexWsBlob::StartUp() {

  BEFE::SetNull(id);
  BEFE::SetNull(len);
  BEFE::SetNull(fstLowBlockNo);
  BEFE::SetNull(lstLowBlockNo);
  BEFE::SetNull(topBlockNo);
  
  return Error::None;
  
}
  
Status LexWsBlob::ShutDown() {
  return Error::None;
}

Status LexWsBlob::Clear() {
  return Reset();
}

Status LexWsBlob::Reset() {
  Status status;
  status = StartUp();
  return status;
}

Boolean LexWsBlob::IsNull() const {
  return BEFE::IsNull(id);
}

Status LexWsBlob::SetNull() {
  return StartUp();
}

Boolean LexWsBlob::IsEmpty() {
  return BEFE::IsNull(len) || len == 0;
}

Status LexWsBlob::SetEmpty() {
  return Error::NotImplemented;
}

Status LexWsBlob::MoveFrom(LexWsBlob const &that) {
  
  Status status;
  
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexWsBlob));
  status = ((LexWsBlob *)&that)->StartUp();

  return status;
  
}

Status LexWsBlob::CopyFrom(LexWsBlob const &that) {
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexWsBlob));
  return Error::None;
}

Boolean LexWsBlob::IsConsumable() const {
  return false;
}

LexWsBlob &LexWsBlob::Consumable() const {
  return *(LexWsBlob *)this;
}

LexWsBlob &LexWsBlob::NotConsumable() const {
  return *(LexWsBlob *)this;
}

//----------------------------------------------------------------------
//
// Persistence
//

Status LexWsBlob::Write(Id fileId, UInt blockNo, UInt entryNo) { // LexWsBlob.Write...
  
  Status          status;
  LexWsBlockHead  blockHead;
  Byte           *block;
  UInt            offset;
  UInt            anInt;
  
  // Sanity checks...
  status = blockHead.Read(fileId, blockNo);
  if (status) goto SOMEERROR;
  if (blockHead.type != LexWs::BlockTypeBlob) goto WRONGTYPE;
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
  anInt = len;
  *block++ = (Byte)(anInt>>24);
  *block++ = (Byte)(anInt>>16);
  *block++ = (Byte)(anInt>>8);
  *block++ = (Byte)(anInt);
  anInt = fstLowBlockNo;
  *block++ = (Byte)(anInt>>24);
  *block++ = (Byte)(anInt>>16);
  *block++ = (Byte)(anInt>>8);
  *block++ = (Byte)(anInt);
  anInt = lstLowBlockNo;
  *block++ = (Byte)(anInt>>24);
  *block++ = (Byte)(anInt>>16);
  *block++ = (Byte)(anInt>>8);
  *block++ = (Byte)(anInt);
  anInt = topBlockNo;
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
  
} // ...LexWsBlob.Write

Status LexWsBlob::Read(Id fileId, UInt blockNo, UInt entryNo) { // LexWsBlob.Read...

  Status          status;
  LexWsBlockHead  blockHead;
  Byte           *block;
  UInt            offset;
  UInt            anInt;
  
  // Sanity checks...
  status = blockHead.Read(fileId, blockNo);
  if (status) goto SOMEERROR;
  if (blockHead.type != LexWs::BlockTypeBlob) goto WRONGTYPE;
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
  len = anInt;
    
  anInt  = ((Byte)*block++)<<24;
  anInt += ((Byte)*block++)<<16; 
  anInt += ((Byte)*block++)<<8; 
  anInt += ((Byte)*block++); 
  fstLowBlockNo = anInt;

  anInt  = ((Byte)*block++)<<24;
  anInt += ((Byte)*block++)<<16; 
  anInt += ((Byte)*block++)<<8; 
  anInt += ((Byte)*block++); 
  lstLowBlockNo = anInt;

  anInt  = ((Byte)*block++)<<24;
  anInt += ((Byte)*block++)<<16; 
  anInt += ((Byte)*block++)<<8; 
  anInt += ((Byte)*block++); 
  topBlockNo = anInt;

  // Handle errors
  while (false) {
    WRONGTYPE:  status = Error::LexWsBlockWrongType;  break;
    WRONGLEVEL: status = Error::LexWsBlockWrongLevel; break;
    BADIDX:     status = Error::LexWsBadEntryIdx;     break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
} // ...LexWsBlob.Read

} // ...Namespace BEFE