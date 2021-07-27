//----------------------------------------------------------------------
// File: StringFormat.cpp - String Class Formatting methods
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

Status String::ToBytes(Byte *buf, UInt maxlen, UInt &usedlen) const {

  Status   status;
  SBuffer *sbuf;
  Byte    *srcbuf;
  UInt     srcbufl;
  UInt     remmult;

  // Validate parameters
  if (BEFE::IsNull(buf)) goto INVALIDPARAMETER;

  // Initialise
  usedlen  = 0;
  srcbuf   = NULL;
  srcbufl  = UNaN;

  // Output it...
  switch (type) {
    case Null:
      srcbufl = 0;
      break;
    case Const:
      if (data) {
        srcbuf = (Byte *)data;
        srcbufl = Strlen((const char *)srcbuf);
      }
      else
        srcbufl = 0;
      break;
    case ConstLen:
      srcbuf = (Byte *)data;
      srcbufl = constLen;
      break;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      srcbuf  = (Byte *)&data;
      srcbufl = type-ASCII0;
      break;
    case UtfInline:
      srcbuf  = (Byte *)&data;
      srcbufl = constLen;
      break;
    case Buffer:
      if (data) {
        sbuf   = (SBuffer *)&data;
        srcbuf = sbuf->GetBuf();
        if (BEFE::IsNull(srcbuf)) break;
        srcbufl = sbuf->GetUsedBytes();
      }
      break;
    default:
      goto INTERNALERROR;
  }

  // Copy the data
  remmult = mult;
  usedlen = 0;
  while (remmult > 0 && srcbufl != 0 && !BEFE::IsNull(srcbufl) ) {
    if (srcbufl > maxlen) {
      MemoryCopyRaw(srcbuf,buf,maxlen);
      usedlen += maxlen;
      buf     += maxlen;
      maxlen   = UNaN;
      break;
    }
    MemoryCopyRaw(srcbuf,buf,srcbufl);
    usedlen += srcbufl;
    buf     += srcbufl;
    maxlen  -= srcbufl;
    remmult--;
  }

  // Handle buffer overflow/truncation
  if ((remmult > 0 && srcbufl > 0) || BEFE::IsNull(maxlen) ) goto TRUNCATED;

  // NUL terminate if room
  if ( !BEFE::IsNull(maxlen) && maxlen != 0)
    *buf = 0x00;

  // Handle errors
  status = Error::None;
  while (false) {
    INVALIDPARAMETER: status = Error::InvalidParameter; break;
    INTERNALERROR:    status = Error::Internal;         break;
    TRUNCATED:        status = Error::StringTruncated;  break;
  }

  return status;

}

Status String::ToEllipsis(Byte *buf, UInt maxlen) const {

  Status status;
  UInt   usedlen;

  status = ToEllipsis(buf,maxlen,usedlen);

  return status;

}

Status String::ToEllipsis(Byte *buf, UInt maxlen, UInt &usedlen) const {

  Status status;

  status = ToBytes(buf,maxlen,usedlen);
  if (status == Error::StringTruncated && maxlen > 4) {
    Strcpy(buf+maxlen-4,"...");
    usedlen--;
    status = Error::None;
  }
  return status;
}

} // ...Namespace BEFE
