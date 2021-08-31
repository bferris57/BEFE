//!befe-publish inc
//----------------------------------------------------------------------
// File: Platform.h - Generic Platform Class declarations
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// NOTE: The "real" implementation of these classes is up to the
//       specific port that the product was built for.  The Process
//       Class serves as an outline of what Methods need implementing
//       and what the methods are supposed to accomplish.
//
//       As this is the case, "Platform.cpp" only contains default
//       behaviour for the implementation Classes.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef PLATFORM_H // PLATFORM_H...
#define PLATFORM_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

class Platform { // Class Platform...

public:

  // PhysicalPort - One of these for each Platform implementation
  enum PhysicalPort {
    
     PhyPost_None           // 0x00 - Unknown or Not Specified
    ,PhyPort_Win32MinGW     // 0x01 - Windows32-MinGW (i386-32)
    ,PhyPort_Linux32        // 0x02 - Linux32 (i386-32)
    ,PhyPort_Win64Mingw     // 0x03 - Windows32-MinGW (i386-64)
    ,PhyPort_Linux64        // 0x04 - Linux64 (i386-64)

    
    ,NumPhysicalPorts

  }; 

  // LogicalPort - Each PhysicalPort may be for a multiple number of these...
  enum LogicalPort { 

     LogPort_None
     
    // High level processor Architecture
    ,LogPort_i8086      = 0x00000001
    ,LogPort_i386       = 0x00000002
    ,LogPort_ARC        = 0x00000004
    ,LogPort_RIM        = 0x00000008
    
    // OS Bit Format
    ,LogPort_OS32Bit    = 0x00000010
    ,LogPort_OS64Bit    = 0x00000020
    
    // Vendors
    ,LogPort_Microsoft  = 0x00000040
    ,LogPort_Apple      = 0x00000080
    ,LogPort_Google     = 0x00000100
    
    // Mobile Platforms
    ,LogPort_Android    = 0x00000200
    ,LogPort_iOS        = 0x00000400
    ,LogPort_Symbian    = 0x00000800
    
    // High level operating system "flavours"
    ,LogPort_Unix       = 0x00001000
    ,LogPort_Linux      = 0x00002000
    ,LogPort_Windows    = 0x00004000
    ,LogPort_Posix      = 0x00008000
    
    // Medium level operating system categories
    ,LogPort_WinCE      = 0x00010000
    ,LogPort_WinNT      = 0x00020000
    ,LogPort_Debian     = 0x00040000
    ,LogPort_Darwin     = 0x00080000
    
    // Specific Low level operating systems
    ,LogPort_Ubuntu     = 0x00100000
    ,LogPort_OSX        = 0x00200000
    
  };

//
// Public enum declarations
//

public:

  enum DataSize {
     Unknown  = 0
    ,Bit0     = 0   // 0 bits (Also means "unknown", "not specified", or "not usable in the slightest")
    ,Bit1           // 1 bit (0,1, bit streams)
    ,Bit2           // 2 bits (0..3)
    ,Bit3           // 3 bits (Octal)
    ,Bit4           // 4 bits (Hexadecimal, Simple colour palette, Nibble, 0..15, etc.)
    ,Bit5           // 5 bits (0..31, like bit addresses in "normal" word)
    ,Bit6           // 6 bits (Base64, etc.)
    ,Bit7           // 7 bits (ASCII)
    ,Bit8           // 1 byte
    ,Bit16          // 2 bytes
    ,Bit20          // 2 bytes + nibble (16-bit segmented like old x86)
    ,Bit24          // 3 bytes (RGB no Alpha, etc.)
    ,Bit32          // 4 bytes ("Normal", RGB with Alpha, etc.)
    ,Bit64          // 8 bytes (long longs, etc.)
    ,Bit128         // 16 bytes (UUIDs, etc.)
  };

  enum Capability {
     None

     // Register/Word info
    ,BigEndian          = 0x00000001
  };

//
// Protected instance members
//

protected:

  String shortName;          // We hard code this
  String version;            // We hard code this
  String baseLib;            // Base library implementation
  UInt   mainVersion;        // We hard code this
  UInt   subVersion;         // We hard code this
  UInt   mainRevision;       // We hard code this
  UInt   subRevision;        // We hard code this
  UInt   physicalPort;       // We hard code this
  UInt   logicalPort;        // We hard code this
  
  String osName;             // We hard code this
  String osDescription;      // Try to get this from OS
  String osVersion;          // Try to get this from OS
  String osVendor;           // Try to get this from OS

  String hwName;             // Try to get this from OS
  String hwModel;            // Try to get this from OS
  String hwVersion;          // Try to get this from OS
  String hwVendor;           // Try to get this from OS

  // Register/Word data size
  UInt   datasize;           // Processor 'word' or 'register' size (Bits)
                             // *** Only here in case 64-bit code can
                             // *** run in either mode
                             
//
// Public instance methods
//

public:

  // C++ Lifecycle
           Platform();
  virtual ~Platform();

  // BEFE Lifecycle
  virtual Status   StartUp();
  virtual Status   ShutDown();
  virtual Status   Reset();

  //  
  // General Member Get methods
  //
  
  virtual String  GetShortName() const;
  virtual String  GetVersionString() const;
  virtual String  GetRevisionString() const;
  virtual UInt    GetMainVersion() const;
  virtual UInt    GetSubVersion() const;
  virtual UInt    GetMainRevision() const;
  virtual UInt    GetSubRevision() const;
  virtual String  GetBaseLibraries() const;

  virtual String  GetOSName() const;
  virtual String  GetOSDescription() const;
  virtual String  GetOSVersion() const;
  virtual String  GetOSVendor() const;

  virtual String  GetHWName() const;
  virtual String  GetHWModel() const;
  virtual String  GetHWVersion() const;
  virtual String  GetHWVendor() const;

  //
  // Platform Port information 
  //
  
  virtual UInt    GetDataSize() const;
  
  virtual UInt    GetPhysicalPort() const;
  virtual String  GetPhysicalPortString() const;
  
  // Logical Port...
  virtual UInt    GetLogicalPort() const;
  virtual String  GetLogicalPortString() const;

  // Logical Port: High level processor Architecture
  virtual Boolean Is_i8086() const;
  virtual Boolean Is_i386() const;
  virtual Boolean Is_ARC() const;
  virtual Boolean Is_RIM() const;
    
  // Logical Port: Vendors
  virtual Boolean IsMicrosoft() const;
  virtual Boolean IsApple() const;
  virtual Boolean IsGoogle() const;
    
  // Logical Port: Mobile Platforms
  virtual Boolean Is_Android() const;
  virtual Boolean Is_iOS() const;
  virtual Boolean Is_Symbian() const;
    
  // Logical Port: High level operating systems
  virtual Boolean Is_Unix() const;
  virtual Boolean Is_Linux() const;
  virtual Boolean Is_Windows() const;
  virtual Boolean Is_Posix() const;

  // Logical Port: Medium level operating system categories
  virtual Boolean Is_WinCE() const;
  virtual Boolean Is_WinNT() const;
  virtual Boolean Is_Debian() const;
  virtual Boolean Is_Darwin() const;
  
  // Low level specific operating systems
  virtual Boolean Is_Ubuntu() const;
  virtual Boolean Is_OSX() const;
                             
}; // ...Class Platform

//
// Macro to answer "Are we 32 or 64 bit pointers?"...
//

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define IS64BIT
#else
#define IS32BIT
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define IS64BIT
#else
#define IS32BIT
#endif
#endif

} // ...Namespace BEFE

#endif // ...PLATFORM_H
