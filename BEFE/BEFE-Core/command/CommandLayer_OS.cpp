//----------------------------------------------------------------------
// File: CommandLayer_OS.cpp - Implementaion of the CommandLayer_OS Class
//----------------------------------------------------------------------
//
// The Command class is responsible for parsing and executing BEFE
// OS command lines.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Error.h"
#include "CommandShell.h"
#include "CommandLayer.h"
#include "CommandLayer_BEFE.h"
#include "CommandLayer_OS.h"
#include "Commands.h"
#include "cmdutils.h"
#include "ToString.h"
#include "ToStrings.h"
#include "Help.h"
#include "MD5.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Command Specifications
//

static CommandSpec commandSpecs[] = {
  // Exit...
   {"e[xit]"                          , (CommandHandler)CommandLayer_BEFE_Exit            , true}
  ,{"x"                               , (CommandHandler)CommandLayer_BEFE_Exit            , true}
  ,{"q[uit]"                          , (CommandHandler)CommandLayer_BEFE_Exit            , true}
  // Finished...
  ,{".."                              , (CommandHandler)CommandLayer_BEFE_Finish          , true}
  ,{"done"                            , (CommandHandler)CommandLayer_BEFE_Finish          , true}
  // Help...
  ,{"h[elp]"                          , (CommandHandler)CommandLayer_OS_Help          , true}
  ,{"?"                               , (CommandHandler)CommandLayer_OS_Help          , true}
  // Switches...
  ,{"Sw[itch[es]]"                    , (CommandHandler)CommandLayer_BEFE_Switches        , true}
  ,{"Arg[ument][s]"                   , (CommandHandler)CommandLayer_BEFE_Switches        , true}
  ,{"Opt[ion][s]"                     , (CommandHandler)CommandLayer_BEFE_Switches        , true}
  ,{"S[how]A[rg[ument][s]]"           , (CommandHandler)CommandLayer_BEFE_ShowArgs        , true}
  ,{"S[how]Sw[itch[es]]"              , (CommandHandler)CommandLayer_BEFE_ShowArgs        , true}
  ,{"S[how]Opt[ion][s]"               , (CommandHandler)CommandLayer_BEFE_ShowArgs        , true}
  // Date/Time...
  ,{"Date"                            , (CommandHandler)CommandLayer_BEFE_DateTime        , true}
  ,{"Time"                            , (CommandHandler)CommandLayer_BEFE_DateTime        , true}
  // Environment Variables...
  ,{"set"                             , (CommandHandler)CommandLayer_BEFE_Set             , true}
  ,{"env[iron]"                       , (CommandHandler)CommandLayer_BEFE_Set             , true}  
  ,{"r[ef[resh]]env[iron]"            , (CommandHandler)CommandLayer_BEFE_RefreshEnv      , true}
  // Run
  ,{"Run"                             , (CommandHandler)CommandLayer_BEFE_Run             , true}
  ,{"Exec[ute]"                       , (CommandHandler)CommandLayer_BEFE_Run             , true}
  
  // Directory stuff...
  ,{"pwd"                             , (CommandHandler)CommandLayer_OS_pwd           , true}
  ,{"cwd"                             , (CommandHandler)CommandLayer_OS_cwd           , true}
  ,{"cd"                              , (CommandHandler)CommandLayer_OS_cwd           , true}
  ,{"dir"                             , (CommandHandler)CommandLayer_OS_ls            , true}
  ,{"ls"                              , (CommandHandler)CommandLayer_OS_ls            , true}
  ,{"mkdir"                           , (CommandHandler)CommandLayer_OS_mkdir         , true}
  ,{"rmdir"                           , (CommandHandler)CommandLayer_OS_rmdir         , true}
  ,{"m[o]v[e]"                        , (CommandHandler)CommandLayer_OS_mv            , true}
  ,{"ren[ame]"                        , (CommandHandler)CommandLayer_OS_mv            , true}
  ,{"c[o]p[y]"                        , (CommandHandler)CommandLayer_OS_cp            , true}
  ,{"r[e]m[ove]"                      , (CommandHandler)CommandLayer_OS_rm            , true}
  ,{"del[ete]"                        , (CommandHandler)CommandLayer_OS_rm            , true}
  ,{"path"                            , (CommandHandler)CommandLayer_OS_path          , true}
  ,{"which"                           , (CommandHandler)CommandLayer_OS_which         , true}
  ,{"cat"                             , (CommandHandler)CommandLayer_OS_cat           , true}
  ,{"type"                            , (CommandHandler)CommandLayer_OS_cat           , true}
  ,{"md5"                             , (CommandHandler)CommandLayer_OS_MD5           , true}
  
  // Validation stuff...
  ,{"[Is]Dev[ice][N[ame]]"            , (CommandHandler)CommandLayer_OS_IsDeviceName  , true}
  ,{"[Is]V[alid]F[ile][N[ame]]"       , (CommandHandler)CommandLayer_OS_ValidFileName , true}
  ,{"[Is]V[alid]P[ath][N[ame]]"       , (CommandHandler)CommandLayer_OS_ValidPathName , true}
  ,{"[Is]V[alid]D[ir[ectory]][N[ame]]", (CommandHandler)CommandLayer_OS_ValidPathName , true}
  ,{"I[s]F[ull]P[ath][N[ame]]"        , (CommandHandler)CommandLayer_OS_IsFullPath    , true}
  ,{"I[s]R[el[ative]]P[ath][N[ame]]"  , (CommandHandler)CommandLayer_OS_IsRelativePath, true}
  ,{"Ex[ist[s]]"                      , (CommandHandler)CommandLayer_OS_Exists        , true}

  // Analyse...
  ,{"Anal[yse]"                       , (CommandHandler)Command_Analyse       , true}

  // Encryption...
  ,{"O[bfuscate]F[ile]"               , (CommandHandler)Command_EncryptFile   , true}
  ,{"Cl[arify]F[ile]"                 , (CommandHandler)Command_DecryptFile   , true}
  ,{"Cr[eate]K[ey]F[ile]"             , (CommandHandler)Command_CreateKeyFile , true}
  ,{"U[se]K[ey]F[ile]"                , (CommandHandler)Command_UseKeyFile    , true}

  ,{"Enc[rypt][F[ile]]"               , (CommandHandler)Command_EncryptFile   , true}
  ,{"Dec[rypt][F[ile]]"               , (CommandHandler)Command_DecryptFile   , true}
  
  // For now, Build...
  ,{"B[ui]ld"                         , (CommandHandler)Command_Build         , true}
  
  ,{NULL,NULL,true}
  
};

//----------------------------------------------------------------------
//
// C++ Lifecycle...
//

CommandLayer_OS::CommandLayer_OS() {  
  StartUp();  
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle...
//

Status CommandLayer_OS::StartUp() {
  
  Status status;
  
  status = CommandLayer::StartUp();
  name = "OS";
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Command Handler Registry
//

Status CommandLayer_OS::RegisterHandlers() {

  Status status;

  status = CommandLayer::RegisterHandlers(commandSpecs);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR: break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Command Execution
//

Status CommandLayer_OS::Enter(BcmdCommand &cmd) {
  
  SetPrompt(GetCurrentWorkingDirectory());
  
  return Error::None;
  
}

//----------------------------------------------------------------------
//
// Header/Trailer management
//

Status CommandLayer_OS::DoHeader() {

  if (GetShell() && GetShell()->IsInteractive()) {
    Cout << '\n';
    //Cout << indent << "Generic OS Command Shell...\n";
    //Cout << '\n';
  }

  return Error::None;

}

Status CommandLayer_OS::DoTrailer() {

  if (GetShell() && GetShell()->IsInteractive() && !GetShell()->IsExiting()) {
    Cout << '\n';
    //Cout << indent << "OS Shell finished...\n";
    //Cout << '\n';
  }

  return Error::None;
}

//----------------------------------------------------------------------
//
// Function: CommandLayer_OS_Help     - Handle 'help' command
//

Status CommandLayer_OS_Help(CommandLayer *layer, BcmdCommand &cmd) {

  Status  status;
  UInt    argNo;
  
  // If no arguments...
  if (cmd.numArgs < 2) {
    status = Help("OS");
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // For each argument...
  for (argNo=1; argNo < cmd.numArgs; argNo++) {
    status = Help(cmd[argNo]);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Functions: CommandLayer_OS_pwd   - Handle 'pwd' command
//            CommandLayer_OS_cwd   - Handle 'cwd' command
//            CommandLayer_OS_ls    - Handle 'ls' command
//            CommandLayer_OS_mkdir - Handle 'mkdir' command
//            CommandLayer_OS_rmdir - Handle 'rmdir' command
//            CommandLayer_OS_mv    - Handle 'mv' command
//            CommandLayer_OS_path  - Handle 'path' command
//            CommandLayer_OS_which - Handle 'which' command
//            CommandLayer_OS_cat   - Handle 'cat' command
//            CommandLayer_OS_MD5   - Handle 'md5' command
//

Status CommandLayer_OS_pwd(CommandLayer *layer, BcmdCommand &cmd) {

  Status status;
  String curDir;
  
  status = GetCurrentWorkingDirectory(curDir);
  if (status) goto SOMEERROR;
  Cout << layer->indent << CmdStringMessage(GetMessage(Error::CmdOS_CurDir), curDir) << '\n';
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

Status CommandLayer_OS_cwd(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           newcwd;
  Char             pathsep;
  
  status = Error::None;

  // Handle bad cases
  if (cmd.numArgs != 2) {
     status = Error::CmdOS_ExpectedWorkDir;
     if (cmd.numArgs == 1)
       status = cmd.SetError(0, status);
     else if (cmd.numArgs > 2)
       status = cmd.SetError(2, status);
     else
       goto INTERNAL;
     goto SOMEERROR;
  }
  newcwd = cmd[1];
  if (newcwd.Length() <= 0) {
    status = Error::CmdOS_ExpectedWorkDir;
    status = cmd.SetError(1, status);
    goto SOMEERROR;
  }

  // If relative path given, form full path from it...
  if (IsRelativePath(newcwd)) {
    status = ApplyRelativePath(GetCurrentWorkingDirectory(),newcwd,newcwd);
    if (status) {
      status = Error::CmdOS_ApplyRelativePath;
      status = cmd.SetError(1, status, CmdStatusMessage(status, status));
      goto SOMEERROR;
    }
  }
  if (!IsFullPath(newcwd)) {
    status = Error::CmdOS_DirNotFullOrRelative;
    status = cmd.SetError(1, status, CmdStringMessage(status, newcwd));
    goto SOMEERROR;
  }

  // Make sure it exists
  if (!Exists(newcwd)) {
    status = Error::CmdOS_DirNotExist;
    status = cmd.SetError(1, status, CmdStringMessage(status, newcwd));
    goto SOMEERROR;
  }
  if (!IsDirectory(newcwd)) {
    status = Error::CmdOS_DirNotDir;
    status = cmd.SetError(1, status, CmdStringMessage(status, newcwd));
    goto SOMEERROR;
  }

  // Tell the OS to make it the current working directory
  status = SetCurrentWorkingDirectory(newcwd);
  if (status) goto SOMEERROR;
  status = GetCurrentWorkingDirectory(newcwd);
  if (status) goto SOMEERROR;

  // Set it
  layer->SetPrompt(newcwd);

  // Handle errors
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }

  return status;

}

Status CommandLayer_OS_ls(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           path;
  Strings          dirs;
  Strings          files;
  FileInfos        fileInfos;
  FileInfo         fileInfo;
  UInt             numFiles;
  Strings          lines;
  String           str;
  Boolean          showLong;
  UInt             maxNameLen;
  UInt             maxSize;
  UInt             i;
  
  // Handle 2nd argument '-l'...
  if (cmd[1] == "-l") {
    showLong = true;
    status = cmd.args.Remove(1);
    if (status) goto SOMEERROR;
    cmd.numArgs--;
  }
  else
    showLong = false;

  // Make sure there's at least one and no more than two...
  // Handle bad cases
  if (cmd.numArgs > 2) {
     status = cmd.SetError(2, Error::CmdOS_ls_OnlyOneArg);
     goto SOMEERROR;
  }

  // Get the full path
  if (cmd.numArgs <= 1)
    path = GetCurrentWorkingDirectory();
  else {
    path = cmd[1];
    status = PathToGeneric(path);
    if (status) goto SOMEERROR;
    if (!IsFullPath(path)) {
      if (!IsRelativePath(path)) {
        status = path.Insert(0, "./");
        if (status) goto SOMEERROR;
      }
      path = ApplyRelativePath(GetCurrentWorkingDirectory(),path);
    }
  }

  // Make sure it exists
  if (!Exists(path)) {
    status = Error::CmdOS_DirNotExist;
    status = cmd.SetError(1, status, CmdStringMessage(status, cmd[1]).Consumable());
    goto SOMEERROR;
  }

  //
  // If it's a file...
  //

  if (IsFile(path)) {

    status = GetFileInfo(path, fileInfo);
    if (status) goto SOMEERROR;
    status = ToStrings(fileInfo, lines);
    if (status) goto SOMEERROR;
    Cout << layer->indent << "File '" << cmd[1] << "' ...\n\n";
    for (i=0; i < lines.Length(); i++) {
      str = lines.Get(i);
      Cout << layer->indent << "  " << str << '\n';
    }

    goto OK;

  }

  //
  // Else, it's a directory...
  //

  dirs = GetDirectories(path);
  dirs.SortInsensitive();
  
  // Get fileInfos or files...
  if (showLong) {
    status = GetFilesInfo(path, fileInfos);
    if (status) goto SOMEERROR;
    numFiles = fileInfos.Length();
  }
  else {
    files = GetFiles(path);
    files.SortInsensitive();
    numFiles = files.Length();
  }
  
  // Handle case where no files and no directories
  if (dirs.Length() == 0 && numFiles == 0) {
    Cout << layer->indent << CmdStringMessage(GetMessage(Error::CmdOS_ls_DirIsEmpty), path) << '\n';
    goto OK;
  }

  // Display directories
  if (dirs.Length() != 0) {
    Cout << layer->indent << GetMessage(Error::CmdOS_ls_Directories) << "\n\n";
    for (i=0;i<dirs.Length();i++) {
      str = dirs.Get(i);
      Cout << layer->indent << "  " << str << '\n';
    }
    Cout << '\n';
  }
  else
    Cout << layer->indent << GetMessage(Error::CmdOS_ls_DirectoriesNone) << "\n\n";

  // Quick whip-through to get some max stuff of fileInfos...
  maxNameLen = 0;
  maxSize    = 0;
  if (showLong) {
    for (i=0; i < numFiles; i++) {
      status = fileInfos.Get(i, fileInfo);
      if (status) goto SOMEERROR;
      maxNameLen = Max(maxNameLen, FileOnly(fileInfo.name).Length());
      maxSize    = Max(maxSize, (UInt)fileInfo.size);
    }
  }
  
  // Display files
  if (numFiles != 0) {

    Cout << layer->indent << GetMessage(Error::CmdOS_ls_Files) << "\n\n";
    
    // Short form...
    if (!showLong) {
      for (i=0; i < files.Length(); i++) {
        str = FileOnly(files.Get(i));
        Cout << layer->indent << "  " << str << '\n';
      }
    }
    
    // Long form...
    if (showLong) {
      for (i=0; i < fileInfos.Length(); i++) {
        status = fileInfos.Get(i, fileInfo);
        if (status) goto SOMEERROR;
        str = fileInfo.name;
        str = FileOnly(str);
        Cout << layer->indent << "  "
             << str.RightPad(maxNameLen)
             << String(' ')*(IntLog10(maxSize)-IntLog10((UInt)fileInfo.size) + 1)
                << (UInt)fileInfo.size
             << '\n';
      }
    }
    
  }
  else
    Cout << layer->indent << GetMessage(Error::CmdOS_ls_FilesNone) << '\n';

  // Handle errors...
  status = Error::None;
  while (false) {
    OK:          status = Error::None;                 break;
    SOMEERROR:                                         break;
  }

  //Cout << '\n';

  return status;

}

Status CommandLayer_OS_mkdir(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           newDir;
  
  status = Error::None;

  // Handle bad cases
  if (cmd.numArgs != 2) {
     status = Error::CmdOS_mkdir_ExpectedDir;
     if (cmd.numArgs == 1)
       status = cmd.SetError(0, status);
     else if (cmd.numArgs > 2)
       status = cmd.SetError(2, status);
     else
       goto INTERNAL;
     goto SOMEERROR;
  }
  newDir = cmd[1];
  status = PathToGeneric(newDir);
  if (status) goto SOMEERROR;
  if (newDir.Length() <= 0) {
    status = Error::CmdOS_mkdir_ExpectedDir;
    status = cmd.SetError(1, status);
    goto SOMEERROR;
  }

  // If relative path given, form full path from it...
  if (!IsFullPath(newDir)) {
    if (!IsRelativePath(newDir)) {
      status = newDir.Insert(0, "./");
      if (status) goto SOMEERROR;
    }
    status = ApplyRelativePath(GetCurrentWorkingDirectory(),newDir,newDir);
    if (status) {
      status = Error::CmdOS_ApplyRelativePath;
      status = cmd.SetError(1, status, CmdStatusMessage(status, status));
      goto SOMEERROR;
    }
  }

  // Make sure it doesn't already exist
  if (Exists(newDir)) {
    status = Error::CmdOS_DirExists;
    status = cmd.SetError(1, status, CmdStringMessage(status, newDir));
    goto SOMEERROR;
  }

  // Tell the OS to make it
  status = CreateDirectories(newDir);
  if (status) {
    status = cmd.SetError(1, status);
    goto SOMEERROR;
  }

  // Handle errors
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }

  return status;

}

Status CommandLayer_OS_rmdir(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           theDir;
  
  status = Error::None;

  // Handle bad cases
  if (cmd.numArgs != 2) {
     status = Error::CmdOS_rmdir_ExpectedDir;
     if (cmd.numArgs == 1)
       status = cmd.SetError(0, status);
     else if (cmd.numArgs > 2)
       status = cmd.SetError(2, status);
     else
       goto INTERNAL;
     goto SOMEERROR;
  }
  theDir = cmd[1];
  if (theDir.Length() <= 0) {
    status = Error::CmdOS_rmdir_ExpectedDir;
    status = cmd.SetError(1, status);
    goto SOMEERROR;
  }

  // If relative path given, form full path from it...
  if (IsRelativePath(theDir)) {
    status = ApplyRelativePath(GetCurrentWorkingDirectory(),theDir,theDir);
    if (status) {
      status = Error::CmdOS_ApplyRelativePath;
      status = cmd.SetError(1, status, CmdStatusMessage(status, status));
      goto SOMEERROR;
    }
  }
  if (!IsFullPath(theDir)) {
    status = Error::CmdOS_DirNotFullOrRelative;
    status = cmd.SetError(1, status, CmdStringMessage(status, theDir));
    goto SOMEERROR;
  }

  // Make sure it exists and is a directory
  if (!Exists(theDir)) {
    status = Error::CmdOS_DirNotExist;
    status = cmd.SetError(1, status, CmdStringMessage(status, theDir));
    goto SOMEERROR;
  }
  if (!IsDirectory(theDir)) {
    status = Error::CmdOS_DirNotDir;
    status = cmd.SetError(1, status, CmdStringMessage(status, theDir));
    goto SOMEERROR;
  }

  // Tell the OS to remove it
  status = DeleteDirectory(theDir);
  if (status) {
    status = cmd.SetError(1, status);
    goto SOMEERROR;
  }

  // Handle errors
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }

  return status;

}

Status CommandLayer_OS_mv(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           cwd;
  String           oldPath;
  String           newPath;

  // Make sure there's two and only two arguments...
  if (cmd.numArgs != 3) {
    if (cmd.numArgs == 0) goto INTERNAL;
    status = cmd.SetError(cmd.numArgs-1, Error::CmdOS_mv_Expected2);
    goto SOMEERROR;
  }

  // Get <old> and <new> into oldPath and newPath...
  oldPath = cmd[1];
  newPath = cmd[2];
  cwd = GetCurrentWorkingDirectory();
  if (!IsFullPath(oldPath))
    oldPath = ApplyRelativePath(cwd, oldPath);
  if (!IsFullPath(newPath))
    newPath = ApplyRelativePath(cwd, newPath);  

  // Make sure source exists and destination doesn't...
  if (!Exists(oldPath)) {
    status = Error::CmdOS_mv_SrcNotExist;
    status = cmd.SetError(1, status, CmdStringMessage(status, oldPath));
    goto SOMEERROR;
  }
  if (Exists(newPath)) {
    status = Error::CmdOS_mv_DstDoesExist;
    status = cmd.SetError(1, status, CmdStringMessage(status, newPath));
    goto SOMEERROR;
  }
  
  // Do it...
  if (IsFile(oldPath))
    status = MoveFile(oldPath, newPath);
  else
    status = MoveDirectory(oldPath, newPath);
  if (status) goto SOMEERROR;
  
  // Handle errors...
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }

  //Cout << '\n';

  return status;

}

Status CommandLayer_OS_cp(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           cwd;
  String           oldPath;
  String           newPath;

  // Make sure there's two and only two arguments...
  if (cmd.numArgs != 3) {
    if (cmd.numArgs == 0) goto INTERNAL;
    status = cmd.SetError(cmd.numArgs-1, Error::CmdOS_cp_Expected2);
    goto SOMEERROR;
  }

  // Get <old> and <new> into oldPath and newPath...
  oldPath = cmd[1];
  newPath = cmd[2];
  cwd = GetCurrentWorkingDirectory();
  if (!IsFullPath(oldPath))
    oldPath = ApplyRelativePath(cwd, oldPath);
  if (!IsFullPath(newPath))
    newPath = ApplyRelativePath(cwd, newPath);  

  // Make sure source exists and destination doesn't...
  if (!Exists(oldPath)) {
    status = Error::CmdOS_cp_SrcNotExist;
    status = cmd.SetError(1, status, CmdStringMessage(status, oldPath));
    goto SOMEERROR;
  }
  if (Exists(newPath)) {
    status = Error::CmdOS_cp_DstDoesExist;
    status = cmd.SetError(1, status, CmdStringMessage(status, newPath));
    goto SOMEERROR;
  }
  
  // Do it...
  if (IsDirectory(oldPath)) {
    status = cmd.SetError(1, Error::CmdOS_cp_DirNotImplemented);
    goto SOMEERROR;
  }
  status = CopyFile(oldPath, newPath);
  if (status) goto SOMEERROR;
  
  // Handle errors...
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }

  //Cout << '\n';

  return status;

}

Status CommandLayer_OS_rm(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           cwd;
  String           theFile;

  // Make sure there's one and only one argument...
  if (cmd.numArgs != 2) {
    if (cmd.numArgs == 0) goto INTERNAL;
    status = cmd.SetError(cmd.numArgs-1, Error::CmdOS_rm_Expected1);
    goto SOMEERROR;
  }


  // Get theFile...
  theFile = cmd[1];
  cwd = GetCurrentWorkingDirectory();
  if (!IsFullPath(theFile))
    theFile = ApplyRelativePath(cwd, theFile);

  // Make sure source exists and destination doesn't...
  if (!Exists(theFile)) {
    status = Error::CmdOS_rm_NotExist;
    status = cmd.SetError(1, status, CmdStringMessage(status, theFile));
    goto SOMEERROR;
  }
  if (!IsFile(theFile)) {
    status = Error::CmdOS_rm_NotFile;
    status = cmd.SetError(1, status, CmdStringMessage(status, theFile));
    goto SOMEERROR;
  }
  
  // Do it...
  status = DeleteFile(theFile);
  if (status) goto SOMEERROR;
  
  // Handle errors...
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }

  //Cout << '\n';

  return status;

}

Status CommandLayer_OS_path(CommandLayer *layer, BcmdCommand &cmd) {

  Status  status;
  String  str;
  UInt    i;
  Strings parts;
  UInt    numParts;

  // Handle bad cases
  if (cmd.numArgs != 1) {
    if (cmd.numArgs == 0) goto INTERNAL;
    status = cmd.SetError(cmd.numArgs-1, Error::CmdOS_path_Expected0);
    goto SOMEERROR;
  }

  // Get and print them
  parts = TheBefe->TheOS->GetSearchPaths();
  numParts = parts.Length();
  if (numParts == 0) {
    status = cmd.SetError(0, Error::CmdOS_path_NoPath);
    goto SOMEERROR;
  }
  Cout << '\n';
  for (i=0; i < numParts; i++) {
    str = parts.Get(i);
    Cout << layer->indent << str << '\n';
  }
  if (numParts)
    Cout << '\n';
    
  // Handle errors...
  status = Error::None;
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }

  return status;

}

Status CommandLayer_OS_which(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  Strings          parts;
  String           fName;
  String           found;
  
  // Handle bad cases
  if (cmd.numArgs != 2) {
    if (cmd.numArgs == 0) goto INTERNAL;
    status = cmd.SetError(cmd.numArgs-1, Error::CmdOS_which_Expected1);
    goto SOMEERROR;
  }

  // Get the file name
  fName = cmd[1];

  // Get the extensions...
  parts = String(".bef,.befe,.exe,.cmd,.sh,.bsh,.cpp,.h.,.c,.py,.pyc,.class,.java,.o,.a,.dll").SplitToStrings(',');

  // Find it
  found = TheBefe->TheOS->FindInSearchPath(fName,parts);
  if (found.Length() == 0) {
    status = Error::CmdOS_which_NotFound;
    status = cmd.SetError(1, status, CmdStringMessage(status, fName));
    goto SOMEERROR;
  }

  // Display it
  Cout << layer->indent << CmdStringMessage(GetMessage(Error::CmdOS_which_Found), found) << '\n';
  
  // Handle errors
  status = Error::None;
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }

  return status;

}

Status CommandLayer_OS_cat(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           fName;
  FileStream       file;
  Byte             buf[4096];
  UInt             numRead;
  UInt             numWritten;
  
  // Handle bad cases
  if (cmd.numArgs != 2) {
    if (cmd.numArgs == 0) goto INTERNAL;
    status = cmd.SetError(cmd.numArgs-1, Error::CmdOS_cat_Expected1);
    goto SOMEERROR;
  }

  // Get the file name
  fName = cmd[1];
  if (IsEmpty(fName)) {
    status = Error::CmdOS_cat_EmptyFileName;
    status = cmd.SetError(1, status, CmdStringMessage(status, fName));
    goto SOMEERROR;
  };
  fName = ApplyRelativePath(fName);
  if (!Exists(fName)) {
    status = Error::CmdOS_cat_NotExist;
    status = cmd.SetError(1, status, CmdStringMessage(status, fName));
    goto SOMEERROR;
  }
  if (!IsFile(fName)) {
    status = Error::CmdOS_cat_NotFile;
    status = cmd.SetError(1, status, CmdStringMessage(status, fName));
    goto SOMEERROR;
  }

  // Display it
  status = file.SetName(fName);
  if (status) goto SOMEERROR;
  do {
    status = file.GetBytes(buf, sizeof(buf), numRead);
    if (status) goto SOMEERROR;
    if (numRead == 0) break;
    status = Cout.PutBytes(buf, numRead, numWritten);
    if (status) goto SOMEERROR;
  } while (true);
  
  // Handle errors
  status = Error::None;
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }

  return status;

}

Status CommandLayer_OS_MD5(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  String           fName;
  FileStream       file;
  Byte             buf[4096];
  UInt             numRead;
  MD5Calc          md5Calc;
  MD5              md5;
  String           tStr;
  
  // Handle bad cases
  if (cmd.numArgs != 2) {
    if (cmd.numArgs == 0) goto INTERNAL;
    status = cmd.SetError(cmd.numArgs-1, Error::CmdOS_MD5_Expected1);
    goto SOMEERROR;
  }

  // Get the file name
  fName = cmd[1];
  if (IsEmpty(fName)) {
    status = Error::CmdOS_MD5_EmptyFileName;
    status = cmd.SetError(1, status, CmdStringMessage(status, fName));
    goto SOMEERROR;
  };
  fName = ApplyRelativePath(fName);
  if (!Exists(fName)) {
    status = Error::CmdOS_MD5_NotExist;
    status = cmd.SetError(1, status, CmdStringMessage(status, fName));
    goto SOMEERROR;
  }
  if (!IsFile(fName)) {
    status = Error::CmdOS_MD5_NotFile;
    status = cmd.SetError(1, status, CmdStringMessage(status, fName));
    goto SOMEERROR;
  }

  // Calculate MD5...
  status = file.SetName(fName);
  if (status) goto SOMEERROR;
  do {
    status = file.GetBytes(buf, sizeof(buf), numRead);
    if (status) goto SOMEERROR;
    if (numRead == 0) break;
    md5Calc.Append(buf, numRead);
  } while (true);
  
  // Display it...
  md5 = md5Calc.Get();
  tStr = ToHexString(md5).ToLowercase();
  Cout << CmdStringMessage(GetMessage(Error::CmdOS_MD5_CheckSum), tStr) << '\n';
  
  // Handle errors
  status = Error::None;
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Functions: CommandLayer_OS_IsDeviceName   - "Is this a device name?"
//            CommandLayer_OS_ValidFileName  - "Is this a valid file name?"
//            CommandLayer_OS_ValidFileName  - "Is this a valid path name?"
//            CommandLayer_OS_IsFullPath     - "Is this a full path name?"
//            CommandLayer_OS_IsRelativePath - "Is this a relative path name?"
//            CommandLayer_OS_Exists         - "Does this exist?"
//

Status CommandLayer_OS_IsDeviceName(CommandLayer *layer, BcmdCommand &cmd) {
  return CommandLayer_OS_Query(layer, cmd, OSQueryType::IsDeviceName);
}

Status CommandLayer_OS_ValidFileName(CommandLayer *layer, BcmdCommand &cmd) {
  return CommandLayer_OS_Query(layer, cmd, OSQueryType::IsValidFileName);
}

Status CommandLayer_OS_ValidPathName(CommandLayer *layer, BcmdCommand &cmd) {
  return CommandLayer_OS_Query(layer, cmd, OSQueryType::IsValidPathName);
}

Status CommandLayer_OS_IsFullPath(CommandLayer *layer, BcmdCommand &cmd) {
  return CommandLayer_OS_Query(layer, cmd, OSQueryType::IsFullPath);
}

Status CommandLayer_OS_IsRelativePath(CommandLayer *layer, BcmdCommand &cmd) {
  return CommandLayer_OS_Query(layer, cmd, OSQueryType::IsRelativePath);
}

Status CommandLayer_OS_Exists(CommandLayer *layer, BcmdCommand &cmd) {
  return CommandLayer_OS_Query(layer, cmd, OSQueryType::Exists);
}

Status CommandLayer_OS_Query(CommandLayer *layer, BcmdCommand &cmd, UInt queryType) {

  Status           status;
  String           name;
  UInt             nameIdx;
  UInt             maxLen;
  Boolean          answer;
  String           pad;
  String           answerTrue;
  String           answerFalse;
  String           answerExtra;
  
  // Make sure at least one is specified
  if (cmd.numArgs < 2) {
    if (cmd.numArgs == 0) goto INTERNAL;
    status = cmd.SetError(0, Error::CmdOS_Query_Expected1);
    goto SOMEERROR;
  }

  // Based on the queryType...
  switch (queryType) {
    case OSQueryType::IsDeviceName:
      answerTrue  = GetMessage(Error::CmdOS_Query_IsDevice);
      answerFalse = GetMessage(Error::CmdOS_Query_IsNotDevice);
      break;
    case OSQueryType::IsValidPathName:
      answerTrue  = GetMessage(Error::CmdOS_Query_IsPath);
      answerFalse = GetMessage(Error::CmdOS_Query_IsNotPath);
      break;
    case OSQueryType::IsValidFileName:
      answerTrue  = GetMessage(Error::CmdOS_Query_IsFile);
      answerFalse = GetMessage(Error::CmdOS_Query_IsNotFile);
      break;
    case OSQueryType::IsFullPath:
      answerTrue  = GetMessage(Error::CmdOS_Query_IsFullPath);
      answerFalse = GetMessage(Error::CmdOS_Query_IsNotFullPath);
      break;
    case OSQueryType::IsRelativePath:
      answerTrue  = GetMessage(Error::CmdOS_Query_IsRelPath);
      answerFalse = GetMessage(Error::CmdOS_Query_IsNotRelPath);
      break;
    case OSQueryType::Exists:
      answerTrue  = GetMessage(Error::CmdOS_Query_Exists);
      answerFalse = GetMessage(Error::CmdOS_Query_NotExist);
      break;
    default:
      status = Error::CmdOS_Query_Invalid;
      status = cmd.SetError(0, status, CmdUIntMessage(status, queryType));
      goto SOMEERROR;
  }

  // Get the maximum name length
  maxLen = 0;
  for (nameIdx=1; nameIdx < cmd.numArgs; nameIdx++) {
    name = cmd[nameIdx];
    if (queryType == OSQueryType::Exists && IsRelativePath(name))
      name = ApplyRelativePath(name);
    maxLen = Max(maxLen,name.Length());
  }

  // Validate each one...
  for (nameIdx=1; nameIdx < cmd.numArgs; nameIdx++) {

    name = cmd[nameIdx];
    if (queryType == OSQueryType::Exists && IsRelativePath(name))
      name = ApplyRelativePath(name);
    
    switch (queryType) {
      case OSQueryType::IsDeviceName:    answer = IsDevice(name);        break;
      case OSQueryType::IsValidPathName: answer = IsValidPathName(name); break;
      case OSQueryType::IsValidFileName: answer = IsValidFileName(name); break;
      case OSQueryType::IsFullPath:      answer = IsFullPath(name);      break;
      case OSQueryType::IsRelativePath:  answer = IsRelativePath(name);  break;
      case OSQueryType::Exists:          answer = Exists(name);          break;
    }

    answerExtra.SetEmpty();
    if (queryType == OSQueryType::Exists) {
      if (IsFile(name))      answerExtra += GetMessage(Error::CmdOS_Query_AndIsFile);
      if (IsDirectory(name)) answerExtra += GetMessage(Error::CmdOS_Query_AndIsDirectory);
    }
    Cout << layer->indent
         << GetEnvironmentValue("BEFE_QuoteLeadIn")
         << name
         << GetEnvironmentValue("BEFE_QuoteLeadOut");
    pad = ' ';
    pad *= (maxLen-name.Length())+1;
    Cout << pad << ((answer)?answerTrue:answerFalse) << answerExtra << '\n';
  }

  // Handle errors
  status = Error::None;
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }

  return status;

}

} // ...Namespace BEFE
