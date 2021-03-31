//!befe-publish inc
//----------------------------------------------------------------------
// File: ToString.h - Various ToString utility functions
//----------------------------------------------------------------------
//
// These functions are the BEFE equivalent of the Python str() and
// Java toString() functions.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TOSTRING_H // TOSTRING_H...
#define TOSTRING_H

#include "Primitives.h"
#include "Slice.h"
#include "Alloc.h"
#include "Range.h"
#include "IdSpace.h"
#include "Integer.h"
#include "MD5.h"

namespace BEFE { // Namespace BEFE...

// ByteUnits -> String
String ByteUnits(Long num);
Status ByteUnits(Long num,String &formatted);

// Boolean -> String...
String ToString(Boolean b);

// Int -> String...
//String ToString(Int8   num);
String ToString(Int16  num);
String ToString(Int32  num);
String ToString(Int64  num);
//String ToString(Int8   num, UInt len);
String ToString(Int16  num, UInt len);
String ToString(Int32  num, UInt len);
String ToString(Int64  num, UInt len);

// UInt -> String...
//String ToString(UInt8  num);
String ToString(UInt16 num);
String ToString(UInt32 num);
String ToString(UInt64 num);
//String ToString(UInt8  num, UInt len);
String ToString(UInt16 num, UInt len);
String ToString(UInt32 num, UInt len);
String ToString(UInt64 num, UInt len);

// Integer -> String...
String ToString(Integer const &num);
String ToString(Integer const &num, UInt len);

// Int -> HexString...
//String ToHexString(Int8  num);
//String ToHexString(Int16 num);
String ToHexString(Int32  num);
String ToHexString(Int32  num, UInt len);   // ◄── len is in "destination Chars"
String ToHexString(UInt32 num);
String ToHexString(UInt32 num, UInt len);   // ◄── len is in "destination Chars"
String ToHexString(Int64  num);
String ToHexString(Int64  num, UInt len);   // ◄── len is in "destination Chars"
String ToHexString(UInt64 num);
String ToHexString(UInt64 num, UInt len);   // ◄── len is in "destination Chars"
String ToHexString(Byte *bp, UInt len);     // ◄── len is in "Source Bytes" NOT "Destination Chars"

// Time -> String handy form
String ToString(Time t1, Time t2);
String ToString(Time ts);
String ToString(Time ts, Time::Unit unit);

// Time -> String normal form
Status ToString(Time t1, Time t2, String &string);
Status ToString(Time ts, String &string);
Status ToString(Time ts, Time::Unit unit, String &string);

// Sequence declarations -> String handy form
String ToString(Span   span);
String ToString(USpan  span);
String ToString(Range  range);
String ToString(URange range);
String ToString(Slice  slice);
String ToString(Alloc  alloc);

// Sequence declarations -> String normal form
Status ToString(Slice slice, String &string);
Status ToString(Alloc alloc, String &string);
Status ToString(Range range, String &string);
Status ToString(URange range, String &string);

// Instance info -> String handy form
String ToString(IdSpaceInfo &info);

// Instance info -> String normal form
Status ToString(IdSpaceInfo &info, String &string);

// SVN Stuff...
String SVNGetName(char const *rawsvn);
String SVNGetValue(char const *rawsvn);
void   SVNParse(char const *rawsvn,String &name,String &value);

// MD5...
String ToHexString(MD5 const &that);

} // ...Namespace BEFE

#endif // ...TOSTRING_H
