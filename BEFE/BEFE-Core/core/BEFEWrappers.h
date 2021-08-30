//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: BEFEWrappers.h - BEFE "Global" function wrappers
//----------------------------------------------------------------------
//
// These "Global" functions, wrap TheBefe and other things to ensure
// TheBefe has been initialised and everything else is okay.
//
// *** See technical documentation for a more detailed description ***
//
// Note: We've only implemented the ones we use, so when you link and
//       get an 'undefined reference', that's why!!!
//
//       If that happens to you, please let us know and we'll do our
//       best to add it into the next release.  In the meantime, you'll
//       just have to code it yourself (it's rather easy, you know!)
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BEFEWRAPPERS_H  // BEFEWRAPPERS_H...
#define BEFEWRAPPERS_H

#include "Primitives.h"
#include "OperatingSystem.h"

namespace BEFE { // Namespace BEFE...

// TheBefe: Process control
Status Exit(Status status);
Status Abort();
Status Abort(String const &message);
Status Abort(char const *message);
Status Warn(String const &message);
Status Warn(char const *message);
Status Congrats(String const &message);
Status Congrats(char const *message);
Status ErrorStatus(char const *func, Status status);
Status ErrorStatus(String const &str, Status status);
Status ErrorString(char const *str);
Status ErrorString(String const &str);
Status Debug(String const &message);
Status Debug(char const *message);
Status Deprecated(char const *message);

// TheBefe: Handy File utility Methods
Status   ValidateNewFileName(String &fName);      // ◄── Validate and adjust non-existent File to Full Path
Status   ValidateExistingFileName(String &fName); // ◄── Validate and adjust existing File to Full Path
Status   FindConfigurationFile(String &fName);    // ◄── Find BEFE Configuration File and update to Full Path

// TheBefe: Environment Variable Management
Status   RefreshEnvironmment();
Status   ClearEnvironment();
Boolean  IsEnvironmentName(String const &name);
Strings  GetEnvironmentNames();
Status   GetEnvironmentNames(Strings &names);
String   GetEnvironmentValue(String const &name);
Status   SetEnvironmentValue(String const &name, String const &value);
Status   RemoveEnvironmentName(String const &name);
String   SubstituteEnvironmentValues(String const &inStr);
Status   SubstituteEnvironmentValues(String const &inStr, String &outStr);

// TheBefe: Error Message Management
Boolean  ErrorMessagesAreLoaded();
Status   LoadErrorMessages();
Status   UnloadErrorMessages();
String   ErrorMessage(Status status);
String   GetMessage(Status msgNo);
  
// TheBefe: Cryptography Methods
CryptKey  GetCryptKey();
Status    GetCryptKey(CryptKey &key);

// TheBefe->TheOS: Existence/Query methods
Boolean   IsFile(String const &fname);
Boolean   IsDirectory(String const &dirname);
Boolean   IsDevice(String const &dname);
Boolean   Exists(String const &fileordirname);
Status    GetFileInfo(String const &fname, FileInfo &info);
FileInfo  GetFileInfo(String const &fname);
Status    CreateDirectories(String &dirname);
Status    GetDirectories(String const &fullpath, Strings &subdirs);
Strings   GetDirectories(String const &fullpath);
Status    GetFiles(String const &fullpath, Strings &files);
Strings   GetFiles(String const &fullpath);
Status    GetFilesInfo(String const &fullpath, FileInfos &files);
FileInfos GetFilesInfo(String const &fullpath);
String    GetCurrentWorkingDirectory();
Status    GetCurrentWorkingDirectory(String &fullpath);
Status    SetCurrentWorkingDirectory(String const &fullpath);

// TheBefe->TheOS: Path/File primitives.
Status    PathToGeneric(String &path);
Status    PathToNative(String &path);
Status    PathSplit(String const &path, Strings &parts);
Status    PathJoin(Strings const &parts, String &full);
String    FileOnly(String const &path);
String    PathOnly(String const &path);
Boolean   IsPath(String const &path);
Boolean   IsFullPath(String const &path);
Boolean   IsRelativePath(String const &path);
Boolean   IsValidFileName(String const &filename);
Boolean   IsValidPathName(String const &filename);
String    ApplyRelativePath(String const &relpath);
String    ApplyRelativePath(String const &path, String const &relpath);
Status    ApplyRelativePath(String const &path, String const &relpath, String &replacement);
Status    WalkPath(char const *dir, OSPathWalker walker, Ptr context);
Status    WalkPath(String const &dir, OSPathWalker walker, Ptr context);

// TheBefe->TheOS: File/Directory Operatios
Status    MoveFile(String const &fromFileName, String const &toFileName);
Status    CopyFile(String const &fromFileName, String const &toFileName);
Status    DeleteFile(String const &fileName);
Status    MoveDirectory(String const &fromDirName, String const &toDirName);
Status    CopyDirectory(String const &fromDirName, String const &toDirName);
Status    DeleteDirectory(String const &dirName);

// TheBefe->TheOS: Search Paths
Status    SetDefaultSearchPaths();
Strings   GetSearchPaths();
String    FindInSearchPath(String const &fName);
String    FindInSearchPath(String const &fName, Strings const &addExts);
Status    FindInSearchPath(String const &fName, Strings const &addExts, String &found);

// TheBefe->TheOS: Temporary files
String    GetTempPath();
String    GetTempFileName();

// TheBefe->TheProcess: Invoking Processes
Status    _Invoke(String const &command);
Status    _Invoke(String const &command, String const &curDir);
Status    _InvokeCaptureOutput(String const &command, String &output);
Status    _InvokeCaptureOutput(String const &command, String const &curDir, String &output);
//Status    _Invoke(String const &command, String const &curDir, NamedStrings const &env);
//Status    _Invoke(String       const &command,
//                  String       const &curDir,
//                  NamedStrings const &env,
//                  String       const &stdInFileName,
//                  String       const &stdOutFileName,
//                  String       const &stdErrFileName);

// TheBefe->ThePlatform: Various Information
//String    GetShortName();
//String    GetVersionString();
//String    GetRevisionString();
UInt      GetMainVersion();
UInt      GetSubVersion();
UInt      GetMainRevision();
UInt      GetSubRevision();
//String    GetBaseLibraries();

//String    GetOSName();
//String    GetOSDescription();
//String    GetOSVersion();
//String    GetOSVendor();

//String    GetHWName();
//String    GetHWModel();
//String    GetHWVersion();
//String    GetHWVendor();

//Int       GetCapabilities();
//Int       GetDataSize();

UInt    GetDataSize();
UInt    GetPhysicalPort();
String  GetPhysicalPortString();
UInt    GetLogicalPort();
String  GetLogicalPortString();
Boolean Is_i8086();
Boolean Is_i386();
Boolean Is_ARC();
Boolean Is_RIM();
Boolean IsMicrosoft();
Boolean IsApple();
Boolean IsGoogle();
Boolean Is_Android();
Boolean Is_iOS();
Boolean Is_Symbian();
Boolean Is_Unix();
Boolean Is_Windows();
Boolean Is_OSX();
Boolean Is_WinCE();
Boolean Is_WinNT();
Boolean Is_Linux();
Boolean Is_Debian();
Boolean Is_Darwin();
Boolean Is_Ubuntu();

// TheBefe->TheCache
Status StartCacheFile(String const &fileName);
Status StartCacheFile(String const &fileName, Boolean writable);
Status StopCacheFile(String const &fileName);
Status LoadFileBlock(Id fileId, UInt blockNo);
Status FlushFileBlock(Id fileId, UInt blockNo);
UInt   GetBlockUsedBytes(Id fileId, UInt blockNo);
Status GetBlockUsedBytes(Id fileId, UInt blockNo, UInt &blockLen);
Status SetBlockUsedBytes(Id fileId, UInt blockNo, UInt blockLen);
Status GetBlockContents(Id fileId, UInt blockNo, UInt offset, Byte *buf, UInt len);
Status SetBlockContents(Id fileId, UInt blockNo, UInt offset, Byte *buf, UInt len);
UInt   GetFileBlockCount(Id fileId);
Status GetFileBlockCount(Id fileId, UInt &numBlocks);
Status AppendFileBlock(Id fileId);
Status AppendFileBlocks(Id fileId, UInt numBlocks);
                             
} // ...Namespace BEFE

#endif // ...BEFEWRAPPERS_H
