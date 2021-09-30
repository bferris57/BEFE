//!befe-publish src
//----------------------------------------------------------------------
// File: trythis.cpp - Transient test code
//----------------------------------------------------------------------
//
// The trythis() function is a general purpose 'whipping boy' for
// use during BEFE development.  It is hooked into the Unit Tests
// with the TestType::TryThis type and into the BEFE Command Shell
// as the 't' or 'TryThis' test to invoke it from the command line.
//
// So, the following command line should invoke this function...
//
//   befe +tests trythis
//
//     or
//
//   befe +tests t
//
// Typically, it contains the latest things we're trying or the
// beginnings of test code that will eventually end up in a Unit Test
// in the future.
//
// We've provided it here in the delivered examples so you can
// plug your own stuff into the Unit Tests and try them from
// "inside BEFE" instead of "outside BEFE".
//
// Note: The comments in trythis() can be uncommented if the body
//       is empty.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------

#include "BEFE.h"
#include "BEFEWrappers.h"
#include "BEFEMacros.h"

#include "Globals.h"

#include <stddef.h>       // For offsetof()

// For LexLangTest...
#include "LexFormat.h"
#include "LexLang.h"
#include "LexStream.h"
// ...For LexLangTest

// For trythis()...
#include "ToString.h"
#include "Time.h"
#include "CommandShell.h"
#include "IntFuncs.h"
// ...For

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// trythis
//

void ErrorMessage(String const &str) {

  UInt   oldcolour;

  if (Cout.GetCursor().x != 0)
    Cout << '\n';
  oldcolour = Cout.SetForegroundColour(Console::Red);
  Cout << str << '\n';
  Cout.SetForegroundColour(oldcolour);

  return;

}

Status trythis() {

  Boolean oldWarn = gShowWarnings;

  gShowWarnings = true;
  LoadErrorMessages();
  gShowWarnings = oldWarn;

  return Error::None;

}

} // Namespace BEFE

