//!befe-publish src
//----------------------------------------------------------------------
// File: testIdspace.cpp - TestIdSpace Class (tests IdSpace Class)
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
#include "ToStrings.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// TestConsole methods
//

Status TestIdSpace::Run() { // TestIdSpace.Run...

  Status      status;
  IdSpaceInfo spaceinfo;
  Id          someid;
  Strings     strings;
  String      string;
  UInt        i;

  if (!gQuiet)
    out  << "***Testing IdSpace..." << Endl;

  if (!TheBefe->TheIdSpace) goto NOIDSPACE;

  //
  // See if IdSpace 0 exists...
  //

  someid = 0;
  status = IdSpace::GetIdSpaceInfo(someid,spaceinfo);
  if (status) goto NOIDSPACEZERO;

  // It does, display it
  if (gVeryVerbose) {
    strings = ToStrings(spaceinfo);
    out << Endl;
    out << "  IdSpace of IdSpaces BEFORE TEST...\n";
    for (i=0;i<strings.Length();i++) {
      string = strings.Get(i);
      out << "    " << string << Endl;
    }
    out << Endl;
  }

  //
  // Now, try creaating a new one...
  //

  status = IdSpace::New(String("TestSpace"),String("Created by TestIdSpace.Run"),someid);
  if (status) goto NEWFAILED;

  // Get it's info and display it...
  status = IdSpace::GetIdSpaceInfo(someid,spaceinfo);
  if (status) goto SOMEERROR;

  // It does, display it
  if (gVeryVerbose) {
    strings = ToStrings(spaceinfo);
    out << Endl;
    out << "  New test IdSpace...\n";
    for (i=0;i<strings.Length();i++) {
      string = strings.Get(i);
      out << "    " << string << Endl;
    }
    out << Endl;
  }

  //
  // Display IdSpace 0 again...
  //

  status = IdSpace::GetIdSpaceInfo(Id(0),spaceinfo);
  if (status) goto NOIDSPACEZERO;

  // It does, display it
  if (gVeryVerbose) {
    strings = ToStrings(spaceinfo);
    out << Endl;
    out << "  IdSpace of IdSpaces AFTER TEST...\n";
    for (i=0;i<strings.Length();i++) {
      string = strings.Get(i);
      out << "    " << string << Endl;
    }
    out << Endl;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    NOIDSPACE:     BEFE_WARN("IdSpace Class not initialised");
                   status = Error::IdSpaceNotInitialised;
                   break;
    NOIDSPACEZERO: BEFE_WARN("Problem retrieving IdSpace Zero IdSpaceInfo");
                   out << ErrorString(status) << Endl;
                   break;
    NEWFAILED:     BEFE_WARN("Error creating new IdSpace");
                   goto SHOWERROR;
    SOMEERROR:     BEFE_WARN("Error performing test");
    SHOWERROR:     out << "... " << ErrorString(status) << "\n";
                   break;
  }

  return status;

} // ...testIdSpace.Run

} // ...Namespace BEFE
