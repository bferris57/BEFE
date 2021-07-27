//----------------------------------------------------------------------
// File: StringInsert.cpp - String Class Insert methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "UTF8.h"
#include "SBuffer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Char Insert...
//

Status String::Insert(Int index, Char c) {

  Status   status;
  Byte    *buf;
  UInt     bufSize;
  UInt     bufLen;
  Byte     charBuf[4];
  UInt     charLen;
  Int      foundPos;
  Char     foundChar;
  Boolean  morphIt;
  
  if (!isMutable) goto NOTMUTABLE;
  
  _BufByteAndCharCount(buf, bufSize, bufLen);
  
  // Adjust if negative
  if (index < 0)
    index = bufLen + index;
  if (index < 0 || (UInt)index > bufLen) goto OUTOFRANGE;
  
  // Put at end if beyond end
  if (index > (Int)bufLen)
    index = (Int)bufLen;
    
  // Find index if it has UTF-8 in it...
  if (isChar) {
    if (index == (Int)bufLen)
      index = bufSize;
    else {
      status = UTF8::GetCharPos(buf, bufSize, index, 0, foundPos, foundChar);
      if (status) goto SOMEERROR;
      index = foundPos;
    }
  }
  
  // Append if at end...
  if ((UInt)index == bufSize) {
    status = Append(c);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Get the encoded character
  status = UTF8::EncodeChar(c, charBuf, charLen);
  if (status) goto SOMEERROR;
  if (charLen == 0) goto BADUNICODE;
  
  // Based on type...
  switch (type) {
    case Null:    
      MemoryCopyRaw(charBuf, (Byte *)&data, charLen);
      if (charLen == 1)
        type = ASCII1;
      else
        type = UtfInline;
      constLen = charLen;
      goto OK;
    case Const:
      goto NOTMUTABLE;
    case ConstLen:
      goto NOTMUTABLE;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
      morphIt = (charLen != 1);
      break;
    case ASCII4:
      morphIt = true;
      break;
    case UtfInline:
      if (constLen != UTF8::GetCharSize(*(Byte *)&data)) goto INTERNALERROR;
      morphIt = true;
      break;
    case Buffer:
      morphIt = false;
      break;
    default:
      goto INTERNALERROR;
  }

  // Morph it if we're supposed to...
  if (morphIt) {
    status = _MorphTo(Buffer);
    if (status) goto SOMEERROR;
  }

  // Append first...
  status = Append(c);
  if (status) goto SOMEERROR;
  _BufByteAndCharCount(buf, bufSize, bufLen);

  // Move bytes aside
  MemoryCopyRaw(buf+index, buf+index+charLen, bufSize-charLen-index);
  
  // Place it in the right place
  MemoryCopyRaw(charBuf, buf+index, charLen);
  
  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFRANGE:    status = Error::IndexOutOfRange;     break;
    INTERNALERROR: status = Error::Internal;            break;
    NOTMUTABLE:    status = Error::StringNotMutable;    break;
    BADUNICODE:    status = Error::BadUnicodeCodePoint; break;
    SOMEERROR:                                          break;
    OK:            status = Error::None;                break;
  }

  return status;

}

Status String::Insert(Int index, char c) {
  return Insert(index, (Char)c);
}

//----------------------------------------------------------------------
//
// String Insert...
//

Status String::Insert(Int index, String const &that) {

  Status   status;
  Byte    *buf;
  UInt     bufSize;
  UInt     bufLen;
  Byte    *thatBuf;
  UInt     thatSize;
  UInt     thatLen;
  Int      foundPos;
  Char     foundChar;
  Boolean  morphIt;
  
  if (!isMutable) goto NOTMUTABLE;
  
  _BufByteAndCharCount(buf, bufSize, bufLen);
  ((String *)&that)->_BufByteAndCharCount(thatBuf, thatSize, thatLen);

  // Adjust if negative
  if (index < 0)
    index = bufLen + index;
  if (index < 0 || (UInt)index > bufLen) goto OUTOFRANGE;
  
  // Put at end if beyond end
  if (index > (Int)bufLen)
    index = (Int)bufLen;
    
  // Find index if it has UTF-8 in it...
  if (isChar) {
    if (index == (Int)bufLen)
      index = bufSize;
    else {
      status = UTF8::GetCharPos(buf, bufSize, index, 0, foundPos, foundChar);
      if (status) goto SOMEERROR;
      index = foundPos;
    }
  }
  
  // Append if at end...
  if ((UInt)index == bufSize) {
    status = Append(that);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Based on type...
  switch (type) {
    case Null:
      status = CopyFrom(that);
      if (status) goto SOMEERROR;
      goto OK;
    case Const:
      goto NOTMUTABLE;
    case ConstLen:
      goto NOTMUTABLE;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
      morphIt = (thatLen != 1);
      break;
    case ASCII4:
      morphIt = true;
      break;
    case UtfInline:
      if (constLen != UTF8::GetCharSize(*(Byte *)&data)) goto INTERNALERROR;
      morphIt = true;
      break;
    case Buffer:
      morphIt = false;
      break;
    default:
      goto INTERNALERROR;
  }

  // Morph it if we're supposed to...
  if (morphIt) {
    status = _MorphTo(Buffer);
    if (status) goto SOMEERROR;
  }
  
  // Append first...
  status = Append(that);
  if (status) goto SOMEERROR;
  _BufByteAndCharCount(buf, bufSize, bufLen);
  
  // Move bytes aside
  MemoryCopyRaw(buf+index, buf+index+thatSize, bufSize-thatSize-index);
  
  // Place it in the right place
  MemoryCopyRaw(thatBuf, buf+index, thatSize);
  
  
  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFRANGE:    status = Error::IndexOutOfRange;     break;
    INTERNALERROR: status = Error::Internal;            break;
    NOTMUTABLE:    status = Error::StringNotMutable;    break;
    SOMEERROR:                                          break;
    OK:            status = Error::None;                break;
  }

  return status;

}

} // ...Namespace BEFE
