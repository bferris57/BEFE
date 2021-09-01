//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: TestLexicon.h - Declarations for the TestLexicon Class
//----------------------------------------------------------------------
//
// The TestLexicon Class tests the functionality of Lexical Analysis
// related classes.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TESTLEXICON_H // TESTLEXICON_H...
#define TESTLEXICON_H

#include "Primitives.h"
#include "Test.h"
#include "MinExp.h"

namespace BEFE { // Namespace BEFE...

class TestLexicon : public Test { // Class TestLexicon...

  public: Status Run();

}; // ...Class TestLexicon

}; // ...Namespace BEFE

#endif // ...TESTLEXICON_H
