//!befe-publish src
//----------------------------------------------------------------------
// File: main.cpp - Sample BEFE Main program
//----------------------------------------------------------------------
//
// This is the typical "main()", in that it does all the work.
//
// This "work" it does in this order...
//
//   1) Initialise BEFE (***Always initialise framework first!!!***)
//   2) Parse and perform, if applicable, the command arguments
//   3) Print header if appropriate
//   4) Perform any requested tests
//   5) Finish BEFE (***Always do this last!!!***)
//
// Note: We've illustrated several ways of initialising BEFE below
//       and commented all of the options out except for the recommended
//       way.
//
// NOTE: Actually, this "example" is the real main for the delivered
//       BEFE command line executables.  ;-)
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEWrappers.h"   // For general purpose BEFE "Wrapper" functions
#include "BEFEMacros.h"     // For BEFE_ERROR and other macros
#include "Globals.h"
#include "CommandShell.h"
#include "CommandLayer_BEFE.h"
#include "CommandLayer_OS.h"
#include "CommandLayer_Lang.h"

// For TryThis...
#include "TryThis.h"
// ...For

using namespace BEFE;

//Befe theBefe;             // ◄── You can instantiate a "Global" Befe if you wish

// Special sub-shell commands (we add these to CommandLayer_BEFE's command list)...
static CommandSpec additionalCommands[] = {
   {"O[p[erating]]S[ystem]" , (CommandHandler)CommandLayer_BEFE_OS,        true}
  ,{"Sh[ell]"               , (CommandHandler)CommandLayer_BEFE_OS,        true}
  ,{"Lang[uage]"            , (CommandHandler)CommandLayer_Lang_EnterLang, true}
  ,{NULL, 0, false}
};

//----------------------------------------------------------------------
//
// Function: main - BEFE main program
//

namespace BEFE {
  CommandShell *theShell = 0;
}

int main(int argc, char **argv) {

  Status status;
  Befe   theBefe;           // ◄── Or, you can instantiate one on the stack if you prefer
  //Befe  *theBefe;           // ◄── Or, you can do a 'new'...
  //theBefe = new Befe();     // ◄──   like this

  status = Error::None;

  do {
    
    String            script;
    String            cmdargs;   // NOTE: We've allocated these in a local scope so the associated
    CommandShell      shell;     //       memory will get freed by the time the Befe exits
    CommandLayer_BEFE cmdBEFE;
    CommandLayer_OS   cmdOS;
    CommandLayer_Lang cmdLang;
    UInt              specNo;
    BcmdCommand       empty;
    BcmdCommand       cmd;
    
    theShell = &shell;

    // Stitch together the CommandShell and theCommandLayers...
    status = shell.RegisterLayer(cmdBEFE);
    status = shell.RegisterLayer(cmdOS);
    status = shell.RegisterLayer(cmdLang);
    status = shell.PushLayer("BEFE", empty);
    if (status) {
      BEFE_ERROR("CommandShell.PushLayer", status);
      break;
    }

    // Add the extra OS/Lex commands to the CommandLayer_BEFE...
    for (specNo=0; additionalCommands[specNo].minExp; specNo++) {
      status = cmdBEFE.RegisterAdditionalHandler(additionalCommands[specNo]);
      if (status) {
        BEFE_ERROR("Commandlayer.RegisterAdditionalHandler", status);
        break;
      }
    }
    
    // TEMP...
    //status = trythis();
    //if (status == Error::CommandExit) return 0;
    //if (status)                       return 1;
    // ...TEMP

    // Load '@/.beferc' if it exists...
    if (true) {
      script = ApplyRelativePath(GetCurrentWorkingDirectory(), "@/.beferc");
      if (Exists(script) && IsFile(script)) {
        status = shell.ExecuteScript(script, empty);
        if (status == Error::CommandExit) break;
      }
    }

    // Load '~/.beferc' if it exists...
    if (true) {
      script = ApplyRelativePath(GetCurrentWorkingDirectory(), "~/.beferc");
      if (Exists(script) && IsFile(script)) {
        status = shell.ExecuteScript(script, empty);
        if (status == Error::CommandExit) break;
      }
    }
    
    // Execute the arguments if any...
    if (argc > 1) {
    	//String args = TheBefe->TheProcess->GetCommandLineArguments();
      String args;
    	if (true) {
				for (int i=1;i < argc;i++) {
					String arg = argv[i];
					// Remove surrounding '"' if no spaces (have to do this because of bloody Eclipse or someone else is fiddling with command line, yuck!!!)...
					// Actually, we need to move this to CommandShell!!!!
					if (arg[0] == '"' && arg[-1] == '"' && arg.Length() >= 2 && !IsNull(arg.Find(' ')) ) {
						if (gShowDebug)
						  Cout << "arg[" << i << "] was ►" << arg << "◄";
						arg.Remove(0);
						arg.Remove(-1);
						if (gShowDebug)
						  Cout << " is now ►" << arg << "◄\n";
					}
					// Do it...
					if (i > 1) args += ' ';
					args += arg;
				}
    	}
    	// DEBUG...
    	//if (gShowDebug)
    	//  Cout << "main: args = ►" << args << "◄\n";
    	// ...DEBUG
    	status = shell.ExecuteString(args);
    	// Note: For some odd reason, in Eclipse Neon, the following statement builds
    	//       the argument string with quotes in it. So we reverted to another
    	//       method above.
      //status = shell.ExecuteString(TheBefe->TheProcess->GetCommandLineArguments());
      if (status && status != Error::CommandExit) {
        BEFE_ERROR("BcmdShell.ExecuteCommand", status);
        break;
      }

    }

    // TEMP...
    //status = trythis();
    //if (status == Error::CommandExit) return 0;
    //if (status)                       return 1;
    // ...TEMP

    // CommandShell main loop...
    // NOTE: We need to write Args.GetCommand() or something, in the
    //       meantime we'll pass it an empty BcmdCommand...
    if (!status) {
      status = shell.MainLoop(empty);
      if (status == Error::CommandExit) status = Error::None;
      if (status) BEFE_ERROR("BcmdShell.MainLoop", status);
    }

    // Load '~/.~beferc' if it exists...
    if (!status || status == Error::CommandExit) {
      script = ApplyRelativePath(GetCurrentWorkingDirectory(), "~/.~beferc");
      status = Error::None;
      if (Exists(script) && IsFile(script)) {
        status = shell.ExecuteScript(script, empty);
        if (status == Error::CommandExit) break;
      }
    }
    
  } while (false);

  if (status == Error::CommandExit)
    status = Error::None;
    
  //TheBefe->Exit(status);    // ◄── You can specifically exit the process here.
                            //     But, If you do this you'll have some un-freed memory
                            //     reported if +ms/+mas was specified because the Befe
                            //     itself will have some memory allocated.  So, it's
                            //     best to just delete it manually
                           
  //delete theBefe;           // ◄── If you dynamically allocated the Befe, you can free it here

  return status;

}
