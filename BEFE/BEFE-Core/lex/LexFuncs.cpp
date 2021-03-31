//----------------------------------------------------------------------
// File: LexFuncs.cpp - Various Lex related Functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Lex.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Functions: LexIsAlphabetic
//            LexIsSpecial
//            LexIsControl
//            LexIsWhiteSpace
//            LexIsQuote
//            LexIsBase2
//            LexIsBase8
//            LexIsBase10
//            LexIsBase16
//

Boolean LexIsAlphabetic(Char c) {
  return c == '_' ||
         (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z');
}

Boolean LexIsSpecial(Char c) {
  return !LexIsControl(c) &&
         !LexIsAlphabetic(c) &&
         !LexIsBase10(c) &&
         IsASCII(c);
}

Boolean LexIsControl(Char c) {
  return c < ' ';
}

Boolean LexIsWhiteSpace(Char c) {
  return (c <= ' ' && c != NUL) || c == 0x7fu;
}

Boolean LexIsQuote(Char c) {
  return c == '\'' || c == '"' || c == '`';
}

Boolean LexIsBase2(Char c) {
  return c == '0' || c == '1';
}

Boolean LexIsBase8(Char c) {
  return c >= '0' && c <= '7';
}

Boolean LexIsBase10(Char c) {
  return c >= '0' && c <= '9';
}

Boolean LexIsBase16(Char c) {
  return (c >= '0' && c <= '9') ||
         (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

//----------------------------------------------------------------------
//
// Functions: LexCharMessage   - Format a Lex ${char} formatted error message
//            LexStringMessage - Format a Lex ${string} formatted error message
//

String LexCharMessage(Status pStatus, Char c) { // Function LexCharMessage...
  
  String       cStr;
  String       msg;
  NamedStrings dict;
  
  msg = ErrorString(pStatus);
  cStr = String(c);
  if (!gConsoleUTF8 && !c.IsASCII())
    cStr = String("\\x")+ToHexString((Int)c);
  dict.Set("char", cStr);
  Substitute(msg, dict, "${", "}", msg);
  
  return msg.Consumable();
  
} // ...Function LexCharMessage
  
String LexStringMessage(Status pStatus, String const &s) { // Function LexStringMessage...
  
  String       cStr;
  String       msg;
  NamedStrings dict;
  
  msg = ErrorString(pStatus);
  dict.Set("string", String(s).Consumable());
  Substitute(msg, dict, "${", "}", msg);
  
  return msg.Consumable();
  
} // ...Function LexStringMessage

} // ... Namespace BEFE
