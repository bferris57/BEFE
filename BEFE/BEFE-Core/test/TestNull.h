//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: TestNull.h - Declarations for the TestNull Class
//----------------------------------------------------------------------
//
// This Class implements the BEFE Null Tests.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TESTNULL_H // TESTNULL_H...
#define TESTNULL_H

#include "Primitives.h"
#include "Test.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// TestNull Class
//

class TestNull : public Test { // Class TestNull...

  public: Status Run         ();
  public: Status testIsNull  ();
  public: Status testSetNull ();
  
}; // ...Class TestNull

} // ...Namespace BEFE

#endif // ...TESTNULL_H
