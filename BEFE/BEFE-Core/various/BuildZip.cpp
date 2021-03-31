//!befe-publish src
//----------------------------------------------------------------------
// File: BuildZip.cpp - Build Class Zip/Tar Methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Build.h"

namespace BEFE { // Namespace BEFE...

struct BuildZipContext {
  String  baseDir;
  Strings zipDirs;
  Strings theFiles;  
};

typedef struct BuildZipContext ZipContext;

//----------------------------------------------------------------------
//
// OSWalker context and function for Build._Zip...
//

Status Build::ZipWalker(String const &dir, UInt32 context) { // Build.ZipWalker...
  
  Status      status;
  String      theDir;
  ZipContext *zip;
  Span        tSpan;
  String      dirRemain;
  Strings     dirFiles;
  UInt        curIdx;
  
  zip = (ZipContext *)context;
  if (BEFE::IsNull((void *)zip)) goto OK;
  if (!Exists(dir)) goto OK;
  theDir = dir;
  PathToGeneric(theDir);
  
  // Strip the base directory from this one and ignore if we don't want it...
  if (theDir.Length() <= zip->baseDir.Length()+1) goto OK;
  dirRemain = theDir.Get(Span(zip->baseDir.Length()+1, theDir.Length()));
  if (!zip->zipDirs.Contains(dirRemain)) goto OK;

  // We want this directory, zip the files...
  dirFiles = GetFiles(theDir);
  for (curIdx=0; curIdx < dirFiles.Length(); curIdx++) {
    status = zip->theFiles.Append(dirFiles.Get(curIdx));
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;
  
} // ...Build.InputWalker

//----------------------------------------------------------------------
//
// Method: Build._Zip - Create and build ZIP/TAR file
//

Status Build::_Zip(UInt zipWhat) { // Build._Zip...

  Status     status;
  String     befe_Build;
  String     befe_SVN;
  ZipContext zip;
  String     outFileName;
  String     cmd;
  UInt       curIdx;
  String     theFile;
  String     theFileName;
  String     theFileDir;
  UInt       curPct;
  UInt       newPct;
  Time       st;
  Time       et;
  
  befe_Build = settings.Get("BEFE_Build");
  if (!Exists(befe_Build)) goto BUILDNOTTHERE;
  befe_SVN = settings.Get("BEFE_SVN");
  if (!Exists(befe_SVN)) goto SVNNOTTHERE;
  
  //
  // Create the initial ZIP
  //
  
  // Get the list of files...
  if (gVerbose)
    Cout << String(' ')*indent << "Locating Files to " << (Is_Windows()?"ZIP":"TAR") << "...";
  zip.baseDir = befe_Build;
  zip.zipDirs = String("bin,doc,home,inc,lib,obj,src").Split(',');
  status = WalkPath(befe_Build, ZipWalker, (UInt32)&zip);
  if (status) goto SOMEERROR;
  if (gVerbose)
    Cout << ' ' << zip.theFiles.Length() << " files to " << (Is_Windows()?"ZIP":"TAR") << '\n';
  if (zip.theFiles.Length() == 0) goto OK;

  // Delete the output file if it already exists...
  outFileName = befe_Build + "/BEFE-" + GetMainVersion() + '.' + GetSubVersion();
  if (Is_Windows())
    outFileName += ".zip";
  else
    outFileName += ".tar";
  if (Exists(outFileName)) {
    status = DeleteFile(outFileName);
    if (status) goto SOMEERROR;
  }

  // Zip the files...
  if (gVerbose)
    Cout << "Populating "
         << (Is_Windows()?"ZIP":"TAR")
         << " file...  0%";
  curPct = 0;
  
  for (curIdx=0; curIdx < zip.theFiles.Length(); curIdx++) {
    
     theFile = zip.theFiles.Get(curIdx);
     
     // Nothing to do if the file doesn't exist anymore...
     if (theFile.Length() < befe_Build.Length() || !Exists(theFile)) continue;
     
     // Build and execute the command
     if (Is_Windows()) {
       cmd = extBinaries.Get("zip")+" -q \"";
       cmd += outFileName;
       cmd += "\" \"";
       cmd += theFile.Get(Span(befe_Build.Length()+1, theFile.Length()));
       cmd += "\"";
     }
     else {
       cmd = extBinaries.Get("tar")+" -rf \"";
       cmd += outFileName;
       cmd += "\" \"";
       cmd += theFile.Get(Span(befe_Build.Length()+1, theFile.Length()));
       cmd += "\"";
     }
     status = _Invoke(cmd, befe_Build);
     if (status) goto SOMEERROR;
     
     // Tell use how far we got...
     newPct = ((curIdx+1)*100)/zip.theFiles.Length();
     if (newPct != curPct && gVerbose && Cout.IsInteractive())
       Cout << String(Char(BS))*4 << ToString(newPct,3) << '%';
     curPct = newPct;
  }
  
  if (gVerbose)
    Cout << String(Char(BS))*4 << " 100%\n";
    
  //
  // Add any files flagged as going in the ZIP root...
  //
  
  if (gVerbose)
    Cout << "Adding supplemental files to "
         << (Is_Windows()?"ZIP":"TAR")
         << " file...  0%";
  
  // Get the list of files...
  PathToGeneric(befe_SVN);
  status = _GetPublishableFiles(befe_SVN, "", zip.theFiles);
  if (status) goto SOMEERROR;
  
  for (curIdx=0; curIdx < zip.theFiles.Length(); curIdx++) {
    
     theFile     = zip.theFiles.Get(curIdx);
     theFileName = FileOnly(theFile);
     theFileDir  = PathOnly(theFile);
     
     // Nothing to do if the file doesn't exist anymore...
     if (theFile.Length() < befe_Build.Length() || !Exists(theFile)) continue;
     
     // Build and execute the command
     if (Is_Windows()) {
       cmd = extBinaries.Get("zip")+" -q \"";
       cmd += outFileName;
       cmd += "\" \"";
       cmd += theFileName;
       cmd += "\"";
     }
     else {
       cmd = extBinaries.Get("tar")+" -rf \"";
       cmd += outFileName;
       cmd += "\" \"";
       cmd += theFileName;
       cmd += "\"";
     }
     status = _Invoke(cmd, theFileDir);
     if (status) goto SOMEERROR;
     
     // Tell use how far we got...
     newPct = ((curIdx+1)*100)/zip.theFiles.Length();
     if (newPct != curPct && gVerbose && Cout.IsInteractive())
       Cout << String(Char(BS))*4 << ToString(newPct,3) << '%';
     curPct = newPct;
  }
  
  if (gVerbose)
    Cout << String(Char(BS))*4 << " 100%\n";
    
  //
  // Now, ZIP it (if creating TAR)...
  //
  
  if (!Is_Windows()) {
    
    if (gVerbose) 
      Cout << "Zipping TAR file...\n";

    cmd = String("gzip \"") + FileOnly(outFileName) + "\"";
    status = _Invoke(cmd, befe_Build);
    if (status) goto SOMEERROR;
  
  }
  
  //
  // Handle errors
  //
  
  status = Error::None;
  while (false) {
    BUILDNOTTHERE: status = Error::UtilBuildBuildNotExist; break;
    SVNNOTTHERE:   status = Error::UtilBuildSVNNotExist;   break;
    OK:            status = Error::None;                   break;
    SOMEERROR:                                             break;
  }
  
  return Error::None;
  
} // ...Build._Zip

} // ...Namespace BEFE