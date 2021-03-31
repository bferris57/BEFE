//----------------------------------------------------------------------
// File: StringCompare.cpp - String Comparison methods/functions
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
// Comparison Wrappers
//
// Note: These wastefull methods are here to make it easy and readible
//       when used in C++
//

Int String::Compare(String const &that) const {
  
  Status status;
  Int    answer;
  
  status = ((String *)this)->_CompareRelative(that, answer);
  if (status) answer = NaN;
  
  return answer;

}

Int String::Compare(String const &that, Boolean sensitive) const {
  
  Status status;
  Int    answer;
  
  if (sensitive)
    status = ((String *)this)->_CompareRelativeSensitive(that, answer);
  else
    status = ((String *)this)->_CompareRelativeInsensitive(that, answer);    
  if (status) answer = NaN;
  
  return answer;

}

Int String::CompareSensitive(String const &that) const {
  
  Status status;
  Int    answer;
  
  status = ((String *)this)->_CompareRelativeSensitive(that, answer);
  if (status) answer = NaN;
  
  return answer;

}

Int String::CompareInsensitive(String const &that) const {

  Status status;
  Int    answer;
  
  status = ((String *)this)->_CompareRelativeInsensitive(that, answer);
  if (status) answer = NaN;
  
  return answer;

}

Boolean String::IsEqual(String const &that) const {
  
  Status  status;
  Boolean answer;
  
  status = ((String *)this)->_CompareEquals(that, answer);
  if (status) answer = false;
  
  return answer;

}

Boolean String::CompareEquals(String const &that) const {
  
  Status  status;
  Boolean answer;
  
  status = ((String *)this)->_CompareEquals(that, answer);
  if (status) answer = false;
  
  return answer;

}

Boolean String::CompareEquals(String const &that, Boolean sensitive) const {
  
  Status  status;
  Boolean answer;
  
  if (sensitive)
    status = ((String *)this)->_CompareEqualsSensitive(that, answer);
  else
    status = ((String *)this)->_CompareEqualsInsensitive(that, answer);    
  if (status) answer = false;
  
  return answer;

}

Boolean String::CompareEqualsSensitive(String const &that) const {

  Status  status;
  Boolean answer;
  
  status = ((String *)this)->_CompareEqualsSensitive(that, answer);
  if (status) answer = false;
  
  return answer;

}

Boolean String::CompareEqualsInsensitive(String const &that) const {

  Status  status;
  Boolean answer;
  
  status = ((String *)this)->_CompareEqualsInsensitive(that, answer);
  if (status) answer = false;
  
  return answer;

}

//----------------------------------------------------------------------
//
// Low Level Comparison...
//
// Note: The "real code" is in these methods
//

Status String::_CompareEquals (String const &that, Boolean &result) {
  
  Status status;
  
  if (isSensitive  && that.isSensitive)
    status = _CompareEqualsSensitive(that, result);
  else
    status = _CompareEqualsInsensitive(that, result);
    
  return status;
  
}

Status String::_CompareRelative (String const &that, Int &result) {
  
  Status status;
  
  if (isSensitive  && that.isSensitive)
    status = _CompareRelativeSensitive(that, result);
  else
    status = _CompareRelativeInsensitive(that, result);
    
  return status;
  
}
  
Status String::_CompareEqualsSensitive (String const &that, Boolean &result) {

  Status  status;
  Int     answer;  

  status = _CompareRelativeSensitive(that, answer);
  if (status)
    result = false;
  else
    result = (answer==0);
    
  return status;
  
}

Status String::_CompareEqualsInsensitive (String const &that, Boolean &result) {

  Status  status;
  Int     answer;  

  status = _CompareRelativeInsensitive(that, answer);
  if (status)
    result = false;
  else
    result = (answer==0);
    
  return status;

}

Status String::_CompareRelativeSensitive (String const &that, Int &answer) {

  Byte    *srcbuf;
  UInt     srcbufl;
  Byte    *dstbuf;
  UInt     dstbufl;

  // Odd cases if NULL, NULL (believe it or not) is less than any non-NULL, even if empty...
  if (IsNull()) {
  	if (that.IsNull()) {
  		answer = 0;
  		goto GOTIT;
  	}
  	answer = -1;
  	goto GOTIT;
  }
  else if (that.IsNull()) {
  	answer = 1;
  	goto GOTIT;
  }

  // Get Source and Destination buffer and length
  this->_BufAndSize(srcbuf, srcbufl);
   that._BufAndSize(dstbuf, dstbufl);

  // See if they're equal...
  if (srcbuf && dstbuf) {
    answer = Strncmp(srcbuf,dstbuf,Min(srcbufl,dstbufl));
    if (answer != 0) goto GOTIT;
  }
  else if (!srcbuf && !dstbuf)
  	goto EQUALS;
  else if (srcbuf)
    goto GREATERTHAN;
  else
    goto LESSTHAN;

  // Equal to shortest length, if not same length...
  if (srcbufl < dstbufl) goto LESSTHAN;
  if (srcbufl > dstbufl) goto GREATERTHAN;

  // Equal in entire length, now check the multiples...
  if (mult < that.mult) goto LESSTHAN;
  if (mult > that.mult) goto GREATERTHAN;

  goto EQUALS;

  // Handle result
  while (false) {
    LESSTHAN:    answer = -1; break;
    EQUALS:      answer =  0; break;
    GREATERTHAN: answer =  1; break;
    GOTIT:                    break;
  }

  return Error::None;

}

Status String::_CompareRelativeInsensitive (String const &that, Int &answer) {

  Status   status;
 
  // General buffer iteration stuff...
  Byte    *thisBuf;
  UInt     thisBufLen;
  Byte    *thatBuf;
  UInt     thatBufLen;
  Byte    *bpThis;
  Byte    *bpThat;
  Byte     bThis;
  Byte     bThat;
  UInt     remLen;

  // Unicode Folding specific stuff...
  Status   statusThis;
  Status   statusThat;
  Int      posThis;
  Int      posThat;
  Char     charThis;
  Char     charThat;
  Char     foldThis;
  Char     foldThat;
  UInt     charLen;
  
  //
  // Okay, let's go...
  //
  
  status = Error::None;
  
  // Odd cases if NULL, NULL (believe it or not) is less than any non-NULL, even if empty...
  if (IsNull()) {
  	if (that.IsNull()) {
  		answer = 0;
  		goto GOTIT;
  	}
  	answer = -1;
  	goto GOTIT;
  }
  else if (that.IsNull()) {
  	answer = 1;
  	goto GOTIT;
  }

  // Get Source and Destination buffer and length
  this->_BufAndSize(thisBuf, thisBufLen);
   that._BufAndSize(thatBuf, thatBufLen);
   
  //
  // Handle Unicode Folding case
  //
  
  if (gUnicodeFolding  && (isChar || that.isChar)) { // Unicode folding...
  
    // See if they're equal...
    if (thisBuf && thatBuf) {
    
      bpThis = thisBuf;
      bpThat = thatBuf;
      remLen = Min(thisBufLen,thatBufLen);
      answer = 0;
      
      while (remLen) {

        // Get the next Chars...
        statusThis = UTF8::GetCharPos(bpThis, remLen, 0, 0, posThis, charThis);
        statusThat = UTF8::GetCharPos(bpThat, remLen, 0, 0, posThat, charThat);
        
        // Handle Errors...
        if (UTF8::GetCharSize(*bpThis) > remLen) {
          BEFE_ABORT("String.CompareRelativeInsensitive: Something's wrong here!!!");
        }
        if (statusThis || statusThat || posThis || posThat) {
          BEFE_ABORT("String.CompareRelativeInsensitive: Don't know what to do here!!!");
        }
        
        // Fold them (special case for Turkish)...
        foldThis = UTF8::Fold(charThis);
        foldThat = UTF8::Fold(charThat);
        if (gUnicodeTurkish) {
          if (foldThis.value == 0x0130 || foldThis.value == 0x131) foldThis = 'i';
          if (foldThat.value == 0x0130 || foldThat.value == 0x131) foldThat = 'i';
        }
        
        // Chars okay, see how they compare...
        if (foldThis < foldThat) {answer = -1; break;}
        if (foldThis > foldThat) {answer = 1;  break;}

        // Move on to the next character
        charLen = UTF8::GetByteSize(charThis);
        // WARN...
        if (gShowWarnings && charLen > remLen)
          BEFE_WARN("Strings._CompareRelativeInsensitive: Something weird is going on here");
        // ...WARN
        remLen -= charLen;
        bpThis += charLen;
        bpThat += charLen;
      }
      
      if (answer) goto GOTIT;
      
    }
    else if (!thisBuf && !thatBuf)
    	goto EQUALS;
    else if (thisBuf)
      goto GREATERTHAN;
    else
      goto LESSTHAN;
   
  } // ...Unicode Folding
  
  //
  // Handle non-Unicode ("normal") case...
  //
  
  else { // Normal folding...
  
    // See if they're equal...
    if (thisBuf && thatBuf) {
    
      bpThis = thisBuf;
      bpThat = thatBuf;
      remLen = Min(thisBufLen,thatBufLen);
      answer = 0;
      while (remLen) {
        bThis = *bpThis++;
        bThat = *bpThat++;
        if (bThis >= 'A' && bThis <= 'Z') bThis = bThis - 'A' + 'a';
        if (bThat >= 'A' && bThat <= 'Z') bThat = bThat - 'A' + 'a';
        if (bThis < bThat) {answer = -1; break;}
        if (bThis > bThat) {answer = 1;  break;}
        remLen--;
      }
      if (answer) goto GOTIT;
    }
    else if (!thisBuf && !thatBuf)
    	goto EQUALS;
    else if (thisBuf)
      goto GREATERTHAN;
    else
      goto LESSTHAN;

  } // ...Normal Folding
  
  // Equal to shortest length, if not same length...
  if (thisBufLen < thatBufLen) goto LESSTHAN;
  if (thisBufLen > thatBufLen) goto GREATERTHAN;

  // Equal in entire length, now check the multiples...
  if (mult < that.mult) goto LESSTHAN;
  if (mult > that.mult) goto GREATERTHAN;

  goto EQUALS;
  
  // Handle result
  while (false) {
    LESSTHAN:    answer = -1; break;
    EQUALS:      answer =  0; break;
    GREATERTHAN: answer =  1; break;
    GOTIT:                    break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Operator == and related methods
//

bool String::operator== (char const *that) const {
  return Compare(String(that)) == 0;  
}

bool String::operator== (char *that) const {
  return Compare(String(that)) == 0;
}

bool String::operator== (String const &that) const {
  return (Compare(that) == 0);
}

bool String::operator!= (char const *that) const {
  return Compare(String(that)) != 0;
}

bool String::operator!= (char *that) const {
  return Compare(String(that)) != 0;
}

bool String::operator!= (String const &that) const {
  return Compare(that) != 0;
}

bool String::operator<  (char const *that) const {
  return Compare(String(that)) < 0;
}

bool String::operator<  (char *that) const {
  return Compare(String(that)) < 0;
}

bool String::operator<  (String const &that) const {
  return Compare(that) < 0;
}

bool String::operator<= (char const *that) const {
  return Compare(String(that)) <= 0;
}

bool String::operator<= (char *that) const {
  return Compare(String(that)) <= 0;
}

bool String::operator<= (String const &that) const {
  return Compare(that) <= 0;
}

bool String::operator>  (char const *that) const {
  return Compare(String(that)) > 0;
}

bool String::operator>  (char *that) const {
  return Compare(String(that)) > 0;
}

bool String::operator>  (String const &that) const {
  return Compare(that) > 0;
}

bool String::operator>= (char const *that) const {
  return Compare(String(that)) >= 0;
}

bool String::operator>= (char *that) const {
  return Compare(String(that)) >= 0;
}

bool String::operator>= (String const &that) const {
  return Compare(that) >= 0;
}

} // ...Namespace BEFE
