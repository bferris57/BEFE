//----------------------------------------------------------------------
// File: LexWsFile.cpp - Implementation of the BEFE LexWsFile Struct
//----------------------------------------------------------------------
//
// This file contains the C++ implementation of the BEFE LexWsFile Struct
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
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

UInt LexWsFile::PerBlock() {
  UInt32 blockSize;
  blockSize = TheBefe->TheCache.GetBlockSize();
  if (blockSize == UNaN)
    return UNaN;
  else
    return (blockSize-LexWsBlockHead::PersistSize(LexWs::BlockTypeFile))/PersistSize();
}

UInt LexWsFile::PersistSize() {
  return 6*sizeof(UInt32) +
         1*sizeof(Long);
}

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LexWsFile::LexWsFile() {
  StartUp();
}
  
LexWsFile::LexWsFile(LexWsFile const &that) {
  StartUp();
  CopyFrom(that);
}

LexWsFile::~LexWsFile() {
  ShutDown();
}

LexWsFile &LexWsFile::operator=(LexWsFile const &that) {
  StartUp();
  CopyFrom(that);
  return *this;
}
	
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status LexWsFile::StartUp() {

  BEFE::SetNull(id);
  BEFE::SetNull(name);
  BEFE::SetNull(text);
  BEFE::SetNull(lang);
  BEFE::SetNull(tokens);
  crc.Reset();
  BEFE::SetNull(snapTime);
  
  return Error::None;
  
}
  
Status LexWsFile::ShutDown() {
  return Error::None;
}

Status LexWsFile::Clear() {
  return Reset();
}

Status LexWsFile::Reset() {
  Status status;
  status = StartUp();
  return status;
}

Boolean LexWsFile::IsNull() const {
  return BEFE::IsNull(id);
}

Status LexWsFile::SetNull() {
  return StartUp();
}

Boolean LexWsFile::IsEmpty() {
  return  BEFE::IsNull(id);
}

Status LexWsFile::SetEmpty() {
  return Error::NotImplemented;
}

Status LexWsFile::MoveFrom(LexWsFile const &that) {
  
  Status status;
  
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexWsFile));
  status = ((LexWsFile *)&that)->StartUp();

  return status;
  
}

Status LexWsFile::CopyFrom(LexWsFile const &that) {
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexWsFile));
  return Error::None;
}

Boolean LexWsFile::IsConsumable() const {
  return false;
}

LexWsFile &LexWsFile::Consumable() const {
  return *(LexWsFile *)this;
}

LexWsFile &LexWsFile::NotConsumable() const {
  return *(LexWsFile *)this;
}

//----------------------------------------------------------------------
//
// Persistence
//

Status LexWsFile::Write(Id fileId, UInt blockNo, UInt entryNo) { // LexWsFile.Write...
  
  Status          status;
  LexWsBlockHead  blockHead;
  Byte           *block;
  UInt            offset;
  UInt            anInt;
  
  // Sanity checks...
  status = blockHead.Read(fileId, blockNo);
  if (status) goto SOMEERROR;
  if (blockHead.type != LexWs::BlockTypeFile) goto WRONGTYPE;
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
  
  anInt = name;
  *block++ = (Byte)(anInt>>24);
  *block++ = (Byte)(anInt>>16);
  *block++ = (Byte)(anInt>>8);
  *block++ = (Byte)(anInt);

  anInt = text;
  *block++ = (Byte)(anInt>>24);
  *block++ = (Byte)(anInt>>16);
  *block++ = (Byte)(anInt>>8);
  *block++ = (Byte)(anInt);

  anInt = lang;
  *block++ = (Byte)(anInt>>24);
  *block++ = (Byte)(anInt>>16);
  *block++ = (Byte)(anInt>>8);
  *block++ = (Byte)(anInt);
  anInt = tokens;
  *block++ = (Byte)(anInt>>24);
  *block++ = (Byte)(anInt>>16);
  *block++ = (Byte)(anInt>>8);
  *block++ = (Byte)(anInt);

  anInt = crc;
  *block++ = (Byte)(anInt>>24);
  *block++ = (Byte)(anInt>>16);
  *block++ = (Byte)(anInt>>8);
  *block++ = (Byte)(anInt);

  anInt = ((Long)snapTime)>>32;
  *block++ = (Byte)(anInt>>24);
  *block++ = (Byte)(anInt>>16);
  *block++ = (Byte)(anInt>>8);
  *block++ = (Byte)(anInt);

  anInt = (UInt)(Long)snapTime;
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
  
} // ...LexWsFile.Write

Status LexWsFile::Read(Id fileId, UInt blockNo, UInt entryNo) { // LexWsFile.Read...

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
  name = anInt;

  anInt  = ((Byte)*block++)<<24;
  anInt += ((Byte)*block++)<<16; 
  anInt += ((Byte)*block++)<<8; 
  anInt += ((Byte)*block++); 
  text = anInt;

  anInt  = ((Byte)*block++)<<24;
  anInt += ((Byte)*block++)<<16; 
  anInt += ((Byte)*block++)<<8; 
  anInt += ((Byte)*block++); 
  lang = anInt;

  anInt  = ((Byte)*block++)<<24;
  anInt += ((Byte)*block++)<<16; 
  anInt += ((Byte)*block++)<<8; 
  anInt += ((Byte)*block++); 
  id = tokens;

  anInt  = ((Byte)*block++)<<24;
  anInt += ((Byte)*block++)<<16; 
  anInt += ((Byte)*block++)<<8; 
  anInt += ((Byte)*block++); 
  crc = anInt;

  anInt  = ((Byte)*block++)<<24;
  anInt += ((Byte)*block++)<<16; 
  anInt += ((Byte)*block++)<<8; 
  anInt += ((Byte)*block++); 
  snapTime = ((Long)anInt)<<32;
  anInt  = ((Byte)*block++)<<24;
  anInt += ((Byte)*block++)<<16; 
  anInt += ((Byte)*block++)<<8; 
  anInt += ((Byte)*block++); 
  snapTime = ((Long)snapTime) + ((Long)anInt);

  // Handle errors
  while (false) {
    WRONGTYPE:  status = Error::LexWsBlockWrongType;  break;
    WRONGLEVEL: status = Error::LexWsBlockWrongLevel; break;
    BADIDX:     status = Error::LexWsBadEntryIdx;     break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
} // ...LexWsFile.Read

} // ...Namespace BEFE