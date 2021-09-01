//----------------------------------------------------------------------
// File: Win32Platform.cpp - Win32Platform Class Implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include <_mingw.h>
#include "BEFE.h"
#include "Win32.h"
#include "IdSpace.h"

namespace BEFE { // Namespace BEFE...

Win32Platform::Win32Platform() {
  StartUp();
}

Win32Platform::~Win32Platform() {
  ShutDown();
}

Status Win32Platform::StartUp() {
  return _StartUp();
}

Status Win32Platform::ShutDown() {
  return _ShutDown();
}

Status Win32Platform::_StartUp() {

  Status status; // ◄── TEMP: It turns out this is the first place in the code that
                 //           creates strings and string arrays.  So, we need the
                 //           status for debugging purposes even though we can't return
                 //           it to the caller

  // If no vtable, make one...
  if (*(UInt *)this == 0) {
    Win32Platform x;
    *(Int32 **)this = *(Int32 **)&x;
  }

  status = Platform::StartUp();
  if (status) {} // ◄── See comment above
  physicalPort = PhyPort_Win32MinGW;
  logicalPort  = LogPort_i386 |
                 LogPort_OS32Bit |
                 LogPort_Microsoft |
                 LogPort_Windows|
                 LogPort_WinNT;
  
  shortName    = "Windows.i386-32";
  version.StartUp();

  baseLib       = "MinGW";

  osName        = "Windows32";
  osDescription.StartUp();
  osVersion.StartUp();
  osVendor      = "Microsoft";

  hwName.StartUp();
  hwModel.StartUp();
  hwVersion.StartUp();
  hwVendor.StartUp();
  
  if (!TheBefe->ThePlatform)
    TheBefe->ThePlatform = this;
      
  return Error::None;

}

Status Win32Platform::_ShutDown() {

  if (TheBefe->ThePlatform) {

    if (TheBefe->ThePlatform == this)
      TheBefe->ThePlatform = NULL;
    else
      Platform::ShutDown();

  }

  return Error::None;
}

String Win32Platform::GetVersionString() const {
  ((Win32Platform *)this)->InitVars();
  return version;
}

String Win32Platform::GetOSDescription() const{
  ((Win32Platform *)this)->InitVars();
  return osDescription;
}

String Win32Platform::GetOSVersion() const {
  ((Win32Platform *)this)->InitVars();
  return osVersion;
}

String Win32Platform::GetHWName() const {
  ((Win32Platform *)this)->InitVars();
  return hwName;
}

String Win32Platform::GetHWModel() const {
  ((Win32Platform *)this)->InitVars();
  return hwModel;
}

String Win32Platform::GetHWVersion() const {
  ((Win32Platform *)this)->InitVars();
  return hwVersion;
}

String Win32Platform::GetHWVendor() const {
  ((Win32Platform *)this)->InitVars();
  return hwVendor;
}

Status Win32Platform::InitVars() {
  
  //Status status;
  
  if (version.IsNull()) {
  
    version  = "BEFE-";
    version << mainVersion << '.' << subVersion;
    version += ", MinGW-";
    version += ToString(__MINGW32_MAJOR_VERSION);
    version += '.';
    version += ToString(__MINGW32_MINOR_VERSION);
#ifdef __MINGW32_PATCHLEVEL
    version += '.';
    version += ToString(__MINGW32_PATCHLEVEL);
#endif

    /*status =*/ Win32RegGetValueString("HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/Windows NT/CurrentVersion:ProductName",osDescription);
    /*status =*/ Win32RegGetValueString("HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/Windows NT/CurrentVersion:CurrentVersion",osVersion);

    /*status =*/ Win32RegGetValueString("HKEY_LOCAL_MACHINE/HARDWARE/DESCRIPTION/System/BIOS:SystemManufacturer",hwName);
    if (hwName.Length() == 0)
      hwName        = CONSTR_Unavailable;
    /*status =*/ Win32RegGetValueString("HKEY_LOCAL_MACHINE/HARDWARE/DESCRIPTION/System/BIOS:SystemProductName",hwModel);
    if (hwModel.Length() == 0)
      hwModel       = CONSTR_Unavailable;
    /*status =*/ Win32RegGetValueString("HKEY_LOCAL_MACHINE/HARDWARE/DESCRIPTION/System/BIOS:SystemVersion",hwVersion);
    if (hwVersion.Length() == 0)
      hwVersion     = CONSTR_Unavailable;
    /*status =*/ Win32RegGetValueString("HKEY_LOCAL_MACHINE/HARDWARE/DESCRIPTION/System/BIOS:SystemManufacturer",hwVendor);
    if (hwVendor.Length() == 0)
      hwVendor      = CONSTR_Unavailable;

  }
  
  return Error::None;

}
  
} // ...Namespace BEFE
