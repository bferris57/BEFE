//!befe-publish inc
//----------------------------------------------------------------------
// File: Globals.h - Base C/C++ "Global" Declarations
//----------------------------------------------------------------------
//
// This header file declares the "Globals" for BEFE.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef GLOBALS_H // GLOBALS_H...
#define GLOBALS_H

#include "Primitives.h"
#include "Platform.h"
#include "OperatingSystem.h"
#include "Console.h"
#include "Network.h"
#include "Process.h"
#include "Processor.h"
#include "IdSpace.h"

namespace BEFE {

//----------------------------------------------------------------------
//
// "Global" String and character Constants
//

extern const char *CONSTR_True         ; // "True"
extern const char *CONSTR_False        ; // "False"
extern const char *CONSTR_Yes          ; // "Yes"
extern const char *CONSTR_No           ; // "No"
extern const char *CONSTR_On           ; // "On"
extern const char *CONSTR_Off          ; // "Off"

extern const char *CONSTR_NaN          ; // "NaN"
extern const char *CONSTR_Null         ; // "Null"
extern const char *CONSTR_null         ; // "null"
extern const char *CONSTR_StarNull     ; // "**Null**"
extern const char *CONSTR_None         ; // "None"
extern const char *CONSTR_QQQ          ; // "???"
extern const char *CONSTR_StarNone     ; // "**None**"
extern const char *CONSTR_StarEmpty    ; // "**Empty**"
extern const char *CONSTR_NotAvailable ; // "Not Available"
extern const char *CONSTR_Unavailable  ; // "**Unavailable**"
extern const char *CONSTR_Deprecated   ; // "**Deprecated, do not use**"

extern const char *CONSTR_Generic      ; // "Generic"
extern const char *CONSTR_Unknown      ; // "Unknown"
extern const char *CONSTR_StarUnknown  ; // "**Unknown**"
extern const char *CONSTR_NotKnown     ; // "Not Known"

extern const char *CONSTR_Error        ; // "Error: "
extern const char *CONSTR_Warning      ; // "Warning: "
extern const char *CONSTR_Syntax       ; // "Syntax Error: "

extern const char *CONSTR_Closed       ; // "Closed"
extern const char *CONSTR_Open         ; // "Open"

extern const char *CONSTR_Bytes        ; // "Bytes"
extern const char *CONSTR_KiloBytes    ; // "KB"
extern const char *CONSTR_MegaBytes    ; // "MB"
extern const char *CONSTR_GigaBytes    ; // "GB"
extern const char *CONSTR_TeraBytes    ; // "TB"

extern const char *CONSTR_SIGINT       ; // "!!! User Requested Termination !!!"

extern const char NUL;
extern const char BEL;
extern const char BS;
extern const char HT;
extern const char LF;
extern const char Endl;
extern const char CR;
extern const char ESC;
extern const char SP;

//----------------------------------------------------------------------
//
// Implementation of "Globals"
//

struct Globals { // Struct Globals...

  //
  // Long values
  //
  
  union {
    Long array[1];
    Long StartTime;
  } longs;
  
  //
  // UInt values
  //
  
  union {
    UInt array[1];
    struct {
      UInt    DefaultBufSize;
      UInt    UnicodeMaxFullFoldChars;
      UInt    UnicodeCurFullFoldChars;
      UInt    ConsoleHistory;   // Number of history lines to keep (should be Null or > 1)
    };
  } uints;
  
  //
  // Boolean values
  //
  
  union {
    
    Boolean array[1];
    
    struct {

      // Flow Control
      Boolean FlowQuitNow;
      
      // Verbosity
      Boolean Quiet;
      Boolean Verbose;
      Boolean VeryVerbose;
      Boolean VeryVeryVerbose;
      Boolean ShowElapsed;
      Boolean ShowTimeStamps;
      Boolean ShowLocalTime;
      Boolean ShowDeprecated;
      Boolean ShowWarnings;
      Boolean ShowInfo;
      Boolean ShowDebug;
      Boolean ShowHeaders;

      // Files
      Boolean KeepTemporaryFiles;
      Boolean ShowHiddenFiles;
      Boolean ShowHiddenDirectories;

      // Unicode Behaviour
      Boolean UnicodeFolding;
      Boolean UnicodeTurkish;
  
      // Memory usage
      Boolean LogMemory;
      Boolean LogMemoryAlloc;
      Boolean LogMemoryAllocSizes;
      Boolean LogMemoryFree;
      Boolean LogMemoryFill;
      Boolean LogMemoryCopy;
      Boolean ShowMemoryStats;
      Boolean MemoryValidate;
      Boolean MemoryValidateAbort;
  
      // Command processors...
      Boolean CommandIndent;
      Boolean CommandEnvSubst;
  
      // Console extensions
      Boolean ConsoleColour;      // Whether or not to display Console Colour
      Boolean ConsoleIntensity;   // Whether or not to display Console Intensity
      Boolean ConsoleUTF8;        // Display UTF-8 on console, otherwise we try hokey line drawing chars
      Boolean ConsoleBeep;        // Console beep enabled
      Boolean ConsoleInteractive; // Force Console display of prompt

      // Others
      Boolean PythonOutput;       // Form Python Friendly output
    };

  } bools;
  
  //
  // Methods
  //

  // C++ Lifecycle
  Globals();
  Globals(Globals const &that);
  ~Globals();
  Globals &operator=(Globals const &that);

  // Befe Lifecycle
  Status StartUp();
  Status CopyFrom(Globals const &that);
  Status ShutDown();

  // Formatting
  Strings ToStrings() const;
  Status  ToStrings(String const &option, Strings &strings) const;
  
}; // ...Struct Globals

// External classes
class Befe;

// The<xxx> Globals...
extern Befe            *TheBefe;

} // ...Namespace BEFE

#endif // ...GLOBALS_H
