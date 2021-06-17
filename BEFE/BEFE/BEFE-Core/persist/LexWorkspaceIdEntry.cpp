//----------------------------------------------------------------------
// File: LexWorkspaceIdEntry.cpp - Implementation of the BEFE LexWorkspace
//                                 Class IdEntry Methods
//----------------------------------------------------------------------
//
// Throughout the LexWorkspace file, there are several Id based entries
// (like LexWsBlob, LexWsString, and LexWsFile) that we manage in the
// same way but only slightly differently.
//
// The most important aspects of them is that they're each associated
// with a specific LexArrayHead instance in the Workspace.  That tells
// us several things like how big each entry is, what block type to
// manage, etc.
//
// In addition to that, ALL these entries have their Id stored in the
// entry IN THE FIRST FOUR BYTES, and in MSB order.
//
// This allows us to treat them all the same.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Lex.h"
#include "LexWorkspace.h"
#include "LexWs.h"

namespace BEFE { // Namespace BEFE...

UInt LexWorkspace::_IdEntryNew(LexWsArrayHead &arrayHead, UInt32 &blockNo, UInt32 &entryNo, Id &newId) {

  Status         status;
  UInt32         newBlockNo;
  LexWsBlockHead blockHead;
  
  // Initialise NaN return values...
  BEFE::SetNull(newId);
  BEFE::SetNull(blockNo);
  BEFE::SetNull(entryNo);
  
  // Sanity checks
  status = _Open();
  if (status) goto SOMEERROR;
  if ((UInt32)arrayHead.hiId == UInt32NaN-1) goto IDOVERFLOW;
  
  // Get Block number to append to...
  blockNo = arrayHead.lstLowBlockNo;
  if (!BEFE::IsNull(arrayHead.lstLowBlockNo)) {
    status = blockHead.Read(fileId, blockNo);
    if (status) goto SOMEERROR;
    if (blockHead.numEntries >= arrayHead.entriesPerBlock)
      BEFE::SetNull(blockNo);
  }
    
  // Create new empty block if none to insert into...
  if (BEFE::IsNull(blockNo)) {
    // New Block...
    status = NewFileBlock(arrayHead.blockType, newBlockNo);
    if (status) goto SOMEERROR;
    // Update old last block's head (if any)...
    if (!BEFE::IsNull(arrayHead.lstLowBlockNo)) {
      // Read old last block's head
      status = blockHead.Read(fileId, arrayHead.lstLowBlockNo);
      if (status) goto SOMEERROR;
      // Update it
      blockHead.nextBlockNo = newBlockNo;
      status = blockHead.Write(fileId, arrayHead.lstLowBlockNo);
      if (status) goto SOMEERROR;
    }
    // Read new last block's head
    status = blockHead.Read(fileId, newBlockNo);
    if (status) goto SOMEERROR;
    // Update new block's head...
    blockHead.type = arrayHead.blockType;
    blockHead.level = 1;
    blockHead.prevBlockNo = arrayHead.lstLowBlockNo;
    blockHead.numEntries = 0;
    status = blockHead.Write(fileId, newBlockNo);
    if (status) goto SOMEERROR;
    // Update Workspace Header
    if (BEFE::IsNull(arrayHead.fstLowBlockNo))
      arrayHead.fstLowBlockNo = newBlockNo;
    arrayHead.lstLowBlockNo = newBlockNo;
    wsHeadIsDirty = true;
    // We'll use the new one now
    blockNo = newBlockNo;
  }
  
  // Return the entry number
  entryNo = blockHead.numEntries;
  
  // Handle errors
  while (false) {
    IDOVERFLOW: status = Error::LexIdOverflow; break;
    SOMEERROR:                                 break;
  }
  
  // If everything's okay, increment the high Blob Id...
  if (!status) {
    arrayHead.hiId++;
    arrayHead.numEntries++;
    wsHeadIsDirty = true;
    newId = arrayHead.hiId;
  }
    
  return status;
  
}

Status LexWorkspace::_IdEntryFind(LexWsArrayHead &arrayHead, Id findId, UInt &blockNo, UInt &entryNo) {

  Status          status;
  Boolean         found;
  UInt32          curBlockNo;
  UInt32          curEntryNo;
  LexWsBlockHead  blockHead;
  UInt32          offset;
  UInt32          ll;
  UInt32          ul;
  UInt32          mid;
  Byte            fakeId[sizeof(Id)];
  UInt32          intId;
  
  found = false;
  
  // Open first...
  status = _Open();
  if (status) goto SOMEERROR;

  // Start at first low block
  blockHead.nextBlockNo = arrayHead.fstLowBlockNo;

  // For each block...
  while (!BEFE::IsNull(blockHead.nextBlockNo)) {
    
    curBlockNo = blockHead.nextBlockNo;
    
    // Load the block header...
    status = blockHead.Read(fileId, curBlockNo);
    if (status) goto SOMEERROR;
    
    // Ignore if empty...
    if (blockHead.numEntries == 0) continue;
    
    // Get the last one...
    offset = blockHead.PersistSize() + arrayHead.entrySize*(blockHead.numEntries-1);
    status = TheBefe->TheCache.GetBlockContents(fileId, curBlockNo, offset, fakeId, sizeof(Id));
    if (status) goto SOMEERROR;
    intId  = ((Int32)fakeId[0]) << 24;
    intId += ((Int32)fakeId[1]) << 16;
    intId += ((Int32)fakeId[2]) << 8;
    intId += ((Int32)fakeId[3]);
    // Position to next if beyond that one...
    if (intId < (UInt32)findId) {
      // Read the block's header for next time around...
      status = blockHead.Read(fileId, curBlockNo);
      if (status) goto SOMEERROR;
      continue;
    }
    
    // Binary search for it...
    ll = 0;
    ul = blockHead.numEntries-1;
    while (ll <= ul) {
      mid = (ll+ul)>>1;
      curEntryNo = mid;
      // Get the last one...
      offset = blockHead.PersistSize() + arrayHead.entrySize*curEntryNo;
      status = TheBefe->TheCache.GetBlockContents(fileId, curBlockNo, offset, fakeId, sizeof(Id));
      if (status) goto SOMEERROR;
      intId  = ((Int32)fakeId[0]) << 24;
      intId += ((Int32)fakeId[1]) << 16;
      intId += ((Int32)fakeId[2]) << 8;
      intId += ((Int32)fakeId[3]);
      // Test it...
      if (intId == findId) {
        found = true;
        break;
      }
      else if (intId < findId)
        ll = mid+1;
      else
        ul = mid-1;
    }
    
    break;
    
  }
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  // Return info
  if (status || !found) {
    blockNo = UNaN;
    entryNo = UNaN;
  }
  else {
    blockNo = curBlockNo;
    entryNo = curEntryNo;
  }

  return status;  
  
}

} // ...Namespace BEFE