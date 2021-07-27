//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: TestBcmd.h - Declarations for the TestBcmd Class
//----------------------------------------------------------------------
//
// The TestNumber Class tests the functionality of .bcmd language.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TESTBCMD_H // TESTBCMD_H...
#define TESTBCMD_H

#include "Primitives.h"
#include "Test.h"
#include "Bcmd.h"

namespace BEFE { // Namespace BEFE...

class TestBcmd : public Test { // Class TestBcmd...

  // Class members
  static char const *typeNames[];

  // Internal methods...
  protected: Status InitialiseInput(Bcmd &bcmd);
  protected: Status DisplayArg(UInt argNo, BcmdArg &arg);
  protected: Status DisplayCommand(UInt cmdNo, BcmdCommand &cmd);

  // "Normal" methods...
  public: Status Run();
  public: Status testCommand();
  public: Status testArgument();
  public: Status testPrompt();

}; // ...Class TestBcmd

} // ...Namespace BEFE

#endif // ...TESTBCMD_H
