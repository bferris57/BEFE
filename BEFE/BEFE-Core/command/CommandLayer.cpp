//----------------------------------------------------------------------
// File: CommandLayer.cpp - Implementaion of the CommandLayer class
//----------------------------------------------------------------------
//
// The CommandLayer class is responsible for parsing and executing BEFE
// command line arguments and command lines for a closely related set
// of commands.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Error.h"
#include "CommandLayer.h"
#include "MinExp.h"
#include "Test.h"
#include "ToStrings.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

CommandLayer::CommandLayer() {
  StartUp();
}

CommandLayer::~CommandLayer() {
  ShutDown();
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle methods...
//

Status CommandLayer::StartUp() { // CommandLayer.StartUp...

  Status retStatus;
  Status status;

  shell = NULL;
  retStatus = name.StartUp();
  status = prompt.StartUp();
  if (status && !retStatus) retStatus = status;
  status = indent.StartUp();
  if (status && !retStatus) retStatus = status;
  status = helpFileName.StartUp();
  if (status && !retStatus) retStatus = status;
  returnCode = Error::None;
  inheritCommands = true;
  status = commands.StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

} // ...CommandLayer.StartUp

Status CommandLayer::ShutDown() { // CommandLayer.ShutDown...

  Status retStatus;
  Status status;

  retStatus = name.ShutDown();
  status = name.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = prompt.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = indent.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = helpFileName.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = commands.ShutDown();
  if (status && !retStatus) retStatus = status;

  return status;

} // ...CommandLayer.ShutDown

//----------------------------------------------------------------------
//
// CommandShell knowledge
//

CommandShell *CommandLayer::GetShell() {
  return shell;
}

Status CommandLayer::SetShell(CommandShell *theShell) {
  shell = theShell;
  return Error::None;
}

Status CommandLayer::SetIndent(String const &pIndent) {
  Status status;
  status = indent.CopyFrom(pIndent);
  return status;
}

Status CommandLayer::SetHelpFile(String const &pHelpFileName) {
  
  Status status;
  String tFileName;
  
  // If help file name isn't Null, validate it...
  tFileName = pHelpFileName;
  if (!tFileName.IsNull()) {
    status = ValidateExistingFileName(tFileName);
    if (status) goto SOMEERROR;
  }
  
  // Set it
  helpFileName = tFileName.Consumable();
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Execute
//

Status CommandLayer::Enter(BcmdCommand &cmd) {
  return Error::None;
}

Status CommandLayer::Execute(BcmdCommand &cmd) { // CommandLayer.Execute...

  Status       status;
  String       cmdName;
  CommandDecl *decls;
  UInt         numDecls;
  UInt         declNo;
  
  // Register commands if not already...
  status = RegisterHandlers();
  if (status) goto SOMEERROR;
  
  // Nothing to do if no command name...
  cmdName = cmd[0];
  if (cmdName.IsNull()) goto EMPTY;

  // Find the command
  numDecls = commands.Length();
  decls = (CommandDecl *)commands._GetAddress(0);
  if (BEFE::IsNull(decls)) goto NULLPOINTER;
  for (declNo=0; declNo < numDecls; declNo++) {
    if (decls[declNo].name == cmdName)
      break;
  }
  
  // Handle "Unknown Command" case...
  if (declNo >= numDecls) goto UNKNOWN;

  // Invoke the callback
  status = (decls[declNo].handler)(*this, cmd);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR:                                          break;
    UNKNOWN:     status = Error::CommandUnknownCommand; break;
    EMPTY:       status = Error::CommandMissing;        break;
    NULLPOINTER: status = Error::InternalNullPointer;   break;
  }

  return status;

} // ...CommandLayer.Execute

Status CommandLayer::Leave() {
  return Error::None;
}

//----------------------------------------------------------------------
//
// Header/Trailer management
//

Status CommandLayer::DoHeader() {

  if (gShowHeaders)
    BEFE_HEADER();

  return Error::None;

}

Status CommandLayer::DoTrailer() {

  if (gShowHeaders) {
    BEFE_TRAILER();
  }

  return Error::None;

}

//----------------------------------------------------------------------
//
// Command helpers
//

void CommandLayer::Msg(const char *msg) { // CommandLayer.Msg...

  Cout << '\n';
  if (msg) {
    Cout << indent;
    Cout << msg;
    Cout << '\n';
  }

} // ...CommandLayer.Msg

void CommandLayer::Note(const char *msg) { // CommandLayer.Note...

  String  message;
  Strings parts;
  Int     numParts;
  Int     partNo;
  String  ourIndent;

  message = msg;
  parts = message.SplitToStrings('\n');
  numParts = parts.Length();
  Cout << '\n';
  ourIndent = ' ';
  ourIndent *= 6;
  for (partNo=0;partNo<numParts;partNo++) {
    message = parts.Get(partNo);
    if (partNo == 0)
      Cout << indent << "Note: ";
    else
      Cout << indent << ourIndent;
    Cout << message << '\n';
  }
  Cout << '\n';

} // ...CommandLayer.Note

//----------------------------------------------------------------------
//
// Command handler registry (protected)
//

Status CommandLayer::RegisterHandlers() {
  return Error::None;
}

Status CommandLayer::RegisterHandlers(CommandSpec const *specs) { // CommandLayer.RegisterHandlers...

  Status status;
  UInt   specNo;
  
  status = Error::None;
  if (commands.Length() == 0) {
    for (specNo=0; specs && specs[specNo].minExp; specNo++) {
      status = RegisterHandler(specs[specNo]);
      if (status) break;
    }
  }

  // Display error if any...
  if (status)
    BEFE_ERROR("CommandLayer.RegisterHandlers", status);
    
  return status;

} // ...CommandLayer.RegisterHandlers

Status CommandLayer::RegisterHandler(CommandSpec const &spec) { // CommandLayer.RegisterHandler...

  Status      status;
  CommandDecl decl;
  
  // Validate parameters
  if (BEFE::IsNull((void *)spec.handler))
    goto BADPARM;

  // Form a CommandDecl from it
  decl.name = spec.minExp;
  if (spec.insensitive)
    decl.name.Insensitive();
  decl.handler = spec.handler;

  // Register the CommandDecl...
  status = RegisterHandler(decl);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    BADPARM:     status = Error::InvalidParameter;         break;
    SOMEERROR:                                             break;
  }

  return status;

} // ...CommandLayer.RegisterHandler

Status CommandLayer::RegisterHandler(CommandDecl const &decl) { // CommandLayer.RegisterHandler...

  Status       status;
  UInt         cmdNo;
  UInt         numCmds;
  MinExp       var;
  UInt         numTerms;
  UInt         termNo;
  Strings      terms;
  String       term;
  CommandDecl  tDecl;
  CommandDecl *decls;
  
  // Validate parameters
  if (BEFE::IsNull((void *)decl.handler))
    goto BADPARM;

  // Special case for Null/Empty name, we'll take this to be the
  // "Unknown Command" handler...
  if (decl.name.Length() == 0) {
    // Make sure it's not already registered
    numCmds = commands.Length();
    decls = (CommandDecl *)commands._GetAddress(0);
    if (BEFE::IsNull((void *)decls) && numCmds) goto NULLPOINTER;
    for (cmdNo=0; cmdNo < numCmds; cmdNo++)
      if (decls[cmdNo].name == decl.name) goto DUPLICATE;
    // Not already registered, register it...
    status = commands.Append(decl);
    if (status) goto SOMEERROR;
    goto OK;
  }
      
  //
  // Handle each term...
  //

  status = var.Set(decl.name);
  if (status) goto SOMEERROR;
  numTerms = var.Count();

  for (termNo=0; termNo < numTerms; termNo++) { // For each term...

    // Get the Term
    term = var[termNo];
    if (term.Length() == 0) goto INTERNAL;
    if (decl.name.IsInsensitive())
      term.Insensitive();
      
    // Make sure it's not already registered
    // TODO: This needs to use a Sparse String/cmdNo array
    //       and use Exists() or whatever...  We're getting
    //       way too many allocations by using Get()
    //       continuously
    numCmds = commands.Length();
    decls = (CommandDecl *)commands._GetAddress(0);
    if (BEFE::IsNull((void *)decls) && numCmds) goto NULLPOINTER;
    for (cmdNo=0; cmdNo < numCmds; cmdNo++)
      if (decls[cmdNo].name == term) goto DUPLICATE;

    // Set up our decl
    tDecl.name.MoveFrom(term);
    tDecl.name.Consumable();
    tDecl.handler = decl.handler;
    status = commands.Append(tDecl);
    if (status) goto SOMEERROR;

  } // ...For each term

  // Handle errors
  status = Error::None;
  while (false) {
    OK:          status = Error::None;                     break;
    BADPARM:     status = Error::InvalidParameter;         break;
    DUPLICATE:   status = Error::CommandAlreadyRegistered; break;
    NULLPOINTER: status = Error::InternalNullPointer;      break;
    INTERNAL:    status = Error::Internal;                 break;
    SOMEERROR:                                             break;
  }

  return status;

} // ...CommandLayer.RegisterHandler

Status CommandLayer::RegisterAdditionalHandler(CommandSpec const &spec) {
  Status status;
  status = RegisterHandlers();
  if (!status)
    status = RegisterHandler(spec);
  return status;
}

Status CommandLayer::RegisterAdditionalHandler(CommandDecl const &decl) {
  Status status;
  status = RegisterHandlers();
  if (!status)
    status = RegisterHandler(decl);
  return status;
}

//----------------------------------------------------------------------
//
// Settings
//

String CommandLayer::GetPrompt() const {
	
	String tPrompt;
	
	tPrompt = prompt;
	
	return tPrompt.Consumable();
	
}

Status CommandLayer::SetPrompt(String const &str) {
  prompt = str;
  return Error::None;
}

} // ...Namespace BEFE
