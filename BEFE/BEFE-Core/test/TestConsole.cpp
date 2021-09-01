//!befe-publish src
//----------------------------------------------------------------------
// File: testConsole.cpp - TestConsole Class (tests Console Class)
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Test.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// TestConsole methods
//

Status TestConsole::Run() { // TestConsole.Run...

  String errmsg;
  UInt   oldfg;

  if (!gQuiet)
    out  << "***Testing Console..." << Endl;

  // Save original foreground colour
  oldfg = out.GetForegroundColour();

  //
  // Try dicking about with the colours...
  //

  if (gVerbose)
    out << Endl;
  if (!gConsoleColour) {
    if (gVerbose)
      out << "  Console Colour disabled\n";
  }
  else {
    if (gVerbose)
      out << "  Console Colour enabled...\n";
    out.SetForegroundColour(Console::Red);
    if (gVerbose)
      out << "    This should be Red!!!...\n";
    out.SetForegroundColour(Console::White);
    if (gVerbose)
      out << "    This should be White!!!...\n";
    out.SetForegroundColour(Console::Blue);
    if (gVerbose)
      out << "    This should be Blue!!!...\n";
    out.SetForegroundColour(Console::Green);
    if (gVerbose)
      out << "    This should be Green!!!...\n";
    out.SetForegroundColour(oldfg);
    if (gVerbose)
      out << "    ...And this should be back to normal!!!\n";
  }

  //
  // Try setting the UTF-8 code page
  //

  errmsg = "└ ┘ ┬ ┴";
  if (gConsoleUTF8) {
    if (gVerbose) {
      out << "  Trying utf-8... '";
      out << errmsg << "'\n";
    }
  }
  else {
    if (gVerbose) {
      out << "  Console UTF-8 disabled (use -cUTF to enable)\n";
      out << "      ...So this should look like rubbish... '";
      out << errmsg << "'\n";
    }
  }

  //
  // Everything worked
  //

  out.SetForegroundColour(oldfg);

  return Error::None;

} // ...testConsole.Run()

} // ...Namespace BEFE
