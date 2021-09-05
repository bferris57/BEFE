//----------------------------------------------------------------------
// File: LexStream.cpp - Implementation of the BEFE LexStream Struct
//----------------------------------------------------------------------
//
// This class contains the C++ implementation of the BEFE LexStream
// Struct.  
//
// The purpose of a LexStream is to take a file, in a specified
// language, as input and to provide a stream of LexTokens as output.
//
// There's a bit more to it than that but that's the basic idea here.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Lex.h"
#include "LexToken.h"
#include "LexStream.h"
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status LexStream::StartUp() {

  Status retStatus;
	Status status;
  
  isConsumable = false;
  isNull       = true;
  isParsing    = false;
  inputType    = InputTypeNone;
  
  lex = NULL;
  
  retStatus = languageName.StartUp();
  status = inputFileName.StartUp();
  if (status && !retStatus) retStatus = status;
  status = inputString.StartUp();
  if (status && !retStatus) retStatus = status;
  
  status = lang.StartUp();
  if (status && !retStatus) retStatus = status;
  status = lineOffsets.StartUp();
  if (status && !retStatus) retStatus = status;  
  status = curTok.StartUp();
  if (status && !retStatus) retStatus = status;
  BEFE::SetNull(blkSize);
  BEFE::SetNull(bufSize);
  BEFE::SetNull(bufBlkNo);
  BEFE::SetNull(buf);
  BEFE::SetNull(curByte);
  BEFE::SetNull(curChar);
  BEFE::SetNull(curCharLen);
  BEFE::SetNull(curOffset);
  BEFE::SetNull(curLineStart);
  BEFE::SetNull(curLineNo);
  BEFE::SetNull(curColNo);
  BEFE::SetNull(bufUsed);
  BEFE::SetNull(bufRem);
  BEFE::SetNull(bufTrig);
  cmtCounting  = false;
  cmtDepth = 0;

  return retStatus;
	
}

Status LexStream::ShutDown() {
	
  Status retStatus;
  Status status;
	
  retStatus = _FinishParsing();
  if (!BEFE::IsNull((void *)lex)) {
    status = lex->Detach(*this);
    if (status && !retStatus) retStatus = status;
  }
  lex = NULL;
  status = lang.ShutDown();
  if (status && !retStatus) retStatus = status;
  
  status = lineOffsets.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = curTok.ShutDown();
  if (status && !retStatus) retStatus = status;

  status = inputString.ShutDown();
  if (status && !retStatus) retStatus = status;  
  status = inputFileName.ShutDown();
  if (status && !retStatus) retStatus = status;  
  status = languageName.ShutDown();
  if (status && !retStatus) retStatus = status;

  return retStatus;
	
}

Status LexStream::Clear() {
	
  Status status;
	
  // Note: Haven't quite decided what to do here yet
  status = Error::None;
	
  return status;
	
}

Status LexStream::Reset() {
	
  Status retStatus;
  Status status;
	
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
	
  return retStatus;
	
}

Boolean LexStream::IsNull() const {
  return isNull;
}

Status LexStream::SetNull() {
  Status status;
  status = Reset();
  return status;
}

Boolean LexStream::IsEmpty() {
  return false;
}

Status LexStream::SetEmpty() {
  return Error::NotApplicable;
}

Status LexStream::MoveFrom(LexStream const &that) {
	
	Status  retStatus;
	Status  status;
  Lex    *tLex;
  
  retStatus = Error::None;	
  if (this != (LexStream *)&that) {	
    retStatus = ShutDown();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(that));

    tLex = (Lex *)that.lex;
    lex = NULL;
		if (!BEFE::IsNull(tLex)) {

      status = tLex->Detach(*(LexStream *)&that);
		  if (status && !retStatus) retStatus = status;
      
		  status = lex->Attach(*this);
		  if (status && !retStatus) retStatus = status;
    }

    status = ((LexStream *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;

    NotConsumable();

  }

  return retStatus;
	
}

Status LexStream::CopyFrom(LexStream const &that) {
	
	Status  retStatus;
	Status  status;

  retStatus = Error::None;	
  
  if (this != (LexStream *)&that) {	// If not "copy self"...

    if (that.IsConsumable()) // If Consumable...

      retStatus = MoveFrom(that);

    else { // Not Consumable...

      retStatus = Reset();

      inputType = that.inputType;
      
      if (!BEFE::IsNull(that.lex)) {
        lex = that.lex;
        status = lex->Attach(*this);
        if (status && !retStatus) retStatus = status;
      }
      status = lang.CopyFrom(that.lang);
      if (status && !retStatus) retStatus = status;
      status = lineOffsets.CopyFrom(that.lineOffsets);
      if (status && !retStatus) retStatus = status;
      status = curTok.CopyFrom(that.curTok);
      if (status && !retStatus) retStatus = status;

      status = inputFileName.CopyFrom(that.inputFileName);
      if (status && !retStatus) retStatus = status;
      status = inputString.CopyFrom(that.inputString);
      if (status && !retStatus) retStatus = status;
      blkSize  = that.blkSize;
      bufSize  = that.bufSize;
      bufBlkNo = that.bufBlkNo;
      if (!BEFE::IsNull(that.buf)) {
        buf = MemoryAllocRaw(bufSize);
        if (BEFE::IsNull(buf) && !retStatus) retStatus = Error::OutOfMemory;
        curByte = buf + (that.curByte - that.buf);
      }
      else {
        buf = that.buf;
        curByte = that.curByte;
      }
      curChar      = that.curChar;
      curCharLen   = that.curCharLen;
      curOffset    = that.curOffset;
      curLineStart = that.curLineStart;
      curLineNo    = that.curLineNo;
      curColNo     = that.curColNo;
      bufUsed      = that.bufUsed;
      bufRem       = that.bufRem;
      bufTrig      = that.bufTrig;
      cmtCounting  = that.cmtCounting;
      cmtDepth     = that.cmtDepth;

    } // ...Not Consumable

  } 	// ...If not "copy self"

  return retStatus;
	
}

Boolean LexStream::IsConsumable() const {
  return isConsumable;
}

LexStream &LexStream::Consumable() const {
  ((LexStream *)this)->isConsumable = true;
  return *((LexStream *)this);
}

LexStream &LexStream::NotConsumable() const {
  ((LexStream *)this)->isConsumable = false;
  return *((LexStream *)this);
}

//----------------------------------------------------------------------
//
// Lex Processing
//

Status LexStream::_Detach() { // LexStream._Detach...
  
  Status status;
  
  if (!BEFE::IsNull((void *)lex)) {
    lex = NULL;
    status = ResetBuffer();
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
} // ...LexStream._Detach

Status LexStream::_Attach(Lex const &theLex) { // LexStream._Attach...
  
  Status status;
  
  if (!BEFE::IsNull((void *)lex)) goto ISATTACHED;
  lex = (Lex *)&theLex;
  status = ResetBuffer();
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    ISATTACHED: status = Error::LexStreamIsAttached; break;
    SOMEERROR:                                       break;
  }
  
  return status;
  
} // ...LexStream._Attach

//----------------------------------------------------------------------
//
// Input Settings
//

Status LexStream::SetLanguageName(String const &name) {  
  Status status;
  status = _FinishParsing();
  languageName.CopyFrom(name);
  return status;
}

Status LexStream::SetInputFileName(String const &name) {

  Status retStatus;
  Status status;
  
  retStatus = _FinishParsing();
  status = inputFileName.CopyFrom(name);
  if (status && !retStatus) retStatus = status;
  status = inputString.Reset();
  if (status && !retStatus) retStatus = status;
  inputType = (status) ? InputTypeNone : InputTypeFile;
  
  return retStatus;

}

Status LexStream::SetInputString(String const &str) {
  
  Status retStatus;
  Status status;
  
  retStatus = _FinishParsing();
  status = inputFileName.Reset();
  if (status && !retStatus) retStatus = status;
  status = inputString.CopyFrom(str);
  if (status && !retStatus) retStatus = status;
  inputType = (status) ? InputTypeNone : InputTypeString;
  
  return retStatus;

}

UInt LexStream::GetInputSize() {
  return (isParsing) ? inputSize : UNaN;
}

Status LexStream::GetInputSize(UInt &size) {
  size = (isParsing) ? inputSize : UNaN;
  return Error::None;
}

Status LexStream::CountInputLines(UInt &count) {
  
  Status  status;
  UInt    blockNo;
  Byte   *block;
  UInt    blockUsed;
  UInt    bytesRem;
  UInt    filePos;
  Byte   *theLF;
  
  // Error if improper input type
  if (inputType >= NumInputTypes || inputType == InputTypeNone) goto BADTYPE;
  
  // Start parsing first
  status = _StartParsing();
  if (status) goto SOMEERROR;

  // InputTypeString: It's relatively easy...
  if (inputType == InputTypeString) {
    count = inputString.Count('\n');
    if (!BEFE::IsNull(count))
      count++;
    goto OK;
  }
  
  //
  // InputTypeFile: For each block...
  //
  
  count = 0;
  filePos = 0;
  bytesRem = inputSize;
  blockNo = 0;
  
  while (bytesRem) {
    // Get the block address and size...
    status = LoadFileBlock(fileId, blockNo);
    if (status) goto SOMEERROR;
    block = TheBefe->TheCache._GetBlockPointer(fileId, blockNo);
    if (BEFE::IsNull(block)) goto NULLPOINTER;
    blockUsed = GetBlockUsedBytes(fileId, blockNo);
    // Prepare for next iteration
    bytesRem -= blockUsed;
    filePos += blockUsed;
    blockNo++;
    // Count the '\n' occurreces...
    while (blockUsed) {
      theLF = Memchr(block, '\n', blockUsed);
      if (BEFE::IsNull(theLF)) break;
      count++;
      theLF++;
      blockUsed -= theLF-block;
      block = theLF;
    }
  }
  
  // Increment count because if there's 1 '\n', there's two lines...
  count++;
  
  //
  // Handle errors
  //
  
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    BADTYPE:     status = Error::LexStreamBadType;    break;
    OK:          status = Error::None;                break;
    SOMEERROR:                                        break;
  }
  if (status)
    count = UNaN;
    
  return status;
  
}

//----------------------------------------------------------------------
//
// Lex Buffer Processing
//

Status LexStream::ResetBuffer() {
  
  Status status;

  // Have a Lex...  
  if (!BEFE::IsNull(lex)) {
    blkSize = TheBefe->TheCache.GetBlockSize();
    status = _AllocateBuf();
    if (status) goto SOMEERROR;
  }

 // Don't have a Lex...
  else {
    BEFE::SetNull(blkSize);
  }
  
  // Clear other stuff that doesn't depend on the buffer
  BEFE::SetNull(bufBlkNo);
  curByte = buf;
  bufRem  = 0;
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

Status LexStream::_AllocateBuf() {
  
  Status status;
  
  // If we need to allocate the buffer...
  if ( !BEFE::IsNull(bufSize) && BEFE::IsNull(buf) ) {
    // Null everything in case we get an error allocating...
    BEFE::SetNull(curByte);
    BEFE::SetNull(bufUsed);
    BEFE::SetNull(bufRem);
    buf = MemoryAllocRaw(bufSize);
    if (BEFE::IsNull(buf)) goto OUTOFMEMORY;
    curByte  = buf;
    bufUsed  = 0;
    bufRem   = 0;
    bufTrig  = 0;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFMEMORY: status = Error::OutOfMemory; break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Parsing Internal
//

Status LexStream::_StartParsing() { // LexStream._StartParsing...

  Status        status;
  Id            langId;
  CacheFileInfo fileInfo;
  
  if (!isParsing) { // Not Parsing yet...

    // Input validation
    if (BEFE::IsNull(lex)) goto NOLEX;
    if (inputType == InputTypeString) {
      if (inputString.Length() == 0) goto EMPTY;
    }
    else if (inputType == InputTypeFile) {
      if (BEFE::IsNull(inputFileName) || BEFE::IsEmpty(inputFileName)) goto NOFILE;
      if (!IsFullPath(inputFileName)) goto NOTFULL;
      if (!Exists(inputFileName)) goto NOTEXIST;
      if (!IsFile(inputFileName)) goto NOTFILE;
    }
    else
      goto BADTYPE;
    if (BEFE::IsNull(languageName) || BEFE::IsEmpty(languageName)) goto NOLANG;
    
    // Get a copy of the language...
    status = lex->GetLanguage(languageName, lang);
    if (status) goto SOMEERROR;
    
    // Denormalise the LexState and LexAction pointers
    states = (LexState *)lang.states._GetAddress(0);
    if (BEFE::IsNull(states)) goto NULLPOINTER;
    numStates = lang.states.Length();
    actions = (LexAction *)lang.actions._GetAddress(0);
    if (BEFE::IsNull(actions)) goto NULLPOINTER;
    numActions = lang.actions.Length();
    
    // Cache the file if inputType is InputTypeFile...
    if (inputType == InputTypeFile) {
      status = TheBefe->TheCache.StartCacheFile(inputFileName, false, true);
      if (status) goto SOMEERROR;
      blkSize = TheBefe->TheCache.GetBlockSize();
      if (BEFE::IsNull(blkSize)) goto INTERNAL;
      status = TheBefe->TheCache.GetFileId(inputFileName, fileId);
      if (status) goto SOMEERROR;
      inputSize = TheBefe->TheCache.GetFileSize(fileId);
    }

    // Set input size if inputType is InputTypeString...
    if (inputType == InputTypeString)
      inputSize = inputString.Size();
    
    // Initialise state, lineOffsets, and token
    numLines = 0;
    status = lineOffsets.SetEmpty();
    if (status) goto SOMEERROR;
    status = lineOffsets.Append(0);
    if (status) goto SOMEERROR;
    nonWhiteCount = 0;
    curState = Lex::StateMain;
    curTok.type      = LexToken::EndOfInput;
    curTok.span      = Span(0, inputSize);
    
    // Set up the Buffer
    if (BEFE::IsNull(lang.contigMax) || lang.contigMax == 0)
      contigMax = 4;
    else
      contigMax = lang.contigMax;
    bufSize = blkSize + contigMax;
    status = _AllocateBuf();
    if (status) goto SOMEERROR;
    curByte = buf;
    bufTrig = 0;

    // We're parsing now
    isParsing = true;
    
  } // ...Not Parsing yet

  // Parsing now, just reset...
  BEFE::SetNull(curTok);
  BEFE::SetNull(bufBlkNo);
  curByte  = buf;
  BEFE::SetNull(curChar);
  BEFE::SetNull(curCharLen);
  curOffset    = 0;
  curLineStart = 0;
  curLineNo    = 0;
  curColNo     = 0;
  bufUsed      = 0;
  BEFE::SetNull(bufRem);
  
  // Handle errors...
  status = Error::None;
  while (false) {
    BADTYPE:     status = Error::LexStreamBadType;     break;
    NOLEX:       status = Error::LexStreamNotAttached; break;
    EMPTY:       status = Error::LexStreamInputEmpty;  break;
    NOFILE:      status = Error::LexStreamNoFileName;  break;
    NOTFULL:     status = Error::FileNotFullPath;      break;
    NOTEXIST:    status = Error::FileDoesNotExist;     break;
    NOTFILE:     status = Error::FileNameIsNotFile;    break;
    NOLANG:      status = Error::LexStreamNoLangName;  break;
    NULLPOINTER: status = Error::InternalNullPointer;  break;
    INTERNAL:    status = Error::Internal;             break;
    SOMEERROR:                                         break;
  }

  return status;
  
} // ...LexStream._StartParsing

Status LexStream::_FinishParsing() { // LexStream._FinishParsing...

  Status    retStatus;
  Status    status;
  
  retStatus = Error::None;  
  if (isParsing) {
    
    // Free the buffer
    BEFE::SetNull(bufBlkNo);
    if (!BEFE::IsNull(buf)) {
      MemoryFreeRaw(buf);
      BEFE::SetNull(buf);
    }
    BEFE::SetNull(curByte);
    BEFE::SetNull(bufRem);
    BEFE::SetNull(bufUsed);
  
    // Stop caching the file (if we have a file name)...
    if (!inputFileName.IsNull()) {
      status = TheBefe->TheCache.StopCacheFile(inputFileName);
      if (status && !retStatus) retStatus = status;
      BEFE::SetNull(fileId);
    }
    BEFE::SetNull(inputSize);
    
    // Reset the language
    status = lang.Reset();
    if (status && !retStatus) retStatus = status;
    states  = NULL;
    actions = NULL;
    
    // We're finished parsing now
    isParsing = false;
    
  }

  return retStatus;
    
} // ...LexStream._FinishParsing

Status LexStream::_PopulateBuffer() { // LexStream._PopulateBuffer...
  
  Status  status;
  UInt    numRead;
  Byte   *sBuf;
  UInt    sSize;
  
  // Sanity checks
  if (BEFE::IsNull(lex))       goto NULLPOINTER;
  if (inputType == InputTypeFile) {
    if (BEFE::IsNull(fileId))    goto INTERNAL;
  }
  if (BEFE::IsNull(inputSize)) goto INTERNAL;
  if (BEFE::IsNull(buf))       goto NULLPOINTER;
  if (BEFE::IsNull(curByte))   goto NULLPOINTER;
  if (BEFE::IsNull(blkSize))   goto INTERNAL;
  if (BEFE::IsNull(bufSize))   goto INTERNAL;
  if (!BEFE::IsNull(bufBlkNo) && bufBlkNo > inputSize/blkSize) goto INTERNAL;
  if (BEFE::IsNull(bufUsed))   goto INTERNAL;
  if (curByte < buf)           goto INTERNAL;
  if ((UInt)(curByte-buf) > bufSize)  goto INTERNAL;
  if (bufUsed != 0 && bufUsed == blkSize && curByte >= buf+bufUsed) goto INTERNAL;

  // Read only if we need to...
  if ((Int)bufTrig <= 0) {
    
    // Special case for EOF...
    if (bufUsed != 0 && bufUsed != blkSize+contigMax && bufUsed != blkSize) goto EOF;

    // Copy any remaining bytes to the beginning...
    if (bufUsed) {
      MemoryCopyRaw(buf+bufUsed-contigMax, buf, contigMax);
      bufUsed = contigMax;
      if (BEFE::IsNull(bufBlkNo)) goto INTERNAL;
      bufBlkNo++;
    }
    else {
      if (!BEFE::IsNull(bufBlkNo)) goto INTERNAL;
      bufBlkNo = 0;
    }

    // Set current position
    curByte = buf;
    bufRem = bufUsed;
    bufTrig = (bufRem) ? (bufRem-contigMax) : 0;
    
    // Handle inputType == InputTypeFile...
    if (inputType == InputTypeFile) { // inputType == InputTypeString...

      //  Read if we're not already at the end...
      if (bufBlkNo == 0 ||( (bufBlkNo-1)*blkSize + bufUsed < inputSize ) ) {
        status = LoadFileBlock(fileId, bufBlkNo);
        if (status) goto SOMEERROR;
        numRead = GetBlockUsedBytes(fileId, bufBlkNo);
        if (BEFE::IsNull(numRead)) goto INTERNAL;
        status = GetBlockContents(fileId, bufBlkNo, 0, buf+bufUsed, numRead);
        if (status) goto SOMEERROR;
        MemoryFillRaw(buf+bufUsed+numRead, 0xff, bufSize-bufUsed-numRead);
        bufUsed += numRead;
        bufRem  += numRead;
        bufTrig  = (numRead != blkSize) ? bufRem : bufUsed-contigMax;
      }

    } // ...inputType == InputTypeFile
    
    // Handle inputType == InputTypeString...
    else if (inputType == InputTypeString) { // inputType == InputTypeString...
      
      // Get from inputString if not already at end...
      if (bufBlkNo*blkSize+bufUsed < inputSize) {
        inputString._BufAndSize(sBuf, sSize);
        if (BEFE::IsNull(sBuf)) goto NULLPOINTER;
        sBuf += bufBlkNo*blkSize;
        numRead = Min(blkSize, sSize - bufBlkNo*blkSize);
        MemoryCopyRaw(sBuf, buf+bufUsed, numRead);
        MemoryFillRaw(buf+bufUsed+numRead, 0xff, bufSize-bufUsed-numRead);
        bufUsed += numRead;
        bufRem  += numRead;
        bufTrig  = (numRead != blkSize) ? bufRem : bufUsed-contigMax;
      }
      
    } // ...inputType == InputTypeString
    
    // Else, something bad's going on (unknown type)...
    else
      goto INTERNAL;
      
  }

  // Handle errors
  status = Error::None;
  while (false) {
    INTERNAL:    status = Error::Internal;            break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    EOF:         status = Error::LexStreamAtEnd;      break;
    SOMEERROR:                                        break;
  }

  return status;
  
} // ...LexStream._PopulateBuffer

//----------------------------------------------------------------------
//
// Parsing Query
//
// Note: LineToOffset and OffsetToLine will return Null if it can't be
//       resolved.
//

UInt LexStream::LineToOffset(UInt lineNo) {
  
  UInt   offset;

  if (lineNo > lineOffsets.Length())
    offset = UNaN;
  else
    offset = lineOffsets.Get(lineNo);  
  
  return offset;
  
}

UInt LexStream::OffsetToLine(UInt inputOffset) {
  
  UInt   ll;      // ◄─┐
  UInt   mid;     //   ├─ For the binary search
  UInt   ul;      // ◄─┘
  UInt   offset;  
  
  ll = 0;
  ul = lineOffsets.Length();

  if (ul == 0)
    return UNaN;
  if (inputOffset > lineOffsets.Get(-1))
    return lineOffsets.Length()-1;
    
  while (ll <= ul) {
    mid = (ul+ll)>>1;
    offset = lineOffsets.Get(mid);
    if (inputOffset < offset)
      ul = mid-1;
    else if (inputOffset > offset)
      ll = mid+1;
    else
      return mid;
  }
  
  return ul;
  
}

Status LexStream::OffsetToLineCol(UInt inputOffset, UInt &lineNo, UInt &colNo) {

  Status status;  
  UInt   lineOffset;
  
  lineNo = OffsetToLine(inputOffset);
  if (BEFE::IsNull(lineNo)) {
    BEFE::SetNull(colNo);
    goto BADOFFSET;
  }
  else {
    lineOffset = LineToOffset(lineNo);
    if (BEFE::IsNull(lineOffset)) goto BADLINEOFFSET;
    colNo = inputOffset - lineOffset;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    BADOFFSET:     status = Error::InvalidOffset; break;
    BADLINEOFFSET: status = Error::Internal;      break;
  }
  
  return status;
  
}

String LexStream::GetInputSpan(Span span) {
  
  //Status status;
  String value;
  
  /*status =*/ GetInputSpan(span, value);
  
  return value.Consumable();
  
}

Status LexStream::GetInputSpan(Span span, String &value) {
  
  Status  status;
  Span    rSpan;
  UInt    remLen;
  UInt    thisBlockNo;
  UInt    numRead;
  Byte   *block;
  
  status = value.Reset();
  if (status) goto SOMEERROR;
  
  // Start parsing if we need to
  if (!isParsing) {
    status = _StartParsing();
    if (status) goto SOMEERROR;
  }

  // Check null/truncated span cases...
  rSpan = span.Resolve(inputSize);
  if (BEFE::IsNull(rSpan.idx1)) goto OK;
  if (BEFE::IsNull(rSpan.idx2)) goto OK;
  remLen = rSpan.Length();
  
  // It's easier(ish) if it's a String.
  //
  // Note: Span is in Bytes not Chars so we have to dick about a bit...
  if (inputType == InputTypeString) {
  	Byte *buf;
  	UInt  size;
  	inputString._BufAndSize(buf,size);
  	if (buf)
      value = String(buf+rSpan.idx1,rSpan.idx2-rSpan.idx1);
    goto OK;
  }
  
  // Nope, it's a File so until we're finished...
  while (rSpan.idx1 < rSpan.idx2) {
    
    // Get the block
    thisBlockNo = rSpan.idx1/blkSize;
    status = LoadFileBlock(fileId, thisBlockNo);
    if (status) goto SOMEERROR;
    numRead = GetBlockUsedBytes(fileId, thisBlockNo);
    if (BEFE::IsNull(numRead)) goto INTERNAL;

    // Get the data from this block
    block = TheBefe->TheCache._GetBlockPointer(fileId, thisBlockNo);
    if (BEFE::IsNull(block)) goto NULLPOINTER;
    if (thisBlockNo*blkSize+numRead < (UInt)rSpan.idx1) break;
    numRead -= rSpan.idx1%blkSize;
    block += rSpan.idx1%blkSize;
    numRead = Min(numRead,remLen);
    
    status = value.Append(block, numRead);
    if (status) goto SOMEERROR;
    
    // Prepare for the next iteration (if any)
    remLen -= numRead;
    rSpan.idx1 += numRead;
    
   }

  // Handle errors
  while (false) {
    OK:          status = Error::None;                break;
    INTERNAL:    status = Error::Internal;            break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
    
  return status;
  
}

//----------------------------------------------------------------------
//
// Single Character Processing (Consumes current character)
//

Status LexStream::_Consume() {

  curOffset += curCharLen;
  if (*curByte == '\n') {
    curTok.lineCount++;
    curLineStart = curOffset;
    curLineNo++;
    curColNo = 0;
  }
  else if (curCharLen != 0)
    curColNo++;
  curByte += curCharLen;
  bufRem  -= curCharLen;
  bufTrig -= curCharLen;
  curTok.span.idx2 += curCharLen;
  
  return Error::None;
  
}

//----------------------------------------------------------------------
//
// Parsing
//
// Note: On a non-zero Status return, NextToken will set the value
//       theToken.type to be LexToken::Error and will set the
//       theToken.span to indicate where the error occurred (it's best
//       to use theToken.span.idx2)
//

Status LexStream::NextToken(LexToken &theToken) { // LexStream.NextToken...

  Status status;
  Byte   byte;
  Byte   actionIdx;
  
  // Start parsing if we need to
  if (!isParsing) {
    status = _StartParsing();
    if (status) goto SOMEERROR;
  }

  // Initialise current token
  curTok.type   = LexToken::Unknown;
  curTok.lineNo = curLineNo;
  curTok.colNo  = curColNo;
  curTok.value.Reset();
  if (bufBlkNo == UNaN)
    curTok.span.idx1 = 0;
  else if (bufBlkNo != 0)
  	curTok.span.idx1 = bufBlkNo*blkSize - contigMax + (curByte-buf);
  else
    curTok.span.idx1 = curByte-buf;
  curTok.span.idx2 = curTok.span.idx1;
  
  //
  // Main Byte loop
  //
  
  status = Error::None;
  
  while (true) { // Main Byte Loop...
    
    if ((Int)bufTrig <= 0) {
      status = _PopulateBuffer();
      if (status == Error::LexStreamAtEnd) break;
      if (status) goto SOMEERROR;
    }
    
    byte = *curByte;
    if (byte < 0x80) {
      curChar = Char(byte);
      curCharLen = 1;
    }
    else {
      curChar = UTF8::GetChar(curByte, 4, 0);
      curCharLen = UTF8::GetByteSize(curChar);
      if (curCharLen == 0 && !IsCombining(curChar))
        curCharLen = 1;
      byte = 0x00;
    }
    
    // Get the action depending on the LexState type...
    if (states[curState].type == LexState::TypeChar)
      actionIdx = states[curState].GetCharActionNo(curChar);
    else
      actionIdx = states[curState].byteActions[byte];
    
    // Special case for UTF-8 in names, if allowed...
    if (byte == 0 && curCharLen > 1 && lang.GetUtfInNames() && (curState == Lex::StateMain || curState == Lex::StateName))
    	actionIdx = Lex::ActionAppend;

    // Handle high bit set (Switch State)...
    if (actionIdx >= 0x80 && actionIdx != 0xFF) {
      
      // If we're in the "Main" state and don't have a token type then
      // start an "Operator" token...
      if (curState == Lex::StateMain) {
        if (curTok.type == LexToken::Unknown) {
          curTok.type      = LexToken::Operator;
          curTok.lineCount = 0;
          curTok.value     = String();
        }
        else
          break;
      }
      
      // Append byte and and switch state
      if (curTok.value.Size() < 4)
        curTok.value.Append(curChar);
      //if (byte == '\n')
      //  curTok.lineCount++;
      LexStreamCONSUME;
      if (actionIdx == 0xFF) {
        curTok.type   = LexToken::Unexpected;
        curTok.lineNo = curLineNo;
        curTok.colNo  = curColNo;
        curTok.status = Error::LexUnexpectedChar;
        LexStreamCONSUME;
        break;
      }
      curState = actionIdx & 0x7f;
      if (curState >= numStates) goto BADSTATE;
      
      continue;
      
    }
 
    // Else, if action handled...
    else if (actions && actionIdx < numActions && actions[actionIdx].charHandler != 0) {
      status = (*actions[actionIdx].charHandler)(*this, curChar);
      if (status == Error::LexExitToken)
        goto OK;
      if (status) {
        if (status != Error::LexUnexpectedChar) goto SOMEERROR;
        if (curTok.type != LexToken::Unknown)
          goto OK;
        curTok.type   = LexToken::Unexpected;
        curTok.lineNo = curLineNo;
        curTok.colNo  = curColNo;
        LexStreamCONSUME;
        status = Error::None;
        break;
      }
      continue;
    }
    
    // Else, Handle special case of unexpected character...
    // ??? Note: These two IFs below can be combined...
    else if (actionIdx == 0xFF) {
      if (curTok.type == LexToken::Unknown) {
        curTok.type      = LexToken::Unexpected;
        curTok.lineNo    = curLineNo;
        curTok.colNo     = curColNo;
        curTok.lineCount = 0;
        curTok.status = Error::LexUnexpectedChar;
        LexStreamCONSUME;
        break;
      }
      else if (curTok.type == LexToken::Unexpected) {
        LexStreamCONSUME;
        break;
      }
      else {
        curTok.type      = LexToken::Unexpected;
        curTok.lineCount = 0;
        curTok.lineNo    = curLineNo;
        curTok.colNo     = curColNo;
        curTok.span.idx1 = curTok.span.idx2;
        curTok.lineCount = 0;
        curTok.status = Error::LexUnexpectedChar;
        LexStreamCONSUME;
        goto OK;
      }
    }

    goto UNHANDLED;
    
  } // ...Main Byte Loop
  
  // Handle end of stream...
  if (status == Error::LexStreamAtEnd) {
    if (curTok.type == LexToken::Unknown) {
      curTok.type      = LexToken::EndOfInput;
      curTok.lineNo    = curLineNo;
      curTok.colNo     = curColNo;
      curTok.lineCount = 0;
    }
    status = Error::None;
  }
  
  // Handle errors
  while (false) {
    OK:        status = Error::None;                break;
    UNHANDLED: status = Error::LexActionNotHandled; break;
    BADSTATE:  status = Error::LexStateNotExist;    break;
    SOMEERROR:                                      break;
  }

  // Tidy some last things up if no error...
  if (status == Error::None) {

    // Bump nonWhiteCount if not WhiteSpace or EndOfLine...
    if (curTok.type != LexToken::Unknown    &&
        curTok.type != LexToken::WhiteSpace &&
        curTok.type != LexToken::EndOfLine)
    nonWhiteCount++;

  }
  
  // Return the token to the caller  
  theToken = curTok;
  if (curTok.type != LexToken::EndOfInput) {
		if (!status) {
			curTok.Reset();
			curTok.type = LexToken::Unknown;
		}
		else {
			curTok.type = LexToken::Error;
			curTok.lineCount = 0;
		}
  }

  return status;
  
} // ...LexStream.NextToken

} // ...Namespace BEFE
