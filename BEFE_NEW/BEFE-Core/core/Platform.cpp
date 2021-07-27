//----------------------------------------------------------------------
// File: Platform.cpp - Generic platform class implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// NOTE: The "real" implementation of these classes is up to the
//       specific port that the product was built for.
//
//       Also, note that Platform::StartUp() is implemented in platform
//       specific code NOT HERE!!!!
//
//       That's because this source's job is to try and fill in all the
//       platform info it can given the standard C++ libraries available
//       across all ports.  This makes it easier to start a new port and
//       fill in the blanks later.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Platform.h"

namespace BEFE { // Namespace BEFE...

Platform::Platform() {
  StartUp();
}

Platform::~Platform() {ShutDown();}

Status Platform::StartUp() {

  //if (this == ThePlatform) goto OK;

  shortName     = CONSTR_QQQ;
  version       = CONSTR_QQQ;
  baseLib       = String();
  mainVersion   = BEFE_VERSION;
  subVersion    = BEFE_SUBVERSION;
  mainRevision  = UNaN;
  subRevision   = UNaN;
  physicalPort  = 0;
  logicalPort   = 0;
  
  osName        = CONSTR_Unknown;
  osDescription = CONSTR_Unknown;
  osVersion     = CONSTR_Unknown;
  osVendor      = CONSTR_Unknown;

  hwName        = CONSTR_Unknown;
  hwModel       = CONSTR_Unknown;
  hwVersion     = CONSTR_Unknown;
  hwVendor      = CONSTR_Unknown;

  datasize     = sizeof(int);

  return Error::None;

}

Status Platform::ShutDown() {

  shortName.ShutDown();
  version.ShutDown();
  baseLib.ShutDown();

  osName.ShutDown();
  osDescription.ShutDown();
  osVersion.ShutDown();
  osVendor.ShutDown();

  hwName.ShutDown();
  hwModel.ShutDown();
  hwVersion.ShutDown();
  hwVendor.ShutDown();
  
  return Error::None;

}

Status Platform::Reset() {
  
  shortName.Reset();
  version.Reset();
  baseLib.Reset();

  osName.Reset();
  osDescription.Reset();
  osVersion.Reset();
  osVendor.Reset();

  hwName.Reset();
  hwModel.Reset();
  hwVersion.Reset();
  hwVendor.Reset();

  return Error::None;
  
}

String Platform::GetShortName() const {
  return shortName;
}

String Platform::GetVersionString() const {
  String version;
  version = this->version;
  if (this->version.Length() == 0)
    version << "Befe-" << mainVersion << '.' << subVersion << '.' << mainRevision;
  return version.Consumable();
}

String Platform::GetRevisionString() const {
  return GITGetValue(BEFE_REVISION);
}

UInt Platform::GetMainVersion() const {
  return mainVersion;
}

UInt Platform::GetSubVersion() const {
  return subVersion;
}

UInt Platform::GetMainRevision() const {
  return mainRevision;
}

UInt Platform::GetSubRevision() const {
  return subRevision;
}

String Platform::GetBaseLibraries() const {
  return baseLib;
}

String Platform::GetOSName() const {
  return osName;
}

String Platform::GetOSDescription() const {
  return osDescription;
}

String Platform::GetOSVersion() const {
  return osVersion;
}

String Platform::GetOSVendor() const {
  return osVendor;
}

String Platform::GetHWName() const {
  return hwName;
}

String Platform::GetHWModel() const {
  return hwModel;
}

String Platform::GetHWVersion() const {
  return hwVersion;
}

String Platform::GetHWVendor() const {
  return hwVendor;
}

//----------------------------------------------------------------------
//
// Platform Port information 
//

UInt Platform::GetDataSize()             const {return datasize;}
UInt Platform::GetPhysicalPort()         const {return physicalPort;}
String Platform::GetPhysicalPortString() const {return CONSTR_Unknown;}
  
// Logical Port...
UInt Platform::GetLogicalPort()         const {return logicalPort;}
String Platform::GetLogicalPortString() const {return CONSTR_Unknown;}

// Logical Port: High level processor Architecture
Boolean Platform::Is_i8086()    const {return (GetLogicalPort()&LogPort_i8086)     ?true:false;}
Boolean Platform::Is_i386()     const {return (GetLogicalPort()&LogPort_i386)      ?true:false;}
Boolean Platform::Is_ARC()      const {return (GetLogicalPort()&LogPort_ARC)       ?true:false;}
Boolean Platform::Is_RIM()      const {return (GetLogicalPort()&LogPort_RIM)       ?true:false;}
    
// Logical Port: Vendors
Boolean Platform::IsMicrosoft() const {return (GetLogicalPort()&LogPort_Microsoft) ?true:false;}
Boolean Platform::IsApple()     const {return (GetLogicalPort()&LogPort_Apple)     ?true:false;}
Boolean Platform::IsGoogle()    const {return (GetLogicalPort()&LogPort_Google)    ?true:false;}
    
// Logical Port: Mobile Platforms
Boolean Platform::Is_Android()  const {return (GetLogicalPort()&LogPort_Android)   ?true:false;}
Boolean Platform::Is_iOS()      const {return (GetLogicalPort()&LogPort_iOS)       ?true:false;}
Boolean Platform::Is_Symbian()  const {return (GetLogicalPort()&LogPort_Symbian)   ?true:false;}
    
// Logical Port: High level operating systems
Boolean Platform::Is_Unix()     const {return (GetLogicalPort()&LogPort_Unix)      ?true:false;}
Boolean Platform::Is_Linux()    const {return (GetLogicalPort()&LogPort_Linux)     ?true:false;}
Boolean Platform::Is_Windows()  const {return (GetLogicalPort()&LogPort_Windows)   ?true:false;}
Boolean Platform::Is_Posix()    const {return (GetLogicalPort()&LogPort_Posix)     ?true:false;}

// Logical Port: Medium level operating systems
Boolean Platform::Is_WinCE()    const {return (GetLogicalPort()&LogPort_WinCE)     ?true:false;}
Boolean Platform::Is_WinNT()    const {return (GetLogicalPort()&LogPort_WinNT)     ?true:false;}
Boolean Platform::Is_Debian()   const {return (GetLogicalPort()&LogPort_Debian)    ?true:false;}
Boolean Platform::Is_Darwin()   const {return (GetLogicalPort()&LogPort_Darwin)    ?true:false;}

  // Low level specific operating systems
Boolean Platform::Is_Ubuntu()   const {return (GetLogicalPort()&LogPort_Ubuntu)    ?true:false;}
Boolean Platform::Is_OSX()      const {return (GetLogicalPort()&LogPort_OSX)       ?true:false;}
                             
} // ...Namespace BEFE
