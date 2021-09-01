//----------------------------------------------------------------------
// File: Char.cpp - Char Class base methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Char.h"
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Constructors
//

Char::Char(String const &that) {
  
  Status  status;
  Byte   *buf;
  UInt    bufSize;
  UInt    charLen;

  that._BufAndSize(buf,bufSize);  
  status = UTF8::DecodeChar(buf, *this, charLen);
  if (status) goto BADVALUE;
  
  // Handle errors...
  while (false) {
    BADVALUE: value = BadValue; break;
  }
  
}
  
//----------------------------------------------------------------------
//
// UTF-8 methods...
//

UInt Char::ByteLength() const {
  if (value <= 0x7fu)
    return 1;
  else
    return UTF8::GetByteSize(*(Char *)this);
}

UInt Char::CharLength(Byte *b) const {
  if (BEFE::IsNull(b))
    return 0;
  if (*b <= 0x7Fu)
    return 1;
  return UTF8::GetCharSize(*b);
}
  
//----------------------------------------------------------------------
//
// Bicameral manipulation...
//
// See technical documentation for further discussion of why we use
// that phrase!!!
//
// Note: These are for ASCII Chars only.  Use the UTF8 Folding and
//       Unfolding if you want "proper"
//
 
Char ToLower(Char c) {
  if (c.value >= 'A' && c.value <= 'Z')
    c = c - 'A' + 'a';
  return c;
}

Char ToUpper(Char c) {
  if (c.value >= 'a' && c.value <= 'a')
    c = c - 'a' + 'A';
  return c;
}

Char Lowercase(Char c) {
  if (c.value >= 'A' && c.value <= 'Z')
    c = c - 'A' + 'a';
  return c;
}

Char Uppercase(Char c) {
  if (c.value >= 'a' && c.value <= 'a')
    c = c - 'a' + 'A';
  return c;
}

Boolean IsLowercase(Char c) {
  return ToLower(c) == c;
}

Boolean IsUppercase(Char c) {
  return ToUpper(c) == c;
}

//----------------------------------------------------------------------
//
// Validation
//

Boolean IsASCII(Char c) {
  return c.value <= 0x7f;
}

Boolean IsUTF8(Char c) {
  return c.value >= Char::MinValue && c.value <= Char::MaxValue &&
         (c.value < 0xD800 || c.value > 0xDFFF);
}

Boolean IsUnicode(Char c) {
  return c.value >= Char::MinValue && c.value <= Char::MaxValue &&
         (c.value < 0xD800 || c.value > 0xDFFF);
}

Boolean IsCombining(Char c) {
  if ( (c.value >= 0x0300 && c.value <= 0x036f) ||
       (c.value >= 0x1DC0 && c.value <= 0x1DFF) ||
       (c.value >= 0x20D0 && c.value <= 0x20FF) ||
       (c.value >= 0xFE20 && c.value <= 0xFE2F))
    return true;
  return false;
}

//----------------------------------------------------------------------
//
// String Joining
//

Status Char::Split(String const &inString, Strings &outStrings) const {

  Status status;

  status = inString.SplitToStrings(*this, outStrings);

  return status;

}

Strings Char::Split(String const &inString) const {

  //Status  status;
  Strings outStrings;
  
  /*status =*/ inString.SplitToStrings(*this, outStrings);
  
  return outStrings.Consumable();

}

String Char::Join(Strings const &strings) const {

  //Status status;
  String outString;

  /*status =*/ strings.Join(String(*this), outString);

  return outString.Consumable();

}
  
Status Char::Join(Strings const &strings, String &outString) const {

  Status status;

  status = strings.Join(String(*this), outString);

  return status;

}

} // ...Namespace BEFE
