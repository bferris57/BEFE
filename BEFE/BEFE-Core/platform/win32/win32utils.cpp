//----------------------------------------------------------------------
// File: win32utils.cpp - Various handy Windows32 utility functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// Functions: Win32GetProcessExecutableName - Return current executable full path
//            Win32GetPhysicalMemory        - Return physical memory (in bytes)
//
//            Win32GetDeviceNames           - Return list of current device names
//            Win32GetVolumeNames           - Return list of mounted volume names
//            Win32GetVolumePaths           - Return list of mounted volume paths
//
//            Win32GetReservedDevices       - Return list of reserved device names
//            Win32GetLogicalDrives         - Return list of currently mapped logical drives
//            Win32GetDeviceName            - Return name given device number (see below)
//            Win32GetDeviceByName          - Return device number (see below) by name
//            Win32GetDeviceDescription     - Given device name, Return device description
//
//            Win32GetHostAndDomains        - Get network host and domain names
//            Win32GetDNSDomains            - Get list of known DNS domains
//            Win32GetMACAddresses          - Get list of available MAC addresses
//            Win32GetIPv4Addresses         - Get list of available host IPv4 addresses
//            Win32GetIPv6Addresses         - Get list of available host IPv6 addresses
//            Win32FormatMACAddress         - Format Windows 6 byte MAC address
//
//            Win32GetStorageFreeSpace      - Return free space info about a storage device
//
// A note on the arbitrairy way we map device numbers to device names...
//
//   -26..-1 = Drive letters (-1='A:', -2='B:', etc.)
//         0 = NUL
//         1 = Console/Keyboard
//        >1 = Other devices (these are deprecated and shouldn't be used)
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "_GlobalMacros.h"
#include "Win32.h"
#include "Win32WCHAR.h"
#include "Storage.h"

namespace BEFE { // Namespace BEFE...

// NOTE: See http://msdn.microsoft.com/en-us/library/aa365247%28v=vs.85%29.aspx
//       for Microsoft's guidelines we're following below...

const char *Win32ReservedFileChars = "<>:\"/\\|?*";

const char *Win32ReservedDevices[] = {
   "NUL"  ,"CON"  ,"PRN"  ,"AUX"
  ,"COM1" ,"COM2" ,"COM3" ,"COM4" ,"COM5" ,"COM6" ,"COM7" ,"COM8" ,"COM9"
  ,"LPT1" ,"LPT2" ,"LPT3" ,"LPT4" ,"LPT5" ,"LPT6" ,"LPT7" ,"LPT8" ,"LPT9"
  ,NULL };

//----------------------------------------------------------------------
//
// General purpose functions
//

Long Win32FileTimeToTime(FILETIME fileTime) {

  ULARGE_INTEGER uli;
  Long           t;

  uli.LowPart = fileTime.dwLowDateTime; // could use memcpy here!
  uli.HighPart = fileTime.dwHighDateTime;
  t = (Long)uli.QuadPart/10000 - ((Long)Time::EpochDaysSince1601*(1000L*60L*60L*24L));
 
  return t;

}

Status Win32GetProcessExecutableName(String &thestring) {

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

Long Win32GetPhysicalMemory() {

  MEMORYSTATUS statex;

  statex.dwLength = sizeof (statex);
  GlobalMemoryStatus (&statex);

  return (Long)statex.dwTotalPhys;

}

UInt Win32GetMemoryPageSize() {
  
  SYSTEM_INFO sysInfo;
  
  GetSystemInfo(&sysInfo);
  
  return (UInt)sysInfo.dwPageSize;
  
}

Boolean Win32MemoryIsReadable(void *addr) {
  

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

Boolean Win32MemoryIsWritable(void *addr) {

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

Status Win32GetVolumeNames(Strings &names) {

  Status status;              // Current error status (including Win32 ones)

  //Int     charcount;
  char    devname[MAX_PATH];
  HANDLE  fvhand;
  Int     volidx;
  Boolean success;
  char    volname[MAX_PATH];
  String  str;

  //  Enumerate all volumes in the system.
  fvhand = FindFirstVolume(volname, sizeof(volname));
  if (fvhand == INVALID_HANDLE_VALUE) goto BADHANDLE;

  // Until we're finished
  for (;;) {

    //  Skip the \\?\ prefix and remove the trailing backslash.
    volidx = Strlen(volname) - 1;

    if (volname[0] != '\\' || volname[1] != '\\' ||
        volname[2] != '?'  || volname[3] != '\\' ||
        volname[volidx] != '\\') goto BADPATH;

    //  QueryDosDevice does not allow a trailing backslash,
    //  so temporarily remove it.
    volname[volidx] = '\0';

    /*charcount =*/ QueryDosDevice(&volname[4], devname, sizeof(devname));

    volname[volidx] = '\\';

    // Let user know what we're doing
    if (gVeryVerbose) {
      Cout << "Found a device: '" << devname << "'\n";
      Cout << "  Volume name:  '" << volname << "'\n";
    }

    // Add the name to the list
    str.SetEmpty();
    str += volname;
    status = names.Append(str);
    if (status) goto SOMEERROR;

    //DisplayVolumePaths(volname);

    //  Move on to the next volume.
    success = FindNextVolume(fvhand, volname, sizeof(volname));
    if (!success) {
      status = GetLastError();
      if (status != ERROR_NO_MORE_FILES) goto FNVFAILED;
      status = ERROR_SUCCESS;
      break;
    }

  }

  //
  // Handle errors
  //

  status = Error::None;
  while (false) {
    BADHANDLE:  if (gVeryVerbose)
                  Cout << "Win32GetVolumeNames: FindFirstVolume failed\n";
                goto WIN32ERROR;
    BADPATH:    if (gVeryVerbose) {
                  Cout << "Win32GetVolumeNames: FindFirstVolume/FindNextVolume returned a bad path: '";
                  Cout << volname << "'" << Endl;
                }
                break;
    FNVFAILED:  if (gVeryVerbose)
                  Cout << "Win32GetVolumeNames: FindNextVolume failed\n";
                goto WIN32ERROR;
    WIN32ERROR: status = GetLastError();
                if (gVeryVerbose)
                  Cout << "Win32GetVolumeNames: Windows32 error " << status << Endl;
                status = Error::OperatingSystemError;
                break;
    SOMEERROR:  break;
  }

  // Clean up if need be
  if (fvhand != INVALID_HANDLE_VALUE)
    FindVolumeClose(fvhand);

  return status;
}

Status Win32GetVolumePaths(String &volume, Strings &paths) {

  Status status;              // Current error status (including Win32 ones)

  char     volname[MAX_PATH+1];
  UInt     volnamelen;
  UInt     charcount;
  char     names[MAX_PATH+1];
  char    *cp;
  Boolean  success;
  String   path;

  // Unpack the string into our volname char buffer...
  volnamelen = 0;
  status = volume.ToBytes((Byte *)volname,sizeof(volname)-1,volnamelen);
  if (status) goto SOMEERROR;
  volname[volnamelen] = 0;

  // Figure out how many bytes Windows wants to return the info
  success = GetVolumePathNamesForVolumeName(volname, names, 0, (DWORD *)&charcount);
  if (success) goto OK;
  status = GetLastError();
  if (status != ERROR_MORE_DATA) goto INTERNALERROR;

  // Make sure it'll fit in our buffer
  if (charcount > sizeof(names)) goto TRUNCATED;

  // Okay, now get it for real
  success = GetVolumePathNamesForVolumeName(volname, names, sizeof(names), (DWORD *)&charcount);
  if (!success) goto WINERROR;

  //  Append the various paths.
  for (cp=names; *cp != 0; cp += Strlen(cp)+1) {
    path.SetEmpty();
    path += cp;
    paths.Append(path);
  }

  // Handle errors
  status = Error::None;
  while (false) {
    INTERNALERROR: status = Error::Internal;             break;
    TRUNCATED:     status = Error::ValueTruncated;       break;
    WINERROR:      status = Error::OperatingSystemError; break;
    OK:            status = Error::None;                 break;
    SOMEERROR:                                           break;
  }

  return status;

}

Status Win32GetLogicalDrives(Strings &drives) {

  Int      charcount;
  Int      len;
  char     names[MAX_PATH+1];
  char    *cp;
  String   drive;

  // Okay, Get them
  charcount = GetLogicalDriveStrings(sizeof(names),names);

  //  Append the various drives.
  cp = names;
  while (charcount > 0) {
    drive.SetEmpty();
    drive += cp;
    drives.Append(drive);
    len = Strlen(cp)+1;
    charcount -= len;
    cp += len;
  }

  return Error::None;
}

Strings Win32GetDeviceNames() {

  Strings  devices;
  String   device;
  Byte     buf[MAX_PATH];
  Int      numbytes;
  Byte    *devname;
  Int      devnamel;        // <-- Including NUL
  Byte    *cp;
  char   **cpp;

  // Get the NUL terminated logical drive names (list terminated by NUL)
  numbytes = GetLogicalDriveStrings(MAX_PATH-1,(char *)&buf[1]);
  if (numbytes == 0) goto DONE;

  // For each logical drive...
  cp = buf+1;
  while (*cp != 0x00) {
    devname = cp;
    while (*cp != 0x00) cp++;
    // Strip off trailing '\', if there (it should always be)
    if (*(cp-1) == '\\') *(cp-1) = 0x00;
    // Add it to the list
    device = (char *)devname;
    devices.Append(device);
    // Skip over the NUL to the next one
    cp++;
  }

  // For each reserved device name...
  cpp = (char **)Win32ReservedDevices;
  while (!IsNull(*cpp)) {

    // Setup the device name
    Strcpy(buf,*cpp);
    cp = buf + Strlen(buf);
    devnamel = cp - buf + 1;

    // See if it's there and, if so, add it to the list
    // (even though QueryDosDevice on CON says "no", we know it works
    // so use it anyway)
    numbytes = QueryDosDevice((const char *)buf,(char *)buf+devnamel,MAX_PATH-devnamel);
    if (numbytes != 0 || cpp == (char **)Win32ReservedDevices) {
      device = (char *)buf;
      devices.Append(device);
    }

    // Hack for "CON"...
    if (numbytes == 0 && Strcmp(*cpp,"CON") == 0)
      devices.Append(*cpp);

    // Bump pointer to next byte
    cpp++;
  }

  // Handle termination cases...
  while (false) {
    DONE: break;
  }

  return *((Strings *)&devices.Consumable());

}

Strings Win32GetVolumeNames() {
  Strings names;
  return names;
}

//----------------------------------------------------------------------
//
// Function: Win32GetDeviceByName - "Is this a windows reserved device name?"
//
// Usage:    devno = GetWin32DeviceName(devname)
//           devno = GetWin32DeviceName(buf,bufl)
//
// Where:    devname - IN:  Device name (String)
//           buf     - IN:  Device name buffer (Byte *)
//           bufl    - IN:  Device name length (in bytes)
//           devno   - OUT: Device number (NaN = "Doesn't exist")
//
// This function compares the given string, case insensitive, with
// reserved Windows device names and returns the device number.
//
// If the device isn't found, -1 is returned.
//
// Note: We expect the list ReservedWin32Devices (see top of this file)
//       to contain pointers to 4 byte uppercase device names.  Devices
//       shorter than three characters should be padded with '.' so we
//       know when to stop comparing.
//

Int Win32GetDeviceByName(String &name) {

  Status status;
  Int    devno;
  UInt   namelen;
  Byte   namebuf[10];

  namelen = name.Length();
  if (namelen <= 0 || namelen > 4) goto NOTFOUND;
  status = name.ToBytes(namebuf,10,namelen);
  if (status) goto NOTFOUND;

  devno = Win32GetDeviceByName(namebuf,namelen);

  // Handle termination cases
  while (false) {
    NOTFOUND: devno = NaN; break;
  }

  return devno;

}

Int Win32GetDeviceByName(Byte *buf, Int bufl) {

  Int    devno;
  int    i;
  Byte   checkbuf[4];
  Byte  *devname;

  // If it's longer than 4 character, don't bother...
  if (bufl < 0 || bufl > 4) goto NOTFOUND;

  // It's a possibility, get local uppercase copy of it
  i = 0;
  while (bufl > 0) {
    if (buf[i] >= 'a' && buf[i] <= 'z')
      checkbuf[i] = buf[i] - 'a' + 'A';
    else if (buf[i] >= 'A' && buf[i] <= 'Z')
      checkbuf[i] = buf[i];
    else
      goto NOTFOUND;
    bufl--;
    i++;
  }

  // Okay, search the list of known uppercase 4 letter devices...
  devno = 0;
  while (!BEFE::IsNull(Win32ReservedDevices[devno])) {
    devname = (Byte *)Win32ReservedDevices[devno];
    if (Strcmp(devname,checkbuf) == 0) break;
    devno++;
  }

  // See if we found one
  if (IsNull(Win32ReservedDevices[devno])) goto NOTFOUND;

  // Yep, it's found
  goto FOUND;

  // Handle termination cases...
  while (false) {
    NOTFOUND: devno = NaN; break;
    FOUND:                 break;
  }

  return devno;
}

//-------------------------------------------------------------------------
//
// Function: Win32GetDeviceDescription - Return named device's description
//
// Usage:    devdesc = GetWin32DeviceDescription(devname)
//
// Where:    devname - IN:  Device name (String)
//           devdesc - OUT: Device description (String)
//
// This function returns the named device's description if available.
//

String Win32GetDeviceDescription(String &pdevname) {

  String  devname;
  String  devdesc;
  Strings devdescparts;
  Int     devno;
  Byte    devbuf[MAX_PATH];
  UInt    devbufl;
  Byte    datbuf[MAX_PATH];
  Int     oserror;
  String  tstring;
  Int     i;

  // Take a copy and turn it into uppercase in case it isn't
  devname = pdevname;
  devname.Uppercase();

  // See if we know it by name...
  devno = Win32GetDeviceByName(devname);
  if ( IsNull(devno) ) goto UNKNOWNDEVICE;

  // If it's a device and it's CON, special case because QueryDosDevice won't
  // give a description for it but it's still there...
  if (devno >= 0 && devname == "CON") {
    devdesc = "**Console and Keyboard**";
    goto OK;
  }

  // See if QueryDosDevice knows about it...
  while (true) {

    // Get info from QueryDosDevice...
    devname.ToBytes(devbuf,sizeof(devbuf),devbufl);
    if (devbufl <= 0) goto UNKNOWNDEVICE;
    oserror = QueryDosDevice((const char *)devbuf,(char *)datbuf,sizeof(datbuf));
    if (oserror == 0) break;

    // Now, we should have a volume name, use that for the description
    // But first, split it into parts based on '\', and if it's got
    // one of those long knackered ones and we're not "very verbose",
    // get rid of the part we don't want to display
    devdesc = (char *)datbuf;
    if (!gVeryVerbose) {
      devdescparts = devdesc.SplitToStrings('\\');
      if (!gVeryVerbose && devdescparts.Length() > 4) {
        tstring = "...";
        devdescparts.Set(2,tstring);
        devdescparts.Set(3,tstring);
      }
      devdesc.SetEmpty();
      for (i=0; i < (Int)devdescparts.Length() ;i++) {
        if (i != 0)
          devdesc += '\\';
        tstring = devdescparts.Get(i);
        devdesc += tstring;
      }
    }

    goto OK;

  }

  goto UNKNOWNDEVICE;

  // Handle termination cases...
  while (false) {
    UNKNOWNDEVICE: devdesc = CONSTR_Unknown;     break;
    OK:                                          break;
  }

  return devdesc;

}

//-------------------------------------------------------------------------
//
// Network info functions
//

Status Win32GetHostAndDomains(String &host, Strings &domains) {

  Int         bufl;
  Byte       *buf;
  FIXED_INFO *info;
  Status      status;
  Int         oserr;
  String      domain;

  buf = NULL;

  // Allocate the information buffer
  oserr = GetNetworkParams(0,(PULONG)&bufl);
  if (oserr != ERROR_BUFFER_OVERFLOW) goto WIN32ERROR;
  buf = new Byte[bufl];
  if (IsNull(buf)) goto OUTOFMEMORY;

  // Get the info
  info = (FIXED_INFO *)buf;
  oserr = BEFE::GetNetworkParams(info,(PULONG)&bufl);
  if (oserr) goto WIN32ERROR;

  // Give it to the caller
  host = info->HostName;
  if (info->DomainName[0] != 0) {
    domain = info->DomainName;
    domains.Append(domain);
  }
  else if (info->ScopeId[0] != 0) {
    domain = info->ScopeId;
    domains.Append(domain);
  }

  // Handle errors...
  status = Error::None;
  while (false) {
    WIN32ERROR:  status = Error::OperatingSystemError; break;
    OUTOFMEMORY: status = Error::OutOfMemory;          break;
  }

  if (!IsNull(buf))
    delete[] buf;

  return status;

}

Status Win32GetDNSDomains(Strings &domains) {

  return Error::None;

}

Status Win32GetMACAddresses(Strings &array) {

  Int              bufl = 0;
  Byte            *buf;
  IP_ADAPTER_INFO *ainfo;
  String           thestring;
  Status           status;
  Int              error;

  // Allocate the adaptor information buffer
  error = GetAdaptersInfo(0,(PULONG)&bufl);
  if (error != ERROR_BUFFER_OVERFLOW) goto WIN32ERROR;
  buf = new Byte[bufl];
  if (IsNull(buf)) goto OUTOFMEMORY;

  // Get the Adapter Information.
  ainfo = (IP_ADAPTER_INFO *)buf;
  BEFE::GetAdaptersInfo( ainfo, (PULONG)&bufl );

  // Iterate the network adapters and print their MAC address.
  while( ainfo )
  {
    thestring.SetEmpty();
    Win32FormatMACAddress((Byte *)&ainfo->Address[0],6,thestring);
    array.Append(thestring);
    // Get next adapter info.
    ainfo = ainfo->Next;
  }

  // Handle erros
  status = Error::None;
  while (false) {
    OUTOFMEMORY: status = Error::OutOfMemory;          break;
    WIN32ERROR:  status = Error::OperatingSystemError; break;
  }

  if (!IsNull(buf))
    delete[] buf;

  return status;

}

Status Win32GetIPv4Addresses(Strings &array) {

  Int              bufl;
  Byte            *buf;
  IP_ADAPTER_INFO *ainfo;
  IP_ADDR_STRING  *ipaddr;
  Int              error;

  String thestring;
  Status status;

  // Since we're on Windows, 127.0.0.1 is always available
  thestring = "127.0.0.1";
  array.Append(thestring);

  // Allocate the adaptor information buffer
  error = (Int)GetAdaptersInfo(0,(PULONG)&bufl);
  if (error != ERROR_BUFFER_OVERFLOW) goto WIN32ERROR;
  buf = new Byte[bufl];
  if (IsNull(buf)) goto OUTOFMEMORY;

  // Get the Adapter Information.
  ainfo = (IP_ADAPTER_INFO *)buf;
  BEFE::GetAdaptersInfo( ainfo, (PULONG)&bufl);

  // Iterate the network adapters and print their MAC address.
  while (!IsNull(ainfo))
  {
    thestring.SetEmpty();
    for (ipaddr=&ainfo->IpAddressList; !IsNull(ipaddr); ipaddr = ipaddr->Next) {
      if (Strcmp(ipaddr->IpAddress.String,"0.0.0.0") == 0)
        continue;
      thestring = ipaddr->IpAddress.String;
      //thestring += " (";
      //thestring += ipaddr->IpMask.String;
      //thestring += ')';
      array.Append(thestring);
    };
    // Get next adapter info.
    ainfo = ainfo->Next;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFMEMORY: status = Error::OutOfMemory;          break;
    WIN32ERROR:  status = Error::OperatingSystemError; break;
  }
  
  if (!IsNull(buf))
    delete[] buf;

  return status;

}

Status Win32GetIPv6Addresses(Strings &array) {
  array.SetEmpty();
  return Error::NotImplemented;
}

void Win32FormatMACAddress(Byte *bytes, int numbytes,String &thestr) {

  int  i;
  int  nib;
  char nibs[3];

  nibs[2] = 0;

  for (i=0;i<numbytes;i++) {
    nib = (bytes[i]>>4)&0x0f;
    if (nib < 10)
      nibs[0] = (char)(nib+'0');
    else
      nibs[0] = (char)(nib-10+'A');
    nib = bytes[i] & 0x0f;
    if (nib < 10)
      nibs[1] = (char)(nib+'0');
    else
      nibs[1] = (char)(nib-10+'A');
    if (i != 0)
      thestr << "-";
    thestr << nibs;
  }

  return;

}

//-------------------------------------------------------------------------
//
// Storage info functions
//

Status Win32GetStorageLimits(String devname,Int &unit, Int &nfreeunits, Int &totalunits) {

  Status  status;
  Byte    root[MAX_PATH];
  UInt    rootlen;
  Boolean ok;

  Int     spc;    // Sectors per cluster
  Int     bps;    // Bytes per sector
  Int     nfc;    // Number of free clusters
  Int     tc;     // Total clusters

  unit = nfreeunits = totalunits = 0;

  status = devname.ToBytes(root,sizeof(root),rootlen);
  if (status) goto SOMEERROR;
  if (rootlen >= sizeof(root)) goto BADDEVICE;

  ok = GetDiskFreeSpace((const char *)root,(LPDWORD)&spc,(LPDWORD)&bps,(LPDWORD)&nfc,(LPDWORD)&tc);
  if (!ok) goto BADDEVICE;

  // Calculate storage stuff
  unit = StorageUnit::Byte *bps * spc;
  nfreeunits = nfc;
  totalunits = tc;

  // Handle errors...
  while (false) {
    BADDEVICE: status = Error::Failed; break;
    SOMEERROR:                         break;
  }

  return status;
}

Status Win32RandomUUID(UUID &uuid) {
  BEFE::CoCreateGuid((GUID *)&uuid);
  return Error::None;
}

Status Win32GetCommandLine(String &cmdLine) {

  UShort *wchar;
  
  wchar = (UShort *)GetCommandLineW();
  cmdLine = Win32W_ToString(wchar);

  return Error::None;

}


} // ...Namespace BEFE
