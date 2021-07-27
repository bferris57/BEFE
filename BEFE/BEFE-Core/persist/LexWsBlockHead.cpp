//----------------------------------------------------------------------
// File: LexWsBlockHead.cpp - Implementation of the BEFE LexWsBlockHead Struct
//----------------------------------------------------------------------
//
// This file contains the C++ implementation of the BEFE LexWsBlockHead Struct
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"

#include "LexWorkspace.h"
#include "LexWs.h"
#include "PersistHead.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Static Class Methods
//

Status LexWsBlockHead::ValidateBlockType(UInt vType) {
  if (IsValidBlockType(vType))
    return Error::None;
  else
    return Error::LexWsInvalidBlockType;
}

Boolean LexWsBlockHead::IsValidBlockType(UInt vType) {
  return (vType < LexWs::NumBlockTypes) || (vType == LexWs::BlockTypeFree);
}

UInt LexWsBlockHead::PersistSize(UInt type) {
  return 10;
}

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LexWsBlockHead::LexWsBlockHead() {
  StartUp();
}

LexWsBlockHead::LexWsBlockHead(LexWsBlockHead const &that) {
  StartUp();
  CopyFrom(that);
}

LexWsBlockHead::~LexWsBlockHead() {
  ShutDown();
}

LexWsBlockHead &LexWsBlockHead::operator=(LexWsBlockHead const &that) {
  StartUp();
  CopyFrom(that);
  return *this;
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status LexWsBlockHead::StartUp() {

  type  = 0;
  level = 0;
  nextBlockNo = UInt32NaN;  
  prevBlockNo = UInt32NaN;  
  numEntries  = UInt16NaN;
  
  return Error::None;

}

Status LexWsBlockHead::ShutDown() {
  return Error::None;
}

Status LexWsBlockHead::Clear() {
  return Reset();
}

Status LexWsBlockHead::Reset() {
  return StartUp();
}

Boolean LexWsBlockHead::IsNull() const {
  return type == LexWs::BlockTypeNull;
}

Status LexWsBlockHead::SetNull() {
  return StartUp();
}

Boolean LexWsBlockHead::IsEmpty() {
  return (type == LexWs::BlockTypeNull) || (numEntries == 0);
}

Status LexWsBlockHead::SetEmpty() {
  return numEntries = 0;
}

Status LexWsBlockHead::MoveFrom(LexWsBlockHead const &that) {
  Status status;
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexWsBlockHead));
  status = ((LexWsBlockHead *)&that)->Reset();
  return status;
}

Status LexWsBlockHead::CopyFrom(LexWsBlockHead const &that) {
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexWsBlockHead));
  return Error::None;
}

//----------------------------------------------------------------------
//
// Other BEFE functionality
//

static char const *lexBlockTypeStrings[] = {
   "Free"
  ,"Blob"
  ,"String"
  ,"File"
  ,"Hash"
  ,0};

String LexWsBlockHead::ToString() const {

  String str;

  str = "type=";
  if (type < LexWs::NumBlockTypes)
    str += lexBlockTypeStrings[type];
  else if (type == LexWs::BlockTypeNull)
    str += "0xFF";
  else {
    str += "0x";
    if ((type>>4) <= 0x09)
      str += (char)('0'+(type>>4));
    else
      str += (char)('A'+((type>>4)-0x0A));
    if ((type&0x04) <= 0x09)
      str += (char)('0'+(type&0x04));
    else
      str += (char)('A'+((type&0x04)-0x0A));
  }
  str += ",level=";
  str += level;
  str += ",nextBlockNo=0x";
  str += ToHexString(nextBlockNo);
  str += ",prevBlockNo=0x";
  str += ToHexString(prevBlockNo);
  if (IsIndexBlock()) {
    str += ",numEntry=0x";
    str += ToHexString(numEntries);
  }
  
  return str.Consumable();

}

Strings LexWsBlockHead::ToStrings() const {

  Strings strings;
  String  str;

  str = "type        = ";
  if (type < LexWs::NumBlockTypes)
    str += lexBlockTypeStrings[type];
  else if (type == LexWs::BlockTypeNull)
    str += "0xFF";
  else {
    str += "0x";
    if ((type>>4) <= 0x09)
      str += (char)('0'+(type>>4));
    else
      str += (char)('A'+((type>>4)-0x0A));
    if ((type&0x04) <= 0x09)
      str += (char)('0'+(type&0x04));
    else
      str += (char)('A'+((type&0x04)-0x0A));
  }
  strings.Append(str.Consumable());  
  strings.Append(String("level       = ")<<level);
  strings.Append(String("nextBlockNo = 0x")<<ToHexString(nextBlockNo));
  strings.Append(String("prevBlockNo = 0x")<<ToHexString(prevBlockNo));
  if (IsIndexBlock())
    strings.Append(String("numEntry    = 0x")<<ToHexString(numEntries));
  
  return strings.Consumable();

}

//----------------------------------------------------------------------
//
// Persistence
//

Boolean LexWsBlockHead::IsIndexBlock() const {
  return (level != UInt8NaN) && (level != 0);
}

UInt LexWsBlockHead::PersistSize() const {
  return (type == LexWs::BlockTypeNull) ? 0 : 10;
}

Status LexWsBlockHead::Write(Id fileId, UInt32 blockNo) {

  Status  status;
  Byte   *block;
  UInt    offset;

  if (BEFE::IsNull(TheBefe)) goto NOBEFE;

  status = TheBefe->TheCache.LoadFileBlock(fileId, blockNo);
  if (status) goto SOMEERROR;
  block = TheBefe->TheCache._GetBlockPointer(fileId, blockNo);
  if (BEFE::IsNull(block)) goto NULLPOINTER;
 
  // Nothing to do if nothing to write...
  // (but we'll let LexWs.BlockTypeNull through okay)
  if (type != LexWs::BlockTypeNull && IsNull()) goto NULLHEADER;

  // Write it... 
  offset = 0;
  block[offset++] = type;
  block[offset++] = (Byte)(nextBlockNo >> 16);
  block[offset++] = (Byte)(nextBlockNo >> 8);
  block[offset++] = (Byte)(nextBlockNo);
  block[offset++] = level;
  block[offset++] = (Byte)(prevBlockNo >> 16);
  block[offset++] = (Byte)(prevBlockNo >> 8);
  block[offset++] = (Byte)(prevBlockNo);
  block[offset++] = (Byte)(numEntries >> 8);
  block[offset]   = (Byte)(numEntries);
    
  // Let Cache know it's dirty...
  status = TheBefe->TheCache._SetBlockIsDirty(fileId, blockNo);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOBEFE:      status = Error::InternalNoTheBefe;    break;
    NULLPOINTER: status = Error::InternalNullPointer;  break;
    NULLHEADER:  status = Error::LexWsNullBlockHeader; break;
    SOMEERROR:                                         break;
  }
  if (status)
    StartUp();

  return status;

}

Status LexWsBlockHead::Read(Id fileId, UInt32 blockNo) {

  Status  status;
  Byte   *block;

  if (BEFE::IsNull(TheBefe)) goto NOBEFE;
  
  // Special case for block 0 (act as it empty header)
  if (blockNo == 0) {
    StartUp();
    goto OK;
  }
  
  // Read it...
  status = TheBefe->TheCache.LoadFileBlock(fileId, blockNo);
  if (status) goto SOMEERROR;
  block = TheBefe->TheCache._GetBlockPointer(fileId, blockNo);
  if (BEFE::IsNull(block)) goto NULLPOINTER;
  
  type  = block[0];
  level = block[4];
  nextBlockNo = (((UInt32)block[1]) << 16) | (((UInt32)block[2])<<8) | (UInt32)block[3];
  if (nextBlockNo == 0xFFFFFFu)
    nextBlockNo = 0xFFFFFFFFu;
  prevBlockNo = (((UInt32)block[5]) << 16) | (((UInt32)block[6])<<8) | (UInt32)block[7];
  if (prevBlockNo == 0xFFFFFFu)
    prevBlockNo = 0xFFFFFFFFu;
  numEntries = ((UInt16)block[8] << 8) | (UInt16)block[9];
    
  // Handle errors
  while (false) {
    OK:          status = Error::None;                break;
    NOBEFE:      status = Error::InternalNoTheBefe;   break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  if (status)
    StartUp();

  return status;

}

} // ...Namespace BEFE
