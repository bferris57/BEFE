//!befe-publish inc
//----------------------------------------------------------------------
// File: OperatingSystem.h - Generic OS class declarations and typedefs
//----------------------------------------------------------------------
//
// The intent of the OperatingSystem class is to abstract the things
// we expect to ask of an operating system.
//
// The declarations here are for the base class which provides the
// framework for specific implementations.
//
// These specific implementations should be written for each platform
// we port to.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef OPERATINGSYSTEM_H // OPERATINGSYSTEM_H...
#define OPERATINGSYSTEM_H

#include "Primitives.h"
#include "File.h"
#include "FileInfo.h"
#include "Strings.h"
#include "Time.h"
#include "UUID.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// OperatingSystem Type declarations
//

typedef Status (*OSPathWalker)(String const &dir, UInt32 context);

//----------------------------------------------------------------------
//
// Class OperatingSystem
//

class OperatingSystem { // Class OperatingSystem...

  // Path Type Enum...
  enum PathType {TypeUnknown
                ,TypeDevice
                ,TypeFile
                ,TypeDirectory
                ,TypeVolume
                ,TypeMax = TypeVolume
                 };

  //
  // Protected instance members
  //

  protected: String  shortname;       // Generic OS name
  protected: String  name;
  protected: String  description;
  protected: String  version;
  protected: String  vendor;
  protected: UInt    memoryPageSize;  // Memory page size (in bytes)
  protected: Char    fileseparator;
  protected: Char    pathseparator;
  protected: Char    listseparator;
  protected: String  hostname;
  protected: String  macaddresses;
  protected: String  ipv4addresses;
  protected: String  ipv6addresses;
  protected: Strings searchPaths;
  protected: Boolean apps16;
  protected: Boolean apps32;
  protected: Boolean apps64;
  
  //
  // Public instance methods
  //

  // C++ Lifecycle
  public: OperatingSystem();
  public: virtual ~OperatingSystem();

  // BEFE Lifecycle
  public: virtual Status  StartUp();
  public: virtual Status  ShutDown();
  public: virtual Status  Reset();
  
  // Member Get methods
  public: virtual String  GetShortName();
  public: virtual String  GetName();
  public: virtual String  GetDescription();
  public: virtual String  GetVersion();
  public: virtual String  GetVendor();
  public: virtual UInt    GetMemoryPageSize();
  public: virtual Char    GetFileSeparator();
  public: virtual Char    GetPathSeparator();
  public: virtual Char    GetListSeparator();
  public: virtual String  GetHostName();
  public: virtual Boolean GetApps16();
  public: virtual Boolean GetApps32();
  public: virtual Boolean GetApps64();
  
  // More info about Operating System...
  public: virtual Long    GetCurrentTime();     // Return UTC time in ms since Jan 1, 2000
  public: virtual Long    GetLocalTime();       // Return Local time in ms since Jan 1, 2000
  public: virtual Long    GetUTCTimeDiff();     // Return Local-UTC time difference (to the second)
  public: virtual Strings GetDeviceNames();
  public: virtual String  GetDeviceDescription(String &devname);

  // Existence/Query methods
  //
  // Note: These work using native OR generic file and path names.
  public: virtual Boolean   IsFile(String const &fileName);
  public: virtual Boolean   IsDirectory(String const &dirName);
  public: virtual Boolean   IsDevice(String const &dirName);
  public: virtual Boolean   Exists(String const &fileOrDirName);
  public: virtual Status    GetFileInfo(String const &fileName, FileInfo &info);
  public: virtual FileInfo  GetFileInfo(String const &fileName);
  public: virtual Status    CreateDirectories(String &dirName);
  public: virtual Status    GetDirectories(String const &fullPath, Strings &subDirs);
  public: virtual Strings   GetDirectories(String const &fullPath);
  public: virtual Status    GetFiles(String const &fullPath, Strings &files);
  public: virtual Strings   GetFiles(String const &fullpPath);
  public: virtual Status    GetFilesInfo(String const &fullPath, FileInfos &files);
  public: virtual FileInfos GetFilesInfo(String const &fullPath);

  // Path/file primitives.
  //
  // Note: These work using native file and path names.
  public: virtual Status    PathToGeneric(String &path) const;
  public: virtual Status    PathToNative(String &path) const;
  public: virtual Status    PathSplit(String const &path, Strings &parts) const;
  public: virtual Status    PathJoin(Strings const &parts, String &full) const;

  public: virtual String    FileOnly(String const &path) const;
  public: virtual String    PathOnly(String const &path) const;

  public: virtual Boolean   IsPath(String const &path) const;
  public: virtual Boolean   IsFullPath(String const &path) const;
  public: virtual Boolean   IsRelativePath(String const &path) const;
  public: virtual Boolean   IsLocalPath(String const &fullPath) const;
  public: virtual Boolean   IsRemotePath(String const &fullPath) const;
  public: virtual Boolean   IsValidFileName(String const &fileName) const;
  public: virtual Boolean   IsValidPathName(String const &fileName) const;
  public: virtual String    ApplyRelativePath(String const &relPath) const;
  public: virtual String    ApplyRelativePath(String const &path, String const &relPath) const;
  public: virtual Status    ApplyRelativePath(String const &path, String const &relPath, String &replacement) const;
  public: virtual Status    WalkPath(char const *dir, OSPathWalker walker, UInt32 context);
  public: virtual Status    WalkPath(String const &dir, OSPathWalker walker, UInt32 context);

  public: virtual Status    SetDefaultSearchPaths();
  public: virtual Strings   GetSearchPaths();
  public: virtual String    FindInSearchPath(String const &fileName);
  public: virtual String    FindInSearchPath(String const &fileName, Strings const &addExts);
  public: virtual Status    FindInSearchPath(String const &fileName, Strings const &addExts, String &found);

  // File and Directory Move/Copy/Delete
  //
  // Note: These have lower level primitive methods _Low<name> implemented by
  //       each Platform.  The higher level perform validation and error handling
  //       common across all platforms and then call _Low<name> to perform the
  //       "real work"...
  //
  public:    virtual Status    MoveFile(String const &fromFileName, String const &toFileName);
  public:    virtual Status    CopyFile(String const &fromFileName, String const &toFileName);
  public:    virtual Status    DeleteFile(String const &fileName);
  public:    virtual Status    MoveDirectory(String const &fromDirName, String const &toDirName);
  public:    virtual Status    CopyDirectory(String const &fromDirName, String const &toDirName);
  public:    virtual Status    DeleteDirectory(String const &dirName);

  protected: virtual Status    _LowMoveFile(String const &fromNativeFileName, String const &toNativeFileName);
  protected: virtual Status    _LowCopyFile(String const &fromNativeFileName, String const &toNativeFileName);
  protected: virtual Status    _LowDeleteFile(String const &nativeFileName);
  protected: virtual Status    _LowMoveDirectory(String const &fromNativeDirName, String const &toNativeDirName);
  protected: virtual Status    _LowCopyDirectory(String const &fromNativeDirName, String const &toNativeDirName);
  protected: virtual Status    _LowDeleteDirectory(String const &nativeDirName);
  
  // Factories
  public: virtual File     *NewFile();
  public: virtual void      FinishedWith(File *file);

  // Temporary files
  public: virtual String    GetTempPath();
  public: virtual String    GetTempFileName();

  // UUIDs (they're here because they have to created from a "stable storage" (see RFC 4122)
  public: virtual Status    NewRandomUUID(UUID &newUUID);
  
}; // ...Class OperatingSystem

} // ...Namespace BEFE

#endif // ...OPERATINGSYSTEM_H