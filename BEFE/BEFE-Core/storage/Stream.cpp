//----------------------------------------------------------------------
// File: Stream.cpp - Base Stream implementation
//----------------------------------------------------------------------
//
// This provides the base Stream implementation.  It is up to sub-classes
// to provide the "real" working implementation for each type of
// Stream.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Stream.h"
#include "UTF8.h"
#include "ToAscii.h"

#define RETURN_ZERO {return 0;}
#define RETURN_ONE  {return 1;}
#define RETURN_UNIT {return StorageUnit::Byte;}
#define RETURN_UNaN {return UNaN;}
#define RETURN_NOTCAPABLE {return Error::NotCapable;}
#define RETURN_OK         {return Error::None;}
#define RETURN_NULL       {return NULL;}
#define RETURN_NULLRange  {return Range(0,-1);}

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Livecycle
//

Stream::Stream() {
}

Stream::~Stream() {
}

//
// C++ Operator overloads
//

// Operator =
Stream &Stream::operator= (Stream const &that) {
  //WARN("Stream.operator=(Stream const &) not implemented yet");
  return (*this);
}

Stream &Stream::operator= (String const &that) {
  //WARN("Stream.operator=(String const &) not implemented yet");
  return (*this);
}

Stream &Stream::operator= (char const *that) {
  //WARN("Stream.operator=(char const *) not implemented yet");
  return (*this);
}

Stream &Stream::operator= (char *that) {
  //WARN("Stream.operator=(char *) not implemented yet");
  return (*this);
}

Stream &Stream::operator= (char that) {
  //WARN("Stream.operator=(char) not implemented yet");
  return (*this);
}

Stream &Stream::operator= (Short that) {
  //WARN("Stream.operator=(Short) not implemented yet");
  return (*this);
}

Stream &Stream::operator= (UShort that) {
  //WARN("Stream.operator=(UShort) not implemented yet");
  return (*this);
}

Stream &Stream::operator= (Int  that) {
  //WARN("Stream.operator=(Int) not implemented yet");
  return (*this);
}

Stream &Stream::operator= (UInt that) {
  //WARN("Stream.operator=(UInt) not implemented yet");
  return (*this);
}

Stream &Stream::operator= (void *that) {
  //WARN("Stream.operator=(void *) not implemented yet");
  return (*this);
}

// Operator <<

Stream &Stream::operator<< (Char that) {
  
  Status status;
  Byte   utfBuf[4];
  UInt   charLen;
  UInt   numOut;
  
  if (that.value <= 0x7f) {
    utfBuf[0] = (Byte)that.value;
    charLen = 1;
  }
  else {
    status = UTF8::EncodeChar(that, utfBuf, charLen);
    if (status) charLen = 0;
  }
  status = _PutBytes(utfBuf,charLen,numOut);

  return (*this);
}

Stream &Stream::operator<< (String const &that) {

  Byte *buf;
  Int   mult;
  UInt  size;
  
  that._BufAndSize(buf,size);
  mult = that.mult;

  for (mult=that.mult; mult; mult--)
    _PutBytes(buf,size,size);

  return (*this);
}

Stream &Stream::operator<< (Integer const &that) {
  return (*this) << that.ToString().Consumable();
}

Stream &Stream::operator<< (char const *that) {

  UInt numwritten;
  UInt size;

  if (that) {
    size = Strlen(that);
    _PutBytes((Byte *)that,size,numwritten);
  }

  return (*this);

}

Stream &Stream::operator<< (char *that) {
  UInt numwritten;
  UInt size;
  if (that) {
    size = Strlen(that);
    _PutBytes((Byte *)that,size,numwritten);
  }
  return (*this);
}

Stream &Stream::operator<< (char that) {
  UInt numwritten;
  if (that)
    _PutBytes((Byte *)&that,1,numwritten);
  return (*this);
}

Stream &Stream::operator<< (Short  that) {

  Byte buf[11];
  if ( IsNull(that) )
    Strcpy(buf,CONSTR_NaN);
  else
    Itoa((Int)that,buf,10);
  return (*this) << (char *)buf;

}

Stream &Stream::operator<< (UShort that) {

  Byte buf[ToAscii_MaxNumLen];

  if ( IsNull(that) )
    Strcpy(buf, CONSTR_NaN);
  else
    ToAscii(that,buf);

  return (*this) << (char *)buf;

}

Stream &Stream::operator<< (Int  that) {

  Byte buf[11];
  if ( IsNaN(that) )
    Strcpy(buf, CONSTR_NaN);
  else
    Itoa(that,buf,10);
  return (*this) << (char *)buf;

}

Stream &Stream::operator<< (UInt that) {

  Byte buf[ToAscii_MaxNumLen];

  if ( IsNull(that) )
    Strcpy(buf, CONSTR_NaN);
  else
    ToAscii(that,buf);

  return (*this) << (char *)buf;

}

Stream &Stream::operator<< (Long that) {
  Byte buf[ToAscii_MaxNumLen];

  if (that == LongNaN)
    Strcpy(buf,"LongNaN");
  else
    ToAscii(that,buf);

  return (*this) << (char *)buf;
}

Stream &Stream::operator<< (void *that) {
  *this << ToHexString((UInt)that);
  return (*this);
}

//----------------------------------------------------------------------
//
// Capabilities
//

Int Stream::GetCapability() const {
  return FixedElementSize | FixedTotalSize | KnownSize | IsFixedLength | IsBigEndian;
}

UInt   Stream::GetElementSize() const           RETURN_UNIT
Status Stream::SetElementSize(UInt elmsiz)      RETURN_NOTCAPABLE
UInt   Stream::Length() const                   RETURN_UNaN
UInt   Stream::Size() const                     RETURN_UNaN

// Positional Range
UInt   Stream::GetPos() const                   RETURN_ZERO
Status Stream::SetPos(UInt pos)                 RETURN_NOTCAPABLE
Status Stream::SetRelativePos(Int pos)          RETURN_NOTCAPABLE
Range  Stream::PosRange() const                 RETURN_NULLRange
Status Stream::NextPos()                        RETURN_NOTCAPABLE
Status Stream::PreviousPos()                    RETURN_NOTCAPABLE
Status Stream::RewindPos()                      RETURN_NOTCAPABLE
Status Stream::EndPos()                         RETURN_NOTCAPABLE
Status Stream::SetSize(UInt size)               RETURN_NOTCAPABLE

// Flushing if Putting...
Status Stream::Flush()                  RETURN_OK

// Base Buffering
Byte  *Stream::GetBuffer() const        RETURN_NULL
UInt   Stream::GetBufferSize() const    RETURN_NOTCAPABLE
Status Stream::SetBufferSize(UInt size) RETURN_NOTCAPABLE

// Base Gets...
Status Stream::_GetElement(Byte *b)    RETURN_NOTCAPABLE

Status Stream::_GetChar(Char &c) {

  Status status;
  Byte   buf[4];
  UInt   numRead;
  UInt   charLen;
  
  // Get the first byte
  status = _GetByte(buf[0]);
  if (status) goto SOMEERROR;
  
  // If it's UTF-8...
  charLen = UTF8::GetCharSize(buf[0]);
  if (charLen > 1) {
    status = _GetBytes(&buf[1], charLen-1, numRead);
    if (status) goto SOMEERROR;
    if (numRead != charLen-1) goto TRUNCATED;
    c = UTF8::GetChar(buf,charLen,0);
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
    TRUNCATED: BEFE_WARN("Stream.GetChar: UTF-8 Character truncated");
               status = Error::UTF8InvalidChar;
               break;
  }
  if (status)
    c = UTF8::BadChar;
  
  return status;
  
}

Status Stream::_GetBytes(Byte *buf, UInt bufsiz, UInt &numread) RETURN_NOTCAPABLE

Status Stream::_GetByte(Byte &b) {
  
  Status status;
  UInt   numRead;

  b = UInt8NaN;

  status = _GetBytes(&b,1,numRead);
  if (status) goto SOMEERROR;
    
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

// Base Puts...
Status Stream::_PutElement(Byte *b) RETURN_NOTCAPABLE
Status Stream::_PutBytes  (Byte *buf, UInt bufsize, UInt &numwrite) RETURN_NOTCAPABLE
Status Stream::_PutByte   (Byte  b) {UInt numWritten; return _PutBytes(&b, 1, numWritten);}

// Line I/O
Status Stream::GetLine(String &line) {
  return Error::NotCapable;  
}

Status Stream::PutLine(String &line) {
  return Error::NotCapable;  
}

// Multiple Line I/O
Strings Stream::GetLines() {
  Strings strings;
  GetLines(strings);
  return strings.Consumable();
}

Status Stream::GetLines(Strings &lines) {

  Status status;
  String string;
  
  status = lines.Reset();
  if (status) goto SOMEERROR;
  
  status = SetPos(0);
  if (status) goto SOMEERROR;
  
  do {
    status = GetLine(string);
    if (status == Error::StreamAtEnd) break;
    if (status) goto SOMEERROR;
    status = lines.Append(string.Consumable());
    if (status) goto SOMEERROR;
  } while (true);
 
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

} // ...Namespace BEFE
