//----------------------------------------------------------------------
// File: .h - Declarations for the Windows 32 platform
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef WIN32_H // WIN32_H...
#define WIN32_H

#include "Primitives.h"  // EVERYTHING NEEDS THIS!!!

// Win32 specialisations of generic classes
#include "Process.h"
#include "Processor.h"
#include "Platform.h"
#include "OperatingSystem.h"
#include "Network.h"
#include "File.h"

//
// For getting at MinGW's windows stuff
//

// Hack... This next bloody definitions are because MinGW gets confused...
int memcmp(const void *a, const void *b, int c);
typedef unsigned int size_t;
// ...Hack

#ifndef WINVER
  #define WINVER 0x0501
  #define _WIN32_WINNT 0x0501
#endif
#include <windows.h>
#include <winbase.h>

#include <psapi.h>          // For process info
#include <iphlpapi.h>       // For process info

#include "stdlib.h"         // For MAX_PATH

// Hack... These next ones are because MinGW uses a #define when
// it should have used a proper namespace name (like we do)...
// Actually, it's Microsoft's bloody fault, surprise surprise!!!
//
// GOD DAMN MICROSOFT, BUNCH OF BLOODY COWBOYS!!!!!!!!!!!!!!!!!!
//
#ifdef GetTempPath
#  undef GetTempPath
#endif
#ifdef GetCurrentTime
#  undef GetCurrentTime
#endif
#ifdef GetTempFileName
#  undef GetTempFileName
#endif
#ifdef GetCurrentDirectory
#  undef GetCurrentDirectory
#endif
#ifdef SetCurrentDirectory
#  undef SetCurrentDirectory
#endif
#ifdef CopyFile
#  undef CopyFile
#endif
#ifdef DeleteFile
#  undef DeleteFile
#endif
#ifdef MoveFile
#  undef MoveFile
#endif
#ifdef GetCommandLine
#  undef GetCommandLine
#endif
// ...Hack

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// General purpose Win32 utility functions
//

// Driving StartUp depth
extern Int         gWin32Depth;

// Some cross source "globals"...
extern const char *Win32ReservedFileChars;
extern const char *Win32ReservedDevices[];

// General purpose
//   implemented in win32utils.cpp
Long    Win32FileTimeToTime(FILETIME ftime);
Status  Win32GetProcessExecutableName(String &thestring);
Status  Win32GetProcessExecutablePath(String &thestring);
Long    Win32GetPhysicalMemory();
UInt    Win32GetMemoryPageSize();
Boolean Win32MemoryIsReadable(void *addr);
Boolean Win32MemoryIsWritable(void *addr);
Strings Win32GetDeviceNames();
Status  Win32GetLogicalDrives(Strings &drives);
Status  Win32GetDeviceName(Int devno,String &name);
Int     Win32GetDeviceByName(Byte *buf, Int bufl);
Int     Win32GetDeviceByName(String &devname);
String  Win32GetDeviceDescription(String &devname);
Status  Win32GetStorageLimits(String devname, Int &bpc, Int &nfc, Int &tc);
Status  Win32RandomUUID(UUID &uuid);
Status  Win32GetCommandLine(String &cmdLine);

// Network info
//   implemented in win32utils.cpp
Status Win32GetHostAndDomains(String &host, Strings &domain);
Status Win32GetDNSDomains(Strings &array);
Status Win32GetMACAddresses(Strings &array);
Status Win32GetIPv4Addresses(Strings &array);
void   Win32FormatMACAddress(Byte *bytes, int numbytes,String &thestr);

// Registry
//   implemented in win32reg.cpp
Status Win32RegGetValueString(const char *key, String &value);
Int    Win32RegGetValueInt(const char *key);

//----------------------------------------------------------------------
//
// Classes: Win32Platform        - Windows 32 specific Platform class
//          Win32Processor       - Windows 32 specific Processor class
//          Win32OperatingSystem - Windows 32 specific OperatingSystem class
//          Win32Process         - Windows 32 specific Process class
//          Win32File            - Windows 32 specific File class
//

class Win32Platform : public Platform {

  // C++ Lifecycle
  public: Win32Platform();
  public: virtual ~Win32Platform();

  // BEFE Lifecycle
  public: virtual Status  StartUp();
  public: virtual Status  ShutDown();
  public:         Status  _StartUp();
  public:         Status  _ShutDown();

  // General Member Get methods  
  virtual String  GetVersionString() const;
  virtual String  GetOSDescription() const;
  virtual String  GetOSVersion() const;
  virtual String  GetHWName() const;
  virtual String  GetHWModel() const;
  virtual String  GetHWVersion() const;
  virtual String  GetHWVendor() const;

  // Internal methods...
  protected: Status InitVars();

};

class Win32Processor : public Processor {

  // C++ Lifecycle
  public: Win32Processor();
  public: virtual ~Win32Processor();

  // BEFE Lifecycle
  public: virtual Status StartUp();
  public: virtual Status ShutDown();
  public:         Status  _StartUp();
  public:         Status  _ShutDown();

  // Processor specific
  virtual String   GetManufacturer();
  virtual String   GetModel();
  virtual String   GetDescription();

  // Internalmethos
  Status InitVars();
  
};

class Win32OperatingSystem : public OperatingSystem {

  // C++ Lifecycle
  public: Win32OperatingSystem();
  public: virtual ~Win32OperatingSystem();

  // BEFE Lifecycle
  public: virtual Status  StartUp();
  public: virtual Status  ShutDown();
  public:         Status  _StartUp();
  public:         Status  _ShutDown();

  // Member Get methods
  public: virtual String  GetDescription();
  public: virtual String  GetVersion();
  public: virtual String  GetHostName();

  // More info about Operating System...
  public: virtual Long    GetCurrentTime();
  public: virtual Long    GetLocalTime();
  public: virtual Strings GetDeviceNames();
  public: virtual String  GetDeviceDescription(String &devname);

  // Existence/Query methods
  public: virtual Boolean IsFile(String const &fname);
  public: virtual Boolean IsDirectory(String const &dirname);
  public: virtual Boolean IsDevice(String const &dname);
  public: virtual Boolean Exists(String const &fileordirname);
  public: virtual Status  GetFileInfo(String const &fname, FileInfo &info);
  public: virtual Status  CreateDirectories(String &dirName);
  public: virtual Status  GetDirectories(String const &fullpath, Strings &subdirs);
  public: virtual Status  GetFiles(String const &fullpath, Strings &files);
  public: virtual Status  GetFilesInfo(String const &fullpath, FileInfos &files);

  // Path/file primitives
  public: virtual Status  PathToGeneric(String &path) const;
  public: virtual Status  PathToNative(String &path) const;
  public: virtual Status  PathSplit(String const &path, Strings &parts) const;
  public: virtual Status  PathJoin(Strings const &parts, String &full) const;
  public: virtual String  FileOnly(String const &path) const;
  public: virtual String  PathOnly(String const &path) const;
  public: virtual Boolean IsPath(String const &path) const;
  public: virtual Boolean IsFullPath(String const &path) const;
  public: virtual Boolean IsRelativePath(String const &path) const;
  public: virtual Boolean IsLocalPath(String const &fullPath) const;
  public: virtual Boolean IsRemotePath(String const &fullPath) const;
  public: virtual Boolean IsValidFileName(String const &filename) const;
  public: virtual Boolean IsValidPathName(String const &filename) const;
  public: virtual String  ApplyRelativePath(String const &path, String const &relpath) const;
  public: virtual Status  ApplyRelativePath(String const &path, String const &relpath, String &replacement) const;

  // Low File and Directory Move/Copy
  protected: virtual Status _LowMoveFile(String const &fromFileName, String const &toFileName);
  protected: virtual Status _LowCopyFile(String const &fromFileName, String const &toFileName);
  protected: virtual Status _LowDeleteFile(String const &fileName);
  //protected: virtual Status _LowMoveDirectory(String const &fromDirName, String const &toDirName);
  //protected: virtual Status _LowCopyDirectory(String const &fromDirName, String const &toDirName);
  //protected: virtual Status _LowDeleteDirectory(String const &dirName);

  // Factories
  public: virtual File   *NewFile();
  public: virtual void    FinishedWith(File *file);

  // Temporary files
  public: virtual String  GetTempPath();
  public: virtual String  GetTempFileName();

  // UUIDs (they're here because they have to created from a "stable storage" (see RFC 4122)
  public: virtual Status  NewRandomUUID(UUID &newUUID);
  
  // Internal methods...
  protected: Status InitVars();
  
};

class Win32Process : public Process {

  // C++ Lifecycle
  public: Win32Process();
  public: virtual ~Win32Process();

  // BEFE Lifecycle
  public: virtual Status  StartUp();
  public: virtual Status  ShutDown();
  public:         Status  _StartUp();
  public:         Status  _ShutDown();

  // Current Working Directory
  public: virtual String  GetCurrentWorkingDirectory();
  public: virtual Status  GetCurrentWorkingDirectory(String &fullPath);
  public: virtual Status  SetCurrentWorkingDirectory(String const &fullPath);
  
  // Query
  public: virtual String  GetExecutableFileName();
  public: virtual String  GetExecutablePath();
  public: virtual Int     GetID();
  public: virtual String  GetCommandLine();
  public: virtual String  GetCommandLineArguments();
  public: virtual Strings GetEnvironmentNames();
  public: virtual String  GetEnvironmentValue(String const &name);
  public: virtual NamedStrings GetEnvironmentValues();
  public: virtual Status  GetEnvironmentValues(NamedStrings &values);
  public: virtual Boolean AddressIsReadable(void *addr);
  public: virtual Boolean AddressIsWritable(void *addr);

  // Last Process Error (Values are OS Dependent)
  public: virtual UInt    LastErrorCode();
  public: virtual String  LastErrorMessage();
  
  // Process Invocation/Control
  public: virtual Status  _Invoke(String const &command);
  public: virtual Status  _Invoke(String const &command, String const &curDir);
  //public: virtual Status  Invoke(String const &command, String const &curDir, NamedStrings const &env);
  public: virtual Status  _Invoke(String       const &command,
                                  String       const &curDir,
                                  NamedStrings const &env,
                                  String       const &stdIn,
                                  String       const &stdOut,
                                  String       const &stdErr);
  public: virtual Status  _InvokeCaptureOutput(String const &command, String &output);
  public: virtual Status  _InvokeCaptureOutput(String const &command, String const &curDir, String &output);  

  // Internal methods...
  protected: Status InitVars();
  
};

class Win32Network : public Network {

  public: Win32Network();
  public: virtual ~Win32Network();

  public: virtual Status StartUp();
  public: virtual Status ShutDown();
  public:         Status _StartUp();
  public:         Status _ShutDown();

  // Member Get methods
  public: virtual String  GetHostName();
  public: virtual Status  GetDomainNames(Strings &dnames);
  public: virtual Status  GetMACAddresses(Strings &mac);
  public: virtual Status  GetIPv4Addresses(Strings &ipv4);
  public: virtual Status  GetIPv6Addresses(Strings &ipv6);  

};

//--------------------------------------------------------------------
//
// Windows types and calls we have to implement ourselves
//
// Note: We got these definitions from Microsoft's web site (sort of)
//

typedef struct _CONSOLE_SCREEN_BUFFER_INFOEX {
  ULONG      cbSize;
  COORD      dwSize;
  COORD      dwCursorPosition;
  WORD       wAttributes;
  SMALL_RECT srWindow;
  COORD      dwMaximumWindowSize;
  WORD       wPopupAttributes;
  BOOL       bFullscreenSupported;
  COLORREF   ColorTable[16];
} CONSOLE_SCREEN_BUFFER_INFOEX, *PCONSOLE_SCREEN_BUFFER_INFOEX;

// These are only here because they're in M$'s declarations and are,
// evidently, simply documentation
#ifndef __in
#  define __in
#endif
#ifndef __out
#  define __out
#endif

// Kernel32.dll...
typedef BOOL (WINAPI *FuncGetConsoleScreenBufferInfoEx)(HANDLE hConsoleOutput
                                                       ,PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx
																											 );

typedef BOOL (WINAPI *FuncSetConsoleScreenBufferInfoEx)(HANDLE hConsoleOutput
																											 ,PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx
                                                       );

BOOL WINAPI GetConsoleScreenBufferInfoEx(HANDLE,PCONSOLE_SCREEN_BUFFER_INFOEX);
BOOL WINAPI GetConsoleScreenBufferInfoEx(HANDLE,PCONSOLE_SCREEN_BUFFER_INFOEX);

// Psapi.dll...

typedef BOOL  (WINAPI *FuncEnumProcessModules)(HANDLE,HMODULE *,DWORD,LPDWORD);
typedef DWORD (WINAPI *FuncGetModuleBaseName) (HANDLE,HMODULE,LPTSTR,DWORD);

BOOL  WINAPI EnumProcessModules(HANDLE,HMODULE *,DWORD,LPDWORD);
DWORD WINAPI GetModuleBaseName (HANDLE,HMODULE,LPTSTR,DWORD);

// Iphlpapi.dll...

typedef DWORD (*FuncGetNetworkParams)  (PFIXED_INFO,PULONG);
typedef DWORD (*FuncGetAdaptersInfo)   (PIP_ADAPTER_INFO,PULONG);

DWORD        GetNetworkParams  (PFIXED_INFO,PULONG);
DWORD        GetAdaptersInfo   (PIP_ADAPTER_INFO,PULONG);

// Ole32.ell...
typedef HRESULT (*FuncCoCreateGuid) (GUID *);

HRESULT CoCreateGuid(GUID *);

} // ...Namespace BEFE

//----------------------------------------------------------------------
//
//  Other Win32 headers...
//

#include "Win32File.h"
#include "Win32Console.h"

//----------------------------------------------------------------------
//
// Base Instances
//

namespace BEFE { // Namespace BEFE...

extern Win32Console         theConsole;
extern Win32Process         theProcess;
extern Win32Platform        thePlatform;
extern Win32OperatingSystem theOS;
extern Win32Processor       theProcessor;
extern Win32Network         theNetwork;

} // ...Namespace BEFE

#endif // ...WIN32_H
