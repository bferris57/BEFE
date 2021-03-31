//----------------------------------------------------------------------
// File: OperatingSystem.cpp - Generic OperatingSystem class implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// NOTE: The "real" implementation of these classes is up to the
//       specific port that the product was built for.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "OperatingSystem.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

OperatingSystem::OperatingSystem() {
  StartUp();
}

OperatingSystem::~OperatingSystem() {
  ShutDown();
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status OperatingSystem::StartUp() {

  Status status;

  if (this == TheBefe->TheOS) goto OK;

  shortname      = CONSTR_Generic;
  name           = CONSTR_Generic;
  description    = CONSTR_Generic;
  //description   += " Operating System";
  version        = CONSTR_Unknown;
  vendor         = "Bruce Ferris (UK)";
  memoryPageSize = UNaN;
  fileseparator  = '.';
  pathseparator  = '/';
  listseparator  = ',';
  hostname       = CONSTR_Unavailable;
  macaddresses   = CONSTR_Unavailable;
  ipv4addresses  = CONSTR_Unavailable;
  ipv6addresses  = CONSTR_Unavailable;
  searchPaths.StartUp();
  apps16 = false;
  apps32 = false;
  apps64 = false;
  
  // Handle errors
  status = Error::None;
  while (false) {
	OK: status = Error::None; break;
  }

  return status;
}

Status OperatingSystem::ShutDown() {

  shortname.ShutDown();
  name.ShutDown();
  description.ShutDown();
  version.ShutDown();
  hostname.ShutDown();
  macaddresses.ShutDown();
  ipv4addresses.ShutDown();
  ipv6addresses.ShutDown();
  searchPaths.ShutDown();

  return Error::None;

}

Status OperatingSystem::Reset() {

  shortname.Reset();
  name.Reset();
  description.Reset();
  version.Reset();
  hostname.Reset();
  macaddresses.Reset();
  ipv4addresses.Reset();
  ipv6addresses.Reset();
  searchPaths.Reset();

  return Error::None;

}

//----------------------------------------------------------------------
//
// Member Get Methods
//

String OperatingSystem::GetShortName() {
  return shortname;
}

String OperatingSystem::GetName() {
  return name;
}

String OperatingSystem::GetDescription() {
  return description;
}

String OperatingSystem::GetVersion() {
  return version;
}

String OperatingSystem::GetVendor() {
  return vendor;
}

UInt OperatingSystem::GetMemoryPageSize() {
  return memoryPageSize;
}

String OperatingSystem::GetHostName() {
  return hostname;
}

Boolean OperatingSystem::GetApps16() {
  return apps16;
}

Boolean OperatingSystem::GetApps32() {
  return apps32;
}

Boolean OperatingSystem::GetApps64() {
  return apps64;
}

Char OperatingSystem::GetFileSeparator() {
  return fileseparator;
}

Char  OperatingSystem::GetPathSeparator() {
  return pathseparator;
}

Char  OperatingSystem::GetListSeparator() {
  return listseparator;
}

//----------------------------------------------------------------------
//
// More info about Operating System...
//

Long OperatingSystem::GetCurrentTime() {
 return 0;
}

Long OperatingSystem::GetLocalTime() {
 return 0;
}

Long OperatingSystem::GetUTCTimeDiff() {
 return (GetLocalTime() - GetCurrentTime())/1000*1000;
}

Strings OperatingSystem::GetDeviceNames() {
  Strings devices;
  return devices;
}

String OperatingSystem::GetDeviceDescription(String &devname) {
  String status;
  status = "**Device description not available**";
  return status;
}

//----------------------------------------------------------------------
//
// Existence/Query Methods
//

Boolean OperatingSystem::IsFile(String const &fname) {
  return false;
}

Boolean OperatingSystem::IsDirectory(String const &dirname) {
  return false;
}

Boolean OperatingSystem::IsDevice(String const &dname) {
  return false;
}

Boolean OperatingSystem::Exists(String const &fileordirname) {
  return false;
}

Status OperatingSystem::GetFileInfo(String const &fname, FileInfo &info) {
  info.Reset();
  return Error::NotCapable;
}

FileInfo OperatingSystem::GetFileInfo(String const &fname) {
  FileInfo info;
  GetFileInfo(fname,info);
  return info;
}

Status OperatingSystem::CreateDirectories(String &dirname) {
  return Error::NotCapable;
}

Status OperatingSystem::GetDirectories(String const &fullpath, Strings &subdirs) {
  return Error::NotCapable;
}

Strings OperatingSystem::GetDirectories(String const &fullpath) {

  Strings strings;

  GetDirectories(fullpath,strings);

  return strings;

}

Status OperatingSystem::GetFiles(String const &fullpath, Strings &files) {
  return Error::NotCapable;
}

Strings OperatingSystem::GetFiles(String const &fullpath) {

  Strings strings;

  GetFiles(fullpath,strings);

  return strings.Consumable();

}

Status OperatingSystem::GetFilesInfo(String const &fullPath, FileInfos &fileInfos) {
  
  Status    status;
  Strings   fileNames;
  FileInfos localInfos;
  FileInfo  someInfo;
  UInt      curIdx;

  // Get the list of files
  status = GetFiles(fullPath, fileNames);
  if (status) goto SOMEERROR;
  status = localInfos.CopyFrom(fileInfos);
  if (status) goto SOMEERROR;
  
  // For each file...
  for (curIdx=0; curIdx < fileNames.Length(); curIdx++) {
    status = GetFileInfo(fileNames.Get(curIdx), someInfo);
    if (status) goto SOMEERROR;
    status = localInfos.Append(someInfo.Consumable());
    if (status) goto SOMEERROR;
  }
  
  // Copy to caller's info...
  status = fileInfos.MoveFrom(localInfos);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

FileInfos OperatingSystem::GetFilesInfo(String const &fullpath) {

  FileInfos files;

  GetFilesInfo(fullpath, files);

  return files.Consumable();

}

//----------------------------------------------------------------------
//
// Path/file primitives
//

Status OperatingSystem::PathToGeneric(String &path) const {
  return Error::None;
}

Status OperatingSystem::PathToNative(String &path) const {
  return Error::None;
}

Status OperatingSystem::PathSplit(String const &path, Strings &parts) const {
  return Error::NotCapable;
}

Status OperatingSystem::PathJoin(Strings const &parts, String &full) const {
  return Error::NotCapable;
}

String OperatingSystem::FileOnly(String const &path) const{
  return String();
}

String OperatingSystem::PathOnly(String const &path) const{
  return String();
}

Boolean OperatingSystem::IsPath(String const &path) const {
  return false;
}

Boolean OperatingSystem::IsFullPath(String const &path) const {
  return false;
}

Boolean OperatingSystem::IsRelativePath(String const &path) const {
  return false;
}

Boolean OperatingSystem::IsLocalPath(String const &path) const {
  return false;
}

Boolean OperatingSystem::IsRemotePath(String const &path) const {
  return false;
}

Boolean OperatingSystem::IsValidFileName(String const &filename) const {
  return false;
}

Boolean OperatingSystem::IsValidPathName(String const &path) const {
  return false;
}

String OperatingSystem::ApplyRelativePath(String const &relpath) const {
  return ApplyRelativePath(GetCurrentWorkingDirectory(), relpath);
}

String OperatingSystem::ApplyRelativePath(String const &path, String const &relpath) const {
  //Status status;
  String replacement;
  /*status =*/ ApplyRelativePath(path, relpath, replacement);
  return replacement.Consumable();
}

Status OperatingSystem::ApplyRelativePath(String const &path, String const &relpath, String &replacement) const {
  return Error::NotCapable;
}

Status OperatingSystem::WalkPath(char const *dir, OSPathWalker walker, UInt32 context) {
  return WalkPath(String(dir),walker,context);
}

Status OperatingSystem::WalkPath(String const &baseDir, OSPathWalker walker, UInt32 context) {

  Status  status;
  Strings dirs;
  UInt    depth;

  Char    pathSep;
  String  dir;
  Strings subDirs;
  String  subDir;
  UInt    numSubDirs;
  UInt    subDirIdx;

  // Validate parameters
  if (IsNull((void *)walker)) goto INVALIDPARAMETERS;

  // Make sure it's a full path...
	if (!IsFullPath(baseDir)) goto NOTFULL;
	
  // Set up initial stack state
  depth = 0;
  pathSep = TheBefe->TheOS->GetPathSeparator();
  dir = baseDir;
  status = TheBefe->TheOS->PathToNative(dir);
  if (status) goto SOMEERROR;
  
  // Call walker for Base Directory...
  status = (*walker)(dir,context);
  if (status) goto SOMEERROR;  
  status = dirs.Append(dir);
  if (status) goto SOMEERROR;
  depth++;

  while (depth) {

    // Get Top one
    status = dirs.Get(-1,dir);
    if (status) goto SOMEERROR;
    status = dirs.Remove(-1);
    if (status) goto SOMEERROR;
    depth--;

    // Get the list of Sub-Directories...
    subDirs.SetEmpty();
    status = TheBefe->TheOS->GetDirectories(dir,subDirs);
    if (status) goto SOMEERROR;
    numSubDirs = subDirs.Length();

    // For each one...
    for (subDirIdx=0; subDirIdx < numSubDirs; subDirIdx++) {

      subDir = subDirs.Get(subDirIdx);

      // Call the Walker
      status = (*walker)(subDir,context);
      if (status) goto SOMEERROR;

      // Push this on the stack
      status = dirs.Append(subDir);
      if (status) goto SOMEERROR;
      depth++;

    }

  }

  // Handle errors
  while (false) {
    INVALIDPARAMETERS: status = Error::InvalidParameter; break;
		NOTFULL:           status = Error::NotFullPath;      break;
    SOMEERROR:                                           break;
  }

  return status;

}

Status OperatingSystem::SetDefaultSearchPaths() {

  Status  status;
  String  cwd;
  Char    listSep;
  String  searchPath;
  Strings tPaths;
  UInt    numPaths;
  String  tPath;
  UInt    strIdx;
  UInt    tIdx;
  String  tString;

  if (BEFE::IsNull(TheBefe)) goto NOBEFE;
  if (BEFE::IsNull(TheBefe->TheOS)) goto NOOS;
  if (BEFE::IsNull(TheBefe->TheProcess)) goto NOPROCESS;
  
  cwd     = TheBefe->TheProcess->GetCurrentWorkingDirectory();
  listSep = TheBefe->TheOS->GetListSeparator();

  // Pull apart BEFE_Path if set...
  searchPath = TheBefe->GetEnvironmentValue(String("BEFE_Path"));
	searchPath = TheBefe->SubstituteEnvironmentValues(searchPath);
  tPaths = searchPath.SplitToStrings(listSep);
  numPaths = tPaths.Length();
  for (strIdx=0; strIdx < numPaths; strIdx++) {
    tPath = tPaths.Get(strIdx);
    tPath = TheBefe->TheOS->ApplyRelativePath(cwd,tPath);
    // NOTE: Need to add a Sparse here to reduce allocations
    for (tIdx=0; tIdx < searchPaths.Length(); tIdx++) {
      tString = searchPaths.Get((Int)tIdx);
      if (tString.CompareInsensitive(tPath) == 0) break;
    }
    if (tIdx >= searchPaths.Length()) {
      tPath.Consumable();
      searchPaths.Append(tPath);
    }
  }

  // Pull apart Path if BEFE_Path not set...
  if (numPaths == 0) {
    searchPath = TheBefe->GetEnvironmentValue(String("Path"));
    tPaths = searchPath.SplitToStrings(listSep);
    for (strIdx=0; strIdx < tPaths.Length(); strIdx++) {
      tPath = tPaths.Get(strIdx);
      tPath = TheBefe->TheOS->ApplyRelativePath(cwd,tPath);
      for (tIdx=0; tIdx < searchPaths.Length(); tIdx++) {
        tString = searchPaths.Get((Int)tIdx);
        if (tString.CompareInsensitive(tPath) == 0) break;
      }
      if (tIdx >= searchPaths.Length()) {
        tPath.Consumable();
        searchPaths.Append(tPath);
      }
    }
  }
	
  // Handle errors
  status = Error::None;
  while (false) {
    NOBEFE:    status = Error::NoBefe;               break;
    NOOS:      status = Error::InternalNoTheOS;      break;
    NOPROCESS: status = Error::InternalNoTheProcess; break;
  }
  
  return status;

}

Strings OperatingSystem::GetSearchPaths() {

  if (searchPaths.Length() == 0)
    SetDefaultSearchPaths();

  return searchPaths;

}

String OperatingSystem::FindInSearchPath(String  const &fName) {

  String found;

  FindInSearchPath(fName, Strings(), found);

  return found.Consumable();

}

String OperatingSystem::FindInSearchPath(String  const &fName, Strings const &addExts) {

  String found;

  FindInSearchPath(fName, addExts, found);

  return found.Consumable();

}

Status OperatingSystem::FindInSearchPath(String const &fName, Strings const &addExts, String &found) {

  Status  status;
  Char    pathSep;
  UInt    numPaths;
  String  searchPath;
  UInt    strIdx;
  String  tString;
  UInt    numExts;
  UInt    extIdx;

  pathSep = TheBefe->TheOS->GetPathSeparator();
  numExts = addExts.Length();

  // Special case if it's a full path...
  if (TheBefe->TheOS->IsFullPath(fName) || fName[0] == '~') {
    if (TheBefe->TheOS->Exists(fName) && TheBefe->TheOS->IsFile(fName))
      found = fName;
    goto OK;
  }
  
  // Populate search paths if they're empty...
  if (searchPaths.Length() == 0)
    SetDefaultSearchPaths();

  // Now, try each one...
  numPaths = searchPaths.Length();
  for (strIdx=0; strIdx < numPaths; strIdx++) { // For each Search Path...

    // Get the search path
    searchPath = searchPaths.Get(strIdx);
    PathToNative(searchPath);
    if (searchPath.Get(-1) != pathSep)
      searchPath.Append(pathSep);

    // Try with just the raw file name...
    found = searchPath;
    found.Append(fName);
    if (IsFile(found)) break;
    
    // Try each extension...
    for (extIdx=0; extIdx < numExts; extIdx++) { // For each extension...
      tString = addExts.Get(extIdx);
      found = searchPath;
      found.Append(fName);
      found.Append(tString);
      if (IsFile(found)) break;
    } // ...For each extension

    // Finished if found it
    if (extIdx < numExts) break;

  }  // ...For each Search Path

  // If didn't find it...
  status = Error::None;
  if (strIdx >= numPaths) {
    found.SetEmpty();
    status = Error::FileDoesNotExist;
  }

  // Handle errors
  while (false) {
    OK: status = Error::None; break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// File and Directory Move/Copy
//

Status OperatingSystem::MoveFile(String const &pFromFileName, String const &pToFileName) {
  
  Status status;
  String fromFileName;
  String toFileName;
  
  if (IsNull(TheBefe)) goto NOBEFE;
  fromFileName = pFromFileName;
	if (!IsFullPath(pFromFileName)) goto NOTFULL;
	if (!IsFullPath(pToFileName))   goto NOTFULL;
  status = PathToNative(fromFileName);
  if (status) goto SOMEERROR;  
  toFileName = pToFileName;
  status = PathToNative(toFileName);
  if (status) goto SOMEERROR;  
  status = TheBefe->ValidateExistingFileName(fromFileName);
  if (status) goto SOMEERROR;
  status = TheBefe->ValidateNewFileName(toFileName);
  if (status) goto SOMEERROR;

  status = _LowMoveFile(fromFileName, toFileName);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
		NOTFULL:   status = Error::NotFullPath; break;
    NOBEFE:    status = Error::NoBefe;      break;
    SOMEERROR:                              break;
  }
  
  return status;
  
}

Status OperatingSystem::CopyFile(String const &pFromFileName, String const &pToFileName) {
  
  Status status;
  String fromFileName;
  String toFileName;
  
  if (IsNull(TheBefe)) goto NOBEFE;
  fromFileName = pFromFileName;
	if (!IsFullPath(pFromFileName)) goto NOTFULL;
	if (!IsFullPath(pToFileName))   goto NOTFULL;
  status = PathToNative(fromFileName);
  if (status) goto SOMEERROR;  
  toFileName = pToFileName;
  status = PathToNative(toFileName);
  if (status) goto SOMEERROR;  
  status = TheBefe->ValidateExistingFileName(fromFileName);
  if (status) goto SOMEERROR;
  status = TheBefe->ValidateNewFileName(toFileName);
  if (status) goto SOMEERROR;

  status = _LowCopyFile(fromFileName, toFileName);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
		NOTFULL:   status = Error::NotFullPath; break;
    NOBEFE:    status = Error::NoBefe;      break;
    SOMEERROR:                              break;
  }
  
  return status;
  
}

Status OperatingSystem::DeleteFile(String const &pFileName) {
  
  Status status;
  String fileName;
  
  if (IsNull(TheBefe)) goto NOBEFE;
  fileName = pFileName;
	if (!IsFullPath(pFileName)) goto NOTFULL;
  status = PathToNative(fileName);
  if (status) goto SOMEERROR;  
  status = TheBefe->ValidateExistingFileName(fileName);
  if (status) goto SOMEERROR;

  status = _LowDeleteFile(fileName);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
		NOTFULL:   status = Error::NotFullPath; break;
    NOBEFE:    status = Error::NoBefe;      break;
    SOMEERROR:                              break;
  }
  
  return status;
  
}

Status OperatingSystem::MoveDirectory(String const &fromDirName, String const &toDirName) {
	
	Status status;
	
	if (!IsFullPath(fromDirName)) goto NOTFULL;
	if (!IsFullPath(toDirName)) goto NOTFULL;
	if (!IsDirectory(fromDirName)) goto NOTDIR;
	if (Exists(toDirName)) goto EXISTS;
	
	status = _LowMoveDirectory(fromDirName, toDirName);
	if (status) goto SOMEERROR;
	
	// Handle errors
	while (false) {
		NOTFULL:   status = Error::NotFullPath;          break;
		NOTDIR:    status = Error::FileDirDoesNotExist;  break;
		EXISTS:    status = Error::FileDirAlreadyExists; break;
		SOMEERROR:                                       break;
	}
	
  return status;
	
}

Status OperatingSystem::CopyDirectory(String const &fromDirName, String const &toDirName) {
	
	Status status;
	
	if (!IsFullPath(fromDirName)) goto NOTFULL;
	if (!IsFullPath(toDirName)) goto NOTFULL;
	if (!IsDirectory(fromDirName)) goto NOTDIR;
	if (Exists(toDirName) && !IsDirectory(toDirName)) goto NOTDIR;
	
	status = _LowCopyDirectory(fromDirName, toDirName);
	if (status) goto SOMEERROR;
	
	// Handle errors
	while (false) {
		NOTFULL:   status = Error::NotFullPath;          break;
		NOTDIR:    status = Error::FileDirDoesNotExist;  break;
		SOMEERROR:                                       break;
	}
	
  return status;
	
}

Status OperatingSystem::DeleteDirectory(String const &dirName) {
	
	Status status;
	
	if (!IsFullPath(dirName)) goto NOTFULL;
	if (!Exists(dirName)) goto NOTEXIST;
	if (!IsDirectory(dirName)) goto NOTDIR;
	
	status = _LowDeleteDirectory(dirName);
	if (status) goto SOMEERROR;
	
	// Handle errors
	while (false) {
		NOTFULL:   status = Error::NotFullPath;          break;
		NOTDIR:    status = Error::FileDirDoesNotExist;  break;
		NOTEXIST:  status = Error::FileDirAlreadyExists; break;
		SOMEERROR:                                       break;
	}
	
  return status;
	
}

Status OperatingSystem::_LowMoveFile(String const &fromNativeFileName, String const &toNativeFileName) {
  return Error::PlatformNotCapable;
}

Status OperatingSystem::_LowCopyFile(String const &fromNativeFileName, String const &toNativeFileName) {
  return Error::PlatformNotCapable;
}

Status OperatingSystem::_LowDeleteFile(String const &nativeFileName) {
  return Error::PlatformNotCapable;
}

Status OperatingSystem::_LowMoveDirectory(String const &fromNativeDirName, String const &toNativeDirName) {
  return Error::PlatformNotCapable;
}

Status OperatingSystem::_LowCopyDirectory(String const &fromNativeDirName, String const &toNativeDirName) {
  return Error::PlatformNotCapable;
}

Status OperatingSystem::_LowDeleteDirectory(String const &nativeDirName) {
  return Error::PlatformNotCapable;
}

//----------------------------------------------------------------------
//
// Factories
//

File *OperatingSystem::NewFile() {
  return NULL;
}

void OperatingSystem::FinishedWith(File *file) {
}

//----------------------------------------------------------------------
//
// Temporary Files
//

String OperatingSystem::GetTempPath() {
  return CONSTR_NotAvailable;
}

String OperatingSystem::GetTempFileName() {
  return String();
}

//----------------------------------------------------------------------
//
// UUIDs
//

Status OperatingSystem::NewRandomUUID(UUID &newUUID) {
  
  Status status;
  
  newUUID = UUID();
  status = Error::NotCapable;
  
  return status;
  
}

} // ...Namespace BEFE
