//----------------------------------------------------------------------
// File: OraNumber.cpp - OraNumber Class implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "OraNumber.h"
#include "ToString.h"

namespace BEFE { // Namespace BEFE...
  
//
// Handy defines
//

#define MAXMANT     20  // Maximum mantissa bytes 
#define MAXDECEXP   124 // Maximum decimal exponent
#define MINDECEXP  -130 // Minimum decimal exponent
#define MAXCENTEXP  62  // Maximum 100's exponent
#define MINCENTEXP -62  // Minimum 100's exponent

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

OraNumber::OraNumber() {

  StartUp();

}

OraNumber::OraNumber(char const *cstr) {

  String str;

  StartUp();
  str = cstr;
  FromString(str);
  
}

OraNumber::OraNumber(String const &str) {

  StartUp();
  FromString(str);
  
}

OraNumber::OraNumber(OraNumber const &that) {
  StartUp();
  CopyFrom(that);
}

OraNumber::~OraNumber() {
  ShutDown();
}

OraNumber &OraNumber::operator = (OraNumber const &that) {

  CopyFrom(that);
  
  return *this;
  
}

//----------------------------------------------------------------------
//
// Befe Lifecycle
//

Status OraNumber::StartUp() {
  value[0] = 0;
  return Error::None;
}

Status OraNumber::ShutDown() {
  return Error::None;
}

Status OraNumber::Reset() {
  return StartUp();
}

Boolean OraNumber::IsNull() const {
  return value[0] == 0;
}

Status OraNumber::SetNull() {
  return Reset();
}

Status OraNumber::MoveFrom(OraNumber const &that) {
  Status status;
  status = CopyFrom(that);
  if (!status)
    status = ((OraNumber *)&that)->Reset();
  return status;
}

Status OraNumber::CopyFrom(OraNumber const &that) {
  Strcpy(&value[0], &that.value[0]);
  return Error::None;
}

UInt OraNumber::Length() const {
  UInt len;
  len = Strlen(value);
  if (len) {
    if (IsNegative())
      len--;
    len = (len-1)*2;
  }
  return len;
}

UInt OraNumber::Size() const {
  return Strlen(value)+1;
}
  
//----------------------------------------------------------------------
//
// Method: OraNumber.FromString - Convert string to a Number
//
// This function attempts to convert a string to an OraNumber.
//
// Note:  f an empty string is passed, the Number will be set to NaN.
//

Status OraNumber::FromString(String const &string) {

  Status  status; // Return status
  Byte   *srcBuf; // Source string buffer
  UInt    srcLen; // Source string length (in bytes)
  Byte   *cp;     // Current character in the string
  UInt    msign;  // The mantissa sign (0 = '+', 1 = '-')
  UInt    mlen;   // Current mantissa len (bytes)
  UInt    mdigs;  // Current mantissa len (digits)
  Int     exp;    // The exponent
  Int     esign;  // The exponent sign
  Int     sexp;   // Specified exponent
  Byte    vbyte;  // Current value byte
  Byte    vdigs;  // Number if digits in current value byte
  Int     i;      // Temp: Loop counter
  Int     carry;  // Temp: Digit to carry to the right
  Int     ncarry; // Temp: Next digit to carry to the right
  Boolean havedot;// "Did we hit a '.'?"
  
  // Initialise stuff
  msign  = 0;
  mlen   = 0;
  mdigs  = 0;
  exp    = 0;
  esign  = 0;
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
    msign = 1;
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
  havedot = 0;
  
  //
  // Main Digit Processing (each digit until we hit '.' or 'E' or end of string)...
  //
  
  for (;srcLen ; srcLen--, cp++) { // For each digit...
  
    // If we hit a 'E', finished
    if (*cp == 'E' || *cp == 'e')
      break;
      
    // If we hit a '.'...
    if (*cp == '.') {
      if (havedot) goto BADNUMBER;
      havedot = 1;
      continue;
    }
      
    // If the byte isn't 0-9, error
    if (*cp < '0' || *cp > '9') goto BADNUMBER;      
  
    // Make sure mantissa isn't too long...
    if (mlen >= MAXMANT) goto TOOMANYMDIGS;
    
    // Flush current mantissa byte if we've got 2 digits...
    if (vdigs >= 2) {
    
      // Flush the byte
      value[mlen + 1] = vbyte + 1;
      
      // Adjust counters
      vdigs = 0;
      vbyte = 0;
      mdigs++;
      mlen++;      
    }
    
    // If we haven't hit a '.' yet
    if (!havedot)
      
      // Increase exponent
      exp++;
      
    // Else, we have hit a dot...
    else {
    
      // If we don't have a digit yet and this one is '0', decrease exp
      if (mdigs == 0 && *cp == '0')
        exp--;
    }
    
    // If it's zero and it's leading and haven't hit first digit yet, ignore it
    if (*cp == '0' && mdigs == 0)
      continue;
      
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
  esign = 1;
  sexp  = 0;
  
  // If '+' or '-'...
  if (*cp == '+' || *cp == '-') {
  
    // If '-', set the sign
    if (*cp == '-')
      esign = -1;
      
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
  
  // Set its sign
  sexp = sexp * esign;
  
  // Add it into current exponent
  exp = exp + sexp;
  
  //
  // Tidy up...
  //
  
TIDYUP:

  // Handle zero...
  if (mdigs == 0) {
    value[0] = 0x80;
    value[1] = 0;
    goto OK;
  }
  
  // If mantissa bytes...
  if (vdigs) {
  
    // If odd mantissa bytes, adjust
    if (vdigs % 2)
      vbyte *= 10;
  
    // Flush the byte
    value[mlen + 1] = vbyte + 1;
    
    // Adjust counters
    mlen++;

  }

  // Output trailing NULL
  value[mlen+1] = 0;

  // Make sure the exponent isn't too big/small
  if (exp > MAXDECEXP || exp < MINDECEXP)
    goto BADEXPONENT;
    
  // At this point the exponent is base 10, we now want to conver to
  // base 100.  So, if it's even it's easy.  If it's odd we need to
  // shift all digits to the RIGHT one digit...
  
  // If even...
  if (!(exp & 1)) {
  
    // Adjust exponent
    exp >>= 1;
    
  }
  
  // else, it's odd...
  else {
  
    // Initialise
    carry = 0;
    
    // For each byte
    for (cp = &value[1]; *cp; cp++) {
    
      // Compute next carry
      ncarry = (*cp - 1) % 10;
      
      // Adjust the byte
      *cp = (*cp - 1) / 10 + (carry * 10) + 1;
      
      // Save next carry
      carry = ncarry;
    }
    
    // If we have carry left over...
    if (ncarry) {
    
      // Make sure it's not too big
      if ( mlen >= MAXMANT - 1)
        goto TOOMANYMDIGS;
        
      // Store remaining carry and NUL terminate
      *cp++ = ncarry * 10 + 1;
      *cp = 0;
      
      // Remind ourselves of mantissa length (in bytes)
      mlen++;
    }
    
    // Adjust exponent
    exp = (exp + 1) >> 1;
    
  }
  
  // Adjust exponent
  exp = 64 + exp;
  
  // Flush exponent
  value[0] = 0x80 + (exp & 0x7f);

  // Remove tailing insignificant zeros...
  for (cp = &value[mlen]; cp >= &value[1] && *cp == 0x01; cp--)
    *cp = 0;

  // If negative, negate it...
  if (msign) {  
    status = Negate();
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
// Method: OraNumber.ToString - Convert OraNumber to a string
//
// This function attempts to convert a OraNumber into a string.
//
// Notes: If an empty string is passed, the OraNumber will be set to NULL.
//

String OraNumber::ToString() const {
  
  Status status;
  String theString;
  
  status = ToString(theString);
  if (status) theString = ErrorString(status);
  
  return theString.Consumable();
  
}

Status OraNumber::ToString(String &string) const {

  Status     status;        // Return status
  OraNumber  that;          // Copy of the number (so we can mess with it)
  Byte      *curdig;        // Current digit index
  Int        curdigs;       // Current digits (-1 = End of number)
  Int        digsleft;      // Digits left until decimal point
  Int        numdigs;       // Number of digits (base 10) left in this byte
  Int        nonzero;       // Number of non-zero digits we've output
  Byte       thedig;        // The current digit

  // Clear the output string
  status = string.Reset();
  if (status) goto SOMEERROR;
  
  // Get a copy of the number
  that = *this;
  
  // Special case if zero...
  if (that.IsZero()) {
    string += '0';
    goto OK;
  }
  
  // Initialise string stuff
  nonzero = 0;
  curdig  = &that.value[1];
    
  // If negative...
  if ((that.value[0] & 0x80) == 0) {
  
    // Negate it
    that.Negate();
    
    // Output '-'
    string += '-';
  }

  // Figure out how many decimal places we have left until '.'
  digsleft = ((that.value[0] & 0x7f) - 64) << 1;
  
  // Until we've exhausted the decimal point...
  for (numdigs = 0; digsleft > 0; digsleft--, numdigs--) { // For each integer digit...
  
    // If no more digits, get another byte
    if (numdigs <= 0) {
    
      // Get the digits
      curdigs = (int)(*curdig++) - 1;
      
      // We've got two digits now
      numdigs = 2;
    }
    
    // If no more digits...
    if (curdigs < 0)
    
      // The digit it '0'
      thedig = 0;
      
    // Else, we have more digits...
    else {
    
      if (numdigs == 2)
        thedig = curdigs/10;
      else
        thedig = curdigs%10;
    }
        
    // If digit is non-zero OR we've already put non-zero digits out...
    if (thedig || nonzero) {
    
      // Output the digit
      string += Char((UInt)thedig + '0');
      
      // We've now put a digit out...
      nonzero++;
    }

  } // For each integer digit
  
  // Output the decimal point
  string += '.';

  // Keep on outputting '0' until we've used up the negative exponent
  for (; digsleft < 0; digsleft++) string += '0';
    
  // Keep on outputting digits until end of value
  for (;curdigs >= 0; numdigs--) {
  
    // If no more digits, get another byte
    if (numdigs <= 0) {
    
      // Get the digits
      curdigs = (int)(*curdig++) - 1;
      
      // If no value left, we're finished
      if (curdigs < 0)
        break;

      // We've got two digits now
      numdigs = 2;
    }
    
    // Output the digits digits...    
    if (numdigs == 2)
      thedig = curdigs/10;
    else
      thedig = curdigs%10;
        
    // Output the digit
    string += Char((UInt)thedig + '0');
      
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
  
//----------------------------------------------------------------------
//
// Method: OraNumber.Negate - Negate an OraNumber
//

Status OraNumber::Negate() {

  Status  status;  // Return status
  Byte   *cp;      // Current position in mantissa
  Int     wasneg;  // "Was it negative to start with?"
  Int     mlen;    // Mantissa length (in bytes)
  Int     exp;     // Current exponent
    
  // If NULL or zero, nothing to do
  if (IsNull() || IsZero()) goto OK;

  // Get number of mantissa bytes
  mlen = Strlen(&value[0]);
  
  // If it's negative...
  if (!(value[0] & 0x80)) {
  
    wasneg = 1;
    
    // Make sure last byte's 0x66 and NUL terminate there
    if (mlen <= 0 || value[mlen-1] != 0x66)
      goto BADNUMBER;
  
    // Discard the 0x66
    value[mlen-1] = 0;    
  }
  
  // It wasn't negative...
  else {
  
    wasneg = 0;
    
    // Make sure we have enough room to put 0x66 at end
    if (mlen >= MAXMANT)
      goto TOOMANYDIGS;
  }
  
  // Negate the sign
  value[0] ^= 0x80;
  
  // Negate the exponent
  exp = (value[0] & 0x7f) - 64;
  value[0] = (value[0] & 0x80) | ( (63 - exp) & 0x7f);
  
  // For each mantissa byte...
  for (cp = &value[1]; *cp; cp++) {
  
    // Negate the mantissa byte
    *cp = 102 - *cp;
    
    // Make sure the mantissa byte is valid
    if (*cp > 101) goto BADNUMBER;
    
  }
  
  // If it wasn't negative...
  if (!wasneg) {
  
    // Put 0x66 at end and NUL terminate
    *cp++ = 0x66;
    *cp = 0;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    BADNUMBER:   status = Error::NumberInvalid;     break;
    TOOMANYDIGS: status = Error::NumberBadMantissa; break;
    OK:          status = Error::None;              break;
  }
  
  return status;
}

//----------------------------------------------------------------------
//
// Method: OraNumber.IsZero - "Is this number zero?"
//

Boolean OraNumber::IsZero() const {

  if (value[0] == 0x80 && value[1] == 0)
    return true;
    
  return false;
  
}

//----------------------------------------------------------------------
//
// Method: OraNumber.IsNegative - "Is this number less than zero?"
//

Boolean OraNumber::IsNegative() const {

  Boolean answer;
  
  answer = (!IsNull() && !IsZero() && !(value[0] & 0x80));
  
  return answer;

}

//----------------------------------------------------------------------
//
// Method: OraNumber.Compare - Compare with another number
//
// This method compares two numbers and returns an Int indicating the
// following...
//
//   -1 => this < that
//    0 => this = that
//    1 => This > that
//

Int OraNumber::Compare(const OraNumber &that) {

  return Strcmp(&value[0], &that.value[0]);

}

//----------------------------------------------------------------------
//
// Method: OraNumber.ToHex - Convert OraNumber to Hex String
//
// Notes:  If the OraNumber is NaN, the string "NaN" is populated.
//
//         *** This is a Utility Method and only useful for debugging
//         *** purposes.
//

String OraNumber::ToHex() const {

  Byte   *cp;               // Current position in the value
  Byte    theByte;          // The current value Byte
  String  string;           // The output string
  Byte    outChar;
  
  // If it's NULL, it's easy...
  if (IsNull()) {
    string = CONSTR_NaN;
    goto OK;
  }
  
  // Output each Byte into the string...
  for (cp = (Byte *)&value[0]; *cp; cp++) {
  
    // Get the byte
    theByte = *cp;
    
    // Output 1st nibble in hex
    outChar = ((theByte >> 4) & 0x0f) + '0';
    if (outChar > '9') outChar += 'a' - '9' - 1;
    string += Char(outChar);
    
    // Output 2nd nibble in hex
    outChar = (theByte & 0x0f) + '0';
    if (outChar > '9') outChar += 'a' - '9' - 1;
    string += Char(outChar);

  }

  while (false) {
    OK: break;
  }
  
  return string.Consumable();
  
}

//----------------------------------------------------------------------
//
// Method: OraNumber.ToDebug - Convert OraNumber to Debug String
//
// Notes:  If the OraNumber is NaN, the string "NaN" is populated.
//
//         *** This is a Utility Method and only useful for debugging
//         *** purposes.
//

String OraNumber::ToDebug() const {

  Byte   *cp;               // Current position in the value
  Int     theExp;           // The Exponent
  Byte    theByte;          // The current value byte
  String  string;           // Output string
  
  // If it's NULL, it's easy...
  if (IsNull()) {
    string = CONSTR_NaN;
    goto OK;
  }

  // If it's Zero, it's easy...
  if (IsZero()) {
    string = '0';
    goto OK;
  }
  
  // Initialise value pointer and buffer
  cp = (Byte *)&value[0];
  
  // Output the sign
  string = String("Sign: '") + ((*cp & 0x80)?'+':'-') + "' ";
  
  // Compute the exponent
  theExp = (*cp & 0x7f) - 64;
  if (! (*cp & 0x80))
    theExp = -(theExp+1);
      
  // Output the exponent
  string += String("Exp: ") + BEFE::ToString(theExp*2) + ' ';
  
  // Output digits
  string += " Digits: .";

  for (++cp; *cp; cp++) {

    // Get the byte
    theByte = *cp - 1;
    
    // If 0x66, we're finished
    if (theByte == (0x66 - 1))
      break;
    
    // Negate if negative
    if (!(value[0] & 0x80))
      theByte = 100 - theByte;
      
    // Output 10's digit
    string += Char((UInt)theByte / 10 + '0');
    
    // Output 1's digit
    string += Char(((UInt)theByte % 10) + '0');
    
  }

  while (false) {
    OK: break;
  }
  
  return string.Consumable();
  
}

//----------------------------------------------------------------------
//
// Method: OraNumber.SetNull - Set OraNumber to Null
//
// DESCRIPTION
//
//   This function sets the Number instance to Null/NaN.
//

void OraNumber::SetNaN(void) {

  value[0] = 0;
  
}

} // ...Namespace BEFE