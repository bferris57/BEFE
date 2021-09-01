//!befe-publish inc
//----------------------------------------------------------------------
// File: Null.h - BEFE Null/NaN Declarations
//----------------------------------------------------------------------
//
// This header file declares, in one place, all the various methods
// associated with "Nullness" in BEFE.
//
// Note: Although we've liberally declared NaN, and its close friends,
//       in Primitives.h, we've pulled all the NaNs together here under
//       the same concept as NULL.
//
//       The reason we've done this is because, as far as we're 
//       concerned, can be modelled in the same way as Null.
//
//       This allows us to get away from using all the NaN flavours
//       throughout our code and simply stick with "Null" instead.
//
// *** See technical documentation for a more detailed description ***
//
// Note: The general Primitive IsNull and SetNull functions aren't
//       declared here.  You'll find them in LifeCycle.h instead because
//       that way they're near their Value IsNull and SetNull friends.
//
// Note: Even though this is all about Nulls and Nullness, we've also
//       thrown in special case Boolean stuff about True, False,
//       IsTrue(), IsFalse(), et cetera along with IsNull(Boolean)
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef NULL_H // NULL_H...
#define NULL_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// BEFE Integer NaN definitions
//
// For Code that wants to define NaN (Not a Number) for integers
// we've provided the following so that all code will be referring to
// the same implementation of NaN.
//
// What we've done is taken advantage of the fact that there's one
// more negative number in a given size Int and used that for
// NaN.
//
// The problem comes with the subtle difference between signed and
// unsigned integers.  The "extra negative number" is different...
//
// For signed integers it's 0x80000000
//
// For unsigned integers it's 0xffffffff (or negative zero for those
// rare few who like one's complement arithmetic over two's complement
// arithmetic.
//
// This difference between signed and unsignd NaN is due to the fact
// that with unsigned values (like addresses), the value 0x800000 just
// gets in the way on your way from 0x00000000 to 0xffffffff.  
//
// Not only that, but specifal semantics have been applied to the value
// zero for unsigned things -- most commonly in one's complement
// arithmetic where there's a "negative zero"!!!
//
// So, we decided to think of "NaN" in the unsigned world as a one's
// complement of zero (that "negative zero").
//
// So there you have it, in the unsigned world we're presenting here,
// there's no 0x80000000 "in the way" for addressing!
//

// Int8 NaN...
enum : unsigned char      { UInt8NaN = 0xffu       };
enum : unsigned char      { UByteNaN = UInt8NaN    };
enum : signed   char      { SInt8NaN = (SInt8)0x80 };
enum : signed   char      { SByteNaN = SInt8NaN    };
enum : signed   char      { NaN8     = SInt8NaN    };
enum : signed   char      { ByteNaN  = SInt8NaN    };

// Int16 NaN...
enum : unsigned short     { UInt16NaN = 0xffffu               , UShortNaN = UInt16NaN };
enum : signed   short     { SInt16NaN = (SInt16)0x8000        , SShortNan = SInt16NaN };
enum : signed   short     { NaN16     = SInt16NaN             , ShortNaN  = SInt16NaN };

// Int32 NaN...
enum : unsigned int       { UInt32NaN = 0xffffffffu           , UIntNaN   = UInt32NaN , UNaN = UInt32NaN};
enum : signed   int       { SInt32NaN = (SInt32)0x80000000    , SIntNaN   = SInt32NaN , SNaN = SInt32NaN};
enum : signed   int       { NaN32     = SInt32NaN             , IntNaN    = SInt32NaN , NaN  = NaN32 };

// Int64 NaN...
enum : unsigned long long { UInt64NaN = 0xffffffffffffffffULL };
enum : signed long long   { SInt64NaN = (SInt64)0x8000000000000000ULL }; // ◄── Compiler demands "manual" typecast
enum : signed long long   { NaN64     = (SInt64)SInt64NaN     }; // ◄── Compiler demands "manual" typecast
enum : unsigned long long { UNaN64    = UInt64NaN             };
enum : signed long long   { NaNLong   = (SInt64)SInt64NaN     }; // ◄── Compiler demands "manual" typecast
enum : signed long long   { LongNaN   = (SInt64)SInt64NaN     }; // ◄── Compiler demands "manual" typecast
enum : unsigned long long { ULongNaN  = UInt64NaN             };
enum : signed long long   { SLongNan  = (SInt64)SInt64NaN     }; // ◄── Compiler demands "manual" typecast

//
// IsNaN functions...
//
// Note: We recommend using IsNull instead of IsNaN if for now other
//       reason than it's easier to type AND it's easier on the eye.
//       But, if you want to think of NaN differently than Null, then
//       feel free to use the NaN functions in place of them.

// General Integer IsNaN...
BEFE_INLINE Boolean IsNaN (Byte   const &b) {return b == UInt8NaN; };
BEFE_INLINE Boolean IsNaN (SByte  const &b) {return b == SInt8NaN; };
BEFE_INLINE Boolean IsNaN (Short  const &s) {return s == SInt16NaN;};
BEFE_INLINE Boolean IsNaN (UShort const &s) {return s == UInt16NaN;};
BEFE_INLINE Boolean IsNaN (Int    const &i) {return i == SInt32NaN;};
BEFE_INLINE Boolean IsNaN (UInt   const &i) {return i == UInt32NaN;};
BEFE_INLINE Boolean IsNaN (Long   const &l) {return l == (Long)SInt64NaN;};
BEFE_INLINE Boolean IsNaN (ULong  const &l) {return l == UInt64NaN;};
BEFE_INLINE Boolean IsNaN (void *l)         {return (l != 0);};

//
// SetNull/SetNaN functions...
//

// General Integer SetNaN...
BEFE_INLINE void SetNaN (Byte   &b) { b = UInt8NaN; };
BEFE_INLINE void SetNaN (SByte  &b) { b = SInt8NaN; };
BEFE_INLINE void SetNaN (Short  &s) { s = SInt16NaN;};
BEFE_INLINE void SetNaN (UShort &s) { s = UInt16NaN;};
BEFE_INLINE void SetNaN (Int    &i) { i = SInt32NaN;};
BEFE_INLINE void SetNaN (UInt   &i) { i = UInt32NaN;};
BEFE_INLINE void SetNaN (Long   &l) { l = SInt64NaN;};
BEFE_INLINE void SetNaN (ULong  &l) { l = UInt64NaN;};

//----------------------------------------------------------------------
//
// BEFE Bool NaN, True, and false definitions
//

enum : UInt8 {
  False = 0,
  True  = 1
};

// NOTE: We have to return Int32 instead of Status in some of the functions
//       below because Status isn't declared yet.  Grrr...
BEFE_INLINE Boolean IsNull(Ptr v)             { return (v == 0 || v == (void *)-1); };
BEFE_INLINE void    SetNull(Ptr *v)           { *v = (PtrInt)NULL; };
BEFE_INLINE Boolean IsNull(char const *ptr)   { return ptr == 0 || ptr == (char const *)-1;}
BEFE_INLINE Int32   SetNull(char const *&ptr) { ptr = 0; return 0;}
BEFE_INLINE Boolean IsNull(Byte *ptr)         { return ptr == 0 || ptr == (Byte *) -1;}
BEFE_INLINE Int32   SetNull(Byte *&ptr)       { ptr = 0; return 0;}

BEFE_INLINE Boolean IsFalse(Boolean b)        { return (b == False);}
BEFE_INLINE Boolean IsTrue(Boolean b)         { return (b == True);}
BEFE_INLINE Boolean IsNull(Boolean b)         { return (b != False && b != True);}
BEFE_INLINE void    SetNull(Boolean &b)       { b = 0xFFu;}
BEFE_INLINE void    SetFalse(Boolean &b)      { b = False;}
BEFE_INLINE void    SetTrue(Boolean &b)       { b = True;}

BEFE_INLINE Boolean IsNull(Int8 b)            { return (b == SInt8NaN);}
BEFE_INLINE void    SetNull(Int8 &b)          { b = SInt8NaN;}
BEFE_INLINE Boolean IsNull(UInt8 b)           { return (b == UInt8NaN);}
BEFE_INLINE void    SetNull(UInt8 &b)         { b = UInt8NaN;}

BEFE_INLINE Boolean IsNull(Int16 b)           { return (b == SInt16NaN);}
BEFE_INLINE void    SetNull(Int16 &b)         { b = SInt16NaN;}
BEFE_INLINE Boolean IsNull(UInt16 b)          { return (b == UInt16NaN);}
BEFE_INLINE void    SetNull(UInt16 &b)        { b = UInt16NaN;}

BEFE_INLINE Boolean IsNull(Int32 b)           { return (b == SInt32NaN);}
BEFE_INLINE void    SetNull(Int32 &b)         { b = SInt32NaN;}
BEFE_INLINE Boolean IsNull(UInt32 b)          { return (b == UInt32NaN);}
BEFE_INLINE void    SetNull(UInt32 &b)        { b = UInt32NaN;}

BEFE_INLINE Boolean IsNull(Int64 b)           { return (b == SInt64NaN);}
BEFE_INLINE void    SetNull(Int64 &b)         { b = SInt64NaN;}
BEFE_INLINE Boolean IsNull(UInt64 b)          { return (b == UInt64NaN);}
BEFE_INLINE void    SetNull(UInt64 &b)        { b = UInt64NaN;}


} // ...Namespace BEFE

#endif // ...NULL_H
