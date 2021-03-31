//----------------------------------------------------------------------
// File: Lines.cpp - Implementation of the Lines class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Lines.h"
#include "SBuffer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Lines::Lines() {
  StartUp();
}

Lines::~Lines() {
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Lines::StartUp() {

  fileName.StartUp();
  buffer.StartUp();
  numBytes = 0;
  lineOffsets.StartUp();
  numLines = 0;
  
  return Error::None;
  
}

Status Lines::ShutDown() {
  
  fileName.ShutDown();
  buffer.ShutDown();
  lineOffsets.ShutDown();
  
  return Error::None;
  
}

Status Lines::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

UInt Lines::Length() {
  return numLines;
}

//----------------------------------------------------------------------
//
// Loading
//

Status Lines::Load(const char *fname) {
  return Load(fname, false);
}

Status Lines::Load(const char *fname, Boolean trimwhite) {

  Status status;
  String sfname;

  sfname = fname;
  status = Load(sfname,trimwhite);
  return status;
}

Status Lines::Load(String const &fname) {
  return Load(fname, false);
}

Status Lines::Load(String const &fname, Boolean trimwhite) {

  Status status;
  File   file;

  file.SetName(fname);
  status = Load(file,trimwhite);

  return status;
}

Status Lines::Load(File &file) {
  return Load(file, false);
}

Status Lines::Load(File &file,Boolean trimwhite) {

  Status  status;
  UInt    readBytes;
  Byte   *srcBuf;
  
  status = Reset();
  if (status) goto SOMEERROR;
  
  fileName = file.GetName();
  
  numBytes = file.Size();
  if ( !IsNull(numBytes) ) {
    status = buffer.Allocate(numBytes+1,0);
    if (status) goto SOMEERROR;
    status = file.Read(0,buffer.GetBuf(),numBytes,readBytes);
    if (status) goto SOMEERROR;
    if (readBytes != numBytes) goto READFAILED;
    
    status = buffer.SetSize(readBytes);
    if (status) goto SOMEERROR;
    
    // NUL terminate the whole lot
    srcBuf = buffer.GetBuf();
    if (IsNull(srcBuf)) goto NULLPOINTER;
    *(srcBuf + readBytes) = 0x00;
    
  }

  status = _Build(trimwhite);
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                                        break;
    READFAILED:  status = Error::FileReadFailed;      break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
  }

  if (status)
    Reset();
    
  return status;
  
}

String Lines::GetLine(UInt index) {
  
  Status status;
  String line;
  
  status = GetLine(index,line);
  if (status)
    BEFE_ERROR("Lines.GetLine",status);
    
  return line.Consumable();
  
}

Status Lines::GetLine(UInt index, String &line) {

  Status status;
  Byte   *srcBuf;
  UInt   *uInts;

  if (index > numLines) goto OUTOFRANGE;
  srcBuf = buffer.GetBuf();
  if (IsNull(srcBuf)) goto NULLPOINTER;
  uInts = (UInt *)lineOffsets.GetBuf();
  if (IsNull(uInts)) goto NULLPOINTER;
  line = String((const char *)(srcBuf + uInts[index]));
  
  // Handle errors  
  status = Error::None;
  while (false) {
    OUTOFRANGE:  status = Error::IndexOutOfRange;     break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Protected and Internal methods
//

Status Lines::_Build(Boolean trimwhite) {

  Status  status;
  Byte   *srcBuf;
  Int     srcLen;
  Byte   *lf;
  UInt   *uInts;
  Boolean haveWarned;
  
  numLines = 0;
  srcBuf = buffer.GetBuf();
  srcLen = buffer.GetUsedBytes();
  if (srcLen == 0) goto OK;
  if (IsNull(srcBuf)) goto NULLPOINTER;
  
  // First pass, count the number of lines...
  lf = srcBuf;
  while (lf < srcBuf+srcLen) {
    lf = Memchr(lf, '\n', srcLen - (lf-srcBuf));
    if (lf) {
      lf++;
      numLines++;
    }
    else
      break;
  };
  if (srcLen)
    numLines++;
    
  // Allocate the line offsets
  if (numLines <= 0) goto OK;
  status = lineOffsets.Allocate(numLines*sizeof(UInt),0);
  if (status) goto SOMEERROR;

  // Second pass, set up the line offsets...
  lf = srcBuf;
  uInts = (UInt *)lineOffsets.GetBuf();
  if (IsNull(uInts)) goto NULLPOINTER;
  haveWarned = false;
  while (lf < srcBuf+srcLen) {
    *uInts = lf - srcBuf;
    lf = Memchr(lf, '\n', srcLen - (lf-srcBuf));
    if (lf)
      *lf++ = 0;
    else
      lf = srcBuf+srcLen;
    // If trimwhite...
    if (trimwhite && !haveWarned) {
      BEFE_WARN("Lines.Build: trimwhite not implemented yet");
      haveWarned = true;
    }
    uInts++;
  };

  // Handle errors
  status = Error::None;
  while (false) {
    //OUTOFMEMORY: status = Error::OutOfMemory; break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
    OK:          status = Error::None;                break;
  }

  return status;

}

void Lines::_BufAndSize(Byte *&buf, UInt &size) {
  buf = buffer.GetBuf();
  size = buffer.GetUsedBytes();
}

Status Lines::_LineAndColumn(UInt &bufOff, UInt &lineNo, UInt &colNo) {
  return Error::NotImplemented;
}

} // ...Namespace BEFE
