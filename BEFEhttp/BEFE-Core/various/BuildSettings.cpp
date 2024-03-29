//!befe-publish src
//----------------------------------------------------------------------
// File: BuildSettings.cpp - Build Class Settings Methods
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

Status Build::_DefaultSettings() {
  
  Status status;
  String cwd;
  String befe_GIT;
  String befe_Build;
  String befe_Temp;
  
  NEED_TheBefe
  
  status = settings.SetEmpty();
  if (status) goto SOMEERROR;
  
  // Do BEFE_GIT...
  cwd = GetCurrentWorkingDirectory();
  befe_GIT = GetEnvironmentValue("BEFE_GIT");
  if (befe_GIT.Length() == 0) goto NOGIT;
  status = PathToGeneric(befe_GIT);
  if (status) goto SOMEERROR;
  if (befe_GIT.Get(-1) == '/')
    befe_GIT.Remove(-1);
  if (IsRelativePath(befe_GIT))
	  befe_GIT = ApplyRelativePath(befe_GIT);
  if (!Exists(befe_GIT) || !IsDirectory(befe_GIT)) goto BADGIT;
  status = settings.Set("BEFE_GIT",befe_GIT);
  if (status) goto SOMEERROR;

  // Initialise some interesting GIT base directories...
  svnPlatformRoot = befe_GIT + "/platform";
  if (Is_Linux())
    svnPlatformDir = svnPlatformRoot + "/linux";
  else if (Is_WinNT())
    svnPlatformDir = svnPlatformRoot + "/win32";
  else
    svnPlatformDir = "";

  // Do BEFE_Build...
  befe_Build = GetEnvironmentValue("BEFE_Build");
  if (befe_Build.Length() == 0) goto NOBUILD;
  status = PathToGeneric(befe_Build);
  if (status) goto SOMEERROR;
  if (IsRelativePath(befe_Build))
	  befe_Build = ApplyRelativePath(befe_Build);
  if (!Exists(befe_Build) || !IsDirectory(befe_Build)) goto BADBUILD;
  status = settings.Set("BEFE_Build",befe_Build);
  if (status) goto SOMEERROR;

  // Do BEFE_Temp...
  befe_Temp = GetEnvironmentValue("BEFE_Temp");
  if (befe_Temp.Length() == 0) goto TEMPNOTSET;
  status = PathToGeneric(befe_Temp);
  if (status) goto SOMEERROR;
  if (IsRelativePath(befe_Temp))
	  befe_Temp = ApplyRelativePath(befe_Temp);
  if (!IsFullPath(befe_Temp))  goto TEMPNOTPATH;
  if (!Exists(befe_Temp))      goto TEMPNOTEXIST;
  if (!IsDirectory(befe_Temp)) goto TEMPNOTDIR;
  status = settings.Set("BEFE_Temp",befe_Temp);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOGIT:        status = Error::NoBEFE_GIT;            break;
    BADGIT:       status = Error::BadBEFE_GIT;           break;
    NOBUILD:      status = Error::NoBEFE_Build;          break;
    BADBUILD:     status = Error::BadBEFE_Build;         break;
    TEMPNOTSET:   status = Error::UtilBuildTempNotSet;   break;
    TEMPNOTPATH:  status = Error::UtilBuildTempNotPath;  break;
    TEMPNOTEXIST: status = Error::UtilBuildTempNotExist; break;
    TEMPNOTDIR:   status = Error::UtilBuildTempNotDir;   break;
    SOMEERROR:                                           break;
  }
  
  return status;
  
}

Status Build::_CheckDependencies() {
  
  Status  status;
  String  path;
  UInt    curIdx;
  String  found;
  Strings exts;
  Strings deps;
  String  dep;
  
  NEED_TheBefe

  if (gVeryVerbose) 
    Cout << String(' ')*indent << "Checking external dependencies...\n";

  // Initialise dependencies
  status = deps.Append("g++");
  status = deps.Append("ar");
  if (options & OptionGIT)
    status = deps.Append("svn");
  status = deps.Append("soffice");
  if (Is_Windows()) {
    status = deps.Append("zip");
  }
  else if (Is_Linux()) {
    status = deps.Append("tar");
    status = deps.Append("gzip");
  }
  else 
    goto UNKNOWNPORT;
    
  //
  // Make sure BEFE_Temp exists and is usable...
  //
  
  path = settings.Get("BEFE_Temp");
  if (path.Length() == 0) goto TEMPNOTSET;
  status = PathToGeneric(path);
  if (status) goto SOMEERROR;
  if (!IsFullPath(path))  goto TEMPNOTPATH;
  if (!Exists(path))      goto TEMPNOTEXIST;
  if (!IsDirectory(path)) goto TEMPNOTDIR;
  
  //
  // Check for executables...
  //
  
  exts = String(".exe,.cmd,.bat,.befe").Split(',');
  
  status = extBinaries.SetEmpty();
  if (status) goto SOMEERROR;
  
  for (curIdx=0; curIdx < deps.Length(); curIdx++) {
    if (gVeryVerbose)
      Cout << "  Searching for '" << deps.Get(curIdx) << "'...\n";
    dep = deps.Get(curIdx);
    status = FindInSearchPath(dep, exts, found);
    if (status) {
      if (dep != "soffice" && dep != "svn") goto SOMEERROR;
      if (dep == "svn")
        options &= ~OptionGIT;
    }
    else {
      status = extBinaries.Set(deps.Get(curIdx), found);
      if (status) goto SOMEERROR;
    }
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    TEMPNOTSET:   status = Error::UtilBuildTempNotSet;   break;
    TEMPNOTPATH:  status = Error::UtilBuildTempNotPath;  break;
    TEMPNOTEXIST: status = Error::UtilBuildTempNotExist; break;
    TEMPNOTDIR:   status = Error::UtilBuildTempNotDir;   break;
    UNKNOWNPORT:  status = Error::UtilBuildUnknownPort;  break;
    SOMEERROR:                                           break;
  }
  
  return status;
  
}

} // ...Namespace BEFE
