//!befe-publish inc
//----------------------------------------------------------------------
// File: Process.h - Generic Process class declarations
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
#ifndef PROCESS_H // PROCESS_H...
#define PROCESS_H

#include "Primitives.h"
#include "Arrays.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class: Process - Generic process declaration
//
// The default behaviour of this class is to answer "Uknown" everywhere
// it can and provide default behaviour available across all ports.
//

class Process { // Class Process...

  //
  // Protected members
  //

  protected: String exeName;
  protected: String exePath;
  protected: Int    processId;
  protected: String commandLine;
  protected: String commandArgs;

  //
  // Public methods
  //

  // C++ Lifecycle
  public:          Process();
  public: virtual ~Process();

  // BEFE Lifecycle
  public: virtual Status  StartUp();
  public: virtual Status  ShutDown();
  public: virtual Status  ShutDown(Int status);
  public: virtual Status  Reset();
  
  // Current Working Directory
  public: virtual String  GetCurrentWorkingDirectory();
  public: virtual Status  GetCurrentWorkingDirectory(String &fullPath);
  public: virtual Status  SetCurrentWorkingDirectory(String const &fullPath);
  
  // Query
  public: virtual String  GetExecutableFileName();
  public: virtual String  GetExecutablePath();
  public: virtual String  GetCommandLine();
  public: virtual String  GetCommandLineArguments();
  public: virtual Int     GetID();
  public: virtual Strings GetEnvironmentNames();  // ◄── Note: You get a new copy each time you call it
  public: virtual String  GetEnvironmentValue(String const &name); // ◄── Note: You get a new copy each time you call it
  public: virtual NamedStrings GetEnvironmentValues(); // ◄── Note: You get a new copy each time you call it
  public: virtual Status  GetEnvironmentValues(NamedStrings &values);
  public: virtual Boolean AddressIsReadable(void *addr);
  public: virtual Boolean AddressIsWritable(void *addr);
  
  // Flow State Reporting
  public: virtual Status  Abort();
  public: virtual Status  Abort(const char *message);
  public: virtual Status  Abort(String const &message);
  public: virtual Status  Warn (const char *message);
  public: virtual Status  Warn (String const &message);
  public: virtual Status  Debug(const char *message);
  public: virtual Status  Debug(String const &message);

  // Last Process Error (Values are OS Dependent)
  public: virtual UInt    LastErrorCode();
  public: virtual String  LastErrorMessage();
  
  // Process Invocation/Control
  //
  // Note: These are subject to change because we're not happy with them
  //       at the moment...
  public: virtual Status  _Invoke(String const &command);
  public: virtual Status  _Invoke(String const &command, String const &curDir);
  public: virtual Status  _Invoke(String const &command, String const &curDir, NamedStrings const &env);
  public: virtual Status  _Invoke(String       const &command,
                                  String       const &curDir,
                                  NamedStrings const &env,
                                  String       const &stdInFileName,
                                  String       const &stdOutFileName,
                                  String       const &stdErrFileName);
  public: virtual Status  _InvokeCaptureOutput(String const &command, String &output);
  public: virtual Status  _InvokeCaptureOutput(String const &command, String const &curDir, String &output);  

  // Internal methods
  public: Status InitVars();
  
}; // ...Class Process

//----------------------------------------------------------------------
//
// Class: ProcessAny - Any old arbitrairy process declaration
//
// This class provides the ability for any caller to populate the
// members with whatever they want.  This is useful for two purposes...
//
//   a) When we know info about other processes and can populate it.
//
//   b) When we're sub-classing Process for a specific port.  In this
//      case we populate the stuff the port knows about and leave the
//      rest undetermined.
//
//----------------------------------------------------------------------

class ProcessAny : public Process {

  public:

  ProcessAny();
  virtual ~ProcessAny();

  virtual String GetExecutableFileName();
  virtual String GetExecutablePath();
  virtual Int    GetID();

  virtual void   SetExecutableFullPath(const char *fullpath);
  virtual void   SetID(Int id);

  virtual Status Abort(const char *message);
  virtual Status Abort();
  virtual Status Exit(int status);

};

} // ...Namespace BEFE

#endif // ...PROCESS_H