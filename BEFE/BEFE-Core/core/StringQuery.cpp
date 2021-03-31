//----------------------------------------------------------------------
// File: StringQuery.cpp - String Class Query/Info methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "UTF8.h"
#include "Crc32.h"
#include "SBuffer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Query methods
//
//

Boolean String::IsASCII() const {
  
  Byte *buf;
  UInt  byteCount;
  UInt  charCount;
  
  ((String *)this)->_BufByteAndCharCount(buf, byteCount, charCount);
  
  return isByte;
}

Boolean String::IsUTF8() const {

  Byte *buf;
  UInt  byteCount;
  UInt  charCount;
  
  ((String *)this)->_BufByteAndCharCount(buf, byteCount, charCount);
  
  return isChar;

}

Boolean String::IsUnicode() const {

  Byte *buf;
  UInt  byteCount;
  UInt  charCount;
  
  ((String *)this)->_BufByteAndCharCount(buf, byteCount, charCount);
  
  return isChar;

}

Boolean String::IsSensitive() const {
  return isSensitive;
}

Boolean String::IsInsensitive() const {
  return !isSensitive;
}

String &String::Sensitivity(Boolean sense) const {
  ((String *)this)->isSensitive = sense;
  return *((String *)this);
}

String &String::Sensitive() const {
  ((String *)this)->isSensitive = true;
  return *((String *)this);
}

String &String::Insensitive() const {
  ((String *)this)->isSensitive = false;
  return *((String *)this);
}

UInt String::GetType() const {
  return (Int)type;
}

UInt String::Multiplier() const {
  return (UInt)mult;
}

UInt32 String::Hash() const {

  UInt answer;

  if (isSensitive)
    answer = HashSensitive();
  else
    answer = HashInsensitive();

  return answer;

}

UInt32 String::HashSensitive() const {

  SBuffer *sbuf;
  Byte    *srcbuf;
  UInt     srcbufl;
  Crc32    crc;
  UInt     hash;

  srcbuf      = NULL;
  srcbufl     = 0;
  switch (type) {
    case Null:
      break;
    case Const:
      srcbuf = (Byte *)data;
      if (BEFE::IsNull(srcbuf)) break;
      srcbufl = Strlen((char *)data);
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
      srcbufl = type - ASCII0;
      break;
    case UtfInline:
      srcbuf  = (Byte *)&data;
      srcbufl = constLen;
      break;
    case Buffer:
      sbuf = (SBuffer *)&data;
      if (BEFE::IsNull(sbuf)) break;
      srcbuf = sbuf->GetBuf();
      srcbufl = sbuf->GetUsedBytes();
      break;
    default:
      break;
  }

  // If we have a buffer and length, ask UTF8 to count it
  if (!BEFE::IsNull(srcbuf) && srcbufl != 0)
    crc.Update(srcbuf,srcbufl);
  hash = crc.GetValue();

  return hash;

}

UInt32 String::HashInsensitive() const {

  SBuffer *sbuf;
  Byte    *srcbuf;
  UInt     srcbufl;
  Crc32    crc;
  UInt     hash;
  Byte     theByte;

  srcbuf      = NULL;
  srcbufl     = 0;
  switch (type) {
    case Null:
      break;
    case Const:
      srcbuf = (Byte *)data;
      if (BEFE::IsNull(srcbuf)) break;
      srcbufl = Strlen((char *)data);
      break;
    case ConstLen:
      srcbuf  = (Byte *)data;
      srcbufl = constLen;
      break;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      srcbuf  = (Byte *)&data;
      srcbufl = type - ASCII0;
      break;
    case UtfInline:
      srcbuf  = (Byte *)&data;
      srcbufl = constLen;
      break;
    case Buffer:
      sbuf = (SBuffer *)&data;
      if (BEFE::IsNull(sbuf)) break;
      srcbuf = sbuf->GetBuf();
      srcbufl = sbuf->GetUsedBytes();
      break;
    default:
      break;
  }

  // If we have a buffer and length, ask UTF8 to count it
  while (!BEFE::IsNull(srcbuf) && srcbufl != 0) {
    theByte = *srcbuf++;
    srcbufl--;
    if (theByte >= 'A' && theByte <= 'Z')
      theByte = theByte - 'A' + 'a';
    crc.Update(theByte);
  }
  hash = crc.GetValue();
  
  return hash;

}

} // ...Namespace BEFE
