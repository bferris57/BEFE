//!befe-publish src
//----------------------------------------------------------------------
// File: BuildDisplay.cpp - Build Class Display Methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Build.h"

namespace BEFE { // Namespace BEFE...

Status Build::_DisplaySettings() {
  
  Status  status;
  Strings keys;
  UInt    numKeys;
  UInt    curIdx;
  UInt    maxKeyLen;
  String  key;
  String  value;
  String  sIndent;
  
  NEED_TheBefe
  
  sIndent = String(' ')*indent;
  
  status = settings.Keys(keys);
  if (status) goto SOMEERROR;
  numKeys = keys.Length();
  if (numKeys == 0) {
    Cout << sIndent << "Settings: ***None***\n";
    goto OK;
  }
  status = keys.Sort();
  if (status) goto SOMEERROR;
  
  // Figure out maxumum key length
  maxKeyLen = String("xxx Executable").Length(); // ◄──  See DisplayExternalBinaries below
  for (curIdx=0; curIdx < numKeys; curIdx++) {
    key = keys.Get(curIdx);
    if (key.Length() > maxKeyLen)
      maxKeyLen = key.Length();
  }
  
  // Display the keys...
  for (curIdx=0; curIdx < numKeys; curIdx++) {
    key = keys.Get(curIdx);
    value = settings.Get(key);
    Cout << sIndent << key.Pad(maxKeyLen) << " = '" << value/*.Escape()*/ << "'\n";
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;
  
}

Status Build::_DisplayDependencies() {
  
  String  sIndent;
  Strings keys;
  UInt    i;
  
  NEED_TheBefe

  sIndent = String(' ')*indent;
  
  keys = extBinaries.Keys();
  keys.Sort();
  for (i=0; i<keys.Length(); i++)
    Cout << sIndent << keys.Get(i).Pad(3) << " Executable = '" << extBinaries.Get(keys.Get(i)) << "'\n";
  
  return Error::None;
  
}

} // ...Namespace BEFE