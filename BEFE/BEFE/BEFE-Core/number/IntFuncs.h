//!befe-publish inc
//----------------------------------------------------------------------
// File: Intfuncs.h - Primitive Int datatype functions
//----------------------------------------------------------------------
//
// This header provides the header for various Int related functions.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef INTFUNCS_H // INTFUNCS_H...
#define INTFUNCS_H

#include "Error.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// High level Int based functions
//

UShort  ShortReverseBytes(UShort i);
UInt    IntReverseBytes(UInt i);
ULong   LongReverseBytes(ULong l);
UInt    IntSqrt(UInt i);
UInt    IntLog2(UInt i);
UInt    IntLog10(UInt i);
Boolean IntIsPrime(UInt i);
UInt    IntNextPrime(UInt i);
UInt    IntGCD(UInt a, UInt b);
UInt    IntLCM(UInt a, UInt b);

UInt32  IntCrc32(UInt8 *s, UInt len);
UInt32  IntCrc32(UInt32 seed, UInt c);

void    BitRange(UInt num, UInt &bit0, UInt &bit1);
Boolean IsPowerOfTwo(UInt n);
UInt    RoundToPowerOfTwo(UInt n);

UInt    Hash(UInt i);
UInt    Hash(UInt8 *addr);
UInt    Hash(const char *s);
UInt    Hash(const char *s, Int len);

// IntBits functions
UInt    IntBitCount(UInt theInt);
UInt    IntBitMax(UInt numbits);
UInt    IntBitScatter(UInt theCollectedInt, UInt theMask);
UInt    IntBitCollect(UInt theScatteredInt, UInt theMask);

// Int and UInt32 from String Workhorse functions
Status  UInt32FromString(Byte *buf, UInt bufl, Slice slice, UInt base, UInt32 &theInt);
UInt32  UInt32FromString(Byte *buf, UInt bufl, Slice slice, UInt base);
Status  UInt32FromString(String const &str, Slice slice, UInt base, UInt32 &theInt);
Status  Int32FromString (Byte *buf, UInt bufl, Slice slice, Int32 &theInt);
Int32   Int32FromString (Byte *buf, UInt bufl, Slice slice);
Status  Int32FromString (String const &str, Slice slice, Int32 &theInt);
Status  Int32FromString (String const &str, Int32 &theInt);

// UInt32 from String convenience functions
//UInt32  UInt32FromString(Byte *buf,UInt bufl);
//UInt32  UInt32FromString(String &str);
//UInt32  UInt32fromString(String &str, UInt base);
//UInt32  UInt32FromString(String &str, Slice slice);
UInt32  UInt32FromString(String const &str, Slice slice, UInt base);
//UInt32  UInt32FromString(const char *buf);
//UInt32  UInt32FromString(const char *buf, UInt base);
//UInt32  UInt32FromString(const char *buf, Slice slice);
//UInt32  UInt32FromString(const char *buf, Slice slice, UInt base);
//UInt32  UInt32FromString(char *buf);
//UInt32  UInt32FromString(char *buf, UInt base);
//UInt32  UInt32FromString(char *buf, Slice slice);
//UInt32  UInt32FromString(char *buf, Slice slice, UInt base);

// Int from String convenience functions
Int32   Int32FromString(Byte *buf, UInt bufl);
Int32   Int32FromString(String const &str);
//Int32   Int32fromString(String &str, UInt base);
Int32   Int32FromString(String const &str, Slice slice);
//Int32   Int32FromString(String &str, Slice slice, UInt base);
//Int32   Int32FromString(const char *buf);
//Int32   Int32FromString(const char *buf, UInt base);
//Int32   Int32FromString(const char *buf, Slice slice);
//Int32   Int32FromString(const char *buf, Slice slice, Int base);
//Int32   Int32FromString(char *buf);
//Int32   Int32FromString(char *buf, UInt base);
//Int32   Int32FromString(char *buf, Slice slice);
//Int32   Int32FromString(char *buf, Slice slice, UInt base);

} // ...Namespace BEFE

#endif // INTFUNCS_H
