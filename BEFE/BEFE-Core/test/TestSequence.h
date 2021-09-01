//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: TestSequence.h - Declarations for the TestSequence Class
//----------------------------------------------------------------------
//
// The TestNumber Class tests the functionality of various Sequence
// Classes like Alloc, Range, Span, et cetera.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TESTSEQUENCE_H // TESTSEQUENCE_H...
#define TESTSEQUENCE_H

#include "Primitives.h"
#include "Test.h"

namespace BEFE { // Namespace BEFE...

class TestSequence : public Test { // Class TestSequence...

  enum RangeType {
     TypeRange
    ,TypeURange
    ,TypeSpan
    ,TypeUSpan
  };

  public: Status Run();
  public: Status testSlice();
  public: Status testAlloc();
  public: Status testRange();
  public: Status testURange();
  public: Status testSpan();
  public: Status testUSpan();
  public: Status testCycle();
  
  protected: Status testRangeType(UInt rangeType);

}; // ...Class TestSequence

} // ...Namespace BEFE

#endif // ...TESTSEQUENCE_H
