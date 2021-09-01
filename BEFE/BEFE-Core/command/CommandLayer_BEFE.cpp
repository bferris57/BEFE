//----------------------------------------------------------------------
// File: Commands.cpp - Implementation of Command Class Handlers
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "cmdutils.h"
#include "CommandShell.h"
#include "CommandLayer.h"
#include "CommandLayer_BEFE.h"
#include "Commands.h"

#include "Test.h"
#include "MinExp.h"
#include "MemoryStats.h"
#include "FileStream.h"
#include "Help.h"
#include "ToStrings.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Command Specifications
//

static CommandSpec commandSpecs[] = {

  // Exit...
   {"e[xit]"                  , (CommandHandler)CommandLayer_BEFE_Exit, true}
  ,{"x"                       , (CommandHandler)CommandLayer_BEFE_Exit, true}
  ,{"d[one]"                  , (CommandHandler)CommandLayer_BEFE_Exit, true}
  ,{"q[uit]"                  , (CommandHandler)CommandLayer_BEFE_Exit, true}

  // Help...
  ,{"h[elp]"                  , (CommandHandler)CommandLayer_BEFE_Help,  true}
  ,{"?"                       , (CommandHandler)CommandLayer_BEFE_Help,  true}

  // Switches...
  ,{"Sw[itch[es]]"            , (CommandHandler)CommandLayer_BEFE_Switches,    true}
  ,{"Arg[ument][s]"           , (CommandHandler)CommandLayer_BEFE_Switches,    true}
  ,{"Opt[ion][s]"             , (CommandHandler)CommandLayer_BEFE_Switches,    true}
  ,{"S[how]A[rg[ument][s]]"   , (CommandHandler)CommandLayer_BEFE_ShowArgs,    true}
  ,{"S[how]Sw[itch[es]]"      , (CommandHandler)CommandLayer_BEFE_ShowArgs,    true}
  ,{"S[how]Opt[ion][s]"       , (CommandHandler)CommandLayer_BEFE_ShowArgs,    true}

  // Environment Variables...
  ,{"set"                     , (CommandHandler)CommandLayer_BEFE_Set,         true}
  ,{"env[iron]"               , (CommandHandler)CommandLayer_BEFE_Set,         true}
  ,{"r[ef[resh]]env[iron]"    , (CommandHandler)CommandLayer_BEFE_RefreshEnv,  true}
  
  // Date/Time...
  ,{"Date"                    , (CommandHandler)CommandLayer_BEFE_DateTime,    true}
  ,{"Time"                    , (CommandHandler)CommandLayer_BEFE_DateTime,    true}

  // Tests...
  ,{"Test[s]"                 , (CommandHandler)CommandLayer_BEFE_Tests,       true}

  // Statistics
  ,{"Stat[istic][s]"          , (CommandHandler)CommandLayer_BEFE_Statistics,  true}
  ,{"Mem[ory]Stat[istic][s]"  , (CommandHandler)CommandLayer_BEFE_MemoryStats, true}
  ,{"St[r[eam]]Stat[istic][s]", (CommandHandler)CommandLayer_BEFE_StreamStats, true}

  // Run
  ,{"Run"                     , (CommandHandler)CommandLayer_BEFE_Run,         true}
  ,{"Exec[ute]"               , (CommandHandler)CommandLayer_BEFE_Run,         true}

  // Build
  ,{"B[uild]"                 , (CommandHandler)CommandLayer_BEFE_Build,       true}

	// UUID
	,{"UUID"                    , (CommandHandler)CommandLayer_BEFE_UUID,        true}
	,{"GUID"                    , (CommandHandler)CommandLayer_BEFE_UUID,        true}
	
  // Other
	,{"Echo"                    , (CommandHandler)CommandLayer_BEFE_Echo,        true}
  ,{"Cl[ear]S[creen]"         , (CommandHandler)CommandLayer_BEFE_ClearScreen, true}

  ,{NULL,NULL,false}
};

//----------------------------------------------------------------------
//
// C++ Lifecycle...
//

CommandLayer_BEFE::CommandLayer_BEFE() {  
  StartUp();  
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle...
//

Status CommandLayer_BEFE::StartUp() {
  
  Status status;
  
  status = CommandLayer::StartUp();
  name = "BEFE";
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Command handler registry
//

Status CommandLayer_BEFE::RegisterHandlers() {

  Status status;

  status = CommandLayer::RegisterHandlers(commandSpecs);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR: break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Functions: CommandLayer_BEFE_Exit   - Handle 'exit' command
//            CommandLayer_BEFE_Finish - Handle '..' command
//            CommandLayer_BEFE_Help   - Handle 'help' command
//

Status CommandLayer_BEFE_Exit(CommandLayer *layer, BcmdCommand &cmd) {

  Status status;

  if (BEFE::IsNull(layer->shell)) goto NOSHELL;

  status = layer->shell->ExitAllLayers();

  while (false) {
    NOSHELL: status = Error::CmdLayerNoShell; break;
  }

  return status;

}

Status CommandLayer_BEFE_Finish(CommandLayer *layer, BcmdCommand &cmd) {

  Status status;

  if (BEFE::IsNull(layer->shell)) goto NOSHELL;

  status = layer->shell->ExitTopLayer();

  while (false) {
    NOSHELL: status = Error::CmdLayerNoShell; break;
  }

  return status;

}

Status CommandLayer_BEFE_Help(CommandLayer *layer, BcmdCommand &cmd) {

  Status  status;           // Return status
  String  option;           // Current option we're looking at
  Int     optIdx;           // Current argument/option index
  MinExp  var;              // Variable Term to test against
  String  errMsg;           // Temporary error/warning message

  // Validate parameters
  if (BEFE::IsNull(layer)) goto BADPARAM;
  
  //
  // Depending on the options...
  //
  
  if (cmd.numArgs == 0) goto MISSING;

  else if (cmd.numArgs == 1) { // Help by itself...

    Help(layer->helpFileName, "BEFE");

  } // ...Help by itself

  else { // Help Options...

    for (optIdx=1; optIdx < (Int)cmd.numArgs; optIdx++) { // For each option...

      // Get the option
      option = cmd[optIdx];

      // If Command...
      var.Set("C[[o]m[man]d]");
      if (var.MatchInsensitive(option)) {
        Help(layer->helpFileName, "Command");
        continue;
      }

      // If Switches...
      var.Set("S[witch[es]]");
      if (var.MatchInsensitive(option)) {
        Help(layer->helpFileName, "Switches");
        continue;
      }

      // If Tests...
      var.Set("T[est[s]]");
      if (var.MatchInsensitive(option)) {
        Help(layer->helpFileName, "Tests");
        continue;
      }

      // If OS...
      var.Set("O[perating]S[ystem]");
      if (var.MatchInsensitive(option)) {
        Help(layer->helpFileName, "OS");
        continue;
      }

      // If Lexicon...
      var.Set("L[ex[icon]]");
      if (var.MatchInsensitive(option)) {
        Help(layer->helpFileName, "Lexicon");
        continue;
      }

      // If Statistics...
      var.Set("Stat[istic][s]");
      if (var.MatchInsensitive(option)) {
        Help(layer->helpFileName, "Statistics");
        continue;
      }

      // If Build...
      var.Set("B[ui]ld");
      if (var.MatchInsensitive(option)) {
        Help(layer->helpFileName, "Build");
        continue;
      }

      // If Environment...
      var.Set("Env[iron[ment]]");
      if (var.MatchInsensitive(option)) {
        Help(layer->helpFileName, "Environment");
        continue;
      }
      
      // Unknown option
      status = Error::CmdBEFEUnknownHelp;
      status = cmd.SetError(optIdx, status, CmdStringMessage(status, option));
      if (status) goto SOMEERROR;

    } // ...For each option

  } // ...Help Options

  // Handle errors
  status = Error::None;
  while (false) {
    BADPARAM:  status = Error::InvalidParameter; break;
    MISSING:   status = Error::CommandMissing;   break;
    SOMEERROR:                                   break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Functions: CommandLayer_BEFE_Switches - Handle 'sw[itch[es]]' command
//            CommandLayer_BEFE_DateTime - handle 'Date' and 'Time' commands
//            CommandLayer_BEFE_Set - Handle 'Set' command
//            CommandLayer_BEFE_RefreshEnv - Handle 'RefreshEnviron' command
//            CommandLayer_BEFE_ShowArgs - Handle 'ShowArgs' command
//

Status CommandLayer_BEFE_Switches(CommandLayer *layer, BcmdCommand &cmd) {

  Status  status;
  
  // If no switches, display them...
  if (cmd.numArgs <= 1) {
    OutputSwitches();
    goto OK;
  }
  
  // Handle errors
  status = Error::NotImplemented;
  while (false) {
    OK: status = Error::None; break;
  }

  return status;

}

Status CommandLayer_BEFE_DateTime(CommandLayer *layer, BcmdCommand &cmd) {

  Boolean saveOpt;
  Time    now;

  now.Set();

  Cout << '\n';
  saveOpt = gShowLocalTime;
  gShowLocalTime = true;
  Cout << ErrorString(Error::CmdBEFE_Local) << ToString(now,Time::Seconds) << "\n\n";
  gShowLocalTime = false;
  Cout << ErrorString(Error::CmdBEFE_GMTUTC) << ToString(now,Time::Seconds) << "\n\n";
  gShowLocalTime = saveOpt;

  return Error::None;

}

Status CommandLayer_BEFE_Set(CommandLayer *layer, BcmdCommand &cmd) {

  Status status;    // Return status
  UInt   argNo;     // Current argument number
  String empty;     // Temp: An empty string

  if (IsNull(TheBefe)) goto NOBEFE;
  
  //
  // Handle 'set' without arg...
  //
  
  if (cmd.Length() <= 1) {
    status = cmd._QueryEnv(0, empty, empty);
    if (status) goto SOMEERROR;
    goto OK;
  }

  //
  // Handle each '<nam>[=<val>]'...
  //

  for (argNo=1; argNo < cmd.Length(); argNo++) {
    status = cmd._ExecuteEnvOption(argNo);
    if (status) goto SOMEERROR;
  }
  
  //
  // Handle errors
  //
  
  while (false) {
    NOBEFE:    status = Error::NoBefe; break;
    OK:        status = Error::None;   break;
    SOMEERROR:                         break;
  }

  return status;

}

Status CommandLayer_BEFE_RefreshEnv(CommandLayer *layer, BcmdCommand &cmd) {

  Status        status;
  Strings       theKeys;
  String        name;
  String        value;
  Char          quote;
  
  if (IsNull(TheBefe)) goto NOBEFE;
  
  // Handle bad cases
  if (cmd.Length() > 1) {
    status = cmd.SetError(1, Error::CmdLang_ExpectedNoArgs);
    goto SOMEERROR;
  }
  
  // Do it...
  status = TheBefe->RefreshEnvironment();
  if (status) goto SOMEERROR;
  
  //
  // Handle errors
  //
  
  while (false) {
    NOBEFE:    status = Error::NoBefe; break;
    SOMEERROR:                         break;
  }

  return status;

}

Status CommandLayer_BEFE_ShowArgs(CommandLayer *layer, BcmdCommand &cmd) {

  Cout << '\n';
  OutputSwitches();
  Cout << '\n';

  return Error::None;

}

//----------------------------------------------------------------------
//
// Functions: CommandLayer_BEFE_Run   - Execute the 'Run' command
//            CommandLayer_BEFE_Build - Execute the 'Build' command
//            CommandLayer_BEFE_UUID   - Execute the 'UUID' command
//

Status CommandLayer_BEFE_Run(CommandLayer *layer, BcmdCommand &cmd) {

  Status  status;
  Strings scriptExts;
  String  scriptName;

  if (BEFE::IsNull(layer->shell)) goto NOSHELL;

  // Make sure at least a script was specified...
  if (cmd.numArgs < 2) {
    status = cmd.AppendError(Error::CmdBEFERunWhat);
    goto SOMEERROR;
  }

  // Remove "run" argument
  status = cmd.args.Remove(0);
  if (status) goto SOMEERROR;
  cmd.numArgs--;

  // Find the script file
  scriptExts = String(".bef,.befe").SplitToStrings(',');
  status = TheBefe->TheOS->FindInSearchPath(cmd[0], scriptExts, scriptName);
  if (status) goto SOMEERROR;

  // Execute it...
  status = layer->shell->ExecuteScript(scriptName, cmd);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    NOSHELL:   status = Error::Internal; BEFE_ERROR("Command_Run",status); break;
    SOMEERROR:                                                             break;
  }

  return status;

}

Status CommandLayer_BEFE_Build(CommandLayer *layer, BcmdCommand &cmd) {

  Status  status;
  String  befe_GIT;
  
  befe_GIT = GetEnvironmentValue("BEFE_GIT").Trim();
  if (befe_GIT.Length() == 0) goto NOBEFEGIT;
  if (!Exists(befe_GIT)) goto NOTEXIST;
  if (!IsDirectory(befe_GIT)) goto NOTDIR;

  // Do it
  status = Command_Build(layer, cmd);
  if (status) goto BAD;
  
  // Handle errors  
  while (false) {
    NOBEFEGIT:  status = Error::NoBEFE_GIT;           break;
    //BADMACHINE: status = Error::UtilBuildNotValid;    break;
    NOTEXIST:   status = Error::UtilBuildGITNotExist; break;
    NOTDIR:     status = Error::UtilBuildGITNotDir;   break;
    BAD:                                              break;
  }
  
  return status;
  
}

Status CommandLayer_BEFE_UUID(CommandLayer *layer, BcmdCommand &cmd) {

  Status status;
  UUID   uuid;
  
	status = uuid.NewRandom();
	if (status) goto SOMEERROR;
	
	Cout << uuid.ToString() << '\n';
	
  // Handle errors
	while (false) {
		SOMEERROR: break;
	}
	
  return status;
  
}

//----------------------------------------------------------------------
//
// Functions: CommandLayer_BEFE_Tests - Handle 'test[s]' command
//            CommandLayer_BEFE_OS    - Handle 'os' command
//            CommandLayer_BEFE_Lang  - Handle 'ang' command
//

Status CommandLayer_BEFE_Tests(CommandLayer *layer, BcmdCommand &cmd) {

  Status status;
  Int    argNo;
  Int    tests;
  Test   test;
  String message;
  UInt   numErrors;

  // Make sure there's a BEFE...
  if (BEFE::IsNull(TheBefe)) goto NOBEFE;

  // Make sure there's at least one test specified...
  if (cmd.numArgs < 2) {
    status = cmd.SetError(Error::CmdLang_ExpectedOneArg);
    if (status) goto SOMEERROR;
    goto OK;
  }

  // Pass 1: Validate each argument...
  numErrors = 0;
  for (argNo=1; argNo < (Int)cmd.numArgs; argNo++) {
    status = test.GetTestByName(cmd[argNo], tests);
    if (status && status != Error::UnknownTest) goto SOMEERROR;
    if (status == Error::UnknownTest) {
      numErrors++;
      status = Error::CmdBEFEUnknownTest;
      message = CmdStringMessage(status, cmd[argNo]);
      status = cmd.SetError(argNo, status, message);
      if (status) goto SOMEERROR;
    }

  }

  // If we've got errors, we're finished...
  if (numErrors) goto OK;

  // Pass 2: Do the tests...
  for (argNo=1; argNo < (Int)cmd.numArgs; argNo++) {
    status = test.GetTestByName(cmd[argNo], tests);
    if (status) goto SOMEERROR;
    status = test.Run(tests);
    if (status) {
      status = cmd.SetError(argNo, status);
      if (status) goto SOMEERROR;
    }
  }

  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None;   break;
    NOBEFE:    status = Error::NoBefe; break;
    SOMEERROR:                         break;
  }

  return status;

}

Status CommandLayer_BEFE_OS(CommandLayer *layer, BcmdCommand &cmd) {

  Status    status;

  status = Error::None;

  if (IsNull(layer)) goto BADPARAM;
	
  if (layer->shell)
    status = layer->shell->PushLayer("OS", cmd);
	
  // Handle errors
	while (false) {
    BADPARAM:  status = Error::InvalidParameter; break;
	}
	
  return status;

}

//----------------------------------------------------------------------
//
// Namespace Stats - Declarations for the Statistics Options
//

namespace Stats { // Namespace Stats...

enum { All, Memory, Stream, NumTerms };

// Get Term enum for given Argument

static char const *names[] = {"All", "Mem[ory]", "S[tr[eam]]", NULL};
static Strings     terms;
static UInts       termNos;

// Clear the MinExp strings
static void ClearTerms() {
  terms.Reset();
  termNos.Reset();
}

// Initialise the MinExp strings
static void InitTerms() {

  MinExp    var;
  char    **cpp;
  UInt      numStrings;
  UInt      stringNo;
  String    string;
  UInt      termNo;

  if (terms.Length() != 0) goto DONE;

  ClearTerms();
  termNo = 0;
  for (cpp=(char **)names; *cpp; cpp++) {
    var.Set(*cpp);
    numStrings = var.Count();
    for (stringNo=0; stringNo < numStrings; stringNo++) {
      string = var[stringNo];
      if (string.Length() == 0) break;
      terms.Append(string);
      termNos.Append(termNo);
    }
    termNo++;
  }

  while (false) {
    DONE: break;
  }
  
  return;
}

}  // ...Namespace Stats

//----------------------------------------------------------------------
//
// Functions: _DisplayMemoryStats - Display Memory statistics
//            _DisplayStreamStats - Display Stream statistics
//            CommandLayer_BEFE_MemoryStats - Handle 'Statistics memory' command
//            CommandLayer_BEFE_StreamStats - Handle 'Statistics stream' command
//            CommandLayer_BEFE_Statistics  - Handle 'Statistics <options>' command
//
//

static Status _DisplayMemoryStats() {

  Status  status;
  Strings info;

  status = Error::None;

  // Display the stats...
  if (TheBefe && gLogMemory) {
    TheBefe->TheMemoryStats.PrintMemoryStatistics();
    Cout << '\n';
    Help("MemoryStats");
  }
  else {
    BEFE_WARN("MemoryStats are not turned on");
    Help("UseOptMS");
  }

  return status;

}

static Status _DisplayStreamStats() {

  StreamStats stats;
  Strings     strings;
  // Display the FileStream stats
  Cout << "FileStream statistics...\n\n";
  strings = ToStrings(FileStream::ClassStats());
  DisplayStrings(strings,2);

  // Display the BufferStream stats
  Cout << '\n';
  Cout << "BufferStream statistics...\n\n";
  strings = ToStrings(BufferStream::ClassStats());
  DisplayStrings(strings,2);

  return Error::None;

}

Status CommandLayer_BEFE_MemoryStats(CommandLayer *layer, BcmdCommand &cmd) {

  Status  status;

  Cout << '\n';

  if (cmd.Length() != 1) goto NOARGSEXPECTED;

  // Display the stats...
  status = _DisplayMemoryStats();
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:      BEFE_ERROR("_DisplayMemoryStats",status);           break;
    NOARGSEXPECTED: BEFE_WARN("MemoryStats does expect any arguments"); break;
  }

  Cout << '\n';

  return status;

}

Status CommandLayer_BEFE_StreamStats(CommandLayer *layer, BcmdCommand &cmd) {

  Status  status;

  Cout << '\n';

  if (cmd.Length() != 1) goto NOARGSEXPECTED;

  // Display the stats...
  status = _DisplayStreamStats();
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:      BEFE_ERROR("Command_Statistics",status);            break;
    NOARGSEXPECTED: BEFE_WARN("StreamStats does expect any arguments"); break;
  }

  Cout << '\n';

  return status;

}

Status CommandLayer_BEFE_Statistics(CommandLayer *layer, BcmdCommand &cmd) {

  Status  status;           // Return status
  Int     optIdx;           // Command line option index
  String  opt;              // Current command line option
  Byte    optFlags[Stats::NumTerms]; // <-- "Do it" flag for each Stats enum
  UInt    numOptFlags;      // Number option flags set (in doOption)
  UInt    termNo;           // User selected Stats enum term number
  UInt    termIdx;          // Current Stats term we're looking at
  String  errorMsg;         // Dynamic error message we build on the fly
  Boolean haveSep;          // "Have we just output a line separator?"

  Stats::InitTerms();

  Cout << '\n';
  haveSep = true;

  // If no commands, internal error
  if (cmd.numArgs == 0) goto INTERNALERROR;

  // If only one command (the command), issue warning...
  else if (cmd.numArgs == 1) goto EXPECTEDOPTION;

  // Troll through remaining options to set up the option masks...
  else { // Options...

    // Clear the options flags
    numOptFlags = 0;        // No option flags set yet
    for (termIdx=0; termIdx < Stats::NumTerms; termIdx++)
      optFlags[termIdx] = -1;

    // Handle the specified options...
    for (optIdx=1; optIdx < (Int)cmd.numArgs; optIdx++) { // For each option...

      opt = cmd[optIdx];

      // Find the term
      for (termIdx=0; termIdx < Stats::terms.Length(); termIdx++) {
        if (Stats::terms.Get(termIdx).CompareInsensitive(opt) == 0) break;
      }
      if (termIdx >= Stats::terms.Length()) {
        errorMsg = "Unknown option '";
        errorMsg += opt;
        errorMsg += "' specified";
        goto ERRORMESSAGE;
      }

      // Turn on the flag
      termNo = Stats::termNos.Get(termIdx);
      optFlags[termNo] = termNo;
      numOptFlags++;

    } // ...For each option

    // If 'All' was specified, turn them all on...
    if (optFlags[0] >= 0) {
      for (termIdx=0;termIdx<Stats::NumTerms; termIdx++)
        optFlags[termIdx] = termIdx;
      numOptFlags = Stats::NumTerms;
    }

    // Do them...
    for (termIdx=1; termIdx<Stats::NumTerms; termIdx++) { // For each option specified...

      errorMsg = "Error processing ";

      switch (optFlags[termIdx]) { // Do this option...
        case Stats::Memory:
          if (!haveSep) {
            Cout << '\n';
            haveSep = true;
          }
          status = _DisplayMemoryStats();
          haveSep = false;
          if (status) goto SOMEERROR;
          break;
        case Stats::Stream:
          if (!haveSep) {
            Cout << '\n';
            haveSep = true;
          }
          status = _DisplayStreamStats();
          haveSep = false;
          if (status) goto SOMEERROR;
          break;
        default:
          status = Error::None;
          break;
      }  // ...Do this option

      // Handle error
      if (status) goto ERRORMESSAGE;

    }  // ...For each option specified

  } // ...Options

  // Handle errors
  status = Error::None;
  while (false) {
    INTERNALERROR:  status = Error::Internal;           goto SOMEERROR;
    EXPECTEDOPTION:
      if (!haveSep) Cout << '\n';
      BEFE_WARN("Expected command option");
      haveSep = false;
      break;
    ERRORMESSAGE:
      if (!haveSep) Cout << '\n';
      BEFE_WARN(errorMsg);
      haveSep = false;
      break;
    SOMEERROR:
      if (!haveSep) Cout << '\n';
      BEFE_ERROR("Command_Statistics",status);
      haveSep = false;
      break;
  }

  Stats::ClearTerms();

  if (!haveSep)
    Cout << '\n';

  return status;

}

//----------------------------------------------------------------------
//
// Functions: CommandLayer_BEFE_Echo - Handle 'Echo' command
//

Status CommandLayer_BEFE_Echo(CommandLayer *layer, BcmdCommand &cmd) {

  UInt argNo;
  UInt numArgs;

  numArgs = cmd.args.Length();
  for (argNo=1; argNo < numArgs; argNo++) {
  	String arg = cmd.args[argNo].value.Unescape();
    Cout << arg << '\n';
  }
  if (cmd.args.Length() == 0)
    Cout << '\n';

  return Error::None;

}

//----------------------------------------------------------------------
//
// Functions: CommandLayer_BEFE_ClearScreen - Handle 'ClearScreen' command
//

Status CommandLayer_BEFE_ClearScreen(CommandLayer *layer, BcmdCommand &cmd) {

  Status status;

  status = Cout.Clear();

  return status;

}

} // ...Namespace BEFE
