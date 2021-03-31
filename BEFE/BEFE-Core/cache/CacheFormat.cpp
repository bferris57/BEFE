//----------------------------------------------------------------------
// File: CacheFormat.cpp - Cache Class Output Formatting
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Cache.h"

namespace BEFE { // Namespace BEFE...

Status Cache::DisplayOverview() {
  return DisplayOverview(0);
}

Status Cache::DisplayOverview(UInt pIndent) {
  
  Status     status;
  String     indent;
  String     indent2;
  CacheInfo  info;
  
  indent  = String(' ')*pIndent;
  //indent2 = String(' ')*(pIndent*2);
  
  // Display Cache Overview Info...
  info = GetInfo();
  Cout << indent  << "Cache Overview...\n\n";
  status = info.Display(Cout, pIndent*2);  
  
  status = Error::None;
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Display Files...
//

Status Cache::DisplayFiles() {
  return DisplayFiles(0);
}

Status Cache::DisplayFiles(UInt pIndent) {
  
  Status     status;
  String     indent;
  String     indent2;
  String     indent3;
  String     head;
  String     middle;
  String     tail;
  String     fileName;
  UInt       curIdx;
  CacheFile *theFiles;
  UInt       fg;
  
  indent  = String(' ')*pIndent;
  indent2 = String(' ')*((pIndent+1)*2);
  indent3 = String(' ')*((pIndent+2)*3);
  
  head   = (gConsoleUTF8)?"MRU » ":"MRU ->";
  middle = (gConsoleUTF8)?"    » ":"    ->";
  tail   = (gConsoleUTF8)?"LRU » ":"LRU ->";
  
  Cout << indent << "Cache Files...\n\n";
  fg = Cout.GetForegroundColour();  
  
  theFiles = (CacheFile *)files._GetAddress(0);
  if (BEFE::IsNull(theFiles)) {
    Cout.SetForegroundColour(Console::Yellow);
    Cout << indent2 << CONSTR_StarNone << '\n';
    Cout.SetForegroundColour(fg);
    goto OK;
  }

  curIdx = firstUsedFileIdx;

  while (!BEFE::IsNull(curIdx)) { // For each used CacheFile...
  
    Cout << indent2;

    Cout.SetForegroundColour(Console::Yellow);
    if (BEFE::IsNull(theFiles[curIdx].prevFileIdx))
      Cout << head;
    else if (BEFE::IsNull(theFiles[curIdx].nextFileIdx))
      Cout << tail;
    else
      Cout << middle;
    Cout.SetForegroundColour(fg);

    Cout << "File[" << curIdx << "]: "
         << "id = 0x" << ToHexString((UInt)theFiles[curIdx].id, 8) << ", "
         << "name = '" << theFiles[curIdx].stream.GetName() << "'\n";
    if (gVeryVerbose) {
      if (theFiles[curIdx].numLogReads || theFiles[curIdx].numLogWrites) {
        Cout << indent3 << "State           = "
             << (theFiles[curIdx].stream.IsOpen() ? CONSTR_Open : CONSTR_Closed)
             << '\n';
        Cout << indent3 << "Size            = " << theFiles[curIdx].size                   << '\n';
        Cout << indent3 << "Last Read       = " << ToString(theFiles[curIdx].lastRead,Time::Milliseconds) << '\n';
        Cout << indent3 << "Last Modified   = " << ToString(theFiles[curIdx].lastModified,Time::Milliseconds) << '\n';
        Cout << indent3 << "Last Written    = " << ToString(theFiles[curIdx].lastWrite,Time::Milliseconds)    << '\n';
        Cout << indent3 << "Logical  Reads  = " << theFiles[curIdx].numLogReads            << '\n';
        Cout << indent3 << "Physical Reads  = " << theFiles[curIdx].numPhyReads            << '\n';
        Cout << indent3 << "Logical  Writes = " << theFiles[curIdx].numLogWrites           << '\n';
        Cout << indent3 << "Physical Writes = " << theFiles[curIdx].numPhyWrites           << '\n';
      }
    }
    
    curIdx = theFiles[curIdx].nextFileIdx;
    
  }  // ...For each used CacheFile
  
  if (BEFE::IsNull(firstUsedFileIdx))
    Cout << indent2 << CONSTR_StarNone << '\n';
    
  //
  // Handle errors
  //
  
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Display Used/Free Blocks...
//

Status Cache::DisplayUsedBlocks() {
  NEED_TheBefe
  NEED_TheOS
  return _DisplayUsedOrFree(true, 0);
}

Status Cache::DisplayUsedBlocks(UInt indent) {
  NEED_TheBefe
  NEED_TheOS
  return _DisplayUsedOrFree(true, indent);
}

Status Cache::DisplayFreeBlocks() {
  NEED_TheBefe
  NEED_TheOS
  return _DisplayUsedOrFree(false, 0);
}

Status Cache::DisplayFreeBlocks(UInt indent) {
  NEED_TheBefe
  NEED_TheOS
  return _DisplayUsedOrFree(false, indent);
}

Status Cache::_DisplayUsedOrFree(Boolean used, UInt pIndent) {
  
  String      indent;
  String      indent2;
  UInt        tInt;
  UInt        intLen;
  UInt        lineIdx;
  String      outLine;
  CacheBlock *theBlocks;
  UInt        blockIdx;
  String      outHead;
  String      outLead;
  String      outSep;
  String      outTail;
  UInt        fg;

  NEED_TheBefe
  NEED_TheOS
  
  fg = Cout.GetForegroundColour();
  
  indent  = String(' ')*pIndent;
  indent2 = String(' ')*(pIndent*2);
  
  Cout << indent << ((used) ? "Used" : "Free") << " Cache Blocks...\n\n";
  
  outHead   = (gConsoleUTF8) ? "MRU » " : "MRU ->";
  outLead   = (gConsoleUTF8) ? "      " : "      ";
  outSep    = (gConsoleUTF8) ? " »"     : " ->";
  outTail   = (gConsoleUTF8) ? "» LRU"  : "-> LRU";
    
  // Figure out how big (in base 10) to make the displayed block numbers
  intLen = 0;
  tInt = maxBlocks;
  while (!BEFE::IsNull(tInt) && tInt >= 10) {intLen++; tInt/=10;}
  if (intLen) intLen++;
    
  // Display them...
  theBlocks = (CacheBlock *)blocks._GetAddress(0);
  if (intLen == 0 || BEFE::IsNull(theBlocks)) { // No Blocks...
    Cout.SetForegroundColour(Console::Yellow);
    Cout << indent2 << CONSTR_StarNone << '\n';
    Cout.SetForegroundColour(fg);
  }
  else { // For each Free Block...
    lineIdx = 0;
    blockIdx =(used) ? firstUsedBlockIdx : firstFreeBlockIdx;
    Cout << indent2;
    Cout.SetForegroundColour(Console::Yellow);
    Cout << outHead;
    Cout.SetForegroundColour(fg);
    while (!BEFE::IsNull(blockIdx)) {
      Cout << ToString(blockIdx, intLen);
      lineIdx++;
      blockIdx = theBlocks[blockIdx].nextBlockIdx;
      if (!BEFE::IsNull(blockIdx)) {
        Cout.SetForegroundColour(Console::Yellow);
        Cout << outSep;
        Cout.SetForegroundColour(fg);
      }
      if (lineIdx >= 8) {
        if (BEFE::IsNull(blockIdx)) {
          Cout.SetForegroundColour(Console::Yellow);
          outLine += outTail;
          Cout.SetForegroundColour(fg);
        }
        Cout << '\n' << indent2 << outLead;
        lineIdx = 0;
      }
      else
        Cout << ' ';
    }
    Cout.SetForegroundColour(Console::Yellow);
    Cout << outTail;
    Cout.SetForegroundColour(fg);
    Cout << '\n';
    
  } // ...For each Block
  
  return Error::None;
  
}

Status Cache::DisplayFileBlock(Id fileId, UInt blockNo, UInt pIndent) {
  
  Status      status;
  UInt        fileIdx;
  UInt        blockIdx;
  CacheBlock *block;
  Byte        lastBytes[16];
  UInt        numSame;
  Byte       *curBytes;
  UInt        numLines;
  UInt        curLine;
  String      indent;
  UInt        curIdx;
  String      outString;
  
  NEED_TheBefe
  NEED_TheOS

  indent = String(' ')*pIndent;

  status = _FileIdToIndex(fileId, fileIdx);
  if (status) goto SOMEERROR;
  status = _FindUsedFileBlock(fileIdx, blockNo, blockIdx);
  if (status) goto SOMEERROR;
  
  block = (CacheBlock *)blocks._GetAddress(blockIdx);
  if (BEFE::IsNull(block)) goto NULLPOINTER;
  curBytes = block->_GetBuf();
  if (BEFE::IsNull(curBytes)) goto NULLPOINTER;
  
  numLines = blockSize/16;
  numSame  = 0;
  
  for (curLine=0; curLine < numLines; curLine++) {

    outString.Reset();
    
    // If Same Contents...
    if (curLine != 0 && curLine != numLines-1 && MemoryCompareRaw(curBytes, lastBytes, 16) == 0) {
      numSame++;
      if (numSame == 1) {
        outString << indent << String(' ')*((pIndent+10)+21);
        outString << ((gConsoleUTF8)?String("•••"):String("..."));
        outString << String(' ')*30;
        outString << ((gConsoleUTF8)?String("•••"):String("..."));
        outString << '\n';
      }
    }
    
    // Different Contents...
    else {
      numSame = 0;
      outString << indent << ToHexString(curLine*16, 8) << ": ";
      for (curIdx=0; curIdx < 16; curIdx++)
        outString << ' ' << ToHexString(curBytes[curIdx], 2);
      outString << "  ";
      for (curIdx=0; curIdx < 16; curIdx++) {
        if (curBytes[curIdx]>=0x20 && curBytes[curIdx]<=0x7f)
          outString << Char(curBytes[curIdx]);
        else if (curBytes[curIdx] < 0x20)
          outString << ((gConsoleUTF8)?String("░"):String('.'));
        else
          outString << ((gConsoleUTF8)?String("■"):String('.'));
      }
      outString << "\n";
    }
    
    Cout << outString;
    
    MemoryCopyRaw(curBytes, lastBytes, 16);
    curBytes += 16;
    
  }
  
  // Handle errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

} // ...Namespace BEFE