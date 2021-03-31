//----------------------------------------------------------------------
// File: StringGet.cpp - String Class Get methods
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

Char String::Get() const {

  //Status status;
  Char   c;

  /*status =*/ Get(0,c);

  return c;

}

Status String::Get(Int index, Char &c) const {

  Status   status;
  SBuffer *sbuf;
  char    *srcBuf;
  Int      srcLen;

  c = UNaN;
  
  if (BEFE::IsNull(index)) goto OUTOFRANGE;
  
  switch (type) {
    case Null:  goto OUTOFRANGE;
    case Const:
      srcBuf = (char *)data;
      if (BEFE::IsNull(srcBuf)) goto OUTOFRANGE;
      c = UTF8::GetChar((Byte *)srcBuf, Strlen(srcBuf), index);
      break;
    case ConstLen:
      srcBuf = (char *)data;
      if (BEFE::IsNull(srcBuf)) goto OUTOFRANGE;
      c = UTF8::GetChar((Byte *)srcBuf, constLen, index);
      break;    
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      srcBuf = (char *)&data;
      srcLen = type-ASCII0;
      c = UTF8::GetChar((Byte *)srcBuf, srcLen, index);
      break;
    case UtfInline:
      srcBuf = (char *)&data;
      srcLen = constLen;
      c = UTF8::GetChar((Byte *)srcBuf, srcLen, index);
      break;
    case Buffer:
      sbuf = (SBuffer *)&data;
      if (BEFE::IsNull(sbuf)) goto OUTOFRANGE;
      if (isByte) {
        srcBuf = (char *)sbuf->GetBuf();
        if (BEFE::IsNull(srcBuf)) goto OUTOFRANGE;
        srcLen = sbuf->GetUsedBytes();
        if (index < 0) index = index+srcLen;
        if (index >= srcLen || index < 0)
          goto OUTOFRANGE;
        c = (Char)srcBuf[index];
        break;
      }
      else {
        c = sbuf->GetChar(index);
        if ( BEFE::IsNull(c) ) goto OUTOFRANGE;
      }
      break;
    default:
      goto INTERNALERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    INTERNALERROR: status = Error::Internal;        break;
    OUTOFRANGE:    status = Error::IndexOutOfRange; break;
  }

  return status;

}

Char String::Get(Int index) const {
  Status status;
  Char   c;
  status = Get(index,c);
  if (status != 0)
    c = (UInt)0;
  return c;
}

String String::Get(Span const &span) const {

  Status status;
  String theString;

  status = Get(span, theString);
  if (status)
    theString = ErrorString(status);

  return theString.Consumable();
}

Status String::Get(Span const &span, String &that) const {

  Status   status;
  Byte    *srcBuf;
  UInt     srcSize;
  Int      curIdx;
  UInt     byteCount;
  UInt     charCount;
  Char     theChar;
  Span     rSpan;
  String   theSpan;
  Int      startPos;
  Int      endPos;
  Char     c;
  
  // Get the Source Buffer and Size (in bytes)
  _BufAndSize(srcBuf, srcSize);
  if (BEFE::IsNull(srcBuf) || srcSize == 0) goto OK;
  
  // We now have srcbuf and srclen, resolve the Byte and Char lengths...
  if (isByte && !isChar)
    byteCount = charCount = srcSize;
  else {
    status = UTF8::Count(srcBuf,srcSize,byteCount,charCount);
    if (status) goto SOMEERROR;
  }
 
  // Normalise the Span...
  rSpan = span.Resolve(charCount);
  if (rSpan.Length() == 0) goto OK;
  
  //
  // Get the Characters...
  //
  
  if (rSpan.idx2 >= rSpan.idx1) { // Forward (natural order)...
    
    if (byteCount == charCount) { // Pure Bytes...

      if (type == Const || type == ConstLen)
        theSpan = String((char const *)srcBuf+rSpan.idx1, rSpan.idx2-rSpan.idx1);
      else
        theSpan = String(srcBuf+rSpan.idx1, rSpan.idx2-rSpan.idx1);      

    } // ...Pure Bytes

    else {  // It has UTF-8 in it...

      BEFE::SetNull(startPos);
      BEFE::SetNull(endPos);
      status = UTF8::GetCharPos(srcBuf, byteCount, rSpan.idx1, 0, startPos, c);
      status = UTF8::GetCharPos(srcBuf, byteCount, rSpan.idx2, 0, endPos, c);
      if (status == Error::UTF8NotFound)
        endPos = byteCount;
      if (!BEFE::IsNull(startPos) && !BEFE::IsNull(endPos) && endPos >= startPos) {
        theSpan = String(srcBuf+startPos, endPos-startPos);
        status = theSpan._MorphTo(Buffer);
        if (status) goto SOMEERROR;
      }
    } // ...It has UTF-8 in it

  } // ...Forward (natural order)
  
  else { // Backward (reverse order)...
    
    if (byteCount == charCount) { // Pure Bytes...

      if (type == Const || type == ConstLen)
        theSpan = String((char const *)srcBuf+rSpan.idx2+1, rSpan.idx1-rSpan.idx2);
      else
        theSpan = String(srcBuf+rSpan.idx2+1, rSpan.idx1-rSpan.idx2);

      // Reverse the bytes
      if (!theSpan.isMutable) {
        status = theSpan.Mutable();
        if (status) goto SOMEERROR;
      }
      theSpan._BufAndSize(srcBuf,srcSize);
      MemoryReverseBytes(srcBuf,srcSize);      

    } // ...Pure Bytes

    else {  // It has UTF-8 in it...

      curIdx = rSpan.idx1;
      while (curIdx > rSpan.idx2) {
        theChar = UTF8::GetChar(srcBuf,srcSize,curIdx);
        status = theSpan.Append(theChar);
        if (status) goto SOMEERROR;
        curIdx--;
      };

    } // ...It has UTF-8 in it

  } // ...Backward
  
  //
  // Handle errors
  //
  
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  that = theSpan.Consumable();

  return status;
}

String String::Get(Range const &range) const {

  Status status;
  String theString;

  status = Get(range,theString);
  if (status)
    theString = ErrorString(status);

  return theString.Consumable();
}

Status String::Get(Range const &range, String &that) const {

  Status   status;
  Byte    *srcBuf;
  UInt     srcSize;
  Int      curIdx;
  UInt     byteCount;
  UInt     charCount;
  Char     theChar;
  Range    rRange;
  String   theRange;
  
  // Get Source Buffer and Size (in bytes)
  _BufAndSize(srcBuf, srcSize);
  if (BEFE::IsNull(srcBuf) || srcSize == 0) goto OK;

  // We now have srcbuf and srclen, resolve the Byte and Char lengths...
  if (isByte && !isChar)
    byteCount = charCount = srcSize;
  else {
    status = UTF8::Count(srcBuf,srcSize,byteCount,charCount);
    if (status) goto SOMEERROR;
  }
   
  // Normalise the range...
  rRange = range.Resolve(charCount);
  if ( (rRange.idx1 < 0 || (UInt)rRange.idx1 >= charCount) ||
       (rRange.idx2 < 0 || (UInt)rRange.idx2 >= charCount))
    goto EMPTY;
    
  //
  // Get the Characters...
  //
  
  if (rRange.idx2 >= rRange.idx1) { // Forward (natural order)...
    
    if (byteCount == charCount) { // Pure Bytes...

      if (type == Const || type == ConstLen)
        theRange = String((char const *)srcBuf+rRange.idx1, rRange.idx2-rRange.idx1 + 1);
      else
        theRange = String(srcBuf+rRange.idx1, rRange.idx2-rRange.idx1 + 1);      

    } // ...Pure Bytes

    else {  // It has UTF-8 in it...

      curIdx = rRange.idx1;
      while (curIdx <= rRange.idx2) {
        theChar = UTF8::GetChar(srcBuf,srcSize,curIdx);
        status = theRange.Append(theChar);
        if (status) goto SOMEERROR;
        curIdx++;
      };

    } // ...It has UTF-8 in it

  } // ...Forward (natural order)
  
  else { // Backward (reverse order)...
    
    if (byteCount == charCount) { // Pure Bytes...

      if (type == Const || type == ConstLen)
        theRange = String((char const *)srcBuf+rRange.idx2, rRange.idx1-rRange.idx2+1);
      else
        theRange = String(srcBuf+rRange.idx2, rRange.idx1-rRange.idx2+1);

      // Reverse the bytes
      if (!theRange.isMutable) {
        status = theRange.Mutable();
        if (status) goto SOMEERROR;
      }
      theRange._BufAndSize(srcBuf,srcSize);
      MemoryReverseBytes(srcBuf,srcSize);      

    } // ...Pure Bytes

    else {  // It has UTF-8 in it...

      curIdx = rRange.idx1;
      while (curIdx >= rRange.idx2) {
        theChar = UTF8::GetChar(srcBuf,srcSize,curIdx);
        status = theRange.Append(theChar);
        if (status) goto SOMEERROR;
        curIdx--;
      };

    } // ...It has UTF-8 in it

  } // ...Backward
  
  //
  // Handle errors
  //
  
  status = Error::None;
  while (false) {
    SOMEERROR:                                      break;
    OK:            status = Error::None;            break;
    EMPTY:         status = Error::None;            break;
  }

  that = theRange.Consumable();

  return status;
}

String String::Get(Slice const &slice) const {

  Status status;
  String theString;

  status = Get(slice,theString);
  if (status)
    theString = ErrorString(status);

  return theString.Consumable();
}

Status String::Get(Slice const &slice, String &that) const {

  Status   status;
  Byte    *srcBuf;
  UInt     srcSize;
  Int      curIdx;
  UInt     byteCount;
  UInt     charCount;
  Char     theChar;
  Slice    rSlice;
  String   theSlice;
  UInt     sliceLen;
  
  // Get the Source Buffer and Size (in bytes)
  _BufAndSize(srcBuf, srcSize);
  
  // We now have srcbuf and srclen, get the slice...
  if (BEFE::IsNull(srcBuf) || srcSize == 0) goto OK;

  // Get the Byte and Char count
  if (isByte && !isChar)
    byteCount = charCount = srcSize;
  else {
    status = UTF8::Count(srcBuf,srcSize,byteCount,charCount);
    if (status) goto SOMEERROR;
  }
  
  // Get the bytes...
  rSlice = ((Slice *)&slice)->Resolve(charCount);
  
  if (rSlice.step > 0) { // Forward....
  
    if (byteCount == charCount && rSlice.step == 1) { // "Fast" way...

      sliceLen = rSlice.Length();
      
      if (type == Const || type == ConstLen)
        theSlice = String((char const *)srcBuf+rSlice.idx1, sliceLen);
      else
        theSlice = String(srcBuf+rSlice.idx1, sliceLen);      

    } // ..."Fast" way

    else { // "Slow" way...

      curIdx = rSlice.idx1;
      while (curIdx < rSlice.idx2) {
        theChar = UTF8::GetChar(srcBuf,srcSize,curIdx);
        status = theSlice.Append(theChar);
        if (status) goto SOMEERROR;
        curIdx += rSlice.step;
      };

    } // ..."Slow" way
    
  } // ...Forward

  else { // Backward (reverse order)...
    
    if (byteCount == charCount && rSlice.step == -1) { // Pure Bytes...

      if (type == Const || type == ConstLen)
        theSlice = String((char const *)srcBuf+rSlice.idx2+1, rSlice.idx1-rSlice.idx2);
      else
        theSlice = String(srcBuf+rSlice.idx2, rSlice.idx1-rSlice.idx2);

      // Reverse the bytes
      if (!theSlice.isMutable) {
        status = theSlice.Mutable();
        if (status) goto SOMEERROR;
      }
      theSlice._BufAndSize(srcBuf,srcSize);
      MemoryReverseBytes(srcBuf,srcSize);      

    } // ...Pure Bytes

    else {  // "Slow" way...

      curIdx = rSlice.idx1;
      while (curIdx > rSlice.idx2) {
        theChar = UTF8::GetChar(srcBuf,srcSize,curIdx);
        status = theSlice.Append(theChar);
        if (status) goto SOMEERROR;
        curIdx += rSlice.step;
      };

    } // ..."Slow" way

  } // ...Backward
    
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                                      break;
    OK:            status = Error::None;            break;
  }

  that = theSlice.Consumable();

  return status;
  
}

} // ...Namespace BEFE