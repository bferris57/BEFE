//!befe-publish inc
//----------------------------------------------------------------------
// File: URange.h - Declarations for the URange class
//----------------------------------------------------------------------
//
// The URange class is a Sequence Foundation class.  As such, it
// appears throughout the product when manipulating sequences,
// especially Ids and Addresses.
//
// *** See technical documentation for a more detailed description ***
//
// Basically, a URange is the same as a Range (in fact, the
// implementation is almost identical) but with one important
// difference...  like Range, it declares an Inclusive range of sequential
// values but, unlike Range, these values cannot be negative.
//
// At first glance this may seem like a silly difference, but it's
// quite important for things like Ids and Address which are, by nature,
// unsigned.
//
// This is because of NaN which is an important concept to keep but,
// with signed values applied to an unsigned Range, it puts NaN smack
// in the middle of the Id or Address space.
//
// So, we're using Unsigned NaN (-1) for NaN, and going full steam
// ahead from there.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef URANGE_H // URANGE_H...
#define URANGE_H

#include "Primitives.h"
#include "Range.h"

namespace BEFE {

// External Class references
class String;

//----------------------------------------------------------------------
//
// Class: URange - Sequence Unsigned Range
//

class URange {

  //
  // Public members
  //

  public: UInt idx1;        // Start of Range (inclusive)
  public: UInt idx2;        // End of Range (inclusive)

  //
  // Lifecycle methods
  //

  // C++ lifecycle
  public: URange();
  public: URange(UInt index1);
  public: URange(UInt index1, UInt index2);
  public: URange(char const *str);
  public: URange(String const &str);
  public: URange(URange const &that);
  public: ~URange() {};
  public: URange &operator=(URange const &that);

  // BEFE Lifecycle
  public: Status  StartUp();
  public: Status  ShutDown() {return Error::None;};
  public: Status  Reset()    {return StartUp();}
  public: Boolean IsNull() const;
  public: Status  SetNull()  {return StartUp();}
  public: Boolean IsEmpty() const;
  public: Status  SetEmpty();
  public: Status  CopyFrom(URange const &that);
  public: Status  MoveFrom(URange const &that);
  public: UInt    Length() const;               // Get Number of Elements in Resolved Range
  public: UInt    Size() const;                 // Always returns Zero

  //
  // URange Setting
  //

  public: Status Set();
  public: Status Set(UInt index1);
  public: Status Set(UInt index1, UInt index2);
  public: Status Set(String &str);
  public: Status Set(const char *str);

  //
  // URange Resolving
  //

  public: Boolean IsResolved() const;           // "Is this URange resolved?"
  public: URange  Resolve(UInt len) const;      // Resolve from specified to real URange
  public: Boolean InRange(UInt val) const;      // "Is value in URange?"

  //
  // Range combination
  //

  //public: UInt   Gap(URange that);
  //public: UInt   Overlap(URange that);
  //public: URange Union(URange that);
  //public: URange Intersection(URange that);
  //public: URange Intersects(URange const &that);
  
  //
  // Range String representation
  //

  public: String ToString();
  public: String ToHexString();
  public: Status FromString(String &str);
  public: Status FromString(const char *str);

  //
  // Public static methods
  //

  public: static Status Parse(String const &str, URange &outrange, char const *sep);

};

//----------------------------------------------------------------------
//
// Common Ranges and Range Functions
//

// Ranges
extern Range ControlCharRange;
extern Range ASCIIRange;
extern Range PrintableASCIIRange;
extern Range SByteRange;
extern Range ByteRange;
extern Range UByteRange;
extern Range ShortRange;
extern Range CharRange;
extern Range SIntRange;
extern Range IntRange;
extern Range UIntRange;

// Range Tests
Boolean IsControlCharRange(Int value);
Boolean IsASCIIRange(Int value);
Boolean IsPrintableASCIIRange(Int value);
Boolean IsSByteRange(Int value);
Boolean IsByteRange(Int value);
Boolean IsUByteRangeRange(Int value);
Boolean IsShortRange(Int value);
Boolean IsCharRange(Int value);
Boolean IsIntRange(Int value);

// BEFE Lifecycle
BEFE_INLINE Boolean  IsNull   (URange const &rThis) {return rThis.IsNull();}
BEFE_INLINE Status   SetNull  (URange &rThis)       {return rThis.IsNull();}

} // ...Namespace BEFE

#endif // ...URANGE_H