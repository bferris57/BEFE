//----------------------------------------------------------------------
// File: StringReverse.cpp - String Reverse Character methods
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

namespace BEFE { // Namespace BEFE...

Status String::Reverse() {
  
  Status  status;
  Byte   *buf;
  UInt    size;
  
  _BufAndSize(buf, size);
  
  if (isChar)
    status = UTF8::ReverseChars(buf, size);
  else {
    MemoryReverseBytes(buf, size);
    status = Error::None;
  }
  
  return status;
  
}

Status String::Reverse(Range const &range) {
  
  Status  status;
  Byte   *buf;
  UInt    size;
  UInt    len;
  Range   rRange;
  Int     pos1;
  Int     pos2;
  Char    someChar;

  // Get the String's buffer info  
  _BufByteAndCharCount(buf, size, len);

  // Resolve the range and normalise it to be idx1 <= idx2
  rRange = range.Resolve(len);
  if (rRange.idx1 > rRange.idx2) {
    rRange.idx1 ^= rRange.idx2;
    rRange.idx2 ^= rRange.idx1;
    rRange.idx1 ^= rRange.idx2;
  }
  if (rRange.idx1 > rRange.idx2) goto OK;
  
  // Get the resolved Range's Byte positions
  status = UTF8::GetCharPos(buf, size, rRange.idx1, 0, pos1, someChar);
  if (status) goto SOMEERROR;
  status = UTF8::GetCharPos(buf, size, rRange.idx2, 0, pos2, someChar);
  if (status) goto SOMEERROR;

  // Adjust the 2nd Byte position to be last byte in the Char
  if (pos2 < (Int)size)
    pos2 += Max(1,UTF8::GetByteSize(someChar));
  buf  += pos1;
  size = pos2-pos1;

  // Now, reverse them...
  if (isChar) {
    status = UTF8::ReverseChars(buf, size);
    if (status) goto SOMEERROR;
  }
  else
    MemoryReverseBytes(buf, size);

  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;
  
}

Status String::Reverse(Span const &span) {
  
  Status  status;
  Byte   *buf;
  UInt    size;
  UInt    len;
  Span    rSpan;
  Int     pos1;
  Int     pos2;
  Char    someChar;

  // Get the String's buffer info  
  _BufByteAndCharCount(buf, size, len);

  // Resolve the range and normalise it to be idx1 <= idx2
  rSpan = span.Resolve(len);
  if (rSpan.idx1 > rSpan.idx2) {
    rSpan.idx1 ^= rSpan.idx2;
    rSpan.idx2 ^= rSpan.idx1;
    rSpan.idx1 ^= rSpan.idx2;
  }
  if (rSpan.idx1 > rSpan.idx2) goto OK;
  
  // Get the resolved Range's Byte positions
  status = UTF8::GetCharPos(buf, size, rSpan.idx1, 0, pos1, someChar);
  if (status) goto SOMEERROR;
  status = UTF8::GetCharPos(buf, size, rSpan.idx2, 0, pos2, someChar);
  if (status) goto SOMEERROR;

  // Adjust the 2nd Byte position to be last byte in the Char
  buf  += pos1;
  size = pos2-pos1;

  // Now, reverse them...
  if (isChar) {
    status = UTF8::ReverseChars(buf, size);
    if (status) goto SOMEERROR;
  }
  else
    MemoryReverseBytes(buf, size);

  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;
  
}

} // ...Namespace BEFE
