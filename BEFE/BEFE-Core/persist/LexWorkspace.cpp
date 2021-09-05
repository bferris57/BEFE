//----------------------------------------------------------------------
// File: LexWorkspace.cpp - Implementation of the BEFE LexWorkspace Class
//----------------------------------------------------------------------
//
// This file contains the C++ implementation of the BEFE LexWorkspace Class
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Lex.h"
#include "LexWorkspace.h"
#include "LexWs.h"
#include "PersistHead.h"
#include "Error.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LexWorkspace::LexWorkspace() {
  StartUp();
}
  
LexWorkspace::LexWorkspace(LexWorkspace const &that) {
  StartUp();
  CopyFrom(that);
}

LexWorkspace::~LexWorkspace() {
  ShutDown();
}

LexWorkspace &LexWorkspace::operator=(LexWorkspace const &that) {
  StartUp();
  CopyFrom(that);
  return *this;
}
	
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status LexWorkspace::StartUp() {

  Status retStatus;
  
  isOpen = false;
  lex = NULL;
  BEFE::SetNull(fileName);
  BEFE::SetNull(fileId);
  BEFE::SetNull(blkSize);
  
  retStatus = wsHead.StartUp();
  wsHeadIsDirty = false;
  
  return retStatus;
  
}
  
Status LexWorkspace::ShutDown() {
  
  Status retStatus;
  Status status;
  
  retStatus = Error::None;
  if (isOpen) {
    status = _Close();
    if (status && !retStatus) retStatus = status;
  }
    
  if (!BEFE::IsNull(lex))
    retStatus = lex->Detach(*this);
  else
    retStatus = Error::None;
  status = fileName.SetNull();
  if (status && !retStatus) retStatus = status;
  BEFE::SetNull(fileId);
  BEFE::SetNull(blkSize);

  status = wsHead.ShutDown();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Status LexWorkspace::Clear() {
  
  Status retStatus;
  
  retStatus = wsHead.Reset();
  
  return retStatus;
  
}

Status LexWorkspace::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Boolean LexWorkspace::IsNull() const {
  return false;
}

Status LexWorkspace::SetNull() {
  return Error::NotImplemented;
}

Boolean LexWorkspace::IsEmpty() {
  return false;
}

Status LexWorkspace::SetEmpty() {
  return Error::NotImplemented;
}

Status LexWorkspace::MoveFrom(LexWorkspace const &that) {
  return Error::NotImplemented;
}

Status LexWorkspace::CopyFrom(LexWorkspace const &that) {
  return Error::NotImplemented;
}

Boolean LexWorkspace::IsConsumable() const {
  return false;
}

LexWorkspace &LexWorkspace::Consumable() const {
  return *(LexWorkspace *)this;
}

LexWorkspace &LexWorkspace::NotConsumable() const {
  return *(LexWorkspace *)this;
}

//----------------------------------------------------------------------
//
// Lex Management
//

Boolean LexWorkspace::IsAttached() const  {
  return !BEFE::IsNull(lex);
}

Status LexWorkspace::_Attach(Lex &pLex) {
  
  Status status;
  
  if (!BEFE::IsNull(lex)) goto ALREADY;
  lex = &pLex;
  
  // Handle errors
  status = Error::None;
  while (false) {
    ALREADY: status = Error::LexWorkspaceIsAttached; break;
  }
  
  return status;
  
}

Status LexWorkspace::_Detach() {
  
  Status status;
  
  if (BEFE::IsNull(lex)) goto NOTATTACHED;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTATTACHED: status = Error::LexWorkspaceNotAttached; break;
  }
  
  return status;
  
}
  
//----------------------------------------------------------------------
//
// File Management
//

Boolean LexWorkspace::IsOpen() const {
  return isOpen;
}

Status LexWorkspace::_Open() {
  
  Status      status;
  FileStream  fStream;
  PersistHead pHead;
  Byte        empty[256];
  UInt        remWrite;
  UInt        toWrite;
  UInt        numWritten;
  
  NEED_TheBefe
  NEED_TheOS
  
  if (!IsAttached()) goto NOTATTACHED;
  if (isOpen) goto OK;

  // Make sure we have a file name...
  if (fileName.Length() == 0) goto NOFILE;
  
  // Create it if not there...
  if (!Exists(fileName)) {
    // Get the Cache Block Size
    if (BEFE::IsNull(blkSize))
      blkSize = TheBefe->TheCache.GetBlockSize();
    if (BEFE::IsNull(blkSize)) goto INTERNAL;
    // Write empty Header Block
    status = pHead.SetTypeName("LXWS");
    if (status) goto SOMEERROR;
    status = pHead.SetTypeRevision(0);
    if (status) goto SOMEERROR;
    status = fStream.SetName(fileName);
    if (status) goto SOMEERROR;
    status = fStream.SetAccess(File::ModeRead | File::ModeWrite);
    if (status) goto SOMEERROR;
    status = pHead.WriteTo(fStream);
    if (status) goto SOMEERROR;
    MemoryFillRaw(empty, 0xFF, sizeof(empty));
    remWrite = blkSize - pHead.PersistSize();
    while (remWrite != 0) {
      toWrite = Min(remWrite, sizeof(empty));
      status = fStream.PutBytes(empty, toWrite, numWritten);
      if (status) goto SOMEERROR;
      if (numWritten != toWrite) goto INTERNAL;
      remWrite -= numWritten;
    }
    status = fStream.Flush();
    if (status) goto SOMEERROR;
    status = fStream.Reset();
    if (status) goto SOMEERROR;
  }
  
  // Cache the file...
  status = TheBefe->TheCache.StartCacheFile(fileName, true, true);
  if (status) goto SOMEERROR;
  fileId = TheBefe->TheCache.GetFileId(fileName);
  if (BEFE::IsNull(fileId)) goto INTERNAL;
  
  // If this is the first Cache operation we've done, get the block size
  if (BEFE::IsNull(blkSize))
    blkSize = TheBefe->TheCache.GetBlockSize();
    
  // Read the LexWsHead and populate it if it's null...
  wsHeadIsDirty = false;
  status = wsHead.Read(fileId);
  if (status) goto SOMEERROR;
  if (wsHead.IsNull()) {
    wsHead.Reset();
    wsHead.workspaceId = 0;
    wsHead.timeCreated.Set();
    wsHead.timeUpdated.Set();
    wsHead.blkSize = TheBefe->TheCache.GetBlockSize();
    // Update Array Info...
    wsHead.blobs.SetEntryPersistSize(LexWsBlob::PersistSize());
    wsHead.strings.SetEntryPersistSize(LexWsString::PersistSize());
    wsHead.files.SetEntryPersistSize(LexWsFile::PersistSize());
    // Flush it
    wsHeadIsDirty = true;
    status = _FlushHead();
    if (status) goto SOMEERROR;
  }
  
  // We're open now...
  isOpen = true;
  
  // Handle errors
  while (false) {
    OK:          status = Error::None;                    break;
    NOTATTACHED: status = Error::LexWorkspaceNotAttached; break;
    NOFILE:      status = Error::LexWorkspaceNoFileName;  break;
    INTERNAL:    status = Error::Internal;                break;
    SOMEERROR:                                            break;
  }
  
  return status;
  
}

Status LexWorkspace::_Close() {
  
  Status status;
  
  if (!isOpen) goto OK;

  // Flush head first...
  status = _FlushHead();
  if (status) goto SOMEERROR;
  
  // Stop caching it now...  
  status = TheBefe->TheCache.StopCacheFile(fileName);
  if (status) goto SOMEERROR;
  
  isOpen = false;
  
  // Handle errors
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;
  
}

Status LexWorkspace::_FlushHead() {
  
  Status status;
  Long   now;
  
  if (wsHeadIsDirty) {
    wsHead.timeUpdated.Set();
    now = (Long)wsHead.timeUpdated;
    if (BEFE::IsNull((Long)wsHead.timeCreated))
      wsHead.timeCreated = now;
    status = wsHead.Write(fileId);
    if (status) goto SOMEERROR;
    wsHeadIsDirty = false;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}
  
//----------------------------------------------------------------------
//
// Workspace methods
//

Status LexWorkspace::SetFileName(String const &pFileName) {
  
  Status status;
  
  if (isOpen) {
    status = _Close();
    if (status) goto SOMEERROR;
  }
  
  // Validate the file name...
  if (pFileName.Length() == 0) goto EMPTYNAME;
  if (!IsValidPathName(pFileName)) goto BADPATH;
  if (Exists(pFileName) && !IsFile(pFileName)) goto NOTFILE;

  // Save it
  fileName = pFileName;
  
  // Handle errors
  status = Error::None;
  while (false) {
    EMPTYNAME: status = Error::FileNoName;          break;
    BADPATH:   status = Error::FileInvalidPathName; break;
    NOTFILE:   status = Error::FileNameIsNotFile;   break;
    SOMEERROR:                                      break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Workspace File Block methods
// 

Status LexWorkspace::NewFileBlock(UInt blockType, UInt32 &blockNo) {
  
  Status         status;
  LexWsBlockHead blockHead;
  
  if (BEFE::IsNull(TheBefe)) goto NOBEFE;
  status = LexWsBlockHead::ValidateBlockType(blockType);
  if (status) goto SOMEERROR;
 
  // Make sure it's open...
  status = _Open();
  if (status) goto SOMEERROR;

  // If we have a free block...
  if (!BEFE::IsNull(wsHead.fstFreeBlock)) {
    blockNo = wsHead.fstFreeBlock;
    status = blockHead.Read(fileId, blockNo);
    if (status) goto SOMEERROR;
    if (blockHead.type != LexWs::BlockTypeFree) goto NOTFREE;
    wsHead.fstFreeBlock = blockHead.nextBlockNo;
    if (BEFE::IsNull(wsHead.fstFreeBlock)) {
      wsHead.lstFreeBlock = UInt32NaN;
      wsHeadIsDirty = true;
    }
    // For Now: Flush the Workspace header
    status = _FlushHead();
    if (status) goto SOMEERROR;
  }
  
  // Else, no free blocks, need to get one off the end...
  else {
    blockNo = TheBefe->TheCache.GetFileBlockCount(fileId);
    if (blockNo == 0 || blockNo == UInt32NaN) goto INTERNAL;
    status = TheBefe->TheCache.AppendFileBlock(fileId);
    if (status) goto SOMEERROR;
    status = TheBefe->TheCache.LoadFileBlock(fileId, blockNo);
    if (status) goto SOMEERROR;
  }

  // Update the block's header  
  blockHead.type = LexWs::BlockTypeNull;
  blockHead.nextBlockNo = UInt32NaN;
  blockHead.level = 0;
  blockHead.prevBlockNo = UInt32NaN;
  status = blockHead.Write(fileId, blockNo);
  if (status) goto SOMEERROR;
    
  // Handle errors
  while (false) {
    NOBEFE:    status = Error::InternalNoTheBefe; break;
    NOTFREE:   status = Error::LexWsBlockNotFree; break;
    INTERNAL:  status = Error::Internal;          break;
    SOMEERROR:                                    break;
  }
  if (status)
    blockNo = UInt32NaN;
    
  return status;
  
}

Status LexWorkspace::FreeFileBlock(UInt32 blockNo) {
  
  Status         status;
  LexWsBlockHead blockHead;
  LexWsBlockHead otherBlockHead;
  
  if (BEFE::IsNull(TheBefe)) goto NOBEFE;
 
  // Make sure it's open...
  status = _Open();
  if (status) goto SOMEERROR;

  // Read the block header...
  status = blockHead.Read(fileId, blockNo);
  if (status) goto SOMEERROR;

  // Make sure it's not already free...
  if (blockHead.type == LexWs::BlockTypeFree) goto ISFREE;

  // If we have a last free block, forward chain it...
  if (!BEFE::IsNull(wsHead.lstFreeBlock)) {
    status = otherBlockHead.Read(fileId, wsHead.lstFreeBlock);
    if (status) goto SOMEERROR;
    otherBlockHead.nextBlockNo = blockNo;
    status = otherBlockHead.Write(fileId, wsHead.lstFreeBlock);
    if (status) goto SOMEERROR;
  }
  
  // Update this block's header  
  blockHead.type = LexWs::BlockTypeFree;
  blockHead.nextBlockNo = UInt32NaN;
  blockHead.level = 0;
  blockHead.prevBlockNo = wsHead.lstFreeBlock;
  status = blockHead.Write(fileId, blockNo);
  if (status) goto SOMEERROR;
  
  // Update Workspace Header...
  if (BEFE::IsNull(wsHead.fstFreeBlock))
    wsHead.fstFreeBlock = blockNo;
  wsHead.lstFreeBlock = blockNo;
  wsHeadIsDirty = true;
  
  // For Now: Flush the Workspace header
  status = _FlushHead();
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    NOBEFE:    status = Error::InternalNoTheBefe; break;
    ISFREE:    status = Error::LexWsBlockIsFree;  break;
    SOMEERROR:                                    break;
  }
  if (status)
    blockNo = UInt32NaN;
    
  return status;
  
}
  
} // ...Namespace BEFE
