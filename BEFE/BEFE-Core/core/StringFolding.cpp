//----------------------------------------------------------------------
// File: StringFolding.cpp - String Case Folding methods
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
// C++ Wrappers
//

String &String::ToLowercase() const {
  
   //Status status;
   
   /*status =*/ ((String *)this)->Fold(true);

   return *(String *)this;

}

String &String::ToUppercase() const {
  
   //Status status;
   
   /*status =*/ ((String *)this)->Fold(false);

   return *(String *)this;

}

String String::Lowercase() {
  
  String that;
  
  that = *this;
  that.ToLowercase();
  
  return that.Consumable();
  
}

String String::Uppercase() {
  
  String that;
  
  that = *this;
  that.ToUppercase();
  
  return that.Consumable();
  
}

//----------------------------------------------------------------------
//
// Low Level methods
//

Status String::Fold(Boolean doFold) {

  Status  status;
  Byte   *buf;
  UInt    byteCount;
  UInt    charCount;
  Char    theChar;
  UInt    charLen;
  UInt    newCharLen;
  Byte    utfBuf[UTF8::MaxLength];
  Byte   *bpSrc;
  Byte   *bpDst;

  static UInt numErrors = 0; // Used to keep from displaying multiple errors
  
  // If not mutable, make it mutable
  status = ((String *)this)->Mutable();
  if (status) goto SOMEERROR;
  
  // Figure out if we've got UTF-8 if we don't already know...
  if (isByte == isChar) {
    status = ((String *)this)->_BufByteAndCharCount(buf,byteCount,charCount);
    if (status) goto SOMEERROR;
    if (byteCount == charCount) {
      ((String *)this)->isByte = true;
      ((String *)this)->isChar = false;
    }
    else {
      ((String *)this)->isByte = false;
      ((String *)this)->isChar = true;
    }
  }
  
  // Get buffer and size (in Bytes)...
  _BufAndSize(buf,byteCount);
  if (BEFE::IsNull(buf) || byteCount == 0) goto OK;

  //
  // Unicode Folding case...
  //
  
  if (UTF8::IsUnicodeFolding() && isChar) { // Unicode Folding case...
  
    while (byteCount > 0) {
      status = UTF8::DecodeChar(buf, theChar, charLen);
      if (status && status != Error::UTF8InvalidChar) goto SOMEERROR;
      if (!status && charLen) {
        if (doFold)
          theChar = UTF8::Fold(theChar);
        else
          theChar = UTF8::Unfold(theChar);
        status  = UTF8::EncodeChar(theChar, utfBuf, newCharLen);
        if (status) goto SOMEERROR;
        if (newCharLen != charLen) goto BADFOLD;
        bpSrc = utfBuf;
        bpDst = buf;
        while (newCharLen) {
          *bpDst++ = *bpSrc++;
          newCharLen--;
        }
      }
      if (status || charLen == 0)
        charLen = 1;
      buf       += charLen;
      byteCount -= charLen;
    }
      
  } // ...Unicode Folding case
  
  //
  // Normal Folding Case...
  //
  
  else { // ASCII Folding case...
  
    if (doFold) { // Fold to lowercase...
      while (byteCount > 0) {
        if (*buf >= 'A' && *buf <= 'Z')
          *buf = *buf - 'A' + 'a';
        ++buf;
        --byteCount;
      }

    } // ...Fold to lowercase
    
    else { // Unfold to uppercase...

      while (byteCount > 0) {
        if (*buf >= 'a' && *buf <= 'z')
          *buf = *buf - 'a' + 'Z';
        ++buf;
        --byteCount;
      }

    } // ...Unfold to uppercase

  } // ...ASCII Folding case
  
  // Handle errors
  status = Error::None;
  while (false) {    
    OK:        status = Error::None;     break;
    SOMEERROR:                           break;
    // Internal errors...
    BADFOLD:   if (numErrors++)
               BEFE_ERROR("String.Fold: Bad Unicode fold length",status);
               status = Error::Internal;
               break;
  }

  return status;

}

} // ...Namespace BEFE