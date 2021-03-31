//!befe-publish src
//----------------------------------------------------------------------
// File: TestCache.cpp - TestCache Class Implementation
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
#include "Null.h"
#include "Test.h"
#include "TestCache.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// TestCache.Run - Main Cache Test method
//

Status TestCache::Run() {

  Status status;
  UInt   numErrors;
  if (!gQuiet)
    out << "***Testing Cache" << "...\n";

  numErrors = 0;
  indent    = 1;

  /* TEMP...
  DisplayBEFE_SVN(indent+1);
  
  //status = testEmpty();
  //if (status) {ERROR("TestCache.Run.testEmpty", status); numErrors++;}
  status = testAllocateBlocks();
  if (status) {BEFE_ERROR("TestCache.Run.testAllocate", status); numErrors++;}
  status = testStartCacheFiles();
  if (status) {BEFE_ERROR("TestCache.Run.testStatCacheFiles", status); numErrors++;}
  status = testBlockLoad();
  if (status) {BEFE_ERROR("TestCache.Run.testBlockLoad", status); numErrors++;}
  status = testBlockLoadSource();
  if (status) {BEFE_ERROR("TestCache.Run.testBlockLoadSource", status); numErrors++;}

  if (gVeryVerbose)
    DisplayCache(indent);
  ...TEMP */
  
  if (numErrors)
    status = Error::Failed;
  else
    status = Error::None;
    
  return status;

}

//----------------------------------------------------------------------
//
// TestCache.DisplayBEFE_SVN - Display the BEFE_SVN Environment vVariable
//

Status TestCache::DisplayBEFE_SVN(UInt pIndent) {

  Status status;
  String sIndent;
  String befeSvn;
  
  sIndent = String("  ")*pIndent;
  if (!gShowDebug) goto OK;
  
  befeSvn = GetEnvironmentValue("BEFE_SVN");
  if (IsNull(befeSvn)) goto NOSVN;
  status = TheBefe->TheOS->PathToGeneric(befeSvn);
  if (status) goto SOMEERROR;
  if (befeSvn.Get(-1) != '/') befeSvn += '/';
  if (gVeryVerbose && gShowDebug) {
    out << sIndent << "BEFE_SVN = '" << befeSvn << "'\n";
    out << sIndent << "  Exists       = " << ((Exists(befeSvn))?"True":"False")      << '\n';
    out << sIndent << "  Is Directory = " << ((IsDirectory(befeSvn))?"True":"False") << '\n';
  }

  // Handle errors
  while (false) {
    OK:        status = Error::None;       break;
    NOSVN:     status = Error::NoBEFE_SVN; break;
    SOMEERROR:                             break;
  }
  
  if (status) BEFE_ERROR("TestCache.DisplayBEFE_SVN",status);
  
  return status;
  
}

//----------------------------------------------------------------------
//
// TestCache.DisplayCache - Display the test Cache contents
//

Status TestCache::DisplayCache(UInt pIndent) {

  Status status;
  String sIndent;
  
  sIndent = String("  ")*pIndent;
  if (!gVeryVerbose) goto OK;
  
  out << sIndent << "***Cache Contents...\n\n";
  status = cache.DisplayOverview((pIndent+1)*2);
  if (status) goto SOMEERROR;
  out << '\n';
  status = cache.DisplayFiles((pIndent+1)*2);
  if (status) goto SOMEERROR;
  out << '\n';
  status = cache.DisplayUsedBlocks((pIndent+1)*2);
  if (status) goto SOMEERROR;
  out << '\n';
  status = cache.DisplayFreeBlocks((pIndent+1)*2);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// TestCache.testEmpty - Test contents of Empty Cache
//

Status TestCache::testEmpty() {
  
  String sIndent;
  
  sIndent = String("  ")*indent;
  
  if (!gQuiet)
    out << sIndent << "Testing Empty Cache...\n";
  //status = DisplayCache(indent+1);
    
  return Error::None;

}

//----------------------------------------------------------------------
//
// TestCache.testAllocateBlocks - Test Cache._AllocateBlocks
//

Status TestCache::testAllocateBlocks() {
  
  Status status;
  String sIndent;
  
  sIndent = String("  ")*indent;
  
  if (gVerbose)
    out << sIndent << "Testing Cache._AllocateBlocks...\n";

  //status = cache.Reset();
  //if (status) goto SOMEERROR;
  status = cache._AllocateBlocks();
  if (status) goto SOMEERROR;
  
  //status = Display(indent+1);
    
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// TestCache.testStartCacheFiles - Test Cache._StartCacheFiles
//

Status TestCache::testStartCacheFiles() {
  
  Status status;
  String befeSvn;
  String fileName;
  String sIndent;
  String sIndent2;
  String sIndent3;
  
  char const *fileNames[] = {
    "trythis.cpp",
    "Test.h",
    "TestArray.cpp",
    NULL};
  UInt curIdx;
  
  sIndent  = String("  ")*indent;
  sIndent2 = String("  ")*(indent+1);
  sIndent3 = String("  ")*(indent+2);

  if (gVerbose)
    out << sIndent << "Testing Cache.StartCacheFiles...\n";

  //status = cache.Reset();
  //if (status) goto SOMEERROR;
      
  befeSvn = GetEnvironmentValue("BEFE_SVN");
  if (IsNull(befeSvn)) goto NOSVN;
  status = PathToGeneric(befeSvn);
  if (status) goto SOMEERROR;
  if (!IsFullPath(befeSvn))
    befeSvn = ApplyRelativePath(GetCurrentWorkingDirectory(), befeSvn);
  if (befeSvn.Get(-1) != '/') befeSvn += '/';
  
  // Register the files...
  for (curIdx=0; fileNames[curIdx]; curIdx++) {
    fileName = befeSvn + "c++/test/" + fileNames[curIdx];
    status = cache.StartCacheFile(fileName);
    if (status && gVeryVerbose && gShowDebug) {
      out << sIndent3 << "Error Caching file '" << fileName << "'...\n";
      BEFE_ERROR("TestCache.testStartCacheFiles",status);
    }
    if (status) goto SOMEERROR;
  }
    
  //status = Display(indent+1);
    
  // Handle errors
  while (false) {
    NOSVN:     status = Error::NoBEFE_SVN; break;
    SOMEERROR:                             break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// TestCache.testBlockLoad - Test Cache.BlockLoad
//

Status TestCache::testBlockLoad() {
  
  Status        status;
  String        sIndent;
  String        sIndent2;
  String        sIndent3;
  String        befe_SVN;
  String        fileName;
  Id            fileId;
  CacheFileInfo fileInfo;
  UInt          numBytes;
  UInt          numBlocks;
  UInt          blockNo;
  
  UInt fg;  
  
  fg = Cout.GetForegroundColour();
  
  sIndent  = String("  ")*indent;
  sIndent2 = String("  ")*(indent+1);
  sIndent3 = String("  ")*(indent+2);

  if (gVerbose)
    out << sIndent << "Testing Cache.BlockLoad...\n";

  //status = cache.Reset();
  //if (status) goto SOMEERROR;
      
  befe_SVN = GetEnvironmentValue("BEFE_SVN");
  if (IsEmpty(befe_SVN)) goto NOSVN;
  status = PathToGeneric(befe_SVN);
  if (status) goto SOMEERROR;
  if (!IsFullPath(befe_SVN))
    befe_SVN = ApplyRelativePath(GetCurrentWorkingDirectory(), befe_SVN);
  fileName = befe_SVN + "/c++/test/TestString.cpp";
  status = ValidateExistingFileName(fileName);
  if (status) goto SOMEERROR;
  
  // Set up the Cache File...
  status = cache.StartCacheFile(fileName);
  if (status) goto SOMEERROR;
  status = cache.GetFileId(fileName, fileId);
  if (status) goto SOMEERROR;
  status = cache.GetFileInfo(fileId, fileInfo);
  if (status) goto SOMEERROR;
  if (gVerbose)
    out << sIndent2 << "File '" << fileName << "' CacheFileInfo...\n\n";
  
  // Get the File size
  status = cache.GetFileSize(fileId, numBytes);
  if (status) goto SOMEERROR;
  numBlocks = (numBytes+cache.GetBlockSize()-1)/cache.GetBlockSize();
  if (gVerbose) {
    out << sIndent2 << "File  size: " << numBytes << " (Bytes)\n";
    out << sIndent2 << "Block size: " << cache.GetBlockSize() << " (Bytes)\n";
    out << sIndent2 << "Blocks:     " << numBlocks << '\n';
  }
 
  // Read each of the blocks...
  for (blockNo=0; blockNo < numBlocks; blockNo++) {
    status = cache.LoadFileBlock(fileId, blockNo);
    if (status) {
      out << sIndent2 << "Block " << ToString(blockNo,3) << ": status = ";
      if (status) Cout.SetForegroundColour(Console::Red);
      out << ErrorString(status) << '\n';
      out.SetForegroundColour(fg);
      continue;
    }
    //out << sindent2 << "Block " << ToString(blockNo,3) <<": size = " << cache.GetBlockUsedBytes(fileId, blockNo) << " (Bytes)\n";
  }

  // Display the file I/O info
  if (gVeryVerbose) {
    out << '\n';
    status = cache.GetFileInfo(fileId, fileInfo);
    if (status) goto SOMEERROR;
    status = fileInfo.Display(out, (indent+1)*2);
    if (status) goto SOMEERROR;
  }

  // Display the last block
  if (gShowDebug) {
    out << '\n';
    out << sIndent2 << "Last Block (" << --blockNo << ")...\n\n";
    status = cache.DisplayFileBlock(fileId, blockNo, (indent+1)*2);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOSVN:     status = Error::NoBEFE_SVN; break;
    SOMEERROR:                             break;
  }  

  return status;
  
}

//----------------------------------------------------------------------
//
// OSWalker context and function for TestCache.testBlockLoadSource...
//

struct WalkContext {
  Strings headerFiles;
  Strings sourceFiles;
};
  
static Status Walker(String const &dir, UInt32 context) {
  
  Status       status;
  WalkContext *walkContext;
  String       curDir;
  Strings      fileNames;
  String       fileName;
  UInt         curIdx;
  
  walkContext = (WalkContext *)context;
  if (IsNull(walkContext)) goto NULLPOINTER;
  curDir = dir;
  status = PathToGeneric(curDir);
  if (status) goto SOMEERROR;
  status = GetFiles(curDir, fileNames);
  if (status) goto SOMEERROR;
  for (curIdx=0; curIdx < fileNames.Length(); curIdx++) {
    fileName = fileNames.Get(curIdx);
    if (fileName.Get(Range(-2,-1)) == ".h") {
      status = walkContext->headerFiles.Append(fileName.Consumable());
      if (status) goto SOMEERROR;
    }
    else if (fileName.Get(Range(-4,-1)) == ".cpp") {
      status = walkContext->sourceFiles.Append(fileName.Consumable());
      if (status) goto SOMEERROR;
    }
  }
  
  // Handle errors  
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// TestCache.testBlockLoadSource - Test Cache.BlockLoad of all BEFE_SVN source
//

Status TestCache::testBlockLoadSource() {
  
  Status      status;
  String      sIndent;
  String      sIndent2;
  String      sIndent3;
  String      befe_SVN;
  String      fileName;
  Id          fileId;
  UInt        fileSize;
  Cache       cache;
  WalkContext walkContext;
  UInt        curIdx;
  Strings     headerFileNames;
  Strings     headerNames;
  Strings     sourceFileNames;
  Strings     sourceNames;
  String      fullPath;
  String      fileOnly;
  UInt        fileBlockCount;
  UInt        blockNo;
  UInt        curPct;
  UInt        newPct;
  
  UInt        numBytesRead;
  UInt        numFilesRead;
  UInt        numBlocksRead;
  
  sIndent  = String("  ")*indent;
  sIndent2 = String("  ")*(indent+1);
  sIndent3 = String("  ")*(indent+2);

  if (gVerbose)
    out << sIndent << "Testing Cache.BlockLoadSource...\n";

  befe_SVN = GetEnvironmentValue("BEFE_SVN");
  if (IsEmpty(befe_SVN)) goto NOSVN;
  status = PathToGeneric(befe_SVN);
  if (status) goto SOMEERROR;
  if (!IsFullPath(befe_SVN))
    befe_SVN = ApplyRelativePath(GetCurrentWorkingDirectory(), befe_SVN);
  fileName = befe_SVN + "/c++/test/TestString.cpp";
  status = ValidateExistingFileName(fileName);
  if (status) goto SOMEERROR;
  
  // Get the list of header and source files...
  status = WalkPath(befe_SVN, Walker, (UInt32)&walkContext);
  if(status) goto SOMEERROR;
  if (gVerbose && !gVeryVerbose) {
    out << walkContext.headerFiles.Length() << " Total .h   files...\n";
    out << walkContext.sourceFiles.Length() << " Total .cpp files...\n";
  }
  if (gShowDebug) {
    out << sIndent2 << "Header Files (" << walkContext.headerFiles.Length() << " files)...\n\n";
    if (walkContext.headerFiles.Length() == 0)
      Cout << sIndent2 << CONSTR_None << '\n';
    else for (curIdx=0; curIdx < walkContext.headerFiles.Length(); curIdx++)
      out << sIndent3 << walkContext.headerFiles.Get(curIdx) << '\n';
    out << '\n';
    out << sIndent2 << "Source Files (" << walkContext.sourceFiles.Length() << " files)...\n\n";
    if (walkContext.sourceFiles.Length() == 0)
      out << sIndent3 << CONSTR_None << '\n';
    else for (curIdx=0; curIdx < walkContext.sourceFiles.Length(); curIdx++)
      out << sIndent2 << "  " << walkContext.sourceFiles.Get(curIdx) << '\n';
  }
  
  // Separate file names from full paths...
  headerFileNames = walkContext.headerFiles.Consumable();
  sourceFileNames = walkContext.sourceFiles.Consumable();
  for (curIdx=0; curIdx < headerFileNames.Length(); curIdx++) {
    status = headerFileNames.Get(curIdx, fullPath);
    if (status) goto SOMEERROR;
    fileOnly = FileOnly(fullPath);
    status = headerNames.Append(fileOnly);
    if (status) goto SOMEERROR;
  }
  status = headerNames.Sort();
  if (status) goto SOMEERROR;

  for (curIdx=0; curIdx < sourceFileNames.Length(); curIdx++) {
    status = sourceFileNames.Get(curIdx, fullPath);
    if (status) goto SOMEERROR;
    fileOnly = FileOnly(fullPath);
    status = sourceNames.Append(fileOnly);
    if (status) goto SOMEERROR;
  }
  status = sourceNames.SortInsensitive();
  if (status) goto SOMEERROR;
  
  //
  // Read each of the header files...
  //
  
  if (gVerbose) 
    out << sIndent2 << "Reading header files...";
  if (gVeryVerbose && out.IsInteractive()) {
    out << "  0%";
    SetNull(curPct);
  }
  else if (gVerbose)
    out << '\n';
  
  numFilesRead  = 0;
  numBlocksRead = 0;
  numBytesRead  = 0;
  
  for (curIdx=0; curIdx < headerNames.Length(); curIdx++) { // For each header file...
 
    // Cache the file and get its info...
    fileName = headerFileNames.Get(curIdx);
    status = cache.StartCacheFile(fileName);
    if (status) goto SOMEERROR;
    status = cache.GetFileId(fileName, fileId);
    if (status) goto SOMEERROR;
    status = cache.GetFileBlockCount(fileId, fileBlockCount);
    if (status) goto SOMEERROR;
    status = cache.GetFileSize(fileId, fileSize);
    if (status) goto SOMEERROR;
    
    // Load each block...
    for (blockNo=0; blockNo < fileBlockCount; blockNo++) {
      status = cache.LoadFileBlock(fileId, blockNo);
      if (status) goto SOMEERROR;
      numBlocksRead++;
    }
    
    // Tell user what we've done
    if (gVeryVerbose) {
      newPct = (curIdx*100)/headerNames.Length();
      if (newPct != curPct)
        Cout << String(Char(BS))*4 << ToString(newPct,3) << '%';
      curPct = newPct;
    }
    
    // Stop Caching the file...
    status = cache.StopCacheFile(fileName);
    if (status) goto SOMEERROR;
    
    numFilesRead++;
    numBlocksRead++;
    numBytesRead += fileSize;
    
  } // ...For each header file
  
  // Tell use what we've done
  if (gVeryVerbose && out.IsInteractive())
    out << String(Char(BS))*4 << "Finished ("
        << numFilesRead  << " Files, "
        << numBlocksRead << " Blocks, and "
        << numBytesRead  << " Bytes read)"
        << '\n';
  
  //
  // Read each of the source files...
  //
  
  if (gVerbose) 
    out << sIndent2 << "Reading source files...";
  if (gVeryVerbose && out.IsInteractive()) {
    out << "  0%";
    SetNull(curPct);
  }
  else if (gVerbose)
    out << '\n';
  
  numFilesRead  = 0;
  numBlocksRead = 0;
  numBytesRead  = 0;
  
  for (curIdx=0; curIdx < sourceNames.Length(); curIdx++) { // For each header file...
 
    // Cache the file and get its info...
    fileName = sourceFileNames.Get(curIdx);
    status = cache.StartCacheFile(fileName);
    if (status) goto SOMEERROR;
    status = cache.GetFileId(fileName, fileId);
    if (status) goto SOMEERROR;
    status = cache.GetFileBlockCount(fileId, fileBlockCount);
    if (status) goto SOMEERROR;
    status = cache.GetFileSize(fileId, fileSize);
    if (status) goto SOMEERROR;
    
    // Load each block...
    for (blockNo=0; blockNo < fileBlockCount; blockNo++) {
      status = cache.LoadFileBlock(fileId, blockNo);
      if (status) goto SOMEERROR;
      numBlocksRead++;
    }
    
    // Tell user what we've done
    if (gVeryVerbose && out.IsInteractive()) {
      newPct = (curIdx*100)/sourceNames.Length();
      if (newPct != curPct)
        out << String(Char(BS))*4 << ToString(newPct,3) << '%';
      curPct = newPct;
    }
    
    // Stop Caching the file...
    status = cache.StopCacheFile(fileName);
    if (status) goto SOMEERROR;
    
    numFilesRead++;
    numBlocksRead++;
    numBytesRead += fileSize;
    
  } // ...For each header file
  
  // Tell use what we've done
  if (gVeryVerbose)
    out << String(Char(BS))*4 << "Finished ("
        << numFilesRead  << " Files, "
        << numBlocksRead << " Blocks, and "
        << numBytesRead  << " Bytes read)"
        << '\n';
  
  //
  // Handle errors
  //
  
  status = Error::None;
  while (false) {
    NOSVN:     status = Error::NoBEFE_SVN; break;
    SOMEERROR:                             break;
  }  
  
  return status;
  
}

} // ...Namespace BEFE
