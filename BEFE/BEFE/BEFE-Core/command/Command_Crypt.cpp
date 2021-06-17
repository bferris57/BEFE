//----------------------------------------------------------------------
// File: Command_OSCrypt.cpp - Command_OS Encryption functions
//----------------------------------------------------------------------
//
// This file implements the Command_OS Encryption commands.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Error.h"
#include "ToStrings.h"
#include "Crypt.h"
#include "CommandDecl.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Function: Command_EncryptFile - EncryptFile Command
//

Status Command_EncryptFile(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           arg;
  String           cwd;
  String           path;
  String           fileName;
  
  // Make sure we've got an OS and a Process...
  if (!TheBefe->TheOS) goto NOOS;
  if (!TheBefe->TheProcess) goto NOPROCESS;
  cwd = TheBefe->TheProcess->GetCurrentWorkingDirectory();

  // Make sure proper number of arguments
  if (cmd.numArgs != 2) goto EXPECTED1;

  // Do it...
  fileName = cmd[1];
  if (TheBefe->TheOS->IsRelativePath(fileName))
    fileName = TheBefe->TheOS->ApplyRelativePath(cwd, fileName);
  status = EncryptFile(fileName);
  if (status) goto SOMEERROR;
  
  //
  // Handle errors
  //

  while (false) {
    NOOS:       status = Error::InternalNoTheOS;      break;
    NOPROCESS:  status = Error::InternalNoTheProcess; break;
    SOMEERROR:                                        break;
    EXPECTED1:  BEFE_WARN("Expected at least one (and only one) argument");
                status = Error::Failed;
                break;
  }

  if (!status)
    Cout << '\n';

  return status;
  
}

//----------------------------------------------------------------------
//
// Function: Command_DecryptFile - DecryptFile Command
//

Status Command_DecryptFile(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           arg;
  String           cwd;
  String           path;
  String           fileName;
  
  // Make sure we've got an OS and a Process...
  if (!TheBefe->TheOS) goto NOOS;
  if (!TheBefe->TheProcess) goto NOPROCESS;
  cwd = TheBefe->TheProcess->GetCurrentWorkingDirectory();

  // Make sure proper number of arguments
  if (cmd.numArgs != 2) goto EXPECTED1;

  // Do it...
  fileName = cmd[1];
  if (TheBefe->TheOS->IsRelativePath(fileName))
    fileName = TheBefe->TheOS->ApplyRelativePath(cwd, fileName);
  status = DecryptFile(fileName);
  if (status) goto SOMEERROR;
  
  //
  // Handle errors
  //

  while (false) {
    NOOS:       status = Error::InternalNoTheOS;      break;
    NOPROCESS:  status = Error::InternalNoTheProcess; break;
    SOMEERROR:                                        break;
    EXPECTED1:  BEFE_WARN("Expected at least one (and only one) argument");
                status = Error::Failed;
                break;
  }

  if (!status)
    Cout << '\n';

  return status;
  
}

//----------------------------------------------------------------------
//
// Function: Command_CreateKeyFile - OS CreateKeyFile Command
//

Status Command_CreateKeyFile(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           arg;
  String           cwd;
  String           path;

  // Make sure we've got an OS and a Process...
  if (!TheBefe->TheOS) goto NOOS;
  if (!TheBefe->TheProcess) goto NOPROCESS;
  cwd = TheBefe->TheProcess->GetCurrentWorkingDirectory();

  // Make sure proper number of arguments
  if (cmd.numArgs != 2) goto EXPECTED1;
  
  // Validate file...
  path = cmd[1];
  status = ValidateNewFileName(path);
  if (status) goto SOMEERROR;
  
  // Do it...
  status = CreateKeyFile(path);
  if (status) goto SOMEERROR;
  
  //
  // Handle errors
  //

  while (false) {
    NOOS:       status = Error::InternalNoTheOS;      break;
    NOPROCESS:  status = Error::InternalNoTheProcess; break;
    SOMEERROR:                                        break;
    EXPECTED1:  BEFE_WARN("Expected at least one (and only one) argument");
                status = Error::Failed;
                break;
  }

  if (!status)
    Cout << '\n';

  return status;

}

//----------------------------------------------------------------------
//
// Function: Command_UseKeyFile - UseKeyFile Command
//

Status Command_UseKeyFile(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           arg;
  String           cwd;
  String           path;

  // Make sure we've got an OS and a Process...
  if (!TheBefe->TheOS) goto NOOS;
  if (!TheBefe->TheProcess) goto NOPROCESS;
  cwd = TheBefe->TheProcess->GetCurrentWorkingDirectory();

  // Make sure proper number of arguments
  if (cmd.numArgs != 2) goto EXPECTED1;
  
  // For now...
  status = Error::NotImplemented;
  goto SOMEERROR;
  
  //
  // Handle errors
  //

  while (false) {
    NOOS:       status = Error::InternalNoTheOS;      break;
    NOPROCESS:  status = Error::InternalNoTheProcess; break;
    SOMEERROR:                                        break;
    EXPECTED1:  BEFE_WARN("Expected at least one (and only one) argument");
                status = Error::Failed;
                break;
  }

  if (!status)
    Cout << '\n';

  return status;

}

} // ...Namespace BEFE
