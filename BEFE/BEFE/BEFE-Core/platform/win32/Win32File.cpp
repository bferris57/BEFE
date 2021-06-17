//----------------------------------------------------------------------
// File: Win32File.cpp - Implementation of the Win32File class
//----------------------------------------------------------------------
//
// This is the Win32 port specific implementation of the File class.
//
// These are automatically created by Win32OperatingSystem when asked
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
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Win32.h"
#include "Win32WCHAR.h"

namespace BEFE { // Namespace BEFE...

// Bloody MinGW defines THIS...
#ifdef THIS
#  undef THIS
#endif

// To get around silly 'const' crap...
#define THIS (*(Win32File *)this)
#define THAT (*(Win32File *)&that)

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Win32File::Win32File ()                   {deferee = NULL; StartUp();}
Win32File::Win32File (char const *name)   {deferee = NULL; StartUp(); SetName(name); }
Win32File::Win32File (String const &name) {deferee = NULL; StartUp(); SetName(name); }
Win32File::~Win32File()                   {ShutDown();}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Win32File::StartUp() {
  name.SetEmpty();
  access    = ModeNone;
  hFile     = 0;
  lastError = 0;
  return Error::None;
}

Status Win32File::ShutDown() {
  Status status;
  status = Close();
  name.SetEmpty();
  return status;
}

Status Win32File::Reset() {

  Status retStatus;
  Status status;

  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Status Win32File::Clear() {
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

Status Win32File::SetAccess(UInt mode) {

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

Status Win32File::GetAccess(UInt &mode) const{
  mode = access;
  return Error::None;
}

//----------------------------------------------------------------------
//
// Naming
//

String Win32File::GetName() const {
  return name;
}

Status Win32File::SetName() {
  Close();
  name.SetEmpty();
  return Error::None;
}

Status Win32File::SetName(const char *name) {

  Status status;
  String strName;
	
	strName = name;
	status = SetName(strName);
	
	return status;

}

Status Win32File::SetName(String const &name) {

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

Boolean Win32File::IsNameValid() const {
  BEFE_WARN("!!! Win32File::IsNameValid not implemented yet !!!");
  return false;
}

UInt Win32File::Length() const {
  return Size();
}

UInt Win32File::Size() const {

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

UInt Win32File::GetMaxLength() const {
  return 0x80000000;
}

UInt Win32File::GetMaxSize() const {
  return 0x80000000;
}

//----------------------------------------------------------------------
//
// Existence
//

Boolean Win32File::Exists() const {

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

Status Win32File::Create() {

  Status status;
  UShort fName[MAX_PATH];
  UInt   fNameLen;
  Int    saveAccess;

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
  // Open with write failed, maybe it's probably because either
  // the file or the directories dont exist...
  //

  // Get the file name
  status = Win32W_FromString(name, fName, MAX_PATH, fNameLen);
  if (status == Error::ValueTruncated) goto TOOLONG;
  if (status) goto SOMEERROR;
  if (fNameLen <=1) goto TOOSHORT;

  // Try creating it...
  hFile = CreateFileW((LPCWSTR)fName, GENERIC_ALL, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    // Opened, close the handle
    CloseHandle(hFile);
    hFile = 0;
    size = UNaN;
    pos  = UNaN;
    goto OK;
  }

  // TEMP: For now, since we haven't written the "create directories" code yet,
  //       we'll just fail
  status = Error::Failed;
  
  // Handle errors
  while (false) {
    TOOSHORT:  status = Error::FileNoName;      break;
    TOOLONG:   status = Error::FileNameTooLong; break;
    OK:        status = Error::None;            break;
    SOMEERROR:                                  break;
  }

  return status;

}

Status Win32File::Delete() {

  Status  status;
  UShort  fName[MAX_PATH];
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
  status = Win32W_FromString(name, fName, MAX_PATH, fNameLen);
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

Status Win32File::Rename(String const &pNewName) {

  Status  status;
  String  oldName;
  UShort  sOldName[MAX_PATH];
  UInt    oldNameLen;
  UShort  sNewName[MAX_PATH];
  String  newName;
  UInt    newNameLen;
  Int     result;

  // Validate old name...
  if (IsOpen()) goto ISOPEN;
  if (!IsFullPath(name)) goto NOTFULLPATH;
  if (!BEFE::Exists(name)) goto NOTEXIST;
  if (!BEFE::IsFile(name)) goto NOTFILE;
  if (name.Size()+1 > MAX_PATH) goto NAMETOOLONG;
  oldName = PathOnly(name);
  status = PathToNative(oldName);
  if (status) goto SOMEERROR;
  oldName += '\\';
  oldName += FileOnly(name);

  // Build the Byte form of pNewName using the two valid new name cases...
  if (IsFullPath(pNewName)) { // Case 1) New Name is Full Path...
    if (pNewName.Size()+1 > MAX_PATH) goto NAMETOOLONG;
    newName = pNewName;
    status = PathToNative(newName);
    if (status) goto SOMEERROR;
    if (PathOnly(oldName) != PathOnly(newName)) goto WRONGPATH;
    if (!IsValidFileName(FileOnly(pNewName))) goto BADNAME;
  }
  else { // Case 2: New Name is File name only...
    if (!IsValidFileName(pNewName)) goto BADNAME;
    newName = PathOnly(oldName) + '\\' + pNewName;
    if (newName.Size()+1 > MAX_PATH) goto NAMETOOLONG;
  }
  
  // Make sure the new one doesn't exist
  if (TheBefe->TheOS->Exists(newName)) goto EXISTS;

  // Get Wide forms of old and new names...
  status = Win32W_FromString(oldName, sOldName, MAX_PATH, oldNameLen);
  if (status == Error::ValueTruncated) goto NAMETOOLONG;
  if (status) goto SOMEERROR;
  status = Win32W_FromString(newName, sNewName, MAX_PATH, newNameLen);
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

Status Win32File::Move(String const &newPath) {

  Status  status;
  String  oldName;
  UShort  sOldName[MAX_PATH];
  UInt    oldNameLen;
  UShort  sNewName[MAX_PATH];
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
  if (name.Size()+1 > MAX_PATH) goto NAMETOOLONG;
  if (newPath.Size()+1 > MAX_PATH) goto NAMETOOLONG;
  oldName = name;
  status = PathToNative(oldName);
  if (status) goto SOMEERROR;
  newName = newPath;
  status = PathToNative(newName);
  if (status) goto SOMEERROR;

  // Get Wide forms of old and new names...
  status = Win32W_FromString(oldName, sOldName, MAX_PATH, oldNameLen);
  if (status == Error::ValueTruncated) goto NAMETOOLONG;
  if (status) goto SOMEERROR;
  status = Win32W_FromString(newName, sNewName, MAX_PATH, newNameLen);
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

Status Win32File::SetLength(UInt len) {
  return SetSize(len);
}

Status Win32File::SetSize(UInt newSize) {

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

Boolean Win32File::IsOpen() const {
  return (hFile > 0);
}

Status Win32File::Open() {
  return Open(access);
}

Status Win32File::Open(Int access) {

  Status              status;
  String              localName;
  USHORT              fName[MAX_PATH];
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
    status = Win32W_FromString(localName, fName, MAX_PATH, fNameLen);
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
    status = Win32W_FromString(name, fName, MAX_PATH, fNameLen);
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

Status Win32File::Close() {

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

Status Win32File::Move(StoreAddress src, Address dst, UInt numBytes, UInt &numMoved) {

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

Status Win32File::Move(Address src, StoreAddress dst, UInt numbytes, UInt &nummoved) {

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

Status Win32File::Append(Address src, UInt numbytes, UInt &nummoved) {

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

void Win32File::ModeToStyle(UInt mode, UInt &aMode, UInt &sMode) {

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
// Win32File utility functions
//

String Win32FileGetTempFileName() {

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

String Win32FileGetTempPath() {

  String  exename;
  Strings parts;
  String  temppath;

  if (TheBefe->TheProcess) { // Something to do...
    exename = TheBefe->TheProcess->GetExecutablePath();
    temppath = exename;
    temppath += '\\';
    temppath += "temp";
  } // ...Something to do

  return temppath;

}

} // ...Namespace BEFE
