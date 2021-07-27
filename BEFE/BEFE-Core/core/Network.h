//!befe-publish inc
//----------------------------------------------------------------------
// File: Network.h - Generic Network class declarations
//----------------------------------------------------------------------
//
// This file contains the declarations associated with the Network
// class.  It is the intention of this class to provide a generic
// platform independent view on TCP/IP networking and associated
// network and internet protocols of TCP/IP.  For example, although
// SMB is not an internet protocol but it IS a network protocol
// available Windows aware networks, including Unix/Linux shops that
// need to provided Windows services.  So we attempt to abstract it
// and pay a little lip service to it.
//
// This base Network class provides the basic structure but not
// implementation.  It is up to the specific platforms to sub-class
// Network and fill in the gaps properly.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef NETWORK_H // NETWORK_H...
#define NETWORK_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

class Network { // Class Network...

//
// Public Methods
//

  // C++ Lifecycle
  public: Network();
  public: virtual ~Network();

  // BEFE Lifecycle
  public: virtual Status StartUp();
  public: virtual Status ShutDown();
  public: virtual Status Reset();
  
  // Member Get methods
  public: virtual String  GetHostName();
  public: virtual Status  GetDomainNames(Strings &dnames);
  public: virtual Status  GetMACAddresses(Strings &mac);
  public: virtual Status  GetIPv4Addresses(Strings &ipv4);
  public: virtual Status  GetIPv6Addresses(Strings &ipv6);


//
// Protected instance members
//

protected:

  String  hostname;
  Strings domainnames;
  Strings macaddresses;
  Strings ipv4addresses;
  Strings ipv6addresses;

}; // ...Class Network

} // ...Namespace BEFE

#endif // ...NETWORK_H