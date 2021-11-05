//----------------------------------------------------------------------
// File: LinuxFile.cpp - Implementation of the LinuxFile class
//----------------------------------------------------------------------
//
// This is the Linux port specific implementation of the File class.
//
// These are automatically created by LinuxOperatingSystem when asked
// by the generic File class.
//
// NOTE: For now, we'll rely on the standard C/C++ library.  Shortly,
//       we'll be upgrading this to use proper Windows calls to do
//       the same things.  This way, we can, eventually, get rid
//       of our dependence on the C/C++ runtime libraries in an
//       effort to make the other ports much smaller and, in some
//       cases, even VIABLE!!!
//
// NOTE: This was originally cribbed from FileCstd.cpp which we've
//       subsequently deprecated in order to start managing and
//       implementing files properly.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Linux.h"

namespace BEFE { // Namespace BEFE...

// To get around silly 'const' crap...
#define THIS (*(LinuxFile *)this)
#define THAT (*(LinuxFile *)&that)

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LinuxFile::LinuxFile ()                   {deferee = NULL; StartUp();}
LinuxFile::LinuxFile (char const *name)   {deferee = NULL; StartUp(); SetName(name); }
LinuxFile::LinuxFile (String const &name) {deferee = NULL; StartUp(); SetName(name); }
LinuxFile::~LinuxFile()                   {ShutDown();}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status LinuxFile::StartUp() {
  name.SetEmpty();
  access    = ModeNone;
  fd        = 0;
  lastError = 0;
  return Error::None;
}

Status LinuxFile::ShutDown() {
  Status status;
  status = Close();
  name.SetEmpty();
  return status;
}

Status LinuxFile::Reset() {

  Status retStatus;
  Status status;

  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Status LinuxFile::Clear() {
  Status status;
  if (IsOpen())
    status = Close();
  else
    status = Error::None;
    
  return status;
}

//----------------------------------------------------------------------
//
// Access
//

Status LinuxFile::SetAccess(UInt mode) {

  Status  status;
  Boolean wasopen;

  if ((mode & ModeMask) != mode) goto BADMODE;
  wasopen = IsOpen();
  if (wasopen) {
    status = Close();
    if (status) goto SOMEERROR;
  }
  access = mode;
  if (wasopen) {
    status = Open();
    if (status) goto SOMEERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    BADMODE:   status = Error::FileInvalidAccessMode; break;
    SOMEERROR:                                        break;
  }

  return status;

}

Status LinuxFile::GetAccess(UInt &mode) const{
  mode = access;
  return Error::None;
}

//----------------------------------------------------------------------
//
// Naming
//

String LinuxFile::GetName() const {
  return name;
}

Status LinuxFile::SetName() {
  Close();
  name.SetEmpty();
  return Error::None;
}

Status LinuxFile::SetName(const char *name) {

  Status status;
  String strName;
	
	strName = name;
	status = SetName(strName);
	
	return status;

}

Status LinuxFile::SetName(String const &name) {

	Status status;
	
  if (!name.IsNull() && !IsFullPath(name)) goto NOTFULL;
  if (this->name != name) {
    Close();
    this->name = name;
    status = TheBefe->TheOS->PathToNative(this->name);
    if (status) goto SOMEERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
		NOTFULL:   status = Error::NotFullPath; break;
    SOMEERROR:                              break;
  }

  return Error::None;
}

//----------------------------------------------------------------------
//
// Query
//

Boolean LinuxFile::IsNameValid() const {
  BEFE_WARN("!!! LinuxFile::IsNameValid not implemented yet !!!");
  return false;
}

UInt LinuxFile::Length() const {
  return Size();
}

UInt LinuxFile::Size() const {

  Status  status;
  UInt    answer;
  UInt    saveaccess;
  Boolean wasopen;

  // If not already open, open it...
  wasopen = IsOpen();
  if (!wasopen) {
    saveaccess = THIS.access;
    THIS.access = ModeRead;
    status = THIS.Open(ModeRead);
    THIS.access = saveaccess;
    if (status) goto NOPE;
  }

  // Get the size
  answer = size;

  // Handle errors...
  while (false) {
    NOPE: answer = UNaN; break;
    //YEP:                 break;
  }

  // Close it if it wasn't already open
  if (IsOpen() && !wasopen)
    THIS.Close();

  return answer;

}

UInt LinuxFile::GetMaxLength() const {
  return 0x80000000;
}

UInt LinuxFile::GetMaxSize() const {
  return 0x80000000;
}

//----------------------------------------------------------------------
//
// Existence
//

Boolean LinuxFile::Exists() const {

  Boolean answer;
  Boolean status;
  Int     saveaccess;

  if (name.Length() == 0) goto NOPE;
  if (IsOpen()) goto YEP;

  saveaccess = access;
  THIS.access = ModeRead;
  status = THIS.Open();
  THIS.Close();
  THIS.access = saveaccess;

  if (status) goto NOPE;

  // Handle errors
  answer = true;
  while (false) {
    NOPE: answer = false; break;
    YEP:  answer = true;  break;
  }

  return answer;

}

Status LinuxFile::Create() {

  Status status;
  char   fName[PATH_MAX];
  UInt   fNameLen;
  int    access;
  Int    saveAccess;
  int    fd;
  int    rc;
  mode_t mode;

  // Error if open...
  if (fd) goto ISOPEN;

  // If no name, need to make one...
  if (name.Length() == 0) {
    name = TheBefe->TheOS->GetTempFileName();
    if (name.Length() <= 0) goto TOOSHORT;
    size = UNaN;
    pos  = UNaN;
  }

  // If already open, it's already created...
  if (IsOpen()) goto OK;

  // Not open, try opening it with write...
  saveAccess = access;
  access = ModeWrite;
  status = Open();
  access = saveAccess;
  if (!status) {
    status = Close();
    if (status) goto SOMEERROR;
    goto OK;
  }

  //
  // Open...
  //

  // Get the file name
  status = LinuxBytesFromString(name, fName, PATH_MAX);
  if (status == Error::ValueTruncated) goto TOOLONG;
  if (status) goto SOMEERROR;
  if (fNameLen <=1) goto TOOSHORT;

  // Try creating it...
  mode  = O_APPEND;
  mode |= O_EXCL;
  mode |= O_RDWR;
  fd = open(fName, access, mode);
  if (fd < 0) goto ERRNO;

  if (fd > 0) {
    // Opened, close the handle
    CloseHandle(hFile);
    fd   = 0;
    size = UNaN;
    pos  = UNaN;
    goto OK;
  }

  // TEMP: For now, since we haven't written the "create directories" code yet,
  //       we'll just fail
  status = Error::Failed;
  
  // Handle errors
  while (false) {
    TOOSHORT:  status = Error::FileNoName;           break;
    TOOLONG:   status = Error::FileNameTooLong;      break;
    ERRNO:     status = Error::OperatingSystemError;
                 lasterror = errno;
                 break;
    OK:        status = Error::None;                 break;
  }

  return status;

}

Status LinuxFile::Delete() {

  Status  status;
  Byte    fName[PATH_MAX];
  UInt    fNameLen;
  Int     saveAccess;
  Boolean win32ok;

  // If it's already open, close it (turn off "transient" if it's set)
  if (IsOpen()) {
    saveAccess = access;
    access &= ~ModeTransient;
    status = Close();
    access = saveAccess;
    if (status) goto SOMEERROR;
  }

  // if it wasn't open, see if it exists...
  else
    if (!Exists()) goto NOTEXIST;

  // Get the file name
  status = LinuxW_FromString(name, fName, PATH_MAX, fNameLen);
  if (status == Error::ValueTruncated) goto TOOLONG;
  if (status) goto SOMEERROR;
  if (fNameLen <= 1) goto TOOSHORT;

  // Try deleting it
  win32ok = DeleteFileW((LPCWSTR)fName);
  if (!win32ok) goto DELETEFAILED;

  // Handle errors...
  status = Error::None;
  while (false) {
    NOTEXIST:     status = Error::FileDoesNotExist; break;
    TOOSHORT:     status = Error::FileNoName;       break;
    TOOLONG:      status = Error::FileNameTooLong;  break;
    SOMEERROR:                                      break;
    DELETEFAILED: status = Error::FileDeleteFailed;
                  lastError = GetLastError();
                  break;
  }

  return status;

}

Status LinuxFile::Rename(String const &pNewName) {

  Status  status;
  String  oldName;
  Byte    sOldName[PATH_MAX];
  UInt    oldNameLen;
  Byte    sNewName[PATH_MAX];
  String  newName;
  UInt    newNameLen;
  Int     result;

  // Validate old name...
  if (IsOpen()) goto ISOPEN;
  if (!IsFullPath(name)) goto NOTFULLPATH;
  if (!BEFE::Exists(name)) goto NOTEXIST;
  if (!BEFE::IsFile(name)) goto NOTFILE;
  if (name.Size()+1 > PATH_MAX) goto NAMETOOLONG;
  oldName = PathOnly(name);
  status = PathToNative(oldName);
  if (status) goto SOMEERROR;
  oldName += '\\';
  oldName += FileOnly(name);

  // Build the Byte form of pNewName using the two valid new name cases...
  if (IsFullPath(pNewName)) { // Case 1) New Name is Full Path...
    if (pNewName.Size()+1 > PATH_MAX) goto NAMETOOLONG;
    newName = pNewName;
    status = PathToNative(newName);
    if (status) goto SOMEERROR;
    if (PathOnly(oldName) != PathOnly(newName)) goto WRONGPATH;
    if (!IsValidFileName(FileOnly(pNewName))) goto BADNAME;
  }
  else { // Case 2: New Name is File name only...
    if (!IsValidFileName(pNewName)) goto BADNAME;
    newName = PathOnly(oldName) + '\\' + pNewName;
    if (newName.Size()+1 > PATH_MAX) goto NAMETOOLONG;
  }
  
  // Make sure the new one doesn't exist
  if (TheBefe->TheOS->Exists(newName)) goto EXISTS;

  // Get Wide forms of old and new names...
  status = LinuxW_FromString(oldName, sOldName, PATH_MAX, oldNameLen);
  if (status == Error::ValueTruncated) goto NAMETOOLONG;
  if (status) goto SOMEERROR;
  status = LinuxW_FromString(newName, sNewName, PATH_MAX, newNameLen);
  if (status == Error::ValueTruncated) goto NAMETOOLONG;
  if (status) goto SOMEERROR;

  // Do it...
  result = MoveFileW((LPCWSTR)sOldName, (LPCWSTR)sNewName);
  if (!result) goto MOVEFAILED;

  // Handle errors
  status = Error::None;
  while (false) {
    NOTFULLPATH: status = Error::FileNotFullPath;     break;
    NOTFILE:     status = Error::FilePathIsNotFile;   break;
    BADNAME:     status = Error::FileInvalidFileName; break;
    NAMETOOLONG: status = Error::FileNameTooLong;     break;
    ISOPEN:      status = Error::FileIsOpen;          break;
    NOTEXIST:    status = Error::FileDoesNotExist;    break;
    EXISTS:      status = Error::FileAlreadyExists;   break;
    WRONGPATH:   status = Error::FilePathsDoNotMatch; break;
    SOMEERROR:                                        break;
    MOVEFAILED:  lastError = GetLastError();
                 status = Error::Failed;
                 break;
  }
    
  return status;
  
}

Status LinuxFile::Move(String const &newPath) {

  Status  status;
  String  oldName;
  Byte    sOldName[PATH_MAX];
  UInt    oldNameLen;
  Byte    sNewName[PATH_MAX];
  String  newName;
  UInt    newNameLen;
  Int     result;

  // Validate old name...
  if (IsOpen()) goto ISOPEN;
  if (!IsFullPath(name)) goto NOTFULLPATH;
  if (!IsFullPath(newPath)) goto NOTFULLPATH;
  if (!BEFE::Exists(name)) goto NOTEXIST;
  if (!BEFE::IsFile(name)) goto NOTFILE;
  if (BEFE::Exists(newPath)) goto EXISTS;
  if (name.Size()+1 > PATH_MAX) goto NAMETOOLONG;
  if (newPath.Size()+1 > PATH_MAX) goto NAMETOOLONG;
  oldName = name;
  status = PathToNative(oldName);
  if (status) goto SOMEERROR;
  newName = newPath;
  status = PathToNative(newName);
  if (status) goto SOMEERROR;

  // Get Wide forms of old and new names...
  status = LinuxW_FromString(oldName, sOldName, PATH_MAX, oldNameLen);
  if (status == Error::ValueTruncated) goto NAMETOOLONG;
  if (status) goto SOMEERROR;
  status = LinuxW_FromString(newName, sNewName, PATH_MAX, newNameLen);
  if (status == Error::ValueTruncated) goto NAMETOOLONG;
  if (status) goto SOMEERROR;

  // Do it...
  result = MoveFileW((LPCWSTR)sOldName, (LPCWSTR)sNewName);
  if (!result) goto MOVEFAILED;

  // Handle errors
  status = Error::None;
  while (false) {
    NOTFULLPATH: status = Error::FileNotFullPath;     break;
    NOTFILE:     status = Error::FilePathIsNotFile;   break;
    NAMETOOLONG: status = Error::FileNameTooLong;     break;
    ISOPEN:      status = Error::FileIsOpen;          break;
    NOTEXIST:    status = Error::FileDoesNotExist;    break;
    EXISTS:      status = Error::FileAlreadyExists;   break;
    SOMEERROR:                                        break;
    MOVEFAILED:  lastError = GetLastError();
                 status = Error::Failed;
                 break;
  }

  return status;

}

Status LinuxFile::SetLength(UInt len) {
  return SetSize(len);
}

Status LinuxFile::SetSize(UInt newSize) {

  Status  status;
  UInt    newPos;
  Boolean ok;

  // Fail if invalid addresses
  if (newSize > GetMaxSize()) goto INVALIDPARAMETER;

  // Open for if not open
  if (!IsOpen()) {
    status = Open();
    if (status) goto SOMEERROR;
  }

  // Make sure it's writable
  if (!(access&ModeWrite)) goto NOTWRITABLE;
  
  // Nothing to do if same size
  if (newSize == size) goto OK;

  // If not at that position, position there
  if (pos != newSize) {
    newPos = SetFilePointer((HANDLE)hFile,newSize,NULL,FILE_BEGIN);
    if (newPos != newSize) goto POSFAILED;
    pos = newSize;
    size = newSize;
  }

  // Set the file size
  ok = SetEndOfFile((HANDLE)hFile);
  if (!ok) goto SETFAILED;

  // Handle errors
  status = Error::None;
  while (false) {
    OK:               status = Error::None;                break;
    INVALIDPARAMETER: status = Error::InvalidParameter;    break;
    NOTWRITABLE:      status = Error::FileNotOpenForWrite; break;
    SOMEERROR:                                             break;
    SETFAILED:
    POSFAILED:
      status = Error::FileSetSizeFailed;
      lastError = GetLastError();
      break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Opening/Closing
//

Boolean LinuxFile::IsOpen() const {
  return (hFile > 0);
}

Status LinuxFile::Open() {
  return Open(access);
}

Status LinuxFile::Open(Int access) {

  Status              status;
  String              localName;
  USHORT              fName[PATH_MAX];
  UInt                fNameLen;
  UInt                aMode;
  UInt                sMode;
  UInt                sizes[2];
  Boolean             isTemp;
  SECURITY_ATTRIBUTES sAttr;
  
  // Make sure not already open
  if (IsOpen()) goto ALREADYOPEN;

  // Get the name bytes...
  if (name.Length() && !TheBefe->TheOS->IsValidPathName(name)) goto BADPATH;
  localName = name;
  status = TheBefe->TheOS->PathToNative(localName);
  if (status) goto SOMEERROR;
  
  if (localName.Length()) {
    status = LinuxW_FromString(localName, fName, PATH_MAX, fNameLen);
    if (status == Error::ValueTruncated) goto NAMETOOLONG;
    if (status) goto SOMEERROR;
    if (fNameLen)
      fNameLen--;
  }
  else
    fNameLen = 0;
    
  // Initialise Security Attributes...
  sAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
  sAttr.bInheritHandle = TRUE; 
  sAttr.lpSecurityDescriptor = NULL;
   
  // General case if no file name and not only reading, make one...
  if (fNameLen == 0) {
    isTemp = true;
    name = TheBefe->TheOS->GetTempFileName();
    // Get the name bytes...
    status = LinuxW_FromString(name, fName, PATH_MAX, fNameLen);
    if (status == Error::ValueTruncated) goto NAMETOOLONG;
    if (status) goto SOMEERROR;
    if (fNameLen)
      fNameLen--;
    // Since we're opening a temp, give all rights but make it transient
    this->access |= ModeRead | ModeWrite | ModeAppend | ModeTransient;
    access = this->access;
  }
  else
    isTemp = false;

  // Clear size and position
  size = UNaN;
  pos  = UNaN;

  // Try opening it
  if (access == 0)
    access = ModeRead;
  ModeToStyle(access, aMode, sMode);
  lastError = 0;
  hFile = CreateFileW((LPCWSTR)fName, aMode, sMode ,&sAttr ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL);
  
  // If it didn't open, try handling it here...
  if (hFile == INVALID_HANDLE_VALUE) { // First Open failed...

    lastError = GetLastError();
    // Error if BEFE_Temp is set and we get "path not found"
    // and it's a temporary file
    if (lastError == ERROR_PATH_NOT_FOUND && isTemp)
      TheBefe->TheProcess->Abort("Temporary file path not found, please set BEFE_Temp and try again");
      
    // If not FILE_NOT_FOUND or ACCESS_DENIED error, bail out...
    if (lastError != ERROR_FILE_NOT_FOUND &&
        lastError != ERROR_ACCESS_DENIED)
      goto OPENFAILED;

    // If we're not trying to write or append to it, error...
    if ((access & (ModeWrite|ModeAppend)) == 0) goto OPENFAILED;

    // Otherwise, try creating it...
    hFile = CreateFileW((LPCWSTR)fName, aMode, sMode, &sAttr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

    // If still error, give up...
    if (hFile == INVALID_HANDLE_VALUE) goto OPENFAILED;

  } // ...First Open failed

  // It opened, get the size
  sizes[0] = UNaN;
  GetFileSizeEx(hFile,(PLARGE_INTEGER)sizes);
  size = sizes[0];

  // If too large, return UNaN
  if (sizes[1] || sizes[0] > GetMaxLength()) goto TOOLARGE;

  // Handle errors...
  status = Error::None;
  while (false) {
    BADPATH:     status = Error::FileInvalidFileName; break;
    ALREADYOPEN: status = Error::FileAlreadyOpen;     break;
    NAMETOOLONG: status = Error::FileNameTooLong;     break;
    TOOLARGE:    status = Error::FileContentsTooLong; break;
    SOMEERROR:                                        break;
    OPENFAILED:  lastError = GetLastError();
                 status = Error::FileOpenFailed;
                 break;
  }
  
  // If all okay, save the access
  if (!status)
    this->access = access;

  // Close if open and failed
  if (status && IsOpen())
    Close();

  return status;

}

Status LinuxFile::Close() {

  Status status;

  if (hFile) {
    CloseHandle((HANDLE)hFile);
    hFile = 0;
    lastError = 0;
    status = Error::None;
  }
  else
    status = Error::FileNotOpen;

  // Clear size/pos
  size = UNaN;
  pos  = UNaN;

  // If Transient, delete it...
  if (!status && (access&ModeTransient) && !gKeepTemporaryFiles)
    status = Delete();

  return status;

}

//------------------------------------------------------------------------------
//
// Data Movement
//

Status LinuxFile::Move(StoreAddress src, Address dst, UInt numBytes, UInt &numMoved) {

  Status status;
  Int    err;
  UInt   newPos;

  // Initialise output
  numMoved = 0;

  // Fail if invalid addresses
  if (src >= GetMaxSize() || IsNull(dst) || numBytes < 0) goto INVALIDPARAMETER;

  // Open for if not open
  if (!IsOpen()) {
    if ((access&ModeMask) == 0)
      access = ModeRead;
    else
      if (!(access&ModeRead)) goto NOTREADABLE;
    status = Open();
    if (status) goto SOMEERROR;
  }

  // Fail if address > file size
  if (src > size) goto BADADDRESS;

  // Nothing to do if address == file size
  if (src == size) goto OK;

  // If not at that position, position there
  if (pos != src) {
    newPos = SetFilePointer((HANDLE)hFile,src,NULL,FILE_BEGIN);
    if (newPos != src) goto POSFAILED;
    pos = newPos;
  }

  // Truncate bytes to read to bytes remaining
  numBytes = Min(numBytes,size-pos);

  // Read the bytes
  lastError = 0;
  err = ReadFile((HANDLE)hFile,(void *)dst,numBytes,(DWORD *)&numMoved,NULL);
  if (err == 0) goto READFAILED;

  // Adjust the position
  pos += numMoved;

  // Handle errors
  status = Error::None;
  while (false) {
    OK:               status = Error::None;               break;
    INVALIDPARAMETER: status = Error::InvalidParameter;   break;
    NOTREADABLE:      status = Error::FileNotOpenForRead; break;
    SOMEERROR:                                            break;
    BADADDRESS:       status = Error::FileBadAddress;     break;
    READFAILED:
    POSFAILED:
      status = Error::FileReadFailed;
      lastError = GetLastError();
      break;
  }

  return status;

}

Status LinuxFile::Move(Address src, StoreAddress dst, UInt numbytes, UInt &nummoved) {

  Status status;
  UInt   newpos;

  // Initialise output
  nummoved = 0;
  lastError = 0;

  // Fail if invalid addresses
  if (IsNull(src) || dst >= GetMaxSize() || numbytes < 0) goto INVALIDPARAMETER;

  // Open for if not open
  if (!IsOpen()) {
    if (!(access&ModeWrite)) goto NOTWRITABLE;
    status = Open();
    if (status) goto SOMEERROR;
  }

  // Fail if address > file size
  if (dst > size) goto BADADDRESS;

  // If not at that position, position there
  if (pos != dst) {
    newpos = SetFilePointer((HANDLE)hFile,dst,NULL,FILE_BEGIN);
    if (newpos != dst) goto POSFAILED;
    pos = newpos;
  }

  // Write the bytes
  status = WriteFile((HANDLE)hFile,(void *)src,numbytes,(DWORD *)&nummoved,NULL);
  if (status == 0) goto WRITEFAILED;

  // Adjust the position
  pos += nummoved;
  size = Max(size,pos);

  // If ModeWriteThrough, flush the buffers
  if (access&ModeWriteThrough) {
    status = FlushFileBuffers((HANDLE)hFile);
    if (status == 0) goto FLUSHFAILED;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    INVALIDPARAMETER: status = Error::InvalidParameter;    break;
    NOTWRITABLE:      status = Error::FileNotOpenForWrite; break;
    SOMEERROR:                                             break;
    BADADDRESS:       status = Error::FileBadAddress;      break;
    POSFAILED:
    WRITEFAILED:
    FLUSHFAILED:
      status = Error::FileWriteFailed;
      lastError = GetLastError();
      break;
  }
  
  return status;

}

Status LinuxFile::Append(Address src, UInt numbytes, UInt &nummoved) {

  Status status;

  if (!IsOpen()) {
    if (name.Length() != 0 && !(access&ModeAppend)) goto NOTAPPENDABLE;
    access |= ModeAppend | ModeWrite | ModeWriteThrough;
    status = Open(access);
    if (status) goto SOMEERROR;
  }

  status = Write(src,size, numbytes,nummoved);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR: if (status == Error::FileWriteFailed)
                 status = Error::FileAppendFailed;
                break;
    NOTAPPENDABLE: status = Error::FileNotOpenForAppend; break;
  }

  return status;

}

//------------------------------------------------------------------------------
//
// Protected Class Methods
//

void LinuxFile::ModeToStyle(UInt mode, UInt &aMode, UInt &sMode) {

  aMode = sMode = 0;
  if (mode & ModeRead) {
    aMode |= GENERIC_READ;
    sMode |= FILE_SHARE_READ; // So other processes can read the same file
  }
  if (mode & ModeWrite) {
    aMode |= GENERIC_WRITE;
    // Thought we might need this next one for inherited handles but we don't
    //sMode |= FILE_SHARE_WRITE;
  }
  
  return;

}

//------------------------------------------------------------------------------
//
// LinuxFile utility functions
//

String LinuxFileGetTempFileName() {

  String tempname;
  Time   now;
  Int    year;
  Int    month;
  Int    day;
  Int    hour;
  Int    minute;
  Int    second;
  Int    millisecond;

  now.Set();
  
  if ( !IsNull(now.Get()) ) { // Something to do...

    year        = now.GetYear();
    month       = now.GetMonthOfYear();
    day         = now.GetDayOfMonth();
    hour        = now.GetAs(Time::Hours);
    minute      = now.GetAs(Time::Minutes);
    second      = now.GetAs(Time::Seconds);
    millisecond = now.GetAs(Time::Milliseconds);

    // YYYY-MM-DD
    tempname += year;
    tempname += '-';
    if (month < 10)
      tempname += '0';
    tempname += month;
    tempname += '-';
    if (day < 10)
      tempname += '0';
    tempname += day;
    //tempname += '}';

    // HH.MM
    tempname += '^';
    if (hour < 10)
      tempname += '0';
    tempname += hour;
    tempname += '.';
    if (minute < 10)
      tempname += '0';
    tempname += minute;
    tempname += '.';
    if (second < 10)
      tempname += '0';
    tempname += second;

    // Milliseconds
    tempname += '_';
    if (millisecond < 100)
      tempname += '0';
    if (millisecond < 10)
      tempname += '0';
    tempname += millisecond;
    tempname += "ms";

  } // ...Something to do

  return tempname;
  
}

String LinuxFileGetTempPath() {

  String  exename;
  Strings parts;
  String  temppath;

  if (TheBefe->TheProcess) { // Something to do...
    exename = TheBefe->TheProcess->GetExecutablePath();
    temppath = exename;
    temppath += '/';
    temppath += "temp";
  } // ...Something to do

  return temppath;

}

} // ...Namespace BEFE
