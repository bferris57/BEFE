//----------------------------------------------------------------------
// File: StringManip.cpp - String Class Manipulation methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "UTF8.h"
#include "SBuffer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Mixtures of operations...
//

String &String::TruncateOrPad(UInt len) {

  Truncate(len);
  Pad(len,' ');

  return *this;

}

String &String::TruncateOrPad(UInt len, Char padchar) {

  Truncate(len);
  Pad(len,padchar);

  return *this;

}

String String::PadOrEllipsis(UInt len) {

  String that;
  
  that = Ellipsis(len);
  that.Pad(len);

  return that.Consumable();

}

String String::PadOrEllipsis(UInt len,Char padchar) {

  String that;
  
  that = Ellipsis(len);
  that.Pad(len,padchar);

  return that.Consumable();

}

//----------------------------------------------------------------------
//
// Truncate...
//

String &String::Truncate() {
  SetEmpty();
  return *this;
}

String &String::Truncate(UInt len) {

  Status   status;
  Byte    *srcBuf;
  UInt     srcBufSize;
  SBuffer *sBuf;
  UInt     byteCount;
  UInt     charCount;
  UInt     vSize;
  UInt     newSrcBufSize;

  // No semantics (yet) for negative lengths, but it's
  // certainly "smaller" than it is right now, so we'll
  // clear it.
  if (len <= 0) {
    SetEmpty();
    goto OK;
  }

  // In case it's not a Buffer, there's special cases here...
  switch (type) {
    case Null:
      goto OK;
    case Const:
      goto NOTMUTABLE;
    case ConstLen:
      goto NOTMUTABLE;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      vSize = type - ASCII0;
      if (len < vSize)
        type = ASCII0 + len;
      goto OK;
    case UtfInline:
      vSize = constLen;
      if (len < vSize)
        type = len;
      goto OK;
    case Buffer:
      break;
    default:
      goto INTERNALERROR;
  }

  // Okay, we've got something in a buffer here...
  // Get its virtual and physical size and new virtual size
  status = _BufByteAndCharCount(srcBuf,byteCount,charCount);
  if (status) goto SOMEERROR;
  vSize = charCount*mult;

  // If already less, no problem
  if (vSize <= len) goto OK;

  // Make it mutable
  status = Mutable();
  if (status) goto SOMEERROR;

  // Truncate it
  sBuf = (SBuffer *)&data;
  srcBuf = sBuf->GetBuf();
  if (BEFE::IsNull(srcBuf)) goto INTERNALERROR;
  srcBufSize = sBuf->GetUsedBytes();
  if (srcBufSize <= len) goto OK;

  // Get the byte and character counts
  byteCount = srcBufSize;
  charCount = sBuf->Length();
  if (charCount <= len) goto OK;

  // Yep, we need to truncate it.  Find where to truncate it...
  //
  // Note: We do this by asking the UTF8 code to find the character
  //       BEFORE tne new length.  We do this because, UTF8::GetChar
  //      does a "Get" and we throw away the value because we don't
  //       really care what's there, we just want to position.  After
  //       it does the "Get", it bumps the "newpos" (our newsrcbufl)
  //       to indicate the NEXT byte position for the next character.
  //
  //       Not to worry, we've already tested for len <= 0 so, in
  //       subtracting one from it we won't be giving UTF8 a negative
  //       number!

  UTF8::GetChar(srcBuf, srcBufSize, len-1, 0, (Int &)newSrcBufSize);
  status = sBuf->SetSize(newSrcBufSize);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    NOTMUTABLE:    status = Error::StringNotMutable; break;
    INTERNALERROR: status = Error::Internal;         break;
    SOMEERROR:                                       break;
    OK:            status = Error::None;             break;
  }

  return *this;

}

//----------------------------------------------------------------------
//
// Trim...
//

String &String::Trim() {

  LeftTrim();
  RightTrim();

  return *this;

}

String &String::LeftTrim() {

  Status   status;
  Byte    *buf;
  UInt     bufl;
  UInt     oldBufl;
  SBuffer *sbuf;

  // Handle special cases...
  if (type == Null)
    goto OK;
  else if (type == Const) {
    buf = data;
    while (buf && *buf && *buf <= SP)
      buf++;
    data = buf;
    goto OK;
  }
  else if (type == ConstLen) {
    buf = data;
    while (constLen && buf && *buf && *buf <= SP) {
      buf++;
      constLen--;
    }
    goto OK;
  }
  else if (type == ASCII0)
    goto OK;
  else if (type >= ASCII1 && type <= ASCII4) {
    buf = (Byte *)&data;
    while (type > ASCII0 && *buf <= 0x20) {
      buf[0] = buf[1];
      buf[1] = buf[2];
      buf[2] = buf[3];
      type--;
      constLen--;
    }
    goto OK;
  }
  
  //
  // Needs to be mutable
  //
  
  status = _MorphTo(Buffer);
  if (status) goto SOMEERROR;

  switch (type) {
    case Null:     goto OK;
    case Const:    goto INTERNALERROR;
    case ConstLen: goto INTERNALERROR;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      buf = (Byte *)&data;
      bufl = type - ASCII0;
      break;
    case UtfInline:
      buf = (Byte *)&data;
      bufl = constLen;
      break;
    case Buffer:
      sbuf = (SBuffer *)&data;
      if (!sbuf) goto OK;
      buf  = sbuf->GetBuf();
      bufl = sbuf->Length();
      break;
    default:
      goto INTERNALERROR;
  }

  // Now, we're either pointing at data (if ASCII) or somewhere
  // in the sBuffer (if Buffer)...
  oldBufl = bufl;
  if (buf) {
    while (bufl && *buf <= SP) {
      buf++;
      bufl--;
    }
  }

  // If it's empty...
  if (!bufl) {
    if (type == Buffer) {
      sbuf = (SBuffer *)&data;
      status = sbuf->Reset();
      if (status) goto INTERNALERROR;
    }
    type = Null;
    goto OK;
  }

  // It's not empty...
  if (type == Buffer) {
    sbuf = (SBuffer *)&data;
    status = sbuf->RemoveBytes(0,oldBufl-bufl);
    if (status) goto SOMEERROR;
  }
  else {
    *((UInt *)&data) = *((UInt *)&data) << (bufl-oldBufl)*8;
    type = ASCII0 + bufl;
    if (type < ASCII0) goto INTERNALERROR;
  }

  // Handle errors
  while (false) {
    INTERNALERROR: status = Error::Internal; break;
    OK:            status = Error::None;     break;
    SOMEERROR:                               break;
  }
  if (status)
    BEFE_ERROR("String.LeftTrim",status); 
    
  return *this;

}

String &String::RightTrim() {

  Status   status;
  Byte    *buf;
  Int      bufl;
  SBuffer *sbuf;

  // Handle special cases...
  if (type == Null)
    goto OK;
  else if (type == ConstLen) {
    if (constLen == 0 || !data) goto OK;
    buf = data + constLen-1;
    while (constLen && *buf <= SP) {
      constLen--;
      buf--;
    }
    goto OK;
  }

  // Special case for ASCII0..ASCII4  
  if (type >= ASCII0 && type <= ASCII4) {
    buf = (Byte *)&data+constLen-1;
    while (type > ASCII0 && *buf-- <= 0x20) {
      constLen--;
      type--;
    }
    goto OK;
  }

  //
  // Needs to be mutable
  //
  
  status = _MorphTo(Buffer);
  if (status) goto SOMEERROR;

  switch (type) {
    case Null:     goto OK;
    case Const:    goto INTERNALERROR;
    case ConstLen: goto INTERNALERROR;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      buf = (Byte *)&data;
      bufl = type - ASCII0;
      break;
    case UtfInline:
      buf = (Byte *)&data;
      bufl = constLen;
      break;
    case Buffer:
      sbuf = (SBuffer *)&data;
      if (!sbuf) goto OK;
      buf  = sbuf->GetBuf();
      bufl = sbuf->Length();
      break;
    default:
      goto INTERNALERROR;
  }

  // Now, we're either pointing at data (if ASCII) or somewhere
  // in the sBuffer (if Buffer)...
  if (buf) {
    buf += bufl-1;
    while (bufl && *buf <= SP) {
      buf--;
      bufl--;
    }
  }

  // If it's empty...
  if (!bufl) {
    if (type == Buffer) {
      sbuf = (SBuffer *)&data;
      status = sbuf->SetEmpty();
      if (status) goto INTERNALERROR;
    }
    type = Null;
    goto OK;
  }

  // It's not empty...
  if (type == Buffer) {
    sbuf = (SBuffer *)&data;
    status = sbuf->SetSize(bufl);
    if (status) goto SOMEERROR;
  }
  else {
    type = ASCII0 + bufl;
    if (type < ASCII0) goto INTERNALERROR;
  }

  goto OK;

  // Handle errors
  while (false) {
    SOMEERROR:                               break;
    INTERNALERROR: status = Error::Internal; break;
    OK:            status = Error::None;     break;
  }
  if (status)
    BEFE_ERROR("String.RightTrim",status); 
    
  return *this;

}

//----------------------------------------------------------------------
//
// Pad...
//

String &String::Pad(UInt len) {
  return RightPad(len,' ');
}

String &String::Pad(UInt len, Char padchar) {
  return RightPad(len, padchar);
}

String &String::RightPad(UInt len) {
  return RightPad(len,' ');
}

String &String::RightPad(UInt len, Char padchar) {

  Status  status;
  Byte   *srcBuf;
  UInt    byteCount;
  UInt    charCount;
  UInt    vLen;

  // No semantics (yet) for zero lengths, but it is definitely
  // "smaller" than it is right now if it has a length, so there's
  // nothing to do.
  if (len == 0) goto OK;

  // Get its virtual and physical size and new virtual size
  status = _BufByteAndCharCount(srcBuf,byteCount,charCount);
  if (status) goto SOMEERROR;
  vLen = charCount*mult;

  // if already this len or greater, no problem
  if (vLen >= len) goto OK;

  // Special case if character length is one ...
  if (vLen == 1 && Get() == padchar) {
    mult = len;
    goto OK;
  }

  // Make it mutable
  status = Mutable();
  if (status) goto SOMEERROR;

  // Pad it
  while (vLen < len) {
    status = Append(padchar);
    if (status) goto SOMEERROR;
    vLen++;
  }

  goto OK;

  // Handle errors
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }
  if (status)
    BEFE_ERROR("String.RightPad",status); 
    
  return *this;

}

String &String::LeftPad(UInt len) {
  return LeftPad(len,' ');
}

String &String::LeftPad(UInt len, Char padchar) {

  Status  status;
  Byte   *srcBuf;
  UInt    byteCount;
  UInt    charCount;
  UInt    vLen;
  String  tString;
  
  // No semantics (yet) for zero lengths, but it is definitely
  // "smaller" than it is right now if it has a length, so there's
  // nothing to do.
  if (len == 0) goto OK;

  // Get its virtual and physical size and new virtual size
  status = _BufByteAndCharCount(srcBuf,byteCount,charCount);
  if (status) goto SOMEERROR;
  vLen = charCount*mult;

  // if already this len or greater, no problem
  if (vLen >= len) goto OK;

  // Special case if character length is one ...
  if (vLen == 1 && Get() == padchar) {
    mult = len;
    goto OK;
  }

  // Make it mutable
  status = Mutable();
  if (status) goto SOMEERROR;

  // Pad it (this is a little more costly than padding to the right but
  //         that's what we were asked to do)...
  if (vLen < len) {
    tString = padchar;
    tString *= len-vLen;
    tString += *this;
    tString.Consumable();
    *this = tString.Consumable();
  }

  // Handle errors
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }
  if (status)
    BEFE_ERROR("String.LeftPad",status);
    
  return *this;

}

//----------------------------------------------------------------------
//
// Ellipsis...
//

String String::Ellipsis(UInt len) {
  return PostEllipsis(len);
}

String String::PreEllipsis(UInt len) {

  Status status;
  UInt   curlen;
  String eString;
  String that;

  that = *this;

  // Arbitrairy assertion: "If we're trying to Ellipsis something to six
  //                        or less characters long, we'll hide more
  //                        than we'll help.  So, we won't add the
  //                        Ellipses. Instead, we'll just truncate"
  if (len <= 6) {
    status = that.Truncate(len);
    if (status) goto SOMEERROR;
    goto OK;
  }

  // Seems to be a sane request, iet's get on with it...
  curlen = that.Length();
  if (curlen <= len) goto OK;
  status = that.Get(Slice(-(len-3)),eString);
  if (status) goto SOMEERROR;
  that = "...";
  status = that.Append(eString.Consumable());
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }
  if (status)
    BEFE_ERROR("String.Ellipsis",status);
    
  return that.Consumable();

}

String String::MidEllipsis(UInt len) {

  Status status;
  UInt   curlen;
  String ePre;
  String ePost;
  String that;
  
  that = *this;

  // Arbitrairy assertion: "If we're trying to Ellipsis something to six
  //                        or less characters long, we'll hide more
  //                        than we'll help.  So, we won't add the
  //                        Ellipses. Instead, we'll just truncate"
  if (len <= 6) {
    status = that.Truncate(len);
    if (status) goto SOMEERROR;
    goto OK;
  }

  // Seems to be a sane request, iet's get on with it...
  curlen = that.Length();
  if (curlen <= len) goto OK;
  status = that.Get(Slice(0,(len-3)/2),ePre);
  if (status) goto SOMEERROR;
  status = that.Get(Slice(-((len-3)-(len-3)/2)),ePost);
  if (status) goto SOMEERROR;
  that  = ePre.Consumable();
  that += "...";
  that += ePost.Consumable();
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }
  if (status)
    BEFE_ERROR("String.MidEllipsis",status);
    
  return that.Consumable();

}

String String::PostEllipsis(UInt len) {

  Status status;
  UInt   curlen;
  String that;
  
  that = *this;
  
  // Arbitrairy assertion: "If we're trying to Ellipsis something to six
  //                        or less characters long, we'll hide more
  //                        than we'll help.  So, we won't add the
  //                        Ellipses. Instead, we'll just truncate"
  if (len <= 6) {
    that.Truncate(len);
    goto OK;
  }

  // Seems to be a sane request, iet's get on with it...
  curlen = that.Length();
  if (curlen <= len) goto OK;

  status = that._MorphTo(Buffer,len);
  if (status) goto SOMEERROR;
  status = that.Truncate(len-3);
  if (status) goto SOMEERROR;
  status = that.Append("...");
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }
  if (status)
    BEFE_ERROR("String.PostEllipsis",status);
    
  return that;

}

} // ...Namespace BEFE
