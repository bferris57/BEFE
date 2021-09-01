//!befe-publish src
//----------------------------------------------------------------------
// File: TestUUID.cpp - Implementation of the TestUUID Class
//----------------------------------------------------------------------
//
// This Class implements the BEFE UUID Class Unit Tests.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Test.h"
#include "TestUTF8.h"

namespace BEFE { // Namespace BEFE...

Status TestUUID::Run() {

  Status status;
  String uuidStr;
  UUID   uuid;
  UInt   i;
  
  if (!gQuiet)
    out << "***Testing UUID...\n";

  for (i=0; i<10; i++) {
    status = TheBefe->TheOS->NewRandomUUID(uuid);
    if (status) goto SOMEERROR;
    if (gVeryVerbose)
      Cout << "  UUID " << i << " = " << uuid.ToString() << '\n';
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

} // ...Namespace BEFE
