//----------------------------------------------------------------------
// File: LexWsHead.cpp - Implementation of the BEFE LexWsHead Struct
//----------------------------------------------------------------------
//
// This file contains the C++ implementation of the BEFE LexWsHead Struct
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "LexWs.h"
#include "PersistHead.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LexWsHead::LexWsHead() {
  StartUp();
}

LexWsHead::LexWsHead(LexWsHead const &that) {
  StartUp();
  CopyFrom(that);
}

LexWsHead::~LexWsHead() {
  ShutDown();
}

LexWsHead &LexWsHead::operator=(LexWsHead const &that) {
  StartUp();
  CopyFrom(that);
  return *this;
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status LexWsHead::StartUp() {

  workspaceId = 0;
  BEFE::SetNull(blkSize);
  BEFE::SetNull(timeCreated);
  BEFE::SetNull(timeUpdated);

  // Free Blocks
  BEFE::SetNull(fstFreeBlock);
  BEFE::SetNull(lstFreeBlock);

  // Array Headers
  blobs.StartUp();
  blobs.blockType = LexWs::BlockTypeBlob;
  files.StartUp();
  files.blockType = LexWs::BlockTypeFile;
  strings.StartUp();
  strings.blockType = LexWs::BlockTypeString;
  BEFE::SetNull(stringsBlobId);
  
  return Error::None;

}

Status LexWsHead::ShutDown() {
  return Error::None;
}

Status LexWsHead::Clear() {
  return Reset();
}

Status LexWsHead::Reset() {
  Status retStatus;
  Status status;
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  return retStatus;
}

Boolean LexWsHead::IsNull() const {
  return BEFE::IsNull(blkSize);
}

Status LexWsHead::SetNull() {
  Status status;
  status = Reset();
  return status;
}

Boolean LexWsHead::IsEmpty() {
  return IsNull();
}

Status LexWsHead::SetEmpty() {
  return Error::NotImplemented;
}

Status LexWsHead::MoveFrom(LexWsHead const &that) {
  Status status;
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexWsHead));
  status = ((LexWsHead *)&that)->Reset();
  return status;
}

Status LexWsHead::CopyFrom(LexWsHead const &that) {
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexWsHead));
  return Error::None;
}

Boolean LexWsHead::IsConsumable() const {
  return false;
}

LexWsHead &LexWsHead::Consumable() const {
  return *(LexWsHead *)this;
}

LexWsHead &LexWsHead::NotConsumable() const {
  return *(LexWsHead *)this;
}

//----------------------------------------------------------------------
//
// Other BEFE functionality
//

String LexWsHead::ToString() const {

  String str;

  return str.Consumable();

}

Strings LexWsHead::ToStrings() const {

  Strings strs;

  return strs.Consumable();

}

//----------------------------------------------------------------------
//
// Persistence
//

UInt LexWsHead::PersistSize() const {
  return sizeof(LexWsHead);
}

Status LexWsHead::Write(Id fileId) { // LexWsHead.Write...

  Status       status;
  PersistHead  pHead;
  Byte        *block;
  Byte        *bytes;
  UInt32       anInt;
  UInt64       aLong;
  
  status = LoadFileBlock(fileId, 0);
  if (status) goto SOMEERROR;

  block = TheBefe->TheCache._GetBlockPointer(fileId, 0);
  if (BEFE::IsNull(block)) goto NULLPOINTER;
  bytes = block + pHead.PersistSize();
  
  anInt = (UInt32)workspaceId;
  *bytes++ = (Byte)(anInt>>24);
  *bytes++ = (Byte)(anInt>>16);
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)anInt;
  
  anInt = (UInt32)blkSize;
  *bytes++ = (Byte)(anInt>>24);
  *bytes++ = (Byte)(anInt>>16);
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)anInt;
  
  aLong = (UInt64)timeCreated;
  *bytes++ = (Byte)(aLong>>56);
  *bytes++ = (Byte)(aLong>>48);
  *bytes++ = (Byte)(aLong>>40);
  *bytes++ = (Byte)(aLong>>32);
  *bytes++ = (Byte)(aLong>>24);
  *bytes++ = (Byte)(aLong>>16);
  *bytes++ = (Byte)(aLong>>8);
  *bytes++ = (Byte)aLong;

  aLong = (UInt64)timeUpdated;
  *bytes++ = (Byte)(aLong>>56);
  *bytes++ = (Byte)(aLong>>48);
  *bytes++ = (Byte)(aLong>>40);
  *bytes++ = (Byte)(aLong>>32);
  *bytes++ = (Byte)(aLong>>24);
  *bytes++ = (Byte)(aLong>>16);
  *bytes++ = (Byte)(aLong>>8);
  *bytes++ = (Byte)aLong;

  // Free Blocks
  anInt = (UInt32)fstFreeBlock;
  *bytes++ = (Byte)(anInt>>24);
  *bytes++ = (Byte)(anInt>>16);
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)anInt;
  
  anInt = (UInt32)lstFreeBlock;
  *bytes++ = (Byte)(anInt>>24);
  *bytes++ = (Byte)(anInt>>16);
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)anInt;
  
  // Array Headers
  status = blobs.Write(fileId, 0, bytes-block);
  if (status) goto SOMEERROR;
  bytes += blobs.PersistSize();

  status = files.Write(fileId, 0, bytes-block);
  if (status) goto SOMEERROR;
  bytes += files.PersistSize();

  status = strings.Write(fileId, 0, bytes-block);
  if (status) goto SOMEERROR;
  bytes += strings.PersistSize();

  anInt = (Int32)stringsBlobId;
  *bytes++ = (Byte)(anInt>>24);
  *bytes++ = (Byte)(anInt>>16);
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)anInt;
  
  // Flush the block
  status = TheBefe->TheCache._SetBlockIsDirty(fileId, 0);
  if (status) goto SOMEERROR;
  status = FlushFileBlock(fileId, 0);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }

  return status;

} // ...LexWshead.Write

Status LexWsHead::Read(Id fileId) {

  Status       status;
  PersistHead  pHead;
  Byte        *block;
  Byte        *bytes;
  LexWsHead    before;
  UInt32       anInt;
  UInt32       anInt2;
  
  MemoryCopyRaw((Byte *)this, (Byte *)&before, sizeof(LexWsHead));

  status = LoadFileBlock(fileId, 0);
  if (status) goto SOMEERROR;

  block = TheBefe->TheCache._GetBlockPointer(fileId, 0);
  if (BEFE::IsNull(block)) goto NULLPOINTER;
  bytes = block + pHead.PersistSize();

  anInt  = (UInt32)(*bytes++ << 24);
  anInt |= (UInt32)(*bytes++ << 16);
  anInt |= (UInt32)(*bytes++ << 8);
  anInt |= (UInt32)(*bytes++ );
  workspaceId = (Id32)anInt;
  
  anInt  = (UInt32)(*bytes++ << 24);
  anInt |= (UInt32)(*bytes++ << 16);
  anInt |= (UInt32)(*bytes++ << 8);
  anInt |= (UInt32)(*bytes++ );
  blkSize = (UInt32)anInt;
  
  anInt   = (UInt32)(*bytes++ << 24);
  anInt  |= (UInt32)(*bytes++ << 16);
  anInt  |= (UInt32)(*bytes++ << 8);
  anInt  |= (UInt32)(*bytes++ );
  anInt2  = (UInt32)(*bytes++ << 24);
  anInt2 |= (UInt32)(*bytes++ << 16);
  anInt2 |= (UInt32)(*bytes++ << 8);
  anInt2 |= (UInt32)(*bytes++ );
  timeCreated = (((Long)anInt) << 32) | ((Long)anInt2);
  
  anInt   = (UInt32)(*bytes++ << 24);
  anInt  |= (UInt32)(*bytes++ << 16);
  anInt  |= (UInt32)(*bytes++ << 8);
  anInt  |= (UInt32)(*bytes++ );
  anInt2  = (UInt32)(*bytes++ << 24);
  anInt2 |= (UInt32)(*bytes++ << 16);
  anInt2 |= (UInt32)(*bytes++ << 8);
  anInt2 |= (UInt32)(*bytes++ );
  timeUpdated = (((Long)anInt) << 32) | ((Long)anInt2);

  // Free Blocks
  anInt  = (UInt32)(*bytes++ << 24);
  anInt |= (UInt32)(*bytes++ << 16);
  anInt |= (UInt32)(*bytes++ << 8);
  anInt |= (UInt32)(*bytes++ );
  fstFreeBlock = (UInt32)anInt;
  
  anInt  = (UInt32)(*bytes++ << 24);
  anInt |= (UInt32)(*bytes++ << 16);
  anInt |= (UInt32)(*bytes++ << 8);
  anInt |= (UInt32)(*bytes++ );
  lstFreeBlock = (UInt32)anInt;
  
  // Array Headers...
  status = blobs.Read(fileId, 0, bytes-block);
  if (status) goto SOMEERROR;
  bytes += blobs.PersistSize();
  
  status = files.Read(fileId, 0, bytes-block);
  if (status) goto SOMEERROR;
  bytes += files.PersistSize();
  
  status = strings.Read(fileId, 0, bytes-block);
  if (status) goto SOMEERROR;
  bytes += strings.PersistSize();
  
  anInt  = (UInt32)(*bytes++ << 24);
  anInt |= (UInt32)(*bytes++ << 16);
  anInt |= (UInt32)(*bytes++ << 8);
  anInt |= (UInt32)(*bytes++ );
  stringsBlobId = (Id32)anInt;

  // Handle errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  if (status)
    MemoryCopyRaw((Byte *)&before, (Byte *)this, sizeof(LexWsHead));

  return status;

}

} // ...Namespace BEFE
