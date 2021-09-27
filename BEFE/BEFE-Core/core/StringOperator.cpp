//----------------------------------------------------------------------
// File: StringOperator.cpp - String Class C++ operator overload methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "UTF8.h"
#include "SBuffer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Operator = methods
//

String &String::operator = (String const &that) {
  if (this != &that) {
    Reset();
    if (that.IsConsumable()) {
      MoveFrom((String &)that);
      NotConsumable();
    }
    else
      _BuildFrom(that);
    mult = that.mult;
  }
  return *this;
}

String &String::operator = (char const *that) {
  Reset();
  _BuildFrom(that);
  return *this;
}

String &String::operator = (char *that) {

  UInt len;

  Reset();
  if (!BEFE::IsNull(that))
    len = Strlen(that);
  else
    len = 0;
  _BuildFrom((Byte *)that,len);
  return *this;
}

String &String::operator = (char that) {
  Reset();
  _BuildFrom(Char(that));
  return *this;
}

String &String::operator = (Char that) {
  Reset();
  _BuildFrom(that);
  return *this;
}

String &String::operator = (Int that) {
  Byte buf[20];
  Itoa(that,buf,10);
  Reset();
  _BuildFrom((const char *)buf);
  return *this;
}

String &String::operator = (UInt that) {
  Byte buf[20];
  Itoa(that,buf,10);
  Reset();
  _BuildFrom((const char *)buf);
  return *this;
}

//----------------------------------------------------------------------
//
// Operator += methods
//

String &String::operator += (String const &that) {
  Append(that);
  return *this;
}

String &String::operator += (Char that) {

  Byte utfbuf[UTF8::MaxLength+1];
  UInt bytelength;

  UTF8::EncodeChar(that,utfbuf,bytelength);
  utfbuf[bytelength] = 0;
  Append(utfbuf,bytelength);
  return *this;
}

String &String::operator += (char const *that) {
  Append(that);
  return *this;
}

String &String::operator += (char *that) {
  Int len;

  if (!BEFE::IsNull(that))
    len = Strlen(that);
  else
    len = 0;
  Append((Byte *)that,len);
  return *this;
}

String &String::operator += (char that) {
  Append((Byte *)&that,1);
  return *this;
}

String &String::operator += (Int that) {
  Byte buf[20];
  Itoa(that,buf,10);
  Append((char *)buf);
  return *this;
}

String &String::operator += (UInt that) {
  Byte buf[20];
  Itoa(that,buf,10);
  Append((char *)buf);
  return *this;
}

//----------------------------------------------------------------------
//
// Operator + methods
//

String String::operator + (String const &that) const {
  String local = *this;
  local += that;
  return local.Consumable();
}

String String::operator + (Char that) const {
  String local = *this;
  local += that;
  return local.Consumable();
}

String String::operator + (char const *that) const {
  String local = *this;
  local.Append(that);
  return local.Consumable();
}

String String::operator + (char *that) const {
  String local = *this;
  Int    len;
  if (!BEFE::IsNull(that))
    len = Strlen(that);
  else
    len = 0;
  local.Append((Byte *)that,len);
  return local.Consumable();
}

String String::operator + (char that) const {
  String local = *this;
  local.Append((Byte *)&that,1);
  return local.Consumable();
}

String String::operator + (Int that) const {
  String local = *this;
  Byte   buf[20];
  Itoa(that,buf,10);
  local.Append((const char *)buf);
  return local.Consumable();
}

String String::operator + (UInt that) const {
  String local = *this;
  Byte   buf[20];
  Itoa(that,buf,10);
  local.Append((const char *)buf);
  return local.Consumable();
}


//----------------------------------------------------------------------
//
// Operator * methods
//

String &String::operator *= (Int theMult) {
  if (BEFE::IsNull(theMult))
    mult = 1;
  else
    mult = Min(255, (UInt)Abs(theMult));
  return *this;
}

String &String::operator *= (size_t theMult) {
  if (BEFE::IsNull(theMult))
    mult = 1;
  else
    mult = Min(255, Abs(theMult));
  return *this;
}


String String::operator * (Int theMult) const {
  String local = *this;
  local *= theMult;
  return local.Consumable();
}

String &String::operator *= (UInt theMult) {
  if (BEFE::IsNull(theMult))
    mult = 1;
  else
    mult = Min(255, theMult);
  return *this;
}

String String::operator * (UInt mult) const {
  String local = *this;
  local *= mult;
  return local.Consumable();
}

//----------------------------------------------------------------------
//
// Operator << and <<= methods
//

String &String::operator << (char const* str) {
  Append(str);
  return *this;
}

String &String::operator << (char * str) {
  Int len;
  len = Strlen(str);
  Append((Byte *)str,len);
  return *this;
}


String &String::operator << (void *ptr) {

  String hex;

#ifdef IS64Bit
  hex = ToHexString((Ptr)ptr,16);
#else
  hex = ToHexString((Ptr)ptr,8);
#endif

  Append(hex);
  return *this;
}

String &String::operator << (bool  b) {
  if (b)
    Append("true");
  else
    Append("false");
  return *this;
}

String &String::operator << (char  c) {
  Append((Byte *)&c,1);
  return *this;
}

String &String::operator << (Int num) {
  Byte buf[12];
  if ( !BEFE::IsNull(num) ) {
    Itoa(num,buf,10);
    Append(buf,Strlen(buf));
  }
  else
    Append("NaN");
  return *this;
}

String &String::operator << (UInt num) {
  Byte buf[12];
  if ( !BEFE::IsNull(num) ) {
    Itoa(num,buf,10);
    Append(buf,Strlen(buf));
  }
  else
    Append("UNaN");
  return *this;
}

String &String::operator << (Char c) {
  Append(c);
  return *this;
}

String &String::operator << (String const &str) {
  Append(str);
  return *this;
}

//--------------

String &String::operator <<= (char const* str) {
  return *this << str;
}

String &String::operator <<= (char * str) {
  return *this << str;
}

String &String::operator <<= (void *ptr) {
  return *this << ptr;
}

String &String::operator <<= (bool  b) {
  return *this << b;
}

String &String::operator <<= (char  c) {
  return *this << c;
}

String &String::operator <<= (Int num) {
  return *this << num;
}

String &String::operator <<= (UInt num) {
  return *this << num;
}

String::operator bool() {
  return (Length() != 0);
}

String::operator int() {

  SBuffer *sbuf;
  Int      val;
  Byte     ourbuf[257];
  Byte    *srcbuf;
  UInt     srcbufl;

  val = NaN;
  switch (type) {
    case Null:   break;
    case Const:
      val = 0;
      if (Length() == 0 || BEFE::IsNull(data)) break;
      val = Atoi(data);
      break;
    case ConstLen:
      val = 0;
      if (Length() == 0 || BEFE::IsNull(data)) break;
      val = Atoi(data,constLen);
      break;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      srcbufl = type - ASCII0;
      if (data==NULL) break;
      MemoryCopyRaw((Byte *)&data,ourbuf,srcbufl);
      ourbuf[srcbufl] = 0;
      val = Atoi(ourbuf);
      break;
    case UtfInline:
      srcbufl = constLen;
      MemoryCopyRaw((Byte *)&data,ourbuf,srcbufl);
      ourbuf[srcbufl] = 0;
      val = Atoi(ourbuf);
      break;
      break;
    case Buffer:
      sbuf = (SBuffer *)&data;
      if (BEFE::IsNull(sbuf)) break;
      srcbuf = sbuf->GetBuf();
      if (BEFE::IsNull(srcbuf)) break;
      srcbufl = sbuf->GetUsedBytes();
      if (srcbufl <= 0) break;
      srcbufl = Min(srcbufl,sizeof(ourbuf)-1);
      MemoryCopyRaw(srcbuf,ourbuf,srcbufl);
      ourbuf[srcbufl] = 0;
      val = Atoi(ourbuf);
      break;
    default:
      val = NaN;
      break;
  }
  return val;
}

} // ...Namespace BEFE
