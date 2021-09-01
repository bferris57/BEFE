//!befe-publish inc
//----------------------------------------------------------------------
// File: Range.h - Declarations for the Range class
//----------------------------------------------------------------------
//
// This class is a Sequence Foundation class.  As such, it appears
// throughout the product when manipulating sequences.
//
// Basically, a Range declares an Inclusive range of sequential values.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef RANGE_H // RANGE_H...
#define RANGE_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

// External Class references
class String;

//----------------------------------------------------------------------
//
// Class: Range - Sequence Range
//
//----------------------------------------------------------------------

class Range {

  //
  // Public members
  //

  public: Int  idx1;        // Start of Range (inclusive)
  public: Int  idx2;        // End of Range (inclusive)

  //
  // Lifecycle methods
  //

  // C++ lifecycle
  public: Range();
  public: Range(Int index1);
  public: Range(Int index1, Int index2);
  public: Range(char const *str);
  public: Range(String const &str);
  public: Range(Range const &that);
  public: ~Range() {};
  public: Range &operator=(Range const &that);

  // BEFE Lifecycle
  public: Status  StartUp();
  public: Status  ShutDown() {return Error::None;};
  public: Status  Reset()    {return StartUp();}
  public: Boolean IsNull() const;
  public: Status  SetNull()  {return StartUp();}
  public: Boolean IsEmpty() const;
  public: Status  SetEmpty();
  public: Status  CopyFrom(Range const &that);
  public: Status  MoveFrom(Range const &that);
  public: UInt    Length() const;               // Get Number of Elements in Resolved Range
  public: UInt    Size() const;                 // Always returns Zero
  
  //
  // Range Setting
  //

  public: Status Set();
  public: Status Set(Int index1);
  public: Status Set(Int index1, Int index2);
  public: Status Set(String const &str);
  public: Status Set(const char *str);

  //
  // Range Resolving
  //

  public: Boolean IsResolved() const;           // "Is this Range resolved?"
  public: Range   Resolve(UInt len) const;      // Resolve from specified to real Range
  public: Boolean InRange(Int val) const;       // "Is value in Range?"
  
  //
  // Resolved Range Order
  //
  
  public: Boolean IsAscending() const;          // "Is Range in Ascending/Forward order?"
  public: Boolean IsDescending() const;         // "Is Range in Descending/Reverse order?"
  public: Status  Reverse();                    // Reverse Range Order
  
  //
  // Range combination
  //

  //public: UInt  Gap(Range that) const;
  //public: UInt  Overlap(Range that) const;
  //public: Range Union(Range that) const;
  //public: Range Intersection(Range that) const;

  //
  // Range String representation
  //

  public: String ToString();
  public: String ToHexString();
  public: Status FromString(String const &str);
  public: Status FromString(const char *str);

  //
  // Public static methods
  //

  public: static Status Parse(String const &str,Range &outRange,char const *sepChars);

};

//----------------------------------------------------------------------
//
// Common Ranges and BEFE Range Functions
//
//----------------------------------------------------------------------

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
BEFE_INLINE Boolean  IsNull   (Range const &rThis) {return rThis.IsNull();}
BEFE_INLINE Status   SetNull  (Range &rThis)       {return rThis.IsNull();}

} // ...Namespace BEFE

#endif // ...RANGE_H