//----------------------------------------------------------------------
// File: IntegerOperator.cpp - Integer Class C++ operators
//----------------------------------------------------------------------
//
// The methods in this source implement the complete set of C++
// operators.
//
// Note: These are provided for "ease of use" in C++ and aren't, really,
//       necessary for the fundamental class itself.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Integer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// operator=
//

Integer &Integer::operator= (Integer const &that) {
  StartUp();
  CopyFrom(that);
  return *this;
}

Integer &Integer::operator= (char const *that) {
  BEFE_WARN("Integer.operator=(char const *) not implemented yet");
  return *this;
}

Integer &Integer::operator= (char *that) {
  BEFE_WARN("Integer.operator=(char *) not implemented yet");
  return *this;
}

Integer &Integer::operator= (char that) {
  BEFE_WARN("Integer.operator=(char) not implemented yet");
  return *this;
}

Integer &Integer::operator=  (String const &that) {
  
  Status status;
  Byte   *buf;
  UInt    bufRem;
  Digit   curDig;
  Digit   oldDig;
  
  status = Reset();
  if (status) goto SOMEERROR;
  
  that._BufAndSize(buf, bufRem);
  if (BEFE::IsNull(buf)) goto ISEMPTY;
  
  // Skip leading whitespace
  while (bufRem && *buf <= 0x20) {buf++; bufRem--;}
  if (bufRem == 0) goto ISEMPTY;
  
  // Handle leading sign
  state = 0;
  if (*buf == '-') {
    state |= StateSGN;
    buf++;
    bufRem--;
  }
  if (!bufRem) goto MALFORMED;

  // Do each digit
  digit = 0;  
  while (bufRem) {
    curDig = *buf++;
    if (curDig < '0' || curDig > '9') goto BADDIGIT;
    curDig = curDig - '0';
    oldDig = digit;
    digit = digit*10 +curDig;
    if (digit < oldDig) goto OVERFLOW;
    bufRem--;
  }
  
  // Finish off
  type = TypeDigit;
  
  // Handle errors
  status = Error::None;
  while (false) {
    ISEMPTY:   status = Error::Failed; break;
    MALFORMED: status = Error::Failed; break;
    BADDIGIT:  status = Error::Failed; break;
    OVERFLOW:  status = Error::Failed; break;
    SOMEERROR:                         break;
  }
  if (status)
    Reset();
    
  return *this;
  
}

Integer &Integer::operator= (UInt8 that) {
  BEFE_WARN("Integer.operator=(UInt8) not implemented yet");
  return *this;
}

Integer &Integer::operator= (SInt8 that) {
  BEFE_WARN("Integer.operator=(SInt8) not implemented yet");
  return *this;
}

Integer &Integer::operator= (UInt16 that) {
  BEFE_WARN("Integer.operator=(UInt16) not implemented yet");
  return *this;
}

Integer &Integer::operator= (SInt16 that) {
  BEFE_WARN("Integer.operator=(SInt16) not implemented yet");
  return *this;
}

Integer &Integer::operator= (UInt32 that) {

  ShutDown();
  StartUp();

  if ( IsNaN(that) ) { // NaN...
    state = StateNaN;
    type  = TypeNone;
  }
  else if (that > 0) { // Positive...
    state = StateNatural;
    type  = TypeDigit;
    digit = that;
  } // ...Positive
  else { // Zero...
    state = StateZero;
    type  = TypeDigit;
    digit = 0;
  } // ...Zero

  return *this;

}

Integer &Integer::operator= (SInt32 that) {
  
  ShutDown();
  StartUp();

  if ( IsNaN(that) ) { // NaN...
    state = StateNaN;
    type  = TypeNone;
  }
  else if (that < 0) { // Negative...
    state = StateNegNatural;
    type  = TypeDigit;
    digit = (UInt32)(-that);
  } // ...Negative
  else if (that > 0) { // Positive...
    state = StateNatural;
    type  = TypeDigit;
    digit = that;
  } // ...Positive
  else { // Zero...
    state = StateZero;
    type  = TypeDigit;
    digit = 0;
  } // ...Zero

  return *this;

}

Integer &Integer::operator= (UInt64 that) {
  BEFE_WARN("Integer.operator=(UInt64) not implemented yet");
  return *this;
}

Integer &Integer::operator= (SInt64 that) {
  BEFE_WARN("Integer.operator=(SInt64) not implemented yet");
  return *this;
}

//----------------------------------------------------------------------
//
// operator+
//

Integer Integer::operator+  (Integer const &that) const {

  Status   status;
  Integer *num1;
  Integer *num2;
  Integer  result;
  Boolean  isSpecial;
  
  // Determine what to do
  isSpecial = false;
  switch (type) {
    case TypeNone:
      goto FINISHED;
    case TypeDigit:
      if ((state & StateInfinity) || (that.state & StateInfinity)) goto FINISHED;
      isSpecial = true;
      break;
    case TypeBuffer:
      if ((state & StateInfinity) || (that.state & StateInfinity)) goto FINISHED;
      break;
    default:
      status = Error::Internal;
      BEFE_ERROR("Integer.operator+",status);
      goto SOMEERROR;
  }
  
  //
  // If special case...
  //
  
  if (isSpecial) {

    if ((state ^ that.state) & StateSGN) { // Opposite signs...

      if (digit == 0 || that.digit == 0) { // One or both are zero...
      
        if (digit == 0)
          result = that;
        else
          result = *this;
        
      } // ...One or both are zero
      
      else { // Both are non-zero...
      
        // Make sure first one, regardless of sign, is greater or equal to second
        if (digit < that.digit) {
          num1 = (Integer *)&that;
          num2 = (Integer *)this;
        }
        else {
          num1 = (Integer *)this;
          num2 = (Integer *)&that;
        }
        
        // Subtract the digits
        result = *num1;
        result.digit -= num2->digit; 

      } // ...Both are non-zero

    } // ...Opposite signs

    else { // Same sign...
    
      result.state = state & StateSGN;
      result.type  = TypeDigit;
      result.digit = digit + that.digit;
      if (result.digit < Min(digit, that.digit)) goto BUFNOTIMP;

    }
    
    // Discard sign if zero...
    if (result.digit == 0)
      result.state = StateZero;
    
    goto FINISHED;
    
  }
  
  //
  // Not special case, do big number operation
  //

  status = Add(*this,that,result);
  if (status) goto SOMEERROR;
  
  goto FINISHED;
  
  // Handle errors
  while (false) {
    BUFNOTIMP: BEFE_WARN("Integer type TypeInteger not implemented yet");
               result.Reset();
               break;
    SOMEERROR: BEFE_ERROR("Integer.operator+",status);
               result.Reset();
               break;
    FINISHED:  break;
  }
  
  return result.Consumable();

}

Integer Integer::operator+  (char const *that) const {
  return *this + Integer(that);
}

Integer Integer::operator+  (char *that) const {
  return *this + Integer(that);
}

Integer Integer::operator+  (char that) const {
  return *this + Integer(that);
}

Integer Integer::operator+  (UInt8 that) const {
  return *this + Integer(that);
}

Integer Integer::operator+  (SInt8 that) const {
  return *this + Integer(that);
}

Integer Integer::operator+  (UInt16 that) const {
  return *this + Integer(that);
}

Integer Integer::operator+  (SInt16 that) const {
  return *this + Integer(that);
}

Integer Integer::operator+  (UInt32 that) const {
  return *this + Integer(that);
}

Integer Integer::operator+  (SInt32 that) const {
  return *this + Integer(that);
}

Integer Integer::operator+  (UInt64 that) const {
  return *this + Integer(that);
}

Integer Integer::operator+  (SInt64 that) const {
  return *this + Integer(that);
}

//----------------------------------------------------------------------
//
// operator+
//

Integer Integer::operator-  (Integer const &that) const {

  Integer answer;
  
  ((Integer *)&that)->state ^= StateSGN;
  answer = *this + that;
  ((Integer *)&that)->state ^= StateSGN;
  
  return answer.Consumable();
  
}

Integer Integer::operator-  (char const *that) const {
  return *this + Integer(that);
}

Integer Integer::operator-  (char *that) const {
  return *this + Integer(that);
}

Integer Integer::operator-  (char that) const {
  return *this + Integer(that);
}

Integer Integer::operator-  (UInt8 that) const {
  return *this + Integer(that);
}

Integer Integer::operator-  (SInt8 that) const {
  return *this + Integer(that);
}

Integer Integer::operator-  (UInt16 that) const {
  return *this + Integer(that);
}

Integer Integer::operator-  (SInt16 that) const {
  return *this + Integer(that);
}

Integer Integer::operator-  (UInt32 that) const {
  return *this + Integer(that);
}

Integer Integer::operator-  (SInt32 that) const {
  return *this + Integer(that);
}

Integer Integer::operator-  (UInt64 that) const {
  return *this + Integer(that);
}

Integer Integer::operator-  (SInt64 that) const {
  return *this + Integer(that);
}

//----------------------------------------------------------------------
//
// Typecasting...
//

Integer::operator Int()  const {
  
  Int val;
  
  BEFE::SetNull(val);
  switch (state) {
    case StateNatural:    val = (Int)digit;  break;
    case StateZero:       val = 0;           break;
    case StateNegNatural: val = -(Int)digit; break;
    default: break;
  }
  
  return val;
  
}

Integer::operator UInt () const {
  
  Int val;
  
  BEFE::SetNull(val);
  switch (state) {
    case StateNatural:    val = (Int)digit;  break;
    case StateZero:       val = 0;           break;
    default: break;
  }
  
  return val;
  
}

} // ...Namespace BEFE