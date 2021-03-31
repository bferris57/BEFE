//----------------------------------------------------------------------
// File: LexWorkspaceFile.cpp - Implementation of the LexWorkspace File Methods
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
#include "Lex.h"
#include "LexWorkspace.h"
#include "LexWs.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Method: LexWorkspace._FileNewFileBlob - Create a new File blob
//
// This method is entirely internal and shouldn't be used outside of
// LexWorkspace code.
//
// Note: This method assumes the file's already being cached
//

UInt LexWorkspace::FileCount() {
  
  Status status;
  UInt   answer;
  
  status = _Open();
  if (!status)
    answer = wsHead.files.numEntries;
  else
    answer = UNaN;

  return answer;
  
}

Status LexWorkspace::FileNew(String const &fileName, Id &newFileId) {

  Status    retStatus;  
  Status    status;
  UInt      blockNo;
  UInt      entryNo;
  LexWsFile fileHead;
  Id        inputFileId;

  BEFE::SetNull(inputFileId);
  
  // Open the Workspace first
  status = _Open();
  if (status) goto SOMEERROR;
  
  // Open the input file first...
  status = TheBefe->TheCache.StartCacheFile(fileName);
  if (status) goto SOMEERROR;
  inputFileId = TheBefe->TheCache.GetFileId(fileName);
  if (BEFE::IsNull(inputFileId)) goto SOMEERROR;
  
  // Create a new LexWsFile
  status = _IdEntryNew(wsHead.files, blockNo, entryNo, fileHead.id);
  if (status) goto SOMEERROR;

  // Create a new string Id for the file name
  status = StringNew(fileName, fileHead.name);
  if (status) goto SOMEERROR;

  // Create a new blob Id for the file contents
  status = _FileNewFileBlob(inputFileId, fileHead.text, fileHead.crc);
  if (status) goto SOMEERROR;

  // And snap the time
  fileHead.snapTime.Set();

  // Finally, write the header back out
  status = fileHead.Write(fileId, blockNo, entryNo);
  if (status) goto SOMEERROR;
  
  // Return the Id to the caller
  newFileId = fileHead.id;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  retStatus = status;
  
  // Stop caching the input file regardless of success or failure
  if (!BEFE::IsNull(inputFileId)) {
    status = TheBefe->TheCache.StopCacheFile(fileName);
    if (status) {
      if (retStatus)
        BEFE_ERROR("LexWorkspace.FileNew (Internal)",status);
      else
        retStatus = status;
    }
  }

  // If an error occurred, try cleaning up...
  if (retStatus) {
    // !!! NEED CODE HERE to clean up the Ids, etc. we created !!!
  }
  
  return retStatus;
  
}

UInt LexWorkspace::_FileNewFileBlob(Id cacheFileId, Id &blobId, Crc32 &crc) {
  
  Status status;
  UInt   numInBytes;
  UInt   numInBlocks;
  UInt   blockNo;
  UInt   blockBytes;
  Byte  *blockData;
  
  BEFE::SetNull(blobId);
  crc.Reset();
  
  // Open Workspace first...
  status = _Open();
  if (status) goto SOMEERROR;

  // Sanity checks...
  if (BEFE::IsNull(cacheFileId)) goto BADPARAM;
  if (BEFE::IsNull(blkSize)) goto INTERNAL;

  // Get file Size...
  numInBytes = TheBefe->TheCache.GetFileSize(cacheFileId);
  if (BEFE::IsNull(numInBytes)) goto INTERNAL;
  numInBlocks = (numInBytes + blkSize - 1)/blkSize;

  // Create a new blob
  status = BlobNew(blobId);
  if (status) goto SOMEERROR;
  
  // Read and Append the file data...
  for (blockNo=0; blockNo < numInBlocks; blockNo++) {

    // Load the block...
    status = TheBefe->TheCache.LoadFileBlock(cacheFileId, blockNo);
    if (status) goto SOMEERROR;

    // Get block len and pointer...
    blockBytes = TheBefe->TheCache.GetBlockUsedBytes(cacheFileId, blockNo);
    blockData  = TheBefe->TheCache._GetBlockPointer(cacheFileId, blockNo);
    if (BEFE::IsNull(blockBytes) || blockBytes > blkSize) goto INTERNAL;
    if (BEFE::IsNull(blockData)) goto INTERNAL;

    // Append it to the blob...
    status = BlobAppend(blobId, blockData, blockBytes);
    if (status) goto SOMEERROR;

    // And udpate the CRC
    crc.Update(blockData, blockBytes);
    
  }
  
  // Handle errors
  while (false) {
    INTERNAL:  status = Error::Internal;         break;
    BADPARAM:  status = Error::InvalidParameter; break;
    SOMEERROR:                                   break;
  }
  
  return status;
  
}

} // ...Namespace BEFE