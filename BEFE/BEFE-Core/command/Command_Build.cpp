//!befe-publish src
//----------------------------------------------------------------------
// File: Command_OSBuild.cpp - Command_OS Build functions
//----------------------------------------------------------------------
//
// This file implements the Command_OS Build command.
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
#include "Build.h"
#include "MinExp.h"
#include "CommandDecl.h"

namespace BEFE { // Namespace BEFE...

//
// Build Options...
//

struct sBuildOpts {
  char const *name;
  UInt        mask;
};

static struct sBuildOpts options[] = {
  
  // General...
  {"A[ll]",                        Build::CleanAll                         },
  {"F[ull]",                       Build::CleanAll                         },
  {"Init[ial]",                    Build::GeneralCreateOutputDirectories |
                                   Build::GeneralValidateSettings        |
                                   Build::GeneralCleanOutputDirectories    },
  {"Cl[ea]n",                      Build::GeneralCleanOutputDirectories    },
  {"Cl[ea]nA[ll]",                 Build::CleanAll                         },
  {"A[ll]Rel[ease]",               Build::CleanRelease                     },
  {"A[ll]Deb[ug]",                 Build::CleanDebug                       },
  {"Def[ault]",                    Build::GeneralDefaultSettings           },
  {"Val[idate]",                   Build::GeneralValidateSettings          },
  {"None|Nothing",                 Build::None                             },
  
  // Options...
  {"Rel[ease]",                    Build::OptionRelease | Build::BuildCopyInput },
  {"Deb[ug]",                      Build::OptionDebug   | Build::BuildCopyInput },
  {"Both",                         Build::OptionRelease | Build::OptionDebug    },
  {"GIT",                          Build::OptionGIT                             },
  
  // Build...
  {"CopyIn[put]",                  Build::BuildCopyInput | Build::GeneralDetermineFileNames},
  {"Comp[ile]",                    Build::BuildCompile         },
  {"Lib[rary]",                    Build::BuildLibraries       },
  {"Lib[raries]",                  Build::BuildLibraries       },
  {"Exe[cutable[s]]",              Build::BuildExecutables     },
  
  // Publish...
  {"P[ub[lish]][All]",             Build::PublishAll           },
  {"P[ub[lish]]H[ead[er[s]]]",     Build::PublishHeaders       },
  {"P[ub[lish]]S[ou]rc[e[s]]",     Build::PublishSource        },
  {"P[ub[lish]]L[ib[rar[y|ies]]",  Build::PublishLibraries     },
  {"P[ub[lish]]Exe[cutable[s]]",   Build::PublishExecutables   },
  {"P[ub[lish]]Doc[ument[ation]]", Build::PublishDocumentation },
  {"P[ub[lish]]Home",              Build::PublishHomeFiles     },
  
  // Zip...
  {"Zip",                          Build::ZipAll | Build::GeneralDefaultSettings | Build::GeneralValidateSettings},

  {0,0}
};

// Local Typedefs
typedef StringMapPrimitive<UInt> BuildOptions;

//----------------------------------------------------------------------
//
// Local Function: GetValidOptions - Build map of valid options
//

static Status GetValidOptions(BuildOptions &buildOpts) {
  
  Status             status;
  struct sBuildOpts *sBuildOpts;  
  BuildOptions       localOpts;

  MinExp             var;
  Int                numStrings;
  String             curString;
  Int                curIdx;
  
  status = localOpts.SetInsensitive();
  if (status) goto SOMEERROR;
  
  for (sBuildOpts=options; !IsNull(sBuildOpts->name); sBuildOpts++) { // For Each Configured Option...
    
    // Generate the MinExp strings...
    status = var.Set(sBuildOpts->name);
    if (status) goto SOMEERROR;
    
    // Populate each MinExp string...
    numStrings = var.Count();
    for (curIdx=0; curIdx < numStrings; curIdx++) {
    	curString = var[curIdx];
      if (!localOpts.HasKey(curString)) {
        status = localOpts.Set(curString.Consumable(), sBuildOpts->mask);
        if (status) goto SOMEERROR;
      }
    }
    
  } // ...For Each Configured Option
  
  // Everything's ok, give the result to the caller...
  status = buildOpts.MoveFrom(localOpts);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Function: Command_Build - Build Command
//

Status Command_Build(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;            // Return Status
  UInt             argNo;             // Curernt argument index

  String           arg;               // Current argument
  UInt             argLen;            // Current argument length (in Chars)
  UInt             argCurPos;         // Current position in argument
  UInt             argPlusPos;        // Where next '+' is
  UInt             argMinusPos;       // Where next  is
  UInt             argNextPos;        // Next position (EOS, '+', or '-')
  String           bldOpt;            // Current <bldOpt> Name
  Span             bldOptSpan;        // Span of current <bldOpt> in arg
  Char             bldOptFlag;        // Current <bldOpt> flag ('+' or '-')
  
  Build            build;             // The Build we're doing
  BuildOptions     bldOpts;           // Valid <bldOpt> setting names/masks
  UInts            masks;             // ◄── We build one UInt mask for each argument
  UInt             mask;              // Current Mask we're building (one per argument)
  UInt             tMask;             // Transient Mask (used to build mask)
  
   // Make sure we've got an OS...
  if (!TheBefe->TheOS) goto NOOS;

  // Get the valid <bldOpt> settings...
  status = GetValidOptions(bldOpts);
  if (status) goto SOMEERROR;
  
  // Parse each argument...
  for (argNo=1; argNo < cmd.numArgs; argNo++) { // For each argument...

    arg = cmd[argNo];
    argLen = arg.Length();
    mask = 0;

    bldOptSpan = Span(0,argLen);
    bldOptFlag = '+';
    argCurPos = 0;

    while (argCurPos < arg.Length()) { // Until finished with arg...

      // Find where this <bldOpt> ends (next '+', '-', or EOS)...
      argPlusPos  = arg.Find('+',Span(argCurPos, argLen));
      argMinusPos = arg.Find('-', Span(argCurPos, argLen));
      argNextPos = argLen;
      if (!IsNull(argPlusPos))
        argNextPos = Min(argNextPos, argPlusPos);
      if (!IsNull(argMinusPos))
        argNextPos = Min(argNextPos, argMinusPos);

      bldOpt = arg.Get(Span(argCurPos, argNextPos));
      
      // DEBUG...
      if (gShowDebug) {
      	Cout << "Command_Build: bldOpt = ►" << bldOpt << "◄\n";
      	Cout.Flush();
      }
      // ...DEBUG

      // Make sure it's a valid <bldOpt> name...
      if (!bldOpts.HasKey(bldOpt)) goto UNKOPT;
      
      // Adjust the mask
      tMask = bldOpts.Get(bldOpt);
      if (bldOptFlag == '+') {
      	if (tMask)
          mask |= tMask; // ◄── Does this if some bits set
      	else
      		mask = 0;      // ◄── Does this if NO bits set (i.e. "[+]None" specified)
      }
      else {
      	if (tMask)
          mask &= ~tMask;
      }
        
      // Position to next <bldOpt>
      argCurPos = argNextPos;
      
      // If '+' or '-', save the flag
      bldOptFlag = Char();
      if (argCurPos < argLen) {
        if (arg.Get(argCurPos) == '+')
          bldOptFlag = '+';
        else
          bldOptFlag = '-';
        argCurPos++;
      }
      
    } // ...Until finished with arg
      
    // If publishing anything, throw in Publish Dependencies
    if (mask & Build::PublishAll)
      mask |= Build::PublishDeps;
      
    // Now DO IT...
    status = build.Options(mask);
    if (status) goto SOMEERROR;
    status = build.Steps(mask | Build::GeneralDefaultSettings);
    if (status) goto SOMEERROR;
  
  } // ...For each argument
  
  //
  // Handle errors
  //

  while (false) {
    NOOS:       status = Error::InternalNoTheOS;        break;
    SOMEERROR:                                          break;
    // Formatted errors...
    UNKOPT:     status = Error::UtilBuildUnknownOption; goto FORMATIT;
    FORMATIT:   { String       message;
                  NamedStrings errVals;
                  UInt         fg;
                  message = String(CONSTR_Error) + ErrorMessage(Error::UtilBuildBadArgument) + '\n' +
                            String(CONSTR_Error) + ErrorMessage(status);
                  errVals.Set("arg",arg);
                  errVals.Set("option",bldOpt);
                  Substitute(message, errVals, '<', '>', message);
                  fg = Cout.SetForegroundColour(Console::Red);
                  Cout << message << '\n';
                  Cout.SetForegroundColour(fg);
                  status = Error::Failed;
                  break;
                 }
  }
  
  if (!status)
    Cout << '\n';

  return status;
  
}

} // ...Namespace BEFE
