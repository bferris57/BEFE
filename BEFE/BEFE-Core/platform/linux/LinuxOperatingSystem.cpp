//----------------------------------------------------------------------
// File: LinuxOperatingSystem.cpp - LinuxOperatingSystem Class Implementation
//----------------------------------------------------------------------
//
// LinuxOperatingSystem Class Implementation
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Time.h"
#include "OperatingSystem.h"
#include "Linux.h"
#include "LinuxFile.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// LinuxOperatingSystem Class
//

LinuxOperatingSystem::LinuxOperatingSystem() {
  _StartUp();
}

LinuxOperatingSystem::~LinuxOperatingSystem() {
  _ShutDown();
}

Status LinuxOperatingSystem::StartUp() {
  return _StartUp();
}

Status LinuxOperatingSystem::ShutDown() {
  return _ShutDown();
}

Status LinuxOperatingSystem::_StartUp() {

  OperatingSystem::StartUp();

  if (!TheBefe->TheOS)
    TheBefe->TheOS = this;
    
  // If no vtable, make one...
  if (*(PtrInt *)this == 0) {
    LinuxOperatingSystem x;
    *(PtrInt **)this = *(PtrInt **)&x;
  }

  //
  // Initialise members
  //

  shortname     = "linux";
  name          = "Linux";
  description.StartUp();
  version.StartUp();
  vendor        = "???";
  memoryPageSize = LinuxGetMemoryPageSize();
  fileseparator = '.';
  pathseparator = '/';
  listseparator = ':';
  hostname.StartUp();
  apps16 = false;
  apps32 = true;
#ifdef IS64BIT
  apps64 = true;
#else
  apps64 = false;
#endif
  
  return Error::None;

}

Status LinuxOperatingSystem::_ShutDown() {

  if (TheBefe->TheOS && TheBefe->TheOS == this) {
    TheBefe->TheOS = NULL;
  }

  return Error::None;

}

String LinuxOperatingSystem::GetDescription() {
  InitVars();
  return description;
}

String LinuxOperatingSystem::GetVersion() {
  InitVars();
  return version;
}

String LinuxOperatingSystem::GetHostName() {
  InitVars();
  return hostname;
}

Long LinuxOperatingSystem::GetCurrentTime() {

  struct timeval now;
  Long           t;

  gettimeofday(&now,NULL);
  t = now.tv_sec*1000 + now.tv_usec/1000 - Time::EpochDaysSince1970 * Time::SecondsPerDay;

  return t;

}

Long LinuxOperatingSystem::GetLocalTime() {

  Long           t;

  // For now...
  t = GetCurrentTime();

  return t;

}

Strings LinuxOperatingSystem::GetDeviceNames() {
  return LinuxGetDeviceNames();
}

String LinuxOperatingSystem::GetDeviceDescription(String &devname) {
  return LinuxGetDeviceDescription(devname);
}

Boolean LinuxOperatingSystem::IsFile(String const &fName) {

  Status      status;
  Boolean     exists;
  struct stat info;

  exists = false;
  status = LinuxStat(fName, &info);
  if (status) goto DONE;
  if (info.st_mode & S_IFREG)
    exists = true;

DONE:

  return exists;

}

Boolean LinuxOperatingSystem::IsDirectory(String const &dirName) {

  Status       status;
  Boolean      exists;
  struct stat  info;
  
  exists = false;
  status = LinuxStat(dirName, &info);
  if (status) goto DONE;
  if (info.st_mode & S_IFDIR)
    exists = true;
 
DONE:

  return exists;

}

Boolean LinuxOperatingSystem::IsDevice(String const &dname) {

  Status       status;
  Boolean      exists;
  struct stat  info;

  exists = false;
  status = LinuxStat(dname, &info);
  if (status) goto DONE;
  if (info.st_mode & (S_IFBLK + S_IFCHR))
    exists = true;

DONE:

  return exists;


}

Boolean LinuxOperatingSystem::Exists(String const &fileOrDirName) {


  Status       status;
  Boolean      exists;
  struct stat  info;

  exists = false;
  status = LinuxStat(fileOrDirName, &info);
  if (status) goto DONE;
  exists = true;

DONE:

  return exists;

}

Status LinuxOperatingSystem::GetFileInfo(String const &fullName, FileInfo &finfo) {

  Status       status;
  struct stat  info;

  status = LinuxStat(fullName, &info);
  if (status) goto DONE;

  finfo.name = String(fullName);
  finfo.size = info.st_size;
  finfo.creationTime = LinuxTimespecToTime(info.st_ctim);
  finfo.accessTime   = LinuxTimespecToTime(info.st_atim);
  finfo.updateTime   = LinuxTimespecToTime(info.st_mtim);

DONE:

  return status;

}

Status LinuxOperatingSystem::CreateDirectories(String &dirName) {
  
  Status  status;
  String  theDir;
  Strings parts;
  UInt    curIdx;
  
  // Make sure it's a full path...
	if (!IsFullPath(dirName)) goto NOTFULL;
	
	// Convert to Native form...
  theDir = dirName;
  status = PathToNative(theDir);
  if (status) goto SOMEERROR;
  if (Exists(theDir)) {
    if (IsDirectory(theDir)) goto OK;
    goto NOTDIR;
  }

  parts = theDir.Split('/');
  theDir = parts.Get(0);
  for (curIdx=1; curIdx < parts.Length(); curIdx++) {
    // Build subdirectory name up to here...
    theDir += '/';
    theDir += parts.Get(curIdx);
    // If it already exists, make sure it's a directory
    if (Exists(theDir)) {
      if (!IsDirectory(theDir)) goto NOTDIR;
      continue;
    }
    // Doesn't exist, create it...
    status = LinuxCreateDirectory(theDir);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
		NOTFULL:   status = Error::NotFullPath;      break;
    NOTDIR:    status = Error::FileNameIsNotDir; break;
    OK:        status = Error::None;             break;
    SOMEERROR:                                   break;
  }

  return status;
  
}

Status LinuxOperatingSystem::GetDirectories(String const &fullName, Strings &subdirs) {

  return Error::NotImplemented;

}

Status LinuxOperatingSystem::GetFiles(String const &fullName, Strings &files) {

  return Error::NotImplemented;

}

Status LinuxOperatingSystem::GetFilesInfo(String const &fullPath, FileInfos &files) {

  return Error::NotImplemented;

}

//----------------------------------------------------------------------
//
// Path/File primitives
//

Status LinuxOperatingSystem::PathToGeneric(String &path) const {

  Status  status;
  Byte   *buf;
  UInt    size;
  Byte   *foundPos;
  
  if (path.Length() == 0) goto OK;
  if (path.Count('/') != 0) goto OK;
  if (path.Count('\\') == 0) goto OK;

  if (!path.IsMutable()) {
    status = path.Mutable();
    if (status) goto SOMEERROR;
  }
  path._BufAndSize(buf, size);
  if (IsNull(buf) || size == 0) goto OK;

  // We've got some path separators in there, split and join...
  while (true) {
    foundPos = Memchr(buf, '\\', size);
    if (IsNull(foundPos)) break;
    *foundPos = '/';
    size -= foundPos - buf;
    buf = foundPos;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  return status;

}

Status LinuxOperatingSystem::PathToNative(String &path) const {

  Status  status;
  Byte   *buf;
  UInt    size;
  Byte   *foundPos;
  
  if (path.Length() == 0) goto OK;
  if (path.Count('/') == 0) goto OK;
  if (path.Count('\\') != 0) goto OK;

  if (!path.IsMutable()) {
    status = path.Mutable();
    if (status) goto SOMEERROR;
  }
  path._BufAndSize(buf, size);
  if (IsNull(buf) || size == 0) goto OK;

  // We've got some path separators in there, split and join...
  while (true) {
    foundPos = Memchr(buf, '/', size);
    if (IsNull(foundPos)) break;
    *foundPos = '\\';
    size -= foundPos - buf;
    buf = foundPos;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  return status;

}

Status LinuxOperatingSystem::PathSplit(String const &path, Strings &parts) const {

  Status status;
  Char   splitchar;

  if (path.Count('\\') != 0)
    splitchar = '\\';
  else if (path.Count('/') != 0)
    splitchar = '/';
  else
    splitchar = (char)0;

  status = Error::None;
  status = path.SplitToStrings(splitchar,parts);

  return status;

}

Status LinuxOperatingSystem::PathJoin(Strings const &parts, String &full) const {

  Status status;

  full.SetEmpty();
  full = parts.Join(pathseparator);

  status = Error::None;

  // Sanity check...
  if (parts.Length() != 0 && full.Length() == 0)
    status = Error::Internal;

  return status;

}

String LinuxOperatingSystem::FileOnly(String const &path) const {
  
  Byte   *buf;
  UInt    size;
  Byte   *sep;
  String  stripped;
  Span    span;

  path._BufAndSize(buf, size);
  span.idx1 = span.idx2 = size;
  sep = buf + size - 1;
  while (size && *sep != '\\' && *sep != '/') {
    sep--;
    size--;
    span.idx1--;
  }
  stripped = path.Get(span);      
  
  return stripped.Consumable();
  
}

String LinuxOperatingSystem::PathOnly(String const &path) const {
  
  Byte   *buf;
  UInt    size;
  Byte   *sep;
  String  stripped;
  Span    span;

  if (!IsFullPath(path))
    return path;
  path._BufAndSize(buf, size);
  span.idx1 = 0;
  span.idx2 = size - 1;
  sep = buf + size - 1;
  while (size && *sep != '\\' && *sep != '/') {
    sep--;
    size--;
    span.idx2--;
  }
  stripped = path.Get(span);      
  
  return stripped.Consumable();
  
}

Boolean LinuxOperatingSystem::IsPath(String const &path) const {
  return path.Count('\\') != 0 || path.Count('/') != 0;
}

Boolean LinuxOperatingSystem::IsFullPath(String const &path) const {

  return LinuxIsFullPath(path);

}

Boolean LinuxOperatingSystem::IsRelativePath(String const &path) const {

  return LinuxIsRelativePath(path);

}

Boolean LinuxOperatingSystem::IsLocalPath(String const &fullPath) const {
  
  return LinuxIsLocalPath(fullPath);

}

Boolean LinuxOperatingSystem::IsRemotePath(String const &fullPath) const {

  return LinuxIsRemotePath(fullPath);

}

Boolean LinuxOperatingSystem::IsValidFileName(String const &fileName) const {

  return LinuxIsValidFileName(fileName);

}

Boolean LinuxOperatingSystem::IsValidPathName(String const &fullpath) const {

  return LinuxIsValidPathName(fullpath);

}

String LinuxOperatingSystem::ApplyRelativePath(String const &path, String const &relpath) const {

  Status status;
  String replacement;

  status = ApplyRelativePath(path,relpath,replacement);
  if (status == Error::NotRelativePath)
    replacement = relpath;
  else if (status == Error::NotFullPath)
    replacement = path;
  else if (status)
    replacement.SetEmpty();

  return replacement.Consumable();

}

Status LinuxOperatingSystem::ApplyRelativePath(String const &pPath, String const &pRelPath, String &replacement) const {

  Status  status;
  String  path;
  String  relPath;
  Strings fullParts;
  UInt    numFull;
  Strings relParts;
  UInt    numRel;
  UInt    relIdx;
  String  relative;
  Char    firstChar;

  // Turn it into Native form first...
  path = pPath;
  status = PathToNative(path);
  if (status) goto SOMEERROR;
  relPath = pRelPath;
  status = PathToNative(relPath);
  if (status) goto SOMEERROR;
  
  // Special case for leading '~'...
  firstChar = relPath.Get(0);
  if (firstChar == '~'       &&
      !BEFE::IsNull(TheBefe) && 
      !BEFE::IsNull(TheBefe->TheProcess)) {
    String homeVar;
    homeVar = TheBefe->TheProcess->GetEnvironmentValue("USERPROFILE");
    if (homeVar.Length())
      relPath = homeVar + relPath.Get(Span(1,NaN));
  }
  
  // Special case for leading '!'...
  firstChar = relPath.Get(0);
  if (firstChar == '!'       &&
      !BEFE::IsNull(TheBefe) &&
      !BEFE::IsNull(TheBefe->TheProcess)) {
    String exeVar;
    exeVar = TheBefe->TheProcess->GetExecutablePath();
    if (exeVar.Length())
      relPath = exeVar + relPath.Get(Span(1,NaN));
  }

  // Special case for leading '@'...
  firstChar = relPath.Get(0);
  if (firstChar == '@'       &&
      !BEFE::IsNull(TheBefe) &&
      !BEFE::IsNull(TheBefe->TheProcess)) {
    String homeVar;
    homeVar = TheBefe->GetEnvironmentValue("BEFE_Home");
    if (!homeVar.Length())
      homeVar = TheBefe->TheProcess->GetExecutablePath();
    if (homeVar.Length())
      relPath = homeVar + relPath.Get(Span(1,NaN));
  }

  // Validate them  
  if (!IsFullPath(path)) goto NOTFULLPATH;
  if (IsFullPath(relPath)) {
    replacement = relPath.Consumable();
    goto OK;
  }
  if (!IsRelativePath(relPath)) goto NOTRELPATH;

  // Split both of them
  status  = PathSplit(path,fullParts);
  if (status) goto SOMEERROR;
  status  = PathSplit(relPath,relParts);
  if (status) goto SOMEERROR;
  numFull = fullParts.Length();
  numRel  = relParts.Length();

  // Get rid of trailing empty parts
  if (numFull > 0 && fullParts.Get(-1).Length() == 0) {
    status = fullParts.Remove(-1);
    if (status) goto SOMEERROR;
    numFull--;
  }
  if (numRel > 0 && relParts.Get(-1).Length() == 0) {
    status = relParts.Remove(-1);
    if (status) goto SOMEERROR;
    numRel--;
  }

  // Special case for leading '\'...
  firstChar = relPath.Get(0);
  if (firstChar == '\\' || firstChar == '/') {
    // Clear all but first part of absolute path
    while (numFull > 1) {
      status = fullParts.Remove(-1);
      if (status) goto SOMEERROR;
      numFull--;
    }
    // Remove first part of relative path (the empty part)
    status = relParts.Remove(0);
    if (status) goto SOMEERROR;
    numRel--;
  }

  // Until we've exhausted relative parts...
  for (relIdx=0; relIdx < numRel; relIdx++) {
    relative = relParts.Get(relIdx);
    if (relative == "..") {
      if (numFull <= 1) goto DEPTHMISMATCH;
      status = fullParts.Remove(-1);
      if (status) goto SOMEERROR;
      numFull--;
    }
    else if (relative == ".") {
      continue;
    }
    else {
      status = fullParts.Append(relative);
      if (status) goto SOMEERROR;
      numFull++;
    }
  }

  // Join the replacement together
  status = PathJoin(fullParts,replacement);
  if (status) goto SOMEERROR;

  // If there's only one part, append a '\'
  if (fullParts.Length() == 1)
    replacement.Append(pathseparator);

  // Handle errors
  status = Error::None;
  while (false) {
    NOTFULLPATH:   status = Error::NotFullPath;       break;
    NOTRELPATH:    status = Error::NotRelativePath;   break;
    DEPTHMISMATCH: status = Error::PathDepthMismatch; break;
    OK:            status = Error::None;              break;
    SOMEERROR:                                        break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// File and Directory Move/Copy
//

Status LinuxOperatingSystem::_LowMoveFile(String const &fromNativeFileName, String const &toNativeFileName) {
  
    return Error::NotImplemented;

}

Status LinuxOperatingSystem::_LowCopyFile(String const &fromNativeFileName, String const &toNativeFileName) {
  
    return Error::NotImplemented;

}

Status LinuxOperatingSystem::_LowDeleteFile(String const &nativeFileName) {
  
    return Error::NotImplemented;

}

/*
Status LinuxOperatingSystem::_LowMoveDirectory(String const &fromDirName, String const &toDirName) {
  return Error::PlatformNotCapable;
}

Status LinuxOperatingSystem::_LowCopyDirectory(String const &fromDirName, String const &toDirName) {
  return Error::PlatformNotCapable;
}

Status LinuxOperatingSystem::_LowDeleteDirectory(String const &dirName) {
  return Error::PlatformNotCapable;
}
*/

//----------------------------------------------------------------------
//
// Factories
//

File *LinuxOperatingSystem::NewFile() {
  LinuxFile *that;
  that = new LinuxFile();
  return that;
}

void LinuxOperatingSystem::FinishedWith(File *file) {
	if (file)
    delete file;
}

//----------------------------------------------------------------------
//
// Temporary Files
//

String LinuxOperatingSystem::GetTempPath() {

  String varname;
  String temppath;
  String tempvar;
  String warn1;
  String warn2;
  
  static UInt numWarns = 0;
  
  // Try to find it everywhere...
  if (temppath.Length() == 0) {
    tempvar = "BEFE_Temp";
    temppath = TheBefe->TheProcess->GetEnvironmentValue(String(tempvar));
  }
  if (temppath.Length() == 0) {
    tempvar = "BEFE_Root";
    temppath = TheBefe->TheProcess->GetEnvironmentValue(String("BEFE_Root"));
  }
  if (temppath.Length() == 0) {
    tempvar = "TEMP";
    temppath = TheBefe->TheProcess->GetEnvironmentValue(String("TEMP"));
  }
  if (temppath.Length() == 0) {
    tempvar = "TMP";
    temppath = TheBefe->TheProcess->GetEnvironmentValue(String("TMP"));
  }

  // Trim it and strip off any trailing path separators
  temppath = temppath.Trim();
  if (temppath.Get(-1) == '\\' || temppath.Get(-1) == '/')
    temppath = temppath.Get(Slice("0:-1"));

  // Validate it
  if (temppath.Length() == 0) {
    warn1 = "Cannot determine temporary file path from BEFE_Temp, BEFE_Root, TEMP, or TMP";
    warn2 = "Using current directory instead";
  }
  else if (!Exists(temppath) || !IsDirectory(temppath)) {
    warn1  = "Temporary file path: ";
    warn1 += tempvar;
    warn1 += " = ";
    warn1 += temppath;
    if (!Exists(temppath))
      warn2 = "Temporary file path does not exist";
    else
      warn2 = "Temporary file path exists but is not a directory";
  }
  
  if (warn1.Length() || warn2.Length()) {
    if (!numWarns++) {
      BEFE_WARN(warn1);
      BEFE_WARN(warn2);
    }
  }
  
  return temppath;

}

String LinuxOperatingSystem::GetTempFileName() {

  String    temppath;
  String    tempfile;
  LinuxFile file;

  temppath = GetTempPath();
  tempfile = LinuxFileGetTempFileName();
  if (temppath.Length() && temppath.Get(-1) != '\\')
    temppath = temppath+'\\';
  temppath += tempfile;

  // Make sure it doesn't exist
  //
  // Note: Actually, this is better done at temp file open time so
  //       we'll leave it commented for now...

  //file.SetName(tempfile);
  //if (file.Exists())
  //  temppath.Clear();

  return temppath;

}

//----------------------------------------------------------------------
//
// UUIDs
//

Status LinuxOperatingSystem::NewRandomUUID(UUID &newUUID) {
  
    return Error::NotImplemented;

}

//----------------------------------------------------------------------
//
// Internal Methods
//

Status LinuxOperatingSystem::InitVars() {

  return Error::None;
  
}

} // ...Namespace BEFE
