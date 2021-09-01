//----------------------------------------------------------------------
// File: SBuffer.cpp - Implementation of the SBuffer class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "SBuffer.h"
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Instance methods
//
// Functions: Length     - Get the number of Characters in the buffer
//            Size       - Get the number of Bytes in the buffer
//            GetChar    - Get the given character in the buffer
//            AppendChar - Append character to end of buffer
//            InsertChar - Insert a character in the buffer
//            RemoveChar - Remove a character from the buffer
//            
// Note: CountBytes is, sort of, the same thing as calling GetUsedBytes
//       but we're separating it because of potential bad UTF-8
//       sequences.  CountBytes tells you the number of "proper"
//       bytes in the buffer, ignoring the invalid UTF-8 ones.
//
//       So, if you want speed instead of "accuracy", use GetUsedBytes
//       instead.  ;-)
//

UInt SBuffer::Length() {

  Status status;
  UInt   numChars;
  UInt   numBytes;

  status = UTF8::Count(GetBuf(),GetUsedBytes(),numBytes,numChars);
  if (status)
    return 0;
  return numChars;

}

UInt SBuffer::Size() {

  Status status;
  UInt   numChars;
  UInt   numBytes;

  status = UTF8::Count(GetBuf(),GetUsedBytes(),numBytes,numChars);
  if (status)
    return 0;
  return numBytes;

}

Char SBuffer::GetChar(Int cIndex) {

  Char    theChar;
  Byte   *srcBuf;
  UInt    usedBytes;

  theChar = UNaN;

  usedBytes = GetUsedBytes();
  if (usedBytes == 0) goto NOTFOUND;
  srcBuf = GetBuf();

  // Ask UTF8 code to walk it
  theChar = UTF8::GetChar(srcBuf,usedBytes,cIndex);

  // Handle errors
  while (false) {
    NOTFOUND: break;
  }

  return theChar;

}

Status SBuffer::AppendChar(Char theChar) {

  Status status;
  Byte   utfBuf[UTF8::MaxLength];
  UInt   utfBufl;

  // Encode the character
  status = UTF8::EncodeChar(theChar, utfBuf, utfBufl);
  if (status) goto SOMEERROR;

  // Append the bytes
  status = AppendBytes(utfBuf, utfBufl);
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }

  return status;

}

Status SBuffer::InsertChar(Int cIndex, Char theChar) {

  Status status;
  Byte  *buf;
  UInt   bufSize;
  Int    foundPos;
  Char   origChar;
  Byte   utfbuf[UTF8::MaxLength];
  UInt   utfbufl;

  // Get buf and bufSize...
  buf = GetBuf();
  bufSize = GetUsedBytes();
  status = UTF8::GetCharPos(buf, bufSize, cIndex, 0, foundPos, origChar);  
  if (status) goto SOMEERROR;
  
  // Encode the character
  status = UTF8::EncodeChar(theChar, utfbuf, utfbufl);
  if (status) goto SOMEERROR;

  // Append the bytes
  status = AppendBytes(utfbuf,utfbufl);
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }

  return status;

}

Status SBuffer::RemoveChar(Int cIndex) {
  
  Status status;
  Byte  *buf;
  UInt   bufSize;
  Int    foundPos;
  Char   origChar;

  // Get buf and bufSize...
  buf = GetBuf();
  bufSize = GetUsedBytes();
  origChar = UTF8::BadChar;
  status   = UTF8::GetCharPos(buf, bufSize, cIndex, 0, foundPos, origChar);  
  if (status) goto SOMEERROR;

  // Remove the bytes
  status = RemoveBytes(foundPos, UTF8::GetByteSize(origChar));
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }

  return status;

}

} // ...Namespace BEFE
