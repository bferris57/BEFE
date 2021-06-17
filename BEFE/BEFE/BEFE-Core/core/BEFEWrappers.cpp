//----------------------------------------------------------------------
// File: BEFEWrappers.cpp - BEFE "Global" wrapper Functions
//----------------------------------------------------------------------
//
// These "Global" functions, wrap TheBefe and other things to ensure
// TheBefe has been initialised and everything else is okay.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEWrappers.h"

//
// Wrapper Macros...
//

#define WRAP_TheBefe(funcCall) {       \
  if (!TheBefe) return Error::NoBefe;  \
  return (TheBefe->funcCall);          \
  }
  
#define WRAP_TheBefeBoolean(funcCall) { \
  if (!TheBefe) return false;           \
  return (TheBefe->funcCall);           \
}

#define WRAP_TheBefeString(funcCall) { \
  if (!TheBefe) return String();       \
  return (TheBefe->funcCall);          \
}

#define WRAP_TheOS(funcCall) {                        \
  if (!TheBefe) return Error::NoBefe;                 \
  if (!TheBefe->TheOS) return Error::InternalNoTheOS; \
  return (TheBefe->TheOS->funcCall);                  \
  }
  
#define WRAP_TheOSBoolean(funcCall) {  \
  if (!TheBefe) return false;          \
  if (!TheBefe->TheOS) return false;   \
  return (TheBefe->TheOS->funcCall);   \
  }

#define WRAP_TheOSString(funcCall) {    \
  if (!TheBefe) return String();        \
  if (!TheBefe->TheOS) return String(); \
  return (TheBefe->TheOS->funcCall);    \
  }

#define WRAP_TheOSStrings(funcCall) {    \
  if (!TheBefe) return Strings();        \
  if (!TheBefe->TheOS) return Strings(); \
  return (TheBefe->TheOS->funcCall);     \
  }

#define WRAP_TheOSFileInfo(funcCall) {    \
  if (!TheBefe) return FileInfo();        \
  if (!TheBefe->TheOS) return FileInfo(); \
  return (TheBefe->TheOS->funcCall);    \
  }

#define WRAP_TheOSFileInfos(funcCall) {    \
  if (!TheBefe) return FileInfos();        \
  if (!TheBefe->TheOS) return FileInfos(); \
  return (TheBefe->TheOS->funcCall);    \
  }

#define WRAP_TheProcess(funcCall) {                             \
  if (!TheBefe)             return Error::NoBefe;               \
  if (!TheBefe->TheOS)      return Error::InternalNoTheOS;      \
  if (!TheBefe->TheProcess) return Error::InternalNoTheProcess; \
  return (TheBefe->TheProcess->funcCall);                       \
  }
  
#define WRAP_TheProcessString(funcCall) {             \
  if (!TheBefe)        return "!!! No TheBEFE!!!";    \
  if (!TheBefe->TheOS) return "!!! No TheOS!!!";      \
  if (!TheBefe->TheOS) return "!!! No TheProcess!!!"; \
  return (TheBefe->TheProcess->funcCall);             \
  }
  
#define WRAP_ThePlatformUInt(funcCall) {                          \
  if (!TheBefe) return Error::NoBefe;                             \
  if (!TheBefe->ThePlatform) return Error::InternalNoThePlatform; \
  return (TheBefe->ThePlatform->funcCall);                        \
  }
  
#define WRAP_ThePlatformBoolean(funcCall) { \
  if (!TheBefe) return false;               \
  if (!TheBefe->ThePlatform) return false;  \
  return (TheBefe->ThePlatform->funcCall);  \
  }
  
#define WRAP_TheCache(funcCall) {       \
  if (!TheBefe) return Error::NoBefe;   \
  return (TheBefe->TheCache.funcCall);  \
  }
  
#define WRAP_TheCacheUInt(funcCall) {   \
  if (!TheBefe) return UNaN;            \
  return (TheBefe->TheCache.funcCall);  \
  }
  
namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// TheBefe: Process control
//

//Status Exit(Status status)                            WRAP_TheBefe(Exit(status))
Status Abort()                                        WRAP_TheBefe(Abort())
Status Abort(String const &message)                   WRAP_TheBefe(Abort(message))
Status Abort(char const *message)                     WRAP_TheBefe(Abort(message))
Status Warn(String const &message)                    WRAP_TheBefe(Warn(message))
Status Warn(char const *message)                      WRAP_TheBefe(Warn(message))
Status Congrats(String const &message)                WRAP_TheBefe(Congrats(message))
Status Congrats(char const *message)                  WRAP_TheBefe(Congrats(message))
Status ErrorStatus(char const *func, Status status)   WRAP_TheBefe(ErrorStatus(func, status))
Status ErrorStatus(String const &str, Status status)  WRAP_TheBefe(ErrorStatus(str, status))
Status ErrorString(char const *str)                   WRAP_TheBefe(ErrorString(str))
Status ErrorString(String const &str)                 WRAP_TheBefe(ErrorString(str))
Status Debug(String const &message)                   WRAP_TheBefe(Debug(message))
Status Debug(char const *message)                     WRAP_TheBefe(Debug(message))
Status Deprecated(char const *message)                WRAP_TheBefe(Debug(message))

//----------------------------------------------------------------------
//
// TheBefe: Handy File utility Methods
//

Status ValidateNewFileName(String &fName)       WRAP_TheBefe(ValidateNewFileName(fName))
Status ValidateExistingFileName(String &fName)  WRAP_TheBefe(ValidateExistingFileName(fName))
Status FindConfigurationFile(String &fName)     WRAP_TheBefe(FindConfigurationFile(fName))
  
//----------------------------------------------------------------------
//
// TheBefe: Environment Variable Management
//

Strings GetEnvironmentNames() {
  if (!TheBefe) return Strings();
  return TheBefe->GetEnvironmentNames();
}

Status GetEnvironmentNames(Strings &names) {
  if (!TheBefe) return Error::NoBefe;
  return TheBefe->GetEnvironmentNames(names);
}

String GetEnvironmentValue(String const &name) {
  if (!TheBefe) return String();
  return TheBefe->GetEnvironmentValue(name).Consumable();
}

Status SetEnvironmentValue(String const &name, String const &value) {
  if (!TheBefe) return Error::NoBefe;
  return TheBefe->SetEnvironmentValue(name, value);
}

Status RemoveEnvironmentName(String const &name) {
  if (!TheBefe) return Error::NoBefe;
  return TheBefe->RemoveEnvironmentName(name);
}

Status RefreshEnvironment() {
  if (!TheBefe) return Error::NoBefe;
  return TheBefe->RefreshEnvironment();
}  

Status ClearEnvironment() {
  if (!TheBefe) return Error::NoBefe;
  return TheBefe->ClearEnvironment();
}

Boolean IsEnvironmentName(String const &name) {
  if (!TheBefe) return false;
  return TheBefe->IsEnvironmentName(name);
}

String SubstituteEnvironmentValues(String const &inStr) {
  if (!TheBefe) return inStr;
  return TheBefe->SubstituteEnvironmentValues(inStr);
}

Status SubstituteEnvironmentValues(String const &inStr, String &outStr) {
  if (!TheBefe) { outStr = inStr; return Error::NoBefe; }
  return TheBefe->SubstituteEnvironmentValues(inStr, outStr);
}

//----------------------------------------------------------------------
//
// TheBefe: Error Message Management
//

Boolean ErrorMessagesAreLoaded()    WRAP_TheBefeBoolean(ErrorMessagesAreLoaded())
Status  LoadErrorMessages()         WRAP_TheBefe(LoadErrorMessages())
Status  UnloadErrorMessages()       WRAP_TheBefe(UnloadErrorMessages())
String  ErrorMessage(Status status) WRAP_TheBefeString(ErrorMessage(status))
String  GetMessage(Status msgNo)    WRAP_TheBefeString(GetMessage(msgNo))
  
//----------------------------------------------------------------------
//
// TheBefe: Cryptography Methods
//

CryptKey GetCryptKey() {
  CryptKey key;
  if (TheBefe)
    TheBefe->GetCryptKey(key);
  return key.Consumable();
}

Status GetCryptKey(CryptKey &key) WRAP_TheBefe(GetCryptKey(key))

//----------------------------------------------------------------------
//
// TheBefe->TheOS: Existence/Query methods
//

Boolean   IsFile(String const &fname)                      WRAP_TheOSBoolean(IsFile(fname))
Boolean   IsDirectory(String const &dirname)               WRAP_TheOSBoolean(IsDirectory(dirname))
Boolean   IsDevice(String const &dname)                    WRAP_TheOSBoolean(IsDevice(dname))
Boolean   Exists(String const &fileordirname)              WRAP_TheOSBoolean(Exists(fileordirname))
Status    GetFileInfo(String const &fname, FileInfo &info) WRAP_TheOS(GetFileInfo(fname,info))
FileInfo  GetFileInfo(String const &fname)                 WRAP_TheOSFileInfo(GetFileInfo(fname))
Status    CreateDirectories(String &dirname)               WRAP_TheOS(CreateDirectories(dirname))
Status    GetDirectories(String const &fullpath, Strings &subdirs)
  WRAP_TheOS(GetDirectories(fullpath, subdirs))
Strings   GetDirectories(String const &fullpath)
  WRAP_TheOSStrings(GetDirectories(fullpath))
Status    GetFiles(String const &fullpath, Strings &files)
  WRAP_TheOS(GetFiles(fullpath, files))
Strings   GetFiles(String const &fullpath)
  WRAP_TheOSStrings(GetFiles(fullpath))
Status    GetFilesInfo(String const &fullpath, FileInfos &files)
  WRAP_TheOS(GetFilesInfo(fullpath, files))
FileInfos GetFilesInfo(String const &fullpath)
  WRAP_TheOSFileInfos(GetFilesInfo(fullpath))

//----------------------------------------------------------------------
//
// TheBefe->TheOS: Path/file primitives.
//

Status    PathToGeneric(String &path)
  WRAP_TheOS(PathToGeneric(path))
Status    PathToNative(String &path)
  WRAP_TheOS(PathToNative(path))
Status    PathSplit(String const &path, Strings &parts)
  WRAP_TheOS(PathSplit(path, parts))
Status    PathJoin(Strings const &parts, String &full)
  WRAP_TheOS(PathJoin(parts, full))
String    FileOnly(String const &path)
  WRAP_TheOSString(FileOnly(path))
String    PathOnly(String const &path)
  WRAP_TheOSString(PathOnly(path))
Boolean   IsPath(String const &path)
  WRAP_TheOSBoolean(IsPath(path))
Boolean   IsFullPath(String const &path)
  WRAP_TheOSBoolean(IsFullPath(path))
Boolean   IsRelativePath(String const &path)
  WRAP_TheOSBoolean(IsRelativePath(path))
Boolean   IsValidFileName(String const &filename)
  WRAP_TheOSBoolean(IsValidFileName(filename))
Boolean   IsValidPathName(String const &filename)
  WRAP_TheOSBoolean(IsValidPathName(filename))
String    ApplyRelativePath(String const &relpath)
  WRAP_TheOSString(ApplyRelativePath(relpath))
String    ApplyRelativePath(String const &path, String const &relpath)
  WRAP_TheOSString(ApplyRelativePath(path, relpath))
Status    ApplyRelativePath(String const &path, String const &relpath, String &replacement)
  WRAP_TheOS(ApplyRelativePath(path, relpath, replacement))
Status    WalkPath(char const *dir, OSPathWalker walker, UInt32 context)
  WRAP_TheOS(WalkPath(dir, walker, context))
Status    WalkPath(String const &dir, OSPathWalker walker, UInt32 context)
  WRAP_TheOS(WalkPath(dir, walker, context))

Status    MoveFile(String const &fromFileName, String const &toFileName)
  WRAP_TheOS(MoveFile(fromFileName, toFileName));
Status    CopyFile(String const &fromFileName, String const &toFileName)
  WRAP_TheOS(CopyFile(fromFileName, toFileName));
Status    DeleteFile(String const &fileName)
  WRAP_TheOS(DeleteFile(fileName));
Status    MoveDirectory(String const &fromDirName, String const &toDirName)
  WRAP_TheOS(MoveDirectory(fromDirName, toDirName));
Status    CopyDirectory(String const &fromDirName, String const &toDirName)
  WRAP_TheOS(CopyDirectory(fromDirName, toDirName));
Status    DeleteDirectory(String const &dirName)
  WRAP_TheOS(DeleteDirectory(dirName));

Status    SetDefaultSearchPaths()
  WRAP_TheOS(SetDefaultSearchPaths())
Strings   GetSearchPaths()
  WRAP_TheOSStrings(GetSearchPaths())
String    FindInSearchPath(String const &fName)
  WRAP_TheOSString(FindInSearchPath(fName))
String    FindInSearchPath(String const &fName, Strings const &addExts)
  WRAP_TheOSString(FindInSearchPath(fName, addExts))
Status    FindInSearchPath(String const &fName, Strings const &addExts, String &found)
  WRAP_TheOS(FindInSearchPath(fName, addExts, found))

//----------------------------------------------------------------------
//
// TheBefe->TheOS: Temporary files
//

String GetTempPath() {
  if (!TheBefe || !TheBefe->TheOS)
    return String();
  else
    return TheBefe->TheOS->GetTempPath();
}

String GetTempFileName() {
  if (!TheBefe || !TheBefe->TheOS)
    return String();
  else
    return TheBefe->TheOS->GetTempFileName();
}

//----------------------------------------------------------------------
//
// TheBefe->TheProcess: Invoking Processes
//

String GetCurrentWorkingDirectory()                       WRAP_TheProcessString(GetCurrentWorkingDirectory())
Status GetCurrentWorkingDirectory(String &fullpath)       WRAP_TheProcess(GetCurrentWorkingDirectory(fullpath))
Status SetCurrentWorkingDirectory(String const &fullpath) WRAP_TheProcess(SetCurrentWorkingDirectory(fullpath))

Status _Invoke(String const &command)                       WRAP_TheProcess(_Invoke(command))
Status _Invoke(String const &command, String const &curDir) WRAP_TheProcess(_Invoke(command, curDir))

Status _Invoke(String const &command, String const &curDir, NamedStrings const &env);
Status _Invoke(String       const &command,
               String       const &curDir,
               NamedStrings const &env,
               String       const &stdInFileName,
               String       const &stdOutFileName,
               String       const &stdErrFileName);
               
Status _InvokeCaptureOutput(String const &command, String &output)
  WRAP_TheProcess(_InvokeCaptureOutput(command, output))
Status _InvokeCaptureOutput(String const &command, String const &curDir, String &output)
  WRAP_TheProcess(_InvokeCaptureOutput(command,curDir,output))

//----------------------------------------------------------------------
//
// TheBefe->ThePlatform: Various Information
//

String  GetShortName();
String  GetVersionString();
String  GetRevisionString();
UInt    GetMainVersion()        WRAP_ThePlatformUInt(GetMainVersion())
UInt    GetSubVersion()         WRAP_ThePlatformUInt(GetSubVersion())
UInt    GetMainRevision()       WRAP_ThePlatformUInt(GetMainRevision())
UInt    GetSubRevision()        WRAP_ThePlatformUInt(GetSubRevision())
String  GetBaseLibraries();

String  GetOSName();
String  GetOSDescription();
String  GetOSVersion();
String  GetOSVendor();

String  GetHWName();
String  GetHWModel();
String  GetHWVersion();
String  GetHWVendor();

UInt    GetDataSize();
UInt    GetPhysicalPort();
String  GetPhysicalPortString();
UInt    GetLogicalPort();
String  GetLogicalPortString();
Boolean Is_i8086();
Boolean Is_i386()               WRAP_ThePlatformBoolean(Is_i386())
Boolean Is_ARC();
Boolean Is_RIM();
Boolean IsMicrosoft()           WRAP_ThePlatformBoolean(IsMicrosoft())
Boolean IsApple();
Boolean IsGoogle();
Boolean Is_Android();
Boolean Is_iOS();
Boolean Is_Symbian();
Boolean Is_Unix()               WRAP_ThePlatformBoolean(Is_Unix())
Boolean Is_Windows()            WRAP_ThePlatformBoolean(Is_Windows())
Boolean Is_OSX();
Boolean Is_WinCE();
Boolean Is_WinNT()              WRAP_ThePlatformBoolean(Is_WinNT())
Boolean Is_Linux()              WRAP_ThePlatformBoolean(Is_Linux())
Boolean Is_Debian()             WRAP_ThePlatformBoolean(Is_Debian())
Boolean Is_Darwin();
Boolean Is_Ubuntu()             WRAP_ThePlatformBoolean(Is_Ubuntu())

//----------------------------------------------------------------------
//
// TheBefe->TheCache
//

Status StartCacheFile(String const &fileName)
  WRAP_TheCache(StartCacheFile(fileName));
Status StartCacheFile(String const &fileName, Boolean writable, Boolean ignoreBOM)
  WRAP_TheCache(StartCacheFile(fileName, writable, ignoreBOM));
Status StopCacheFile(String const &fileName)
  WRAP_TheCache(StopCacheFile(fileName));
Status LoadFileBlock(Id fileId, UInt blockNo)
  WRAP_TheCache(LoadFileBlock(fileId, blockNo));
Status FlushFileBlock(Id fileId, UInt blockNo)
  WRAP_TheCache(FlushFileBlock(fileId, blockNo));
UInt   GetBlockUsedBytes(Id fileId, UInt blockNo)
  WRAP_TheCacheUInt(GetBlockUsedBytes(fileId, blockNo));
Status GetBlockUsedBytes(Id fileId, UInt blockNo, UInt &blockLen)
  WRAP_TheCache(GetBlockUsedBytes(fileId, blockNo, blockLen));
Status SetBlockUsedBytes(Id fileId, UInt blockNo, UInt blockLen)
  WRAP_TheCache(SetBlockUsedBytes(fileId, blockNo, blockLen));
Status GetBlockContents(Id fileId, UInt blockNo, UInt offset, Byte *buf, UInt len)
  WRAP_TheCache(GetBlockContents(fileId, blockNo, offset, buf, len));
Status SetBlockContents(Id fileId, UInt blockNo, UInt offset, Byte *buf, UInt len)
  WRAP_TheCache(SetBlockContents(fileId, blockNo, offset, buf, len));
UInt   GetFileBlockCount(Id fileId)
  WRAP_TheCacheUInt(GetFileBlockCount(fileId));
Status GetFileBlockCount(Id fileId, UInt &numBlocks)
  WRAP_TheCache(GetFileBlockCount(fileId, numBlocks));
Status AppendFileBlock(Id fileId)
  WRAP_TheCache(AppendFileBlock(fileId));
Status AppendFileBlocks(Id fileId, UInt numBlocks)
  WRAP_TheCache(AppendFileBlocks(fileId, numBlocks));

} // ...Namespace BEFE