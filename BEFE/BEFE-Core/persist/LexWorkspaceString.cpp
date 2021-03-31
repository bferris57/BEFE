//----------------------------------------------------------------------
// File: LexWorkspaceString.cpp - Implementation of the LexWorkspace File Methods
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

UInt LexWorkspace::StringCount() {
  
  Status status;
  UInt   count;
  
  count = UNaN;
  
  // Initialise strings first...
  status = _StringInit(false);
  if (status) goto SOMEERROR;
  
  // Got it
  count = wsHead.strings.numEntries;
  
  // Handle errors;
  while (false) {
    SOMEERROR: break;
  }
  
  return count;
  
}

Status LexWorkspace::StringNew(String const &str, Id &strId) {
  
  Status          status;
  UInt32          blockNo;          // LexWsString's block number
  UInt32          entryNo;          // LexWsString's block entry number
  LexWsString     string;           // This string's array header
  UInt            stringsBlobSize;  // Length of entire string blob
  Byte           *buf;              // String's buffer
  UInt            bufSize;          // String's length (in Bytes)
  
  // Initialise strings first...
  status = _StringInit(true);
  if (status) goto SOMEERROR;

  // Ask IdEntry to create the new string for us...
  status = _IdEntryNew(wsHead.strings, blockNo, entryNo, string.id);
  if (status) goto SOMEERROR;

  //
  // Now for the contents (if any)...
  //
  
  if (!str.IsNull()) {
    
    // Get current blob length
    stringsBlobSize = BlobSize(wsHead.stringsBlobId);
    if (BEFE::IsNull(stringsBlobSize))
      stringsBlobSize = 0;
    
    // Get buf and size to append to strings blob
    str._BufAndSize(buf, bufSize);
    if (BEFE::IsNull(buf) || BEFE::IsNull(bufSize)) goto INTERNAL;
    
    // If we have something to append...
    if (bufSize) {
      
      // Append it to the blob
      status = BlobAppend(wsHead.stringsBlobId, buf, bufSize);
      if (status) goto SOMEERROR;
      
      // Update the string header's offsets and write it
      string.blobSpan = USpan(stringsBlobSize, stringsBlobSize+bufSize);
      status = string.Write(fileId, blockNo, entryNo);
      if (status) goto SOMEERROR;
      
    }
    
  }
  
  // Handle errors
  while (false) {
    INTERNAL:   status = Error::Internal;          break;
    SOMEERROR:                                     break;
  }
  
  // Return the String Id...
  strId = string.id;
    
  return status;

}

Status LexWorkspace::StringDelete(Id strId) {
  return Error::NotImplemented;
}

Boolean LexWorkspace::StringExists(Id strId) {
  return false;
}

Boolean LexWorkspace::StringExists(String &that) {
  return false;
}

UInt LexWorkspace::StringLength(Id strId) {
  return 0;
}

UInt LexWorkspace::StringSize(Id strId) {
  return 0;
}

String LexWorkspace::StringGet(Id strId) {

  //Status status;
  String string;
  
  /*status =*/ StringGet(strId, string);
  
  return string.Consumable();
  
}

Status LexWorkspace::StringGet(Id strId, String &str) {
  
  Status       status;
  UInt         blockNo;
  UInt         entryNo;
  LexWsString  strHead;
  UInt         strSize;
  Bytes        bytes;
  Byte        *buf;
  UInt         bufLen;
  
  status = str.Reset();
  if (status) goto SOMEERROR;
  
  // Initialise first...
  status = _StringInit(false);
  if (status) goto SOMEERROR;
  
  // Get the string header
  status = _IdEntryFind(wsHead.strings, strId, blockNo, entryNo);
  if (status) goto SOMEERROR;
  status = strHead.Read(fileId, blockNo, entryNo);
  if (status) goto SOMEERROR;
  
  // If it's not got a Blob Span, it's null...
  if (BEFE::IsNull(strHead.blobSpan.idx1) || BEFE::IsNull(strHead.blobSpan.idx2))
    goto OK;
    
  // If it's zero long, make it empty...
  strSize = strHead.blobSpan.Length();
  if (strSize == 0) {
    status = str.SetEmpty();
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Build a Bytes instance of the right length
  status = bytes.SetLength(strSize);
  if (status) goto SOMEERROR;
  status = bytes.SetUsedBytes(strSize);
  if (status) goto SOMEERROR;
  bytes.BufAndSize(buf, bufLen);
  if (BEFE::IsNull(buf)) goto NULLPOINTER;
  if (bufLen != strSize) goto INTERNAL;
  
  // Read it from the blob...
  status = BlobGet(wsHead.stringsBlobId, strHead.blobSpan, buf);
  if (status) goto SOMEERROR;
  
  // Morph the bytes into a string
  str = bytes.MorphToString(true);
  
  // Handle errors
  while (false) {
    OK:          status = Error::None;                break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    INTERNAL:    status = Error::Internal;            break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

Status LexWorkspace::_StringInit(Boolean needBlob) {
  
  Status status;
  
  // Open workspace first
  status = _Open();
  if (status) goto SOMEERROR;

  // If we need the strings blob...
  if (needBlob) {

    // If already have it, nothing to do...
    if (!BEFE::IsNull(wsHead.stringsBlobId)) goto DONE;
    
    // Create the strings blob...
    status = BlobNew(wsHead.stringsBlobId);
    if (status) goto SOMEERROR;
    
  }

  // Handle errors
  while (false) {
    DONE:      break;
    SOMEERROR: break;
  }
  
  return status;
  
}

} // ...Namespace BEFE