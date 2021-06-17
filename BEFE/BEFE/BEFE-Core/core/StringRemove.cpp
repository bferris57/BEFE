//----------------------------------------------------------------------
// File: StringRemove.cpp - String Class Remove methods
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
// Single Char Remove...
//

Status String::Remove(Int index) {

  Status   status;
  Byte    *buf;
  UInt     bufSize;
  UInt     sLen;
  Char     chr;
  Int      charPos;
  Int      charLen;
  Int      curPos;
  SBuffer *sBuf;
  
  sLen = Length();

  // Adjust if negative
  if (index < 0) {
    index = sLen + index;
  }
  if (index < 0 || (UInt)index >= sLen) goto OUTOFRANGE;
  
  // Based on type...
  switch (type) {
    case Null:
      goto OUTOFRANGE;
    case Const:
      goto NOTMUTABLE;
    case ConstLen:
      goto NOTMUTABLE;
    case ASCII0:
      type = Null;
      break;
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      buf     = (Byte *)&data;
      bufSize = type - ASCII0;
      Memmove(buf+index,buf+index+1,bufSize-index);
      bufSize--;
      buf[bufSize] = 0x00;
      type = type-1;
      constLen = type-ASCII0;
      break;
    case UtfInline:
      type = ASCII0;
      constLen = 0;
      break;
    case Buffer:
      // Get the buffer and length...
      _BufAndSize(buf,bufSize);
      if (BEFE::IsNull(buf) || bufSize == 0) goto OUTOFRANGE;
      // ASCII, or ask UTF-8 where it is
      if (isByte && !isChar) {
        charPos = index;
        charLen = 1;
      }
      else { // UTF-8...
        curPos = 0;
        status = UTF8::GetCharPos(buf,bufSize,index,curPos,charPos,chr);
        if (status) goto SOMEERROR;
        charLen = UTF8::GetByteSize(chr);
      } // ...UTF-8
      // Remove it
      sBuf = (SBuffer *)&data;
      status = sBuf->RemoveBytes(charPos,charLen);
      if (status) goto SOMEERROR;
      break;
    default:
      goto INTERNALERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFRANGE:    status = Error::IndexOutOfRange;  break;
    INTERNALERROR: status = Error::Internal;         break;
    NOTMUTABLE:    status = Error::StringNotMutable; break;
    SOMEERROR:                                       break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Range Remove...
//

Status String::Remove(Range const &range) {
  
  Status status;
  Range  phyRange;
  UInt   phyLen;
  Byte  *buf;
  UInt   bufLen;
  
  // Special case for '..'
  if ( IsNaN(range.idx1) &&  IsNaN(range.idx2) ) {
    status = Reset();
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Compute physical Range...
  phyRange = _LogToPhyRange(range);
  if (!phyRange.IsResolved()) goto AMBIGUOUS;
  if (phyRange.IsDescending())
    phyRange.Reverse();
  phyLen = phyRange.Length();
  if ( BEFE::IsNull(phyLen) ) goto AMBIGUOUS;
  if (phyLen == 0) goto OK;
  
  // Make it mutable if possible...
  if (!IsMutable()) goto NOTMUTABLE;

  // Get the physical address and length
  _BufAndSize(buf, bufLen);
  if (BEFE::IsNull(buf) || bufLen == 0) goto INTERNAL;
  
  // Move the bytes
  MemoryCopyRaw(buf + phyRange.idx2 + 1, buf + phyRange.idx1, bufLen - phyRange.idx2 - 1);
  
  // Adjust size based on bytes
  bufLen -= phyLen;
  switch (type) {
    case Null:
    case Const:
      goto INTERNAL;
    case ConstLen:
      if (phyLen > constLen) goto INTERNAL;
      constLen -= phyLen;
      break;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      if (type <= ASCII0 || phyLen > type-ASCII0) goto INTERNAL;
      type -= phyLen;
      constLen -= phyLen;
      break;
    case UtfInline:
      if (phyLen != 1) goto INTERNAL;
      type = ASCII0;
      constLen = 0;
      break;
    case Buffer:
      status = ((SBuffer *)&data)->SetUsedBytes(bufLen);
      if (status) goto SOMEERROR;
      break;
    default:
      goto INTERNAL;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    AMBIGUOUS:  status = Error::StringRemoveInvalid; break;
    NOTMUTABLE: status = Error::StringNotMutable;    break;
    INTERNAL:   status = Error::Internal;            break;
    OK:         status = Error::None;                break;
    SOMEERROR:                                       break;
  }
  
  if (status == Error::Internal)
    BEFE_ERROR("String.Remove(Span const &)",status);
    
  return status;
  
}

//----------------------------------------------------------------------
//
// Span Remove...
//

Status String::Remove(Span const &span) {
  
  Status status;
  Span   phySpan;
  UInt   phyLen;
  Byte  *buf;
  UInt   bufLen;
  
  // Special case for ':'
  if ( IsNaN(span.idx1) &&  IsNaN(span.idx2) ) {
    status = Reset();
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Compute physical Span...
  phySpan = _LogToPhySpan(span);
  if (phySpan.IsDescending())
    phySpan.Reverse();
  phyLen = phySpan.Length();
  if ( BEFE::IsNull(phyLen) ) goto AMBIGUOUS;
  if (phyLen == 0) goto OK;
  
  // Make it mutable if possible...
  if (!IsMutable()) goto NOTMUTABLE;

  // Get the physical address and length
  _BufAndSize(buf, bufLen);
  if (BEFE::IsNull(buf) || bufLen == 0) goto INTERNAL;
  
  // Move the bytes
  MemoryCopyRaw(buf + phySpan.idx1+phyLen, buf + phySpan.idx1, bufLen - phySpan.idx1 - phyLen);
  
  // Adjust size based on bytes
  switch (type) {
    case Null:
    case Const:
      goto INTERNAL;
    case ConstLen:
      if (phyLen > constLen) goto INTERNAL;
      constLen -= phyLen;
      break;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      if (type <= ASCII0 || phyLen > type-ASCII0) goto INTERNAL;
      type -= phyLen;
      constLen -= phyLen;
      break;
    case UtfInline:
      if (phyLen != 1) goto INTERNAL;
      type = ASCII0;
      constLen = 0;
      break;
    case Buffer:
      status = ((SBuffer *)&data)->SetUsedBytes(bufLen-phyLen);
      if (status) goto SOMEERROR;
      break;
    default:
      goto INTERNAL;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    AMBIGUOUS:  status = Error::StringRemoveInvalid; break;
    NOTMUTABLE: status = Error::StringNotMutable;    break;
    INTERNAL:   status = Error::Internal;            break;
    OK:         status = Error::None;                break;
    SOMEERROR:                                       break;
  }
  
  if (status == Error::Internal)
    BEFE_ERROR("String.Remove(Span const &)",status);
    
  return status;
  
}

} // ...Namespace BEFE
