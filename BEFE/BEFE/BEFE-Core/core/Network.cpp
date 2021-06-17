//----------------------------------------------------------------------
// File: Network.cpp - Generic Network class implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// NOTE: The "real" implementation of these classes is up to the
//       specific platform/port that the product was built for.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Network.h"

namespace BEFE { // Namespace BEFE...

Network::Network()  {StartUp();};
Network::~Network() {ShutDown();}

Status Network::StartUp() {
  return Error::None;
}

Status Network::ShutDown() {

  hostname.ShutDown();
  domainnames.ShutDown();
  macaddresses.ShutDown();
  ipv4addresses.ShutDown();
  ipv6addresses.ShutDown();

  return Error::None;

}

Status Network::Reset() {

  hostname.Reset();
  domainnames.Reset();
  macaddresses.Reset();
  ipv4addresses.Reset();
  ipv6addresses.Reset();

  return Error::None;
}

String Network::GetHostName() {
  return hostname;
}

Status Network::GetDomainNames(Strings &dnames) {

  String s;
  UInt   i;

  dnames.SetEmpty();
  for (i=0; i < domainnames.Length(); i++) {
    s.SetEmpty();
    domainnames.Get(i,s);
    if (s.Length() != 0)
      dnames.Append(s);
  }

  return Error::None;
}

Status Network::GetMACAddresses(Strings &mac) {

  UInt   i;
  String macaddr;

  mac.SetEmpty();
  for (i=0;i<macaddresses.Length();i++) {
    macaddr.SetEmpty();
    macaddresses.Get(i,macaddr);
    if (macaddr.Length() != 0)
      mac.Append(macaddr);
  }

  return Error::None;
}

Status Network::GetIPv4Addresses(Strings &ipv4) {

  UInt   i;
  String ipaddr;

  ipv4.SetEmpty();
  for (i=0; i < ipv4addresses.Length(); i++) {
    ipaddr.SetEmpty();
    ipv4addresses.Get(i,ipaddr);
    if (ipaddr.Length() != 0)
      ipv4.Append(ipaddr);
  }

  return Error::None;
}

Status Network::GetIPv6Addresses(Strings &ipv6) {

  UInt   i;
  String ipaddr;

  ipv6.SetEmpty();
  for (i=0; i < ipv6addresses.Length(); i++) {
    ipaddr.SetEmpty();
    ipv6addresses.Get(i,ipaddr);
    if (ipaddr.Length() != 0)
      ipv6.Append(ipaddr);
  }

  return Error::None;
}

} // ...Namespace BEFE
