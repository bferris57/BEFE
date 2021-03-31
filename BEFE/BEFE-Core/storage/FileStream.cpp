//----------------------------------------------------------------------
// File: FileStream.cpp - Base implementation of the FileStream class
//----------------------------------------------------------------------
//
// This is the basic empty implementation of the FileStream class.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "FileStream.h"
#include "Buffer.h"
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

//
// Handy macros
//

#define ALLOCBUF if (!buffer.pointer) {status=AllocBuffer(); if (status) goto SOMEERROR;}

// To get around silly 'const' crap...
#define THIS (*(FileStream *)this)
#define THAT (*(FileStream *)&that)

//
// Class (static) members
//

StreamStats FileStream::classStats;

//----------------------------------------------------------------------
//
// Class (static) Methods
//

StreamStats FileStream::ClassStats() {
  StreamStats stats;
  ClassStats(stats);
  return stats;
}

Status FileStream::ClassStats(StreamStats &stats) {
  stats = classStats;
  return Error::None;
}

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

FileStream::FileStream() {
  StartUp();
}

FileStream::FileStream(char const *name) {
  StartUp();
  SetName(name);
}

FileStream::FileStream(String const &name) {
  StartUp();
  SetName(name);
}

FileStream::FileStream(FileStream const &fileStream) {
  BEFE_ABORT("FileStream.FileStream(FileStream const &) Not Implemented");
}

FileStream::~FileStream() {
  ShutDown();
}

FileStream &FileStream::operator=(char const *name) {
  SetName(name);
  return *this;
}

FileStream &FileStream::operator=(String const &name) {
  SetName(name);
  return *this;
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status FileStream::StartUp() {

  Status retStatus;
  Status status;
  
  retStatus = file.StartUp();
  fileSize = UNaN;
  curPos   = 0;
  bufPos   = UNaN;
  status = buffer.StartUp();
  if (status && !retStatus) retStatus = status;
  
  isDirty      = false;
  setPosCalled = false;

  return retStatus;

}

Status FileStream::ShutDown() {

  Status retStatus;
  Status status;

  retStatus = Flush();
  status = file.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = buffer.ShutDown();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Status FileStream::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status    = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Boolean FileStream::IsNull() const {
  return (/*file.IsNull() &&*/
          BEFE::IsNull(fileSize) &&
          BEFE::IsNull(curPos)   &&
          BEFE::IsNull(bufPos)   &&
          buffer.IsNull());
}

Status FileStream::SetNull() {
  return Reset();
}

Status FileStream::MoveFrom(FileStream const &that) {
  
  Status status;
  
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(FileStream));
  status = ((FileStream *)&that)->StartUp();
  
  return status;
  
}

Status FileStream::CopyFrom(FileStream const &that) {
  
  Status retStatus;
  Status status;

  retStatus = Reset();  
  status = file.CopyFrom(that.file);
  if (status && !retStatus) retStatus = status;
  fileSize = that.fileSize;
  curPos   = that.curPos;
  bufPos   = that.bufPos;
  status = buffer.CopyFrom(that.buffer);
  if (status && !retStatus) retStatus = status;
  isDirty  = that.isDirty;
  setPosCalled = that.setPosCalled;
  
  return retStatus;
  
}

FileStream &FileStream::Consumable()    const {return *(FileStream *)this;}
FileStream &FileStream::NotConsumable() const {return *(FileStream *)this;}
Boolean     FileStream::IsConsumable()  const {return false;}

//----------------------------------------------------------------------
//
// Access...
//

Status FileStream::GetAccess(UInt &mode) const {
  return file.GetAccess(mode);
}

UInt FileStream::GetAccess() const {
  
  UInt mode;
  mode = File::ModeNone;
  file.GetAccess(mode);
  
  return mode;
  
}

Status FileStream::SetAccess(UInt mode) {
  return file.SetAccess(mode);
}

Status FileStream::ValidateBOM() {
  
  Status  status;
  UInt    oldPos;
  UInt    streamSize;
  Byte    bytes[4];
  UInt    numRead;
  UInt    len;
  Boolean isBOM;
  
  // Save old position and get size
  oldPos = GetPos();
  streamSize = Size();
  if (BEFE::IsNull(streamSize)) goto INTERNAL;
  
  // Position to beginning of stream and read first 4 bytes...
  status = SetPos(0);
  if (status) goto SOMEERROR;
  len = Min(4, streamSize);
  status = _GetBytes(bytes, len, numRead);
  if (status) goto SOMEERROR;
  
  // Validate it...
  status = file.ValidateBOM(bytes, numRead);
  if (status && status != Error::CommandBOM) goto SOMEERROR;
  isBOM = (status == Error::CommandBOM);
  
  // Reposition...
  status = SetPos(oldPos);
  if (status) goto SOMEERROR;
  
  // Fudge return status
  if (isBOM)
    status = Error::CommandBOM;
    
  // Handle errors
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Naming...
//

String FileStream::GetName() const {
  return file.GetName();
}

Status FileStream::SetName() {
  ShutDown();
  StartUp();
  return file.SetName();
}

Status FileStream::SetName(const char *name) {
  Status status;
  if (file.GetName() != name) {
    ShutDown();
    StartUp();
    status = file.SetName(name);
  }
  else
    status = Error::None;
  return status;
}

Status FileStream::SetName(String const &name) {
  ShutDown();
  StartUp();
  return file.SetName(name);
}

//----------------------------------------------------------------------
//
// Query
//

Boolean FileStream::IsNameValid() const {
  return file.IsNameValid();
}

UInt FileStream::GetMaxLength() const {
  return file.GetMaxLength();
}

UInt FileStream::GetMaxSize() const {
  return file.GetMaxSize();
}

Boolean FileStream::IsOpen() const {
  return file.IsOpen();
}

//----------------------------------------------------------------------
//
// Capabilities
//

Int FileStream::GetCapability() const {

  return FixedElementSize |
         KnownSize        |

         CanPosition      |
         CanPut           |
         CanGet           |
         CanFlush         |
         CanTruncate      |
         //CanLookAhead     |
         //CanLookBehind    |

         IsRaw            |
         IsUTF8           |
         IsBigEndian      |
         0;
}

UInt FileStream::GetElementSize() const {
  return 1;
}

Status FileStream::SetElementSize(UInt elmsiz) {
  return Error::NotCapable;
}

UInt FileStream::Length() const {
  return file.Length();
}

UInt FileStream::Size() const {
  if ( BEFE::IsNull(fileSize) )
    THIS.fileSize = file.Size();
  return fileSize;
}

//----------------------------------------------------------------------
//
// Positional Range
//

UInt FileStream::GetPos() const {
  return curPos;
}

Status FileStream::SetPos(UInt pos) {

  Status status;

  // Get file Size if we don't have it
  if ( BEFE::IsNull(fileSize) ) {
    fileSize = file.Size();
    if ( BEFE::IsNull(fileSize) )
      fileSize = 0;
  }

  // Error if beyond end
  if ( BEFE::IsNull(fileSize) || pos > fileSize) goto ATEND;

  // Nothing to do if same position
  if (pos == curPos) goto OK;

  // Read the buffer if we need to...
  status = PositionBuffer(pos);
  if (status) goto SOMEERROR;

  // Set it
  curPos = pos;
  setPosCalled = true;

  // Handle errors
  while (false) {
    ATEND:     status = Error::StreamAtEnd; break;
    OK:        status = Error::None;        break;
    SOMEERROR:                              break;
  }

  return status;

}

Range FileStream::PosRange() const {

  Range  posRange;

  posRange = Range(NaN,NaN);

  // Get file size if we don't already have it
  if ( BEFE::IsNull(fileSize) ) {
    THIS.fileSize = file.Size();
    if ( BEFE::IsNull(fileSize) ) goto OK;
  }

  // Set the range based on the file size
  posRange.idx1 = 0;
  posRange.idx2 = fileSize-1;

OK:

  return posRange;

}

Status FileStream::NextPos() {

  Status status;

  // Get file size if we don't already have it
  if ( BEFE::IsNull(fileSize) ) {
    fileSize = file.Size();
    if ( BEFE::IsNull(fileSize) ) goto ATEND;
  }

  // Set the position
  if ( BEFE::IsNull(curPos) )
    status = SetPos(0);
  else if (curPos >= fileSize)
    goto ATEND;
  else
    status = SetPos(curPos+1);

  // Handle errors
  status = Error::None;
  while (false) {
    ATEND: status = Error::StreamAtEnd; break;
  }

  return status;

}

Status FileStream::PreviousPos() {

  Status status;

  // Get file size if we don't already have it
  if ( BEFE::IsNull(fileSize) ) {
    fileSize = file.Size();
    if ( BEFE::IsNull(fileSize) ) goto ATEND;
  }

  // Set the position
  if ( BEFE::IsNull(curPos) )
    status = SetPos(0);
  else if (curPos == 0)
    goto ATBEGIN;
  else
    status = SetPos(curPos-1);

  // Handle errors
  status = Error::None;
  while (false) {
    ATBEGIN: status = Error::StreamAtBeginning; break;
    ATEND:   status = Error::StreamAtEnd;       break;
  }

  return status;

}

Status FileStream::RewindPos() {
  return SetPos(0);
}

Status FileStream::EndPos() {

  Status status;

  // Get file size if we don't already have it
  if ( BEFE::IsNull(fileSize) ) {
    fileSize = file.Size();
    if ( BEFE::IsNull(fileSize) ) goto ATEND;
  }

  // Set the position
  status = SetPos(fileSize);

  // Handle errors
  status = Error::None;
  while (false) {
    ATEND: status = Error::StreamAtEnd; break;
  }

  return status;

}

Status FileStream::SetSize(UInt newSize) {

  Status status;
  UInt   access;

  status = file.SetSize(newSize);
  if (status && status != Error::FileNotOpenForWrite) goto SOMEERROR;
  if (!status) goto OK;

  // Wasn't open for write, close it and reopen it with write access...
  status = file.GetAccess(access);
  if (status) goto SOMEERROR;
  access |= File::ModeWrite | File::ModeWriteThrough;
  status = file.SetAccess(access);
  if (status) goto SOMEERROR;

  // Try again...
  status = file.SetSize(newSize);
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
// Buffering
//

UInt FileStream::GetBufferSize() {
  return gDefaultBufSize;
}

Status FileStream::SetBufferSize(UInt size) {
  return Error::NotCapable;
}

//----------------------------------------------------------------------
//
// Gets and Puts...
//

Status FileStream::GetElement(Byte *b) {

  Status status;
  UInt   numRead;

  status = GetBytes(b,1,numRead);
  if (status) goto SOMEERROR;
  if (numRead != 1) goto READFAILED;

  // Handle errors
  while (false) {
    READFAILED: status = Error::FileReadFailed; break;
    SOMEERROR:                                  break;
  }

  return status;

}

Status FileStream::_GetBytes(Byte *buf, UInt bufLen, UInt &numRead) {

  Status  status;
  Byte   *theBuf;
  UInt    theBufRem;

  Byte   *curByte;
  UInt    numIn;

  numRead      = 0;
  setPosCalled = false;

  // If it's not open, open it...
  if (!file.IsOpen()) {
    status = file.Open();
    if (status) goto SOMEERROR;
  }
  
  // Make sure it's okay to read...
  if (!file.CanRead()) goto CANTREAD;
  
  // Validate parameters
  if (BEFE::IsNull(buf) || BEFE::IsNull(bufLen) ) goto INVALIDPARAMETER;

  // Set to position 0 if not set
  if ( BEFE::IsNull(curPos) ) curPos = 0;

  // Until we're finished or ran out...
  while (bufLen) {

    // Read the buffer if we need to
    status = PositionBuffer(curPos);
    if (status) goto SOMEERROR;

    // Now, get the address of where we are in the buffer
    theBuf     = buffer.GetBuf();
    theBufRem  = buffer.GetUsedBytes() - (curPos-bufPos);
    if (theBufRem == 0) break;
    curByte    = theBuf + (curPos-bufPos);

    // Read the bytes
    numIn = Min(bufLen, theBufRem);
    MemoryCopyRaw(curByte, buf, numIn);

    // Update curPos, buf, bufLen, and caller's numRead
    curPos  += numIn;
    buf     += numIn;
    bufLen  -= numIn;
    numRead += numIn;

    classStats.numLogReads++;
    classStats.numBytesRead += numIn;

  }

  // Handle errors
  status = Error::None;
  while (false) {
    CANTREAD:         status = Error::FileNotOpenForRead; break;
    INVALIDPARAMETER: status = Error::InvalidParameter;   break;
    SOMEERROR:                                            break;
  }

  return status;

}

Status FileStream::GetBytes(Byte *buf, UInt bufLen, UInt &numRead) {
  return _GetBytes(buf, bufLen, numRead);
}

Status FileStream::PutElement(Byte *b) {

  Status status;
  UInt   numWritten;

  status = PutBytes(b,1,numWritten);
  if (status) goto SOMEERROR;
  if (numWritten != 1) goto WRITEFAILED;

  // Handle errors
  while (false) {
    SOMEERROR:                                    break;
    WRITEFAILED: status = Error::FileWriteFailed; break;
  }

  return status;

}

Status FileStream::_PutBytes(Byte *buf, UInt bufLen, UInt &numWritten) {

  Status  status;

  Byte   *outBuf;
  UInt    outBufLen;
  UInt    outBufRem;
  UInt    newUsedBytes;

  Byte   *curByte;
  UInt    numOut;

  setPosCalled = false;
  numWritten   = 0;

  // If it's not open, open it...
  if (!file.IsOpen()) {
    status = file.Open();
    if (status) goto SOMEERROR;
  }
  
  // Make sure it's okay to write...
  if (!file.CanWrite()) goto CANTWRITE;
  
  // If no current Position, set it to End Of File...
  if ( BEFE::IsNull(curPos) ) {
    curPos = Size();
    if ( BEFE::IsNull(curPos) ) goto GETSIZEFAILED;
  }

  // Until we're done writing...
  while (bufLen) {

    // Setup the Buffer
    status = PositionBuffer(curPos);
    if (status) goto SOMEERROR;

    // Now, get the address of where we are in the buffer
    outBuf     = buffer.GetBuf();
    outBufLen  = buffer.GetAllocBytes();
    curByte    = outBuf + (curPos-bufPos);
    outBufRem  = outBufLen - (curByte-outBuf);
    if (outBufRem == 0) goto NUMREM0;

    // Write it to the buffer...
    numOut = Min(bufLen,outBufRem);
    if (numOut == 0) goto NUMOUT0;
    MemoryCopyRaw(buf,curByte,numOut);
    isDirty = true;

    // Update curPos, bufLen, and buf
    curPos     += numOut;
    bufLen     -= numOut;
    buf        += numOut;
    numWritten += numOut;

    // Update File Size if we extended it
    if ( BEFE::IsNull(fileSize) || curPos > fileSize)
      fileSize = curPos;

    // Update buffer
    newUsedBytes = Max(buffer.GetUsedBytes(),(curPos-bufPos));
    status = buffer.SetUsedBytes(newUsedBytes);
    if (status) goto SOMEERROR;

    classStats.numLogWrites++;
    classStats.numBytesWritten += numOut;

  };

  // Handle errors
  status = Error::None;
  while (false) {
    CANTWRITE:     status = Error::FileNotOpenForWrite;                                      break;
    GETSIZEFAILED: status = Error::FileGetSizeFailed;                                        break;
    NUMREM0:       BEFE_WARN("FileStream.PutBytes - numRem==0"); status = Error::Internal;   break;
    NUMOUT0:       BEFE_WARN("FileStream.PutBytes - numOut==0"); status = Error::Internal;   break;
    SOMEERROR:                                                                               break;
  }

  return status;

}

Status FileStream::PutBytes(Byte *buf, UInt bufLen, UInt &numWritten) {
  return _PutBytes(buf, bufLen, numWritten);
}

//----------------------------------------------------------------------
//
// Line I/O
//

String FileStream::GetLine() {

  //Status status;
  String line;

  /*status =*/ GetLine(line);

  return line.Consumable();

}

Status FileStream::GetLine(String &line) {

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

    // Read the buffer if we need to
    status = PositionBuffer(curPos);
    if (status) goto SOMEERROR;

    // Now, get the address of where we are in the buffer
    theBuf     = buffer.GetBuf();
    theBufLen  = buffer.GetUsedBytes();
    theBufRem  = theBufLen - (curPos-bufPos);
    curByte    = theBuf    + (curPos-bufPos);

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
    if (lf || curPos >= fileSize) break;

  } while (true);

  // Update Class statistics
  classStats.numLineReads++;

  // If we're at the end (line is empty), say so
  if (curPos >= fileSize && line.Length() == 0) goto ATEND;

  // Handle errors
  status = Error::None;
  while (false) {
    ATEND:     status = Error::StreamAtEnd; break;
    SOMEERROR:                              break;
  }

  return status;

}

Status FileStream::PutLine(String &line) {

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
  status = Error::None;
  while (false) {
    GETSIZEFAILED: status = Error::FileGetSizeFailed; break;
    SOMEERROR:                                        break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Multiple Line I/O
//

Status FileStream::GetLines(Strings &lines) {
  
  Status status;
  
  status = lines.Reset();
  if (status) goto SOMEERROR;
  
  BEFE_WARN("FileStream.GetLines(Strings &) not implemented yet");  
  status = Error::NotImplemented;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
}

//----------------------------------------------------------------------
//
// Flushing if buffered and Putting...
//

Status FileStream::Flush() {
  Status status;
  status = FlushBuffer();
  return status;
}

//----------------------------------------------------------------------
//
// Protected methods
//

Status FileStream::AllocBuffer() {

  Status status;

  if (buffer.pointer) goto OK;

  // For now, we'll allocate a fixed size buffer
  status = buffer.Allocate(gDefaultBufSize,0);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }

  return status;

}

Status FileStream::PositionBuffer(UInt newPos) {

  Status  status;
  Byte   *dstAddress;
  UInt    numRead;
  UInt    newBufPos;

  // Get file Size if we don't have it
  if ( BEFE::IsNull(fileSize) ) {
    fileSize = file.Size();
    if ( BEFE::IsNull(fileSize) )
      fileSize = 0;
  }

  // Error if beyond end
  if ( BEFE::IsNull(fileSize) || newPos > fileSize) goto ATEND;

  // Adjust new Buffer position
  newBufPos = newPos/gDefaultBufSize*gDefaultBufSize;
  if (newBufPos == bufPos) goto OK;

  // Allocate the buffer if not already
  ALLOCBUF

  // Flush current buffer if needed
  status = FlushBuffer();
  if (status) goto SOMEERROR;

  // Read the Block...
  dstAddress = buffer.GetBuf();
  if (BEFE::IsNull(dstAddress)) goto NOBUF;
  numRead = 0;
  status = file.Read(newBufPos,dstAddress,gDefaultBufSize,numRead);
  if (status) goto SOMEERROR;
  classStats.numPhyReads++;

  // Update buffer position and size
  bufPos = newBufPos;
  status = buffer.SetUsedBytes(numRead);
  if (status) goto SOMEERROR;

  // Update EOF if it changed
  fileSize = Max(fileSize,bufPos+numRead);

  // Handle errors
  while (false) {
    ATEND:     status = Error::StreamAtEnd;
               break;
    NOBUF:     status = Error::InternalNullPointer;
               BEFE_WARN("FileStream.ReadBuffer has NULL Buffer address");
               break;
    SOMEERROR: break;
    OK:        status = Error::None;
               break;
  }

  return status;

}

Status FileStream::FlushBuffer() {

  Status status;
  UInt   numToWrite;
  UInt   numWrote;

  // No point if nothing to do
  if (!isDirty                     ||
      //IsNull(fileSize)             ||
      BEFE::IsNull(bufPos)         ||
      BEFE::IsNull(buffer.pointer)
     ) goto OK;

  // Get number to write
  numToWrite = buffer.GetUsedBytes();
  if (numToWrite == 0) goto OK;

  // Write it...
  status = file.Write(buffer.GetBuf(), bufPos, numToWrite, numWrote);
  isDirty = false;
  classStats.numPhyWrites++;
  if (status) goto SOMEERROR;
  if (numWrote != numToWrite) goto INTERNALERROR;

  // Handle errors
  while (false) {
    SOMEERROR:     break;
    INTERNALERROR: status = Error::Internal;
                   BEFE_WARN("FileStream.FlushBuffer didn't write correct number of Bytes");
                   break;
    OK:            status = Error::None;
                   break;
  }

  return status;

}

} // ...Namespace BEFE
