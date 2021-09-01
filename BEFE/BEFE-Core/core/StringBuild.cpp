//----------------------------------------------------------------------
// File: StringBuild.cpp - String Class Build methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "UTF8.h"
#include "SBuffer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Internal methods
//
// Methods: _BuildFrom - Clear and initialise from various sources
//
// These BuildFrom... functions assume StartUp() has already been called.
// This is because these functions may come in handy in other cases
// like operator= when it already contains data.
//

Status String::_BuildFrom(Byte *buf, UInt size) {

  Status   status;
  UInt     byteCount;
  UInt     charCount;
  Char     theChar;
  Byte    *dstBuf;
  SBuffer *sBuf;

  // Nothing to do if no buffer or len
  if (BEFE::IsNull(buf) || size == 0) goto OK;

  // Get info about it's UTF-8 content
  status = UTF8::Count(buf,size,byteCount,charCount);
  if (status) goto SOMEERROR;

  // If there's nothing there, there's nothing to do
  if (byteCount == 0 || charCount == 0) goto OK;

  // If there's only one char use type UTF8...
  if (charCount == 1) {
    theChar = UTF8::GetChar(buf,size,0);
    if ( BEFE::IsNull(theChar) ) goto BADCODEPOINT;
    // If the character is <= 0x7f, there must have been bad sequences in there,
    // so use ASCII0 instead;
    if (theChar <= (UInt)0x7f) {
      type = ASCII1;
      *(Byte *)&data = theChar;
      isByte = true;
      isChar = false;
      constLen = 1;
    }
    // Nope, it's proper Unicode...
    else {
      type     = UtfInline;
      isByte   = false;
      isChar   = true;
      constLen = byteCount;
      data = 0x00;
      dstBuf = (Byte *)&data;
      while (size) {*dstBuf++ = *buf++; size--;}
    }
    goto OK;
  }

  // There's more than one character out there.  if theres < 5 characters and
  // they're all ascii, use one of the ASCII types
  if (charCount < 5 && byteCount == charCount) {
    dstBuf = (Byte *)&data;
    type = ASCII0 + charCount;
    while (charCount != 0) {
      *dstBuf++ = *buf++;
      charCount--;
    };
    isByte = true;
    isChar = false;
    constLen = byteCount;
    goto OK;
  }

  // There's more than we can fit without allocating memory, create a SBuffer...
  sBuf = (SBuffer *)&data;
  sBuf->StartUp();
  status = sBuf->Allocate(size,UNaN);
  if (status) goto SOMEERROR;

  // Save it and append the data
  type = Buffer;
  isByte = (byteCount == charCount);
  isChar = !isByte;
  status = sBuf->AppendBytes(buf,size);
  if (status) goto SOMEERROR;

  // Errors none
  status = Error::None;
  while (false) {
    OK:           status = Error::None;                break;
    BADCODEPOINT: status = Error::BadUnicodeCodePoint; break;
    SOMEERROR:                                         break;
  }

  return status;

}

Status String::_BuildFrom(String const &that) {

  Status   status;
  SBuffer *sbuf;

  // If it's consumable, consume it...
  if (that.IsConsumable()) {
    status = MoveFrom(that);
    if (status) goto SOMEERROR;
    goto FINISHED;
  }
  
  // Need to copy it...  
  switch (that.type) {
    case Null:
      break;
    case Const:
    case ConstLen:
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
    case UtfInline:
      type = that.type;
      data = that.data;
      isMutable   = (type != Const && type != ConstLen);
      isByte      = that.isByte;
      isChar      = that.isChar;
      isSensitive = that.isSensitive;
      mult        = that.mult;
      constLen    = that.constLen;
      break;
    case Buffer:
      type = Buffer;
      sbuf = (SBuffer *)&data;
      status = sbuf->CopyFrom(*(BEFE::Buffer *)&that.data);
      if (status) goto SOMEERROR;
      isMutable   = true;
      isByte      = that.isByte;
      isChar      = that.isChar;
      isSensitive = that.isSensitive;
      mult        = that.mult;
      break;
    default:
      goto INTERNALERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    INTERNALERROR: status = Error::Internal;
                   BEFE_WARN("String.BuildFrom: INTERNAL ERROR");
                   break;
    FINISHED:      break;
    SOMEERROR:     break;
  }

  return status;

}

Status String::_BuildFrom(char const *that) {

  Status  status;
  Byte   *srcBuf;
  UInt    srcSize;
  UInt    byteCount;
  UInt    charCount;

  if (BEFE::IsNull(that)) goto OK;

  // Get the length
  srcBuf  = (Byte *)that;
  srcSize = Strlen(that);

  // Special case for zero length string constant...
  if (srcSize == 0) {
  	type = ASCII0;
  	isMutable = false;
    isByte = isChar = false;
  	goto OK;
  }
  // Do it...
  if (srcSize >= UInt16NaN)
    type = Const;
  else
    type = ConstLen;
  constLen = srcSize;
  isMutable = false;
  data = (Byte *)that;
  isByte = isChar = false;

  // Count the characters
  status = UTF8::Count(srcBuf, srcSize, byteCount, charCount);
  if (status) goto SOMEERROR;
  isByte = (byteCount == charCount);
  isChar = !isByte;

  // If it's one character and it's UTF-8, make it UtfInline
  if (charCount == 1 && isChar) {
    type = UtfInline;
    MemoryCopyRaw(srcBuf, (Byte *)&data, byteCount);
    constLen = byteCount;
  }
  
  // If it's ASCII and <= 4 characters, make it ASCII0..ASCII4...
  else if (isByte && constLen <= 4) {
    type = ASCII0 + byteCount;
    MemoryCopyRaw(srcBuf, (Byte *)&data, byteCount);    
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  return status;

}

Status String::_BuildFrom(char const *that, UInt len) {

  Status  status;
  UInt    srcSize;

  if (BEFE::IsNull(that)) goto OK;

  // Get the length
  srcSize = len;
  if (srcSize == 0) goto OK;

  if (srcSize >= UInt16NaN)
    type = Const;
  else {
    type = ConstLen;
    constLen = srcSize;
  }
  isMutable = false;
  data = (Byte *)that;

  // NOTE: For speed's sake, we've eliminated the UTF8::Count call that's
  //       in BuildFrom(char const *) and set isByte/isChar to "don't know"
  //       instead.  If that pans out we may want to remove it from the
  //       other one as well for faster massive string construction.
  isByte = isChar = false;

  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
  }

  return status;

}

Status String::_BuildFrom(Char thechar) {

  Status status;
  UInt   charLen;
  
  // If ASCII, build it from that
  if (thechar <= (UInt)0x7f) {
    type = ASCII1;
    isByte = true;
    isChar = false;
    *((Byte *)&data) = (Byte)thechar;
    constLen = 1;
  }

  // Else, if Unicode, build it from that
  else if (thechar <= (UInt)0x10ffff) {
    type = UtfInline;
    isByte = false;
    isChar = true;
    UTF8::EncodeChar(thechar,(Byte *)&data,charLen);
    constLen = charLen;
  }

  // Else, it's a bad codepoint
  else
    goto INVALIDCODEPOINT;

  // Handle errors
  status = Error::None;
  while (false) {
    INVALIDCODEPOINT: status = Error::BadUnicodeCodePoint; break;
  }

  return status;

}

} // ...Namespace BEFE
