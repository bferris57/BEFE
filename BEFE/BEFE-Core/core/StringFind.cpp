//----------------------------------------------------------------------
// File: StringFind.cpp - String Class Find methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "UTF8.h"
#include "SBuffer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Find(Char)...
//
// Note: We can't combine Find(Char, Range) with Find(Char, Span) 
//       because they're subtly different... You can't turn an
//       unresolved Range into an unresolved Span (eg -1..0 Range
//       would be considered an "unresolved Span") and Find(Char,Span)
//       needs to be passed an unresolved Span.
//

Status String::Find(Char c, Range const &range, Int &foundPos) const {

  Status status;
  Range  rRange;
  Span   rSpan;

  rRange = range.Resolve(Length());
  rSpan.idx1 = rRange.idx1;
  rSpan.idx2 = rRange.idx2;
  if (rSpan.IsAscending())
    rSpan.idx2++;
  else
    rSpan.idx2--;

  status = ((String *)this)->_FindCharLow(c, rSpan, foundPos);
  
  return status;
  
}

Status String::Find(Char c, Span const &span, Int &foundPos) const {

  Status status;
  Span   rSpan;
  
  rSpan = span.Resolve(Length());
  status = ((String *)this)->_FindCharLow(c, rSpan, foundPos);
  
  return status;
  
};

Int String::Find(Char c, Range const &range) const {
  Status status;
  Int    foundPos;
  status = Find(c, range, foundPos);
  if (status)
    BEFE::SetNull(foundPos);
  return foundPos;
}

Int String::Find(Char c, Span const &span) const {
  Status status;
  Int    foundPos;
  status = Find(c, span, foundPos);
  if (status)
    BEFE::SetNull(foundPos);
  return foundPos;
};

Status String::Find(Char c, Int &foundPos) const {
  Status status;
  Span   span;
  status = Find(c,span,foundPos);
  return status;
};

Int String::Find(Char c) const {
  Status status;
  Int    foundPos;
  Span   span;
  status = Find(c,span,foundPos);
  if (status)
    BEFE::SetNull(foundPos);
  return foundPos;
};

//----------------------------------------------------------------------
//
// Find(char)...
//
// Note: We had to add these because even though Char(char) is normally
//       assumed, there's an ambiguity with String.Find(String const &)
//       since String has a (char) constructor too.

Status String::Find(char c, Range const &range, Int &foundPos) const {
  return Find(Char(c), range, foundPos);
}

Status String::Find(char c, Span const &span, Int &foundPos) const {
  return Find(Char(c),span,foundPos);
}

Status String::Find(char c, Int &foundPos) const {
  return Find(Char(c),foundPos);
}

Int String::Find(char c, Range const &range) const {
  return Find(Char(c), range);
}

Int String::Find(char c, Span const &span) const {
  return Find(Char(c),span);
}

Int String::Find(char c) const {
  return Find(Char(c));
}

//----------------------------------------------------------------------
//
// Find(String)...
//

Status String::Find(String const &that, Range const &range, Int &foundPos) const {
  

  Status status;
  Range  rRange;
  Span   rSpan;

  rRange = range.Resolve(Length());
  rSpan.idx1 = rRange.idx1;
  rSpan.idx2 = rRange.idx2;
  if (rSpan.IsAscending())
    rSpan.idx2++;
  else
    rSpan.idx2--;

  status = ((String *)this)->_FindStringLow(that, rSpan, foundPos);
  
  return status;
    
}

Status String::Find(String const &that, Span const &span, Int &foundPos) const {

  Status status;
  Span   rSpan;

  rSpan = span.Resolve(Length());
  status = ((String *)this)->_FindStringLow(that, rSpan, foundPos);

  return status;
  
}

Status String::Find(String const &that, Int &foundPos) const {
  Status status;
  status = Find(that, Span(), foundPos);
  return status;
}

Int String::Find(String const &that, Range const &range) const {
  Status status;
  Int    foundPos;
  status = Find(that, range, foundPos);
  if (status)
    BEFE::SetNull(foundPos);
  return foundPos;
}

Int String::Find(String const &string, Span const &span) const {
  Status status;
  Int    foundPos;
  status = Find(string, span, foundPos);
  if (status)
    BEFE::SetNull(foundPos);
  return foundPos;
}

Int String::Find(String const &string) const {
  Status status;
  Int    foundPos;
  status = Find(string, foundPos);
  if (status)
    BEFE::SetNull(foundPos);
  return foundPos;
}

Int String::Find(char const *string) const {
	return Find(String(string));
}

Int String::FirstBlank() const {
  
  Byte *buf;
  UInt  len;
  UInt  idx;
  
  _BufAndSize(buf, len);
  idx = 0;
  if (buf && len)
    while (idx < len && *buf++ != ' ') idx++;
  if (idx >= len)
    BEFE::SetNull(idx);
    
  return idx;
  
}

Int String::LastBlank() const {

  Byte *buf;
  UInt  len;
  Int   idx;
  
  _BufAndSize(buf, len);
  idx = len;
  if (buf && len) {
    idx = len-1;
    buf += idx;
    while (idx >= 0 && *buf-- != ' ') idx--;
  }
  if (idx >= 0)
    BEFE::SetNull(idx);
  
  return idx;
  
}

Int String::FirstWhite() const {
  
  Byte *buf;
  UInt  len;
  UInt  idx;
  
  _BufAndSize(buf, len);
  idx = 0;
  if (buf && len)
    while (idx < len && *buf++ > 0x20) idx++;
  if (idx >= len)
    BEFE::SetNull(idx);
  
  return idx;
  
}

Int String::LastWhite() const {

  Byte *buf;
  UInt  len;
  Int   idx;
  
  _BufAndSize(buf, len);
  idx = (Int)len;
  if (buf && len) {
    idx = (Int)len-1;
    buf += idx;
    while (idx > 0x20) idx--;
  }
  if (idx < 0)
    BEFE::SetNull(idx);
  
  return idx;
  
}

Int String::FirstNonBlank() const {
  
  Byte *buf;
  UInt  len;
  UInt  idx;
  
  _BufAndSize(buf, len);
  idx = 0;
  if (buf && len)
    while (idx < len && *buf++ == ' ') idx++;
  if (idx >= len)
    BEFE::SetNull(idx);
    
  return idx;
  
}

Int String::LastNonBlank() const {

  Byte *buf;
  UInt  len;
  Int   idx;
  
  _BufAndSize(buf, len);
  idx = (Int)len;
  if (buf && len) {
    idx = len-1;
    buf += idx;
    while (idx >= 0 && *buf-- == ' ') idx--;
  }
  if (idx < 0)
    BEFE::SetNull(idx);
  
  return idx;
  
}

Int String::FirstNonWhite() const {
  
  Byte *buf;
  UInt  len;
  Int   idx;
  
  _BufAndSize(buf, len);
  idx = 0;
  if (buf && len)
    while (idx < (Int)len && *buf++ <= 0x20) idx++;
  if (idx >= (Int)len)
    BEFE::SetNull(idx);
  
  return idx;
  
}

Int String::LastNonWhite() const {

  Byte *buf;
  UInt  len;
  Int   idx;
  
  _BufAndSize(buf, len);
  idx = (Int)len;
  if (buf && len) {
    idx = (Int)len-1;
    buf += idx;
    while (idx >= 0 && *buf-- <= 0x20) idx--;
  }
  if (idx < 0)
    BEFE::SetNull(idx);
  
  return idx;
  
}

//----------------------------------------------------------------------
//
// FindChars - Find first occurance of a set of characters starting
//             at a specified position
//
// Notes: Returns position found, -1 == "Not found"
//
//        Negative startPos is wrapped from end
//
//        The current implementationi assumes char.Length() is less
//        than this->Length().  If not, should either search the other
//        way around or we should check it here.
//

Int String::FindChars(String const &chars, Int startPos) const {

	Int  foundPos;
	Int  searchIdx;
	Int  charsLen = chars.Length();
	Int  len      = Length();
	Char curChar;

	// Normalise startPos
	if (startPos < 0) {
		startPos = Length()+startPos;
		if (startPos < 0)
			return -1;
	}

	// Until pos off end...
	for (foundPos = startPos; foundPos < len; foundPos++) {
		curChar = Get(foundPos);
    for (searchIdx = 0; searchIdx < charsLen; searchIdx++)
    	if (chars[searchIdx] == curChar)
    		return foundPos;
	}

	return -1;

}

//----------------------------------------------------------------------
//
// Low Level Find(Char) and Find(String)
//
// Note: We've implemented these strange methods because Range and String
//       are so very close to each other in behaviour.  But, the problem
//       is that an Unresolved Range cannot be converted to an Unresolved
//       Span so we have to resolve it at a higher level and be passed
//       it as a Resolved Span to this method instead.
//
// Note: Also, we MODIFY the caller's Span instead of keeping our own.
//

Status String::_FindCharLow(Char c, Span &rSpan, Int &foundPos) {

  Status   status;
  Byte    *srcBuf;
  UInt     srcSize;
  Byte    *srcFound;
  Int      curIdx;
  UInt     byteCount;
  UInt     charCount;
  Char     theChar;
  String   charString;

  BEFE::SetNull(foundPos);

  // Special case for Insensitive target, we'll do a String
  // search instead...
  if (!isSensitive) {
    charString = String(UTF8::Fold(c));
    status = _FindStringLow(charString, rSpan, foundPos);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Get srcBuf, srcSize, and first go at rSpan...
  _BufAndSize(srcBuf, srcSize);
  
  // Nothing to do if no size
  if (BEFE::IsNull(srcBuf) || srcSize == 0) goto OK;
  
  // Special case for forward and ASCII...
  if (!isChar && c.value <= 0x7f && rSpan.IsAscending()) {
    srcFound = Memchr(srcBuf+rSpan.idx1, c.value, srcSize-rSpan.idx1);
    if (!BEFE::IsNull(srcFound))
      foundPos = srcFound - srcBuf;
    else
      BEFE::SetNull(foundPos);
    goto OK;
  }
  
  // Now, set up the search Span...
  status = UTF8::Count(srcBuf,srcSize,byteCount,charCount);
  if (status) goto SOMEERROR;
  
  curIdx = rSpan.idx1;
  if (curIdx < 0 || (UInt)curIdx >= charCount) goto OK;

  if (rSpan.idx2 < rSpan.idx1) { // Walk backward...
    while (curIdx > rSpan.idx2) {
      theChar = UTF8::GetChar(srcBuf,srcSize,curIdx);
      if (theChar == c) {
        foundPos = curIdx;
        goto OK;
      }
      curIdx--;
    };
  }
  else {                // Walk forward....
    while (curIdx < rSpan.idx2) {
      theChar = UTF8::GetChar(srcBuf,srcSize,curIdx);
      if (theChar == c) {
        foundPos = curIdx;
        goto OK;
      }
      curIdx++;
    };
  }

  // Handle errors
  while (false) {
    SOMEERROR:                               break;
    OK:            status = Error::None;     break;
  }

  return status;

}

Status String::_FindStringLow(String const &that, Span &rSpan, Int &foundPos) {

  Status status;
  Byte  *thisBuf;
  UInt   thisByteCount;
  UInt   thisCharCount;
  Byte  *thatBuf;
  UInt   thatByteCount;
  UInt   thatCharCount;
  UInt   rSpanLen;
  String thisConst;
  String thatConst;
  UInt   curIdx;
  UInt   utfCharLen;
  Int    tPos;
  Char   theChar;
  
  BEFE::SetNull(foundPos);
  
  // Resolve this and that buffers and Byte/Char counts
  ((String *) this)->_BufByteAndCharCount(thisBuf, thisByteCount, thisCharCount);
  ((String *)&that)->_BufByteAndCharCount(thatBuf, thatByteCount, thatCharCount);
  if (BEFE::IsNull(thisBuf) || thisByteCount == 0 ||
      BEFE::IsNull(thatBuf) || thatByteCount == 0  )
    goto NOTFOUND;
  if( thisByteCount > 0xffff || thatByteCount > 0xffff) goto TOOBIG;
  
  // Verify the search Range
  rSpanLen = rSpan.Length();
  if (rSpanLen == 0 || rSpanLen < thatCharCount) goto NOTFOUND;
  
  // Set up the thatConst string
  thatConst = String((char const *)thatBuf, thatByteCount);
  if (!that.isSensitive)
    thatConst.isSensitive = false;
    
  //
  // Forward...
  //
  
  if (rSpan.IsAscending()) {
    
    // Adjust sSpan based on thatCharLen
    rSpan.idx2 -= thatCharCount-1;
    if (rSpan.idx2 <= rSpan.idx1) goto NOTFOUND;
    
    // Set up the thisConst string
    if (isByte)
      thisConst = String((char const *)thisBuf+rSpan.idx1, thatByteCount);
    else {
      tPos = 0;
      status = UTF8::GetCharPos(thisBuf, thisByteCount, rSpan.idx1, 0, tPos, theChar);
      if (status) goto SOMEERROR;
      thisConst = String((char const *)thisBuf+tPos, thatByteCount);
    }
    if (!isSensitive)
      thisConst.isSensitive = false;
      
    // For each position...
    for (curIdx=rSpan.idx1; curIdx < (UInt)rSpan.idx2; curIdx++) { // For each Char...
 
      if (thisConst == thatConst) { // If we found it...
        foundPos = curIdx;
        goto FOUND;
      }

      // Skip to next Char
      if (isByte) // It's ASCII...
        thisConst.data++;
      else {           // It's UTF-8...
        utfCharLen = UTF8::GetCharSize(*thisConst.data);
        thisConst.data += utfCharLen;
        while (utfCharLen == 0) { // Skip bad UTF-8 bytes...
          thisConst.data++;
          if (thisConst.data >= thisBuf+thisByteCount) goto NOTFOUND;
          utfCharLen = UTF8::GetCharSize(*thisConst.data);
        } // ...Skip bad UTF-8 bytes
      } // ...It's UTF-8

    } // ...For each Char
    
  }
  
  //
  // Reverse...
  //
  
  else {

    // Adjust sSpan based on thatCharLen
    rSpan.idx1 -= thatCharCount-1;
    if (rSpan.idx1 <= rSpan.idx2) goto NOTFOUND;
    
    // Set up the thisConst string
    if (isByte)
      thisConst = String((char const *)thisBuf+rSpan.idx1, thatByteCount);
    else {
      tPos = 0;
      status = UTF8::GetCharPos(thisBuf, thisByteCount, rSpan.idx1, 0, tPos, theChar);
      if (status) goto SOMEERROR;
      thisConst = String((char const *)thisBuf+tPos, thatByteCount);
    }
    if (!isSensitive)
      thisConst.isSensitive = false;
      
    // For each position...
    for (curIdx=rSpan.idx1; curIdx > (UInt)rSpan.idx2; curIdx--) {
      
      // Skip bad UTF-8 characters...
      while (isChar && UTF8::GetCharSize(*thisConst.data) == 0) {// Skip bad UTF-8 Characters
        if (thisConst.data <= thisBuf) goto NOTFOUND;
        thisConst.data--;
      }
      
      // If we found it...
      if (thisConst == thatConst) {
        foundPos = curIdx;
        goto FOUND;
      }
      
      // Skip to previous Char...
      thisConst.data--;

    }
    
  }
  
  //
  // Handle errors
  //
  
  status = Error::None;
  while (false) {
    NOTFOUND:  status = Error::None; break;
    FOUND:     status = Error::None; break;
    SOMEERROR:                       break;
    TOOBIG:    BEFE_WARN("String._FindStringLow can only search strings less than 64KB");
               status = Error::Failed;
               break;
  }

  return status;
  
}

} // ...Namespace BEFE
