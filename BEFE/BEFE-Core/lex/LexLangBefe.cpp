// File: LexLangBefe.cpp - LexLanguage Befe State/Action Functions
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
#include "LexLang.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Function: LexLangBefe  - Declare C++ Language
//

Status LexLangBefe(LexLanguage &lang) { // Function LexLangBefe...

  static char const *whites   = " \t\r";
  static char const *name0    = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static char const *name1    = "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static char const *seps     = ",;";
  static char const *bracks[] = {"[]", "()", "{}", 0 };
  static char const *ops[] = {
    //">>=*",
    ".", "..", "...",
    "!", "$", "&", "+", "-", "/", "/%", "*", "^", "%", "~", "|"
    "!=",          "+=","-=","/=","/%=","*=","^=","%=","~=","|=",
		"++","--",
    ":", "<", "=",  ">", "?", "@","<>",
    ":=","<=","==", ">=","?=","@=",
		"->","<-","<->",
    "<<",">>","&&","||",
    "<<<",">>>",
    "\\","\\\\",
    "|",
		//"is","is not","not","in","not in","if","else",
		//"⅛","¼","⅜","⅝","½","¾","⅞","√","²","³","∞","∑","≤","≥","≡","≠",
    0};
  static char const *coms[] = {"/*", "\\*", "#", "//",0};
    
  Status    status;
  String    whiteChars;
  String    nameStart;
  String    nameChars;
  String    sepChars;
  Strings   brackPairs;
  String    quoteChars;
  Strings   operators;
  Strings   comments;
  UInt      i;
  Strings   errors;
  Boolean   isValid;
  LexState  state;
  LexAction action;

  // Start from scratch on the LexLanguage...
  status = lang.Reset();
  if (status) goto SOMEERROR;
  
  // Setup our local values...
  whiteChars = whites;
  nameStart  = name0;
  nameChars  = name1;
  sepChars   = seps;  
  for (i=0; ops[i]; i++) {
    status = operators.Append(ops[i]);
    if (status) goto SOMEERROR;
  }
  for (i=0; bracks[i]; i++) {
    status = brackPairs.Append(bracks[i]);
    if (status) goto SOMEERROR;
  }
  for (i=0; coms[i]; i++) {
    status = comments.Append(coms[i]);
    if (status) goto SOMEERROR;
  }
  
  // Set the Language Name..
  status = lang.SetName(".befe");
  if (status) goto SOMEERROR;
  
  // Set the LexLanguage Generate members...
  status = lang.SetWhiteChars(whiteChars);
  if (status) goto SOMEERROR;
  status = lang.SetNameStart(nameStart);
  if (status) goto SOMEERROR;
  status = lang.SetNameChars(nameChars);
  if (status) goto SOMEERROR;
  status = lang.SetOperators(operators);
  if (status) goto SOMEERROR;
  status = lang.SetBrackPairs(brackPairs);
  if (status) goto SOMEERROR;
  status = lang.SetSepChars(sepChars);
  if (status) goto SOMEERROR;
  status = lang.SetQuoteChars("\"'`");
  if (status) goto SOMEERROR;
  status = lang.SetComments(comments);
  if (status) goto SOMEERROR;
  status = lang.SetUtfInNames(true);
  if (status) goto SOMEERROR;
  
  // Generate it
  isValid = lang.IsValid();
  if (!isValid) {
    for (i=0; i < lang.errors.Length(); i++)
      BEFE_WARN(String("LexLangBefe: ") << lang.errors.Get(i));
    goto BADSPEC;
  }
  status = lang.Generate();
  if (status) goto SOMEERROR;
  
  // Register Action handler overrides...
  status = lang.SetActionHandler("Quote", &LexActionCppQuote);
  if (status) goto SOMEERROR;  
  status = lang.SetActionHandler("Comment", &LexActionCppComment);
  if (status) goto SOMEERROR;
  status = lang.SetActionHandler("Preprocessor", &LexActionCppPreprocessor);
  if (status) goto SOMEERROR;
  status = lang.SetActionHandler("BracketLeft", &LexActionBracketLeft);
  if (status) goto SOMEERROR;  
  status = lang.SetActionHandler("BracketRight", &LexActionBracketRight);
  if (status) goto SOMEERROR;
  
  // Register 'CheckEscape' Action for string/char literals
  action.Reset();
  action.name = "CheckEscape";
  action.charHandler = &LexActionCheckEscape;
  status = lang.NewAction(action);
  if (status) goto SOMEERROR;
  
  // Register 'Dliteral' Action and State for handling characters during "..."
  state.Reset();
  state.type = LexState::TypeByte;
  state.name = "DLiteral";
  MemoryFillRaw(&state.byteActions[0], (Byte)Lex::ActionAppend,   0x80);
  state.byteActions['"']  = Lex::ActionAppendFlushMain;
  state.byteActions['\n'] = Lex::ActionNewLine;
  state.byteActions['\\'] = lang.GetActionIndex("CheckEscape");
  status = lang.NewState(state);
  if (status) goto SOMEERROR;
  
  // Register 'Sliteral' State for handling characters during '...'
  state.Reset();
  state.type = LexState::TypeByte;
  state.name = "SLiteral";
  MemoryFillRaw(&state.byteActions[0], (Byte)Lex::ActionAppend,   0x80);
  state.byteActions['\''] = (Byte)Lex::ActionAppendFlushMain;
  state.byteActions['\n'] = Lex::ActionNewLine;
  state.byteActions['\\'] = lang.GetActionIndex("CheckEscape");
  status = lang.NewState(state);
  if (status) goto SOMEERROR;
  
  // Register 'Bliteral' State for handling characters during '...'
  state.Reset();
  state.type = LexState::TypeByte;
  state.name = "BLiteral";
  MemoryFillRaw(&state.byteActions[0], (Byte)Lex::ActionAppend,   0x80);
  state.byteActions['`']  = (Byte)Lex::ActionAppendFlushMain;
  state.byteActions['\n'] = Lex::ActionNewLine;
  state.byteActions['\\'] = lang.GetActionIndex("CheckEscape");
  status = lang.NewState(state);
  if (status) goto SOMEERROR;

  // Register 'WaitForEOL' Action and State for handling characters during '#...' or '//...'
  action.Reset();
  action.name = "WaitForEOL";
  action.charHandler = &LexActionWaitForEOL;
  status = lang.NewAction(action);
  if (status) goto SOMEERROR;
  
  state.Reset();
  state.type = LexState::TypeByte;
  state.name = "WaitForEOL";
  MemoryFillRaw(&state.byteActions[0], (Byte)Lex::ActionAppend,   0x80);
  state.byteActions['\n'] = (Byte)Lex::ActionNewLine;
  status = lang.NewState(state);
  if (status) goto SOMEERROR;
  
  // Register "/*...*/" Actions and State...
  action.Reset();
  action.name = "CheckForEOC";
  action.charHandler = &LexActionCheckForEOC;
  status = lang.NewAction(action);
  if (status) goto SOMEERROR;
  action.Reset();
  action.name = "EOCNewLine";
  action.charHandler = &LexActionEOCNewLine;
  status = lang.NewAction(action);
  if (status) goto SOMEERROR;
  
  state.Reset();
  state.type = LexState::TypeByte;
  state.name = "WaitForEOC";
  MemoryFillRaw(&state.byteActions[0], (Byte)Lex::ActionAppend,   0x80);
  state.byteActions['*']  = lang.GetActionIndex("CheckForEOC");
  state.byteActions['\n'] = lang.GetActionIndex("EOCNewLine");
  status = lang.NewState(state);
  if (status) goto SOMEERROR;

  // Register/override quotes because we want Python like multi-quotes...
  action.Reset();
  action.name = "Quote";
  action.charHandler = &LexActionQuote;
  status = lang.NewAction(action);
  if (status) goto SOMEERROR;
  action.Reset();
  action.name = "CheckEOQ";
  action.charHandler = &LexActionCheckEOQ;
  status = lang.NewAction(action);
  if (status) goto SOMEERROR;

  state.Reset();
  state.type = LexState::TypeByte;
  state.name = "MultiQuote";
  MemoryFillRaw(&state.byteActions[0], (Byte)lang.GetActionIndex("CheckEOQ"),   0x80);
  status = lang.NewState(state);
  if (status) goto SOMEERROR;
  
  status = lang.GetState("Main", state);
  if (status) goto SOMEERROR;
  // Note: We'll leave this next one commented.  Uncomment it if we
  //       decide to treat ALL UTF-8 characters as operators in the
  //       .befe language...
  //
  //state.byteActions[0]    = (Byte)lang.GetActionIndex("Operator");
  state.byteActions['\''] = (Byte)lang.GetActionIndex("Quote");
  state.byteActions['"']  = (Byte)lang.GetActionIndex("Quote");
  state.byteActions['`']  = (Byte)lang.GetActionIndex("Quote");
  status = lang.UpdateStateById(state);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    BADSPEC:   status = Error::LexLangSpecErrors; break;
    SOMEERROR:                                    break;
  }
  
  return status;
  
} // ...Function LexLangBefe

} // ...Namespace BEFE
