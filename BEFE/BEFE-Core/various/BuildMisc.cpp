//!befe-publish src
//----------------------------------------------------------------------
// File: BuildMisc.cpp - Build Class Miscellaneous Methods
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
// OSWalker context and function for Build._GetPublishableFiles...
//

struct PublishContext {
  // Members
  String  targetType;
  Strings files;
  Buffer  firstBlock;
  
  // Methods...
  PublishContext() {
    targetType.StartUp();
    files.StartUp();
    firstBlock.StartUp();
  }
  
  ~PublishContext() {
    targetType.ShutDown();
    files.ShutDown();
    firstBlock.ShutDown();
  }
  
  Status AllocateBuffer() {
    
    Status status;
    UInt   blockSize;
    
    if (firstBlock.GetAllocBytes() == 0) {
      blockSize = gDefaultBufSize;
      if (BEFE::IsNull(blockSize) || blockSize < 1024)
        blockSize = 1024;
      status = firstBlock.SetSize(blockSize);
      if (status) goto SOMEERROR;
    }
    else
      status = Error::None;
      
    // Handle errors
    while (false) {
      SOMEERROR: break;
    }
    
    return status;
  }
  
};

static Status PublishWalker(String const &dir, UInt32 context) { // Build.PublishWalker...
  
  Status          status;
  PublishContext *pubContext;
  String          curDir;
  Strings         fileNames;
  String          fileName;  
  UInt            curIdx;
  File            file;
  Byte           *buf;                // Block Buffer Address
  UInt            bufSize;            // Number of Bytes allocated in Block
  UInt            bufUsed;            // Number of Bytes used in Block
  String          bufString;          // Block contents as a String
  UInt            targLen;            // Length of target string
  
  // Get the context...  
  pubContext = (PublishContext *)context;
  if (BEFE::IsNull(context)) goto NULLPOINTER;
  status = pubContext->AllocateBuffer();
  if (status) goto SOMEERROR;
  buf = pubContext->firstBlock.GetBuf();
  if (BEFE::IsNull(buf)) goto NULLPOINTER;
  bufSize = pubContext->firstBlock.GetAllocBytes();
  
  pubContext->targetType.Insensitive();
  targLen = pubContext->targetType.Length();
  
  // Get the list of files...
  curDir = dir;
  status = PathToGeneric(curDir);
  if (status) goto SOMEERROR;
  status = GetFiles(curDir, fileNames);
  if (status) goto SOMEERROR;
  
  // For each file...
  for (curIdx=0; curIdx < fileNames.Length(); curIdx++) {
    
    // Set up the File...
    fileName = fileNames.Get(curIdx);
    status = PathToGeneric(fileName);
    if (status) goto SOMEERROR;
    status = file.SetName(fileName);
    if (status) goto SOMEERROR;

    // Read the first block...
    status = file.Read(0, buf, bufSize, bufUsed);
    if (status) goto SOMEERROR;
    
    // Turn into a string and chop it to first line...
    bufString = String((char const *)buf, bufUsed);
    bufString = bufString.Get(Span(0,bufString.Find('\n')));
    
    // Trim off whitespace...
    bufString = bufString.Trim();
    
    // See if we want this file...
    if (bufString.Get(Slice("0:14")).CompareInsensitive(";!befe-publish")  == 0 ||
        bufString.Get(Slice("0:14")).CompareInsensitive("#!befe-publish")  == 0)
      bufString = bufString.Get(Span(15,NaN)).Trim();
    else if (bufString.Get(Span(0,15)).Compare("//!befe-publish") == 0 ||
             bufString.Get(Span(0,15)).Compare("/*!befe-publish") == 0)
      bufString = bufString.Get(Span(16,NaN)).Trim();
    else
      continue;
    
    // We may want this one, see if the target matches...
    if (bufString.Length() != 0 || targLen) {
      if (bufString.Length() > targLen && bufString.Get(targLen) > 0x20u)
        continue;
      if (bufString.Get(Slice(0,targLen)) != pubContext->targetType)
        continue;
    }
      
    // We want this one, add to list...
    status = pubContext->files.Append(fileName.Consumable());
    if (status) goto SOMEERROR;
    
  }
  
  // Handle errors  
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
} // ...Build.PublishWalker

//----------------------------------------------------------------------
//
// Method: _GetPublishableFiles - Get Publishable Files of a specified type
//

Status Build::_GetPublishableFiles(String const &dirPath, String const &targetType, Strings &files) { // Build._GetPublishableFiles...
  
  Status         status;
  PublishContext context;
  
  // Do it
  if (!Exists(dirPath)) goto DIRNOTEXIST;
  if (!IsDirectory(dirPath)) goto DIRNOTDIR;  

  // Set up context and walk it...
  context.targetType = targetType;
  status = WalkPath(dirPath, PublishWalker, (UInt)&context);
  if (status) goto SOMEERROR;
  context.files.Consumable();
  files = context.files;
  
  // Handle errors
  while (false) {
    DIRNOTEXIST: status = Error::FileDirDoesNotExist; break;
    DIRNOTDIR:   status = Error::FilePathIsNotDir;    break;
    SOMEERROR:                                        break;
  }

  return status;
  
} // ...Build._GetPublishableFiles

//----------------------------------------------------------------------
//
// Method: _PatchSVNRevision - Patch SVN revision in a source file
//

Status Build::_PatchSVNRevision(String const &fileName) { // Build._PatchSVNRevision...

  Status  status;
  String  svnOutput;
  Strings lines;
  UInt    i;
  String  line;
  Int     colon;
  Integer revision;
  File    file;
  Bytes   bContents;
  String  sContents;
  Int     dollar;
  Int     dollar2;
  UInt    numChanged;
  Byte   *buf;
  UInt    bufSize;
  UInt    numWritten;
  // TEMP...
  String  tString;
  // ...TEMP
  
  // Make sure file exists
  if (!IsFullPath(fileName)) goto NOTFULL;
  if (!Exists(fileName)) goto NOTEXIST;
  if (!IsFile(fileName)) goto NOTFILE;
  
  // Get the SVN Info
  status = _InvokeCaptureOutput("svn info -r HEAD", GetEnvironmentValue("BEFE_SVN"), svnOutput);
  if (status) goto SOMEERROR;
  lines = svnOutput.Split('\n');
  for (i=0; i < lines.Length(); i++) {
    line = lines.Get(i).Trim();
    colon = line.Find(':');
    if (!BEFE::IsNull(colon) && line.Get(Span(0,colon)) == "Revision") {
      revision = line.Get(Span(colon+1,NaN));
      break;
    }
  }
  
  // Read the contents
  file.SetAccess(File::ModeReadWrite);
  file.SetName(fileName);
  status = file.ReadContents(bContents);
  if (status) goto SOMEERROR;
  sContents = bContents.MorphToString();
  
  // Adjust the contents...
  dollar = 0;
  numChanged = 0;
  while (dollar < (Int)sContents.Length()) {
    dollar = sContents.Find("$Revision: ", Span(dollar, NaN));
    if (BEFE::IsNull(dollar)) break;
    dollar2 = sContents.Find('$', Span(dollar+1, NaN));
    if (BEFE::IsNull(dollar2)) break;
    // Sanity check...
    if (dollar2-dollar > 20) goto MALFORMED;
    // Adjust it...
    status = sContents.Remove(Span(dollar, dollar2+1));
    if (status) goto SOMEERROR;
    tString = String("$Revision: ");
    tString += ToString(revision);
    tString += " $";
    status = sContents.Insert(dollar, tString.Consumable());
    if (status) goto SOMEERROR;
    dollar = dollar+1;
    numChanged++;
  }
  
  // Write the contents
  if (numChanged) {
    status = file.SetLength(0);
    if (status) goto SOMEERROR;
    sContents._BufAndSize(buf, bufSize);
    if (!BEFE::IsNull(buf)) {
      status = file.Write(buf, 0, bufSize, numWritten);
      if (status) goto SOMEERROR;
    }
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTFULL:   status = Error::FileNotFullPath;   break;
    NOTEXIST:  status = Error::FileDoesNotExist;  break;
    NOTFILE:   status = Error::FilePathIsNotFile; break;
    MALFORMED: status = Error::Failed;            break;
    SOMEERROR: break;
  }
  
  return status;
  
} // ...Build_PatchSVNRevision

} // ...Namespace BEFE