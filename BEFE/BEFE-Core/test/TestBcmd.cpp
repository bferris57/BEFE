//!befe-publish src
//----------------------------------------------------------------------
// File: TestBcmd.cpp - Implementation of the TestBcmd Class
//----------------------------------------------------------------------
//
// This Class implements the Bcmd Class Unit Tests.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "TestBcmd.h"
#include "Bcmd.h"
#include "BcmdArg.h"

using namespace std;

namespace BEFE { // Namespace BEFE...

char const *TestBcmd::typeNames[] = {"Unk", "Err", "Wrn", "Cmt", "Cmd", "Opt", "Arg", "CNT", "EOC", "EOI", "???"};

//----------------------------------------------------------------------
//
// Internal Methods...
//

Status TestBcmd::InitialiseInput(Bcmd &bcmd) {

  Status status;

  //status = bcmd.SetInputFile(??? something from _testdata ???)
  //status = bcmd.SetInputFile("C:/BEFE/t1.befe");
  status = bcmd.SetInputString("echo \"hello world!\", echo \"Line 2\", x!");

  return status;

}

Status TestBcmd::DisplayArg(UInt argNo, BcmdArg &arg) {

  String      typeName;

  if (gVerbose) {

    if (arg.type > BcmdArg::TypeEndOfInput)
      typeName = "???";
    else
      typeName = typeNames[arg.type];
    out << "    Arg[" << argNo << "] = (" << typeName
        << ", " << arg.lineNo+1 << '/' << arg.colNo+1
        << ") «" << arg.value << "»";
    if (arg.type == BcmdArg::TypeOption)
      out << " (TypeOption)";
    else if (arg.type == BcmdArg::TypeArgument)
      out << " (TypeArgument)";
    out << '\n';

  }

  return Error::None;

}

Status TestBcmd::DisplayCommand(UInt cmdNo, BcmdCommand &cmd) {

  Status  status;
  UInt    argNo;
  String  tStr;
  UInt    fg;
  BcmdArg arg;

  fg = Cout.GetForegroundColour();

  // Display Command Header...
  if (gVerbose) {
    Cout << "  Command";
    if (!BEFE::IsNull(cmdNo))
      Cout << "[" << cmdNo << "]";
      Cout << ": numArgs = " << cmd.numArgs
           << ", numErrors = " << cmd.numErrors
           << ", numWarnings = " << cmd.numWarnings;
    if (gVeryVerbose && cmd.numArgs)
      Cout << "...";
    Cout << '\n';
  }

  status = cmd.NormaliseErrors();
  if (status) goto SOMEERROR;

  if (cmd.numErrors) {
    Cout.SetForegroundColour(Console::Red);
    for (argNo = 0; argNo < cmd.numErrors; argNo++) {
      arg = cmd.messages[argNo];
      Cout << arg.value << '\n';
    }
    Cout.SetForegroundColour(fg);
    //continue;
  }

  if (gVeryVerbose) {

    for (argNo = 0; argNo < cmd.numArgs; argNo++) { // For each argument...
      arg = cmd.args[argNo];
      DisplayArg(argNo, arg);
    } // ...For each argument

  }

  // Handle errors
  while (false) {
    SOMEERROR: break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// testArgument - Test Bcmd.GetNextArgument
//

Status TestBcmd::testArgument() { // TestBcmd.testArgument...

  Status      status;
  Bcmd        bcmd;
  BcmdCommand command;
  BcmdArg     arg;
  UInt        argNo;
  String      typeName;
  
  if (gVerbose)
    out << "  Bcmd.NextArgument...\n";

  status = InitialiseInput(bcmd);
  if (status) goto SOMEERROR;

  argNo = 0;
    
  do { // For each argument...
    
    status = bcmd._NextArgument(command, arg);
    if (status) goto SOMEERROR;
    if (arg.type == BcmdArgType::TypeEndOfInput)
      break;
    DisplayArg(argNo, arg);
    argNo++;
      
  } while (true); // ...For each argument
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

} // ...TestBcmd.testArgument

//----------------------------------------------------------------------
//
// testCommand - Test Bcmd.GetNextCommand
//

Status TestBcmd::testCommand() { // TestBcmd.testCommand...

  Status      status;
  Bcmd        bcmd;
  BcmdCommand cmd;
  UInt        cmdNo;

  if (gVerbose)
    out << "  Bcmd.GetNextCommand...\n";

  status = InitialiseInput(bcmd);
  if (status) goto SOMEERROR;
  
  cmdNo = 0;
    
  do { // For each command...
  
    status = bcmd.GetNextCommand(cmd);
    if (status == Error::LangBefeEndOfStream) break;
    if (status) goto SOMEERROR;
    
    if (cmd.numArgs == 0) continue;

    DisplayCommand(cmdNo, cmd);

    cmdNo++;

  } while (true); // ...For each command

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

} // ...TestBcmd.testCommand

//----------------------------------------------------------------------
//
// testPrompt - Test Bcmd.PromptCommand
//

Status TestBcmd::testPrompt() { // TestBcmd.testPrompt...

  Status      status;
  Bcmd        bcmd;
  BcmdCommand cmd;
  BcmdArg     arg;
  String      tStr;

  if (gVerbose)
    out << "  Bcmd.PromptCommand...\n";

  if (!out.IsInteractive()) goto OK;

  status = InitialiseInput(bcmd);
  if (status) goto SOMEERROR;
  
  status = bcmd.SetInputPrompt   ("  .bcmd> ");
  status = bcmd.SetContinuePrompt("     ... ");

  do {

    status = bcmd.PromptCommand(cmd);
    if (status == Error::LangBefeEndOfStream) break;
    if (status) goto SOMEERROR;

    DisplayCommand(UNaN, cmd);

    // For now, if 'e', 'exit', or 'x' or 'x!', we're finished
    arg = cmd.args.Get(0);
    tStr = arg.value;
    tStr.Insensitive();
    if (tStr == "e" || tStr == "exit" || tStr == "x" || tStr == "!x" || tStr == "x!") {
      status = Error::LangBefeEndOfStream;
      break;
    }

  } while (true);

  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;

} // ...TestBcmd.testPrompt

//----------------------------------------------------------------------
//
// Run - Run all Bcmd tests
//

Status TestBcmd::Run() { // TestBcmd.Run...

  Status retStatus;
  Status status;

  if (!gQuiet)
    out << "***Testing Bcmd..." << Endl;

  retStatus = Error::None;
  status    = Error::None;

  //status = testArgument();
  //if (status && !retStatus) retStatus = status;
  status = testCommand();
  if (status && !retStatus) retStatus = status;
  //status = testPrompt();
  //if (status && !retStatus) retStatus = status;

  return retStatus;
 
} // ...TestBcmd.Run
 
} // ...Namespace BEFE
