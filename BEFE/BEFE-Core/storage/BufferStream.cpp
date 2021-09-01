//----------------------------------------------------------------------
// File: BufferStream.cpp - Base implementation of the BufferStream class
//----------------------------------------------------------------------
//
// This is the implementation of the BufferStream class.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BufferStream.h"
#include "Buffer.h"
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

//
// Handy macros
//

#define ALLOCBUF if (!buffer.pointer) {status=AllocBuffer(); if (status) goto SOMEERROR;}

// To get around silly 'const' crap...
#define THIS (*(BufferStream *)this)
#define THAT (*(FufferStream *)&that)

//
// Class (static) members
//

StreamStats BufferStream::classStats;

//----------------------------------------------------------------------
//
// Class (static) Methods
//

StreamStats BufferStream::ClassStats() {
  StreamStats stats;
  ClassStats(stats);
  return stats;
}

Status BufferStream::ClassStats(StreamStats &stats) {
  stats = classStats;
  return Error::None;
}

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

BufferStream::BufferStream() {
  StartUp();
}


BufferStream::BufferStream(BufferStream const &that) {
  BEFE_ABORT("BufferStream copy constructor not implemented yet");
}

BufferStream::~BufferStream() {
  ShutDown();
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status BufferStream::StartUp() {

  Status status;

  setPosCalled = false;
  curPos       = 0;
  status = buffer.StartUp();
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
	  SOMEERROR: break;
  }

  return status;

}

Status BufferStream::ShutDown() {

  Status status;

  status = buffer.ShutDown();

  return status;

}

Status BufferStream::Reset() {
  return buffer.Reset();
}

Boolean BufferStream::IsNull() const {
  return buffer.IsNull();
}

Status BufferStream::SetNull() {
  return buffer.SetNull();
}

Boolean BufferStream::IsEmpty() const {
  return buffer.IsEmpty();
}

Status BufferStream::SetEmpty() {
  return buffer.SetEmpty();
}

//----------------------------------------------------------------------
//
// Extra BufferStream methods
//

Byte *BufferStream::GetBuf() const {
  return buffer.GetBuf();
}

//----------------------------------------------------------------------
//
// Capabilities
//

Int BufferStream::GetCapability() const {

  return FixedElementSize |
         KnownSize        |

         CanPosition      |
         CanPut           |
         CanGet           |
         CanFlush         |
         CanTruncate      |
         CanLookAhead     |
         CanLookBehind    |

         IsRaw            |
         IsUTF8           |
         IsBigEndian      |
         0;
}

UInt BufferStream::GetElementSize() const {
  return 1;
}

Status BufferStream::SetElementSize(UInt elmsiz) {
  return Error::NotCapable;
}

UInt BufferStream::Length() const {
  return buffer.Length();
}

UInt BufferStream::Size() const {
  return buffer.Size();
}

//----------------------------------------------------------------------
//
// Positional Range
//

UInt BufferStream::GetPos() const {
  return curPos;
}

Status BufferStream::SetPos(UInt pos) {

  Status status;

  // Error if beyond end
  if (pos > buffer.Size()) goto ATEND;

  // Set it
  curPos       = pos;
  setPosCalled = true;
  
  // Handle errors
  status = Error::None;
  while (false) {
    ATEND:     status = Error::StreamAtEnd; break;
  }

  return status;

}

Range BufferStream::PosRange() const {

  Range  posRange;

  posRange = Range(0,buffer.Length()-1);

  return posRange;

}

Status BufferStream::NextPos() {

  Status status;

  // Set the position
  if ( BEFE::IsNull(curPos) ) {
    status = SetPos(0);
    if (status) goto SOMEERROR;
  }
  else if (curPos >= buffer.Length())
    goto ATEND;
  else {
    status = SetPos(curPos+1);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    ATEND:     status = Error::StreamAtEnd; break;
    SOMEERROR:                              break;
  }

  return status;

}

Status BufferStream::PreviousPos() {

  Status status;

  // Set the position
  if ( BEFE::IsNull(curPos) ) {
    status = SetPos(0);
    if (status) goto SOMEERROR;
  }
  else if (curPos == 0)
    goto ATBEGIN;
  else {
    status = SetPos(curPos-1);
    if (status) goto SOMEERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                                    break;
    ATBEGIN:   status = Error::StreamAtBeginning; break;
  }

  return status;

}

Status BufferStream::RewindPos() {
  return SetPos(0);
}

Status BufferStream::EndPos() {

  Status status;

  // Set the position
  status = SetPos(buffer.Length());

  return status;

}

Status BufferStream::SetSize(UInt newSize) {

  Status status;

  status = buffer.SetUsedBytes(newSize);
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                       break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Buffering
//
// Note: Address returned by GetBuffer is transient
//

Byte *BufferStream::GetBuffer() const {
  return buffer.GetBuf();
}

UInt BufferStream::GetBufferSize() const {
  return gDefaultBufSize;
}

Status BufferStream::SetBufferSize(UInt size) {
  return Error::NotCapable;
}

//----------------------------------------------------------------------
//
// Gets and Puts...
//

Status BufferStream::GetElement(Byte *b) {

  Status status;
  UInt   numRead;

  status = GetBytes(b,1,numRead);
  if (status) goto SOMEERROR;
  if (numRead != 1) goto READFAILED;

  // Handle errors
  status = Error::None;
  while (false) {
    READFAILED: status = Error::FileReadFailed; break;
    SOMEERROR:                                  break;
  }

  return status;

}

Status BufferStream::GetBytes(Byte *buf, UInt bufLen, UInt &numRead) {

  Status  status;
  Byte   *theBuf;
  UInt    theBufLen;
  UInt    theBufRem;

  Byte   *curByte;
  UInt    numIn;

  numRead      = 0;
  setPosCalled = false;
  
  // Validate parameters
  if ( BEFE::IsNull(buf) ||  BEFE::IsNull(bufLen) ) goto INVALIDPARAMETER;

  // Set to position 0 if not set
  if ( BEFE::IsNull(curPos) ) curPos = 0;

  // Until we hit a '\n'...
  while (bufLen) {

    // Now, get the address of where we are in the buffer
    theBuf     = buffer.GetBuf();
    theBufLen  = buffer.GetAllocBytes();
    theBufRem  = theBufLen - curPos;
    curByte    = theBuf + curPos;

    // Read the bytes
    numIn = Min(bufLen,theBufRem);
    MemoryCopyRaw(curByte,buf,numIn);

    // Update curPos, buf, bufLen, and caller's numRead
    curPos  += numIn;
    buf     += numIn;
    bufLen  -= numIn;
    numRead += numIn;

    classStats.numLogReads++;
    classStats.numBytesRead += numIn;

  };

  // Handle errors
  status = Error::None;
  while (false) {
    INVALIDPARAMETER: status = Error::InvalidParameter; break;
  }

  return status;

}

Status BufferStream::PutElement(Byte *b) {

  Status status;
  UInt   numWritten;

  status = PutBytes(b,1,numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != 1) goto WRITEFAILED;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                                    break;
    WRITEFAILED: status = Error::FileWriteFailed; break;
  }

  return status;

}

Status BufferStream::PutBytes(Byte *buf, UInt bufLen, UInt &numWritten) {

  Status  status;
  UInt    moveLen;
  
  setPosCalled = true;
  numWritten = 0;
  classStats.numLogWrites++;
  
  // Allocate the buffer if it' not already
  status = AllocBuffer(bufLen);
  if (status) goto SOMEERROR;
  
  // 
  // Move to existing buffer first
  //
  
  if (curPos < buffer.Length()) {
    
    moveLen = Min(bufLen, buffer.Length()-curPos);
    MemoryCopyRaw(buf,buffer.GetBuf()+curPos, moveLen);
    numWritten += moveLen;
    buf        += moveLen;
    bufLen     -= moveLen;
    curPos     += moveLen;
    classStats.numBytesWritten += moveLen;
    
  }
  
  //
  // Append the rest to the end of the buffer
  //
  
  status = buffer.AppendBytes(buf, bufLen);
  if (status) goto SOMEERROR;
  numWritten      += bufLen;
  curPos          += bufLen;
  classStats.numBytesWritten += bufLen;
  
  //
  // Handle errors
  //
  
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Line I/O
//

String BufferStream::GetLine() {

  //Status status;
  String line;

  /*status =*/ GetLine(line);

  return line.Consumable();
}

Status BufferStream::GetLine(String &line) {

  Status  status;
  Byte   *theBuf;
  UInt    theBufLen;
  UInt    theBufRem;

  Byte   *curByte;
  Byte   *lf;
  UInt    numIn;
  String  theLine;

  // Set to position 0 if not set
  if ( BEFE::IsNull(curPos) ) curPos = 0;
  setPosCalled = false;
  
  // Clear caller's string first...
  line.SetEmpty();

  // Until we hit a '\n'...
  do {

    // Now, get the address of where we are in the buffer
    theBuf     = buffer.GetBuf();
    theBufLen  = buffer.GetUsedBytes();
    theBufRem  = theBufLen - curPos;
    curByte    = theBuf    + curPos;
    
    // If no buffer left, we're finished
    if (theBufRem == 0) break;

    // Find the next '\n'
    lf = Memchr(curByte,'\n',theBufRem);

    // Append the non-LF bytes to the line (if any)
    if (lf)
      numIn = lf-curByte;
    else
      numIn = theBufRem;
    if (numIn) {
      status = line.Append(curByte,numIn);
      if (status) goto SOMEERROR;
    }
      
    // Update curPos
    curPos += numIn;
    if (lf) curPos++;

    // Update Class statistics
    classStats.numLogReads++;
    classStats.numBytesRead += numIn;
    if (lf)
      classStats.numBytesRead++;

    // If we found an LF, we're finished
    if (lf || curPos >= buffer.Size()) break;

  } while (true);

  // Update Class statistics
  classStats.numLineReads++;

  // If we're at the end (line is empty), say so
  if ((theBufRem == 0 || curPos >= buffer.Size()) && line.Length() == 0) goto ATEND;

  // Handle errors
  status = Error::None;
  while (false) {
    ATEND:     status = Error::StreamAtEnd; break;
    SOMEERROR:                              break;
  }

  return status;

}

Status BufferStream::PutLine(String &line) {

  Status  status;

  Byte   *strBuf;
  UInt    strLen;
  UInt    strWrote;
  Byte    lf;
  
  // If the caller hasn't called SetPos since the last Write, set
  // the position to the end of file...
  if (!setPosCalled) {
    curPos = Size();
    if ( BEFE::IsNull(curPos) ) goto GETSIZEFAILED;
  }

  // Get the String buffer and length...
  line._BufAndSize(strBuf,strLen);

  // If we have something to write, write it
  if (strBuf && strLen) {
    status = PutBytes(strBuf,strLen,strWrote);
    if (status) goto SOMEERROR;
  }

  // Output the LF...
  lf = '\n';
  status = PutBytes(&lf,(UInt)1,strWrote);
  if (status) goto SOMEERROR;

  // Update Class statistics
  classStats.numLineWrites++;

  // Handle errors
  while (false) {
    GETSIZEFAILED: status = Error::FileGetSizeFailed; break;
    SOMEERROR:                                        break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Flushing if buffered and Putting...
//

Status BufferStream::Flush() {
  return Error::None;
}

//----------------------------------------------------------------------
//
// Protected methods
//

Status BufferStream::AllocBuffer() {

  Status status;

  if (buffer.pointer) goto OK;

  // For now, we'll allocate a fixed size buffer
  status = buffer.Allocate(gDefaultBufSize,UNaN);
  if (status) goto SOMEERROR;

  goto OK;

  // Handle errors
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }

  return status;

}

Status BufferStream::AllocBuffer(UInt size) {

  Status status;

  if (buffer.pointer) goto OK;

  // For now, we'll allocate a fixed size buffer
  status = buffer.Allocate(size,UNaN);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }

  return status;

}

} // ...Namespace BEFE
