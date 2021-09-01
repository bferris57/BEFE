//----------------------------------------------------------------------
// File: StringAppend.cpp - String Class Append methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "UTF8.h"
#include "SBuffer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Method: Append(Char thechar) - Append single Char to String
//
//

Status String::Append(Char thechar) {

  Status   status;
  SBuffer *sbuf;
  Byte    *srcbuf;
  UInt     srcbufl;
  UInt     newlen;
  Byte     utfbuf[UTF8::MaxLength];

  // If Null or ASCII0, use BuildFrom...
  if (type == Null || type == ASCII0) {
    status = _BuildFrom(thechar);
    if (status) goto SOMEERROR;
    goto OK;
  }

  // Else, if ASCII0..ASCII3, just append it here unless it needs UTF-8...
  else if (type >= ASCII0 && type <= ASCII3) {
    if (thechar <= (UInt)0x7f) {
      srcbuf = (Byte *)&data;
      srcbuf[type-ASCII0] = (Byte)thechar;
      type++;
      constLen = type-ASCII0;
      goto OK;
    }
  }

  // Calculate the new length
  newlen = Size() + UTF8::GetByteSize(thechar);

  // Time to Morph to a Buffer and append it
  status = _MorphTo(Buffer,newlen);
  if (status) goto SOMEERROR;
  if (type != Buffer) goto INTERNALERROR;
  sbuf = (SBuffer *)&data;
  if (BEFE::IsNull(sbuf->pointer)) goto INTERNALERROR;

  // Form the UTF
  status = UTF8::EncodeChar(thechar,utfbuf,srcbufl);
  if (status) goto SOMEERROR;
  status = sbuf->AppendBytes(utfbuf,srcbufl);
  if (status) goto SOMEERROR;

  // Update isByte and IsChar
  if (srcbufl > 1) {
    isByte = false;
    isChar = true;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                               break;
    INTERNALERROR: status = Error::Internal; break;
    OK:            status = Error::None;     break;
  }

  return status;

}

Status String::Append(char thechar) {
  return Append(Char(thechar));
}

//----------------------------------------------------------------------
//
// Method: Append(Byte *buf, Int len) - Append from buffer
//
// !!! Note: We don't handle UTF-8 properly yet !!!
//

Status String::Append(Byte *buf, UInt size) {

  Status   status;
  SBuffer *sBuf;
  Byte    *sBufBuf;
  UInt     byteCount;
  UInt     charCount;
  UInt     newSize;
  UInt     oldMult;
  UInt     i;
  String   tString;
  
  // Validate parameters
  if (BEFE::IsNull(buf)) goto INVALIDPARAMETER;

  // If zero len, nothing to do
  if (size == 0) goto OK;

  // If we have a mult that's not 1, we need to expand into a non-mult...
  if (mult == 0) { // Zero mult...
    status = SetEmpty();
    if (status) goto SOMEERROR;
    mult = 1;
  }
  else if (mult > 1) {
    oldMult = mult;
    mult = 1;
    for (i=oldMult; i > 0; i--) {
      status = tString.Append(*this);
      if (status) goto SOMEERROR;
    }
    *this = tString.Consumable();
  }
  
  // Calculate the byte/char count
  status = UTF8::Count(buf,size,byteCount,charCount);
  if (status) goto SOMEERROR;
  newSize = Size()*mult+size;

  // Special case if the new len <= 4 bytes and can fit in
  // ASCII<N>...
  if (newSize <= 4 && byteCount == charCount) {
    if (type == Null || (type >= ASCII0 && type <= ASCII3)) {
      sBufBuf = (Byte *)&data;
      if (type >= ASCII0 && type <= ASCII3)
        sBufBuf += (type-ASCII0);
      type = ASCII0+newSize;
      constLen = newSize;
      isByte = true;
      isChar = false;
      while (size) {
        *sBufBuf++ = *buf++;
        size--;
      }
      goto OK;
    }
  }

  // We need a SBuffer, so morph into one...
  status = _MorphTo(Buffer,newSize);
  if (status) goto SOMEERROR;
  if (type != Buffer || BEFE::IsNull(data)) goto CANTMORPH;

  // Get the SBuffer and append it
  sBuf = (SBuffer *)&data;
  //???sBuf->StartUp();
  status = sBuf->AppendBytes(buf,size);
  if (status) goto SOMEERROR;

  // Save the SBuffer in case it changed
  //???data = (Byte *)sBuf;

  // Update the isByte/isChar flags
  if (byteCount == charCount) {
    if (isByte == false && isChar == false)
      isByte = true;
  }
  else {
    isByte = false;
    isChar = true;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    OK:               status = Error::None;             break;
    INVALIDPARAMETER: status = Error::InvalidParameter; break;
    CANTMORPH:        status = Error::StringNotMutable; break;
    SOMEERROR:                                          break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Method: Append(String const &that) - Append from another String
//
// !!! Note: We don't handle UTF-8 properly yet !!!
//

Status String::Append(String const &that) {

  Status   status;
  SBuffer *thatsbuf;
  Byte    *thatbuf;
  UInt     thatbufl;
  Int      i;

  switch (that.type) {
    case Null:
      break;
    case Const:
      thatbuf = (Byte *)that.data;
      if (BEFE::IsNull(thatbuf)) break;
      thatbufl = Strlen((const char *)that.data);
      for (i=that.mult; i > 0 ; i--) {
        status = Append(thatbuf,thatbufl);
        if (status) goto SOMEERROR;
      }
      break;
    case ConstLen:
      thatbuf = (Byte *)that.data;
      if (BEFE::IsNull(thatbuf)) break;
      for (i=that.mult; i > 0 ; i--) {
        status = Append(thatbuf,that.constLen);
        if (status) goto SOMEERROR;
      }
      break;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      thatbuf = (Byte *)&that.data;
      thatbufl = that.type-ASCII0;
      for (i=that.mult; i > 0 ; i--) {
        status = Append(thatbuf,thatbufl);
        if (status) goto SOMEERROR;
      }
      break;
    case UtfInline:
      for (i=that.mult; i > 0 ; i--) {
        status = Append((Byte *)&that.data,that.constLen);
        if (status) goto SOMEERROR;
      }
      break;
    case Buffer:
      thatsbuf = (SBuffer *)&that.data;
      if (BEFE::IsNull(thatsbuf)) break;
      thatbuf = thatsbuf->GetBuf();
      if (BEFE::IsNull(thatbuf)) break;
      thatbufl = thatsbuf->GetUsedBytes();
      for (i=that.mult; i > 0 ; i--) {
        status = Append(thatbuf,thatbufl);
        if (status) goto SOMEERROR;
      }
      break;
    default:
      goto INTERNALERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                               break;
    INTERNALERROR: status = Error::Internal; break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Method: Append(char const *that) - Append from C/C++ string literal
//

Status String::Append(char const *that) {

  Status  status;
  Byte   *buf;
  Int     bufl;

  if (BEFE::IsNull(that)) goto OK;
  buf = (Byte *)that;
  bufl = Strlen(that);
  if (bufl == 0) goto OK;
  if (type == Null) {
    status = _BuildFrom(that);
    if (status) goto SOMEERROR;
  }
  else {
    status = Append(buf,bufl);
    if (status) goto SOMEERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Method: Append(char *that) - Append from C/C++ string buffer
//

Status String::Append(char *that) {

  Status  status;
  Byte   *buf;
  UInt    bufl;

  if (BEFE::IsNull(that)) goto OK;
  buf = (Byte *)that;
  bufl = Strlen(that);
  if (bufl == 0) goto OK;
  if (type == Null) {
    status = _BuildFrom(buf,bufl);
    if (status) goto SOMEERROR;
  }
  else {
    status = Append(buf,bufl);
    if (status) goto SOMEERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  return status;

}

} // ...Namespace BEFE
