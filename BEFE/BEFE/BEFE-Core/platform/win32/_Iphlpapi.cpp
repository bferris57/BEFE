//----------------------------------------------------------------------
// File: _Iphlpapi.cpp - Wrapper for Windows Iphlpapi functions
//----------------------------------------------------------------------
//
// This is here because MinGW doesn't provide, by default, the
// libiphlp library, you have to specify it when linking.  So, to keep
// people that link with us from having to change their link parameters,
// we implement the wrappers ourselves.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Win32.h"

namespace BEFE {

HMODULE dllIphlpapi = NULL;

FuncGetNetworkParams funcGetNetworkParams = NULL;
FuncGetAdaptersInfo  funcGetAdaptersInfo  = NULL;

static HMODULE LoadDllIphlpapi() {

  Int oldfg;

  if (!dllIphlpapi) { // DLL not loaded...

    dllIphlpapi = LoadLibrary("iphlpapi");
    if (IsNull(dllIphlpapi)) {
      oldfg = Cout.SetForegroundColour(Console::Red);
      Cout << "!!! Windows Error " << (Int)GetLastError() << " loading iphlpapi.dll\n";
      Cout.SetForegroundColour(oldfg);
    }
    else {
      oldfg = Cout.SetForegroundColour(Console::Red);

      funcGetNetworkParams = (FuncGetNetworkParams)GetProcAddress(dllIphlpapi,"GetNetworkParams");
      if (!funcGetNetworkParams)
        Cout << "!!! Windows Error " << (Int)GetLastError() << " loading GetNetworkParams\n";

      funcGetAdaptersInfo = (FuncGetAdaptersInfo)GetProcAddress(dllIphlpapi,"GetAdaptersInfo");
      if (!funcGetAdaptersInfo)
        Cout << "!!! Windows Error " << (Int)GetLastError() << " loading GetAdaptersInfo\n";

      Cout.SetForegroundColour(oldfg);
    }

  } // ...DLL not loaded

  return dllIphlpapi;
}

DWORD GetNetworkParams(
  PFIXED_INFO pFixedInfo,
  PULONG      pOutBufLen
) {

  DWORD result;

  result = ERROR_NO_DATA;
  if (LoadDllIphlpapi() && funcGetNetworkParams)
    result = (*funcGetNetworkParams)(pFixedInfo,pOutBufLen);
  return result;

}

DWORD GetAdaptersInfo(
  PIP_ADAPTER_INFO pAdapterInfo,
  PULONG pOutBufLen
) {

  DWORD result;

  result = ERROR_NO_DATA;

  if (LoadDllIphlpapi() && funcGetAdaptersInfo)
    result = (*funcGetAdaptersInfo)(pAdapterInfo,pOutBufLen);

  return result;

}

} // Namespace BEFE