//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: TestString.h - Declarations for the TestString Class
//----------------------------------------------------------------------
//
// This Class implements the BEFE String Class Unit Tests.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TESTSTRING_H // TESTSTRING_H...
#define TESTSTRING_H

#include "Primitives.h"
#include "Test.h"

namespace BEFE { // Namespace BEFE...

class TestString : public Test { // Class TestString...

  public: Status Run();

  public: void testConstruct     ();
  public: void testOpEquals      ();
  public: void testOpPlusEquals  ();
  public: void testOpPlus        ();
  public: void testOpMultiply    ();
  public: void testOpLeftShift   ();
  public: void testOpCompares    ();
  public: void testManip         ();
  public: void testFindChar      (Boolean sensitive);
  public: void testFindString    (Boolean sensitive);
  public: void testSBuffer       ();
  public: void testLogToPhySpan  ();
  public: void testLogToPhyRange ();
  public: void testRemoveRange   ();
  public: void testRemoveSpan    ();
  public: void testInsertChar    ();
  public: void testInsertString  ();
  public: void testReplaceInt    ();
  public: void testReplaceRange  ();
  public: void testReplaceSpan   ();
  public: void testSplit         ();
  public: void testGeneral       ();
  public: void testReverse       ();
  public: void testOpSubscript   ();

}; // ...Class TestString

} // ...Namespace BEFE

#endif // ...TESTSTRING_H
