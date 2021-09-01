//----------------------------------------------------------------------
// File: Win32Process.h - Implementation of the Win32Process Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEWrappers.h"
#include "_GlobalMacros.h"
#include "Win32.h"
#include "Win32Console.h"
#include "Win32WCHAR.h"

namespace BEFE { // Namespace BEFE...

extern Console Cout;

// Forward function declarations...
Status PipeIt(HANDLE hRead, HANDLE hWrite);

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Win32Process::Win32Process() {
  StartUp();
}

Win32Process::~Win32Process() {
  ShutDown();
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Win32Process::StartUp() {
  return _StartUp();
}

Status Win32Process::ShutDown() {
  return _ShutDown();
}

Status Win32Process::_StartUp() {

  // If no vtable, make one...
  if (*(UInt *)this == 0) {
    Win32Process x;
    *(Int32 **)this = *(Int32 **)&x;
  }

  Process::StartUp();

  exeName.StartUp();
 
  processId = GetCurrentProcessId();

  if (!TheBefe->TheProcess)
    TheBefe->TheProcess = this;
    
  return Error::None;

}

Status Win32Process::_ShutDown() {

  if (TheBefe->TheProcess) {
    if (TheBefe->TheProcess == this)
      TheBefe->TheProcess = NULL;
    else
      Process::ShutDown();
  }

  return Error::None;
}

String Win32Process::GetCommandLine() {

  if (commandLine.IsNull())
    Win32GetCommandLine(commandLine);

  return commandLine;

}

String Win32Process::GetCommandLineArguments() {

  String tStr;
  Char   quote;
  UInt   pos;

  if (commandArgs.IsNull()) {

    tStr = GetCommandLine();
    if (tStr[0] == '"') {
      for (pos=1; pos < tStr.Length() && tStr[pos] != '"'; pos++);
      if (pos < tStr.Length())
        pos++;
    }
    else {
      for (pos=0; pos < tStr.Length() && tStr[pos] != ' '; pos++);
    }
    while (pos < tStr.Length() && tStr[pos] == ' ') pos++;
    tStr = tStr[Span(pos,NaN)];

  }

  return tStr.Consumable();

}

Int Win32Process::GetID() {
  return processId;
}

//----------------------------------------------------------------------
//
// Current Working Directory
//

String Win32Process::GetCurrentWorkingDirectory() {
  
  //Status status;
  String cwd;
  
  /*status =*/ GetCurrentWorkingDirectory(cwd);
  
  return cwd.Consumable();
  
}

Status Win32Process::GetCurrentWorkingDirectory(String &fullpath) {

  Status status;
  UShort cwd[MAX_PATH];
  Int    cwdl;
  String tString;

  fullpath.SetEmpty();
  cwdl = ::GetCurrentDirectoryW(MAX_PATH,(LPWSTR)cwd);
  if (cwdl > (Int)sizeof(cwd)-1) {
    status = Error::WouldOverflow;
    cwdl = 0;
  }
  else {
    tString = Win32W_ToString((UShort *)cwd);
    status = fullpath.Append(tString.Consumable());
  }

  return status;

}

Status Win32Process::SetCurrentWorkingDirectory(String const &fullPath) {

  Status  status;
  UShort  cwd[MAX_PATH];
  UInt    cwdl;
  Boolean ok;

  if (!Exists(fullPath)) goto NOTEXIST;
  if (!IsDirectory(fullPath)) goto NOTDIR;

  status = Win32W_FromString(fullPath, cwd, MAX_PATH, cwdl);
  if (status) goto SOMEERROR;
  
  ok = ::SetCurrentDirectoryW((LPWSTR)cwd);
  if (!ok) goto OSERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTEXIST:  status = Error::FileDoesNotExist; break;
    NOTDIR:    status = Error::FileNameIsNotDir; break;
    SOMEERROR:                                   break;
    OSERROR:   status = Error::OSError;          break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Query
//

String Win32Process::GetExecutableFileName() {
  InitVars();
  return exeName;
}

String Win32Process::GetExecutablePath() {
  InitVars();
  return exePath;
}

Strings Win32Process::GetEnvironmentNames() {

  UShort  *vars;
  UInt     len;
  Int      equals;
  String   thisOne;
  Strings  names;
  String   name;
  
  vars = (UShort *)GetEnvironmentStringsW();
  if (IsNull(vars)) goto DONE;
  while (*vars != 0x00) {
    len = Win32W_Strlen(vars);
    if (len == 0) break;
    thisOne = Win32W_ToString(vars);
    if (thisOne.Length() == 0) break;
    equals = thisOne.Find('=');
    if (equals > 0) {
      name = thisOne.Get(Span(0,equals));
      names.Append(name.Consumable());
    }
    vars += len + 1;
  }

  // Handle Errors
  while (false) {
    DONE: break;
  }

  return names;

}

String Win32Process::GetEnvironmentValue(String const &name) {

  String   value;
  UShort  *vars;
  UInt     len;
  String   thisOne;
  Int      equals;
  String   varName;
  
  // Get the variable ames
  vars = (UShort *)GetEnvironmentStringsW();
  if (IsNull(vars)) goto DONE;

  // Search the names
  while (*vars != 0x00) {

    // Isolate the name
    len = Win32W_Strlen(vars);
    if (len == 0) break;
    thisOne = Win32W_ToString(vars);
    equals = thisOne.Find('=');
    
    // If name...
    if (equals >= 0) {
      varName = thisOne.Get(Span(0,equals)).Insensitive();
      if (varName == name) {
        value = thisOne.Get(Span(equals+1,len)).Consumable();
        break;
      }
    }
    vars += len + 1;
  }

  // Handle errors
  while (false) {
    DONE: break;
  }

  return value.Consumable();

}

NamedStrings Win32Process::GetEnvironmentValues() {
  
  //Status       status;
  NamedStrings map;
  
  /*status =*/ GetEnvironmentValues(map);
  
  return map.Consumable();
  
}

Status Win32Process::GetEnvironmentValues(NamedStrings &map) {
  
  UShort       *theVars;
  UShort       *vars;
  Int           len;
  String        thisOne;
  Int           equals;
  String        name;
  String        value;
  UInt          numVars;

  map.Reset();  
  map.SetInsensitive();
  
  // See how many there are first...
  theVars = (UShort *)GetEnvironmentStringsW();
  if (IsNull(theVars)) goto DONE;
  vars = theVars;
  numVars = 0;
  while (*vars != 0x00) {
    len = Win32W_Strlen(vars);
    if (len == 0) break;
    numVars++;
    vars += len + 1;
  }
  
  // Build the "normal" ones...
  map.SetLength(numVars);
  vars = theVars;
  while (*vars != 0x00) {
    len = Win32W_Strlen(vars);
    if (len == 0) break;
    thisOne = Win32W_ToString(vars);
    equals = thisOne.Find('=');
    if (equals >0) {
      name = thisOne.Get(Span(0,equals)).Consumable();
      value = thisOne.Get(Span(equals+1, len)).Consumable();
      map.Set(name, value);
    }
    vars += len + 1;
  }

  // Handle errors
  while (false) {
    DONE: break;
  }
  
  return Error::None;
  
}

Boolean Win32Process::AddressIsReadable(void *addr) {
  return Win32MemoryIsReadable(addr);
}

Boolean Win32Process::AddressIsWritable(void *addr) {
  return Win32MemoryIsWritable(addr);
}

//----------------------------------------------------------------------
//
// Last Process Error (Values are OS Dependent)
//

UInt Win32Process::LastErrorCode() {
  
  UInt lastError;
  
  lastError = GetLastError();
  
  return lastError;
  
}

String Win32Process::LastErrorMessage() {
  
  UInt   lastError;
  String lastErrorMessage;
  Byte   buf[MAX_PATH];
  UInt   numBytes;
  
  lastError = GetLastError();
  
  if (lastError != 0) {
    
    numBytes = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, // __in      DWORD dwFlags,
                             NULL,                       // __in_opt  LPCVOID lpSource,
                             lastError,                  // __in      DWORD dwMessageId,
                             LANG_SYSTEM_DEFAULT,        // __in      DWORD dwLanguageId,
                             (LPTSTR)buf,                // __out     LPTSTR lpBuffer,
                             sizeof(buf),                // __in      DWORD nSize,
                             NULL                        // __in_opt  va_list *Arguments
                            );
    lastErrorMessage = String(buf, numBytes);
    if (lastErrorMessage.Get(-1) == '\n') lastErrorMessage.Remove(-1);
    if (lastErrorMessage.Get(-1) == '\r') lastErrorMessage.Remove(-1);
    if (lastErrorMessage.Get(-1) == '\n') lastErrorMessage.Remove(-1);
    if (lastErrorMessage.Get(-1) == '\r') lastErrorMessage.Remove(-1);
    
  }
  
  return lastErrorMessage;
  
}
  
//----------------------------------------------------------------------
//
// Process Invocation/Control
//

Status Win32Process::_Invoke(String const &command) {
  
  Status              status;
  STARTUPINFO         sInfo;
  PROCESS_INFORMATION pInfo;
  String              cmd;
  Byte               *cmdBuf;
  UInt                cmdSize;
  Boolean             ok;
  //UInt                waitResult;

  MemoryZeroRaw((Byte *)&sInfo, sizeof(sInfo));
  sInfo.cb = sizeof(sInfo);
  cmd = command + Char(0u);
  cmd._BufAndSize(cmdBuf, cmdSize);

  ok = CreateProcess(
                     NULL,            // lpApplicationName [in, optional]
                     (char *)cmdBuf,  // lpCommandLine [in, out, optional]
                     NULL,            // lpProcessAttributes [in, optional]
                     NULL,            // lpThreadAttributes [in, optional]
                     TRUE,            // bInheritHandles [in]
                     0,               // dwCreationFlags [in]
                     NULL,            // lpEnvironment [in, optional]
                     NULL,            // lpCurrentDirectory [in, optional]
                     &sInfo,          // lpStartupInfo [in]
                     &pInfo);         // lpProcessInformation [out]
  
  if (!ok) goto OSERROR;
  /*waitResult =*/ WaitForSingleObject(pInfo.hProcess,INFINITE);
  CloseHandle(pInfo.hThread);
  CloseHandle(pInfo.hProcess);
  
  // Handle errors
  status = Error::None;
  while (false) {
    OSERROR:   status = Error::OSError;                    break;
  }
  
  Cout.Flush();
  
  return status;
  
}

Status Win32Process::_Invoke(String const &command, String const &curDir) {
  
  Status              status;
  STARTUPINFO         sInfo;
  PROCESS_INFORMATION pInfo;
  String              cmd;
  Byte               *cmdBuf;
  UInt                cmdSize;
  Boolean             ok;
  //UInt                waitResult;
  String              dir;
  Byte               *dirBuf;
  UInt                dirSize;
  
  // Special case for no curDir specified...
  if (curDir.Length() == 0) {
    status = _Invoke(command);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Get the current directory in the proper form...
  dir = curDir;
  status = TheBefe->TheOS->PathToNative(dir);
  if (status) goto SOMEERROR;
  if (!TheBefe->TheOS->Exists(dir)) goto PATHNOTEXIST;
  if (!TheBefe->TheOS->IsDirectory(dir)) goto PATHNOTDIR;
  dir += Char(0u);
  dir._BufAndSize(dirBuf, dirSize);
  
  // Do it...
  MemoryZeroRaw((Byte *)&sInfo, sizeof(sInfo));
  sInfo.cb = sizeof(sInfo);
  cmd = command + Char(0u);
  cmd._BufAndSize(cmdBuf, cmdSize);

  ok = CreateProcess(
                     NULL,            // lpApplicationName [in, optional]
                     (char *)cmdBuf,  // lpCommandLine [in, out, optional]
                     NULL,            // lpProcessAttributes [in, optional]
                     NULL,            // lpThreadAttributes [in, optional]
                     TRUE,            // bInheritHandles [in]
                     0,               // dwCreationFlags [in]
                     NULL,            // lpEnvironment [in, optional]
                     (char *)dirBuf,  // lpCurrentDirectory [in, optional]
                     &sInfo,          // lpStartupInfo [in]
                     &pInfo);         // lpProcessInformation [out]
  
  if (!ok) goto OSERROR;
  /*waitResult =*/ WaitForSingleObject(pInfo.hProcess,INFINITE);
  CloseHandle(pInfo.hThread);
  CloseHandle(pInfo.hProcess);

  // Handle errors
  status = Error::None;
  while (false) {
    OK:           status = Error::None;                       break;
    PATHNOTEXIST: status = Error::FileDirDoesNotExist;        break;
    PATHNOTDIR:   status = Error::FilePathIsNotDir;           break;
    OSERROR:      status = Error::OSError;                    break;
    SOMEERROR:    status = Error::Win32FailedToInvokeProcess; break;
  }

  Cout.Flush();

  return status;
  
}

Status Win32Process::_Invoke(String       const &command,
                             String       const &curDir,
                             NamedStrings const &env,
                             String       const &stdInFileName,
                             String       const &stdOutFileName,
                             String       const &stdErrFileName) {
  
  Status              status;
  STARTUPINFO         sInfo;
  PROCESS_INFORMATION pInfo;
  String              cmd;
  Byte               *cmdBuf;
  UInt                cmdSize;
  Boolean             ok;
  //UInt                waitResult;
  String              dir;
  Byte               *dirBuf;
  UInt                dirSize;
  Win32File           fileStdIn;
  Win32File           fileStdOut;
  Win32File           fileStdErr;
  
  // Make sure we have a Befe because we need the TheOS and TheConsole  
  if (!TheBefe) goto NOBEFE;
  
  // Special case for no curDir specified...
  if (curDir.Length() == 0) {
    status = _Invoke(command);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Get the current directory in the proper form...
  dir = curDir;
  status = TheBefe->TheOS->PathToNative(dir);
  if (status) goto SOMEERROR;
  if (!TheBefe->TheOS->Exists(dir)) goto PATHNOTEXIST;
  if (!TheBefe->TheOS->IsDirectory(dir)) goto PATHNOTDIR;
  dir += Char(0u);
  dir._BufAndSize(dirBuf, dirSize);
  
  // Setup stdin if specified...
  if (!IsNull(stdInFileName)) {
    status = fileStdIn.SetName(stdInFileName);
    if (status) goto SOMEERROR;
    if (!fileStdOut.Exists()) goto FILENOTEXIST;
    // Open the file for reading
    status = fileStdOut.Open(File::ModeRead);
    if (status) goto SOMEERROR;
    // Make the handle sharable
    ok = SetHandleInformation(fileStdOut.hFile, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    if (!ok) goto OSERROR;
  }
  
  // Setup stdout if specified...
  if (stdOutFileName.Length()) {
    status = fileStdOut.SetName(stdOutFileName);
    if (status) goto SOMEERROR;
    if (!fileStdOut.Exists()) {
      status = fileStdOut.Create();
      if (status) goto SOMEERROR;
    }
    // Open the file for writing
    status = fileStdOut.Open(File::ModeWrite);
    if (status) goto SOMEERROR;
    // Make the handle sharable
    ok = SetHandleInformation(fileStdOut.hFile, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    if (!ok) goto OSERROR;
  }
  
  // Setup stderr if specified...
  if (stdErrFileName.Length()) {
    status = fileStdErr.SetName(stdErrFileName);
    if (status) goto SOMEERROR;
    if (!fileStdOut.Exists()) {
      status = fileStdErr.Create();
      if (status) goto SOMEERROR;
    }
    // Open the file for writing
    status = fileStdErr.Open(File::ModeWrite);
    if (status) goto SOMEERROR;
    // Make the handle sharable
    ok = SetHandleInformation(fileStdErr.hFile, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    if (!ok) goto OSERROR;
  }
  
  // Do it...
  MemoryZeroRaw((Byte *)&sInfo, sizeof(sInfo));
  sInfo.cb          = sizeof(sInfo);
  if (fileStdIn.hFile != INVALID_HANDLE_VALUE)
    sInfo.hStdInput = fileStdIn.hFile;
  else
    sInfo.hStdInput   = Win32Console::stdin;
  if (fileStdOut.hFile != INVALID_HANDLE_VALUE)
    sInfo.hStdOutput = fileStdOut.hFile;
  else
    sInfo.hStdOutput  = Win32Console::stdout;
  if (fileStdErr.hFile != INVALID_HANDLE_VALUE)
    sInfo.hStdError = fileStdErr.hFile;
  else
    sInfo.hStdError   = sInfo.hStdOutput;
  sInfo.dwFlags    |= STARTF_USESTDHANDLES;
  
  cmd = command + Char(0u);
  cmd._BufAndSize(cmdBuf, cmdSize);

  ok = CreateProcess(
                     NULL,            // lpApplicationName [in, optional]
                     (char *)cmdBuf,  // lpCommandLine [in, out, optional]
                     NULL,            // lpProcessAttributes [in, optional]
                     NULL,            // lpThreadAttributes [in, optional]
                     TRUE,            // bInheritHandles [in]
                     0,               // dwCreationFlags [in]
                     NULL,            // lpEnvironment [in, optional]
                     (char *)dirBuf,  // lpCurrentDirectory [in, optional]
                     &sInfo,          // lpStartupInfo [in]
                     &pInfo);         // lpProcessInformation [out]
  
  if (!ok) goto OSERROR;
  
  // Wait until it's finished
  /*waitResult =*/ WaitForSingleObject(pInfo.hProcess,INFINITE);
  CloseHandle(pInfo.hProcess);
  CloseHandle(pInfo.hThread);

  // Handle errors
  status = Error::None;
  while (false) {
    NOBEFE:       status = Error::NoBefe;                     break;
    OK:           status = Error::None;                       break;
    PATHNOTEXIST: status = Error::FileDirDoesNotExist;        break;
    PATHNOTDIR:   status = Error::FilePathIsNotDir;           break;
    FILENOTEXIST: status = Error::FileDoesNotExist;           break;
    OSERROR:      status = Error::OSError;                    break;
    SOMEERROR:    status = Error::Win32FailedToInvokeProcess; break;
  }

  // Clean up
  Cout.Flush();
  
  return status;
  
}

Status Win32Process::_InvokeCaptureOutput(String const &command, String &output) {

  Status  status;
  Bytes   contents;
  String  fileName;
  String  cwd;
  File    file;
  
  status = output.Reset();

  // Try creating the file first...
  status = file.Open();
  if (status) goto SOMEERROR;
  fileName = file.GetName();
  status = file.Close();
  if (status) goto SOMEERROR;
  
  // Invoke the command...
  cwd = GetCurrentWorkingDirectory();
  status = _Invoke(command,           // command
                   cwd,               // curDir,
                   NamedStrings(),    // env,
                   String(),          // stdInFileName
                   fileName,          // stdOutFileName
                   String());         // stdErrFilename
  if (status) goto SOMEERROR;
  
  // Read the output...
  file.Reset();
  status = file.SetName(fileName);
  if (status) goto SOMEERROR;
  status = file.ReadContents(contents);
  if (status) goto SOMEERROR;
  output = contents.MorphToString();
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }

  status = file.Reset();
  status = DeleteFile(fileName);
  if (status && TheBefe && !gQuiet) {
    TheBefe->ErrorStatus("Win32Process::_InvokeCaptureOutput",status);
  }
  
  return status;
  
}

Status Win32Process::_InvokeCaptureOutput(String const &command, String const &curDir, String &output) {
  Status status;
  String cwd;
  
  cwd = GetCurrentWorkingDirectory();
  status = SetCurrentWorkingDirectory(curDir);
  if (status) goto SOMEERROR;
  status = _InvokeCaptureOutput(command, output);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  if (!IsNull(cwd))
    SetCurrentWorkingDirectory(curDir);
    
  return status;
  
}

//----------------------------------------------------------------------
//
// Process Pipe utility functions
//
// Note: These were nicked from Microsoft examples at...
//
//         http://msdn.microsoft.com/en-us/library/ms682499%28v=VS.85%29.aspx
//
// Note: Of couse, we changed them a bit to fit in here.  ;-)
//

#define BUFSIZE 4096

Status PipeIt(HANDLE hRead, HANDLE hWrite) {

   Status status;
   DWORD  dwRead;
   DWORD  dwWritten; 
   CHAR   chBuf[BUFSIZE];
   BOOL   bSuccess = FALSE;

   while (true)  { 
      bSuccess = ReadFile(hRead, chBuf, BUFSIZE, &dwRead, NULL);
      if ( ! bSuccess || dwRead == 0 ) break; 
      
      bSuccess = WriteFile(hWrite, chBuf, dwRead, &dwWritten, NULL);
      if ( ! bSuccess ) break; 
   } 
   if (!bSuccess) goto OSERROR;
   
   // Handle errors
   status = Error::None;
   while (false) {
     OSERROR: status = Error::OSError; break;
   }
   
   return status;
} 
 
//----------------------------------------------------------------------
//
// Internal methods
//

Status Win32Process::InitVars() {
  
  if (exeName.IsNull()) {
    Win32GetProcessExecutableName(exeName);
    if (TheBefe && TheBefe->TheOS)
      exePath = TheBefe->TheOS->PathOnly(exeName);
  }
  
  return Error::None;
  
}
    
} // ...Namespace BEFE
