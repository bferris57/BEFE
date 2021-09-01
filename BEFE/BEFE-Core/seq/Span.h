//!befe-publish inc
//----------------------------------------------------------------------
// File: Span.h - Declarations for the Span class
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
#ifndef SPAN_H // SPAN_H...
#define SPAN_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class: Span - Sequence Span
//

class Span {

  //
  // Public members
  //

  public: Int  idx1;        // Start of Span (exclusive)
  public: Int  idx2;        // End of Span (exclusive)
  
  //
  // Lifecycle methods
  //

  // C++ lifecycle
  public: Span();
  public: Span(Int index1);
  public: Span(Int index1, Int index2);
  public: Span(char const *str);
  public: Span(String const &str);
  public: Span(Span const &that);
  public: ~Span() {};
  public: Span &operator=(Span const &that);

  // BEFE Lifecycle
  public: Status  StartUp();
  public: Status  ShutDown() {return Error::None;};
  public: Status  Reset()    {return StartUp();}
  public: Boolean IsNull() const;
  public: Status  SetNull()  {return StartUp();}
  public: Boolean IsEmpty() const;
  public: Status  SetEmpty();
  public: Status  CopyFrom(Span const &that);
  public: Status  MoveFrom(Span const &that);
  public: UInt    Length() const;               // Get Number of Elements in Resolved Range
  public: UInt    Size() const;                 // Always returns Zero

  //
  // Span Setting
  //

  public: Status Set(String const &str);
  public: Status Set(char const *str);

  //
  // Span Resolving
  //

  public: Boolean IsResolved() const;           // "Is this Span resolved?"
  public: Span    Resolve(UInt len) const;      // Resolve from specified to real Span
  public: Boolean InSpan(Int val) const;        // "Is value in Span?"

  //
  // Resolved Span Order
  //
  
  public: Boolean IsAscending() const;          // "Is Span in Ascending/Forward order?"
  public: Boolean IsDescending() const;         // "Is Span in Descending/Reverse order?"
  public: Status  Reverse();                    // Reverse Span Order
  
  //
  // Span String representation
  //

  public: String ToString();
  public: String ToHexString();
  public: Status FromString(String const &str);
  public: Status FromString(const char *str);

};

//----------------------------------------------------------------------
//
// Span BEFE Lifecycle...
//

// BEFE Lifecycle
BEFE_INLINE Boolean  IsNull   (Span const &rThis) {return rThis.IsNull();}
BEFE_INLINE Status   SetNull  (Span &rThis)       {return rThis.IsNull();}
BEFE_INLINE Boolean  IsEmpty  (Span const &rThis) {return rThis.IsEmpty();}
BEFE_INLINE Status   SetEmpty (Span &rThis)       {return rThis.SetEmpty();}

} // Namespace BEFE

#endif // SPAN_H