//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: TestArray.h - Declarations for the TestArray class
//----------------------------------------------------------------------
//
// The TestArray Class tests various aspects of Array behaviour.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TESTARRAY_H // TESTARRAY_H...
#define TESTARRAY_H

#include "Primitives.h"
#include "Test.h"
#include "Array.h"

namespace BEFE { // Namespace BEFE...

class TestArray : public Test { // Class TestArray...

  public: enum Which {
     TestByte
    ,TestShort
    ,TestInt
    ,TestLong
    ,NUMTESTS
  };

  public: Status Run();
  public: Status test(UInt which, void *theArray);
  public: Status testArrayShort();
  public: Status testArrayInt();
  public: Status testArrayLong();
  public: Status testStrings();
  public: Status testNumbers();
  public: Status testBytes();
  public: Status testSparse1();
  public: Status testSparse2();
  public: Status testRemove();
  public: Status testCopy();
  public: Status testReverse();
  public: Status testUIntMap();
  public: Status testStringMap();
  public: Status testSort();
  public: Status testNamedStrings();
  
}; // ...Class TestArray

} // ...Namespace BEFE

#endif // ...TESTARRAY_H
