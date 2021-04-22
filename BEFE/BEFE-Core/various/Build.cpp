//!befe-publish src
//----------------------------------------------------------------------
// File: Build.cpp - Build Class implementation
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

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Build::Build() {
  StartUp();
}

Build::Build(Build const &that) {
  StartUp();
  CopyFrom(that);
}

Build::~Build() {
  ShutDown();
}

Build &Build::operator=(Build const &that) {
  CopyFrom(that);
  NotConsumable();
  return *this;
}
  
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Build::StartUp() {
  
  Status retStatus;
  Status status;
  
  retStatus = Error::None;
  
  consumable = false;
  indent     = 0;
  status = settings.StartUp();
  if (status && !retStatus) retStatus = status;
  options = 0;
  status = svnPlatformRoot.StartUp();
  options = 0;
  status = svnPlatformDir.StartUp();
  if (status && !retStatus) retStatus = status;
  status = headerFiles.StartUp();
  if (status && !retStatus) retStatus = status;
  status = sourceFiles.StartUp();
  if (status && !retStatus) retStatus = status;
  status = dataFiles.StartUp();
  if (status && !retStatus) retStatus = status;
  status = docFiles.StartUp();
  if (status && !retStatus) retStatus = status;
  status = homeFiles.StartUp();
  if (status && !retStatus) retStatus = status;
  status = extBinaries.StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Status Build::ShutDown() {

  Status retStatus;
  Status status;
  
  retStatus = Error::None;
  status = svnPlatformRoot.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = svnPlatformRoot.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = svnPlatformDir.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = settings.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = headerFiles.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = sourceFiles.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = dataFiles.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = docFiles.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = homeFiles.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = extBinaries.ShutDown();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Status Build::Reset() {

  Status retStatus;
  Status status;
  
  retStatus = Error::None;
  status = ShutDown();
  if (status && !retStatus) retStatus = status;
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Boolean Build::IsNull() const {
  return BEFE::IsEmpty(settings);
}

Status Build::SetNull() {
  return Reset();
}

Boolean Build::IsEmpty() const {
  return BEFE::IsEmpty(settings);
}

Status Build::SetEmpty() {

  Status retStatus;
  Status status;
  
  retStatus = Error::None;
  status = svnPlatformRoot.SetEmpty();
  if (status && !retStatus) retStatus = status;
  status = svnPlatformDir.SetEmpty();
  if (status && !retStatus) retStatus = status;  
  status = settings.SetEmpty();
  if (status && !retStatus) retStatus = status;
  status = headerFiles.SetEmpty();
  if (status && !retStatus) retStatus = status;
  status = sourceFiles.SetEmpty();
  if (status && !retStatus) retStatus = status;
  status = dataFiles.SetEmpty();
  if (status && !retStatus) retStatus = status;
  status = docFiles.SetEmpty();
  if (status && !retStatus) retStatus = status;
  status = homeFiles.SetEmpty();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Status Build::MoveFrom(Build const &that) {
  
  Status retStatus;
  Status status;
  
  retStatus = Error::None;
  if (this != &that) {
    status = ShutDown();
    if (status && !retStatus) retStatus = status;
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(Build));
    status = ((Build *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
  }
  
  return retStatus;
  
}

Status Build::CopyFrom(Build const &that) {
  
  Status retStatus;
  Status status;
  
  retStatus = Error::None;
  if (this != &that) {
    if (IsConsumable())
      retStatus = MoveFrom(that);
    else {
      status = Reset();
      if (status) goto SOMEERROR;
      status = settings.CopyFrom(that.settings);
      if (status) goto SOMEERROR;
      status = headerFiles.CopyFrom(that.headerFiles);
      if (status) goto SOMEERROR;
      status = sourceFiles.CopyFrom(that.sourceFiles);
      if (status) goto SOMEERROR;
      status = dataFiles.CopyFrom(that.dataFiles);
      if (status) goto SOMEERROR;
    }
  }
    
  // Handle errors
  while (false) {
    SOMEERROR: if (status && !retStatus) retStatus = status; break;
  }
  
  return retStatus;
  
}
  
UInt Build::Length() const {
  return 0;
}

UInt Build::Size() const {
  return 0;
}
  
Build &Build::Consumable() const {
  ((Build *)this)->consumable = true;
  return *((Build *)this);
}

Build &Build::NotConsumable() const {
  ((Build *)this)->consumable = false;
  return *((Build *)this);
}

Boolean Build::IsConsumable() const {
  return consumable;
}

//----------------------------------------------------------------------
//
// Public Methods
//
  
Status Build::FullBuild() { // Build.FullBuild...
  
  Status status;
  Time   start;
  Time   end;

  NEED_TheBefe
  NEED_TheOS
  NEED_TheProcess
  NEED_ThePlatform
  
  Cout << String(' ')*indent << "***Build started...***\n\n";
  
  // Initialise
  indent = 2;
  start.Set();

  status = Options(OptionRelease | OptionDebug);
  if (status) goto SOMEERROR;
  status = Steps(GeneralDefaultSettings  |
                 GeneralValidateSettings |
                 GeneralDetermineFileNames);
  if (status) goto SOMEERROR;

  // Display File Names...
  if (gVerbose) {
    Cout << String(' ')*indent << "Header Files: " << headerFiles.Length() << '\n';
    Cout << String(' ')*indent << "Source Files: " << sourceFiles.Length() << '\n';
    Cout << String(' ')*indent << "Data   Files: " << dataFiles.Length()   << '\n';
    Cout << String(' ')*indent << "Doc    Files: " << docFiles.Length()    << '\n';
    Cout << String(' ')*indent << "Home   Files: " << homeFiles.Length()   << '\n';
  }

  // Do the rest of the steps...
  status = Steps(
    GeneralCreateOutputDirectories |
    GeneralCleanOutputDirectories  |
    BuildCopyInput                 |
    BuildCompile                   |
    BuildLibraries                 |
    BuildExecutables               |
    PublishHeaders                 |
    PublishSource                  |
    PublishLibraries               |
    PublishExecutables             |
    PublishDocumentation           |
    0);
  if (status) goto SOMEERROR;
  
  // Finish up
  end.Set();
  indent = 0;
  Cout << '\n';
  Cout << String(' ')*indent << "***Build Finished";
  if (gShowElapsed)
    Cout << " (Elapsed Time: " << ToString(start, end) << ") ";
  Cout << "***\n";
    
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
} // ...Build.FullBuild

Status Build::Options(UInt pOptions) { // Build.Options...
  options = pOptions;
  return Error::None;
} // ...Build.Options

Status Build::Steps(UInt steps) { // Build.Steps...

  Status status;
  
  status = Error::None;

  if (steps & GeneralDefaultSettings) {
    status = _DefaultSettings();
    if (status) goto SOMEERROR;
  }
  
  if (steps & GeneralValidateSettings) {
    status = _CheckDependencies();
    if (status) goto SOMEERROR;
  }
  
  if (steps & GeneralDetermineFileNames) {
    status = _DetermineFileNames();
    if (status) goto SOMEERROR;
  }
  
  if (steps & GeneralCreateOutputDirectories) {
    status = _CreateOutputDirectories();
    if (status) goto SOMEERROR;
  }
  
  if (steps & GeneralCleanOutputDirectories) {
    status = _CleanOutputDirectories();
    if (status) goto SOMEERROR;
  }
  
  if ((steps & BuildCopyInput) && (options & OptionGIT)) {
    status = _CopyInputFiles();
    if (status) goto SOMEERROR;
  }
  
  if (steps & BuildCompile) {
    if (options & OptionRelease) {
      status = _CompileCppSourceFiles(false);
      if (status) goto SOMEERROR;
    }
    if (options & OptionDebug) {
      status = _CompileCppSourceFiles(true);
      if (status) goto SOMEERROR;
    }
  }
  
  if (steps & BuildLibraries) {
    if (options & OptionRelease) {
      status = _CreateLibrary(false);
      if (status) goto SOMEERROR;
    }
    if (options & OptionDebug) {
      status = _CreateLibrary(true);
      if (status) goto SOMEERROR;
    }
  }
  
  if (steps & BuildExecutables) {
    if (options & OptionRelease) {
      status = _CreateExecutable(false);
      if (status) goto SOMEERROR;
    }
    if (options & OptionDebug) {
      status = _CreateExecutable(true);
      if (status) goto SOMEERROR;
    }
  }
  
  if (steps & PublishHeaders) {
    status = _PublishHeaders();
    if (status) goto SOMEERROR;
  }
  
  if (steps & PublishSource) {
    status = _PublishSources();
    if (status) goto SOMEERROR;
  }
  
  if (steps & PublishLibraries) {
    if (options & OptionRelease) {
      status = _PublishReleaseOrDebug("lib",false);
      if (status) goto SOMEERROR;
    }
    if (options & OptionDebug) {
      status = _PublishReleaseOrDebug("lib",true);
      if (status) goto SOMEERROR;
    }
  }
  if (steps & PublishExecutables) {
    if (options & OptionRelease) {
      status = _PublishReleaseOrDebug("bin",false);
      if (status) goto SOMEERROR;
    }
    if (options & OptionDebug) {
      status = _PublishReleaseOrDebug("bin",true);
      if (status) goto SOMEERROR;
    }
  }
  if (steps & PublishDocumentation) {
    status = _PublishDocumentation();
    if (status) goto SOMEERROR;
  }
  if (steps & PublishHomeFiles) {
    status = _PublishHomeFiles();
    if (status) goto SOMEERROR;
  }
    
  // Finally, do any zipping requested...
  if (steps & ZipAll) {
    status = _Zip(steps);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
} // ...Build.Steps

Status Build::Display(UInt info) { // Build.Display...
  return Error::NotImplemented;
} // ...Build.Display

} // ...Namespace BEFE
