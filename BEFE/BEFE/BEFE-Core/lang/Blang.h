//----------------------------------------------------------------------
// File: Blang.h - Declarations for the BEFE Languages Super-Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// The Blang class is simply a convenient class that contains common
// declarations and code for each different language built on the
// .befe lexical structure.
//
// These languages include...
//
//   .bcmd - BEFE Command Scripts
//   .bmsg - BEFE Error/Warning Messages
//   .btyp - BEFE Type Model Declarations
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BLANG_H // BLANG_H...
#define BLANG_H

#include "Primitives.h"
#include "Lex.h"
#include "LexToken.h"
#include "LexStream.h"
#include "LexLanguage.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class Blang - The .befe Languages Super-Class
//

class Blang { // Class Blang...

  //
  // Methods
  //
  
  // C++ Lifecycle
	public: BEFE_inline Blang()                             {StartUp();}
  public: BEFE_inline Blang(Blang const &that)            {StartUp(); CopyFrom(that);}
	public: BEFE_inline ~Blang()                            {ShutDown();}
  public: BEFE_inline Blang &operator=(Blang const &that) {CopyFrom(that); return *this;}
  
  // BEFE Lifecycle
  public: Status   StartUp();
  public: Status   ShutDown();
  public: Status   Reset();
  public: Status   MoveFrom(Blang const &that);
  public: Status   CopyFrom(Blang const &that);
  public: Boolean  IsNull() const;
  public: Status   SetNull();
  public: Boolean  IsEmpty() const;
  public: Status   SetEmpty();
  public: UInt     Length() const;
  public: UInt     Size() const;
	public: Boolean  IsConsumable() const;
  public: Blang    &Consumable() const;
  public: Blang    &NotConsumable() const;
  
  // Operational Methods
  public: Status   GetStatistics(UInt &inTokens,
                                 UInt &outTokens);
                                 
  // Low Level Under-the-cover Methods
  protected: Status  _StartParsing();
  protected: Status  _StopParsing();
  protected: Status  _GetLexTokens();
  
  //
  // Members
  //
  
  protected: Boolean       isConsumable:1;
  protected: Boolean       isStarted:1;    // "Parsing is started"
  protected: Boolean       isAtEnd:1;      // "Is at end of stream" (only applicable if isStarted==true)
  protected: Boolean       skipToEOL:1;    // "Skipping to end of line?" (only applicable if isStarted==true)
                                           //   Note: Is checked all the way down in _GetLexTokens
                                
  // Input specification...
  protected: String        inputFileName;
  protected: String        inputString;
  protected: String        inputPrompt;
  
  // Messages and MessagesRanges we're building...
  protected: UInt          totInTokens;    // ◄── Total input tokens processed so far
  protected: UInt          totOutTokens;   // ◄── Total output tokens delivered so far
  
  // Internal: Lex context and tie-ins...
  protected: Lex           lex;
  protected: LexLanguage   lang;
  protected: LexStream     stream;
  
  // Internal: LexToken "buffer"...
  protected: LexTokens     tokens;
  protected: LexToken     *token0;         // ◄── Denormalised pointer managed by _GetLexTokens
  protected: UInt          curTokNo;       // ◄── "Current Token" index (0:maxTokens)
  protected: UInt          lookBehind;     // ◄── Number of tokens to look behind
  protected: UInt          lookAhead;      // ◄── Number of tokens to look ahead
  protected: UInt          curTokens;      // ◄── Current number of LexTokens in tokens
  protected: UInt          maxTokens;      // ◄── Maximum number of LexTokens in tokens
  protected: UInt          curLineNo;      // ◄── Line number we're currently processing
  protected: UInt          curLineStart;   // ◄── File offset of current Line we're processing
  
}; // ...Class Blang

} // ...Namespace BEFE

#endif // ...BLANG_H