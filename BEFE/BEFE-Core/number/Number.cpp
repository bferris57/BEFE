//----------------------------------------------------------------------
// File: Number.cpp - Number Class implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Number.h"
#include "ToString.h"
#include "Crc32.h"

namespace BEFE { // Namespace BEFE...
  
//
// Handy defines
//

#define MAXMANT     511     // Maximum mantissa bytes (excluding NUL)
#define MAXDECEXP   1023    // Maximum 10's exponent
#define MINDECEXP  -1023    // Minimum 10's exponent

//----------------------------------------------------------------------
//
// General Number Functions...
//

Boolean IsNull(Number const &v) {
  return v.IsNull();
}

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Number::Number() {

  StartUp();

}

Number::Number(char const *cstr) {

  String str;

  StartUp();
  str = cstr;
  FromString(str);
  
}

Number::Number(String const &str) {

  StartUp();
  FromString(str);
  
}

Number::Number(Number const &that) {
  StartUp();
  CopyFrom(that);
}

Number::~Number() {
  ShutDown();
}

//----------------------------------------------------------------------
//
// Befe Lifecycle
//

Status Number::StartUp() {

  sign      = false;
  eSign     = false;
  isInf     = false;
  len       = 0;
  exp       = 0;
  value[0]  = value[1] = 0;
  u.pointer = NULL;
  
  return Error::None;
}

Status Number::ShutDown() {
  
  Status status;
  
  if (len == 0)
    status = ((Buffer *)&u.pointer)->ShutDown();
  else
    status = Error::None;
    
  return status;
  
}

Status Number::Reset() {
  Status retStatus;
  Status status;
  retStatus = ShutDown();
  status    = StartUp();
  if (status && !retStatus) retStatus = status;
  return retStatus;
}

Status Number::MoveFrom(Number const &that) {

  Status  status;
  UInt32 *tsrc;
  UInt32 *tdst;
  
  // NOTE: This (UInt 32 *) stuff is quick but DANGEROUS!!!
  tsrc = (UInt32 *)&that;
  tdst = (UInt32 *)this;
  *tdst++ = *tsrc++;
  *tdst   = *tsrc;
  
  status = ((Number *)&that)->StartUp();
  
  return status;
  
}

Status Number::CopyFrom(Number const &that) {
  
  Status  status;
  UInt32 *tsrc;
  UInt32 *tdst;
  
  if (this == &that) goto FINISHED;
  
  if (that.IsConsumable()) {
    status = MoveFrom(that);
    if (status) goto SOMEERROR;
  }
  else {
    // NOTE: This (UInt 32 *) stuff is quick but DANGEROUS!!!
    tsrc = (UInt32 *)&that;
    tdst = (UInt32 *)this;
    *tdst++ = *tsrc++;
    *tdst   = *tsrc;
    if (len == 0)
      *((Buffer *)&u.pointer) = *((Buffer *)&that.u.pointer);
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    FINISHED:  status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;

  
}

Number &Number::Consumable() const {
  if (len == 0 && !BEFE::IsNull(u.pointer))
    ((Buffer *)&u.pointer)->Consumable();
  return *((Number *)this);
}

Number &Number::NotConsumable() const {
  if (len == 0 && !BEFE::IsNull(u.pointer))
    ((Buffer *)&u.pointer)->NotConsumable();
  return *((Number *)this);
}

Boolean Number::IsConsumable() const {
  if (len != 0 || BEFE::IsNull(u.pointer))
    return false;
  return ((Buffer *)&u.pointer)->IsConsumable();
}

UInt Number::Length() const {

  UInt retLen;

  if (len == 0)
    retLen = ((Buffer *)&u.pointer)->Length();
  else
    retLen = len;
  
  return retLen;

}

UInt Number::Size() const {
  
  UInt retLen;
  
  if (len == 0)
    retLen = ((Buffer *)&u.pointer)->Size();
  else
    retLen = len;
    
  return retLen;
  
}
  
//----------------------------------------------------------------------
//
// C++ Operators
//

Boolean Number::operator == (Number const &that) const {
  UInt answer;
  answer = Compare(that);
  return answer==0;
}

Number &Number::operator = (Number const &that) {
  
  if (this != &that) {
    Reset();
    if (that.IsConsumable()) {
      MoveFrom(that);
      NotConsumable();
    }
    else
      CopyFrom(that);
  }

  return *this;

}

//----------------------------------------------------------------------
//
// Setting contents
//

Status Number::FromString(String const &string) {

  Status  status;           // Return status
  Byte   *srcBuf;           // Source string buffer
  UInt    srcLen;           // Source string length (in bytes)
  Byte   *cp;               // Current character in the string
  UInt    mlen;             // Current mantissa len (bytes)
  UInt    mdigs;            // Current mantissa len (digits)
  Int     lexp;             // Local exponent
  Int     sexp;             // Specified exponent
  Byte    vbyte;            // Current value byte
  Byte    vdigs;            // Number if digits in current value byte
  Boolean haveDot;          // "Did we hit a '.'?"
  Byte    ourBuf[MAXMANT];  // Our internal digit buffer
  Int     i;                // Temporary Loop counter
  
  status = Reset();
  if (status) goto SOMEERROR;
  
  // Initialise stuff
  mlen   = 0;
  mdigs  = 0;
  lexp   = 0;
  vbyte  = 0;
  vdigs  = 0;
  
  // Get the source buffer and length
  string._BufAndSize(srcBuf, srcLen);
  
  //
  // Initial setup and scan of the string
  //

  // If no string, set to NULL
  if (srcLen == 0) {
    SetNaN();
    goto OK;  
  }

  // We have a string...  Position to 1st non-white
  for (cp = srcBuf; srcLen && (*cp == ' ' || *cp == '\t'); cp++, srcLen--);
  
  // If no string left, bad number
  if (!srcLen) goto BADNUMBER;
  
  // Trim trailing blanks
  for (i = srcLen - 1; i > 0 && (cp[i] == ' ' || cp[i] == '\t'); srcLen--, i--);
  
  // Handle sign
  if (*cp == '+') {
    cp++;
    srcLen--;
  }
  else if (*cp == '-') {
    sign = 1;
    cp++;
    srcLen--;
  }

  // If no string left, it's bad
  if (!srcLen) goto BADNUMBER;
  
  // Skip leading zero's
  while (srcLen && *cp == '0') {cp++; srcLen--;}
  
  // If nothing left, it's zero
  if (!srcLen) goto TIDYUP;
  
  // We haven't hit a '.' yet
  haveDot = 0;
  
  //
  // Main Digit Processing (each digit until we hit '.' or 'E' or end of string)...
  //
  
  for (;srcLen ; srcLen--, cp++) { // For each digit...
  
    // If we hit a 'E', finished
    if (*cp == 'E' || *cp == 'e')
      break;
      
    // If we hit a '.'...
    if (*cp == '.') {
      if (haveDot) goto BADNUMBER;
      haveDot = 1;
      if (mdigs == 0)
        lexp = -1;
      continue;
    }
      
    // If the byte isn't 0-9, error
    if (*cp < '0' || *cp > '9') goto BADNUMBER;      

    // Make sure mantissa isn't too long...
    if (mlen >= MAXMANT) goto TOOMANYMDIGS;
    
    // Flush current mantissa byte if we've got 2 digits...
    if (vdigs >= 2) {
    
      // Flush the byte
      ourBuf[mlen] = vbyte;
      
      // Adjust counters
      vdigs = 0;
      vbyte = 0;
      mdigs++;
      mlen++;      
    }
    
    // If we don't have a digit yet and this one is '0', decrease exp
    if (mdigs == 0 && *cp == '0') {
      if (haveDot)
        lexp--;
      continue;
    }

    // If we haven't hit a '.' yet
    if (!haveDot) {
      
      // Increase exponent
      if (mdigs)
        lexp++;

    }
      
    // Add the digit into the value byte
    vbyte = vbyte * 10 + (int)(*cp - '0');
    
    // Adjust counters
    vdigs++;
    mdigs++;
    
  } // ...For each digit
    
  // 
  // Handle Exponent
  //
    
  // If we're finished with the string...
  if (srcLen <= 0) goto TIDYUP;
    
  // If not 'E' or 'e', error
  if (*cp != 'E' && *cp != 'e') goto BADNUMBER;

  // Consume the 'E'
  cp++;
  srcLen--;
  
  // If no length, error
  if (srcLen <= 0) goto BADNUMBER;
    
  // Assume positive exponent sign
  eSign = 0;
  sexp  = 0;
  
  // If '+' or '-'...
  if (*cp == '+' || *cp == '-') {
  
    // If '-', set the sign
    if (*cp == '-')
      eSign = 1;
      
    // Consume it
    cp++;
    srcLen--;

  }
    
  // If no length, error
  if (srcLen <= 0) goto BADNUMBER;  

  // Get exponent
  for (; srcLen; srcLen--, cp++) { // For each exponent Byte...
  
    // Error if it's not a digit
    if (*cp < '0' || *cp > '9') goto BADNUMBER;
     
    // Add digit to exp
    sexp = sexp * 10 + (int)(*cp - '0');
    
    // Make sure it's not too big (we don't want int overflow losing the exp)
    if (sexp > MAXDECEXP*10) goto BADEXPONENT;

  } // ...For each exponent Byte
  
  // Adjust if negative
  if (eSign) {
    sexp = -sexp;
  }
 
  // Add it into current exponent
  lexp = lexp + sexp;
  
  //
  // Tidy up...
  //
  
TIDYUP:

  // Handle zero...
  if (mdigs == 0) {
    value[0] = 0x00;
    len = 1;
    goto OK;
  }
  
  // If mantissa bytes...
  if (vdigs) {
  
    // If odd mantissa bytes, adjust
    if (vdigs % 2)
      vbyte *= 10;
  
    // Flush the byte
    ourBuf[mlen] = vbyte;
    
    // Adjust counters
    mlen++;

  }

  // Output trailing NULL (for debugging purposes)
  ourBuf[mlen] = 0;

  // Make sure the exponent isn't too big/small
  if (lexp > MAXDECEXP || lexp < MINDECEXP)
    goto BADEXPONENT;
    
  // Remove tailing insignificant zeros...
  //
  // NOTE: Need to make sure mlen > 0 at this point!!!
  for (cp = &ourBuf[mlen-1]; cp >= &value[0] && *cp == 0x00; cp--) {
    *cp = 0;
    mlen--;
  }
  
  //
  // Now, move into the number
  //
  
  if (lexp >= 0) {
    exp = lexp;
    eSign = 0;
  }
  else {
    exp   = -lexp;  
    eSign = 1;
  }
  
  // If it will fit into the Number itself...
  if ( mlen && mlen <= 6) {
    
    MemoryCopyRaw(ourBuf, value, mlen);
    len = mlen;
    
  }
  
  // It won't fit into the Number, make a Byte Buffer...
  else {
    len = 0;
    status = ((Buffer *)&u.pointer)->StartUp();
    if (status) goto SOMEERROR;
    status = ((Buffer *)&u.pointer)->AppendBytes(ourBuf, mlen);
    if (status) goto SOMEERROR;
  }
  
  //
  // Handle errors
  //
  status = Error::None;
  while (false) {
    OK:           status = Error::None;              break;  
    SOMEERROR:                                       break;
    BADNUMBER:    status = Error::NumberInvalid;     break;
    BADEXPONENT:  status = Error::NumberBadExponent; break;
    TOOMANYMDIGS: status = Error::NumberBadMantissa; break;
  }
  
  // If error, clear it...
  if (status)
    value[0] = 0;

  return status;
  
}

//----------------------------------------------------------------------
//
// Setting contents
//

String Number::ToString() const {
  
  Status status;
  String theString;
  
  status = ToString(theString);
  if (status) theString = ErrorString(status);
  
  theString.Consumable();
  return theString;
  
}

Status Number::ToString(String &string) const {

  Status  status;           // Return status
  Byte   *curByte;          // Current 100's digit Byte
  Int     numBytes;         // Remaining 100's digits
  Int     curDigs;          // Current 100's digit
  Int     digsLeft;         // Digits left until decimal point
  Int     numDigs;          // Number of digits (base 10) left in this byte
  Int     nonZero;          // Number of non-zero digits we've output
  Byte    theDig;           // The current digit

  // Clear the output string
  status = string.Reset();
  if (status) goto SOMEERROR;
  
  // Special cases
  if (len == 0 && !u.pointer) {
    string = "NaN";
    goto OK;
  }
  else if (len == 1 && value[0] == 0) {
    string = "0";
    goto OK;
  }
  else if (isInf) {
    if (sign)
      string = "-Inf";
    else
      string = "Inf";
    goto OK;
  }
  
  // Get the Digits pointer and Length
  if (len > 0) {
    curByte  = (Byte *)&value[0];
    numBytes = len;
  }
  else {
    numBytes = ((Buffer *)&u.pointer)->GetUsedBytes();
    curByte  = ((Buffer *)&u.pointer)->GetBuf();
  }
  
  // Initialise string stuff
  nonZero = 0;
    
  // If negative...
  if (sign)
    string += '-';

  // Figure out how many decimal places we have left until '.'
  digsLeft = exp;
  if (eSign)
    digsLeft = -digsLeft;
  else
    digsLeft++;
    
  // Until we've exhausted the decimal point...
  for (numDigs = 0; digsLeft > 0; digsLeft--, numDigs--) { // For each integer digit...
  
    // If no more digits, get another byte
    if (numDigs <= 0) {
    
      // Get the digits
      if (numBytes == 0) break;
      curDigs = (Int)(*curByte++);
      numBytes--;
      
      // We've got two digits now
      numDigs = 2;
    }
    
    // If no more digits...
    if (curDigs < 0)
    
      // The digit it '0'
      theDig = 0;
      
    // Else, we have more digits...
    else {
    
      if (numDigs == 2)
        theDig = curDigs/10;
      else
        theDig = curDigs%10;
    }
        
    // If digit is non-zero OR we've already put non-zero digits out...
    if (theDig || nonZero) {
    
      // Output the digit
      string += Char((UInt)theDig + '0');
      
      // We've now put a digit out...
      nonZero++;
    }

  } // For each integer digit
  
  // Output trailing zeros if needed
  while (digsLeft > 0 && numBytes == 0) {
    string += '0';
    digsLeft--;
  }
  
  // Output the decimal point
  string += '.';

  // Keep on outputting '0' until we've used up the negative exponent
  for (digsLeft++; digsLeft < 0; digsLeft++) string += '0';
    
  // Keep on outputting digits until end of value
  while (numDigs || numBytes > 0) {
  
    // If no more digits, get another byte
    if (numDigs <= 0) {
    
      // Get the digits
      curDigs = (Int)(*curByte++);
      numBytes--;
      
      // If no value left, we're finished
      if (curDigs < 0)
        break;

      // We've got two digits now
      numDigs = 2;
    }
    
    // Output the digits digits...    
    if (numDigs == 2)
      theDig = curDigs/10;
    else
      theDig = curDigs%10;
    numDigs--;
    
    // Output the digit
    string += Char((UInt)theDig + '0');
      
  }
  
  // Trim trailing '0's...
  while (string.Get(-1) == '0') {
    status = string.Remove(-1);
    if (status) goto SOMEERROR;
  }
  
  // Trim trailing '.'
  if (string.Get(-1) == '.') {
    status = string.Remove(-1);
    if (status) goto SOMEERROR;
  }
  
  //
  // Handle errors
  //
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  // If error... 
  if (status)
    string = "###";
  
  return status;

}
    
String Number::ToExpString() const {
  
  Status status;
  String theString;
  
  status = ToExpString(theString);
  if (status) theString = ErrorString(status);
  
  theString.Consumable();
  return theString;
  
}

Status Number::ToExpString(String &string) const {

  Status  status;           // Return status
  Byte   *curByte;          // Current 100's digit Byte
  Int     numBytes;         // Remaining 100's digits
  Int     curDigs;          // Current 100's digit
  Int     numDigs;          // Number of digits (base 10) left in this byte
  Byte    theDig;           // The current digit

  // Clear the output string
  status = string.Reset();
  if (status) goto SOMEERROR;
  
  // Special cases
  if (len == 0 && !u.pointer) {
    string = "NaN";
    goto OK;
  }
  else if (len == 1 && value[0] == 0) {
    string = "0";
    goto OK;
  }
  else if (isInf) {
    if (sign)
      string = "-Inf";
    else
      string = "Inf";
    goto OK;
  }
  
  // Get the Digits pointer and Length
  if (len > 0) {
    curByte  = (Byte *)&value[0];
    numBytes = len;
  }
  else {
    numBytes = ((Buffer *)u.pointer)->GetUsedBytes();
    curByte  = ((Buffer *)u.pointer)->GetBuf();
  }
  
  // Initialise string stuff
  numBytes = len;
  
  // If negative...
  if (sign)
    string += '-';

  // Output the first digit, the decimal point, and the second digit
  string += (char)((*curByte/10) + '0');
  string += '.';
  string += (char)((*curByte%10) + '0');
  numDigs = 0;
  curByte++;
  numBytes--;
  
  // Keep on outputting digits until end of value
  while (numDigs || numBytes > 0) {
  
    // If no more digits, get another byte
    if (numDigs <= 0) {
    
      // Get the digits
      curDigs = (Int)(*curByte++);
      numBytes--;
      
      // If no value left, we're finished
      if (curDigs < 0)
        break;

      // We've got two digits now
      numDigs = 2;
    }
    
    // Output the digits digits...    
    if (numDigs == 2)
      theDig = curDigs/10;
    else
      theDig = curDigs%10;
    numDigs--;
    
    // Output the digit
    string += Char((UInt)theDig + '0');
      
  }
  
  // Trim trailing '0's...
  while (string.Get(-1) == '0') {
    status = string.Remove(-1);
    if (status) goto SOMEERROR;
  }
  
  // Trim trailing '.'
  if (string.Get(-1) == '.') {
    status = string.Remove(-1);
    if (status) goto SOMEERROR;
  }
  
  // Output exponent
  string += 'e';
  if (eSign)
    string += '-';
  string += exp;
  
  //
  // Handle errors
  //
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  // If error... 
  if (status)
    string = "###";
  
  return status;

}

//----------------------------------------------------------------------
//
// Operations
//

Status Number::Negate() {

  sign = 1-sign;
  
  return Error::None;
  
}

//----------------------------------------------------------------------
//
// Query
//

Boolean Number::IsNaN() const {

  return (len == 0 && BEFE::IsNull(u.pointer));

}

Boolean Number::IsZero() const {

  return (len == 1 && value[0] == 0);
  
}

Boolean Number::IsNegative() const {

  return (sign == 1);
  
}

Int Number::Exponent() const {
  
  Int theExp;
  
  theExp = exp;
  if (eSign)
    theExp = -(Int)exp;
    
  return theExp;

}

Span Number::ExponentSpan() const {

  Span  span;
  Byte *buf;
  UInt  bufLen;
  UInt  numDigits; 
 
  // Special cases...
  if (isInf || IsNaN()) {
    span.idx1 = 0;
    span.idx2 = -1;
    goto GOTIT;
  }
  else if (IsZero()) {
    span.idx1 = 0;
    span.idx2 = -1;
    goto GOTIT;
  }
  
  // "Normal" cases
  span.idx1 = exp;
  if (eSign)
    span.idx1 = -exp;
  if (len == 0) {
    buf    = ((Buffer *)&u.pointer)->GetBuf();
    bufLen = ((Buffer *)&u.pointer)->GetUsedBytes();
  }
  else {
    buf    = (Byte *)&value[0];
    bufLen = len;
  }
  numDigits = len * 2;
  if (buf && bufLen && (buf[len-1]%10) == 0)
    numDigits--;
  span.idx2 = span.idx1 - numDigits;
  
  // Handle end conditions
  while (false) {
    GOTIT: break;
  }
  
  return span;
  
}

//----------------------------------------------------------------------
//
// Method: Hash - Compute Number Hash value
//
// Hash "firstShort" format...
//
//  ┌─┬─┬─┬─┬─┬─┬─┬─┼─┬─┬─┬─┬─┬─┬─┬─┐
//  │1 1 1 1 1 1 0 0│0 0 0 0 0 0 0 0│
//  │5 4 3 2 1 0 9 8│7 6 5 4 3 2 1 0│
//  └┬┴┬┴┬┴┬┴─┴┬┴┬┴─┼─┴─┴─┴─┴─┴─┴─┴┬┘
//   │ │ │ └─┬─┘ └────────┬────────┘
//   │ │ │   │            │
//   │ │ │   │            └─── exp
//   │ │ │   └──────────────── len
//   │ │ └──────────────────── isInf
//   │ └────────────────────── eSign
//   └──────────────────────── sign
//

UInt Number::Hash() const {
  
  Crc32   crc;
  UInt32  result;
  Byte   *buf;
  UInt    bufLen;
  UInt16  firstShort;
  //UInt32  firstShort;
 
  // Get Buffer and Length
  if (len == 0) {
    buf    = ((Buffer *)&u.pointer)->GetBuf();
    bufLen = ((Buffer *)&u.pointer)->GetUsedBytes();
  }
  else {
    buf    = (Byte *)&value[0];
    bufLen = len;
  }
  
  // Form first word...
  firstShort  = sign  << 15;
  firstShort |= eSign << 14;
  firstShort |= isInf << 13;
  firstShort |= len   << 10;
  firstShort |= exp;
  
  // Calculate the CRC
  crc.Update((Int32)firstShort);
  result = crc.Update(buf,bufLen);
  
  return result;
  
}
  
//----------------------------------------------------------------------
//
// Comparison
//

Int Number::Compare(const Number &that) const {

  Int   answer;
  Byte *thisBuf;
  UInt  thisBufLen;
  Byte *thatBuf;
  UInt  thatBufLen;
  
  // Special cases if signs different...
  if (sign != that.sign) {
    if (len == 0 && that.len == 0)
      answer = 0;
    else if (sign)
      answer = -1;
    else
      answer = 1;
    goto GOTIT;
  }
  
  // Sign is same, handle case of either one of them zero...
  if (len == 0 || that.len == 0) {
    if (len == 0 && that.len == 0)
      answer = 0;
    else if (len == 0)
      answer = -1;
    else
      answer = 1;
    goto GOTIT;
  }
  
  // Same sign, if different precision exponent signs it's easy...
  if (eSign != that.eSign) {
    if (eSign)
      answer = -1;
    else
      answer = 1;
    goto GOTIT;
  }
  
  // Same sign and same exponent signs, if different exponent it's easy...
  if (exp != that.exp) {
    if (exp < that.exp)
      answer = -1;
    else
      answer = 1;
    goto GOTIT;
  }
  
  // Same sign, same exponent signs, and same exponent, need to do a byte compare...
  if (len) {
    thisBufLen = len;
    thisBuf    = (Byte *)&value[0];
  }
  else {
    thisBufLen = ((Buffer *)&u.pointer)->GetUsedBytes();
    thisBuf    = ((Buffer *)&u.pointer)->GetBuf();
  }
  if (that.len) {
    thatBufLen = that.len;
    thatBuf    = (Byte *)&that.value[0];
  }
  else {
    thatBufLen = ((Buffer *)&u.pointer)->GetUsedBytes();
    thatBuf    = ((Buffer *)&u.pointer)->GetBuf();
  }
  answer = Strncmp(thisBuf, thatBuf, Min(thisBufLen,thatBufLen));
  if (answer == 0) {
    if (thisBufLen < thatBufLen)
      answer = -1;
    else if (thisBufLen > thatBufLen)
      answer = 1;
  }
  
  // Handle ending conditions
  while (false) {
    GOTIT: break;
  }
  
  // If both negative, negate the answer
  if (sign && that.sign)
    answer = -answer;
    
  return answer;
  
}

//----------------------------------------------------------------------
//
// Protected Methods
//

void Number::SetNaN(void) {

  Reset();
  
}

//----------------------------------------------------------------------
//
// For debugging purposes...
//

Strings Number::ToDebugStrings() {
  
  Strings  out;
  Byte    *buf;
  UInt     bufLen;
  String   str;
  
  if (len == 0) {
    buf    = ((Buffer *)&u.pointer)->GetBuf();  
    bufLen = ((Buffer *)&u.pointer)->GetUsedBytes();
  }
  else {
    buf    = (Byte *)&value[0];
    bufLen = len;
  }

  out.Append(String("Sign:     '") + ((sign) ? '-':'+'        ) + '\'' );
  out.Append(String("eSign:    '") + ((eSign)? '-':'+'        ) + '\'' );    
  out.Append(String("Infinity: ")  + ((isInf)? "True":"False" )        );
  out.Append(String("Exponent: ")+(UInt)exp);
  
  str = "Digits:   ";  
  if (bufLen) {
    while (bufLen) {
      str += Char( ((UInt)*buf)/10+'0' );
      str += Char( ((UInt)*buf)%10+'0' );
      buf++;
      bufLen--;
      if (bufLen)
        str += ' ';
    }
  }
  else 
    str += "**None**";
  out.Append(str.Consumable());
  
  return out.Consumable();
  
}
  
} // ...Namespace BEFE