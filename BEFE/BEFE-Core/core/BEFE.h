//!befe-publish inc
//----------------------------------------------------------------------
// File: BEFE.h - Base C/C++ BEFE Declarations
//----------------------------------------------------------------------
//
// This header file is pretty much the only one needed by any C/C++
// code.
//
// The most important Class is the Befe class.  It manages the entire
// BEFE <RunTime> and <DeclTime> environments.
//
// *** See technical documentation for a more detailed description ***
//
// Having said that...
//
// Everything coalesces here and all you should have to do is to include
// it and get on with it.
//
// The good part about this is it's easy to use and, once you get your
// head around the acronym BEFE, it's easy to remember.
//
// The bad part about this is if you make a change here, or anything
// it includes (which is EVERYTHING), you need to recompile EVERYTHING.
//
// Sort of sucks, eh?  Oh well...  Until we get our own development
// framework in place (it's in the works) you just have to suffer with
// the rest of us  because that's the bad part about programming in
// C/C++ anyway!
//
// Have fun!!!!!!
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BEFE_H  // BEFE_H...
#define BEFE_H

#include "Primitives.h"     // Primitive Data Types
#include "MemoryUtils.h"    // Memory allocation, free, etc.
#include "Error.h"          // BEFE Error Status declarations
#include "Null.h"           // Null related declarations
#include "Globals.h"        // BEFE "Globals"
#include "LifeCycle.h"      // BEFE Primitive/Value Lifecycle
#include "UUID.h"           // UUIDs

// Really basic value types
#include "Coord.h"          // X/Y Coordinate Class
#include "Time.h"           // Time Class
#include "Storage.h"        // General Storage related declarations
#include "Array.h"          // Arrays and related templates
#include "Sparse.h"         // Sparse Arrays and related templates
#include "Map.h"            // Maps and related Templates
#include "StringGeneral.h"  // String Functions
#include "ToString.h"       // String formatting
#include "Arrays.h"         // Array/Sparse/Map speciailisations
#include "Crypt.h"          // Cryptography

// Integers and Numbers...
#include "IntFuncs.h"       // Various Int, UInt, etc functions
#include "Integer.h"        // General Integer Class (***Unfinished**)
#include "Number.h"         // General Number Class (***Unfinished**)

// Sequence specifiers...
#include "Sequence.h"       // General Sequence related declarations
#include "Range.h"          // Range Class
#include "URange.h"         // URange Class
#include "Span.h"           // Span Class
#include "USpan.h"          // USpan Class
#include "Slice.h"          // Slice Class
#include "Alloc.h"          // Alloc Class
#include "Cycle.h"          // Cycle Class (may become deprecated, haven't decided yet)

// Specialisations of Bytes...
#include "Bytes.h"          // Array of Bytes
#include "Strings.h"        // Array of Strings
#include "Numbers.h"        // Array of Numbers
#include "Stream.h"         // Stream Class

// Other important stuff
#include "Console.h"        // BEFE Console Class
#include "MemoryStats.h"    // MemoryStats Class
#include "OperatingSystem.h"// OperatingSystem Class
#include "Platform.h"       // Platform Class
#include "Process.h"        // Process Class
#include "Processor.h"      // Processor Class
#include "Network.h"        // Network Class

// Error Messages
#include "Message.h"        // General Message Class and related declarations

// I/O
#include "File.h"           // File Class
#include "Lines.h"          // Lines Class (for text I/O)
#include "Cache.h"          // File Block Cache

// Internal Debugging
#include "_Debug.h"         // Debugging declarations

//
// Just in case...
//

#ifndef NULL
  #define NULL (void *)0
#endif

//----------------------------------------------------------------------
//
// Port specific calls, implemented in port specific code
//

namespace BEFE {

//
// Our substitutions for standard C/C++ calls
//
// The bad part of this is, for now, we have to supply wrapper
// functions for these.
//
// The good part of this is...
//
//   a) at some time in the future we can write them properly
//      in assembler across all ports
//
//   b) we can put debug breakpoints in them and also gather
//      stats on how many times each one is called.
//

Byte *Malloc(UInt size);
void  Free(Byte *themem);
Byte *Memmove(Byte *dst, Byte *src, Int len);
Byte *Memchr(Byte *buf, Byte byte, Int len);

UInt  Strlen(const void *s1);
Int   Strcmp(const void *s1, const void *s2);
Int   Strcmpi(const void *s1, const void *s2);
Int   Strncmp(const void *s1, const void *s2, UInt size);
void  Strcpy(void *s1, const void *s2);
void  Strcat(void *s1, const void *s2);
void *Strchr(const void *s1,UInt character);
void *Strstr(void *s1, const void *s2);

void Itoa(Int num,Byte *buf,Int base);
Int  Atoi(Byte *buf);
Int  Atoi(Byte *buf, UInt bufLen);

void Srand(Int seed);
Int  Rand();

//void Qsort (void *base, size_t num, size_t size, int (*comparator ) (const void *, const void *) );

// Convert error code to language translated string
//   Implemented in Error.cpp

String ErrorString(Status error);

//----------------------------------------------------------------------
//
// Class: BEFE - The BEFE "Master Class"
//
// The BEFE Version, SubVersion, and Revision...
//
// Note: Change add or remove some of the dots below to get GIT to
//       update the revision...
//
//       ......
//
#define BEFE_VERSION    0
#define BEFE_SUBVERSION 4
#define BEFE_REVISION   "$Revision: 3864 $"

// External BEFE "Globals"
extern Console Cout;
extern Console Cin;
extern Befe    *TheBefe;

// The BEFE Class...
class Befe { // Class Befe...

  // C++ Lifecycle
  //   Implemented in BEFE.cpp
  public: Befe();
  public: Befe(String &message);
  public: Befe(const char *message);
  public: ~Befe();

  // BEFE Lifecycle
  //   Implemented in BEFE.cpp
  public: Status StartUp();
  public: Status StartUp(String const &message);
  public: Status StartUp(char const *message);
  public: Status ShutDown();
  // Display Header/Trailer
  //   Implemented in BEFE.cpp
  public: Status Header();
  public: Status Trailer();

  // Process control
  //   Implemented in BEFE.cpp
  public: Status Exit(Status status);
  public: Status Abort();
  public: Status Abort(String const &message);
  public: Status Abort(char const *message);
  public: Status Warn(String const &message);
  public: Status Warn(char const *message);
  public: Status Congrats(String const &message);
  public: Status Congrats(char const *message);
  public: Status ErrorStatus(char const *func, Status status);
  public: Status ErrorStatus(String const &str, Status status);
  public: Status ErrorString(char const *func);
  public: Status ErrorString(String const &str);
  public: Status Debug(String const &message);
  public: Status Debug(char const *message);
  public: Status Deprecated(char const *message);

  // Environment Variable Management
  //   Implemented in BEFEEnviron.cpp
  public: Status              RefreshEnvironment();
  public: Status              ClearEnvironment();
  public: Boolean             IsEnvironmentName(String const &name);
  public: Strings             GetEnvironmentNames();
  public: NamedStrings const *GetEnvironmentValues();
  public: Status              GetEnvironmentValues(NamedStrings &values);
  public: String              GetEnvironmentValue(String const &name);
  public: Status              GetEnvironmentNames(Strings &names);
  public: Status              SetEnvironmentValue(String const &name, String const &value);
  public: Status              RemoveEnvironmentName(String const &name);
  public: String              SubstituteEnvironmentValues(String const &inStr);
  public: Status              SubstituteEnvironmentValues(String const &inStr, String &outStr);

  // Application Identificiation
  public: Status  SetApplicationName(String const &name);
  public: Status  SetApplicationDescription(String const &desc);
  public: Status  SetApplicationVersion(UInt version);
  public: Status  SetApplicationSubVersion(UInt subVersion);
  public: Status  SetApplicationRevision(UInt revision);
  public: String  GetApplicationName();
  public: String  GetApplicationDescription();
  public: UInt    GetApplicationVersion();
  public: UInt    GetApplicationSubVersion();
  public: UInt    GetApplicationRevision();
	
  // Error Message Management
  //   Implemented in BEFE.cpp
  public: Boolean ErrorMessagesAreLoaded() const;
  public: Status  LoadErrorMessages();
  public: Status  UnloadErrorMessages();
  public: String  ErrorMessage(Status status);
  public: String  GetMessage(Status status);
  
  // Cryptography Methods
  //   Implemented in BEFE.cpp
  public: CryptKey GetCryptKey();
  public: Status   GetCryptKey(CryptKey &key);
  
  // Handy File utility Methods
  //   Implemented in BEFE.cpp
  public: Status _ValidateFileName(String &fName, Boolean exists); // ◄── Validate and adjust existing/new File to Full Path
  public: Status ValidateNewFileName(String &fName);      // ◄── Validate and adjust non-existent File to Full Path
  public: Status ValidateExistingFileName(String &fName); // ◄── Validate and adjust existing File to Full Path
  public: Status FindConfigurationFile(String &fName);    // ◄── Find BEFE Configuration File and update to Full Path
  
  // File Block Cache Methods
  
  // CallBack registry...
  //    Implemented in BEFECallback.cpp
  public: Status RegisterCallback(String const &name, void *callBack);
  public: Status RemoveCallback(String const &name);
  public: Status FindCallback(String const &name, void **callBack);
	
  // Protected Methods
  //   Implemented in BEFEEviron.cpp
  protected: Status LoadEnvValues() const;
  
  //
  // Public Members
  //
  
  public: Console          TheConsole;
  public: Globals          TheGlobals;
  public: MemoryStats      TheMemoryStats;
  public: OperatingSystem *TheOS;
  public: Platform        *ThePlatform;
  public: Process         *TheProcess;
  public: Processor       *TheProcessor;
  public: Network         *TheNetwork;
  public: IdSpace         *TheIdSpace;
  public: Cache            TheCache;
  
  //
  // Protected Members
  //

  // Application info
  protected: String        appName;
  protected: String        appDescription;
  protected: UInt          appVersion;
  protected: UInt          appSubVersion;
  protected: UInt          appRevision;
	
  // Environment Variables
  protected: Boolean       haveEnvValues;
  protected: NamedStrings  envValues;  
  
  // Error Message Management
  protected: Boolean       errLoading;
  protected: MessageRanges errRngArray;
  protected: Messages      errMsgArray;
  protected: MessageIds    errIdIndexes;
  protected: MessageNames  errNameIndexes;
    
  // Cryptography
  protected: CryptKey      cryptKey;
  
  // Callbacks...
  protected: NamedInts     callbacks;
	
}; // ...Class Befe

} // ...Namespace BEFE

//
// Other stuff for everyone...
//

//#include "BEFEWrappers.h"

#endif // ...BEFE_H
