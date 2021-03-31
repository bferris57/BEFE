//----------------------------------------------------------------------
// File: UTF8.cpp - Implementaion of the UTF8 class (except Folding)
//----------------------------------------------------------------------
//
// The UTF8 class is responsible for performing operations on, and
// giving information about UTF-8 character strings.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"       // For Int, etc.
#include "BEFEMacros.h" // For Min, etc.
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Function: GetCharSize - Calculate the size (in bytes) given a start byte
//           GetByteSize  - Calculate the size (in bytes) given a UTF-8 character
//
// These functions calculate the UTF-8 size (in bytes) given either
// a start byte, or a UTF-8 character.
//
// Both will return zero if either an invalid UTF-8 character or byte
// sequence is indicated.
//
// In the case of the first function, this is when the given byte is not
// a valid UTF-8 start byte, e.g. an "invalid sequence".
//
// In the case of the second function, this is when an invalid UTF-8
// character is given, e.g. an "invalid code point"
//

UInt UTF8::GetCharSize(Byte byte) {
  if (byte < 0x80)
    return 1;
  if      ( byte == 0xc0 || byte == 0xc1 || (byte >= 0xf5 && byte <= 0xff))
    return 0;
  else if ( (byte & 0xf8) == 0xf0)                          return 4;
  else if ( (byte & 0xf0) == 0xe0)                          return 3;
  else if ( (byte & 0xe0) == 0xc0 || (byte & 0xe0) == 0xa0) return 2;
  return 0;
}

UInt UTF8::GetByteSize(Char thechar) {

  if (thechar >= (UInt)0 && thechar <= (UInt)0x10ffff) {
    if      (thechar <= (UInt)0x00007f)
      return 1;
    else if (thechar <= (UInt)0x0007ff)
      return 2;
    else if (thechar <= (UInt)0x00ffff)
      return 3;
    return 4;
  }
  return 0;
}

//----------------------------------------------------------------------
//
// Function: GetChar - Get character relative to current position
//
// Usage:    thechar = GetChar(buf,len,index)
//           thechar = GetChar(buf,len,index,curpos,newpos)
//
// Where:    buf     - Byte buffer
//           bufl    - Number of bytes in buffer
//           index   - Relative character index from curpos
//           curpos  - Current byte position in buffer
//           newpos  - New byte position in buffer (see below)
//
//           thechar - The UTF-8 character (UTF8::BadChar if none)
//
// This function gets a Unicode character relative to a position in
// a UTF-8 byte buffer.
//
// In the first form, a zero or positive index means "Nth character
// after the beginning of the buffer, where the first character is
// index zero.  In the second form, a zero or positive index means
// "Nth character AFTER current byte position", where the current
// byte position is character zero.
//
// In the first form, a negative index means "Nth character before
// the end of the buffer.  In the second form, a negative index means
// "Nth character BEFORE current byte position".
//
// On byte positions...
//
//   The odd case arises when the current byte position points to
//   an invalid character in a UTF-8 sequence.  If non-negative
//   indexing is used, these bytes are skipped "forwards".  If
//   negative indexing is used, the trailing bytes are skipped
//   until a valid ASCII character or a valid UTF-8 start byte
//   is encountered.
//
//   In the case of byte positioning, the newpos parameter will always
//   be updated to indicate the byte position of the beginning of
//   the next character if non-negative indexing is used, and to the
//   end of the previous character if negative indexing is used.
//
// If an invalid UTF-8 sequence is detected in the buffer, it is
// skipped in the indicated direction when searching.
//
// In the case of byte positioning, the newpos parameter will be
// updated to reflect this skipping.
//

Char UTF8::GetChar(Byte *buf, UInt bufl, Int index) {

  Int  curpos;
  Char thechar;

  if (index < 0)
    curpos = bufl;
  else
    curpos = 0;
  thechar = GetChar(buf,bufl,index,curpos,curpos);

  return thechar;

}

Char UTF8::GetChar(Byte *buf, UInt bufl, Int index, Int curpos, Int &newpos) {

  Int  status;
  Int  foundpos;
  Char thechar;
  Int  charlen;

  // Try finding it
  status = GetCharPos(buf,bufl,index,curpos,foundpos,thechar);
  if (status) {
    thechar.value = BadChar;
    goto DONE;
  }

  // Get its length
  charlen = GetByteSize(thechar);
  newpos = foundpos+charlen;

  while (false) {
    DONE: break;
  }

  return thechar;
}

//----------------------------------------------------------------------
//
// Function: GetCharPos - Find relative character position
//
// Usage: status = GetCharPos(buf, bufLen, index, curPos, foundPos, theChar)
//
// Where: buf
//        bufLen
//        relIndex
//        curPos
//        foundPos
//        theChar
//

Status UTF8::GetCharPos(Byte *buf, UInt bufLen, Int relIndex, Int curPos, Int &foundPos, Char &theChar) {

  Status status;
  UInt   charLen;
  Int    newPos;

  // Initialise
  theChar   = BadChar;
  foundPos  = curPos;
  newPos    = curPos;

  // Make sure we've got some length
  if ( IsNull(bufLen) ) goto NOTFOUND;
  
  // For safety's sake, fudge current position if negative index and it's beyond end...
  //
  // NOTE: If we're starting at end with negative indexing, it's proper to start
  //       one byte beyond end of buf because we decrement before accessing the buffer.
  if (relIndex < 0 && (UInt)curPos > bufLen) {
    curPos   = bufLen;
    newPos   = bufLen;
    foundPos = curPos;
  }

  // Do nothing if starting outside the buffer
  if (IsNull(buf) || bufLen <= 0 || curPos < 0 || (UInt)curPos > bufLen)
    goto NOTFOUND;

  // Do nothing if request is way out of bounds...
  if ( (relIndex < 0 && -relIndex > curPos) || (relIndex > 0 && curPos + relIndex >= (Int)bufLen) )
    goto NOTFOUND;
    
  //
  // If walking forward or backward...
  //

  // If forward...
  if (relIndex > 0) {
    while (relIndex && (UInt)newPos < bufLen) {
      charLen = GetCharSize(*(buf+foundPos));
      foundPos += charLen;
      relIndex--;
    }
    if ((UInt)foundPos >= bufLen) goto NOTFOUND;
  }

  // If backward
  else if (relIndex < 0) {
    while (relIndex && newPos >= 0) {
      foundPos--;
      charLen = GetCharSize(*(buf+foundPos));
      if (charLen != 0)
        relIndex++;
    }
    if (foundPos < 0) goto NOTFOUND;
  }

  // We're here now...
  charLen = GetCharSize(*(buf+foundPos));
  newPos = foundPos;

  // We're positined at the right place, so get the Char...
  if (charLen == 0 || newPos+charLen > bufLen)
    goto NOTFOUND;
  status = DecodeChar(buf+foundPos, theChar, charLen);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTFOUND:  foundPos = NaN; status = Error::UTF8NotFound;    break;
    SOMEERROR:                                                  break;
  }
  if (status)
    theChar = BadChar;
    
  return status;

}

//----------------------------------------------------------------------
//
// Function: EncodeChar - Encode a Unicode Char into a UTF-8 byte sequence
//
// Usage:    status = EncodeChar(theChar,buf,bytecount)
//
// Where:    thechar   - IN:  The Unicode character
//           buf       - OUT: Byte buffer
//           bytecount - OUT: Number of valid bytes in buffer
//
//           status    - Error status
//
// This function creates a valid, or empty, UTF-8 encoding of a given
// character.  If the character is an invalid Unicode character,
// the bytecount will be returned as zero and and the error
// Error::BadUnicodeCodePoint will be returned.
//
// NOTE: !!! IMPORTANT !!!
//
//       It is up to the caller to ensure the byte buffer passed is
//       at least UTF8::MaxLength bytes long.
//

Status UTF8::EncodeChar(Char thechar, Byte *buf, UInt &charlen) {

  Status status;

  charlen = 0;

  // Make sure it's not an invalid code point
  if ( (thechar.value >= 0xD800 && thechar.value <= 0xDFFF) ||
       (thechar.value >= 0xFFFE && thechar.value <= 0xFFFF) ||
        thechar.value >= 0x110000
     )
    goto INVALIDCODEPOINT;

  // 0x00..0x7f
  if (thechar.value <= 0x7f) {
    *buf++ = thechar;
    ++charlen;
    goto OK;
  }

  // 0x0080..0x07ff
  else if (thechar.value <= 0x07ff) {
    *buf++ = ((thechar.value>>6 ) & 0x1f) | 0xc0;
    ++charlen;
    *buf++ = ((thechar.value    ) & 0x3f) | 0x80;
    ++charlen;
    goto OK;
  }

  // 0x0800..0xffff
  else if (thechar.value <= 0xffff) {
    *buf++ = ((thechar.value>>12) & 0x0f) | 0xe0;
    ++charlen;
    *buf++ = ((thechar.value>>6 ) & 0x3f) | 0x80;
    ++charlen;
    *buf++ = ((thechar.value    ) & 0x3f) | 0x80;
    ++charlen;
    goto OK;
  }

  // 0x10000..0x10ffff
  *buf++ = ((thechar.value>>18) & 0x07) | 0xf0;
  ++charlen;
  *buf++ = ((thechar.value>>12) & 0x3f) | 0x80;
  ++charlen;
  *buf++ = ((thechar.value>>6 ) & 0x3f) | 0x80;
  ++charlen;
  *buf++ = ((thechar.value    ) & 0x3f) | 0x80;
  ++charlen;

  // Handle errors
  status = Error::None;
  while (false) {
    INVALIDCODEPOINT: status = Error::BadUnicodeCodePoint; break;
    OK:               status = Error::None;                break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Function: DecodeChar - Decode single UTF-8 byte sequence into a Unicode character
//
// Usage:    status = DecodeChar(buf,theChar,bytecount)
//
// Where:    buf       - IN:  Byte buffer
//           thechar   - OUT:  The Unicode character
//           bytecount - OUT: Number of buffer bytes in character
//
//           status    - Error status
//

Status UTF8::DecodeChar(Byte *buf, Char &theChar, UInt &charLen) {

  Status status;
  Byte   theByte;

  // Initialise
  theChar   = BadChar;

  // Get the UTF-8 character length...
  theByte = *buf;
  charLen = GetCharSize(*buf);

  // Get a 1 byte UTF-8 character...
  if      (charLen == 1) {
    theChar.value = *buf & 0x7f;
  }

  // Get a 2 byte UTF-8 character...
  else if (charLen == 2) {
    theChar.value = *(buf++) & 0x3f;
    // Process next byte
    theByte = *(buf++);
    if ( (theByte & 0xc0) != 0x80 ) goto BADSEQ;
    theByte &= 0x3f;
    theChar.value = (theChar.value << 6) | theByte;
  }

  // Get a 3 byte UTF-8 character...
  else if (charLen == 3) {
    theChar.value = *(buf++) & 0x0f;
    // Process next byte
    theByte = *(buf++);
    if ( (theByte & 0xc0) != 0x80) goto BADSEQ;
    theByte &= 0x3f;
    theChar.value = (theChar.value << 6) | theByte;
    // Process next byte
    theByte = *(buf++);
    if ( (theByte & 0xc0) != 0x80) goto BADSEQ;
    theByte &= 0x3f;
    theChar.value = (theChar.value << 6) | theByte;
  }

  // Get a 4 byte UTF-8 character...
  else {
    theChar.value = *(buf++) & 0x07;
    // Process next byte
    theByte = *(buf++);
    if ( (theByte & 0xc0) != 0x80) goto BADSEQ;
    theByte &= 0x3f;
    theChar.value = (theChar.value << 6) | theByte;
    // Process next byte
    theByte = *(buf++);
    if ( (theByte & 0xc0) != 0x80) goto BADSEQ;
    theByte &= 0x3f;
    theChar.value = (theChar << 6) | theByte;
    // Process next byte
    theByte = *(buf++);
    if ( (theByte & 0xc0) != 0x80) goto BADSEQ;
    theByte &= 0x3f;
    theChar.value = (theChar.value << 6) | theByte;
  }

  // Weed out invalid characters
  theChar._Validate();
  if (theChar == BadChar) goto BADCHAR;

  // Handle errors
  status = Error::None;
  while (false) {
    BADSEQ:                   theChar = BadChar; status = Error::UTF8InvalidSeq;  break;
    BADCHAR:                  theChar = BadChar; status = Error::UTF8InvalidChar; break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Function: Count - Get byte and character counts in UTF-8 byte string
//
// Usage:    status = Count(buf,len,bytecount,charcount)
//
// Where:    buf       - IN:  Byte buffer
//           bufl      - IN:  Number of bytes in buffer
//           bytecount - OUT: Number of valid bytes in buffer
//           charcount - OUT: Number of valid UTF-8 characters in buffer
//
//           status    - Error code (Error::None = "Ok")
//
// This function passes over the entire string and calculates important
// UTF-8 information about the string contents.
//

Status UTF8::Count(Byte *buf, UInt size, UInt &byteCount, UInt &charCount) {

  Int    curPos;
  Int    newPos;
  Char   theChar;
  UInt   charLen;

  if (!IsNull(buf)) {
    byteCount = 0;
    charCount = 0;
    curPos    = 0;
    while ((UInt)curPos < size) {
      theChar = GetChar(buf+curPos,size-curPos,0,0,newPos);
      if (!theChar._IsValid()) {
        charLen = GetByteSize(theChar);
        curPos += Max(charLen,1);
        byteCount += charLen;
      }
      else {
        charCount++;
        curPos    += newPos;
        byteCount += newPos;
      }
    }
  }

  return Error::None;

}

//----------------------------------------------------------------------
//
// Miscellaneous
//

Status UTF8::ReverseChars(Byte *buf, UInt size) {
  
  Status  status;
  Byte   *bp;
  UInt    remLen;
  UInt    charLen;
  
  if (IsNull(buf) || IsNull(size)) goto OK;
  
  // Reverse the bytes first...
  MemoryReverseBytes(buf, size);
  
  // Now, reverse each valid UTF-8 character...
  //
  // Note: We have to do this from the end of the buffer "backwards"
  //       because each leading UTF-8 byte is now at the end of the
  //       UTF-8 character's sequence
  //
  // Note: Also, I'm not sure what do about "modifier characters"
  //       at this point.  I guess it's a rather philosophical
  //       question because I'm not even sure why someone would be
  //       reversing UTF-8 strings since this is mainly used only
  //       when reversing bytes in number to character conversions
  //       and MemoryReverseBytes would be called instead anyway.
  //       So, we'll just POINT IT OUT HERE and just burn that bridge
  //       when we come to it.  ;-)
  
  remLen = size;
  bp = buf+size-1;
  while (remLen) {
    if (*bp >= 0x80) {
      charLen = GetCharSize(*bp);
      if (charLen > 1 && remLen >= charLen) {
        MemoryReverseBytes(bp-charLen+1, charLen);
        bp     -= charLen;
        remLen -= charLen;
        continue;
      }
    }
    bp--;
    remLen--;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
  }
  
  return status;
  
}

} // ...Namespace BEFE
