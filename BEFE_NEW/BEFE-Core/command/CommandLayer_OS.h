//!befe-publish inc
//----------------------------------------------------------------------
// File: CommandLayer_OS.h - Declarations for the CommandLayer_OS Class
//----------------------------------------------------------------------
//
// This class is a Command handler for generic Operating System
// command execution.  As such, it's sort of like a genereic "shell".
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef COMMANDLAYER_OS_H // COMMANDLAYER_OS_H...
#define COMMANDLAYER_OS_H

#include "Primitives.h"
#include "CommandLayer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Namespace OSQueryType...
//

namespace OSQueryType {
  enum {
     None
    ,IsDeviceName
    ,IsValidPathName
    ,IsValidFileName
    ,IsFullPath
    ,IsRelativePath
    ,Exists
    ,QueryMax = Exists
  };
} // Namespace OSQueryType

//----------------------------------------------------------------------
//
// Class: CommandLayer_OS - OS Command processor
//

class CommandLayer_OS : public CommandLayer { // Class CommandLayer_OS...

  // C++ Lifecycle
  public: CommandLayer_OS();
  
  // BEFE Lifecycle
  public: virtual Status StartUp();
  
  // Command handler registry
  public: virtual Status RegisterHandlers();

  // Single command execution
  public: virtual Status Enter(BcmdCommand &cmd);

  // Header/Trailer management
  public: virtual Status DoHeader();
  public: virtual Status DoTrailer();

  //
  // Protected members
  //

}; // ...Class CommandLayer_OS

//----------------------------------------------------------------------
//
// CommandLayer_OS Command Handlers...
//

Status CommandLayer_OS_Help(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_pwd(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_cwd(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_ls(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_mkdir(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_rmdir(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_mv(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_cp(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_rm(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_path(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_which(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_cat(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_MD5(CommandLayer *layer, BcmdCommand &cmd);

// Query functions...
Status CommandLayer_OS_IsDeviceName(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_ValidFileName(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_ValidPathName(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_IsFullPath(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_IsRelativePath(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_Exists(CommandLayer *layer, BcmdCommand &cmd);

// Query function workers...
Status CommandLayer_OS_Query(CommandLayer *layer, BcmdCommand &cmd, UInt queryType);

// Analysis...
//  Implemented in CommandLayer_OS_Analyse.cpp
Status CommandLayer_OS_Analyse(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_AnalyseFile_txt(String const &fileName, UInt indent);
Status CommandLayer_OS_AnalyseFile_lang(String const &fileName, UInt indent);

// Encryption...
//  Implemented in CommandLayer_OS_Crypt.cpp
Status CommandLayer_OS_EncryptFile(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_DecryptFile(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_CreateKeyFile(CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_OS_UseKeyFile(CommandLayer *layer, BcmdCommand &cmd);

// Building...
//  Implemented in CommandLayer_OS_Build.cpp
Status CommandLayer_OS_Build(CommandLayer *layer, BcmdCommand &cmd);

} // ...Namespace BEFE

#endif // ...COMMANDLAYER_OS_H