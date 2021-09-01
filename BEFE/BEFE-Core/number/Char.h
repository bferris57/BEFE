//!befe-publish inc
//----------------------------------------------------------------------
// File: Char.h - Char Struct declarations
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CHAR_H // CHAR_H...
#define CHAR_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

// Forward Struct/Class declarations
struct Char;
class  Strings;
class  String;

//----------------------------------------------------------------------
//
// Char based functions
//

Char    ToLower(Char that);     // ◄── Convert to lowercase (Unicode folding)
Char    ToUpper(Char that);     // ◄── Convert to uppercase (reverse Unicode folding)
Boolean IsASCII(Char that);     // ◄── "Is this Char ASCII?"
Boolean IsUTF8(Char that);      // ◄── "Is this Char a Unicode character?" (0x000000..0x10FFFF)
Boolean IsUnicode(Char that);   // ◄── "Is this Char a Unicode character?" (0x000000..0x10FFFF)
Boolean IsCombining(Char that); // ◄── "Is this Char a Unicode Combining character?"

//----------------------------------------------------------------------
//
// Struct: Char - A single Unicode character
//

struct Char { // Char Struct...

  UInt value;

  // Limits...

  enum : UInt {
     MinValue = 0
    ,MaxValue = 0x10ffff
    ,BadValue = 0xffffffff
    };

  // Constructors...
  BEFE_inline Char()                   {value=BadValue;};
  BEFE_inline Char(UInt a)             {value=a;          _Validate();};
  BEFE_inline Char(UShort s)           {value=s;          _Validate();};
  BEFE_inline Char(char a)             {value=a;};
  BEFE_inline Char(Byte b)             {value=b;};
  BEFE_inline Char(Char const &that)   {value=that.value; _Validate();};
              Char(String const &that);
  // Typecasting...
  BEFE_inline operator UInt()          {return value;};
  
  // BEFE Lifecycle
  BEFE_inline Boolean IsNull() const   {return (value == UNaN);}
  BEFE_inline Status  SetNull()        {value = UNaN; return Error::None;}
  
  // UTF-8 methods...
  UInt    ByteLength() const;
  UInt    CharLength(Byte *b) const;
  
  // Assignment operators...
  BEFE_inline Char &operator = (Char c) {value  = c.value; return *this;}
  BEFE_inline Char &operator = (char c) {value  = c; return *this;}
  BEFE_inline Char &operator = (UInt i) {value  = i; return *this;}
              Char &operator = (char const *that);
  BEFE_inline Char &operator-= (char c) {value -= c; return *this;}
  BEFE_inline Char &operator-= (UInt i) {value -= i; return *this;}
  BEFE_inline Char &operator+= (char c) {value += c; return *this;}
  BEFE_inline Char &operator+= (UInt i) {value += i; return *this;}

  // Char Arithmatic operators..
  BEFE_inline Char  operator+ (Char that) {
    Char t;
    t.value = this->value;
    t.value += that.value;
    return t;
  }
  BEFE_inline Char  operator- (Char that) {
    Char t;
    t.value = this->value;
    t.value -= that.value;
    return t;
  }

  // char Arithmetic operators..
  BEFE_inline Char  operator+ (char that) {Char t; t.value = this->value; t.value += that; return t;}
  BEFE_inline Char  operator- (char that) {Char t; t.value = this->value; t.value -= that; return t;}

  // Char Comparative operators...
  BEFE_inline Boolean operator<  (Char that) {return value <  that.value;}
  BEFE_inline Boolean operator<= (Char that) {return value <= that.value;}
  BEFE_inline Boolean operator== (Char that) {return value == that.value;}
  BEFE_inline Boolean operator!= (Char that) {return value != that.value;}
  BEFE_inline Boolean operator>  (Char that) {return value >  that.value;}
  BEFE_inline Boolean operator>= (Char that) {return value >= that.value;}

  // char Comparative operators...
  BEFE_inline Boolean operator<  (char that) {return value <  (UInt)that;}
  BEFE_inline Boolean operator<= (char that) {return value <= (UInt)that;}
  BEFE_inline Boolean operator== (char that) {return value == (UInt)that;}
  BEFE_inline Boolean operator!= (char that) {return value != (UInt)that;}
  BEFE_inline Boolean operator>  (char that) {return value >  (UInt)that;}
  BEFE_inline Boolean operator>= (char that) {return value >= (UInt)that;}

  // Int Comparative operators...
  BEFE_inline Boolean operator<  (UInt that) {return value <  that;}
  BEFE_inline Boolean operator<= (UInt that) {return value <= that;}
  BEFE_inline Boolean operator== (UInt that) {return value == that;}
  BEFE_inline Boolean operator!= (UInt that) {return value != that;}
  BEFE_inline Boolean operator>  (UInt that) {return value >  that;}
  BEFE_inline Boolean operator>= (UInt that) {return value >= that;}

  // Bitwise operators...
  BEFE_inline Char  operator& (Char that) {
    Char t;
    t.value &= this->value;
    return t;
  }

  // Bicameral manipulation...
  //
  // See technical documentation for further discussion of why we use
  // that phrase!!!
  //

  BEFE_inline Char    ToLower  () const   {return BEFE::ToLower(*this);};
  BEFE_inline Char    ToUpper  () const   {return BEFE::ToUpper(*this);};
  BEFE_inline Char    Lowercase() const   {return BEFE::ToLower(*this);};
  BEFE_inline Char    Uppercase() const   {return BEFE::ToUpper(*this);};
  //Boolean IsLowercase() const;
  //Boolean IsUppercase() const;
  
  // Silly UTF-8 BOM... Shouldn't even be there but it CAN occur, under
  // Microsoft at least, as the first UTF-8 Char in a text file...
  BEFE_inline Boolean IsBOM()     const   {return value==0xFFFE || value==0xFEFF;}
  
  // String joining and splitting...
  Status  Split(String const &inString, Strings &outStrings) const;
  Strings Split(String const &inString) const;
  Status  Join(Strings const &strings, String &outString) const;
  String  Join(Strings const &strings) const;
  
  // Validation
  BEFE_inline Boolean IsASCII()   const    {return BEFE::IsASCII(*this);}
  BEFE_inline Boolean IsUTF8()    const    {return BEFE::IsUTF8(*this);}
  BEFE_inline Boolean IsUnicode() const    {return BEFE::IsUnicode(*this);}
  BEFE_inline Boolean IsBadUnicode() const {return !IsASCII() && !IsUnicode();}
  
  // "Protected" methods (eg, we don't guarantee they'll always work this way)...
  BEFE_inline void _Validate()           {if (value > MaxValue || 
                                              (value >= 0xD800 && value <= 0xDFFF)
                                             ) value = UNaN;}
  BEFE_inline Boolean _IsValid()         {if (value > MaxValue || 
                                              value == 0xFEFF  ||
                                              value == 0xFFFE  ||
                                              (value >= 0xD800 && value <= 0xDFFF)
                                             ) 
                                            return false;
                                          return true;
                                         }

}; // ...Char Struct

//----------------------------------------------------------------------
//
// Char BEFE Lifecycle...
//

            Status   StartUp       (Char &rThis)                     ;
            Status   ShutDown      (Char &rThis)                     ;
            Status   Reset         (Char &rThis)                     ;
BEFE_INLINE Boolean  IsNull        (Char const &rThis)               {return rThis.IsNull();}
BEFE_INLINE Status   SetNull       (Char &rThis)                     {return rThis.SetNull();}
            Boolean  IsEmpty       (Char const &rThis)               ;
            Status   SetEmpty      (Char &rThis)                     ;
            Status   MoveFrom      (Char &rThis, UInt8 const &rThat) ;
            Status   CopyFrom      (Char &rThis, UInt8 const &rThat) ;
            Boolean  IsConsumable  (Char const &rThis)               ;
            Char    &Consumable    (Char &rThis)                     ;
            Char    &NotConsumable (Char &rThis)                     ;
            Boolean  IsMutable     (Char const &rThis)               ;
            Char    &Mutable       (Char &rThis)                     ;
            Char    &Immutable     (Char &rThis)                     ;
            UInt     Length        (Char const &rThis)               ;
            UInt     Size          (Char const &rThis)               ;

} // ...Namespace BEFE

#endif // ...CHAR_H
