//!befe-publish inc
//----------------------------------------------------------------------
// File: Slice.h - Declarations for the Slice class
//----------------------------------------------------------------------
//
// This class is a sequence foundation class.  As such, it appears
// throughout the product when manipulating sequences.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef SLICE_H // SLICE_H...
#define SLICE_H

#include "Primitives.h"
#include "Span.h"
#include "USpan.h"

namespace BEFE {

// Forward class declarations
class String;

//----------------------------------------------------------------------
//
// Class: Slice - Sequence slice
//

class Slice {

  //
  // Public members
  //

  public: Int idx1;         // Start of slice (inclusive)
  public: Int idx2;         // End of slice (exclusive)
  public: Int step;         // Slice step (idx1..idx2, step slice)

  //
  // Lifecycle methods
  //

  // C++ lifecycle
  public: Slice();
  public: Slice(Int index1);
  public: Slice(Int index1, Int index2);
  public: Slice(Int index1, Int index2, Int step);
  public: Slice(char   const *str);
  public: Slice(Range  const &range);
  public: Slice(URange const &range);
  public: Slice(Span   const &span);
  public: Slice(USpan  const &span);
  public: Slice(String const &str);
  public: ~Slice();

  // BEFE lifecycle
  //public: Status StartUp();
  //public: Status Assign(Slice &that);
  //public: Status Copy();
  //public: Status Delete();
  //public: Status MoveFrom(Slice const &that);
  //public: Status ShutDown();

  //
  // Slice Setting
  //

  public: void Set();
  public: void Set(Int index1);
  public: void Set(Int index1, Int index2);
  public: void Set(Int index1, Int index2, Int repeat);
  public: void Set(Range  range);
  public: void Set(URange range);
  public: void Set(Span   span);
  public: void Set(USpan  span);

  //
  // Slice Resolving
  //

  public: Slice   Resolve(UInt len) const; // Resolve from specified to real slice
  public: UInt    Length() const;          // Get number of elements in resolved slice
  public: Boolean InSlice(Int num) const; // "Is num in slice?"

  //
  // Slice String representation
  //

  public: String ToString() const;
  public: Status FromString(String const &str);
  public: Status FromString(char const *str);

  //
  // Public static methods
  //

  public: static Status Parse(String const &str,Slice &outslice, Char sdelim, Char edelim);

};

} // ...Namespace BEFE

#endif // ...SLICE_H