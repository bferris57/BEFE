//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: TestContains.h - Declarations for the TestContains Class
//----------------------------------------------------------------------
//
// This Class implements the BEFE Containment Tests.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TESTCONTAINS_H // TESTCONTAINS_H...
#define TESTCONTAINS_H

#include "Primitives.h"
#include "Test.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// TestContains Class
//

class TestContains : public Test { // Class TestContains...

  public: Status Run();
  public: Status testContains();
  
}; // ...Class TestContains

} // ...Namespace BEFE

#endif // ...TESTCONTAINS_H
