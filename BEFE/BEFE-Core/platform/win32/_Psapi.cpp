//----------------------------------------------------------------------
// File: _Psapi.cpp - Wrapper for Windows Psapi functions
//----------------------------------------------------------------------
//
// This is here because MinGW doesn't provide, by default, the
// libpsapi library, you have to specify it when linking.  So, to keep
// people that link with us from having to change their link parameters,
// we implement the wrappers ourselves.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Win32.h"

namespace BEFE { // Namespace BEFE...

HMODULE dllPsapi    = NULL;

FuncEnumProcessModules funcEnumProcessModules = NULL;
FuncGetModuleBaseName  funcGetModuleBaseName  = NULL;

static HMODULE LoadDllPsapi() {

  Int oldfg;

  if (!dllPsapi) { // DLL not loaded...

    dllPsapi = LoadLibrary("psapi");
    if (IsNull(dllPsapi)) {
      oldfg = Cout.SetForegroundColour(Console::Red);
      Cout << "!!! Win32LoadDllPsapi... Error " << (Int)GetLastError() << " loading psapi.dll\n";
      Cout.SetForegroundColour(oldfg);
    }
    else {
      oldfg = Cout.SetForegroundColour(Console::Red);

      funcEnumProcessModules = (FuncEnumProcessModules)GetProcAddress(dllPsapi,"EnumProcessModules");
      if (!funcEnumProcessModules)
        Cout << "!!! Win32LoadDllpsapi... Error " << (Int)GetLastError() << " loading EnumProcessModules\n";

      funcGetModuleBaseName = (FuncGetModuleBaseName)GetProcAddress(dllPsapi,"GetModuleBaseNameA");
      if (!funcGetModuleBaseName)
        Cout << "!!! Win32LoadDllpsapi... Error " << (Int)GetLastError() << " loading GetModuleBaseName\n";

      Cout.SetForegroundColour(oldfg);
    }

  } // ...DLL not loaded

  return dllPsapi;
}

BOOL WINAPI EnumProcessModules(
  HANDLE hProcess,
  HMODULE *lphModule,
  DWORD cb,
  LPDWORD lpcbNeeded
) {

  BOOL result;

  result = false;
  if (LoadDllPsapi() && funcEnumProcessModules)
    result = (*funcEnumProcessModules)(hProcess,lphModule,cb,lpcbNeeded);

  return result;

}

DWORD WINAPI GetModuleBaseName(
  HANDLE  hProcess,
  HMODULE hModule,
  LPTSTR  lpBaseName,
  DWORD   nSize
) {

  DWORD result;

  result = 0;
  if (LoadDllPsapi() && funcGetModuleBaseName)
    result = (*funcGetModuleBaseName)(hProcess,hModule,lpBaseName,nSize);

  return result;

}

} // ...Namespace BEFE
