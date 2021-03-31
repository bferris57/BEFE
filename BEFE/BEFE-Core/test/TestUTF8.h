//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: TestUTF8.h - Declarations for the TestUTF8 Class
//----------------------------------------------------------------------
//
// This Class implements the BEFE UTF8 Class Unit Tests.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TESTUTF8_H // TESTUTF8_H...
#define TESTUTF8_H

#include "Primitives.h"
#include "Test.h"
#include "TestUTF8.h"
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

class TestUTF8 : public Test { // Class TestUTF8...

  public: Status Run();

  public: Status testCount();
  public: Status testGetChar();
  public: Status testFolding();
  
}; // ...Class TestUTF8

} // ...Namespace BEFE

#endif // ...TESTUTF8_H
