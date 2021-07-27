//----------------------------------------------------------------------
// File: File.cpp - Base implementation of the File class
//----------------------------------------------------------------------
//
// This is the basic empty implementation of the File class
// interface.  It provides default behaviour for all the proper
// sub-classes of File.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "File.h"
#include "OperatingSystem.h"
#include "Lines.h"

namespace BEFE {

// To get around silly 'const' crap...
#define THIS (*(File *)this)
#define THAT (*(File *)&that)

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

File::File ()                   {deferee = NULL; StartUp();}
File::File (char const *name)   {deferee = NULL; StartUp(); SetName(name); }
File::File (String const &name) {deferee = NULL; StartUp(); SetName(name); }
File::~File()                   {ShutDown();}

File &File::operator=(char const *name)   {SetName(name); return *this;}
File &File::operator=(String const &name) {SetName(name); return *this;}
File &File::operator=(File const &that)   {CopyFrom(that);return *this;}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status File::StartUp() {

  deferee    = NULL;
  deferDepth = 0;

  return Error::None;

}

Status File::ShutDown() {

  Status  status;
  File   *deferto;

  status = Error::None;

  Reset();

  if (!deferee) goto OK;

  deferto = deferee;
  if (deferto && TheBefe->TheOS) {
    deferee = NULL;
    deferDepth++;
    TheBefe->TheOS->FinishedWith(deferto);
    deferDepth++;
  }

  // Handle errors
  while (false) {
	OK: break;
  }

  return status;

}

Status File::Reset() {

  Status  status;
  File   *deferto;

  if (!deferee) goto OK;

  deferto = GetDeferee();
  if (deferto && !deferDepth) {
    deferDepth++;
    deferto->Reset();
    deferDepth--;
  }

  // Handle errors
  status = Error::None;
  while (false) {
	OK: status = Error::None; break;
  }

  return status;

}

Status File::MoveFrom(File const &that) {

  Status status;
  Status retStatus;

  retStatus = Reset();

  deferee    = that.deferee;
  deferDepth = that.deferDepth;

  status = THAT.StartUp();
  if (status && !retStatus)
    retStatus = status;

  return retStatus;

}

Status File::CopyFrom(File const &that) {

  Status status;
  Status retStatus;

  retStatus = Reset();
  deferee = NULL;
  status = SetName(that.GetName());
  if (status && !retStatus) retStatus = status;
  status = SetAccess(that.GetAccess());
  if (status && !retStatus) retStatus = status;

  return retStatus;

}

//----------------------------------------------------------------------
//
// Access
//

Status File::SetAccess(UInt mode) {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->SetAccess(mode);
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

UInt File::GetAccess() const {

  UInt mode;
  GetAccess(mode);

  return mode;

}

Status File::GetAccess(UInt &mode) const {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    THIS.deferDepth++;
    answer = deferto->GetAccess(mode);
    THIS.deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Boolean File::CanRead() const {
  return (GetAccess() & ModeRead) != 0;
}

Boolean File::CanWrite() const {
  return (GetAccess() & ModeWrite) != 0;
}

//----------------------------------------------------------------------
//
// Naming
//

String File::GetName() const {

  String  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    THIS.deferDepth++;
    answer = deferto->GetName();
    THIS.deferDepth--;
  }

  return answer;

}

Status File::SetName() {


  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->SetName();
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Status File::SetName(const char *name) {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->SetName(name);
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Status File::SetName(String const &name) {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->SetName(name);
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

//----------------------------------------------------------------------
//
// Query
//

Boolean File::IsNameValid() const {

  Boolean answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    THIS.deferDepth++;
    answer = deferto->IsNameValid();
    THIS.deferDepth--;
  }
  else
    answer = false;

  return answer;
}

UInt File::Length() const {

  UInt  answer;
  File *deferto;

  deferto = GetDeferee();
  if (deferto) {
    THIS.deferDepth++;
    answer = deferto->Length();
    THIS.deferDepth--;
  }
  else
    answer = UNaN;

  return answer;

}

UInt File::Size() const {

  UInt  answer;
  File *deferto;

  deferto = GetDeferee();
  if (deferto) {
    THIS.deferDepth++;
    answer = deferto->Size();
    THIS.deferDepth--;
  }
  else
    answer = UNaN;

  return answer;

}

UInt File::GetMaxLength() const {

  UInt  answer;
  File *deferto;

  deferto = GetDeferee();
  if (deferto) {
    THIS.deferDepth++;
    answer = deferto->GetMaxLength();
    THIS.deferDepth--;
  }
  else
    answer = UNaN-1;

  return answer;

}

UInt File::GetMaxSize() const {

  UInt  answer;
  File *deferto;

  deferto = GetDeferee();
  if (deferto) {
    THIS.deferDepth++;
    answer = deferto->GetMaxSize();
    THIS.deferDepth--;
  }
  else
    answer = UNaN-1;

  return answer;

}

//----------------------------------------------------------------------
//
// Existence
//

Boolean File::Exists() const {

  Boolean answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    THIS.deferDepth++;
    answer = deferto->Exists();
    THIS.deferDepth--;
  }
  else
    answer = false;

  return answer;

}

Status File::Create() {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->Create();
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Status File::Delete() {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->Delete();
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Status File::Rename(String const &newName) {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->Rename(newName);
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Status File::Move(String const &newPath) {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->Move(newPath);
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Status File::SetLength(UInt len) {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->SetLength(len);
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Status File::SetSize(UInt size) {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->SetSize(size);
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

//----------------------------------------------------------------------
//
// Opening/Closing
//

Boolean File::IsOpen() const {

  Boolean  answer;
  File    *deferto;

  deferto = GetDeferee();
  if (deferto) {
    THIS.deferDepth++;
    answer = deferto->IsOpen();
    THIS.deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Status File::Open() {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->Open();
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Status File::Open(Int access) {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->Open(access);
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Status File::Close() {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->Close();
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

//------------------------------------------------------------------------------
//
// Low level Data Movement
//

Status File::Read (StoreAddress src, Address dst, UInt numbytes, UInt &nummoved) {
  return Move(src,dst,numbytes,nummoved);
}

Status File::Write(Address src, StoreAddress dst, UInt numbytes, UInt &nummoved) {
  return Move(src,dst,numbytes,nummoved);
}

Status File::Move(StoreAddress src, Address dst, UInt numbytes, UInt &nummoved) {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->Move(src,dst,numbytes,nummoved);
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Status File::Move (Address src, StoreAddress dst, UInt numbytes, UInt &nummoved) {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->Move(src,dst,numbytes,nummoved);
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

Status File::Append(Address src, UInt numbytes, UInt &nummoved) {

  Status  answer;
  File   *deferto;

  deferto = GetDeferee();
  if (deferto) {
    deferDepth++;
    answer = deferto->Append(src,numbytes,nummoved);
    deferDepth--;
  }
  else
    answer = Error::NotCapable;

  return answer;

}

//------------------------------------------------------------------------------
//
// High level Data Movement
//

Status File::ReadLines(Lines &lines) {

  Status  status;
  
  status = lines.Load(*this);
  
  return status;
  
}

Status File::ReadContents(Bytes &bytes) {
  
  Status status;
  UInt   toRead;
  UInt   numRead;

  status = bytes.Reset();
  if (status) goto SOMEERROR;
  
  // Get the contents size
  toRead = Size();
  if (toRead == 0) goto OK;
  
  // If we know the size, set the buffer to that and read it...
  if (!BEFE::IsNull(toRead)) {
    status = bytes.SetLength(toRead);
    if (status) goto SOMEERROR;
    status = Read(0, bytes.GetBuf(), toRead, numRead);
    if (status) goto SOMEERROR;
    if (numRead != toRead) goto TRUNCATED;
    status = bytes.SetLength(toRead);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Don't know the size, read it chunk by chunk...
  {
    Byte buf[4096];
    UInt curPos;
    
    curPos = 0;
    do {
      toRead = sizeof(buf);
      status = Read(curPos, buf, sizeof(buf), numRead);
      if (status) goto SOMEERROR;
      if (numRead == 0) break;
      status = bytes.Append(buf, numRead);
      if (status) goto SOMEERROR;
      curPos += numRead;
    } while (true);
    
  }
  
  // Handle errors
  while (false) {
    SOMEERROR:                                 break;
    OK:        status = Error::None;           break;
    TRUNCATED: status = Error::ValueTruncated; break;
  }
  
  if (status)
    bytes.Reset();
    
  return status;
  
}

Bytes File::ReadContents() {
  
  //Status status;
  Bytes  bytes;
  
  /*status =*/ ReadContents(bytes);
  
  return bytes.Consumable();
  
}

//----------------------------------------------------------------------

File *File::GetDeferee() const {

  if (!deferee && TheBefe && TheBefe->TheOS && !deferDepth)
    THIS.deferee = TheBefe->TheOS->NewFile();
  else if (deferDepth)
    return NULL;
  return deferee;

}

// 
// Method: IsValidBOM - Perform BOM check on first N bytes of the file
//
// This is used internally from various places when a file is Opened/Read,
// and it is passed the first N bytes of the file.
//
// The following cases are checked (see http://en.wikipedia.org/wiki/Byte_order_mark)...
//
//   EF BB BF    - Skip 1st three bytes (it's Microsoft's UTF-8 BOM, crazy!)
//   FE FF       - Error (UTF-16 BE)
//   FF FE       - Error (UTF-16 LE)
//   00 00 FE FF - Error (UTF-32 BE)
//   FF FE 00 00 - Error (UTF-32 LE)
//   2B 2F 76 38 - Error (UTF-7 "+/v8")
//   2B 2F 76 39 - Error (UTF-7 "+/v9")
//   2B 2F 76 2B - Error (UTF-7 "+/v+")
//   2B 2F 76 2F - Error (UTF-7 "+/v/")
//   F7 64 4C    - Error (UTF-1)
//   DD 73 66 73 - Error (UTF-EBCDIC)
//   0E FE FF    - Error (SCSU)
//   FB EE 28    - Error (BOCU-1)
//   84 31 95 33 - Error (GB-18030)
//
// I know it's a bit nasty, but in the first case, it's a Valid Unicode
// character (U+FEFF or U+FFFE) but we need to ignore these because they
// don't matter in the slightest in UTF-8.  So, we return Error::CommandBOM
// (or -2 if you wish to think of it that way) and it's up to whoever called
// Open() (probably FileStream) to skip forward three Bytes to ignore it.
//

Status File::ValidateBOM(Byte *buf, UInt bufLen) const { // File.IsValidBOM...
  
  // Check first case...
  if (bufLen >= 3) {
    if (buf[0] == 0xEF && buf[1] == 0xBB && buf[2] == 0xBF)
      return Error::CommandBOM;
  }
  
  // Check two Byte cases...
  if (bufLen >= 2) {
    if ((buf[0] == 0xFE && buf[1] == 0xFF) ||
        (buf[0] == 0xFF && buf[1] == 0xFE))
      return Error::FileBadInitialBOM;
  }
  
  // Check three Byte cases...
  if (bufLen >= 3) {
    if ((buf[0] == 0xF7 && buf[1] == 0x64 && buf[2] == 0x4C) ||
        (buf[0] == 0x0E && buf[1] == 0xFE && buf[2] == 0xFF) ||
        (buf[0] == 0xFB && buf[1] == 0xEE && buf[2] == 0x28))
      return Error::FileBadInitialBOM;
  }
  
  // Check four Byte cases...
  if (bufLen >= 4) {
    if ((buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0xFE && buf[3] == 0xFF) ||
        (buf[0] == 0xFF && buf[1] == 0xFE && buf[2] == 0x00 && buf[3] == 0x00) ||
        (buf[0] == 0xDD && buf[1] == 0x73 && buf[2] == 0x66 && buf[3] == 0x73) ||
        (buf[0] == 0x84 && buf[1] == 0x31 && buf[2] == 0x95 && buf[3] == 0x33) ||
        (buf[0] == 0x2B && buf[1] == 0x2F && buf[2] == 0x76 && 
           (buf[4] == 0x38 || buf[4] == 0x39 || buf[4] == 0x2B || buf[4] == 0x2F)))
      return Error::FileBadInitialBOM;
  }
  
  return Error::None;

} // ...File.IsValidBOM

}; // Namespace BEFE
