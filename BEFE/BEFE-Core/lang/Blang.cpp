//----------------------------------------------------------------------
// File: Blang.h - Implementation of the Blang Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Blang.h"
#include "LexLanguage.h"
#include "LexLang.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// BEFE Lifecycle...
//

Status Blang::StartUp() {

  Status retStatus;  
  Status status;
  
  isConsumable = false;
  isStarted    = false;
  isAtEnd      = false;
  skipToEOL    = false;

  retStatus = inputFileName.StartUp();
  status = inputString.StartUp();
  if (status && !retStatus) retStatus = status;
  status = inputPrompt.StartUp();
  if (status && !retStatus) retStatus = status;
  
  status = lex.StartUp();
  if (status && !retStatus) retStatus = status;
  status = lang.StartUp();
  if (status && !retStatus) retStatus = status;
  status = stream.StartUp();
  if (status && !retStatus) retStatus = status;

  status = tokens.StartUp();
  if (status && !retStatus) retStatus = status;
  token0 = NULL;

  BEFE::SetNull(curTokNo);
  BEFE::SetNull(lookBehind);
  BEFE::SetNull(lookAhead);
  BEFE::SetNull(curTokens);
  BEFE::SetNull(maxTokens);
  BEFE::SetNull(curLineNo);
  BEFE::SetNull(curLineStart);

  BEFE::SetNull(totInTokens);
  BEFE::SetNull(totOutTokens);
  
  return retStatus;
  
}

Status Blang::ShutDown() {

  Status retStatus;  
  Status status;
  
  retStatus = inputFileName.ShutDown();
  status = inputString.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = inputPrompt.ShutDown();
  if (status && !retStatus) retStatus = status;
  
  status = lex.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = lang.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = stream.ShutDown();
  if (status && !retStatus) retStatus = status;

  status = tokens.ShutDown();
  if (status && !retStatus) retStatus = status;

  return retStatus;
  
}

Status Blang::Reset() {

  Status retStatus;
  Status status;

  retStatus = ShutDown();
  status    = StartUp();

  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status Blang::MoveFrom(Blang const &that) {
  
  Status retStatus;
  Status status;

  if ((Blang *)&that == this)
    retStatus = Error::None;
  else {
    retStatus = ShutDown();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(Blang));
    status = ((Blang *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
  }
  NotConsumable();
  
  return retStatus;
  
}

Status Blang::CopyFrom(Blang const &that) {

  Status retStatus;  
  Status status;
  
  if (this == (Blang *)&that)
    retStatus = Error::None;
  else {
    if (IsConsumable())
      retStatus = MoveFrom(that);
    else {

      isConsumable = false;
      isStarted    = that.isStarted;
      isAtEnd      = that.isAtEnd;
      skipToEOL    = that.skipToEOL;
     
      retStatus = inputFileName.CopyFrom(that.inputFileName);
      status = inputString.CopyFrom(that.inputString);
      if (status && !retStatus) retStatus = status;
      status = inputPrompt.CopyFrom(that.inputPrompt);
      if (status && !retStatus) retStatus = status;
      
      status = lex.CopyFrom(that.lex);
      if (status && !retStatus) retStatus = status;
      status = lang.CopyFrom(that.lang);
      if (status && !retStatus) retStatus = status;
      status = stream.CopyFrom(that.stream);
      if (status && !retStatus) retStatus = status;

      status = tokens.CopyFrom(that.tokens);
      if (status && !retStatus) retStatus = status;
      curTokens = that.curTokens;
      maxTokens = that.maxTokens;

      curTokNo     = that.curTokNo;
      lookBehind   = that.lookBehind;
      lookAhead    = that.lookAhead;
      curTokens    = that.curTokens;
      maxTokens    = that.maxTokens;
      totInTokens  = that.totInTokens;
      totOutTokens = that.totOutTokens;
      curLineNo    = that.curLineNo;
      curLineStart = that.curLineStart;
      
    }

  }
  
  return retStatus;
  
}

Boolean Blang::IsNull() const {
  return (
    lex.IsNull()               &&
    lang.IsNull()              &&
    stream.IsNull()            &&
    tokens.IsNull()            &&
    BEFE::IsNull(curTokens)    &&
    BEFE::IsNull(maxTokens)    &&
    BEFE::IsNull(totInTokens)  &&
    BEFE::IsNull(totOutTokens) &&
    BEFE::IsNull(curLineNo)    &&
    BEFE::IsNull(curLineStart)
  );
}

Status Blang::SetNull() {
  return Reset();
}

Boolean Blang::IsEmpty() const {
  return IsNull();
}

Status Blang::SetEmpty() {
  return Reset();
}

UInt Blang::Length() const {
  return 0;
}

UInt Blang::Size() const {
  return 0;
}

Boolean Blang::IsConsumable() const {
  return isConsumable;
}

Blang &Blang::Consumable() const {
  ((Blang *)this)->isConsumable = true;
  return *((Blang *)this);
}

Blang &Blang::NotConsumable() const {
  ((Blang *)this)->isConsumable = false;
  return *((Blang *)this);
}

//----------------------------------------------------------------------
//
// Operational Methods
//

Status Blang::GetStatistics(UInt &inTokens,
                            UInt &outTokens) {
  
  inTokens = totInTokens;
  outTokens = totOutTokens;
  
  return Error::None;
  
}
//----------------------------------------------------------------------
//
// Low level Under-the-cover Methods
//

Status Blang::_StartParsing() { // Blang._StartParsing...
  
  Status status;
  
  // Nothing to do if already parsing...
  if (isStarted) goto OK;

  // Some sanity checks
  if (BEFE::IsNull(TheBefe)) goto NOBEFE;
  
  // Reset stuff first...
  status = lex.Reset();
  if (status) goto SOMEERROR;
  status = lang.Reset();
  if (status) goto SOMEERROR;
  status = stream.Reset();
  if (status) goto SOMEERROR;
  status = tokens.Reset();
  if (status) goto SOMEERROR;
  BEFE::SetNull(curTokens);
  BEFE::SetNull(maxTokens);
  BEFE::SetNull(curLineNo);
  BEFE::SetNull(curLineStart);

  // Set up the language and register it
  status = LexLangBefe(lang);
  if (status) goto SOMEERROR;
  status = lex.RegisterLanguage(lang);
  if (status) goto SOMEERROR;
  
  // Set up the LexStream...
  status = lex.Attach(stream);
  if (status) goto SOMEERROR;
  if (!inputFileName.IsNull()) {
    status = stream.SetInputFileName(inputFileName);
    if (status) goto SOMEERROR;
  }
  else if (!inputString.IsNull()) {
    status = stream.SetInputString(inputString);
    if (status) goto SOMEERROR;
  }
  else
    goto NOINPUT;
    
  status = stream.SetLanguageName(".befe");
  if (status) goto SOMEERROR;
  
  // We're all ready to go...
  isStarted = true;
  isAtEnd   = false;
  
  // Get the first N tokens...
  status = _GetLexTokens();
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    OK:        status = Error::None;                 break;
    NOBEFE:    status = Error::NoBefe;               break;
    NOINPUT:   status = Error::LangNoInputSpecified; break;
    SOMEERROR:                                       break;
  }
  
  return status;
  
} // ...Blang._StartParsing

Status Blang::_StopParsing() {

  Status retStatus;
  Status status;
  
  retStatus = Error::None;

  if (isStarted) {
  
    status = lex.Reset();
    if (status && !retStatus) retStatus = status;
    status = lang.Reset();
    if (status && !retStatus) retStatus = status;
    status = stream.Reset();
    if (status && !retStatus) retStatus = status;
    status = tokens.Reset();
    if (status && !retStatus) retStatus = status;
    BEFE::SetNull(curLineNo);
    BEFE::SetNull(curLineStart);
    
    if (!retStatus)
      isStarted = false;
      
  }
  
  return retStatus;
  
}

Status Blang::_GetLexTokens() { // Blang._GetLexTokens...
  
  Status    status;
  LexToken  token;
  LexToken *curToken;
  UInt      i;
  
  // Make sure we have something to do...
  if (!isStarted || isAtEnd) goto OK;
  if (!BEFE::IsNull(curTokens) &&
      !BEFE::IsNull(curTokNo)  &&
      curTokNo < curTokens-lookAhead)
    goto OK;
    
  // Preallocate the Array if we haven't already...
  token0 = (LexToken *)tokens._GetAddress(0);
  if (BEFE::IsNull(token0)) {
    
    // Fixed stuff
    curTokens  = 0;
    maxTokens  = 16;
    lookBehind = 2;
    lookAhead  = 2;
    curTokNo   = 0;
    
    // Allocate it...
    status = tokens.SetLength(maxTokens);
    if (status) goto SOMEERROR;
    for (i=0; i < maxTokens; i++) {
      status = tokens.Append(token);
      if (status) goto SOMEERROR;
    }
    token0 = (LexToken *)tokens._GetAddress(0);
    if (BEFE::IsNull(token0)) goto NULLPOINTER;
    
  }
  
  // Slide remaining tokens down...
  if (curTokNo >= maxTokens-lookAhead) {
    for (i=0; i < (lookAhead+lookBehind+1); i++)
      token0[i] = token0[maxTokens-(lookAhead+lookBehind+1)+i];
    curTokNo  = curTokNo - (maxTokens-(lookAhead+lookBehind+1));
    curTokens = lookAhead+lookBehind+1;
  }
  
  // Until we've got 'em...
  while (curTokens < maxTokens) {

    curToken = token0+curTokens;
    status = stream.NextToken(*curToken);
    if (status) goto SOMEERROR;
    totInTokens++;
    
    // Skip it we're skipping to EOL and it's a newLine...
    if (skipToEOL && curToken->type != LexToken::EndOfLine)
      continue;
      
    // Got one
    curTokens++;
    
    // Handle end of input stream...
    if (curToken->type == LexToken::EndOfInput) {
      isAtEnd = true;
      break;
    }
    
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:          status = Error::None;                break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
} // ...Blang._GetLexTokens

} // ...Namespace BEFE
