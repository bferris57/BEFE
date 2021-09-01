//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: GlobalMacros.h - Base C/C++ "Global" preprocessor macro Declarations
//----------------------------------------------------------------------
//
// This header file declares easy to remember preprocessor macros for
// all the globals in Globals.h
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef GLOBALMACROS_H // GLOBALMACROS_H...
#define GLOBALMACROS_H

// Non-configurable settings
#define gStartTime               TheBefe->TheGlobals.longs.StartTime
#define gDefaultBufSize          TheBefe->TheGlobals.uints.DefaultBufSize
#define gUnicodeMaxFullFoldChars TheBefe->TheGlobals.uints.UnicodeMaxFullFoldChars
#define gUnicodecurFullFoldChars TheBefe->TheGlobals.uints.UnicodeConfigFullFoldChars
#define gConsoleHistory          TheBefe->TheGlobals.uints.ConsoleHistory

// Flow Control
#define gFlowQuitNow           TheBefe->TheGlobals.bools.FlowQuitNow

// Verbosity
#define gQuiet                 TheBefe->TheGlobals.bools.Quiet
#define gVerbose               TheBefe->TheGlobals.bools.Verbose
#define gVeryVerbose           TheBefe->TheGlobals.bools.VeryVerbose
#define gVeryVeryVerbose       TheBefe->TheGlobals.bools.VeryVeryVerbose
#define gShowElapsed           TheBefe->TheGlobals.bools.ShowElapsed
#define gShowTimeStamps        TheBefe->TheGlobals.bools.ShowTimeStamps
#define gShowLocalTime         TheBefe->TheGlobals.bools.ShowLocalTime
#define gShowDeprecated        TheBefe->TheGlobals.bools.ShowDeprecated
#define gShowWarnings          TheBefe->TheGlobals.bools.ShowWarnings
#define gShowInfo              TheBefe->TheGlobals.bools.ShowInfo
#define gShowDebug             TheBefe->TheGlobals.bools.ShowDebug
#define gShowHeaders           TheBefe->TheGlobals.bools.ShowHeaders

// Files
#define gKeepTemporaryFiles    TheBefe->TheGlobals.bools.KeepTemporaryFiles
#define gShowHiddenFiles       TheBefe->TheGlobals.bools.ShowHiddenFiles
#define gShowHiddenDirectories TheBefe->TheGlobals.bools.ShowHiddenDirectories

// Unicode Behaviour
#define gUnicodeFolding        TheBefe->TheGlobals.bools.UnicodeFolding
#define gUnicodeTurkish        TheBefe->TheGlobals.bools.UnicodeTurkish
  
// Memory usage
#define gLogMemory             TheBefe->TheGlobals.bools.LogMemory
#define gLogMemoryAlloc        TheBefe->TheGlobals.bools.LogMemoryAlloc
#define gLogMemoryAllocSizes   TheBefe->TheGlobals.bools.LogMemoryAllocSizes
#define gLogMemoryFree         TheBefe->TheGlobals.bools.LogMemoryFree
#define gLogMemoryFill         TheBefe->TheGlobals.bools.LogMemoryFill
#define gLogMemoryCopy         TheBefe->TheGlobals.bools.LogMemoryCopy
#define gShowMemoryStats       TheBefe->TheGlobals.bools.ShowMemoryStats
#define gMemoryValidate        TheBefe->TheGlobals.bools.MemoryValidate
#define gMemoryValidateAbort   TheBefe->TheGlobals.bools.MemoryValidateAbort

// Sequence behaviour
#define gAllocDefaultOpen      TheBefe->TheGlobals.bools.AllocDefaultOpen

// Command processors...
#define gCommandEcho           TheBefe->TheGlobals.bools.CommandEcho
#define gCommandIndent         TheBefe->TheGlobals.bools.CommandIndent
#define gCommandScripts        TheBefe->TheGlobals.bools.CommandScripts
#define gCommandTests          TheBefe->TheGlobals.bools.CommandTests
#define gCommandEnvSubst       TheBefe->TheGlobals.bools.CommandEnvSubst

// Console extensions
#define gConsoleColour         TheBefe->TheGlobals.bools.ConsoleColour
#define gConsoleIntensity      TheBefe->TheGlobals.bools.ConsoleIntensity
#define gConsoleUTF8           TheBefe->TheGlobals.bools.ConsoleUTF8
#define gConsoleTerminal       TheBefe->TheGlobals.bools.ConsoleTerminal
#define gConsoleBeep           TheBefe->TheGlobals.bools.ConsoleBeep
#define gConsoleInteractive    TheBefe->TheGlobals.bools.ConsoleInteractive

// Others
#define gPythonOutput          TheBefe->TheGlobals.bools.PythonOutput

#endif // ...GLOBALMACROS_H
