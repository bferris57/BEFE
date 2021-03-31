//----------------------------------------------------------------------
// File: LexWsArrayHead.cpp - Implementation of the BEFE LexWsArrayHead Struct
//----------------------------------------------------------------------
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

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LexWsArrayHead::LexWsArrayHead() {
  StartUp();
  return;
}

LexWsArrayHead::LexWsArrayHead(LexWsArrayHead const &that) {
  StartUp();
  CopyFrom(that);
  return;
}

LexWsArrayHead::~LexWsArrayHead() {
  ShutDown();
  return;
}

LexWsArrayHead &LexWsArrayHead::operator=(LexWsArrayHead const &that) {
  CopyFrom(that);
  return *this;
}
	
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status LexWsArrayHead::StartUp() {
  nameId          = UInt32NaN;
  // Can't decide whether or not to make hiId start at
  // NaN or 0.  Hmmm...
  //hiId            = UInt32NaN;
  hiId            = 0;
  blockType       = LexWs::BlockTypeNull;
  entrySize       = UShortNaN;
  entriesPerBlock = UShortNaN;
  numEntries      = 0;
  fstLowBlockNo   = UInt32NaN;
  lstLowBlockNo   = UInt32NaN;
  topBlockNo      = UInt32NaN;
  return Error::None;
}

Status LexWsArrayHead::ShutDown() {
  return Error::None;
}

Status LexWsArrayHead::Clear() {
  return Reset();
}

Status LexWsArrayHead::Reset() {
  return StartUp();
}

Boolean LexWsArrayHead::IsNull() const {
  return false;
}

Status LexWsArrayHead::SetNull() {
  return StartUp();
}

Boolean LexWsArrayHead::IsEmpty() {
  return false;
}

Status LexWsArrayHead::SetEmpty() {
  return StartUp();
}

Status LexWsArrayHead::MoveFrom(LexWsArrayHead const &that) {
  Status status;
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(*this));
  status = ((LexWsArrayHead *)&that)->StartUp();
  return status;
}

Status LexWsArrayHead::CopyFrom(LexWsArrayHead const &that) {
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(*this));
  return Error::None;
}

//----------------------------------------------------------------------
//
// Other BEFE functionality
//

String LexWsArrayHead::ToString() const {
  return String();
}

Strings LexWsArrayHead::ToStrings() const {
  return Strings();
}

//----------------------------------------------------------------------
//
// Persistence
//

UInt LexWsArrayHead::PerBlock() const {
  UInt32 blockSize;
  blockSize = TheBefe->TheCache.GetBlockSize();
  if (blockSize == UInt32NaN)
    return 0;
  else
    return blockSize/PersistSize();
}

UInt LexWsArrayHead::PersistSize() const {
  return 1*sizeof(Byte) +
         2*sizeof(Id32) +
         2*sizeof(UShort) +
         4*sizeof(UInt32);
}

Status LexWsArrayHead::SetEntryPersistSize(UInt numBytes) {
  
  Status status;
  UInt32 blockSize;
  
  if (numBytes >= (UInt)UInt16NaN) goto BADPARAM;
  
  blockSize = TheBefe->TheCache.GetBlockSize();
  entrySize = numBytes;
  
  if (blockSize == UInt32NaN || entrySize == 0)
    entriesPerBlock = 0;
  else
    entriesPerBlock = (blockSize-LexWsBlockHead::PersistSize((UInt)blockType))/entrySize;
    
  // Handle errors
  status = Error::None;
  while (false) {
    BADPARAM: status = Error::InvalidParameter; break;
  }
  if (status)
    entriesPerBlock = UInt16NaN;
    
  return status;
  
}

Status LexWsArrayHead::Write(UInt fileId, UInt blockNo, UInt offset) {
  
  Status          status;
  LexWsBlockHead  blockHead;
  UInt            bhPersistSize;
  UInt            blockSize;
  Byte           *bytes;
  UInt32          anInt;
  
  // Get the block header...
  status = blockHead.Read(fileId, blockNo);
  if (status) goto SOMEERROR;
  
  // Sanity checks...
  bhPersistSize = blockHead.PersistSize();
  blockSize = TheBefe->TheCache.GetBlockSize();
  if (blockSize == UInt32NaN) goto INTERNAL;
  if (offset < bhPersistSize || offset >= (blockSize - PersistSize()))
    goto BADOFFSET;
    
  // Initialise output pointer...
  bytes = TheBefe->TheCache._GetBlockPointer(fileId, blockNo);
  if (BEFE::IsNull(bytes)) goto NULLPOINTER;
  bytes += offset;

  //  
  // Write the data...
  //
  
  *bytes++ = blockType;

  anInt = nameId;
  *bytes++ = (Byte)(anInt>>24);
  *bytes++ = (Byte)(anInt>>16);
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)(anInt);
  
  anInt = hiId;
  *bytes++ = (Byte)(anInt>>24);
  *bytes++ = (Byte)(anInt>>16);
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)(anInt);
  
  anInt = (UInt)entrySize;
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)(anInt);
  
  anInt = (UInt)entriesPerBlock;
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)(anInt);

  anInt = numEntries;
  *bytes++ = (Byte)(anInt>>24);
  *bytes++ = (Byte)(anInt>>16);
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)(anInt);
  
  anInt = fstLowBlockNo;
  *bytes++ = (Byte)(anInt>>24);
  *bytes++ = (Byte)(anInt>>16);
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)(anInt);
  
  anInt = lstLowBlockNo;
  *bytes++ = (Byte)(anInt>>24);
  *bytes++ = (Byte)(anInt>>16);
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)(anInt);
  
  anInt = topBlockNo;
  *bytes++ = (Byte)(anInt>>24);
  *bytes++ = (Byte)(anInt>>16);
  *bytes++ = (Byte)(anInt>>8);
  *bytes++ = (Byte)(anInt);
  
  // Flag block as dirty...
  status = TheBefe->TheCache._SetBlockIsDirty(fileId, blockNo);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    INTERNAL:    status = Error::Internal;            break;
    BADOFFSET:   status = Error::LexWsBadBlockOffset; break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

Status LexWsArrayHead::Read(UInt fileId, UInt blockNo, UInt offset) {
  
  Status          status;
  LexWsBlockHead  blockHead;
  UInt            bhPersistSize;
  UInt            blockSize;
  Byte           *bytes;
  UInt32          anInt;
  
  // Get the block header...
  status = blockHead.Read(fileId, blockNo);
  if (status) goto SOMEERROR;
  
  // Sanity checks...
  bhPersistSize = blockHead.PersistSize();
  blockSize = TheBefe->TheCache.GetBlockSize();
  if (offset < bhPersistSize || offset >= blockSize - PersistSize())
    goto BADOFFSET;
    
  // Initialise output pointer...
  bytes = TheBefe->TheCache._GetBlockPointer(fileId, blockNo);
  if (BEFE::IsNull(bytes)) goto NULLPOINTER;
  bytes += offset;
  
  //
  // Read the data...
  //
  
  blockType = *bytes++;
  
  anInt  = ((UInt32)*bytes++)<<24;
  anInt |= ((UInt32)*bytes++)<<16;
  anInt |= ((UInt32)*bytes++)<<8;
  anInt |=  (UInt32)*bytes++;
  nameId = (Id32)anInt;
  
  anInt  = ((UInt32)*bytes++)<<24;
  anInt |= ((UInt32)*bytes++)<<16;
  anInt |= ((UInt32)*bytes++)<<8;
  anInt |=  (UInt32)*bytes++;
  hiId = (Id32)anInt;

  anInt  = ((UInt32)*bytes++)<<8;
  anInt |=  (UInt32)*bytes++;
  entrySize = (UShort)anInt;
  
  anInt  = ((UInt32)*bytes++)<<8;
  anInt |=  (UInt32)*bytes++;
  entriesPerBlock = (UShort)anInt;

  anInt  = ((UInt32)*bytes++)<<24;
  anInt |= ((UInt32)*bytes++)<<16;
  anInt |= ((UInt32)*bytes++)<<8;
  anInt |=  (UInt32)*bytes++;
  numEntries = (UShort)anInt;
    
  anInt  = ((UInt32)*bytes++)<<24;
  anInt |= ((UInt32)*bytes++)<<16;
  anInt |= ((UInt32)*bytes++)<<8;
  anInt |=  (UInt32)*bytes++;
  fstLowBlockNo = anInt;

  anInt  = ((UInt32)*bytes++)<<24;
  anInt |= ((UInt32)*bytes++)<<16;
  anInt |= ((UInt32)*bytes++)<<8;
  anInt |=  (UInt32)*bytes++;
  lstLowBlockNo = anInt;

  anInt  = ((UInt32)*bytes++)<<24;
  anInt |= ((UInt32)*bytes++)<<16;
  anInt |= ((UInt32)*bytes++)<<8;
  anInt |=  (UInt32)*bytes++;
  topBlockNo = anInt;
  
  // Flag block as dirty...
  status = TheBefe->TheCache._SetBlockIsDirty(fileId, blockNo);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    BADOFFSET:   status = Error::LexWsBadBlockOffset; break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

} // ...namespace BEFE