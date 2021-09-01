//----------------------------------------------------------------------
// File: Command_OS_Analyse.cpp - Command_OS Analyse functions
//----------------------------------------------------------------------
//
// This file implements the Command_OS Command Handler and related
// functions and structs.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Error.h"
#include "ToStrings.h"
#include "Sort.h"
#include "Lines.h"
#include "UTF8.h"
#include "CommandLayer.h"

namespace BEFE { // Namespace BEFE...

#define CSTR_MemoryAllocRaw "MemoryAllocRaw:"
#define CSTR_MemoryFreeRaw  "MemoryFreeRaw:"

// Forward prototypes
Status Command_AnalyseFile_txt(String const &fileName, UInt indent);
Status Command_AnalyseFile_lang(String const &fileName, UInt indent);

//----------------------------------------------------------------------
//
// Struct: WalkContext - Directory walker context
//

struct WalkContext {

  UInt       numDirs;
  UInt       numFiles;
  UInt       minDepth;
  UInt       maxDepth;
  ULong      totalSize;

  void Init(CommandLayer *theLayer, String const &startDir) {
    Strings parts;
    TheBefe->TheOS->PathSplit(startDir,parts);
    numDirs   = 0;
    numFiles  = 0;
    minDepth  = parts.Length();
    maxDepth  = minDepth;
    totalSize = 0;
  }

};

static Status Walker(String const &dir, Ptr context) {

  Status       status;
  WalkContext *ctxt;
  Strings      files;
  Strings      parts;
  String       fName;
  FileInfo     fInfo;
  UInt         fileIdx;

  ctxt = (WalkContext *)context;
  ctxt->numDirs++;
  files = TheBefe->TheOS->GetFiles(dir);
  ctxt->numFiles += files.Length();
  for (fileIdx=0; fileIdx < files.Length(); fileIdx++) {
    fName = files.Get(fileIdx);
    status = TheBefe->TheOS->GetFileInfo(fName,fInfo);
    if (!status)
      ctxt->totalSize += fInfo.size;
  }

  files.SetEmpty();

  status = TheBefe->TheOS->PathSplit(dir,parts);
  ctxt->maxDepth = Max(ctxt->maxDepth,parts.Length());

  return Error::None;

}

//----------------------------------------------------------------------
//
// Class: LinesSortTarget
//

struct LinesSortTarget : public SortTarget {
  
           LinesSortTarget();
  virtual ~LinesSortTarget();

  virtual Status  SetLines(Strings const &pLines);    
  virtual UInt    Length  () const;
  virtual Int     Compare (UInt elm1, UInt elm2);
  virtual Status  Swap    (UInt elm1, UInt elm2);
  virtual Status  Move    (UInt elm1, UInt elm2);
  virtual Boolean Selected(UInt elm);
  
  // File name
  Strings *lines;
  UInt     numLines;
  Strings  lineParts; // We keep this here so it's not realloc'd for each line
  UInt     numErrors;
  
};

LinesSortTarget::LinesSortTarget() {

  lines      = NULL;
  numLines   = 0;
  lineParts.SetLength(10);
  numErrors  = 0;
  
}

LinesSortTarget::~LinesSortTarget() {
}

Status LinesSortTarget::SetLines(Strings const &pLines) {
  lines = (Strings *)&pLines;
  numLines = lines->Length();
  numErrors  = 0;
  return Error::None;
}

UInt LinesSortTarget::Length () const {
  return numLines;
}

Int LinesSortTarget::Compare(UInt elm1, UInt elm2) {
  
  Int    answer;
  String str1;
  String str2;
  
  answer = 0;
  if (lines) {
    str1 = lines->Get(elm1);
    str2 = lines->Get(elm2);
    answer = str1.Compare(str2);
  }
 
  return answer;
}

Status LinesSortTarget::Swap(UInt elm1, UInt elm2) {
  
  Status status;
  String str1;
  String str2;
  
  status = Error::None;
  if (lines) {
    str1 = lines->Get(elm1);
    str2 = lines->Get(elm2);
    lines->Set(elm1,str2);
    lines->Set(elm2,str1);
  }
 
  return status;

}

Status LinesSortTarget::Move(UInt elm1, UInt elm2) {
  if (!numErrors)
    BEFE_WARN("LinesSortTarget.Move not implemented yet");
  ((LinesSortTarget *)this)->numErrors++;
  return Error::NotImplemented;
}

Boolean LinesSortTarget::Selected(UInt elm) {
  
  Boolean answer;
  String  line;
  String  part1;
  
  if (!lines) goto NOPE;
  line = lines->Get(elm);
  if (line.Length() == 0 || line.Get(0) != '0' || line.Get(1) != 'x') goto NOPE;
  line._SplitToConstStrings(' ',*(Strings *)&lineParts);
  part1 = lineParts.Get(1);
  if (part1 != CSTR_MemoryAllocRaw && part1 != CSTR_MemoryFreeRaw) goto NOPE;

  answer = true;
  while (false) {
    NOPE: answer = false; break;
  }
  
  return answer;
  
}

//----------------------------------------------------------------------
//
// Function: Command_Analyse - Main OS Analyse Command Handler
//

Status Command_Analyse(CommandLayer *layer, BcmdCommand &cmd) {

  Status           status;
  OperatingSystem *os;
  UInt             argNo;
  UInt             argStart;
  String           arg;
  String           cwd;
  String           path;
  UInt             oldfg;
  UInt             numBad;

  FileInfo         fInfo;
  Strings          strings;
  String           string;
  UInt             i;

  WalkContext      walkInfo;

  // Make sure we've got an OS...
  if (!TheBefe->TheOS) goto NOOS;
  if (!TheBefe->TheProcess) goto NOPROCESS;
  os = TheBefe->TheOS;
  cwd = TheBefe->TheProcess->GetCurrentWorkingDirectory();

  // If no arguments, we'll use CWD...
  argStart = (cmd.numArgs) ? 1 : 0;
  if (cmd.numArgs < 2)
    argStart = 0;

  // First pass: Verify each one exists...
  oldfg  = Cout.SetForegroundColour(Console::Yellow);
  numBad = 0;
  for (argNo=1; argNo < cmd.numArgs; argNo++) {
    arg = cmd[argNo];
    path = os->ApplyRelativePath(cwd, arg);
    if (!os->Exists(path)) {
      Cout << layer->indent << "Path '" << path << "' does not exist\n";
      numBad++;
    }
  }
  Cout.SetForegroundColour(oldfg);
  if (numBad) goto OK;

  //
  // Analyse each target...
  //

  if (argStart == 0)
    arg = cwd;

  for (argNo=argStart; argNo < cmd.numArgs; argNo++) { // For each target...

    if (argStart)
      arg = cmd[argNo];
    else
      arg = cwd;

    path = os->ApplyRelativePath(cwd, arg);

    // If directory...
    if (os->IsDirectory(path)) {
      Cout << '\n' << layer->indent << "Directory '" << path << "'...\n";

      // Get the info
      walkInfo.Init(layer, path);
      status = os->WalkPath(path, Walker, (Ptr) &walkInfo);
      if (status) goto SOMEERROR;

      // Add the info from the base directory
      //Walker(path,(Int32 &)walkInfo);

      // Display it...
      Cout << '\n';
      Cout << layer->indent << "  SubDirectories: " << walkInfo.numDirs << '\n';
      Cout << layer->indent << "  Depth:          " << walkInfo.maxDepth-walkInfo.minDepth << '\n';
      Cout << layer->indent << "  Files:          " << walkInfo.numFiles << '\n';
      Cout << layer->indent << "  Disk Used:      " << ByteUnits(walkInfo.totalSize);
      if (gVeryVerbose)
        Cout << " (Exactly " << (UInt32)walkInfo.totalSize << " Bytes)";
      Cout << '\n';

    }

    // Else, if file...
    else if (os->IsFile(path)) {

      Cout << '\n' << layer->indent << "File '" << path << "'...\n";
      status = os->GetFileInfo(path,fInfo);
      if (status) goto SOMEERROR;
      strings.SetEmpty();
      status = ToStrings(fInfo,strings);
      if (status) goto SOMEERROR;
      for (i=0; i < strings.Length(); i++) {
        if (i==0)
          Cout << '\n';
        string = strings.Get(i);
        Cout << "  " << layer->indent << string << '\n';
      }
      if ( path.Get(Slice("-4:")).CompareInsensitive(String(".txt"))      == 0 &&
          !path.Get(Slice("-8:")).CompareInsensitive(String("_msg.txt"))  == 0)
        status = Command_AnalyseFile_txt(path,2);
      else if (path.Get(Slice("-5:")).CompareInsensitive(String(".befe")) == 0 ||
               path.Get(Slice("-2:")).CompareInsensitive(String(".h"))    == 0 ||
               path.Get(Slice("-2:")).CompareInsensitive(String(".c"))    == 0 ||
               path.Get(Slice("-4:")).CompareInsensitive(String(".cpp"))  == 0 ||
               path.Get(Slice("-3:")).CompareInsensitive(String(".py"))   == 0 ||
               path.Get(Slice("-5:")).CompareInsensitive(String(".java")) == 0 ||
               path.Get(Slice("-3:")).CompareInsensitive(String(".js"))   == 0 ||
               path.Get(Slice("-5:")).CompareInsensitive(String(".json")) == 0 ||
               path.Get(Slice("-4:")).CompareInsensitive(String(".htm"))  == 0 ||
               path.Get(Slice("-5:")).CompareInsensitive(String(".html")) == 0 ||
               path.Get(Slice("-4:")).CompareInsensitive(String(".xml"))  == 0 ||
               path.Get(Slice("-4:")).CompareInsensitive(String(".txt"))  == 0  )
        status = Command_AnalyseFile_lang(path,2);
      if (status) goto SOMEERROR;
    }

    // Else, don't know what it is
    else {
      oldfg  = Cout.SetForegroundColour(Console::Yellow);
      Cout << layer->indent << "Don't know what '" << path << "' is all about!!!\n";
      Cout.SetForegroundColour(oldfg);
    }

  } // ...For each target

  //
  // Handle errors
  //

  status = Error::None;
  while (false) {
    OK:         status = Error::None;                 break;
    NOOS:       status = Error::InternalNoTheOS;      break;
    NOPROCESS:  status = Error::InternalNoTheProcess; break;
    SOMEERROR:                                        break;
  }

  if (!status)
    Cout << '\n';

  return status;
}

//----------------------------------------------------------------------
//
// Function: Command_Analyse_txt - Analyse a Text file
//
// NOTE: For now, we'll assume it's BEFE output and simply look for
//       MemoryAllocRaw and MemoryFreeRaw output to report missing
//       memory.
//

Status Command_AnalyseFile_txt(String const &fileName, UInt indent) {

  Status          status;
  String          ind;
  LinesSortTarget sTarget;
  Bytes           bytes;
  String          contents;
  UInt            numLines;
  Strings         lines;
  UInts           offsets;
  String          curLine;
  String          prevLine;
  Strings         curParts;
  String          curPart0;
  String          curPart1;
  String          prevPart0;
  String          prevPart1;
  UInt            offsetNo;
  UInt            numAlloc;
  UInt            numFree;
  UInt            numMismatched;
  Byte           *buf;
  UInt            bufLen;
  Time            times[5];
  
  ind = ' ';
  ind *= indent;
  Cout << '\n';
  Cout << ind << "Analysing Text file '" << fileName << "'...\n\n";
  ind += "  ";

  // Get the contents as a single String
  times[0].Set();
  status = File(fileName).ReadContents(bytes);
  if (status) goto SOMEERROR;
  contents = bytes.MorphToString(false);
  numLines = contents.Count('\n');
  if (contents.Get(-1) != '\n')
    numLines++;
  
  // Get the lines
  lines.SetLength(numLines);
  status = contents.Immutable();
  if (status) goto SOMEERROR;
  status = contents._SplitToConstStrings('\n',lines);
  if (status) goto SOMEERROR;
  times[1].Set();
  
  // Sort them
  sTarget.SetLines(lines);
  status = SortMerge(sTarget,offsets);
  if (status) goto SORTERROR;
  times[2].Set();
  Cout << '\n';
  Cout << ind << "File has " << numLines << " lines\n";
  Cout << ind << "There are " << offsets.Length() << " BEFE AllocRaw/FreeRaw lines to process...\n";
  
  // Process them
  numMismatched = 0;
  prevLine.Reset();
  prevPart0.Reset();
  prevPart1.Reset();
  contents._BufAndSize(buf, bufLen);
  numAlloc = numFree = 0;
  times[3].Set();
  for (offsetNo=0; offsetNo < offsets.Length(); offsetNo++) {
    curLine = lines.Get(offsets.Get(offsetNo));
    // Ignore one we immediately know we don't want...
    if (curLine.Length() == 0) continue;
    if (curLine.Get(0) != '0' && curLine.Get(1) != 'x') continue;
    // Split into Parts...
    curLine._SplitToConstStrings(' ', curParts);
    curPart0 = curParts.Get(0);
    curPart1 = curParts.Get(1);
    
    if (curPart1 == CSTR_MemoryAllocRaw)
      numAlloc++;
    else if (curPart1 == CSTR_MemoryFreeRaw)
      numFree++;
      
    if (curPart0 != prevPart0) {
      if (curPart1 == CSTR_MemoryAllocRaw && prevPart1 == CSTR_MemoryAllocRaw) {
        if (numMismatched == 0) Cout << '\n';
        numMismatched++;
        Cout << ind << "MisMatch: " << prevLine << '\n';
      }
    }
    prevLine = curLine;
    prevPart0 = curPart0.Consumable();
    prevPart1 = curPart1.Consumable();
  }

  if (curPart1 == CSTR_MemoryAllocRaw && prevPart1 != CSTR_MemoryFreeRaw) {
    if (numMismatched == 0) Cout << '\n';
    numMismatched++;
    Cout << ind << "MisMatch: " << prevLine << '\n';
  }

  if (numMismatched != 0) Cout << '\n';

  Cout << ind << "*** " << numMismatched << " Mismatches\n";
  Cout << ind << "*** MemoryAllocRaw calls: " << numAlloc << '\n';
  Cout << ind << "*** MemoryFreeRaw  calls: " << numFree  << '\n';
  Cout << ind << "***           Difference: " << (Int)numAlloc-(Int)numFree << '\n';
  
  times[4].Set();

  // Summary timing...
  Cout << '\n';
  if (gVeryVerbose) {
    Cout << ind << "Read     Time: " << ToString(times[0],times[1]) << '\n';
    Cout << ind << "Sort     Time: " << ToString(times[2],times[1]) << '\n';
    Cout << ind << "Analysis Time: " << ToString(times[4],times[3]) << '\n'; 
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    SORTERROR: break;
    SOMEERROR: break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Function: Command_Analyse_lang - Analyse a programming Language file
//

Status Command_AnalyseFile_lang(String const &fileName, UInt indent) {
  
  Status status;
  
  /*
  Cout << '\n';
  status = CommandLayer_Lex_AnalyseFile(fileName);
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;  
  while (false) {
    SOMEERROR: BEFE_ERROR("CommandLex_AnalyseFile",status); break;
  }
  */
  status = Error::NotImplemented;
  
  return status;
  
}

} // ...Namespace BEFE
