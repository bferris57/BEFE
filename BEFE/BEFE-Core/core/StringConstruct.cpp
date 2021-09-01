//----------------------------------------------------------------------
// File: StringConstruct.cpp - String Class C++ constructor methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Constructors and destructors
//

String::String() {
  StartUp();
}

String::~String() {
  ShutDown();
}

String::String(Byte *buf, UInt size) {
  StartUp();
  _BuildFrom(buf,size);
}

String::String(Char that) {
  StartUp();
  _BuildFrom(that);
}

String::String(String const &that) {
  StartUp();
  _BuildFrom(that);
}

String::String(char const *that) {
  StartUp();
  _BuildFrom(that);
}

String::String(char const *that, UInt len) {
  StartUp();
  _BuildFrom(that,len);
}

String::String(char *that) {

  UInt size;

  StartUp();
  if (!BEFE::IsNull(that))
    size = Strlen(that);
  else
    size = 0;
  _BuildFrom((Byte *)that,size);

}

String::String(int val) {

  Byte buf[20];
  StartUp();
  if ( IsNaN(val) )
    _BuildFrom(CONSTR_NaN);
  else {
    Itoa(val,buf,10);
    _BuildFrom(buf,Strlen(buf));
  }
}

String::String(char val) {
  StartUp();
  _BuildFrom((Byte *)&val,(UInt)1);
}

} // ...Namespace BEFE
