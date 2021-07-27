//----------------------------------------------------------------------
// File: BEFE.cpp - Class Befe implementation
//----------------------------------------------------------------------
//
// The most important Class is the BEFE class.  It manages the entire
// BEFE <RunTime> and <DeclTime> environments.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "FormatOut.h"
#include "MemoryStats.h"
#include "_SanityCheck.h"

namespace BEFE { // Namespace BEFE...

//
// External function prototypes
//
// Note: These are implemented in platform specific code
//

extern void StartUp();
extern void ShutDown();
extern void Exit(Status status);

//----------------------------------------------------------------------
//
// Befe "Globals"...
//

       Befe    *TheBefe = NULL;
       Console  Cout;
       Console  Cin;
static Int     gHeaderCount = 0;

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

// C++ Lifecycle
Befe::Befe() {
  StartUp();
}

Befe::Befe(String &message) {
  StartUp(message);
}

Befe::Befe(const char *message) {
  StartUp(message);
}

Befe::~Befe() {
  ShutDown();
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Befe::StartUp() {

  String varname;
  String varpath;

  // Save where we are
  if (!TheBefe)
    TheBefe = this;

  // Start the Globals stuff first...
  TheGlobals.StartUp();

  // Clear our pointers
  TheConsole.StartUp();
  Cout.deferTo = &TheConsole;
  Cin.deferTo  = &TheConsole;
  TheMemoryStats.StartUp();
  TheOS        = NULL;
  ThePlatform  = NULL;
  TheProcess   = NULL;
  TheProcessor = NULL;
  TheNetwork   = NULL;
  TheIdSpace   = NULL;
  
	// Start the Application info...
	appName.StartUp();
	appDescription.StartUp();
	SetNull(appVersion);
	SetNull(appSubVersion);
	SetNull(appRevision);
	
  // Start the Error Message info...
  errLoading = false;
  errRngArray.StartUp();
  errMsgArray.StartUp();
  errIdIndexes.StartUp();
  errNameIndexes.StartUp();
  
  // Start Cryptography...
  cryptKey.StartUp();
  
	// Start callbacks...
	callbacks.StartUp();
	
  // Start All platform specific stuff...
  BEFE::StartUp();

  if (IsNull(TheOS        ) ||
      IsNull(ThePlatform  ) ||
      IsNull(TheProcess   ) ||
      IsNull(TheProcessor ) )
    BEFE_ABORT("Platform/OS/Process failed to initialise");
  
  //
  // Populate Globals from things we now know...
  //
  
  TheGlobals.uints.DefaultBufSize = TheOS->GetMemoryPageSize();
  
  //
  // Initialise all other classes and set up our instance variables
  //

  //IdSpace::ClassStartUp();

  haveEnvValues = false;
  
  // And finally, start the Cache...
  TheCache.StartUp();
  
  //
  // Now, do final "Sanity Checks"
  //

  _SanityCheck();

  return Error::None;

}

Status Befe::StartUp(String const &message) {
  BEFE_WARN("Befe.StartUp(String &) not implemented yet");
  return Error::None;
}

Status Befe::StartUp(char const *message) {
  BEFE_WARN("Befe.StartUp(const char *) not implemented yet");
  return Error::None;
}

Status Befe::ShutDown() {
  
  TheCache.ShutDown();
  UnloadErrorMessages();  
  envValues.ShutDown();
  cryptKey.ShutDown();
  callbacks.ShutDown();
	
  IdSpace::ClassShutDown();
  BEFE::ShutDown();
  
  // Show the memory statistics if we're supposed to
  if (TheBefe && gShowMemoryStats) {
    TheBefe->TheMemoryStats.PrintMemoryStatistics();
    gShowMemoryStats = false;
  }
 
  // Decided we shouldn't take control of the entire user's process
  // here so we commented this out...
  //if (TheBefe == this) {
  //  TheBefe = NULL;
  //  BEFE::Exit(Error::None);
  //}
    
  Cout.deferTo = NULL;
  Cin.deferTo  = NULL;
  TheConsole.ShutDown();
  
  return Error::None;
}

//----------------------------------------------------------------------
//
// Header/Trailer
//

Status Befe::Header() {

  FormatOut fmt;
  Strings   fmtarr;
  String    fmtline;
  UInt      i;

  if (!gQuiet && !gHeaderCount) {

    fmt.FormatHeader(fmtarr);
    for (i=0; i<fmtarr.Length(); i++) {
      fmtarr.Get(i,fmtline);
      Cout << fmtline << Endl;
    }
    Cout << Endl;
    fmtarr.SetEmpty();
    gHeaderCount++;
  }

  return Error::None;
}

Status Befe::Trailer() {

  FormatOut fmt;
  Strings   fmtarr;
  String    fmtline;
  UInt      i;

  if (!gQuiet && gHeaderCount) {

    fmt.FormatFooter(fmtarr);
    Cout << '\n';
    for (i=0; i<fmtarr.Length(); i++) {
      fmtarr.Get(i,fmtline);
      Cout << fmtline << Endl;
    }
    fmtarr.SetEmpty();

    gHeaderCount--;

  }

  return Error::None;

}

//----------------------------------------------------------------------
//
// Process control
//

Status Befe::Exit(Status status) {

  // Reset stuff that may have allocating memory...
  if (TheOS)
    TheOS->Reset();
  if (TheNetwork)
    TheNetwork->Reset();
  if (ThePlatform)
    ThePlatform->Reset();
  if (TheProcess)
    TheProcess->Reset();
  if (TheProcessor)
    TheProcessor->Reset();
    
  // Unload the Error Message info
  UnloadErrorMessages();
  
  // Show the memory statistics if we're supposed to
  if (gShowMemoryStats) {
    Cout << '\n';
    TheMemoryStats.PrintMemoryStatistics();
    gShowMemoryStats = false;
  }
 
  BEFE::Exit(status);
  
  return status;
  
}

Status Befe::Abort(String const &message) {

  Byte buf[128];
  UInt bufl;

  ((String *)&message)->ToBytes(buf,(Int)sizeof(buf),bufl);
  Cout << "*** ABORTING: ";
  Cout << (char const *)buf;
  Cout << '\n';

  Exit(1);
  
  return Error::Failed;
  
}

Status  Befe::Abort(char const *message) {

  UInt oldcolour;

  oldcolour = Cout.SetForegroundColour(Console::Red);
  Cout << "*** ABORTING: ";
  if (IsNull(message))
    Cout << "No reason given\n";
  else
    Cout << message;
  Cout.SetForegroundColour(oldcolour);
  Cout << '\n';

  Exit(1);
  
  return Error::Failed;
  
}

Status Befe::Abort() {
  return Abort((char const *)NULL);
}

Status Befe::Warn(String const &message) {

  Int oldcolour;

  oldcolour = Cout.SetForegroundColour(Console::Yellow);
  if (Cout.GetCursor().x != 0)
    Cout << '\n';
  Cout << "!!! WARNING: " << message << '\n';
  Cout.SetForegroundColour(oldcolour);

  return Error::None;
  
}

Status Befe::Warn(char const *message) {

  Int oldcolour;

  oldcolour = Cout.SetForegroundColour(Console::Yellow);
  if (Cout.GetCursor().x != 0)
    Cout << '\n';
  if (IsNull(message))
    Cout << "!!! WARNING: No reason given\n";
  else
    Cout << "!!! WARNING: " << message << '\n';
  Cout.SetForegroundColour(oldcolour);

  return Error::None;
  
}

Status Befe::Congrats(String const &message) {

  Int oldcolour;

  if (gShowWarnings) {
    oldcolour = Cout.SetForegroundColour(Console::Yellow);
    if (Cout.GetCursor().x != 0)
      Cout << '\n';
    Cout << "!!! HURRAH !!! " << message << '\n';
    Cout.SetForegroundColour(oldcolour);
  }

  return Error::None;
  
}

Status Befe::Congrats(char const *message) {

  Int oldcolour;

  if (gShowWarnings) {
    oldcolour = Cout.SetForegroundColour(Console::Yellow);
    if (Cout.GetCursor().x != 0)
      Cout << '\n';
    if (IsNull(message))
      Cout << "!!! HURRAH !!! (for something or other, I guess)\n";
    else
      Cout << "!!! HURRAH !!! " << message << '\n';
    Cout.SetForegroundColour(oldcolour);
  }

  return Error::None;
  
}

Status Befe::ErrorStatus(char const *func, Status status) {
  ErrorStatus(String(func), status);
  return Error::None;
}

Status Befe::ErrorStatus(String const &func, Status status) {

  UInt   oldcolour;

  if (Cout.GetCursor().x != 0)
    Cout << '\n';
  oldcolour = Cout.SetForegroundColour(Console::Red);
  Cout << "*** ERROR " << func << ": " << BEFE::ErrorString(status) << '\n';
  Cout.SetForegroundColour(oldcolour);

  return Error::None;
  
}

Status Befe::ErrorString(char const *str) {
  return ErrorString(String(str));
}

Status Befe::ErrorString(String const &str) {

  UInt   oldcolour;

  if (Cout.GetCursor().x != 0)
    Cout << '\n';
  oldcolour = Cout.SetForegroundColour(Console::Red);
  Cout << str << '\n';
  Cout.SetForegroundColour(oldcolour);

  return Error::None;
  
}

Status Befe::Debug(String const &message) {

  if (Cout.GetCursor().x != 0)
    Cout << '\n';
  Cout.PutBytes("*** DEBUG: ");
  if (IsNull(message))
    Cout.PutBytes("No reason given\n");
  else {
    Cout.PutBytes(message);
    Cout.PutBytes("\n");
  }

  return Error::None;
  
}

Status Befe::Debug(char const *message) {

  if (Cout.GetCursor().x != 0)
    Cout << '\n';
  Cout.PutBytes("*** DEBUG: ");
  if (IsNull(message))
    Cout.PutBytes("No reason given\n");
  else {
    Cout.PutBytes(message);
    Cout.PutBytes("\n");
  }

  return Error::None;
  
}

Status Befe::Deprecated(char const *message) {

  if (gShowDeprecated) {

    if (Cout.GetCursor().x != 0)
      Cout << '\n';
    Cout.PutBytes("*** DEPRECATED: ");
    if (IsNull(message))
      Cout.PutBytes("No reason given\n");
    else {
      Cout.PutBytes(message);
      Cout.PutBytes("\n");
    }

  }
  
  return Error::None;
  
}

//----------------------------------------------------------------------
//
// Befe._ValidateFileName - Validate and adjust File to Full Path
//

Status Befe::_ValidateFileName(String &fName, Boolean exists) {
  
  Status  status;
  String  genericName;
  String  cwd;
  String  fullPath;
  String  fileName;
  Int     star;
  Strings parts;
  
  if (!TheOS) goto NOOS;
  if (!TheProcess) goto NOPROCESS;
  
  genericName = fName;
  status = TheOS->PathToGeneric(genericName);    
  if (status) goto SOMEERROR;
  
  // Make sure no wildcards...
  star = genericName.Find('*');
  if (!IsNull(star))
    if (star == 0 || genericName.Get(star-1) != '\\') goto NOWILDCARDS;
  
  // Handle relative Path...
  if (!TheOS->IsFullPath(genericName)) {
    if (!TheOS->IsValidFileName(genericName)) goto INVALIDNAME;
    if (!TheOS->IsRelativePath(genericName))
      genericName = String("./") + genericName;
    cwd = TheProcess->GetCurrentWorkingDirectory();
    status = TheOS->ApplyRelativePath(cwd, genericName, genericName);
    if (status) goto SOMEERROR;
  }
    
  // Now, Separate Directories from file and validate...
  status = TheOS->PathSplit(genericName, parts);
  if (status) goto SOMEERROR;
  if (parts.Length() <= 1) goto INVALIDNAME;
  fileName = parts.Get(-1);
  status = parts.Remove(-1);
  if (status) goto SOMEERROR;
  fullPath = parts.Join('/');
  fullPath = fullPath + '/';
  if (!TheOS->Exists(fullPath)) goto DIRNOTEXIST;
  fullPath += fileName;
  
  // Check for Exists/Not-Exists...
  if (!exists &&  TheOS->Exists(fullPath)) goto ALREADYEXISTS;
  if ( exists && !TheOS->Exists(fullPath)) goto DOESNOTEXIST;
  
  // All is OK
  fName = genericName;
  
  // Handle Errors
  status = Error::None;
  while (false) {
    NOOS:          status = Error::InternalNoTheOS;       break;
    NOPROCESS:     status = Error::InternalNoTheProcess;  break;
    NOWILDCARDS:   status = Error::FileWildCardsNotValid; break;
    INVALIDNAME:   status = Error::FileInvalidFileName;   break;
    DIRNOTEXIST:   status = Error::FileDirDoesNotExist;   break;
    ALREADYEXISTS: status = Error::FileAlreadyExists;     break;
    DOESNOTEXIST:  status = Error::FileDoesNotExist;      break;
    SOMEERROR:                                            break;
  }

  return status;
  
}

//----------------------------------------------------------------------
//
// Befe.ValidateNewFileName      - Validate a new File and adjust to Full Path
// Befe.ValidateExistingFileName - Validate an existing File and adjust to Full Path
//

Status Befe::ValidateNewFileName(String &fName) {
  return _ValidateFileName(fName,false);
}

Status Befe::ValidateExistingFileName(String &fName) {
  return _ValidateFileName(fName,true);
}

//----------------------------------------------------------------------
//
// Befe.FindConfigurationFile - Find Configuration File
//

Status Befe::FindConfigurationFile(String &fName) {

  Status status;            // Return status
  String tryDir;            // Directory we're trying in
  String fileName;          // The found file name
  UInt   option;            // Current option we're trying
  String cwd;               // Current working directory

  if (IsNull(TheOS)) goto NOOS;
  if (IsNull(TheProcess)) goto NOPROCESS;
  
  cwd = TheProcess->GetCurrentWorkingDirectory();
  
  //
  // For each directory option...
  //

  option = 0;
  
  while (true) {
    
    // Based on option
    switch (option) {
      case 0: // Try executable directory
        if (!IsNull(TheProcess))
          tryDir = TheProcess->GetExecutablePath();
        else
          tryDir.SetEmpty();
        break;
      case 1: // Try Befe_Home
        tryDir = TheBefe->GetEnvironmentValue("BEFE_Home");
        break;
      case 2: // Try Befe_Root
        tryDir = TheBefe->GetEnvironmentValue("BEFE_Root");
        break;
      case 3: // Try Befe_TestData
        tryDir = TheBefe->GetEnvironmentValue("BEFE_TestData");
        break;
      case 4: // Try Current Working Direcvtory
        tryDir = cwd;
        break;
      default:
        goto NOTFOUND;
    }
  
    // Turn into full path with trailing '/'...  
    if (tryDir.Length()) {
      status = TheOS->PathToGeneric(tryDir);
      if (status) goto SOMEERROR;
      if (!TheOS->IsFullPath(tryDir)) {
        status = TheOS->ApplyRelativePath(cwd, tryDir, tryDir);
        if (status) goto SOMEERROR;
      }
      if (tryDir.Get(-1) != '/') tryDir.Append('/');
    }
  
    // See if it's there...
    if (tryDir.Length()) {
      fileName = tryDir + fName;
      if (TheOS->IsFile(fileName) && TheOS->Exists(fileName)) goto FOUNDIT;
    }
  
    // Nope, try next directory (if any)...
    option++;
    
  }
    
  // Handle errors
  status = Error::None;
  while (false) {
    NOOS:      status = Error::InternalNoTheOS;      break;
    NOPROCESS: status = Error::InternalNoTheProcess; break;
    FOUNDIT:   status = Error::None;                 break;
    NOTFOUND:  status = Error::FileNotFound;         break;
    SOMEERROR:                                       break;
  }

  // Return the file name if found it
  if (!status)
    fName = fileName.Consumable();
    
  return status;
  
}

//----------------------------------------------------------------------
//
// Befe Application Identification Methods
//

Status Befe::SetApplicationName(String const &name)        { appName        = name;       return Error::None;}
Status Befe::SetApplicationDescription(String const &desc) { appDescription = desc;       return Error::None;}
Status Befe::SetApplicationVersion(UInt version)           { appVersion     = version;    return Error::None;}
Status Befe::SetApplicationSubVersion(UInt subVersion)     { appSubVersion  = subVersion; return Error::None;}
Status Befe::SetApplicationRevision(UInt revision)         { appRevision    = revision;   return Error::None;}
String Befe::GetApplicationName()                          { return appName;        }
String Befe::GetApplicationDescription()                   { return appDescription; }
UInt   Befe::GetApplicationVersion()                       { return appVersion;     }
UInt   Befe::GetApplicationSubVersion()                    { return appSubVersion;  }
UInt   Befe::GetApplicationRevision()                      { return appRevision;    }
	
//----------------------------------------------------------------------
//
// Befe Error Management Methods
//

Boolean Befe::ErrorMessagesAreLoaded() const {
  return errMsgArray.Length() != 0;
}

Status Befe::LoadErrorMessages() {
  
  Status status;
  String fileName;
  UInt   numErrors;
  
  if (errLoading) goto OK;
  if (ErrorMessagesAreLoaded()) goto LOADED;

  // Find the file
  fileName = "BEFE_ErrorMessages.bmsg";
  status = FindConfigurationFile(fileName);
  if (status) goto SOMEERROR;
  
  // Load it
  errLoading = true;
  status = MessageLoadFile(fileName, errRngArray, errMsgArray, errIdIndexes, errNameIndexes, numErrors);
  errLoading = false;
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    LOADED:    status = Error::BEFEMessagesAlreadyLoaded; break;
    OK:        status = Error::None;                      break;
    SOMEERROR:                                            break;
  }
  
  return status;

}

Status Befe::UnloadErrorMessages() {
  
  Status retStatus;
  Status status;
  
  retStatus = errRngArray.Reset();
  status    = errMsgArray.Reset();
  if (status && !retStatus) retStatus = status;
  status    = errIdIndexes.Reset();
  if (status && !retStatus) retStatus = status;
  status    = errNameIndexes.Reset();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}
  
String Befe::ErrorMessage(Status status) {

  Message      theMessage;
  UInt         msgIdx;
  MessageRange theRange;
  Int          rngIdx;
  String       theString;
  
  // Load messages if not already loaded...
  LoadErrorMessages();
  
  // Get on with it...
  SetNull(msgIdx);
  errIdIndexes.Get(status, msgIdx);
  if (!IsNull(msgIdx)) {
    rngIdx = errRngArray.Find(status);
    if (rngIdx >= 0) {
      theRange = errRngArray.Get(rngIdx);
      theString = theRange.shortName + '+' + ToString(status-theRange.range.idx1).LeftPad(3,'0');
    }
    else
      theString = String("???+0x") + ToHexString(status,8);
    theString += ": ";
    theMessage = errMsgArray.Get(msgIdx);
    theString += theMessage.message.Consumable();
  }
  else
    theString = (String("Error 0x") + ToHexString(status,8)).Consumable();
    
  // Finally, unescape it...
  theString = theString.Unescape();
  
  return theString.Consumable();
  
}

String Befe::GetMessage(Status msgNo) {
  
  UInt    msgIdx;
  Message theMessage;
  String  theString;
  
  // Load messages if not already loaded...
  LoadErrorMessages();
  
  // Get the message
  SetNull(msgIdx);
  errIdIndexes.Get(msgNo, msgIdx);
  if (!IsNull(msgIdx)) {
    theMessage = errMsgArray.Get(msgIdx);
    theString += theMessage.message.Consumable();
  }
  
  return theString.Consumable();
  
}

//----------------------------------------------------------------------
//
// Cryptography Methods
//

CryptKey Befe::GetCryptKey() {

  CryptKey key;
  
  if (IsNull(cryptKey))
    GetCryptKey(key);

  return key;

}

Status Befe::GetCryptKey(CryptKey &key) {
  
  Status   status;
  String   befeKeyFile;
  CryptKey localKey;
  
  befeKeyFile = GetEnvironmentValue("BEFE_KeyFile");
  if (IsEmpty(befeKeyFile)) goto NOBEFEKEYFILE;
  status = TheOS->PathToGeneric(befeKeyFile);
  if (status) goto SOMEERROR;
  status = ReadKeyFile(befeKeyFile, localKey);
  if (status == Error::FileDirDoesNotExist || status== Error::FileDoesNotExist)
    status = Error::CryptKeyKeyFileNotExist;
  if (status) goto SOMEERROR;
  cryptKey = localKey;
  key = localKey;
  
  // Handle errors
  while (false) {
    NOBEFEKEYFILE: status = Error::CryptKeyNoBEFEKeyFile; break;
    SOMEERROR:                                            break;
  }
  if (status)
    key.Reset();
  
  return status;
  
}

} // ...Namespace BEFE
