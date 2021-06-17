//----------------------------------------------------------------------
// File: Win32Network - Win32Network Class Implementation
//----------------------------------------------------------------------
//
// This file provides the port specific implementations of functions
// and globals used throughout the BEFE code base.
//
// The functions and globals here are referenced in the header
// file "BEFE.h" and "globals.h" and throughout the code.
//
// The reason these functions are here is because they are, or we
// expect them to be, very port specific and may need to be written in
// different forms on each Platform, Processor, Operating System,
// and so on.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Win32.h"

namespace BEFE { // Namespace BEFE...

Win32Network::Win32Network()  {StartUp();};
Win32Network::~Win32Network() {ShutDown();};

extern Network      *TheNetwork;
extern Win32Network  theNetwork;

Status Win32Network::StartUp() {
  return _StartUp();
}

Status Win32Network::ShutDown() {
  return _ShutDown();
}

Status Win32Network::_StartUp() {

  // If no vtable, make one...
  if (*(UInt *)this == 0) {
    Win32Network x;
    *(Int32 **)this = *(Int32 **)&x;
  }

  Network::StartUp();
  
  hostname.StartUp();
  domainnames.StartUp();
  macaddresses.StartUp();
  ipv4addresses.StartUp();
  
  if (!TheBefe->TheNetwork)
    TheBefe->TheNetwork = this;
    
  return Error::None;
  
}

Status Win32Network::_ShutDown() {
  return Network::ShutDown();
}

String Win32Network::GetHostName() {

  if (hostname.IsNull()) {
    Win32RegGetValueString("HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Control/ComputerName/ComputerName:ComputerName",hostname);
    Win32GetHostAndDomains(hostname,domainnames);
  }

  return hostname;

}

Status Win32Network::GetDomainNames(Strings &dnames) {

  if (hostname.IsNull()) {
    Win32RegGetValueString("HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Control/ComputerName/ComputerName:ComputerName",hostname);
    Win32GetHostAndDomains(hostname,domainnames);
  }

  return dnames.CopyFrom(domainnames);

}

Status Win32Network::GetMACAddresses(Strings &mac) {

  if (macaddresses.IsNull())
    Win32GetMACAddresses(macaddresses);

  return mac.CopyFrom(macaddresses);

}

Status Win32Network::GetIPv4Addresses(Strings &ipv4) {

  if (ipv4addresses.IsNull())
    Win32GetIPv4Addresses(ipv4addresses);

  return ipv4.CopyFrom(ipv4addresses);

}

Status Win32Network::GetIPv6Addresses(Strings &ipv6) {
  return ipv6.Reset();
}

} // ...Namespace BEFE
