//----------------------------------------------------------------------
// File: Process.cpp - Generic platform process implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// NOTE: The "real" implementation of these classes is up to the
//       specific port that the product was built for.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Process.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Process::Process() {
  StartUp();
}

Process::~Process() {
  ShutDown();
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Process::StartUp() {

  if (this != TheBefe->TheProcess) {
    exeName = CONSTR_Unknown;
    exePath.ShutDown();
    processId = NaN;
    commandLine = String();
    commandArgs = String();
  }

  return Error::None;
}

Status Process::ShutDown() {

  exeName.ShutDown();
  exePath.ShutDown();
  commandLine.ShutDown();
  commandArgs.ShutDown();

  return Error::None;

}

Status Process::Reset() {
  
  exeName.Reset();
  exePath.Reset();

  return Error::None;
  
}

Status Process::ShutDown(Int status) {
  ShutDown();
  return Error::None;
}

//----------------------------------------------------------------------
//
// Current Working Directory
//

String Process::GetCurrentWorkingDirectory() {
  
  //Status status;
  String fullPath;
  
  /*status =*/ GetCurrentWorkingDirectory(fullPath);
  
  return fullPath.Consumable();
  
}

Status Process::GetCurrentWorkingDirectory(String &fullpath) {
  fullpath = String();
  return Error::NotCapable;
}

Status Process::SetCurrentWorkingDirectory(String const &fullpath) {
  return Error::NotCapable;
}

//----------------------------------------------------------------------
//
// Query
//

String Process::GetExecutableFileName() {
  return exeName;
}

String Process::GetExecutablePath() {

  String  string;
  Strings parts;
  Char    sep;

  if (TheBefe->TheOS) {
    sep = TheBefe->TheOS->GetFileSeparator();
    parts = exeName.SplitToStrings(sep);
    parts.Remove(-1);
    string = parts.Join(sep);
  }

  return string;
}

Int Process::GetID() {
  return processId;
}

String Process::GetCommandLine() {
  return commandLine;
}

String Process::GetCommandLineArguments() {
  return commandArgs;
}

Strings Process::GetEnvironmentNames() {
  return Strings();
}

String Process::GetEnvironmentValue(String const &name) {
  return String();
}

NamedStrings Process::GetEnvironmentValues() {
  return NamedStrings();
}

Status Process::GetEnvironmentValues(NamedStrings &values) {
  
  //Status status;
  
  /*status =*/ values.SetEmpty();
  
  return Error::PlatformNotApplicable;

}

Boolean Process::AddressIsReadable(void *addr) {
  return false;
}

Boolean Process::AddressIsWritable(void *addr) {
  return false;
}

//----------------------------------------------------------------------
//
// Flow State Reporting
//

Status Process::Abort() {
  BEFE_ABORT(NULL);
  return Error::NotCapable;
}

Status Process::Abort(const char *message) {
  BEFE_ABORT(message);
  return Error::None;
}

Status Process::Abort(String const &message) {
  BEFE_ABORT(message);
  return Error::None;
}

Status Process::Warn(char const *message) {
  BEFE_WARN(message);
  return Error::None;
}

Status Process::Warn(String const &message) {
  BEFE_WARN(message);
  return Error::None;
}

Status Process::Debug(char const *message) {
  if (gShowDebug)
    Cout << "Debug: " << message << Endl;
  return Error::None;
}

Status Process::Debug(String const &message) {
  if (gShowDebug)
    Cout << "Debug: " << message << Endl;
  return Error::None;
}

//----------------------------------------------------------------------
//
// Last Process Error (Values are OS Dependent)
//

UInt Process::LastErrorCode() {
  return 0;
}

String Process::LastErrorMessage() {
  return String();
}
  
//----------------------------------------------------------------------
//
// Process Invocation/Control
//

Status Process::_Invoke(String const &message) {
  return Error::NotCapable;
}

Status Process::_Invoke(String const &command, String const &curDir) {
  return Error::NotCapable;
}

Status Process::_Invoke(String const &command, String const &curDir, NamedStrings const &env) {
  return Error::NotCapable;
}

Status Process::_Invoke(String       const &command,
                        String       const &curDir,
                        NamedStrings const &env,
                        String       const &stdInFileName,
                        String       const &stdOutFileName,
                        String       const &stdErrFileName) {
  return Error::NotCapable;
}

Status Process::_InvokeCaptureOutput(String const &command, String &output) {
  return Error::NotCapable;
}

Status Process::_InvokeCaptureOutput(String const &command, String const &curDir, String &output) {
  return Error::NotCapable;
}

} // ...Namespace BEFE
