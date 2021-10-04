//----------------------------------------------------------------------
// File: linux.h - Declarations for the linux platform
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LINUX_H // LINUX_H...
#define LINUX_H

#include "Primitives.h"  // EVERYTHING NEEDS THIS!!!

// Linux specialisations of generic classes
#include "Process.h"
#include "Processor.h"
#include "Platform.h"
#include "OperatingSystem.h"
#include "Network.h"
#include "File.h"

namespace BEFE { // Namespace BEFE...

// General purpose
//   implemented in win32utils.cpp
Long    LinuxFileTimeToTime(FILETIME ftime);
Status  LinuxGetProcessExecutableName(String &thestring);
Status  LinuxGetProcessExecutablePath(String &thestring);
Long    LinuxGetPhysicalMemory();
UInt    LinuxGetMemoryPageSize();
Boolean LinuxMemoryIsReadable(void *addr);
Boolean LinuxMemoryIsWritable(void *addr);
Strings LinuxGetDeviceNames();
Status  LinuxGetLogicalDrives(Strings &drives);
Status  LinuxGetDeviceName(Int devno,String &name);
Int     LinuxGetDeviceByName(Byte *buf, Int bufl);
Int     LinuxGetDeviceByName(String &devname);
String  LinuxGetDeviceDescription(String &devname);
Status  LinuxGetStorageLimits(String devname, Int &bpc, Int &nfc, Int &tc);
Status  LinuxRandomUUID(UUID &uuid);
Status  LinuxGetCommandLine(String &cmdLine);

// Network info
//   implemented in win32utils.cpp
Status LinuxGetHostAndDomains(String &host, Strings &domain);
Status LinuxGetDNSDomains(Strings &array);
Status LinuxGetMACAddresses(Strings &array);
                                                                                                                                    99,23         21%


//----------------------------------------------------------------------
//
// Classes: LinuxPlatform        - Linux Platform class
//          LinuxProcessor       - Linux Processor class
//          LinuxOperatingSystem - Linux OperatingSystem class
//          LinuxProcess         - Linux Process class
//          LinuxFile            - Linux File class
//

class LinuxPlatform : public Platform {

  // C++ Lifecycle
  public:          LinuxPlatform();
  public: virtual ~LinuxPlatform();

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

class LinuxProcessor : public Processor {

  // C++ Lifecycle
  public:          LinuxProcessor();
  public: virtual ~LinuxProcessor();

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

class LinuxOperatingSystem : public OperatingSystem {

  // C++ Lifecycle
  public:          LinuxOperatingSystem();
  public: virtual ~LinuxOperatingSystem();

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

class LinuxProcess : public Process {

  // C++ Lifecycle
  public:          LinuxProcess();
  public: virtual ~LinuxProcess();

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

class LinuxNetwork : public Network {

  public: LinuxNetwork();
  public: virtual ~LinuxNetwork();

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

} // ...Namespace BEFE

//----------------------------------------------------------------------
//
//  Other Linux headers...
//

//#include "LinuxFile.h"
//#include "LinuxConsole.h"

//----------------------------------------------------------------------
//
// Base Instances
//

namespace BEFE { // Namespace BEFE...

extern LinuxProcess         theProcess;
extern LinuxPlatform        thePlatform;
extern LinuxOperatingSystem theOS;
extern LinuxProcessor       theProcessor;
extern LinuxNetwork         theNetwork;

} // ...Namespace BEFE

#endif // ...LINUX_H
