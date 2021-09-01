//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: Build.h - Build Class declaration
//----------------------------------------------------------------------
//
// The Build Class is responsible for building the BEFE product and its
// deliverables on various platforms and flavours.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BUILD_H // BUILD_H...

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...
  
//----------------------------------------------------------------------
//
// Build Class
//

class Build { // Class Build...

  // Build Display Enum...
  public: enum : UInt {
    DisplaySettings             = 0x00000001,
    DisplayExternalBinaries     = 0x00000002,
    DisplayInputHeaders         = 0x00000004,
    DisplayInputSource          = 0x00000008,
    DisplayInputDoc             = 0x00000010,
    DisplayOutputLibraries      = 0x00000020,
    DisplayOutputExecutables    = 0x00000040,
    DisplayPublishedHeaders     = 0x00000080,
    DisplayPublishedSource      = 0x00000100,
    DisplayPublishedLibraries   = 0x00000200,
    DisplayPublishedExecutables = 0x00000400,
    DisplayPublishHomeFiles     = 0x00000800,
    DisplayAll                  = 0x00000FFF,

  };
    
  // Build Steps Enum...
  public: enum : UInt {

    OptionDebug                    = 0x00000001,
    OptionRelease                  = 0x00000002,
    OptionBoth                     = 0x00000003,
    OptionGIT                      = 0x00000004,
    
    GeneralDefaultSettings         = 0x00000008,
    GeneralValidateSettings        = 0x00000010,
    GeneralDetermineFileNames      = 0x00000020,
    GeneralCreateOutputDirectories = 0x00000040,
    GeneralCleanOutputDirectories  = 0x00000080,
    GeneralAll                     = 0x000000F0,
    
    BuildCopyInput                 = 0x00000100,
    BuildCompile                   = 0x00000200,
    BuildLibraries                 = 0x00000400,
    BuildExecutables               = 0x00000800,
    BuildAll                       = 0x00000FF7,
    
    PublishDeps                    = GeneralDefaultSettings         | 
                                     GeneralValidateSettings        |
                                     GeneralCreateOutputDirectories | 
                                     GeneralDetermineFileNames,
    PublishHeaders                 = 0x00001000,
    PublishSource                  = 0x00002000,
    PublishLibraries               = 0x00004000,
    PublishExecutables             = 0x00008000,
    PublishDocumentation           = 0x00010000,
    PublishHomeFiles               = 0x00020000,
    PublishAll                     = 0x0003F000,

    ZipHeaders                     = 0x00100000,
    ZipSource                      = 0x00200000,
    ZipLibraries                   = 0x00400000,
    ZipExecutables                 = 0x00800000,
    ZipDocumentation               = 0x01000000,
    ZipHome                        = 0x02000000,
    ZipAll                         = 0x03F00000,
    
    CleanAll                       = 0xFFFFFFFF,
    CleanRelease                   = 0xFFFFFFFE,
    CleanDebug                     = 0xFFFFFFFC,
		None                           = 0x00000000,
    
  };
  
  // C++ Lifecycle
  public: Build();
  public: Build(Build const &that);
  public: ~Build();
  public: Build &operator=(Build const &that);
  
  // BEFE Lifecycle
  public: Status   StartUp();
  public: Status   ShutDown();
  public: Status   Reset();
  public: Boolean  IsNull() const;
  public: Status   SetNull();
  public: Boolean  IsEmpty() const;
  public: Status   SetEmpty();
  public: Status   MoveFrom(Build const &that);
  public: Status   CopyFrom(Build const &that);
  
  public: UInt     Length () const;
  public: UInt     Size   () const;
  
  public: Build   &Consumable() const;
  public: Build   &NotConsumable() const;
  public: Boolean  IsConsumable() const;

  //
  // Public Methods
  //
 
  public: Status FullBuild(); 
  public: Status Options(UInt options);
  public: Status Steps(UInt steps);
  public: Status Display(UInt info);
 
  //
  // Protected Members
  //

  protected: Boolean      consumable:1;   // "Is this Build consumable?"
  protected: UInt         reserved:31;    // ◄── Reserved for future use
  protected: UInt         indent;         // Output indentation
  protected: NamedStrings settings;       // Current Settings
  protected: UInt         options;        // Current Options

  // Paths...
  protected: String       svnPlatformRoot;// Root Path for platform specific code (e.g., '.../c++/platform/')
  protected: String       svnPlatformDir; // Port Path for platform specific code (e.g., '.../c++/platform/Win32')
 
  // Files
  protected: Strings      headerFiles;    // Header (.h) files
  protected: Strings      sourceFiles;    // Source (.cpp) files
  protected: Strings      dataFiles;      // Data (.befe, .txt, etc) files
  protected: Strings      docFiles;       // Documentation (BEFE*.odt) files
  protected: Strings      homeFiles;      // Home (.txt, etc.) files
  
  // External binary executables
  protected: NamedStrings extBinaries;    // External binaries (name to Full Path mapping)
  
  //
  // Protected Methods
  //
  
  // Static methods
  //   Implemented in BuildFiles.cpp
  protected: static Status InputWalker(String const &dir, Ptr context);
  protected: static Status CleanWalker(String const &dir, Ptr context);
  
  // Display
  //   Implemented in BuildDisplay.cpp
  protected: Status _DisplaySettings();
  protected: Status _DisplayDependencies();
  
  // Settings
  //   Implemented in BuildSettings.cpp
  protected: Status _DefaultSettings();
  protected: Status _ValidateSettings();
  protected: Status _CheckDependencies();

  // Files (both Input and Output files)
  //   Implemented in BuildFiles.cpp
  protected: Status _DetermineFileNames();
  protected: Status _CleanOutputDirectories();
  protected: Status _CreateOutputDirectories();
  protected: Status _CopyInputFiles();
  protected: Status _CompileCppSourceFiles(Boolean debug);
  protected: Status _CreateLibrary(Boolean debug);
  protected: Status _CreateExecutable(Boolean debug);
  protected: Status _PublishHeaders();
  protected: Status _PublishSources();
  protected: Status _PublishReleaseOrDebug(String const &subDir, Boolean debug);
  protected: Status _PublishDocumentation();
  protected: Status _PublishHomeFiles();
  
  // Zip File
  //   Implemented in BuildZip.cpp
  protected: static Status ZipWalker(String const &dir, Ptr context);

  protected: Status _Zip(UInt zipWhat);
  
  // Miscellaneous
  //   Implemented in BuildMisc.cpp
  public: Status _GetPublishableFiles(String const &dirPath, String const &targetType, Strings &files);
  public: Status _PatchGITRevision(String const &fileName);
  
}; // ...Class Build

//----------------------------------------------------------------------
//
// Build Class BEFE Lifecycle...
//

BEFE_INLINE Boolean  IsNull   (Build const &rThis) {return rThis.IsNull();}
BEFE_INLINE Status   SetNull  (Build &rThis)       {return rThis.SetNull();}
BEFE_INLINE Boolean  IsEmpty  (Build const &rThis) {return rThis.IsEmpty();}
BEFE_INLINE Status   SetEmpty (Build &rThis)       {return rThis.SetEmpty();}

} // ...Namespace BEFE

#endif // ...BUILD_H
