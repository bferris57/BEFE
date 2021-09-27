//!befe-publish inc
//----------------------------------------------------------------------
// File: ToAscii.h - Various ToAscii utility functions
//----------------------------------------------------------------------
//
// These functions are the ASCII equivalent of various ToString 
// functions
//
// These functions sort of perform the same functions as standard C/C++
// functions like itoa(), etc.
//
// As such, note that it's entirely up to the caller to ensure that the
// buffer passed to them is big enough to contain the entire ASCII
// form requested.
//
// Note: As a convenience, we've provided an enum 'ToAscii_MaxNumLen'
//       which can be used to allocate local buffers.  It indicates
//       the absolute maximum bytes needed for these conversions
//       including the trailing NUL.
//
// Note: For Hex, the maximum bytes required is 17 (16 hex digits and
//       trailing NUL)
//
//       For signed Decimal, the maximum bytes required is 24
//       (22 decimal digits, possible sign, and trailing NUL)
//
//       For unsigned Decimal, the maximum bytes required is 23
//       (22 decimal digits and trailing NUL)
//
// Note: Although here at BEFE we have a great dislike for the whole
//       trailing NUL thing but, since it seems to stick in so many
//       coder's heads, we've kept with the flow mainly because it
//       keeps us from having to pass a length around everywhere.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TOASCII_H // TOASCII_H...
#define TOASCII_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

// Handy enum...
enum {
  ToAscii_MaxNumLen = 32
};

// Int -> Ascii...
//void ToAscii(Int8   num, Byte *buf);
void ToAscii(Int16  num, Byte *buf);
void ToAscii(Int32  num, Byte *buf);
void ToAscii(Int64  num, Byte *buf);
//void ToAscii(Int8   num, size_t len, Byte *buf);
void ToAscii(Int16  num, size_t len, Byte *buf);
void ToAscii(Int32  num, size_t len, Byte *buf);
void ToAscii(Int64  num, size_t len, Byte *buf);

// UInt -> Ascii...
//void ToAscii(UInt8  num, Byte *buf);
void ToAscii(UInt16 num, Byte *buf);
void ToAscii(UInt32 num, Byte *buf);
void ToAscii(UInt64 num, Byte *buf);
//void ToAscii(UInt8  num, size_t len, Byte *buf);
void ToAscii(UInt16 num, size_t len, Byte *buf);
void ToAscii(UInt32 num, size_t len, Byte *buf);
void ToAscii(UInt64 num, size_t len, Byte *buf);

// Int -> HexAscii...
//void ToHexAscii(Int8   num, Byte *buf);
//void ToHexAscii(Int16  num, Byte *buf);
//void ToHexAscii(Int32  num, Byte *buf);
//void ToHexAscii(Int64  num, Byte *buf);
void ToHexAscii(UInt num, Byte *buf);
void ToHexAscii(UInt num, UInt len, Byte *buf);

} // ...Namespace BEFE

#endif // ...TOASCII_H
