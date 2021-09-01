//!befe-publish inc
//----------------------------------------------------------------------
// File: Numbers.h - Declarations for the Numbers class
//----------------------------------------------------------------------
//
// The Numbers class implements an array of Numbers.  Each instance of
// this class is a variable length array of Numbers.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef NUMBERS_H // NUMBERS_H...
#define NUMBERS_H

#include "Primitives.h"
#include "Array.h"
#include "Number.h"

namespace BEFE { // Namespace BEFE...

class Numbers : public ArrayValue<Number> { // Class Numbers...

  // Finding
  public: Boolean  Contains(Number const &str);
  
}; // ...Class Numbers

//----------------------------------------------------------------------
//
// Commonly used Numbers functions
//

BEFE_INLINE Boolean IsNull  (Numbers const &t) {return ((Numbers *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (Numbers &t)       {return ((Numbers *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (Numbers const &t) {return ((Numbers *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(Numbers &t)       {return ((Numbers *)&t)->SetEmpty();}

} // ...Namespace BEFE

#endif // ...NUMBERS_H
