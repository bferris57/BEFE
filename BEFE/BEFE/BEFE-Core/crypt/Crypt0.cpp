//----------------------------------------------------------------------
// File: Crypt0.cpp - Implementation of EnCRYPTion/DeCRYPTion Algorithm 0
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEWrappers.h"
#include "Crypt.h"
#include "FileStream.h"
#include "PersistHead.h"
#include "UUID.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Private: _CryptHead_0 Struct
//
// This Struct is used as the header for each Crypt0 encrypted file
//

struct _CryptHead_0 {
  PersistHead pHead;
  UInt        origCRC;
  UInt        origLen;
};

//----------------------------------------------------------------------
//
// Basic Algorithm...
//

Status CryptEncrypt_0(Byte *key, UInt keyLen, Byte *buf, UInt bufLen) {

  Status  status;
  Byte   *kBuf;
  UInt    kRem;
  
  if (IsNull(key) || keyLen == 0) goto EMPTYKEY;
  if (IsNull(buf) || bufLen == 0) goto OK;
  kRem = 0;
  while (bufLen) {
    if (!kRem) {
      kRem = keyLen;
      kBuf = key;
    }
    *buf = *buf ^ *kBuf++;
	buf++;
    kRem--;
    bufLen--;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    EMPTYKEY: status = Error::CryptKeyEmptyKey; break;
    OK:       status = Error::None;             break;
  }
  
  return status;
  
}

Status CryptDecrypt_0(Byte *key, UInt keyLen, Byte *buf, UInt bufLen) {
  return CryptEncrypt_0(key, keyLen, buf, bufLen);
}

//----------------------------------------------------------------------
//
// Algorithm Variants...
//

Status CryptEncrypt_0(UUID const &key, String &theBytes) {
  
  Status  status;
  Byte   *buf;
  UInt    size;
  
  if (!key.IsValid()) goto INVALIDUUID;
  if (!theBytes.IsMutable()) goto NOTMUTABLE;
  theBytes._BufAndSize(buf, size);
  status = CryptEncrypt_0((Byte *)&key, sizeof(UUID), buf, size);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    INVALIDUUID: status = Error::InvalidUUID;      break;
    NOTMUTABLE:  status = Error::StringNotMutable; break;
    SOMEERROR:                                     break;
  }
  
  return status;
  
}

Status CryptDecrypt_0(UUID const &key, String &theBytes) {
  return CryptEncrypt_0(key, theBytes);
}

Status CryptEncrypt_0(UUID const &key, Bytes &theBytes) {
  
  Status  status;
  Byte   *buf;
  UInt    size;

  if (!key.IsValid()) goto INVALIDUUID;
  theBytes.BufAndSize(buf, size);
  status = CryptEncrypt_0((Byte *)&key, sizeof(UUID), buf, size);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    INVALIDUUID: status = Error::InvalidUUID;      break;
    SOMEERROR:                                     break;
  }
  
  return status;
  
}

Status CryptDecrypt_0(UUID const &key, Bytes &theBytes) {
  return CryptEncrypt_0(key, theBytes);
}

Status CryptEncrypt_0(UUID const &key, Buffer &theBytes) {
  
  Status  status;
  Byte   *buf;
  UInt    size;

  if (!key.IsValid()) goto INVALIDUUID;
  buf  = theBytes.GetBuf();
  size = theBytes.GetUsedBytes();
  status = CryptEncrypt_0((Byte *)&key, sizeof(UUID), buf, size);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    INVALIDUUID: status = Error::InvalidUUID;      break;
    SOMEERROR:                                     break;
  }
  
  return status;
  
}

Status CryptDecrypt_0(UUID const &key, Buffer &theBytes) {
  return CryptEncrypt_0(key, theBytes);
}

//----------------------------------------------------------------------
//
// Key Management...
//

Status CryptWriteKeyFile_0(String const &fileName, UUID &key) {
  
    Status      status;
    String      pathOnly;
    String      fullPath;
    PersistHead pHead;
    FileStream  stream;
    
    // Make sure key is valid...
    if (!key.IsValid()) goto KEYINVALID;

    // Make sure fileName is valid...
    fullPath = fileName;
    status = ValidateNewFileName(fullPath);
    if (status) goto SOMEERROR;

    // Write it
    status = stream.SetName(fullPath);
    if (status) goto SOMEERROR;
    status = stream.SetAccess(File::ModeRead|File::ModeWrite);
    if (status) goto SOMEERROR;
    status = pHead.SetTypeName("Key0");
    if (status) goto SOMEERROR;
    status = pHead.WriteTo(stream);
    if (status) goto SOMEERROR;
    status = key.WriteTo(stream);
    if (status) goto SOMEERROR;
    status = stream.Flush();
    if (status) goto SOMEERROR;
    status = stream.Reset();
    if (status) goto SOMEERROR;
    
    // Handle errors
    while (false) {
      KEYINVALID: status = Error::InvalidUUID; break;
      SOMEERROR:                               break;
    }
    
    return status;

}

Status CryptReadKeyFile_0(String const &fileName, UUID &key) {
  
    Status      status;
    String      pathOnly;
    String      fullPath;
    PersistHead pHead;
    FileStream  stream;
    
    // Make sure fileName is valid...
    fullPath = fileName;
    status = ValidateExistingFileName(fullPath);
    if (status) goto SOMEERROR;

    // Write it
    status = stream.SetName(fullPath);
    if (status) goto SOMEERROR;
    status = stream.SetAccess(File::ModeRead);
    if (status) goto SOMEERROR;
    status = pHead.ReadFrom(stream);
    if (status) goto SOMEERROR;
    if (pHead.GetTypeName() != "Key0") goto INVALIDKEYFILE;
    status = key.ReadFrom(stream);
    if (status) goto SOMEERROR;
    status = stream.Flush();
    if (status) goto SOMEERROR;
    status = stream.Reset();
    if (status) goto SOMEERROR;
    
    // Handle errors
    while (false) {
      INVALIDKEYFILE: status = Error::InvalidUUID; break;
      SOMEERROR:                                   break;
    }
    
    return status;

}

} // ...Namespace BEFE
