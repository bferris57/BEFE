//----------------------------------------------------------------------
// File: StringSubscript.cpp - String Class C++ operator[] overload methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"

namespace BEFE { // Namespace BEFE...

Char String::operator[] (Int index) const {

  Status status;
  Char   c;

  status = Get(index, c);
  if (status)
    c = Char();

  return c;

}

String String::operator[] (Range const &range) const {

  Status status;
  String s;

  status = Get(range, s);
  if (status)
    s.Reset();

  return s.Consumable();

}

String String::operator[] (Span const &span) const {

  Status status;
  String s;

  status = Get(span, s);
  if (status)
    s.Reset();

  return s.Consumable();

}

String String::operator[] (Slice const &slice) const {

  Status status;
  String s;

  status = Get(slice, s);
  if (status)
    s.Reset();

  return s.Consumable();

}

} // ...Namespace BEFE