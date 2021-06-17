//----------------------------------------------------------------------
// File: Crypt.CPP - Implementation of EnCRYPTion/DeCRYPTion General Functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEWrappers.h"
#include "BEFEMacros.h"
#include "Crypt.h"
#include "FileStream.h"
#include "PersistHead.h"
#include "CryptFileHead.h"
#include "Crc32.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Encrypt/Decrypt Files...
//

Status EncryptFile(String const &inFileName) {
  
  Status   status;
  CryptKey befeKey;

  status = GetCryptKey(befeKey);
  if (status) goto SOMEERROR;    
  status = EncryptFile(inFileName, befeKey);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status DecryptFile(String const &inFileName) {

  
  Status   status;
  CryptKey befeKey;

  status = GetCryptKey(befeKey);
  if (status) goto SOMEERROR;
  status = DecryptFile(inFileName, befeKey);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
    
  return status;

}

Status EncryptFile(String const &inFileName, String const &outFileName) {
  
  Status   status;
  CryptKey befeKey;

  status = GetCryptKey(befeKey);
  if (status) goto SOMEERROR;  
  
  status = EncryptFile(inFileName, outFileName, befeKey);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}


Status DecryptFile(String const &inFileName, String const &outFileName) {
  
  Status   status;
  CryptKey befeKey;

  status = GetCryptKey(befeKey);
  if (status) goto SOMEERROR;  
  
  status = DecryptFile(inFileName, outFileName, befeKey);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status EncryptFile(String const &pInFileName, CryptKey const &key) {

  Status      status;
  String      inFileName;
  String      outFileName;
  String      inFileOnly;
  File        file;
  
  if (!TheBefe) goto NOBEFE;
  if (!TheBefe->TheOS) goto  NOOS;
  if (!TheBefe->TheOS->IsFullPath(pInFileName)) goto NOTFULLPATH;
  if (!TheBefe->TheOS->Exists(pInFileName)) goto NOTEXIST;
  if (!TheBefe->TheOS->IsFile(pInFileName)) goto NOTFILE;
  inFileName = pInFileName.NotConsumable();
  status = file.SetName(pInFileName);
  if (status) goto SOMEERROR;

  outFileName = inFileName+".tmp";
  status = EncryptFile(inFileName, outFileName, key);
  if (status) goto SOMEERROR;
  
  status = file.SetName(pInFileName);
  if (status) goto SOMEERROR;
  status = file.Delete();
  if (status) goto SOMEERROR;
  file.SetName(outFileName);
  status = file.Rename(inFileName);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOBEFE:      status = Error::NoBefe;            break;
    NOOS:        status = Error::InternalNoTheOS;   break;
    NOTFULLPATH: status = Error::FileNotFullPath;   break;
    NOTEXIST:    status = Error::FileDoesNotExist;  break;
    NOTFILE:     status = Error::FilePathIsNotFile; break;
    SOMEERROR:                                      break;
  }
  
  return status;

}

Status DecryptFile(String const &pInFileName, CryptKey const &key) {


  Status      status;
  String      inFileName;
  String      outFileName;
  String      inFileOnly;
  File        file;
  
  if (!TheBefe) goto NOBEFE;
  if (!TheBefe->TheOS) goto  NOOS;
  if (!TheBefe->TheOS->IsFullPath(pInFileName)) goto NOTFULLPATH;
  if (!TheBefe->TheOS->Exists(pInFileName)) goto NOTEXIST;
  if (!TheBefe->TheOS->IsFile(pInFileName)) goto NOTFILE;
  inFileName = pInFileName.NotConsumable();
  status = file.SetName(pInFileName);
  if (status) goto SOMEERROR;

  outFileName = inFileName+".tmp";
  status = DecryptFile(inFileName, outFileName, key);
  if (status) goto SOMEERROR;
  
  status = file.SetName(pInFileName);
  if (status) goto SOMEERROR;
  status = file.Delete();
  if (status) goto SOMEERROR;
  file.SetName(outFileName);
  status = file.Rename(inFileName);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOBEFE:      status = Error::NoBefe;            break;
    NOOS:        status = Error::InternalNoTheOS;   break;
    NOTFULLPATH: status = Error::FileNotFullPath;   break;
    NOTEXIST:    status = Error::FileDoesNotExist;  break;
    NOTFILE:     status = Error::FilePathIsNotFile; break;
    SOMEERROR:                                      break;
  }
  
  return status;

}

Status EncryptFile(String const &pInFileName, String const &pOutFileName, CryptKey const &key) {
  
  Status         status;
  String         inFileName;
  Byte          *inFileNameBuf;
  UInt           inFileNameSize;
  String         outFileName;
  FileStream     inStream;
  FileStream     outStream;
  Buffer         buffer;
  Byte          *buf;
  UInt           bufSize;
  UInt           numToRead;
  UInt           numRead;
  UInt           numWritten;
  UInt           inSize;
  UInt           remSize;
  CryptFileHead  cHead;
  PersistHead    pHead;
  Time           startTime;
  Time           endTime;
  Crc32          crc;
  
  // Validate input/output files
  inFileName = pInFileName.NotConsumable();  
  status = ValidateExistingFileName(inFileName);
  if (status) goto SOMEERROR;

  inFileName._BufAndSize(inFileNameBuf, inFileNameSize);
  if (inFileNameBuf == 0) goto NULLPOINTER;

  outFileName = pOutFileName;  
  status = ValidateNewFileName(outFileName);
  if (status) goto SOMEERROR;
  
  // Setup the Buffer
  bufSize = gDefaultBufSize;
  if (IsNull(bufSize) || bufSize < 1024)
    bufSize = 1024;
  status = buffer.Allocate(bufSize, 0);
  if (status) goto SOMEERROR;
  status = buffer.SetUsedBytes(bufSize);
  if (status) goto SOMEERROR;
  buf = buffer.GetBuf();
  if (IsNull(buf)) goto NULLPOINTER;
  
  // Set up the CryptHead
  cHead.inFileName = inFileName;
  
  // Set up the input stream
  status = inStream.SetName(inFileName);
  if (status) goto SOMEERROR;
  status = inStream.SetAccess(File::ModeRead);
  if (status) goto SOMEERROR;

  // Make sure it's not a BEFE file...
  status = pHead.ReadFrom(inStream);
  if (!status) goto NOTELIGIBLE;
  status = inStream.SetPos(0);
  if (status) goto SOMEERROR;

  // Set up the output stream  
  status = outStream.SetName(outFileName);
  if (status) goto SOMEERROR;
  status = outStream.SetAccess(File::ModeRead | File::ModeWrite);
  if (status) goto SOMEERROR;
  inSize = inStream.Size();
  status = outStream.SetSize(inSize + cHead.PersistSize());
  if (status) goto SOMEERROR;
  
  // Write the header...
  cHead.startTime = startTime.Set();
  status = cHead.WriteTo(outStream);
  if (status) goto SOMEERROR;
  
  // Until we've done it...
  remSize = inSize;
  cHead.inCrc = cHead.outCrc = 0xBEFE5EED;
  
  while (remSize) {

    // Read the next block...
    numToRead = Min(remSize, bufSize);
    status = inStream.GetBytes(buf, numToRead, numRead);
    if (status) goto SOMEERROR;
    if (numRead != numToRead) goto READFAILED;
    
    // Calculate inCrc, encrypt, and Calculate outCrc
    crc.Reset(cHead.inCrc);
    cHead.inCrc = crc.Update(buf, numRead);
    status = CryptEncrypt_0((Byte *)&key.key_0, sizeof(key.key_0), buf, numRead);
    if (status) goto SOMEERROR;
    crc.Reset(cHead.outCrc);
    cHead.outCrc = crc.Update(buf, numRead);
    
    // Write the next block...
    status = outStream.PutBytes(buf, numRead, numWritten);
    if (status) goto SOMEERROR;
    if (numWritten != numRead) goto TRUNCATED;
    
    remSize -= numRead;
    
  }

  // Update and rewrite the header
  cHead.endTime = endTime.Set();
  cHead.pHead.SetTypeName("Enc0");
  status = outStream.SetPos(0);
  if (status) goto SOMEERROR;
  status = cHead.WriteTo(outStream);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTELIGIBLE: status = Error::CryptFileNotEligible; break;
    NULLPOINTER: status = Error::InternalNullPointer;  break;
    TRUNCATED:   status = Error::FileWriteFailed;      break;
    READFAILED:  status = Error::FileReadFailed;       break;
    SOMEERROR:                                         break;
  }
  
  // Remove the output file if we have an error
  if (status) {
    outStream.Reset();
    if (Exists(outFileName))
      File(outFileName).Delete();
  }

  return status;
  
}

Status DecryptFile(String const &pInFileName, String const &pOutFileName, CryptKey const &key) {

  Status         status;
  String         inFileName;
  Byte          *inFileNameBuf;
  UInt           inFileNameSize;
  String         outFileName;
  FileStream     inStream;
  FileStream     outStream;
  Buffer         buffer;
  Byte          *buf;
  UInt           bufSize;
  UInt           numToRead;
  UInt           numRead;
  UInt           numWritten;
  UInt           inSize;
  UInt           outSize;
  UInt           remSize;
  CryptFileHead  cHead;
  Time           startTime;
  Time           endTime;
  Crc32          crc;
  UInt           inCrc;
  UInt           outCrc;
  
  // Validate input/output files
  inFileName = pInFileName.NotConsumable();  
  status = ValidateExistingFileName(inFileName);
  if (status) goto SOMEERROR;

  inFileName._BufAndSize(inFileNameBuf, inFileNameSize);
  if (inFileNameBuf == 0) goto NULLPOINTER;

  outFileName = pOutFileName;  
  status = ValidateNewFileName(outFileName);
  if (status) goto SOMEERROR;
  
  // Setup the Buffer
  bufSize = gDefaultBufSize;
  if (IsNull(bufSize) || bufSize < 1024)
    bufSize = 1024;
  status = buffer.Allocate(bufSize, 0);
  if (status) goto SOMEERROR;
  status = buffer.SetUsedBytes(bufSize);
  if (status) goto SOMEERROR;
  buf = buffer.GetBuf();
  if (IsNull(buf)) goto NULLPOINTER;
  
  // Set up the streams
  status = inStream.SetName(inFileName);
  if (status) goto SOMEERROR;
  status = inStream.SetAccess(File::ModeRead);
  if (status) goto SOMEERROR;
  status = outStream.SetName(outFileName);
  if (status) goto SOMEERROR;
  status = outStream.SetAccess(File::ModeRead | File::ModeWrite);
  if (status) goto SOMEERROR;
  inSize = inStream.Size();
  
  // Read and Validate the header...
  status = cHead.ReadFrom(inStream);
  if (status == Error::PersistNotBEFE) goto NOTELIGIBLE;
  if (status) goto SOMEERROR;
  if (!cHead.IsValid()) goto NOTENCFILE;
  if (cHead.pHead.GetTypeName() != "Enc0") goto NOTELIGIBLE;
  
  // Set output file size...
  outSize = inSize - cHead.PersistSize();
  status = outStream.SetSize(outSize);
  if (status) goto SOMEERROR;
  
  // Until we've done it...
  remSize = outSize;
  inCrc = outCrc = 0xBEFE5EED;
  
  while (remSize) {

    // Read the next block...
    numToRead = Min(remSize, bufSize);
    status = inStream.GetBytes(buf, numToRead, numRead);
    if (status) goto SOMEERROR;
    if (numRead != numToRead) goto READFAILED;
    
    // Calculate inCrc, encrypt, and Calculate outCrc
    crc.Reset(inCrc);
    inCrc = crc.Update(buf, numRead);
    status = CryptDecrypt_0((Byte *)&key.key_0, sizeof(key.key_0), buf, numRead);
    if (status) goto SOMEERROR;
    crc.Reset(outCrc);
    outCrc = crc.Update(buf, numRead);
    
    // Write the next block...
    status = outStream.PutBytes(buf, numRead, numWritten);
    if (status) goto SOMEERROR;
    if (numWritten != numRead) goto TRUNCATED;
    
    remSize -= numRead;
    
  }

  // Make sure CRCs match
  if (cHead.inCrc != outCrc || cHead.outCrc != inCrc) goto WRONGKEY;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;   break;
    NOTENCFILE:  status = Error::CryptFileNotEncrypted; break;
    NOTELIGIBLE: status = Error::CryptFileNotEligible;  break;
    WRONGKEY:    status = Error::CryptFileWrongKey;     break;
    TRUNCATED:   status = Error::FileWriteFailed;       break;
    READFAILED:  status = Error::FileReadFailed;        break;
    SOMEERROR:                                          break;
  }
  
  // Remove the output file if we have an error
  /*
  if (status) {
    outStream.Reset();
    if (Exists(outFileName))
      File(outFileName).Delete();
  }
  */
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Crypt Key Files...
//

Status CreateKeyFile() {

  Status   status;
  String   keyFileName;
  CryptKey key;
  
  if (!TheBefe) goto NOBEFE;
  if (!TheBefe->TheOS) goto NOOS;
  
  keyFileName = GetEnvironmentValue("BEFE_KeyFile");
  if (keyFileName.Length() == 0) goto NOTSET;
  if (Exists(keyFileName)) goto EXISTS;
  status = key.NewRandom();
  if (status) goto SOMEERROR;
  status = CreateKeyFile(keyFileName, key);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOBEFE:    status = Error::NoBefe;                break;
    NOOS:      status = Error::InternalNoTheOS;       break;
    NOTSET:    status = Error::CryptKeyNoBEFEKeyFile; break;
    EXISTS:    status = Error::CryptKeyKeyFileExists; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

Status CreateKeyFile(String const &keyFileName) {
  
  Status   status;
  CryptKey key;

  status = key.NewRandom();
  if (status) goto SOMEERROR;
  status = CreateKeyFile(keyFileName, key);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

Status CreateKeyFile(String const &keyFileName, CryptKey const &key) {

  Status      status;
  String      fullFilePath;
  CryptKey    localKey;
  PersistHead pHead;
  FileStream  file;
  Crc32       crc;
  UInt        crcValue;
  UInt        numWritten;
  
  if (!TheBefe) goto NOBEFE;
  if (!TheBefe->TheOS) goto NOOS;
  
  fullFilePath = keyFileName;
  status = ValidateNewFileName(fullFilePath);
  if (status) goto SOMEERROR;
  
  // Initialise header...
  status = pHead.SetTypeName("Key0");
  if (status) goto SOMEERROR;
  
  // Calculate CRC..
  crc.Reset(0xBEFE5EED);
  crcValue = IntReverseBytes( crc.Update((Byte *)&key, sizeof(CryptKey)) );
  
  // Write it...
  status = file.SetName(fullFilePath);
  if (status) goto SOMEERROR;
  status = file.SetAccess(File::ModeRead|File::ModeWrite);
  if (status) goto SOMEERROR;
  status = pHead.WriteTo(file);
  if (status) goto SOMEERROR;
  status = file.PutBytes((Byte *)&crcValue, sizeof(UInt), numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != sizeof(UInt)) goto TRUNCATED;
  status = key.WriteTo(file);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOBEFE:    status = Error::NoBefe;          break;
    NOOS:      status = Error::InternalNoTheOS; break;
    TRUNCATED: status = Error::FileWriteFailed; break;
    SOMEERROR:                                  break;
  }
  
  return status;
  
}

Status ValidateKeyFile() {

  Status status;
  String befeKeyFile;

  befeKeyFile = GetEnvironmentValue("BEFE_KeyFile");
  if (befeKeyFile.Length() == 0) goto NOKEYFILE;
  status = ValidateKeyFile(befeKeyFile);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOKEYFILE: status = Error::CryptKeyNoBEFEKeyFile;   break;
    SOMEERROR:                                          break;
  }
  
  return status;
  
}

Status ValidateKeyFile(String const &keyFileName) {

  Status   status;
  CryptKey key;

  if (keyFileName.Length() == 0) goto NOKEYFILE;
  if (!Exists(keyFileName)) goto NOTEXIST;
  if (!IsFile(keyFileName)) goto NOTFILE;
  status = ReadKeyFile(keyFileName, key);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOKEYFILE: status = Error::CryptKeyKeyFileNotSpecified; break;
    NOTEXIST:  status = Error::CryptKeyKeyFileNotExist;     break;
    NOTFILE:   status = Error::CryptKeyKeyFileNotFile;      break;
    SOMEERROR:                                              break;
  }
  
  return status;
  
}

Boolean IsValidKeyFile() {
  return ValidateKeyFile() == Error::None;
}

Boolean IsValidKeyFile(String const &keyFileName) {
  return ValidateKeyFile(keyFileName) == Error::None;
}

Status ReadKeyFile() {

  Status   status;
  String   befeKeyFile;
  CryptKey befeCryptKey;
  
  befeKeyFile = GetEnvironmentValue("BEFE_KeyFile");
  if (befeKeyFile.Length() == 0) goto NOKEYFILE;
  status = ReadKeyFile(befeKeyFile, befeCryptKey);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOKEYFILE: status = Error::CryptKeyNoBEFEKeyFile;   break;
    SOMEERROR:                                          break;
  }
  
  return status;
  
}

CryptKey ReadKeyFile(String const &keyFileName) {
  
  Status   status;
  CryptKey localKey;
  
  status = ReadKeyFile(keyFileName, localKey);
  if (status != Error::None)
    SetNull(localKey);
  
  return localKey.Consumable();

}

Status ReadKeyFile(String const &keyFileName, CryptKey &key) {

  Status      status;
  String      fullFilePath;
  CryptKey    localKey;
  FileStream  file; 
  PersistHead pHead;
  Crc32       crc;
  UInt        crcValue;
  UInt        numRead;
  
  SetNull(key);
  
  fullFilePath = keyFileName;
  status = ValidateExistingFileName(fullFilePath);
  if (status) goto SOMEERROR;
  
  // Read and Validate Header...
  status = file.SetName(fullFilePath);
  if (status) goto SOMEERROR;
  status = file.SetAccess(File::ModeRead);
  if (status) goto SOMEERROR;
  status = pHead.ReadFrom(file);
  if (status) goto SOMEERROR;
  if (pHead.GetTypeName().Get(Span(0,3)) != "Key") goto NOTKEYFILE;
  if (pHead.GetTypeName().Get(3) != '0') goto UNKTYPE;
  if (pHead.GetTypeRevision() != 0) goto UNKREV;
  
  // Read the Crc
  status = file.GetBytes((Byte *)&crcValue, sizeof(UInt), numRead);
  if (status) goto SOMEERROR;
  if (numRead != sizeof(UInt)) goto CORRUPT;
  crcValue = IntReverseBytes(crcValue);
  
  // Read and validate the key
  status = localKey.ReadFrom(file);
  if (status) goto SOMEERROR;
  crc.Reset(0xBEFE5EED);
  if (crcValue != (UInt)crc.Update((Byte *)&localKey, sizeof(CryptKey))) goto CORRUPT;
  
  // Everything's okay, give it to the caller
  status = key.MoveFrom(localKey);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTKEYFILE: status = Error::CryptKeyNotKeyFile;     break;
    UNKTYPE:    status = Error::CryptKeyUnknownKeyType; break;
    UNKREV:     status = Error::CryptKeyUnknownKeyRev;  break;
    CORRUPT:    status = Error::CryptKeyKeyFileCorrupt; break;
    SOMEERROR:                                          break;
  }
  
  return status;
  
}

} // ...Namespace BEFE