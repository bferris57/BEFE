//----------------------------------------------------------------------
// File: _Ole32.cpp - Wrapper for Windows ole32.dll functions
//----------------------------------------------------------------------
//
// This is here because MinGW doesn't provide, by default, the
// libole32 library, you have to specify it when linking.  So, to keep
// people that link with us from having to change their link parameters,
// we implement the wrappers ourselves.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Win32.h"

namespace BEFE { // Namespace BEFE...

HMODULE dllOle32 = NULL;

FuncCoCreateGuid funcCoCreateGuid = NULL;

static HMODULE LoadDllOle32() {

  Int oldfg;

  if (!dllOle32) { // DLL not loaded...

    dllOle32 = LoadLibrary("ole32");
    if (IsNull(dllOle32)) {
      oldfg = Cout.SetForegroundColour(Console::Red);
      Cout << "!!! Windows Error " << (Int)GetLastError() << " loading ole32.dll\n";
      Cout.SetForegroundColour(oldfg);
    }
    else {
      oldfg = Cout.SetForegroundColour(Console::Red);

      funcCoCreateGuid = (FuncCoCreateGuid)GetProcAddress(dllOle32,"CoCreateGuid");
      if (!funcCoCreateGuid)
        Cout << "!!! Windows Error " << (Int)GetLastError() << " loading CoCreateGuid\n";

      Cout.SetForegroundColour(oldfg);
    }

  } // ...DLL not loaded

  return dllOle32;
}

HRESULT CoCreateGuid(GUID *guid) {

  HRESULT result;

  result = ERROR_NO_DATA;
  if (LoadDllOle32() && funcCoCreateGuid)
    result = (*funcCoCreateGuid)(guid);
  return result;

}

} // ...Namespace BEFE
