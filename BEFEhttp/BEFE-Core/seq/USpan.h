//!befe-publish inc
//----------------------------------------------------------------------
// File: USpan.h - Declarations for the USpan class
//----------------------------------------------------------------------
//
// This class is a sequence foundation class.  As such, it appears
// throughout the product when manipulating sequences.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef USPAN_H // USPAN_H...
#define USPAN_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class: USpan - Sequence USpan
//

class USpan {

  //
  // Public members
  //

  public: UInt  idx1;       // Start of Span (exclusive)
  public: UInt  idx2;       // End of Span (exclusive)
  
  //
  // Lifecycle methods
  //

  // C++ lifecycle
  public: USpan();
  public: USpan(UInt index1);
  public: USpan(UInt index1, UInt index2);
  public: USpan(char const *str);
  public: USpan(String const &str);
  public: USpan(USpan const &that);
  public: ~USpan() {};
  public: USpan &operator=(USpan const &that);

  // BEFE Lifecycle
  public: Status  StartUp();
  public: Status  ShutDown() {return Error::None;};
  public: Status  Reset()    {return StartUp();}
  public: Boolean IsNull() const;
  public: Status  SetNull()  {return StartUp();}
  public: Boolean IsEmpty() const;
  public: Status  SetEmpty();
  public: Status  CopyFrom(USpan const &that);
  public: Status  MoveFrom(USpan const &that);
  public: UInt    Length() const;               // Get Number of Elements in Resolved Range
  public: UInt    Size() const;                 // Always returns Zero

  //
  // USpan Setting
  //

  public: Status Set(String const &str);
  public: Status Set(const char *str);

  //
  // USpan Resolving
  //

  public: USpan   Resolve(UInt len) const;      // Resolve from specified to real Span
  public: Boolean InSpan(UInt val) const;       // "Is value in Span?"

  //
  // USpan String representation
  //

  public: String ToString();
  public: String ToHexString();
  public: Status FromString(String const &str);
  public: Status FromString(const char *str);

};

//----------------------------------------------------------------------
//
// USpan BEFE Lifecycle...
//

// BEFE Lifecycle
BEFE_INLINE Boolean  IsNull   (USpan const &rThis) {return rThis.IsNull();}
BEFE_INLINE Status   SetNull  (USpan &rThis)       {return rThis.IsNull();}
BEFE_INLINE Boolean  IsEmpty  (USpan const &rThis) {return rThis.IsEmpty();}
BEFE_INLINE Status   SetEmpty (USpan &rThis)       {return rThis.SetEmpty();}

} // ...Namespace BEFE

#endif // ...USPAN_H