//!befe-publish src
//----------------------------------------------------------------------
// File: main.cpp - BEFE-Lex Main program
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
// Copyright (©) 2011-2012, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"     // For ERROR, etc.
#include "BEFEWrappers.h"   // For general purpose BEFE "Wrapper" functions
#include "Globals.h"
#include "Args.h"
#include "CommandShell.h"
#include "VarTerm.h"
#include "TestLexicon.h"
#include "ToStrings.h"
#include "CommandLayer_BEFE.h"
#include "CommandLayer_OS.h"
//#include "CommandLayer_LexWs.h"
#include "TestLexicon.h"

using namespace BEFE;

#include "Lex.h"

// Special sub-shell commands (we add these to CommandLayer_BEFE's command list)...
static CommandSpec additionalCommands[] = {
   {"O[p[erating]]S[ystem]"   , (CommandHandler)CommandLayer_BEFE_OS,          true}
  ,{"Sh[ell]"                 , (CommandHandler)CommandLayer_BEFE_OS,          true}
  ,{"W[ork]S[pace]"           , (CommandHandler)CommandLayer_LexWs_Workspace,  true}
  ,{NULL, 0, false}
};

//----------------------------------------------------------------------
//
// Function: main - BEFE-Lex main program
//

int main(int argc, char **argv) {

  Status status;
  Befe   theBefe;

  status = Error::None;
	
  do {
    
    String              script;
    String              cmdargs;   // NOTE: We've allocated these in a local scope so the associated
    CommandShell        shell;     //       memory will get freed by the time the Befe exits
    CommandLayer_BEFE   cmdBEFE;
    CommandLayer_OS     cmdOS;
    CommandLayer_LexWs  cmdLexWs;
    String              helpFile;
    UInt                specNo;
		Int                 argNo;
		Strings             args;
		String              arg;
		Char                c;
    
    // Stitch together the CommandShell and theCommandLayers...
    status = shell.RegisterLayer(cmdBEFE);
    status = shell.RegisterLayer(cmdOS);
    status = shell.RegisterLayer(cmdLexWs);
    status = shell.PushLayer("BEFE","");
    if (status) {
      BEFE_ERROR("CommandLayer.PushLayer", status);
      break;
    }

    // Add the extra OS/Lex commands to the CommandLayer_BEFE...
    for (specNo=0; additionalCommands[specNo].varTerm; specNo++) {
      status = cmdBEFE.RegisterAdditionalHandler(additionalCommands[specNo]);
      if (status) {
        BEFE_ERROR("Commandlayer.RegisterAdditionalHandler", status);
        break;
      }
    }
    
    // Tell BEFE what we are...
		theBefe.SetApplicationName("BEFE-Lex");
		theBefe.SetApplicationDescription("BEFE Lexical Analyser");
		theBefe.SetApplicationVersion(BEFE_VERSION);
		theBefe.SetApplicationSubVersion(BEFE_SUBVERSION);
		theBefe.SetApplicationRevision((Int)TheBefe->ThePlatform->GetRevisionString());
    
    // Load '~/.beferc' if it exists...
    if (true) {
      script = ApplyRelativePath(GetCurrentWorkingDirectory(), "~/.beferc");
      status = Error::None;
      if (Exists(script) && IsFile(script)) {
        status = shell.ExecuteScript(script, "", false);
        if (status == Error::CommandExit) break;
      }
    }
    
    // Set the help files...
    helpFile = GetEnvironmentValue("BEFE_Home");
    status = PathToGeneric(helpFile);
    helpFile << '/' << "BEFE_LexHelp.txt";
    status = cmdBEFE.SetHelpFile(helpFile);
    status = cmdOS.SetHelpFile(helpFile);
    
    // Process the arguments...
		for (argNo=1; argNo < argc; argNo++) {
			arg = argv[argNo];
			c = arg.Get(0);
			if (c == '+' || c == '-' || c == '?' || c == '!') {
  			status = Args::ExecuteOption(arg, true);
				if (status) {
					Cout << "Error in argument " << argNo << "...\n";
					BEFE_ERROR("Args.ExecuteOption",status);
				}
			}
			else {
				args.Append(arg);
			}
		}
		
    // Process the arguments...
    cmdargs = Args::ToString(argc,argv);
    
    // CommandShell main loop...
    status = Args::ExecuteOption("+env:BEFE_HelpFile=BEFE_LexHelp.txt");
    status = shell.MainLoop(cmdargs,true);    

    // Display footer if we're supposed to
    if (!gQuiet)
      status = Args::ExecuteOption("?foot",true);
		
  } while (false);

  if (status == Error::CommandExit)
    status = Error::None;
  else if (status)
    BEFE_ERROR("BEFE-Lex.main",status);
    
  return status;

}
