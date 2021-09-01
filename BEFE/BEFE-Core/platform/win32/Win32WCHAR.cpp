//----------------------------------------------------------------------
// File: Win32WCHAR.cpp - Win32 WCHAR utility Functions Implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "Win32.h"
#include "BEFE.h"
#ifdef ERROR  // So we can use our own Bloody ERROR instead of Microsoft's silly one...
#  undef ERROR
#endif
#include "BEFEMacros.h"
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

UInt Win32W_Strlen(UShort *str) {
  UInt slen;
  slen = 0;
  if (str)
    while (*str++) ++slen;
  return slen;
}

UInt Win32W_Strcpy(UShort *dst, UShort *src) {
  UInt slen;
  slen = Win32W_Strlen(src);
  if (slen) {
    slen++;
    MemoryCopyRaw((Byte *)src, (Byte *)dst, slen*sizeof(UShort));
  }
  return slen;
}
  
UInt Win32W_UShortToByteLen(UShort *str) {
  UInt bLen;
  bLen = 0;
  while (*str) {
    if (*str <= 0x7fu)
      bLen++;
    else if (*str >= 0xD800u && *str <= 0xDFFFu)
      bLen += 4;
    else
      bLen += Char(*str).ByteLength();
    str++;
  }
  return bLen;
}

UInt Win32W_ByteToUShortLen(Byte *str, UInt numBytes) {
  UInt sLen;
  UInt cLen;
  sLen = 0;
  if (str) {
    while (numBytes) {
      if (*str <= 0x7fu)
        cLen = 1;
      else
        cLen = Char().CharLength(str);
      if (cLen == 0)
        str++;
      else {
        str += cLen;
        sLen++;
      }
      numBytes--;
    }
  }
  return sLen;
}

String Win32W_ToString(UShort *str) {
  
  Status status;
  String theString;
  Char   theChar;
  UInt   numBytes;
  UInt   numErrors;
  UInt   numWarnings;
  
  numErrors = 0;
  numWarnings = 0;
  numBytes = Win32W_UShortToByteLen(str);
  if (str && numBytes) {
    status = theString._Allocate(numBytes);
    if (status) {
      if (!numErrors++)
        BEFE_ERROR("Win32W_ToString",status);
    }
    while (*str) {
      if (*str >= 0xD800u && *str <= 0xDFFFu) {
        if (*str & (0xD800u != 0xD800)) 
          BEFE_ABORT("Win32W_ToString: Invalid Surrogate Pair detected!!!");
        theChar.value   = *(str++) - 0xD800u;
        theChar.value  *= 0x400u;
        theChar.value  |= *(str++) - 0xDC00u;
        theChar.value  += 0x10000u;
        theChar = Char(theChar.value); // To validate it
      }
      else
        theChar = Char((UInt)*str++);        
      if (IsNull(theChar)) {
        if (!numWarnings++)
          BEFE_WARN("Win32W_ToString: Invalid UTF-16 surrogate pair");
      }
      status = theString.Append(theChar);
      if (status) {
        if (!numErrors++)
          BEFE_ERROR("Win32W_ToString",status);
      }
    }
  }

  return theString.Consumable();
  
}

Status Win32W_FromString(String const &str, UShort *outStr, UInt maxLen, UInt &numUsed) {
  
  Status  status;
  Byte   *inBuf;
  UInt    inBufSize;
  UShort *outBuf;
  UInt    remInSize;
  UInt    remOutLen;
  Char    theChar;
  UInt    charBytes;
  
  numUsed = 0;
  str._BufAndSize(inBuf, inBufSize);
  if (IsNull(inBuf) || IsNull(outStr)) goto NOBUF;
  
  // Until we're finished...
  
  remInSize = inBufSize;
  remOutLen = maxLen;
  outBuf = outStr;
  
  while (remInSize) {
    
    // Get Input Character
    if (*inBuf <= 0x7f) {
      theChar = Char(*(inBuf++));
      remInSize--;
    }
    else {
      status = UTF8::DecodeChar(inBuf, theChar, charBytes);
      if (charBytes > remInSize) break;
      if (status) goto SOMERROR;
      if (charBytes) {
        inBuf += charBytes;
        remInSize -= charBytes;
      }
      else {
        inBuf += 1;
        remInSize--;
      }
    }
    
    // Output the Character...
    if (remOutLen == 0) goto TRUNCATED;    
    if (theChar <= 0xFFFFu) { // "Normal"...
      *outBuf++ = (UShort)theChar;
      remOutLen--;
      numUsed++;
    } // ..."Normal"
    else { // Needs Surrogate Pair...
      if (remOutLen < 2) goto TRUNCATED;
      theChar.value -= 0x10000u;
      *outBuf++ = (UShort)theChar.value/0x400u + 0xD800u;
      remOutLen--;
      *outBuf++ = ((UShort)theChar.value&0x3FFu) + 0xDC00u;
      remOutLen--;
      numUsed += 2;
    } // ...Needs Surrogate Pai
  }

  // Append trailing NUL if we can
  if (remOutLen == 0) goto TRUNCATED;
  *outBuf = 0;
  remOutLen--;
  numUsed++;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOBUF:     status = Error::InternalNullPointer; break;
    TRUNCATED: status = Error::ValueTruncated;      break;
    SOMERROR:                                       break;
  }
  
  return status;
  
}

} // ...Namespace BEFE
