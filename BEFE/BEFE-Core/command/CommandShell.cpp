//----------------------------------------------------------------------
// File: CommandShell.cpp - Implementaion of the CommandShell class
//----------------------------------------------------------------------
//
// The Command class is responsible for reading command input lines,
// dispatching them to the appropriate Command instances.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "_GlobalMacros.h"
#include "CommandShell.h"
#include "CommandLayer.h"
#include "cmdutils.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// BEFE Instance Lifecycle methods...
//

Status CommandShell::StartUp() {

  Status retStatus;
  Status status;
  
  retStatus = layers.StartUp();
  status = stack.StartUp();
  if (status && !retStatus) retStatus = status;
  popCount  = 0;
  status = inputs.StartUp();
  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status CommandShell::ShutDown() {

  Status      retStatus;
  Status      status;

  retStatus = layers.ShutDown();
  status = stack.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = inputs.ShutDown();

  return retStatus;

}

Status CommandShell::Reset() {
  
  Status   retStatus;
  Status   status;

  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

CommandShell &CommandShell::Consumable()    const {return *(CommandShell *)this;}
CommandShell &CommandShell::NotConsumable() const {return *(CommandShell *)this;}
Boolean       CommandShell::IsConsumable()  const {return false;}

//----------------------------------------------------------------------
//
// State query
//

Boolean CommandShell::IsInteractive() {
  return inputs.Length() == 0;
}

Boolean CommandShell::IsExiting() {
  return popCount > 1;
}

//----------------------------------------------------------------------
//
// List of known Command Processors (CommandLayer *)...
//

Status CommandShell::RegisterLayer(CommandLayer &layer) {

  Status status;

  if (!layers.HasKey(layer.name)) {
    layer.SetShell(this);
    status = layers.Set(layer.name,(UInt)&layer);
  }
  else
    status = Error::None;
  
  return status;
  
}

CommandLayer *CommandShell::FindLayer(String const &layerName) {
  
  Status status;
  CommandLayer *theLayer;
  
  status = FindLayer(layerName, theLayer);
  if (status)
    theLayer = NULL;
    
  return theLayer;
  
}
  
Status CommandShell::FindLayer(String const &layerName, CommandLayer *&pTheLayer) {
  
  Status        status;
  UInt          layerNo;
  union {
    UInt          theLayerUInt;
    CommandLayer *theLayer;
  };
  
  pTheLayer = NULL;
  
  for (layerNo=0; layerNo < layers.Length(); layerNo++) {
    theLayerUInt = layers.Get(layerName);
    if (!BEFE::IsNull(theLayerUInt) && theLayer->name == layerName)
      break;
    status = layers.Get(layerName, theLayerUInt);
    if (!status && !BEFE::IsNull(theLayer) && theLayer->name == layerName)
      break;
  }
  if (layerNo >= layers.Length()) {
    goto NOTFOUND;
  }
  pTheLayer = theLayer;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTFOUND: status = Error::CommandUnknownCommand; break;
  }
  
  return status;
  
}
  
//----------------------------------------------------------------------
//
// Command stack maintenance
//

Status CommandShell::PushLayer(String const &layerName, BcmdCommand &cmd) {

  Status        status;
  CommandLayer *theLayer;

  // Find the layer...
  status = FindLayer(layerName, theLayer);
  if (status) goto SOMEERROR;

  // Tell it we're entering...
  status = theLayer->Enter(cmd);
  if (status) goto SOMEERROR;
  
  // Display any command messages
  if (cmd.numMessages) {
    status = cmd.DisplayMessages("");
    if (status) goto SOMEERROR;
  }

  // Add it to the shell stack
  status = stack.Append((UInt)theLayer);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR: break;
  }

  return status;

}

Status CommandShell::ExitTopLayer() {
  popCount = 1;
  return Error::None;
}

Status CommandShell::ExitAllLayers() {
  gFlowQuitNow = true;
  return Error::None;
}

//----------------------------------------------------------------------
//
// Command Shell File stack maintenance
//

Status CommandShell::PushInputFile(String const &pFileName) {

  Status  status;
  String  fileName;
  Bcmd   *bcmd;
  Bcmd    emptyBcmd;

  // Make sure it exists...
  fileName = pFileName;
  status = ApplyRelativePath(GetCurrentWorkingDirectory(), fileName, fileName);
  if (status) goto SOMEERROR;
  
  // Append empty FileStream...
  status = inputs.Append(emptyBcmd);
  if (status) goto SOMEERROR;

  // Initialise it and set the file name
  bcmd = (Bcmd *)inputs._GetAddress(-1);
  if (BEFE::IsNull(bcmd)) goto NULLPOINTER;
  status = bcmd->SetInputFile(fileName);
  if (status) {
    inputs.Remove(-1);
    goto SOMEERROR;
  }

  // Handle errors
  while (false) {
    SOMEERROR:
      break;
    NULLPOINTER:
      status = Error::InternalNullPointer;
      BEFE_ERROR("CommandShell.PushInputFile Internal Error",status);
      break;
  }

  return status;
  
}

Status CommandShell::PushInputString(String const &string) {

  Status  status;
  Bcmd   *bcmd;
  Bcmd    emptyBcmd;

  // Append empty Bcmd...
  status = inputs.Append(emptyBcmd);
  if (status) goto SOMEERROR;

  // Initialise it and set the string
  bcmd = (Bcmd *)inputs._GetAddress(-1);
  if (BEFE::IsNull(bcmd)) goto NULLPOINTER;
  status = bcmd->SetInputString(string);
  if (status) {
    inputs.Remove(-1);
    goto SOMEERROR;
  }

  // Handle errors
  while (false) {
    SOMEERROR:
      break;
    NULLPOINTER:
      status = Error::InternalNullPointer;
      BEFE_ERROR("CommandShell.PushInputFile Internal Error",status);
      break;
  }

  return status;
  
}

//----------------------------------------------------------------------
//
// Input Processing
//

Status CommandShell::MainLoop(BcmdCommand &pCmd) {

  Status        status;
  Boolean       headerOut;

  union {
    CommandLayer *layer;
    UInt          layerUInt;
  };
  
  UInt         contigErrors;  // Number of contiguous command line errors
  UInt         i;             // Temporary loop counter
  Char         c;             // Temporary Char
  String       prompt;        // Console prompt (if applicable)
  String       tPrompt;       // Temporary Console prompt
  String       cPrompt;       // Temporary Console continuation prompt
  String       tScript;       // Temporary script name (full path)
  Bcmd        *bcmd;          // Dereferenced &inputs[-1]
  BcmdCommand  cmd;           // Current command
  String       errPrefix;     // Error/Message prefix

  if (!TheBefe) goto NOBEFE;
  
  headerOut = false;
  status = Error::None;

  // We were called SO, we'll clear gFlowQuitNow
  gFlowQuitNow = false;

  // If no more CommandLayers left we've already exited...
  if (stack.Length() == 0) goto FINISHED;
  
  // Display header if we haven't already
  if (!headerOut && gShowHeaders) {
    layer = (CommandLayer *)(stack.Get(-1));
    if (BEFE::IsNull(layer)) goto NULLPOINTER;
    if (!gQuiet)
      Cout << '\n';
    layer->DoHeader();
  }

  //
  // Command Loop...
  //

  status = cmd.MoveFrom(pCmd);
  contigErrors = 0;

  while (stack.Length()) { // Until command stack empty...

    // Clear the 'exit' FLAGS
    popCount = 0;

    // Get the current Command Shell...
    layer = (CommandLayer *)stack.Get(-1);
    if (BEFE::IsNull(layer)) goto NULLPOINTER;
    
    // If no further inputs, push a Console one...
    if (inputs.Length() == 0) {
      Bcmd empty;
      status = inputs.Append(empty);
      if (status) goto SOMEERROR;
    }

    // If this is a Console Bcmd, set its prompt...
    bcmd = (Bcmd *)inputs._GetAddress(-1);
    if (BEFE::IsNull(bcmd)) goto NULLPOINTER;

    if (!bcmd->IsFileInput()) { // Is Console input...
    
      // Calculate the prompt
      if (layer->GetPrompt().Length()) {
        tPrompt = layer->GetPrompt();
        tPrompt.MidEllipsis(20);
        cPrompt.Reset();
        for (i=0; i < tPrompt.Length(); i++)
          cPrompt += ' ';
      }
      else {
        tPrompt = "Shell";
        cPrompt = "     ";
      }
      tPrompt += "> ";
      cPrompt += ". ";
      if (cPrompt.Length() >= 4)
        cPrompt.Replace(Range(-4,-3),"..");

      // Indent the prompt
      prompt.SetEmpty();
      if (gCommandIndent) {
        for (i=2*(stack.Length()-1); i > 0; i--)
          prompt += ' ';
      }
      layer->SetIndent(prompt);
      prompt += tPrompt;

      // Set the prompt
      status = bcmd->SetInputPrompt(prompt);
      if (status) goto SOMEERROR;
      status = bcmd->SetContinuePrompt(cPrompt);
      if (status) goto SOMEERROR;
    
    } // ...Is Console input

    // Until we've run out of input...
    while (inputs.Length() && cmd.IsNull()) { // Get Next Command...

      // Get top FileStream in inputFiles....
      bcmd = (Bcmd *)inputs._GetAddress(-1);
      if (BEFE::IsNull(bcmd)) break;

      // Get the line...
      cmd.Reset();
      if (bcmd->IsFileInput())
        status = bcmd->GetNextCommand(cmd);
      else
        status = bcmd->PromptCommand(cmd);
      if (status && status != Error::LangBefeEndOfStream) goto SOMEERROR;

      // If at end of stream, get rid of it and try again...
      if (status == Error::LangBefeEndOfStream) { // End of Stream...

        // Remove this input
        status = inputs.Remove(-1);
        if (status) goto SOMEERROR;

        // If none more, exit...
        if (inputs.Length() == 0) {
          gFlowQuitNow = true;
          break;
        }

        // Else, keep on going
        continue;

      } // ...End of Stream

    } // ...Main Command Loop

    // Execute it...
    if (gFlowQuitNow) break;
    /*status =*/ ExecuteCommand(*bcmd, cmd);
    if (status) goto SOMEERROR;
    if (gFlowQuitNow) break;

    // Bail if too many contiguous errors
    if (bcmd->IsConsoleInput() && contigErrors > 16) goto BAILOUT;

    // If leaving...
    while (popCount && stack.Length()) {

      // Tell this one we're leaving...
      status = stack.Get(-1, layerUInt);
      if (status) goto SOMEERROR;
      if (BEFE::IsNull(layerUInt) || layerUInt == 0) goto NULLPOINTER;
      status = layer->Leave();
      if (status) goto SOMEERROR;
      
      // Remove it from the stack
      status = stack.Remove(-1);
      if (status) goto SOMEERROR;

      popCount--;

    }

    // Reset cmd so it'll read next time
    cmd.Reset();

  }; // ...Until command stack empty

  //
  // Handle errors
  //

  status = Error::None;
  while (false) {
    NOBEFE:      status = Error::NoBefe;
                 break;
    FINISHED:    if (status == Error::CommandExit) status = Error::None;
                 break;
    BAILOUT:     BEFE_WARN("Too many errors, bailing...");
                 status = Error::Failed;
                 break;
    NULLPOINTER: status = Error::InternalNullPointer;
    SOMEERROR:   break;
  }

  // Handle proper "Exit" state
  if (status == Error::None && gFlowQuitNow)
    status = Error::CommandExit;

  return status;

} // ...CommandShell.MainLoop

Status CommandShell::ExecuteScript(String const &fileName, BcmdCommand &pCmd) { // CommandShell.ExecuteScript...

  Status        status;
  String        tPrompt;
  CommandLayer *layer;

  String        line;       // Current command line
  String        tScript;    // Temporary script name (full path)
  Bcmd         *bcmd;       // Input Bcmd
  UInt          numInputs;  // Starting number of input scripts
  BcmdCommand   cmd;        // Current BcmdCommand

  if (!TheBefe) goto NOBEFE;

  // We were called SO, we'll clear gFlowQuitNow
  gFlowQuitNow = false;

  // Try opening the input file...
  numInputs = inputs.Length();
  status = PushInputFile(fileName);
  if (status) goto SOMEERROR;
 
  // Until we're finished...
  
  while (stack.Length() && inputs.Length() > numInputs) { // Until Finished with this script...

    // Get the current Command Shell...
    layer = (CommandLayer *)stack.Get(-1);
    if (BEFE::IsNull(layer)) goto INTERNALNULL;
    
    // Get top FileStream in inputFiles....
    bcmd = (Bcmd *)inputs._GetAddress(-1);
    if (BEFE::IsNull(bcmd)) break;

    // Get current command if we don't already have one...
    if (cmd.IsNull()) { // Get next BcmdCommand...

      status = bcmd->GetNextCommand(cmd);
      if (status && status != Error::LangBefeEndOfStream) goto SOMEERROR;

      // If at end of streeam, pop it and try again...
      if (status == Error::LangBefeEndOfStream) {
        status = inputs.Remove(-1);
        if (status) goto SOMEERROR;
        continue;
      }

    } // ...Get next BcmdCommand

    // Execute it...
    status = ExecuteCommand(*bcmd, cmd);
    if (status) break;
    
    // Reset the BcmdCommand
    status = cmd.Reset();
    if (status) goto SOMEERROR;

  } // ...Until Finished with this script

  //
  // Handle errors
  //

  status = Error::None;
  while (false) {
    NOBEFE:       status = Error::NoBefe;
                  break;
    INTERNALNULL: status = Error::InternalNullPointer;
    SOMEERROR:    BEFE_ERROR("CommandShell.ExecuteScript",status);
                  break;
  }

  // Handle proper "Exit" state
  if (status == Error::None && gFlowQuitNow)
    status = Error::CommandExit;

  return status;

} // ...CommandShell.ExecuteScript

Status CommandShell::ExecuteString(String const &string) { // CommandShell.ExecuteString...

  Status        status;
  String        tPrompt;
  CommandLayer *layer;

  Bcmd         *bcmd;       // Input Bcmd
  UInt          numInputs;  // Starting number of input scripts
  BcmdCommand   cmd;        // Current BcmdCommand

  if (!TheBefe) goto NOBEFE;
  if (gFlowQuitNow) goto EXITALL;
  
  // Try opening the input file...
  numInputs = inputs.Length();
  status = PushInputString(string);
  if (status) goto SOMEERROR;
 
  // Until we're finished...
  
  while (!gFlowQuitNow && stack.Length() && inputs.Length() > numInputs) { // Until Finished with this script...

    // Get the current Command Shell...
    layer = (CommandLayer *)stack.Get(-1);
    if (BEFE::IsNull(layer)) goto NULLPOINTER;
    
    // Get top FileStream in inputFiles....
    bcmd = (Bcmd *)inputs._GetAddress(-1);
    if (BEFE::IsNull(bcmd)) break;

    // Get current command if we don't already have one...
    if (cmd.IsNull()) { // Get next BcmdCommand...

      // Get the next command...
      status = bcmd->GetNextCommand(cmd);
      if (status && status != Error::LangBefeEndOfStream) goto SOMEERROR;

      // If at end of streeam, pop it and try again...
      if (status == Error::LangBefeEndOfStream) {
        status = inputs.Remove(-1);
        if (status) goto SOMEERROR;
        continue;
      }

    } // ...Get next BcmdCommand

    // Execute it...
    status = ExecuteCommand(*bcmd, cmd);
    if (status) goto SOMEERROR;
    if (gFlowQuitNow) break;

    // Reset the BcmdCommand
    status = cmd.Reset();
    if (status) goto SOMEERROR;

  } // ...Until Finished with this script

  //
  // Handle errors
  //

  status = Error::None;
  while (false) {
    NOBEFE:      status = Error::NoBefe;              break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
    EXITALL:     status = Error::CommandExit;         break;
  }

  // Handle proper "Exit" state
  if (status == Error::None && gFlowQuitNow)
    status = Error::CommandExit;

  return status;

} // ...CommandShell.ExecuteString

Status CommandShell::ExecuteCommand(Bcmd &bcmd, BcmdCommand &cmd) { // CommandShell.ExecuteCommand...

  Status status;

  // Normalise error first
  status = cmd.NormaliseErrors();
  if (status) goto SOMEERROR;

  // If we already have errors/warnings (probably Lex errors), just display them
  if (cmd.numErrors || cmd.numWarnings) {
    status = bcmd.DisplayMessages(cmd);
    if (status) goto SOMEERROR;
    goto OK;
  }

  // Execute it...
  status = ExecuteCommand(cmd);
  if (status) goto SOMEERROR;

  // Display any errors/warnings...
  status = cmd.NormaliseErrors();
  if (status) goto SOMEERROR;
  if (cmd.numErrors || cmd.numWarnings) {
    status = bcmd.DisplayMessages(cmd);
    if (status) goto SOMEERROR;
  }

  // Handle errors
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  // Handle proper "Exit" state
  if (status == Error::None && gFlowQuitNow)
    status = Error::CommandExit;

  return status;

} // ...ComandShell.ExecuteCommand

Status CommandShell::ExecuteCommand(BcmdCommand &cmd) { // ComandShell.ExecuteCommand...

  Status        status;
  BcmdArg      *arg0;
  BcmdArg       arg;
  UInt          argNo;

  union {
    CommandLayer *layer;
    UInt          layerUInt;
  };

  if (!TheBefe) goto NOBEFE;
  
  // Nothing to do if no BcmdCommand
  if (cmd.IsNull()) goto OK;

  // Strip off the options first...
  status = cmd.ExecuteOptions();
  if (status) goto SOMEERROR;
  status = cmd.NormaliseErrors();
  if (status) goto SOMEERROR;
  if (cmd.numErrors) goto OK;

  // Do environment variable substitution if we're supposed to...
  if (gCommandEnvSubst && cmd.numArgs) {
    arg0 = (BcmdArg *)cmd.args._GetAddress(0);
    if (BEFE::IsNull(arg0)) goto NULLPOINTER;
    for (argNo=0; argNo < cmd.numArgs; argNo++)
      arg0[argNo].value = TheBefe->SubstituteEnvironmentValues(arg0[argNo].value);
  }

  // Remove any options because we've done them now...
  status = cmd.RemoveOptions();
  if (status) goto SOMEERROR;

  // If no arguments, we're finished
  if (cmd.numArgs == 0) goto OK;

  // Get the current CommandLayer...
  layer = (CommandLayer *)stack.Get(-1);
  if (BEFE::IsNull(layer)) goto NULLPOINTER;

  // Special case for '..'
  arg0 = (BcmdArg *)cmd.args._GetAddress(0);
  if (cmd.numArgs && BEFE::IsNull(arg0)) goto NULLPOINTER;
  if (cmd.numArgs && arg0[0].value == "..") {
    
    // Make sure we have something to pop...
    if (stack.Length()) {

      // Tell this one we're leaving...
      status = stack.Get(-1, layerUInt);
      if (status) goto SOMEERROR;
      if (BEFE::IsNull(layerUInt) || layerUInt == 0) goto NULLPOINTER;
      status = layer->Leave();
      if (status) goto SOMEERROR;
      
      // Remove it from the stack
      status = stack.Remove(-1);
      if (status) goto SOMEERROR;

      // Update popCount if any...
      if (!BEFE::IsNull(popCount) && popCount)
        popCount--;

    }

    goto OK;

  }

  // Execute it...
  status = layer->Execute(cmd);
  if (status && status != Error::CommandUnknownCommand) goto SOMEERROR;
  if (status == Error::CommandUnknownCommand) {
    status = cmd.SetError(0, status, CmdStringMessage(ErrorMessage(status), cmd[0]));
    if (status) goto SOMEERROR;
  }

  // Handle errors
  while (false) {
    NOBEFE:      status = Error::NoBefe;              break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    OK:          status = Error::None;                break;
    SOMEERROR:   //if (status != Error::CommandExit)
                 //  BEFE_ERROR("CommandShell.ExecuteLine",status);
                 break;
  }

  // Handle proper "Exit" state
  if (status == Error::None && gFlowQuitNow)
    status = Error::CommandExit;

  return status;

} // ...CommandShell.ExecuteLine

} // ...Namespace BEFE
