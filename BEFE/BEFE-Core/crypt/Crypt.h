//!befe-publish inc
//----------------------------------------------------------------------
// File: Crypt.h - Declarations for BEFE EnCRYPTion/DeCRYPTion
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CRYPT_H // CRYPT_H...
#define CRYPT_H

#include "Primitives.h"
#include "CryptKey.h"
#include "UUID.h"

namespace BEFE { // Namespace BEFE...

//
// CryptKey Struct
//

//
// General Algorithms
//

// File Encrypt/Decript...
Status   EncryptFile(String const &inFileName);
Status   DecryptFile(String const &inFileName);
Status   EncryptFile(String const &inFileName, String const &outFileName);
Status   DecryptFile(String const &inFileName, String const &outFileName);
Status   EncryptFile(String const &inFileName, CryptKey const &key);
Status   DecryptFile(String const &inFileName, CryptKey const &key);
Status   EncryptFile(String const &inFileName, String const &outFileName, CryptKey const &key);
Status   DecryptFile(String const &inFileName, String const &outFileName, CryptKey const &key);

// Crypt Key Files...
Status   CreateKeyFile();
Status   CreateKeyFile(String const &keyFileName);
Status   CreateKeyFile(String const &keyFileName, CryptKey const &key);
Status   ValidateKeyFile();
Status   ValidateKeyFile(String const &keyFileName);
Boolean  IsValidKeyFile();
Boolean  IsValidKeyFile(String const &keyFileName);
Status   ReadKeyFile();
CryptKey ReadKeyFile(String const &keyFileName);
Status   ReadKeyFile(String const &keyFileName, CryptKey &key);

//
// Encryption Algorithm 0
//

// Basic Algorithm...
Status CryptEncrypt_0(Byte *key, UInt keyLen, Byte *buf, UInt bufLen);
Status CryptDecrypt_0(Byte *key, UInt keyLen, Byte *buf, UInt bufLen);

// Algorithm Variants...
Status CryptEncrypt_0(UUID const &key, String &theBytes);
Status CryptDecrypt_0(UUID const &key, String &theBytes);
Status CryptEncrypt_0(UUID const &key, Bytes  &theBytes);
Status CryptDecrypt_0(UUID const &key, Bytes  &theBytes);
Status CryptEncrypt_0(UUID const &key, Buffer &theBytes);
Status CryptDecrypt_0(UUID const &key, Buffer &theBytes);

// Key Management...
Status CryptWriteKeyFile_0(String const &fileName, UUID &key);
Status CryptReadKeyFile_0 (String const &fileName, UUID &key);

} // ...Namespace BEFE

#endif // ...CRYPT_H