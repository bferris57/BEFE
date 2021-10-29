//----------------------------------------------------------------------
// File: win32utils.cpp - Various handy Windows32 utility functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// Functions: LinuxGetProcessExecutableName - Return current executable full path
//            LinuxGetPhysicalMemory        - Return physical memory (in bytes)
//
//            LinuxGetDeviceNames           - Return list of current device names
//            LinuxGetVolumeNames           - Return list of mounted volume names
//            LinuxGetVolumePaths           - Return list of mounted volume paths
//
//            LinuxGetReservedDevices       - Return list of reserved device names
//            LinuxGetLogicalDrives         - Return list of currently mapped logical drives
//            LinuxGetDeviceName            - Return name given device number (see below)
//            LinuxGetDeviceByName          - Return device number (see below) by name
//            LinuxGetDeviceDescription     - Given device name, Return device description
//
//            LinuxGetHostAndDomains        - Get network host and domain names
//            LinuxGetDNSDomains            - Get list of known DNS domains
//            LinuxGetMACAddresses          - Get list of available MAC addresses
//            LinuxGetIPv4Addresses         - Get list of available host IPv4 addresses
//            LinuxGetIPv6Addresses         - Get list of available host IPv6 addresses
//            LinuxFormatMACAddress         - Format Windows 6 byte MAC address
//
//            LinuxGetStorageFreeSpace      - Return free space info about a storage device
//
//----------------------------------------------------------------------
// Copyright (©) 2021, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "_GlobalMacros.h"
#include "Linux.h"
#include "Storage.h"

#include <unistd.h> // For getpagesize
#include <limits.h> // For MAX_PATH


namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// General purpose functions
//

/* TEMP...
Status LinuxGetProcessExecutableName(String &thestring) {

  DWORD   processid;
  HANDLE  hprocess;
  HMODULE hmodule;
  DWORD   cbneeded;
  char    processname[256];
  char    processfile[256];

  processid = GetCurrentProcessId();
  hprocess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processid );

  if (!hprocess) {
    if (!gQuiet)
      Cout << "GetProcessExecutableName: *** Unable to get process info ***" << Endl;
    goto DONE;
  }
  if (! BEFE::EnumProcessModules( hprocess, &hmodule, sizeof(hmodule),&cbneeded) ) {
    if (!gQuiet)
      Cout << "GetProcessExecutableName: ***Unable to get module info***" << Endl;
    goto DONE;
  }

  BEFE::GetModuleBaseName( hprocess, hmodule, processname, sizeof(processname));
  GetModuleFileName( hmodule, processfile, sizeof(processfile));

  thestring = processfile;

  // Handle termination conditions...
  while (false) {
    DONE: break;
  }

  return Error::None;

}

Long LinuxGetPhysicalMemory() {

  MEMORYSTATUS statex;

  statex.dwLength = sizeof (statex);
  GlobalMemoryStatus (&statex);

  return (Long)statex.dwTotalPhys;

}

UInt LinuxGetMemoryPageSize() {
  
  SYSTEM_INFO sysInfo;
  
  GetSystemInfo(&sysInfo);
  
  return (UInt)getpagesize();
  
}

Boolean LinuxMemoryIsReadable(void *addr) {
  

  Boolean                  answer;
  MEMORY_BASIC_INFORMATION memInfo;
  UInt                     numBytes;
  URange                   allocRange;

  numBytes = VirtualQuery(addr, &memInfo, sizeof(memInfo));

  if (numBytes == 0) goto NOPE;
  if (memInfo.Protect != PAGE_READONLY && memInfo.Protect != PAGE_READWRITE) goto NOPE;

  allocRange = URange((UInt)memInfo.BaseAddress,(UInt)memInfo.BaseAddress+memInfo.RegionSize-1);
  if (!allocRange.InRange((UInt)addr)) goto NOPE;

  // Handle errors
  answer = true;
  while (false) {
    NOPE: answer = false;
  }
  
  return answer;

}

Boolean LinuxMemoryIsWritable(void *addr) {

  Boolean                  answer;
  MEMORY_BASIC_INFORMATION memInfo;
  UInt                     numBytes;
  URange                   allocRange;

  numBytes = VirtualQuery(addr, &memInfo, sizeof(memInfo));

  if (numBytes == 0) goto NOPE;
  if (memInfo.Protect != PAGE_READWRITE && memInfo.Protect != PAGE_WRITECOPY) goto NOPE;

  allocRange = URange((UInt)memInfo.BaseAddress,(UInt)memInfo.BaseAddress+memInfo.RegionSize-1);
  if (!allocRange.InRange((UInt)addr)) goto NOPE;

  // Handle errors
  answer = true;
  while (false) {
    NOPE: answer = false;
  }
  
  return answer;

}

//----------------------------------------------------------------------
//
// Device/Volume functions
//

Status LinuxGetVolumeNames(Strings &names) {

  return Error::NotImplemented;

}

Status LinuxGetVolumePaths(String &volume, Strings &paths) {

  return Error::NotImplemented;

}

Status LinuxGetLogicalDrives(Strings &drives) {

  return Error::NotImplemented;

}

Strings LinuxGetDeviceNames() {

  return Error::NotImplemented;

}

Strings LinuxGetVolumeNames() {
  Strings names;
  return names;
}

//-------------------------------------------------------------------------
//
// Network info functions
//

Status LinuxGetHostAndDomains(String &host, Strings &domains) {

  return Error::NotImplemented;

}

Status LinuxGetDNSDomains(Strings &domains) {

  return Error::NotImplemented;

}

Status LinuxGetMACAddresses(Strings &array) {

    return Error::NotImplemented;

}

Status LinuxGetIPv4Addresses(Strings &array) {

    return Error::NotImplemented;

}

Status LinuxGetIPv6Addresses(Strings &array) {

  return Error::NotImplemented;
}

void LinuxFormatMACAddress(Byte *bytes, int numbytes,String &thestr) {

  return Error::NotImplemented;

}

//-------------------------------------------------------------------------
//
// Storage info functions
//

Status LinuxGetStorageLimits(String devname,Int &unit, Int &nfreeunits, Int &totalunits) {

  return Error::NotImplemented;

}

Status LinuxRandomUUID(UUID &uuid) {
  BEFE::CoCreateGuid((GUID *)&uuid);
  return Error::None;
}

Status LinuxGetCommandLine(String &cmdLine) {

  return Error::NotImplemented;

}
... TEMP */


} // ...Namespace BEFE
