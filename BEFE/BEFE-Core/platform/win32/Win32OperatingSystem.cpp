//----------------------------------------------------------------------
// File: Win32OperatingSystem.cpp - Win32OperatingSystem Class Implementation
//----------------------------------------------------------------------
//
// Win32OperatingSystem Class Implementation
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Time.h"
#include "OperatingSystem.h"
#include "Win32.h"
#include "Win32File.h"
#include "Win32WCHAR.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Win32OperatingSystem Class
//

Win32OperatingSystem::Win32OperatingSystem() {
  _StartUp();
}

Win32OperatingSystem::~Win32OperatingSystem() {
  _ShutDown();
}

Status Win32OperatingSystem::StartUp() {
  return _StartUp();
}

Status Win32OperatingSystem::ShutDown() {
  return _ShutDown();
}

Status Win32OperatingSystem::_StartUp() {

  Byte buf[MAX_PATH];
  UInt dirLen;
  
  OperatingSystem::StartUp();

  if (!TheBefe->TheOS)
    TheBefe->TheOS = this;
    
  // If no vtable, make one...
  if (*(UInt *)this == 0) {
    Win32OperatingSystem x;
    *(Int32 **)this = *(Int32 **)&x;
  }

  //
  // Initialise members
  //

  shortname     = "win32";
  name          = "Windows32";
  description.StartUp();
  version.StartUp();
  vendor        = "Microsoft";
  memoryPageSize = Win32GetMemoryPageSize();
  fileseparator = '.';
  pathseparator = '\\';
  listseparator = ';';
  hostname.StartUp();
  apps16 = true;
  apps32 = true;
  dirLen = GetSystemWow64Directory((LPTSTR)&buf[0], sizeof(buf)-1);
  apps64 = dirLen != 0;
  
  return Error::None;

}

Status Win32OperatingSystem::_ShutDown() {

  if (TheBefe->TheOS && TheBefe->TheOS == this) {
    TheBefe->TheOS = NULL;
  }

  return Error::None;

}

String Win32OperatingSystem::GetDescription() {
  InitVars();
  return description;
}

String Win32OperatingSystem::GetVersion() {
  InitVars();
  return version;
}

String Win32OperatingSystem::GetHostName() {
  InitVars();
  return hostname;
}

Long Win32OperatingSystem::GetCurrentTime() {

  SYSTEMTIME     systemTime;
  FILETIME       fileTime;
  Long           t;

  GetSystemTime( &systemTime );
  SystemTimeToFileTime( &systemTime, &fileTime );
  t = Win32FileTimeToTime(fileTime);

  return t;

}

Long Win32OperatingSystem::GetLocalTime() {

  SYSTEMTIME     localTime;
  FILETIME       fileTime;
  Long           t;

  ::GetLocalTime( &localTime );
  SystemTimeToFileTime( &localTime, &fileTime );
  t = Win32FileTimeToTime(fileTime);

  return t;

}

Strings Win32OperatingSystem::GetDeviceNames() {
  return Win32GetDeviceNames();
}

String Win32OperatingSystem::GetDeviceDescription(String &devname) {
  return Win32GetDeviceDescription(devname);
}

Boolean Win32OperatingSystem::IsFile(String const &fName) {

  Boolean  exists;
  Status   status;
  UShort   buf[MAX_PATH];
  UInt     bufl;
  UInt     attributes;
  UInt     genCount;
  UInt     natCount;
  UShort  *tBuf;
  
  exists = true;
  
  // Doesn't exist if it doesn't have a name
  if (fName.Length() == 0) goto NOPE;

  // Make sure it's a full path...
	if (!IsFullPath(fName)) goto NOPE;
	
  // Get the full name bytes
  status = Win32W_FromString(fName, buf, MAX_PATH, bufl);
  if (status) goto NOPE;
  if (bufl) 
    bufl--;
    
  // Convert to Native format if needed
  natCount = fName.Count('\\');
  genCount = fName.Count('/');
  if (genCount && !natCount)
    for (tBuf=buf; *tBuf; tBuf++) if (*tBuf == '/') *tBuf = '\\';
  
  // Strip off trailing '\'
  if (bufl && buf[bufl-1] == '\\')
    buf[bufl-1] = 0x00;

  // Get the attributes
  attributes = GetFileAttributesW((LPCWSTR)buf);
  if (attributes == INVALID_FILE_ATTRIBUTES ||
      (attributes & FILE_ATTRIBUTE_DIRECTORY)
	 ) goto NOPE;

  // Handle errors
  while (false) {
    NOPE: exists = false; break;
  }
  
  return exists;

}

Boolean Win32OperatingSystem::IsDirectory(String const &dirName) {

  Boolean  exists;
  Status   status;
  UShort   buf[MAX_PATH];
  UInt     bufl;
  UInt     attributes;
  UInt     genCount;
  UInt     natCount;
  UShort  *tBuf;
  
  // Doesn't exist if it doesn't have a name
  if (dirName.Length() == 0) goto NOPE;

  // Doesn't exist if it's not a full path...
	if (!IsFullPath(dirName)) goto NOPE;
	
  // Get the full name bytes
  status = Win32W_FromString(dirName, buf, MAX_PATH, bufl);
  if (status) goto NOPE;
  if (bufl) 
    bufl--;
    
  // Convert to Native format if needed
  natCount = dirName.Count('\\');
  genCount = dirName.Count('/');
  if (genCount && !natCount)
    for (tBuf=buf; *tBuf; tBuf++) if (*tBuf == '/') *tBuf = '\\';
  
  // Strip off trailing '\'
  if (bufl && buf[bufl-1] == '\\')
    buf[bufl-1] = 0x00;

  // Get the attributes
  attributes = GetFileAttributesW((LPCWSTR)buf);
  if (attributes == INVALID_FILE_ATTRIBUTES) goto NOPE;

  if (attributes & FILE_ATTRIBUTE_DIRECTORY) goto YEP;

  // Handle errors...
  exists = false;
  while (false) {
    NOPE: exists = false; break;
    YEP:  exists = true;  break;
  }
 
  return exists;

}

Boolean Win32OperatingSystem::IsDevice(String const &fname) {

  Strings devNames;
  UInt    numNames;
  UInt    nameIdx;
  String  name;

  devNames = Win32GetDeviceNames();
  numNames = devNames.Length();
  for (nameIdx=0; nameIdx < numNames; nameIdx++) {
    name = devNames.Get(nameIdx);
    if (name.CompareInsensitive(fname) == 0) break;
  }

  return nameIdx < numNames;

}

Boolean Win32OperatingSystem::Exists(String const &fileOrDirName) {

  Status   status;
  Boolean  exists;
  UShort   buf[MAX_PATH];
  UInt     bufl;
  UInt     attributes;
  UInt     genCount;
  UInt     natCount;
  UShort  *tBuf;
  
  // Doesn't exist if it doesn't have a name or it's not a full path...
  if (fileOrDirName.Length() == 0) goto NOPE;
  if (!IsFullPath(fileOrDirName)) goto NOPE;
	
  // Get the full name bytes
  status = Win32W_FromString(fileOrDirName, buf, MAX_PATH, bufl);
  if (status) goto NOPE;
  if (bufl) 
    bufl--;
    
  // Convert to Native format if needed
  natCount = fileOrDirName.Count('\\');
  genCount = fileOrDirName.Count('/');
  if (genCount && !natCount)
    for (tBuf=buf; *tBuf; tBuf++) if (*tBuf == '/') *tBuf = '\\';
  
  // Strip off any trailing '\'
  if (bufl && buf[bufl-1] == '\\')
    buf[bufl-1] = 0x00;

  // Try getting the file attributes
  attributes = GetFileAttributesW((LPCWSTR)buf);
  if (attributes == INVALID_FILE_ATTRIBUTES) goto NOPE;

  // Handle errors
  exists = true;
  while (false) {
    NOPE: exists = false; break;
  }

  return exists;

}

Status Win32OperatingSystem::GetFileInfo(String const &fullName, FileInfo &info) {

  Status           status;
  UShort           buf[MAX_PATH];
  UInt             bufl;
  WIN32_FIND_DATAW findData;
  HANDLE           findHandle;
  UInt             genCount;
  UInt             natCount;
  UShort          *tBuf;

  info.Reset();

  // Invalid if not full path...
	if (!IsFullPath(fullName)) goto NOTFULL;
	
  // Get the name into our local buf
  status = Win32W_FromString(fullName, buf, MAX_PATH, bufl);
  if (status) goto SOMEERROR;
  if (bufl) 
    bufl--;
    
  // Convert to Native format if needed
  natCount = fullName.Count('\\');
  genCount = fullName.Count('/');
  if (genCount && !natCount)
    for (tBuf=buf; *tBuf; tBuf++) if (*tBuf == '/') *tBuf = '\\';

  // Try finding it
  findHandle = FindFirstFileW((LPCWSTR)buf, &findData);
  if (findHandle == INVALID_HANDLE_VALUE) goto NOTEXIST;
  FindClose(findHandle);

  // Make sure it's a file
  if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) goto ISNOTFILE;

  // Populate the info
  info.name = Win32W_ToString((UShort *)findData.cFileName);
  info.size = (ULong)findData.nFileSizeHigh;
  info.size <<= 32;
  info.size += (UInt)findData.nFileSizeLow;
  // Note: These times MAY BE WRONG according to Microsoft...
  //       So, we may need to change it to use GetFileInformationByHandle
  //       (see notes at http://msdn.microsoft.com/en-us/library/windows/desktop/aa364418%28v=vs.85%29.aspx)
  info.creationTime = Win32FileTimeToTime(findData.ftCreationTime);
  info.accessTime   = Win32FileTimeToTime(findData.ftLastAccessTime);
  info.updateTime   = Win32FileTimeToTime(findData.ftLastWriteTime);

  // Handle errors
  status = Error::None;
  while (false) {
		NOTFULL:   status = Error::NotFullPath;        break;
    NOTEXIST:  status = Error::FileDoesNotExist;   break;
    ISNOTFILE: status = Error::FilePathIsNotFile;  break;
    SOMEERROR:                                     break;
  }

  return status;

}

Status Win32OperatingSystem::CreateDirectories(String &dirName) {
  
  Status  status;
  String  theDir;
  Strings parts;
  UInt    curIdx;
  UShort  winDir[MAX_PATH+1];
  UInt    winDirLen;
  Boolean ok;
  
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

  parts = theDir.Split('\\');
  theDir = parts.Get(0);
  for (curIdx=1; curIdx < parts.Length(); curIdx++) {
    // Build subdirectory name up to here...
    theDir += '\\';
    theDir += parts.Get(curIdx);
    // If it already exists, make sure it's a directory
    if (Exists(theDir)) {
      if (!IsDirectory(theDir)) goto NOTDIR;
      continue;
    }
    // Doesn't exist, create it...
    status = Win32W_FromString(theDir, winDir, MAX_PATH, winDirLen);
    if (status) goto SOMEERROR;
    winDir[winDirLen] = 0;
    ok = CreateDirectoryW((LPCWSTR)winDir,NULL);
    if (!ok) goto OSERR;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
		NOTFULL:   status = Error::NotFullPath;      break;
    NOTDIR:    status = Error::FileNameIsNotDir; break;
    OSERR:     status = Error::OSError;          break;
    OK:        status = Error::None;             break;
    SOMEERROR:                                   break;
  }

  return status;
  
}

Status Win32OperatingSystem::GetDirectories(String const &fullName, Strings &subdirs) {

  Status   status;
  String   subdir;
  UShort   buf[MAX_PATH];
  UInt     bufl;
  HANDLE   hsearch;
  UInt     namelen;
  UInt     genCount;
  UInt     natCount;
  UShort  *tBuf;
  String   tDirName;
  
  WIN32_FIND_DATAW info;

  // Doesn't exist if it doesn't have a name
  if (fullName.Length() == 0) goto OK;

  // Make sure it's a full path...
	if (!IsFullPath(fullName)) goto NOTFULL;
	
  // Get the full name bytes
  status = Win32W_FromString(fullName, buf, sizeof(buf)-3, bufl);
  if (status) goto SOMEERROR;
  if (bufl) bufl--;

  // Convert to Native format if needed
  natCount = fullName.Count('\\');
  genCount = fullName.Count('/');
  if (genCount && !natCount)
    for (tBuf=buf; *tBuf; tBuf++) if (*tBuf == '/') *tBuf = '\\';

  // Make sure it ends in '\' and append a '*'
  if (buf[bufl-1] != '\\') {
    buf[bufl++] = '\\';
  }
  buf[bufl++] = '*';
  buf[bufl] = 0x00;
    
  // Try getting first file name
  hsearch = FindFirstFileW((LPCWSTR)buf,&info);
  if (hsearch == INVALID_HANDLE_VALUE) goto OK;

  // For each one...
  do {
    if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      // Ignore if hidden and we're not supposed to show it...
      if (!gShowHiddenDirectories && (info.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) )
        continue;
      // Normal processing
      subdir  = fullName;
      namelen = Win32W_Strlen((UShort *)info.cFileName);
      if (natCount == 0 && genCount && subdir.Get(-1) != '/')
        subdir += '/';
      if (natCount && subdir.Get(-1) != '\\')
        subdir += '\\';
      // Skip '.' and '..'
      if (namelen <= 2 && info.cFileName[0] == '.' &&
          (namelen == 1 || info.cFileName[1] == '.'))
        continue;
      // Append it to the list
      tDirName = Win32W_ToString((UShort *)info.cFileName);
      subdir.Append(tDirName);
      subdirs.Append(subdir.Consumable());
    }
  } while (FindNextFileW(hsearch,&info));

  // Close the search handle
  FindClose(hsearch);

  // Handle errors...
  status = Error::None;
  while (false) {
		NOTFULL:   status = Error::NotFullPath; break;
    OK:        status = Error::None;        break;
    SOMEERROR:                              break;
  }

  return status;

}

Status Win32OperatingSystem::GetFiles(String const &fullName, Strings &files) {

  Status   status;
  String   file;
  UShort   buf[MAX_PATH];
  UInt     bufl;
  HANDLE   hsearch;
  UInt     namelen;
  UInt     genCount;
  UInt     natCount;
  UShort  *tBuf;
  String   tFileName;
  
  WIN32_FIND_DATAW info;

  // Doesn't exist if it doesn't have a name
  if (fullName.Length() == 0) goto OK;

  // Make sure it's a full path...
	if (!IsFullPath(fullName)) goto NOTFULL;
	
  // Get the full name in Bytes...
  status = Win32W_FromString(fullName, buf, sizeof(buf)-3, bufl);
  if (status) goto SOMEERROR;
  if (bufl) bufl--;
  
  // Convert to Native format if needed
  natCount = fullName.Count('\\');
  genCount = fullName.Count('/');
  if (genCount && !natCount)
    for (tBuf=buf; *tBuf; tBuf++) if (*tBuf == '/') *tBuf = '\\';

  // Make sure it ends in '\' and append a '*'
  if (buf[bufl-1] != '\\') {
    buf[bufl++] = '\\';
  }
  buf[bufl++] = '*';
  buf[bufl] = 0x00;
    
  // Try getting first file name
  hsearch = FindFirstFileW((LPCWSTR)buf, &info);
  if (hsearch == INVALID_HANDLE_VALUE) goto OK;

  // For each one...
  do {

    if ((info.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY |
                                  FILE_ATTRIBUTE_ENCRYPTED |
                                  FILE_ATTRIBUTE_REPARSE_POINT |
                                  FILE_ATTRIBUTE_OFFLINE) ) == 0
       ) {

      // Ignore if hidden and we're not supposed to show it...
      if (!gShowHiddenFiles && (info.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) )
        continue;
      // Normal processing
      file  = fullName;
      namelen = Win32W_Strlen((UShort *)info.cFileName);
      if (natCount == 0 && genCount && fullName.Get(-1) != '/')
        file += '/';
      if (natCount && fullName.Get(-1) != '\\')
        file += '\\';

      // Skip '.' and '..'
      if (namelen <= 2 && info.cFileName[0] == '.' &&
          (namelen == 1 || info.cFileName[1] == '.'))
        continue;

      // Append it to the list
      tFileName = Win32W_ToString((UShort *)info.cFileName);
      file.Append(tFileName);
      files.Append(file.Consumable());

    }
  } while (FindNextFileW(hsearch,&info));

  // Close the search handle
  FindClose(hsearch);

  // Handle errors
  status = Error::None;
  while (false) {
		NOTFULL:   status = Error::NotFullPath; break;
    OK:        status = Error::None;        break;
    SOMEERROR:                              break;
  }

  return status;

}

Status Win32OperatingSystem::GetFilesInfo(String const &fullPath, FileInfos &files) {

  Status    status;
  String    natDirName;
  String    file;
  UShort    buf[MAX_PATH];
  UInt      bufl;
  HANDLE    hsearch;
  UInt      namelen;
  FileInfo  fInfo;
  UInt      genCount;
  UInt      natCount;
  UShort   *tBuf;
  String    tString;
  
  WIN32_FIND_DATAW findInfo;

  if (fullPath.Length() == 0) goto OK;
  
  // Make sure it's a full path...
	if (!IsFullPath(fullPath)) goto NOTFULL;
	
  // Transform the name to native and into our local buf
  status = Win32W_FromString(fullPath, buf, MAX_PATH, bufl);
  if (status) goto SOMEERROR;
  bufl--;
    
  // Convert to Native format if needed
  natCount = fullPath.Count('\\');
  genCount = fullPath.Count('/');
  if (genCount && !natCount)
    for (tBuf=buf; *tBuf; tBuf++) if (*tBuf == '/') *tBuf = '\\';

  // Get the full name bytes
  if (bufl+1 >= MAX_PATH-3) goto TOOLONG;
  if (buf[bufl-1] != '\\') {
    buf[bufl] = '\\';
    bufl++;
  }
  buf[bufl] = '*';
  bufl++;
  buf[bufl] = 0x00;

  // Try getting first file name
  hsearch = FindFirstFileW((LPCWSTR)buf,&findInfo);
  if (hsearch == INVALID_HANDLE_VALUE) goto OK;

  // For each one...
  do {

    if ((findInfo.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY |
                                      FILE_ATTRIBUTE_ENCRYPTED |
                                      FILE_ATTRIBUTE_REPARSE_POINT |
                                      FILE_ATTRIBUTE_OFFLINE) ) == 0
       ) { // It's a file...

      // Ignore if hidden and we're not supposed to show it...
      if (!gShowHiddenFiles && (findInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) )
        continue;
      // Normal processing
      file = fullPath;
      namelen = Win32W_Strlen((UShort *)findInfo.cFileName);
      if (natCount == 0 && genCount && fullPath.Get(-1) != '/')
        file += '/';
      if (natCount && fullPath.Get(-1) != '\\')
        file += '\\';
      tString = Win32W_ToString((UShort *)findInfo.cFileName).Consumable();
      file.Append(tString);

      // Skip '.' and '..'
      if (namelen <= 2 && findInfo.cFileName[0] == '.' &&
          (namelen == 1 || findInfo.cFileName[1] == '.'))
        continue;

      // Populate the info
      fInfo.name = file.Consumable();
      fInfo.size = (ULong)findInfo.nFileSizeHigh;
      fInfo.size <<= 32;
      fInfo.size += (UInt)findInfo.nFileSizeLow;
      // Note: These times MAY BE WRONG according to Microsoft...
      //       So, we may need to change it to use GetFileInformationByHandle
      //       (see notes at http://msdn.microsoft.com/en-us/library/windows/desktop/aa364418%28v=vs.85%29.aspx)
      fInfo.creationTime = Win32FileTimeToTime(findInfo.ftCreationTime);
      fInfo.accessTime   = Win32FileTimeToTime(findInfo.ftLastAccessTime);
      fInfo.updateTime   = Win32FileTimeToTime(findInfo.ftLastWriteTime);

      // Append it
      status = files.Append(fInfo.Consumable());
      if (status) break;

    } // ...It's a file

  } while (!status && FindNextFileW(hsearch,&findInfo));

  // Close the search handle
  FindClose(hsearch);

  // Handle errors...
  while (false) {
		NOTFULL:   status = Error::NotFullPath;     break;
    OK:        status = Error::None;            break;
    TOOLONG:   status = Error::FileNameTooLong; break;
    SOMEERROR:                                  break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Path/File primitives
//

Status Win32OperatingSystem::PathToGeneric(String &path) const {

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

Status Win32OperatingSystem::PathToNative(String &path) const {

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

Status Win32OperatingSystem::PathSplit(String const &path, Strings &parts) const {

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

Status Win32OperatingSystem::PathJoin(Strings const &parts, String &full) const {

  Status status;

  full.SetEmpty();
  full = parts.Join(pathseparator);

  status = Error::None;

  // Sanity check...
  if (parts.Length() != 0 && full.Length() == 0)
    status = Error::Internal;

  return status;

}

String Win32OperatingSystem::FileOnly(String const &path) const {
  
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

String Win32OperatingSystem::PathOnly(String const &path) const {
  
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

Boolean Win32OperatingSystem::IsPath(String const &path) const {
  return path.Count('\\') != 0 || path.Count('/') != 0;
}

Boolean Win32OperatingSystem::IsFullPath(String const &path) const {

  UInt    pathLen;
  Char    chars[3];
  Boolean answer;

  pathLen = path.Length();

  // Must be at least three long
  if (pathLen < 3) goto NOPE;

  // Get the first three characters
  chars[0] = path.Get(0);
  chars[1] = path.Get(1);
  chars[2] = path.Get(2);

  // Special case for Network (SMB) file names
  if ((chars[0] == '\\' && chars[1] == '\\') ||
      (chars[0] == '/'  && chars[1] == '/' ) ) goto YEP;

  // Make sure it's <Drive Letter>:
  if (  chars[1] == ':' &&
       (chars[2] == '\\' || chars[2] == '/') &&
       ((chars[0] >= 'a' && chars[1] <= 'z') || (chars[0] >= 'A' && chars[1] <= 'Z') )
     ) goto YEP;

  // Handle errors
  answer = false;
  while (false) {
    NOPE: answer = false; break;
    YEP:  answer = true;  break;
  }

  return answer;

}

Boolean Win32OperatingSystem::IsRelativePath(String const &path) const {

  Status  status;
  Strings parts;
  UInt    numParts;
  UInt    partNo;
  String  part;
  Boolean answer;
  Char    firstChar;

  // Special case for empty, local, and remote paths
  if (path.Length() == 0) goto NOPE;
  if (IsRemotePath(path)) goto NOPE;
  if (IsLocalPath(path)) goto NOPE;

  // Special case if starts with '~'...
  firstChar = path.Get(0);
  if (firstChar == '~' || firstChar == '!' || firstChar == '@') goto YEP;
  
  // Special case for  '\'...
  if (firstChar == '\\' || firstChar == '/') goto YEP;

  // Special case for no '\' or '/'...
  if (path.Count('\\') == 0 && path.Count('/') == 0) goto YEP;
  
  // Pull apart by the Path Separator
  status = PathSplit(path,parts);
  if (status) goto SOMEERROR;
  numParts = parts.Length();

  // For each Part...
  for (partNo=0; partNo < numParts; partNo++) {

    part = parts.Get(partNo);
    if (part.Length() == 0) continue;
    if (part == ".." || part == ".") break;
    if (!IsValidFileName(part)) goto NOPE;

  }

  answer = (partNo < numParts);
  
  // Handle errors...
  status = Error::None;
  while (false) {
    SOMEERROR:
    NOPE:      answer = false; break;
    YEP:       answer = true;  break;
  }

  return answer;

}

Boolean Win32OperatingSystem::IsLocalPath(String const &fullPath) const {
  
  Int foundPosColon;
  Int foundPos;
  
  if (!IsFullPath(fullPath))
    return false;
  foundPosColon = fullPath.Find(':');
  if (IsNull(foundPosColon)) 
    return false;
  foundPos = fullPath.Find('/');
  if (foundPos >= 0)
    return foundPosColon < foundPos;
  foundPos = fullPath.Find('\\');
  if (foundPos >= 0)
    return foundPosColon < foundPos;
  
  return false;
  
}

Boolean Win32OperatingSystem::IsRemotePath(String const &fullPath) const {
  String firstTwo;
  firstTwo = fullPath.Get(Span(0,2));
  return firstTwo == "//" || firstTwo == "\\\\";
}

Boolean Win32OperatingSystem::IsValidFileName(String const &fileName) const {

  Boolean  answer;
  Byte    *buf;
  UInt     size;
  Byte    *curChar;
  Byte    *firstDot;
  Byte    theChar;
  UInt    reml;

  char    *cp;

  fileName._BufAndSize(buf, size);

  // Invalid if empty or too long...
  if (size <= 0 || size >= MAX_PATH) goto NOPE;

  // Validate the characters used in the name...
  //   (and find the first '.', or end of file)
  firstDot = NULL;
  curChar  = buf;
  reml = size;
  while (reml > 0) {
    // Get the character
    theChar = *curChar++;
    reml--;
    // If it's less than 0x20, it's bad
    if (theChar < 0x20) goto NOPE;
    // If it's in the list of bad chars, it's bad
    for (cp=(char *)Win32ReservedFileChars;*cp;cp++)
      if (*cp == theChar) goto NOPE;
    // If it's a '.' and we don't have the first one yet, save where it is
    if (theChar == '.' && IsNull(firstDot))
      firstDot = curChar-1;
  }
  if (IsNull(firstDot))
    firstDot = buf + size - 1;

  // Validate last character
  theChar = buf[size-1];
  if (theChar == ' ' || theChar == '.') goto NOPE;

  // If everything before the first '.' or end of string is a reserved
  // win32 device name, it's not valid
  if ( !IsNull(Win32GetDeviceByName(buf, firstDot-buf)) ) goto NOPE;

  // *** It seems to qualify as a "valid file name" according to Microsoft
  answer = true;
  
  // Handle errors
  while (false) {
    NOPE: answer = false; break;
  }

  return answer;

}

Boolean Win32OperatingSystem::IsValidPathName(String const &fullpath) const {

  Boolean answer;
  Status  status;
  Strings parts;
  Int     numparts;
  Int     devno;
  Char    achar;
  Int     partno;
  String  part;
  UInt    valStart;         // Part validation starting index

  // If empty, it's not valid
  if (fullpath.Length() <= 0) goto NOPE;

  // Split it into consitituent parts
  status = PathSplit(fullpath,parts);
  if (status) goto NOPE;
  numparts = parts.Length();
  if (numparts <= 0) goto NOPE;

  // If it's a network path...
  if (numparts > 2 && parts.Get(0).Length() == 0 && parts.Get(1).Length() == 0) {

    valStart  = 2;

  }

  // It's not a network path...
  else {  // ...It's a "local" path

    valStart  = 1;

    // Validate the first part.  This must be either a drive letter followed by
    // a ':', or a reserved drive name.  In the first case, there has to be
    // more parts.  In the second case there can't be any more parts

    status = parts.Get(0,part);
    if (status) goto NOPE;

    // If two letters, it HAS TO be a drive number followed by a ':'...
    if (part.Length() == 2) {
      achar = part.Get(0);
      // Turn into uppercase
      if (achar >= 'a' && achar <= 'z')
        achar = achar - 'a' + 'A';
      // Must be a drive letter
      if (achar < 'A' || achar > 'Z') goto NOPE;
      // Must be followed by ':'
      achar = part.Get(1);
      if (achar != ':') goto NOPE;
      // It's a drive letter, must be followed by more parts
      if (numparts == 1) goto NOPE;
    }

    // Not two letters, make sure it's a reserved device name
    else {
      // Not two letters, see if it's a reserved device name
      devno = Win32GetDeviceByName(part);
      if ( IsNull(devno) ) goto NOPE;
      // If there's other parts, it's not valid
      if (numparts > 1) goto NOPE;
      goto YEP;
    }

  } // ...It's a "local" path

  // First part's okay and it's a drive letter and we have more parts...
  // Make sure the remaining parts are valid file/directory names...
  //
  // Note: Ignore empty last part
  //
  for (partno=valStart; partno < numparts; partno++) {
    status = parts.Get(partno,part);
    if (status) goto NOPE;
    if (partno == numparts-1 && part.Length() == 0) break;
    if (!IsValidFileName(part)) goto NOPE;
  }

  // Handle errors...
  answer = true;
  while (false) {
    NOPE: answer = false; break;
    YEP:  answer = true;  break;
  }

  return answer;
  
}

String Win32OperatingSystem::ApplyRelativePath(String const &path, String const &relpath) const {

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

Status Win32OperatingSystem::ApplyRelativePath(String const &pPath, String const &pRelPath, String &replacement) const {

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

Status Win32OperatingSystem::_LowMoveFile(String const &fromNativeFileName, String const &toNativeFileName) {
  
  Status  status;
  UShort  fromBuf[MAX_PATH+1];
  UInt    fromLen;
  UShort  toBuf[MAX_PATH+1];
  UInt    toLen;
  Boolean ok;
  
  status = Win32W_FromString(fromNativeFileName, fromBuf, MAX_PATH, fromLen);
  if (status) goto SOMEERROR;
  fromBuf[fromLen] = 0;
  status = Win32W_FromString(toNativeFileName, toBuf, MAX_PATH, toLen);
  if (status) goto SOMEERROR;
  toBuf[toLen] = 0;
  ok = MoveFileW((LPCWSTR)fromBuf, (LPCWSTR)toBuf);
  if (!ok) goto OSERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    OSERROR:   status = Error::OSError;           break;
    SOMEERROR:                                    break;
  }
  
  return status;
  
}

Status Win32OperatingSystem::_LowCopyFile(String const &fromNativeFileName, String const &toNativeFileName) {
  
  Status  status;
  UShort  fromBuf[MAX_PATH+1];
  UInt    fromLen;
  UShort  toBuf[MAX_PATH+1];
  UInt    toLen;
  Boolean ok;
  
  status = Win32W_FromString(fromNativeFileName, fromBuf, MAX_PATH, fromLen);
  if (status) goto SOMEERROR;
  fromBuf[fromLen] = 0;
  status = Win32W_FromString(toNativeFileName, toBuf, MAX_PATH, toLen);
  if (status) goto SOMEERROR;
  toBuf[toLen] = 0;
  ok = CopyFileW((LPCWSTR)fromBuf, (LPCWSTR)toBuf, TRUE);
  if (!ok) goto OSERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    OSERROR:   status = Error::OSError;           break;
    SOMEERROR:                                    break;
  }
  
  return status;
  
}

Status Win32OperatingSystem::_LowDeleteFile(String const &nativeFileName) {
  
  Status  status;
  UShort  nameBuf[MAX_PATH+1];
  UInt    nameLen;
  Boolean ok;
  
  status = Win32W_FromString(nativeFileName, nameBuf, MAX_PATH, nameLen);
  if (status) goto SOMEERROR;
  nameBuf[nameLen] = 0;
  ok = DeleteFileW((LPCWSTR)nameBuf);
  if (!ok) goto OSERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    OSERROR:   status = Error::OSError;           break;
    SOMEERROR:                                    break;
  }
  
  return status;
  
}

/*
Status Win32OperatingSystem::_LowMoveDirectory(String const &fromDirName, String const &toDirName) {
  return Error::PlatformNotCapable;
}

Status Win32OperatingSystem::_LowCopyDirectory(String const &fromDirName, String const &toDirName) {
  return Error::PlatformNotCapable;
}

Status Win32OperatingSystem::_LowDeleteDirectory(String const &dirName) {
  return Error::PlatformNotCapable;
}
*/

//----------------------------------------------------------------------
//
// Factories
//

File *Win32OperatingSystem::NewFile() {
  Win32File *that;
  that = new Win32File();
  return that;
}

void Win32OperatingSystem::FinishedWith(File *file) {
	if (file)
    delete file;
}

//----------------------------------------------------------------------
//
// Temporary Files
//

String Win32OperatingSystem::GetTempPath() {

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
    if (temppath.Length() == 0)
      Win32RegGetValueString("HKEY_CURRENT_USER/Environment:BEFE_Temp",temppath);
  }
  if (temppath.Length() == 0) {
    tempvar = "BEFE_Root";
    temppath = TheBefe->TheProcess->GetEnvironmentValue(String("BEFE_Root"));
    if (temppath.Length() == 0)
      Win32RegGetValueString("HKEY_CURRENT_USER/Environment:BEFE_Root",temppath);
  }
  if (temppath.Length() == 0) {
    tempvar = "TEMP";
    temppath = TheBefe->TheProcess->GetEnvironmentValue(String("TEMP"));
    if (temppath.Length() == 0)
      Win32RegGetValueString("HKEY_CURRENT_USER/Environment:TEMP",temppath);
  }
  if (temppath.Length() == 0) {
    tempvar = "TMP";
    temppath = TheBefe->TheProcess->GetEnvironmentValue(String("TMP"));
    if (temppath.Length() == 0)
      Win32RegGetValueString("HKEY_CURRENT_USER/Environment:TMP",temppath);
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

String Win32OperatingSystem::GetTempFileName() {

  String    temppath;
  String    tempfile;
  Win32File file;

  temppath = GetTempPath();
  tempfile = Win32FileGetTempFileName();
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

Status Win32OperatingSystem::NewRandomUUID(UUID &newUUID) {
  
  Status  status;
  HRESULT oleError;
  
  oleError = BEFE::CoCreateGuid((GUID *)&newUUID);
  if (oleError) {
    newUUID = UUID();
    status = Error::Win32OLEError;
  }
  else {
    // Turn the parts into big-endian order...
    newUUID.value.microsoft.data1 = ReverseBytes(newUUID.value.microsoft.data1);
    newUUID.value.microsoft.data2 = ReverseBytes(newUUID.value.microsoft.data2);
    newUUID.value.microsoft.data3 = ReverseBytes(newUUID.value.microsoft.data3);
    status = Error::None;
  }  
  return status;
  
}

//----------------------------------------------------------------------
//
// Internal Methods
//

Status Win32OperatingSystem::InitVars() {

  if (description.IsNull()) {  
    Win32RegGetValueString("HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/Windows NT/CurrentVersion:ProductName",description);
    Win32RegGetValueString("HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/Windows NT/CurrentVersion:CurrentVersion",version);
    Win32RegGetValueString("HKEY_CURRENT_USER/Volatile Environment:USERDOMAIN", hostname);
  }
  
  return Error::None;
  
}

} // ...Namespace BEFE
