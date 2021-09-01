//!befe-publish src
//----------------------------------------------------------------------
// File: BuildFiles.cpp - Build Class File Methods
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
#include "Build.h"

namespace BEFE { // Namespace BEFE...

static char const *BUILD_outputDirs[] = {"bin","src","inc","lib","obj","doc","home",0};

//----------------------------------------------------------------------
//
// OSWalker context and function for Build.GetInputFileNames...
//

Status Build::InputWalker(String const &dir, Ptr context) { // Build.InputWalker...
  
  Status  status;
  Build  *build;
  String  curDir;
  Strings fileNames;
  String  fileName;
  String  fileNameOnly;
  String  pathOnly;
  UInt    curIdx;
  
  build = (Build *)context;
  if (BEFE::IsNull(build)) goto NULLPOINTER;
  curDir = dir;
  status = PathToGeneric(curDir);
  if (status) goto SOMEERROR;
  if (BEFE::IsNull(curDir.Find("/BEFE-Core")) &&
      BEFE::IsNull(curDir.Find("/docs"))
     ) goto OK;
  status = GetFiles(curDir, fileNames);
  if (status) goto SOMEERROR;
  for (curIdx=0; curIdx < fileNames.Length(); curIdx++) {
    
    // Get fhe file name...
    fileName = fileNames.Get(curIdx);
    status = PathToGeneric(fileName);
    if (status) goto SOMEERROR;
    fileNameOnly = FileOnly(fileName);

    // If platform specific file make sure we want it...
    if (fileName.Get(Span(0,build->svnPlatformRoot.Length())) == build->svnPlatformRoot) {
      if (fileName.Get(Span(0,build->svnPlatformDir.Length())) != build->svnPlatformDir) {
        continue;
      }
    }
    // We want this file...
    if (fileName.Get(Range(-2,-1)) == ".h") {
      status = build->headerFiles.Append(fileName.Consumable());
      if (status) goto SOMEERROR;
    }
    else if (fileName.Get(Range(-4,-1)) == ".cpp") {
      status = build->sourceFiles.Append(fileName.Consumable());
      if (status) goto SOMEERROR;
    }
    else if (fileName.Get(Range(-5,-1)) == ".befe" || fileName.Get(Range(-4,-1)) == ".txt") {
      status = build->dataFiles.Append(fileName.Consumable());
      if (status) goto SOMEERROR;
    }
    else if (fileName.Get(Range(-4,-1)) == ".odt" && fileNameOnly.Get(Span(0,4)) == "BEFE") {
      status = build->docFiles.Append(fileName.Consumable());
      if (status) goto SOMEERROR;
    }
  }
  
  // Handle errors  
  while (false) {
    OK:          status = Error::None;                break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
} // ...Build.InputWalker

//----------------------------------------------------------------------
//
// OSWalker context and function for Build.CleanOutputDirectories...
//

Status Build::CleanWalker(String const &dir, Ptr context) { // Build.CleanWalker...
  
  Status  status;
  Build  *build;
  String  curDir;
  String  tDir;
  Strings fileNames;
  String  fileName;  
  UInt    curIdx;
  
  build = (Build *)context;
  if (BEFE::IsNull(build)) goto NULLPOINTER;

  // Special case for '_' directories if no GIT option...
  curDir = dir;
  status = PathToGeneric(curDir);
  if (dir.Get(-4) == '_' && !(build->options & OptionGIT)) {
    tDir = curDir.Get(Span(-5, NaN));
    if (tDir != "/_bin" && tDir != "/_obj" && tDir != "/_lib") goto OK;
  }

  // Clean the files...
  if (status) goto SOMEERROR;
  status = GetFiles(curDir, fileNames);
  if (status) goto SOMEERROR;
  for (curIdx=0; curIdx < fileNames.Length(); curIdx++) {
    fileName = fileNames.Get(curIdx);
    status = DeleteFile(fileName);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors  
  while (false) {
    OK:          status = Error::None;                break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
} // ...Build.CleanWalker

//----------------------------------------------------------------------
//
// Protected Methods: _DetermineFileNames  
//

Status Build::_DetermineFileNames() { // Build._DetermineFileNames...
  
  Status status;
  String befe_GIT;
  
  // Clear current settings
  status = headerFiles.SetEmpty();
  if (status) goto SOMEERROR;
  status = sourceFiles.SetEmpty();
  if (status) goto SOMEERROR;
  status = dataFiles.SetEmpty();
  if (status) goto SOMEERROR;

  // Do it
  befe_GIT = settings.Get("BEFE_GIT");
  status = WalkPath(befe_GIT, InputWalker, (Ptr)this);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }

  return status;
  
} // ...Build._DetermineFileNames

//----------------------------------------------------------------------
//
// Protected Methods: _CleanOutputDirectories
//

Status Build::_CleanOutputDirectories() { // Build._CleanOutputDirectories...

  Status status;
  String befe_Build;
  
  // Do it
  befe_Build = settings.Get("BEFE_Build");
  if (!Exists(befe_Build)) goto OK;

  if (gVerbose)
    Cout << String(' ')*indent << "Cleaning Output Directories...";

  status = WalkPath(befe_Build, CleanWalker, (Ptr)this);
  if (status) goto SOMEERROR;

  if (gVerbose) Cout << '\n';

  // Handle errors
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  return status;
  
} // ...Build._CleanOutputDirectories

//----------------------------------------------------------------------
//
// Protected Methods: _CreateOutputDirectories
//

Status Build::_CreateOutputDirectories() { // Build._CreateOutputDirectories...
  
  Status status;
  String befe_Build;
  String theDir;
  UInt   curIdx;
  
  if (gVerbose)
    Cout << String(' ')*indent << "Creating Output Directories...";

  befe_Build = settings.Get("BEFE_Build");
  for (curIdx=0; BUILD_outputDirs[curIdx]; curIdx++) {
    // Create "public" or "exposed" one...
    theDir = befe_Build + "/" + BUILD_outputDirs[curIdx];
    if (!Exists(theDir)) {
      status = CreateDirectories(theDir);
      if (status) goto SOMEERROR;
    }
    if (!IsDirectory(theDir)) goto NOTDIR;
    // No need to create .../_home
    if (Strcmp(BUILD_outputDirs[curIdx],"home") == 0) continue;
    // Create "private" or "working" one...
    theDir = befe_Build + "/_" + BUILD_outputDirs[curIdx];
    if (!Exists(theDir)) {
      status = CreateDirectories(theDir);
      if (status) goto SOMEERROR;
    }
    if (!IsDirectory(theDir)) goto NOTDIR;
  }
  
  if (gVerbose) Cout << '\n';
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTDIR:    status = Error::FileNameIsNotDir; break;
    SOMEERROR:                                   break;
  }

  return status;
  
} // ...Build._CreateOutputDirectories

//----------------------------------------------------------------------
//
// Protected Methods: _CopyInputFiles
//

Status Build::_CopyInputFiles() { // Build._CopyInputFiles...

  Status status;
  String befe_Build;
  String toDir;
  String fromFileName;
  String toFileName;
  UInt   curIdx;
  UInt   curPct;
  UInt   newPct;
  Time   st;
  Time   et;
  
  befe_Build = settings.Get("BEFE_Build");
  
  //
  // Copy header files into BEFE_Build/_include
  //
  
  toDir = befe_Build + "/_inc";
  if (!Exists(toDir)) goto DIRNOTEXIST;

  if (gVerbose) {
    Cout << String(' ')*indent << "Copying header files...";
    if (Cout.IsInteractive())
      Cout << "  0%";
  }
  BEFE::SetNull(curPct);
  st.Set();

  for (curIdx=0; curIdx < headerFiles.Length(); curIdx++) { // For Each Header File...
    fromFileName = headerFiles.Get(curIdx);
    toFileName = toDir + '/' + FileOnly(fromFileName);
    if (Exists(toFileName)) {
      status = DeleteFile(toFileName);
      if (status) goto SOMEERROR;
    }
    status = CopyFile(fromFileName, toFileName);
    if (status) goto SOMEERROR;
    // Display progress if need be...
    newPct = ((curIdx+1)*100)/headerFiles.Length();
    if (newPct != curPct && gVerbose && Cout.IsInteractive()) {
      Cout << String(Char(BS))*4 << ToString(newPct,3) << '%';
      Cout.Flush();
    }
  } // ...For Each Header File
  if (gVerbose && Cout.IsInteractive()) Cout << String(Char(BS))*4 << "100%";
  et.Set();
  if (gVerbose && gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';
  
  if (gVerbose) Cout << '\n';
  
  //
  // Patch Revision in BEFE.h...
  //

  st.Set();
  toFileName = toDir + "/BEFE.h";
  if (!IsFile(toFileName))
    BEFE_WARN("File BEFE.h Missing");
  else if (options & OptionGIT) {
    if (gVerbose) {
      Cout << "Patching BEFE.h...";
      Cout.Flush();
    }
    status = _PatchGITRevision(toFileName);
    if (status) goto SOMEERROR; 
    et.Set();
    if (gVerbose && gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';  
    if (gVerbose) Cout << '\n';
    //Cout.Flush();
  }
  
  //
  // Copy source files into BEFE_Build/src
  //
  
  toDir = befe_Build + "/_src";
  if (!Exists(toDir)) goto DIRNOTEXIST;

  if (gVerbose) {
    Cout << String(' ')*indent << "Copying source files...";
    if (Cout.IsInteractive())
      Cout << "  0%";
  }
  BEFE::SetNull(curPct);
  st.Set();
  
  for (curIdx=0; curIdx < sourceFiles.Length(); curIdx++) { // For Each Source File...
    fromFileName = sourceFiles.Get(curIdx);
    toFileName = toDir + '/' + FileOnly(fromFileName);
    if (Exists(toFileName)) {
      status = DeleteFile(toFileName);
      if (status) goto SOMEERROR;
    }
    status = CopyFile(fromFileName, toFileName);
    if (status) goto SOMEERROR;
    // Display progress if need be...
    newPct = ((curIdx+1)*100)/headerFiles.Length();
    if (newPct != curPct && gVerbose && Cout.IsInteractive()) {
      Cout << String(Char(BS))*4 << ToString(newPct,3) << '%';
      Cout.Flush();
    }
  } // ...For Each Source File
  if (gVerbose && Cout.IsInteractive()) Cout << String(Char(BS))*4 << "100%";
  et.Set();
  if (gVerbose && gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';

  if (gVerbose) Cout << '\n';
  
  //
  // Handle errors
  //
  status = Error::None;
  while (false) {
    DIRNOTEXIST: status = Error::FileDirDoesNotExist; break;
    SOMEERROR:                                        break;
  }

  return status;
    
} // ...Build._CopyInputFiles

//----------------------------------------------------------------------
//
// Protected Methods: _CompileCppSourceFiles
//

Status Build::_CompileCppSourceFiles(Boolean debug) { // Build._CompileCppSourceFiles...

  Status        status;
  String        befe_Build;
  String        fromDir;
  String        toDir;
  Strings       fromFiles;
  String        fromFileName;
  String        toFileName;
  UInt          curIdx;
  UInt          curPct;
  UInt          newPct;
  String        cmd;
  Time          st;
  Time          et;
  
  befe_Build = settings.Get("BEFE_Build");
  
  fromDir = befe_Build + "/_src";
  if (!Exists(fromDir)) goto DIRNOTEXIST;
  toDir = befe_Build + "/_obj";
  if (!Exists(toDir)) goto DIRNOTEXIST;
  
  // Get the list of source files, and keep only the '.cpp' ones...
  status = GetFiles(fromDir, fromFiles);
  if (status) goto SOMEERROR;
  curIdx = 0;
  while (curIdx < fromFiles.Length()) {
    fromFileName = fromFiles.Get(curIdx);
    if (fromFileName.Get(Range(-4,-1)) != ".cpp") {
      status = fromFiles.Remove(curIdx);
      if (status) goto SOMEERROR;
    }
    else
      curIdx++;
  }
  
  //
  // Compile each one...
  // 
  
  if (gVerbose) {
    Cout << String(' ')*indent << "Compiling C++ ";
    if (debug)
      Cout << "DEBUG";
    else
      Cout << "RELEASE";
    Cout << " source files...";
    if (Cout.IsInteractive())
      Cout << "  0%";
  }
  BEFE::SetNull(curPct);
  st.Set();
  for (curIdx=0; curIdx < fromFiles.Length(); curIdx++) { // For Each C++ File...

    fromFileName = FileOnly(fromFiles.Get(curIdx));
    toFileName = toDir + "/" + fromFileName.Get(Span(0,-4));
    if (debug) toFileName += "_dbg";
    toFileName += ".o";
    if (Exists(toFileName)) {
      status = DeleteFile(toFileName);
      if (status) goto SOMEERROR;
    }
    
    // Compile it
    cmd  = extBinaries.Get("g++");
    if (gVeryVerbose)
      cmd += " -Wall";
    cmd += String(" -c ") + fromDir + '/' + fromFileName;
    cmd += String(" -o ") + toFileName;
    if (debug)
      cmd += " -g";
    cmd += String(" -std=c++0x -fno-exceptions -finline-functions -nodefaultlibs -fno-rtti");
    cmd += String(" -I ") + befe_Build + "/_inc";
    status = _Invoke(cmd);
    if (status) goto SOMEERROR;
    
    // Display progress if need be...
    newPct = ((curIdx+1)*100)/fromFiles.Length();
    if (newPct != curPct && gVerbose && Cout.IsInteractive()) {
      Cout << String(Char(BS))*4 << ToString(newPct,3) << '%';
      Cout.Flush();
    }

  } // ...For Each C++ Source File

  if (gVerbose && Cout.IsInteractive()) Cout << String(Char(BS))*4 << "100%";
  et.Set();
  if (gVerbose && gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';
  
  if (gVerbose) Cout << '\n';
  
  //
  // Handle errors
  //
  status = Error::None;
  while (false) {
    DIRNOTEXIST: status = Error::FileDirDoesNotExist; break;
    SOMEERROR:                                        break;
  }

  return status;
    
} // ...Build._CompileCppSourceFiles

//----------------------------------------------------------------------
//
// Protected Methods: _CreateLibrary
//

Status Build::_CreateLibrary(Boolean debug) { // Build._CreateLibrary...

  Status        status;
  String        befe_Build;
  String        fromDir;
  String        toDir;
  Strings       fromFiles;
  String        fromFileName;
  String        toFileName;
  UInt          curIdx;
  UInt          curPct;
  UInt          newPct;
  String        cmd;
  Boolean       keepIt;
  Time          st;
  Time          et;
  
  befe_Build = settings.Get("BEFE_Build");
  
  fromDir = befe_Build + "/_obj";
  if (!Exists(fromDir)) goto DIRNOTEXIST;
  toDir = befe_Build + "/_lib";
  if (!Exists(toDir)) goto DIRNOTEXIST;
  
  // Get the list of the object files, and keep only the proper '.o' ones...
  status = GetFiles(fromDir, fromFiles);
  if (status) goto SOMEERROR;
  curIdx = 0;
  while (curIdx < fromFiles.Length()) {
    fromFileName = fromFiles.Get(curIdx);
    keepIt = fromFileName.Get(Range(-6,-1)) == "_dbg.o";
    if (!debug)
      keepIt = !keepIt;
    if (!keepIt) {
      status = fromFiles.Remove(curIdx);
      if (status) goto SOMEERROR;
    }
    else
      curIdx++;
  }
  
  //
  // Append each one...
  // 
  
  if (gVerbose) {
    Cout << String(' ')*indent << "Creating ";
    if (debug)
      Cout << "DEBUG";
    else
      Cout << "RELEASE";
    Cout << " library...";
    if (Cout.IsInteractive())
      Cout << "  0%";
  }
  BEFE::SetNull(curPct);
  st.Set();
  
  toFileName = toDir + "/libBEFE-";
  toFileName += ToString(GetMainVersion()) + '.' + ToString(GetSubVersion());  
  if (debug)
    toFileName += "Debug";
  toFileName += ".a";

  if (Exists(toFileName)) {
    status = DeleteFile(toFileName);
    if (status) goto SOMEERROR;
  }
  
  for (curIdx=0; curIdx < fromFiles.Length(); curIdx++) { // For Each Object File...

    fromFileName = FileOnly(fromFiles.Get(curIdx));
    cmd  = extBinaries.Get("ar");
    cmd += String(" -rcs ");
    cmd += /*String(" -qc ") +*/ toFileName;
    cmd += String(' ') + fromDir + '/' + fromFileName;
    status = _Invoke(cmd);
    if (status) goto SOMEERROR;
    
    // Display progress if need be...
    newPct = ((curIdx+1)*100)/fromFiles.Length();
    if (newPct != curPct && gVerbose && Cout.IsInteractive()) {
      Cout << String(Char(BS))*4 << ToString(newPct,3) << '%';
      Cout.Flush();
    }

  } // ...For Each C++ Source File

  if (gVerbose && Cout.IsInteractive()) Cout << String(Char(BS))*4 << "100%";
  et.Set();
  if (gVerbose && gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';
  
  if (gVerbose) Cout << '\n';
  
  //
  // Handle errors
  //
  status = Error::None;
  while (false) {
    DIRNOTEXIST: status = Error::FileDirDoesNotExist; break;
    SOMEERROR:                                        break;
  }

  return status;
    
} // ...Build._CreateLibrary

//----------------------------------------------------------------------
//
// Protected Methods: _CreateExecutable
//

Status Build::_CreateExecutable(Boolean debug) { // Build.CreateExecutable...

  Status        status;
  String        befe_Build;
  String        fromDir;
  String        toDir;
  String        fromFileName;
  String        toFileName;
  String        libFileName;
  String        libFullPath;
  String        cmd;
  Time          st;
  Time          et;
  
  befe_Build = settings.Get("BEFE_Build");
  
  if (gVerbose) {
    Cout << String(' ')*indent << "Creating ";
    if (debug)
      Cout << "DEBUG";
    else
      Cout << "RELEASE";
    Cout << " Executable...";
  }
  st.Set();
  
  fromDir = befe_Build + "/_src";
  if (!Exists(fromDir)) goto DIRNOTEXIST;
  toDir = befe_Build + "/_bin";
  if (!Exists(toDir)) goto DIRNOTEXIST;

  // Calculate file names...
  fromFileName = fromDir + "/main.cpp";
  if (!Exists(fromFileName)) goto FILENOTEXIST;
  
  toFileName = toDir + "/Befe";
  if (debug)
    toFileName += "Debug";
  if (Is_WinNT())
    toFileName += ".exe";
  if (Exists(toFileName)) {
    status = DeleteFile(toFileName);
    if (status) goto SOMEERROR;
  }
  
  libFileName = String("BEFE-") + ToString(GetMainVersion()) + '.' + ToString(GetSubVersion());
  if (debug) libFileName += "Debug";
  libFullPath = befe_Build+"/_lib/lib"+libFileName+".a";
  if (!Exists(libFullPath))
    goto FILENOTEXIST;
  
  // Do it...
  cmd = extBinaries.Get("g++");
  if (debug)
    cmd += " -g";
  cmd += " -static -std=c++0x -fno-exceptions";
  cmd += String(" -I ") + befe_Build + "/_inc";
  cmd += String(' ') + fromFileName;  
  cmd += String(" -o ") + toFileName;
  cmd += String(" -L") + befe_Build + "/_lib";
  cmd += String(" -l") + libFileName;
  if (Is_Ubuntu()) 
    cmd += String(" -lrt -luuid");
	// TEMP...
  if (true)
		cmd += String(" -Xlinker -Map=") + toFileName + ".map";
	// ...TEMP
  status = _Invoke(cmd);
  if (status) goto SOMEERROR;
  et.Set();
  if (gVerbose && gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';
  
  if (gVerbose) Cout << '\n';

  //
  // Handle errors
  //
  
  status = Error::None;
  while (false) {
    FILENOTEXIST: status = Error::FileDoesNotExist;    break;
    DIRNOTEXIST:  status = Error::FileDirDoesNotExist; break;
    SOMEERROR:                                         break;
  }

  return status;
    
} // ...Build._CreateExecutable

//----------------------------------------------------------------------
//
// Protected Methods: _Publish<xxx>
//

Status Build::_PublishHeaders() {

  Status        status;
  String        befe_Build;
  String        fromDir;
  String        toDir;
  Strings       fileNames;
  String        fromFileName;
  String        toFileName;
  UInt          numFiles;
  UInt          i;
  UInt          numCopied;
  Time          st;
  Time          et;
  
  numCopied = 0;
  BEFE::SetNull(numFiles);
  
  befe_Build = settings.Get("BEFE_Build");
  fromDir = befe_Build + "/_inc";
  if (!Exists(fromDir)) goto DIRNOTEXIST;
  toDir = befe_Build + "/inc";
  if (!Exists(toDir)) goto DIRNOTEXIST;
  
  // Get the File Names...
  if (gVerbose)
    Cout << String(' ')*indent << "Locating Header Files to publish...";
  st.Set();
  status = _GetPublishableFiles(fromDir, "inc", fileNames);
  if (status) goto SOMEERROR;
  et.Set();
  if (gVerbose) {
    Cout << ' ' << fileNames.Length() << " file";
    if (fileNames.Length() != 1) Cout << 's';
    Cout << " to publish";
    if (gVeryVerbose && gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';
    Cout << '\n';
  }
  
  //
  // Publish them...
  //
  
  if (gVerbose)
    Cout << String(' ')*indent << "Publishing Header Files...";
  st.Set();
  
  numFiles = 0;
  
  for (i=0; i < fileNames.Length(); i++) {
    numFiles++;
    fromFileName = fileNames.Get(i);
    toFileName = toDir + '/' + FileOnly(fromFileName);
    if (!Exists(fromFileName) || !IsFile(fromFileName)) continue;
    if (Exists(toFileName)) {
      status = DeleteFile(toFileName);
      if (status) goto SOMEERROR;
    }
    status = CopyFile(fromFileName,  toFileName);
    if (status) goto SOMEERROR;
    numCopied++;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    DIRNOTEXIST:  status = Error::FileDirDoesNotExist; break;
    SOMEERROR:                                         break;
  }

  if (!status && gVerbose) {
    et.Set();
    if (numCopied == 0)
      Cout << "***No Header Files Published***";
    else
      Cout << ' ' << numCopied << " of " << numFiles << " Header Files Published";
    if (gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';
    Cout << '\n';
  }
  
  return status;
  
}

Status Build::_PublishSources() {

  Status        status;
  String        befe_Build;
  String        fromDir;
  String        toDir;
  Strings       fileNames;
  String        fromFileName;
  String        toFileName;
  UInt          numFiles;
  UInt          i;
  UInt          numCopied;
  Time          st;
  Time          et;
  
  numCopied = 0;
  BEFE::SetNull(numFiles);
  
  befe_Build = settings.Get("BEFE_Build");
  fromDir = befe_Build + "/_src";
  if (!Exists(fromDir)) goto DIRNOTEXIST;
  toDir = befe_Build + "/src";
  if (!Exists(toDir)) goto DIRNOTEXIST;
    
  // Get the File Names...
  if (gVerbose)
    Cout << String(' ')*indent << "Locating Source Files to publish...";
  st.Set();
  status = _GetPublishableFiles(fromDir, "src", fileNames);
  if (status) goto SOMEERROR;
  et.Set();
  if (gVerbose) {
    Cout << ' ' << fileNames.Length() << " file";
    if (fileNames.Length() != 1) Cout << 's';
    Cout << " to publish";
    if (gVeryVerbose && gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';
    Cout << '\n';
  }
  
  //
  // Publish them...
  //
  
  if (gVerbose)
    Cout << String(' ')*indent << "Publishing Source Files...";
  st.Set();
  
  numFiles = 0;
  
  for (i=0; i < fileNames.Length(); i++) {
    numFiles++;
    fromFileName = fileNames.Get(i);
    toFileName = toDir + '/' + FileOnly(fromFileName);
    if (!Exists(fromFileName) || !IsFile(fromFileName)) continue;
    if (Exists(toFileName)) {
      status = DeleteFile(toFileName);
      if (status) goto SOMEERROR;
    }
    status = CopyFile(fromFileName,  toFileName);
    if (status) goto SOMEERROR;
    numCopied++;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    DIRNOTEXIST:  status = Error::FileDirDoesNotExist; break;
    SOMEERROR:                                         break;
  }

  if (!status && gVerbose) {
    et.Set();
    if (numCopied == 0)
      Cout << "***No Source Files Published***";
    else
      Cout << ' ' << numCopied << " of " << numFiles << " Source Files Published";
    if (gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';
    Cout << '\n';
  }
  
  return status;
  
}

Status Build::_PublishReleaseOrDebug(String const &subDir, Boolean debug) {
  
  Status  status;
  String  theBase;
  String  theDir;
  Strings theFiles;
  UInt    curIdx;
  String  fromFile;
  String  toFile;
  Boolean keepIt;
  
  if (gVerbose)
    Cout << String(' ')*indent << "Publishing "
        << ((debug) ? "DEBUG" : "RELEASE") << " directory '" << subDir << "'...";

  theBase = settings.Get("BEFE_Build");
  PathToNative(theBase);
  theDir = theBase + "/_" + subDir;
  if (!Exists(theDir) || !IsDirectory(theDir)) goto OK;
  
  theFiles = GetFiles(theDir);
  for (curIdx=0; curIdx < theFiles.Length(); curIdx++) { // For Each File...
  
    fromFile = theFiles.Get(curIdx);
    toFile = theBase + '/' + subDir + '/' + fromFile.Get(Span(theBase.Length()+2+subDir.Length(), fromFile.Length()));
    // See if we want this based on Release/Debug...
    keepIt = debug == !BEFE::IsNull(fromFile.Find(String("Debug").Insensitive()));
    if (!keepIt) continue;
    
    // Delete it if already there
    if (Exists(toFile)) {
      status = DeleteFile(toFile);
      if (status) goto SOMEERROR;
    }
    
    // Copy it
    status = CopyFile(fromFile, toFile);
    if (status) goto SOMEERROR;

  } // ...For Each File
  
  if (gVerbose) Cout << '\n';

  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;

}

Status Build::_PublishDocumentation() {
  
  Status status;
  String befe_Build;
  String befe_Temp;
  String toDir;
  String tempDir;
  String fromFileName;
  String toFileName;
  String command;
  UInt   curIdx;
  UInt   curPct;
  UInt   newPct;
  Time   st;
  Time   et;
  
  toDir = settings.Get("BEFE_Build");
  status = PathToGeneric(toDir);
  if (status) goto SOMEERROR;
  if (toDir.Get(-1) != '/')
    toDir += '/';
  toDir += "doc";
  status = PathToNative(toDir);
  if (status) goto SOMEERROR;
  
  befe_Temp = settings.Get("BEFE_Temp");
  status = PathToNative(befe_Temp);
  if (status) goto SOMEERROR;
  
  if (gVerbose) {
    Cout << String(' ')*indent << "Publishing Documentation...";
    if (Cout.IsInteractive())
      Cout << "  0%";
    Cout.Flush();
  }
  BEFE::SetNull(curPct);

  st.Set();

  for (curIdx=0; curIdx < docFiles.Length(); curIdx++) { // For Each Documentation File...
    fromFileName = docFiles.Get(curIdx);
    toFileName = toDir + '/' + FileOnly(fromFileName);
    if (Exists(toFileName)) {
      status = DeleteFile(toFileName);
      if (status) goto SOMEERROR;
    }
    // Turn file names into Native format...
    status = PathToNative(toDir);
    if (status) goto SOMEERROR;
    status = PathToNative(fromFileName);
    if (status) goto SOMEERROR;    
    status = PathToNative(toFileName);
    if (status) goto SOMEERROR;
    // Invoke LibreOffice or whatever
    command  = '"';
    command += extBinaries.Get("soffice");
    command += '"';
    if (Is_Linux()) {
      command += " \"-env:UserInstallation=file://";
      command += befe_Temp;
      command += '"';
    }
    command << " --invisible --nodefault --nolockcheck --headless --writer --convert-to pdf -outdir"
            << ' ' << '"' << toDir  << '"'
            << ' ' << '"' << fromFileName << '"';
    // TEMP...
    if (Is_Unix())
      command += " >/dev/null";
    // ...TEMP
    status = _Invoke(command);
    if (status) goto SOMEERROR;
    // Display progress if need be...
    newPct = ((curIdx+1)*100)/docFiles.Length();
    if (newPct != curPct && gVerbose && Cout.IsInteractive()) {
      Cout << String(Char(BS))*4 << ToString(newPct,3) << '%';
      Cout.Flush();
    }
  } // ...For Each Documentation File
  if (gVerbose && Cout.IsInteractive()) Cout << String(Char(BS))*4 << "100%";
  et.Set();
  if (gVerbose && gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';
  if (gVerbose) Cout << '\n';

  //
  // Handle errors
  //
  status = Error::None;
  while (false) {
    SOMEERROR:                                        break;
  }

  return status;
  
}

Status Build::_PublishHomeFiles() { // Build._PublishHomeFiles...

  Status        status;
  String        befe_GIT;
  String        befe_Build;
  String        fromDir;
  String        toDir;
  Strings       fileNames;
  String        fromFileName;
  String        toFileName;
  UInt          numFiles;
  UInt          i;
  UInt          numCopied;
  Time          st;
  Time          et;
  
  numCopied = 0;
  BEFE::SetNull(numFiles);

  befe_GIT = settings.Get("BEFE_GIT");
  if (!Exists(befe_GIT)) goto GITNOTEXIST;
  PathToGeneric(befe_GIT);
  fromDir = befe_GIT;
  befe_Build = settings.Get("BEFE_Build");
  PathToGeneric(befe_Build);
  toDir = befe_Build + "/home";
  if (!Exists(toDir)) goto BLDDIRNOTEXIST;
  
  // Get the File Names...
  if (gVerbose)
    Cout << String(' ')*indent << "Locating Home Files to publish...";
  st.Set();
  status = _GetPublishableFiles(fromDir, "home", fileNames);
  if (status) goto SOMEERROR;
  et.Set();
  if (gVerbose) {
    Cout << ' ' << fileNames.Length() << " file";
    if (fileNames.Length() != 1) Cout << 's';
    Cout << " to publish";
    if (gVeryVerbose && gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';
    Cout << '\n';
  }
  
  //
  // Publish them...
  //
  
  if (gVerbose)
    Cout << String(' ')*indent << "Publishing Home Files...";
  st.Set();
  
  numFiles = 0;
  
  for (i=0; i < fileNames.Length(); i++) {
    numFiles++;
    fromFileName = fileNames.Get(i);
    toFileName = toDir + '/' + FileOnly(fromFileName);
    if (!Exists(fromFileName) || !IsFile(fromFileName)) continue;
    if (Exists(toFileName)) {
      status = DeleteFile(toFileName);
      if (status) goto SOMEERROR;
    }
    status = CopyFile(fromFileName,  toFileName);
    if (status) goto SOMEERROR;
    numCopied++;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    GITNOTEXIST:     status = Error::UtilBuildGITNotExist; break;
    BLDDIRNOTEXIST:  status = Error::FileDirDoesNotExist;  break;
    SOMEERROR:                                             break;
  }

  if (!status && gVerbose) {
    et.Set();
    if (numCopied == 0)
      Cout << "***No Header Files Published***";
    else
      Cout << ' ' << numCopied << " of " << numFiles << " Header Files Published";
    if (gShowElapsed) Cout << " (Elapsed Time: " << ToString(st, et) << ')';
    Cout << '\n';
  }
  
  return status;
  
} // ...Build._PublishHomeFiles

} // ...Namaspace BEFE
