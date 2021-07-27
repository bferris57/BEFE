//----------------------------------------------------------------------
// File: LexFormat.h - Declarations for the BEFE LexFormat Classes
//----------------------------------------------------------------------
//
// This file contains the declarations for the LexFormat Classes.
//
// LexFormatOut is responsible for output formatting of various Lexical
// Analysis Classes, while LexFormatIn is responsible for input
// formatting.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LEXFORMAT_H // LEXWORKSPACE_H...
#define LEXFORMAT_H

#include "Primitives.h"
#include "LexWorkspace.h"
#include "LexLanguage.h"
#include "LexToken.h"
#include "LexStream.h"
#include "Bmsg.h"
//#include "MatchNode.h"

namespace BEFE { // Namespace BEFE...

struct LexFormatOut { // Struct LexFormatOut...

  // Members
  Boolean isConsumable:1;
  UInt    indentCols;       // Number of columns to indent
  UInt    linesBefore;      // Number of leading blank lines to emit
  UInt    linesAfter;       // Number of trailing blank linesto emit
  
  // C++ Lifecycle
  LexFormatOut();
  LexFormatOut(LexFormatOut const &that);
	~LexFormatOut();
  LexFormatOut &operator=(LexFormatOut const &that);

  // BEFE Lifecycle
  Status        StartUp();
  Status        ShutDown();
  Status        Clear();
  Status        Reset();
  Boolean       IsNull() const;
  Status        SetNull();
  Boolean       IsEmpty();
  Status        SetEmpty();
  Status        MoveFrom(LexFormatOut const &that);
  Status        CopyFrom(LexFormatOut const &that);
	Boolean       IsConsumable() const;
  LexFormatOut &Consumable() const;
  LexFormatOut &NotConsumable() const;
  
  // Output settings
  Status SetIndent(UInt numCols);

  // Output utilities
  Status EmitLinesBefore();
  Status EmitLinesAfter();
  
  // Cache Formatting
  Status DisplayCache();
  Status DisplayBlock(LexWorkspace &ws, UInt blockNo);

  // LexWorkspace formatting
  Status Display(LexWorkspace &ws);
  Status Display(LexWsArrayHead &ah);
  
  // LexLanguage formatting
  Status DisplayLanguage(LexLanguage &lang);
  Status DisplayState(LexLanguage &lang, UInt stateNo);
  Status DisplayAction(LexLanguage &lang, UInt actionNo);
  Status DisplayToken(UInt tokenNo, LexToken &token);
  Status DisplayToken(UInt tokenNo, BmsgToken &token);
  Status DisplayLine(BmsgLine &line);
  String ToString(LexToken &token);
  
  Strings ToStrings(LexStream &stream);

}; // ...Struct LexFormatOut

} // ...Namespace BEFE

#endif // ...LEXFORMAT_H
