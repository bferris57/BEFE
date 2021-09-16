//!befe-publish inc
//----------------------------------------------------------------------
// File: Primitives.h - Primitive Data Type declarations
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// Note:  We've chosen to name the datatypes to follow the Java way of
//        thinking about them, sort of...
//
//        We may have to change these to Jint, Jbyte, etc. soon to avoid
//        the distinction between primitive integers, etc. and the Java
//        classes Integer, Long, etc.  Java treats these differently
//        than C++ does, depending on the compiler, and so Java blurs
//        it a bit from the C++ point of view.
//
//        We haven't decided which one wins yet.  Probably C++ will win
//        but Java's in the running because it's one of the more
//        reasonable prototyping languages out there -- much closer to
//        C++ than, say, Python or JavaScript.
//
//        *** see below for more on how we do this in C++ ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef PRIMITIVES_H // PRIMITIVES_H...
#define PRIMITIVES_H

// Note: This MUST be first, at at least early on, since it tell us
//       what Port we're building for...
#include "Port.h"

// Note: We hate this but it's become such a widespread standard that
//       we'll have to stifle our pride and go with the flow...
#ifndef NULL
  #define NULL 0
#endif

#include <cstddef> // For size_t

//
// Macro to answer "Are we 32 or 64 bit pointers?"...
//

// Check windows
#if _WIN32 || _WIN64
#  if _WIN64
#    define IS64BIT
#  else
#    define IS32BIT
#  endif
#endif

// Check GCC
#if __GNUC__
#  if __x86_64__ || __ppc64__
#    define IS64BIT
#  else
#    define IS32BIT
#  endif
#endif

// Note: We don't like preprocessor macros but you gotta use one now
//       and again...
#ifndef BEFE_INLINE
#  define BEFE_INLINE /*static*/ inline
#endif

#ifndef BEFE_inline
#  define BEFE_inline inline
#endif

#ifndef BEFE_Register
#  define BEFE_Register register
#endif

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// BEFE Integer -> C++ mapping
//
//----------------------------------------------------------------------
//
// For coders that care more about Java than physical size in bytes we
// have a few words to add here...
//
//     Most C++ coders yuck at the thought of a Byte being signed.
//     Well, most Java coders have got used to it quickly.  Why the
//     big difference?  Because most bytes that C++ coders come across
//     are often  expressed in hexidecimal.  So, they tend think of them
//     that way.
//
//     Of course, all numbers on computers are simply a set of bits
//     which are usually contiguous (believe it or not, we make good
//     use of non-contiguous bits in some parts of BEFE).
//     
//     But, C/C++ developers are NOT used to thinking about the
//     notation "-0x01".  It happens occasionally but that's the
//     exception not the rule in C/C++.
//
//     On the other hand, most bytes handled by Java programmers are just
//     a character in a string.  So, sign comes in handy because it
//     indicates quickly that it represents a Unicode value and they
//     need to start thinking multi-byte.
//
//     At least, that's our current theory...
//
//     But still, we've stuck for now to the C++ way of thinking and
//     keep data type Byte as an unsigned 8 bit integer (UInt8).
//     The main reason for this is that we want left shifts (operator<<)
//     and right shift (operator>>) on bytes to NOT extend the sign bit
//     because when you're doing stuff like that you're more likely not
//     to want that fairly anti-social sign getting in your way --
//     we've seen way too many bugs caused by C/C++ programmers who
//     forgot about the signs being extended when performing a right 
//     shift on an 'int'.  ;-)
//

//
// General Scalar Sizes...
//
//   For code that cares more about the logical nature of the
//   functionality than the specific implementation
//

typedef          bool      Boolean; // ◄── Wish C++ had a bool:1 or bool:2... but you have to do it by yourself.  :-(
typedef unsigned char      Byte;    // ◄── Forget what Java thinks, signed bytes suck most of the time
typedef unsigned char      UByte;   // Only here for symmetry
typedef signed   char      SByte;   // ◄── Yuck!  That's Java talk that is!!!
typedef signed   short     Short;
typedef unsigned short     UShort;
typedef signed   short     SShort;
typedef signed   int       Int;     // ◄── All hell breaks loose if this changes from 4 bytes!
typedef unsigned int       UInt;
typedef signed   int       SInt;
typedef signed   long long Long;    // ◄── Just like Java.  Get used to this as well!
typedef unsigned long long ULong;
typedef signed   long long SLong;
typedef void *             Ptr;
// Pointer as UInt equivalent...
#ifdef IS64BIT
typedef unsigned long long PtrInt;
#endif
#ifdef IS32BIT
typedef unsigned int       PtrInt;
#endif


//
// Specific Integer Sizes...
//
//   For code that really cares more about physical size in bytes,
//   more like a C/C++ developer than a Java developer.
//

// Definitely signed integers
//   (<, <=, >, >= and >> defined)
typedef signed   char      SInt8;
typedef signed   short     SInt16;
typedef signed   int       SInt32;
typedef signed   long long SInt64;

// Implicitly signed integers
//   (<, <=, >, >= and >> undefined if different signs)
typedef signed   char      Int8;
typedef signed   short     Int16;
typedef signed   int       Int32;
typedef signed   long long Int64;

// Definitely unsigned integers
//   (<, <=, >, >= and >> defined)
typedef unsigned char      UInt8;
typedef unsigned short     UInt16;
typedef unsigned int       UInt32;
typedef unsigned long long UInt64;

//----------------------------------------------------------------------
//
//  BEFE Integer limits
//

namespace Limits { enum {

   Lower       = 0x80000000
  ,Upper       = 0x80000000

  ,UInt8Lower  = 0x00
  ,UInt8Upper  = 0xff
  ,UInt16Lower = 0x0000
  ,UInt16Upper = 0xffff
  ,UInt32Lower = 0x80000001
  ,UInt32Upper = 0x7fffffff

  ,SInt8Lower  = 0x81
  ,SInt8Upper  = 0x7f
  ,SInt16Lower = 0x8001
  ,SInt16Upper = 0x7fff
  ,SInt32Lower = 0x80000001
  ,SInt32Upper = 0x7fffffff

  ,ByteLower   = 0x00
  ,ByteUpper   = 0xff

  ,ShortLower  = 0x8001
  ,ShortUpper  = 0x7fff

  ,IntLower    = 0x80000001
  ,IntUpper    = 0x7fffffff

  ,CharLower   = 0x00000000
  ,CharUpper   = 0x0010ffff

}; } // NamespaceEnum Limits

//----------------------------------------------------------------------
//
// BEFE Floating Point -> C++ mapping
//

typedef float  Float;       // ◄─┬─── Yuck! How we hate native Floating Points...
typedef double Double;      // ◄─┘    The whole implementation/concept sucks!!!!

} // ...Namespace BEFE
  
//----------------------------------------------------------------------
//
// Other headers used everwhere at the "lower level" includes
//   (as opposed to the "upper level" in BEFE.h)
//

#include "Null.h"
#include "Error.h"
#include "Buffer.h"
#include "Storage.h"
#include "Char.h"
#include "_String.h"
#include "Array.h"
#include "Range.h"
#include "URange.h"
#include "Span.h"
#include "USpan.h"
#include "Slice.h"
#include "LifeCycle.h"

#include "_SanityCheck.h"

#endif // ...PRIMITIVES_H
