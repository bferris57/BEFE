//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: TestNumber.h - Declarations for the TestNumbers Class
//----------------------------------------------------------------------
//
// The TestNumber Class tests the functionality of various number
// classes and numeric data type functions.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TESTNUMBER_H // TESTNUMBER_H...
#define TESTNUMBER_H

#include "Primitives.h"
#include "Test.h"
#include "IntFuncs.h"

namespace BEFE { // Namespace BEFE...

class TestNumber : public Test { // Class TestNumber...

  public: Status Run();
  public: Status testBitRange();
  public: Status testRoundToPowerOfTwo();
  public: Status testUInt32FromString();
  public: Status testIntFromString();
  public: Status testIntBitScatter();
  public: Status testOraNumber();
  public: Status testNumber();
  public: Status testNumberCompare();
  public: Status testIntLog2();
  
}; // ...Class TestNumber

} // ...Namespace BEFE

#endif // ...TESTNUMBER_H
