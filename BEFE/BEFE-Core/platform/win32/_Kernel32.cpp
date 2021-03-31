//----------------------------------------------------------------------
// File: _Kernel32api.cpp - Wrapper for Windows kernel32 functions
//----------------------------------------------------------------------
//
// This is here because MinGW doesn't provide, by default, some of the
// kernel32 functions -- THE BASTARDS!!!
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Win32.h"

namespace BEFE {

HMODULE dllKernel32 = NULL;

FuncGetConsoleScreenBufferInfoEx funcGetConsoleScreenBufferInfoEx = NULL;

static HMODULE LoadDllKernel32() {

	char const *name;

  if (!dllKernel32) { // DLL not loaded...

    dllKernel32 = LoadLibrary("kernel32");
    if (IsNull(dllKernel32)) {
      //Cout << "!!! Windows Error " << (Int)GetLastError() << " loading iphlpapi.dll\n";
    }
    else {
      name = "GetConsoleScreenBufferInfoEx";
      funcGetConsoleScreenBufferInfoEx = (FuncGetConsoleScreenBufferInfoEx)GetProcAddress(dllKernel32,name);
    }

  } // ...DLL not loaded

  return dllKernel32;
}

BOOL WINAPI GetConsoleScreenBufferInfoEx(HANDLE hConsoleOutput
                                        ,PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx
                                        ) {

  BOOL result;

  result = 0;
  if (LoadDllKernel32() && funcGetConsoleScreenBufferInfoEx)
    result = (*funcGetConsoleScreenBufferInfoEx)(hConsoleOutput, lpConsoleScreenBufferInfoEx);

  return result;

}

} // Namespace BEFE