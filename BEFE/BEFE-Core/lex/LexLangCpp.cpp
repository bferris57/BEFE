//----------------------------------------------------------------------
// File: LexLangCpp.cpp - LexLanguage C++ State/Action Functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Lex.h"
#include "LexLang.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Function: LexLangCpp  - Declare C++ Language
//

Status LexLangCpp(LexLanguage &lang) { // Function LexLangCpp...

  static char const *whites   = " \t\r";
  static char const *names    = "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static char const *seps     = ",;";
  static char const *bracks[] = {"[]", "()", "{}", 0 };
  static char const *ops[] = {
    "\\",                                     // Escape Character
    "+",  "++", "-",  "--", "%",  "*",  "/",  // Arithmetic (depending on context)
    "<",  ">",  "<=", ">=", "==", "<>", "!=", // Comparison
    "<<", ">>",                               // Shift/Pipe
    "~",  "&",  "|",  "^",                    // Bitwise
    "!",  "&&", "||",                         // Logical
    "?",  ":",                                // Ternary
    "::",                                     // Namespace separator
    "->", ".",                                // Addressing
    "=",  "+=", "-=", "*=", "/=", "%=",       // Assignment...
    "|=", "&=", "~=", "^=",
    0};
  static char const *coms[] = {"/*", "*/", "//",0};
    
  Status    status;
  String    whiteChars;
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
  nameChars  = names;
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
  status = lang.SetName("C++");
  if (status) goto SOMEERROR;
  
  // Set the LexLanguage Generate members...
  status = lang.SetWhiteChars(whiteChars);
  if (status) goto SOMEERROR;
  status = lang.SetNameStart(nameChars);
  if (status) goto SOMEERROR;
  status = lang.SetNameChars(nameChars);
  if (status) goto SOMEERROR;
  status = lang.SetOperators(operators);
  if (status) goto SOMEERROR;
  status = lang.SetBrackPairs(brackPairs);
  if (status) goto SOMEERROR;
  status = lang.SetSepChars(sepChars);
  if (status) goto SOMEERROR;
  status = lang.SetQuoteChars("\"'");
  if (status) goto SOMEERROR;
  status = lang.SetComments(comments);
  if (status) goto SOMEERROR;
  status = lang.SetPreprocessorChar('#');
  if (status) goto SOMEERROR;
  
  // Generate it
  isValid = lang.IsValid();
  if (!isValid) goto BADSPEC;
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
  
  // Register 'DLiteral' Action and State for handling characters during "..."
  state.Reset();
  state.name = "DLiteral";
  MemoryFillRaw(&state.byteActions[0], (Byte)Lex::ActionAppend,   0x80);
  state.byteActions['"']  = Lex::ActionAppendFlushMain;
  state.byteActions['\n'] = Lex::ActionNewLine;
  state.byteActions['\\'] = lang.GetActionIndex("CheckEscape");
  status = lang.NewState(state);
  if (status) goto SOMEERROR;
  
  // Register 'SLiteral' State for handling characters during '...'
  state.Reset();
  state.name = "SLiteral";
  MemoryFillRaw(&state.byteActions[0], (Byte)Lex::ActionAppend,   0x80);
  state.byteActions['\''] = (Byte)Lex::ActionAppendFlushMain;
  state.byteActions['\n'] = Lex::ActionNewLine;
  state.byteActions['\\'] = lang.GetActionIndex("CheckEscape");
  status = lang.NewState(state);
  if (status) goto SOMEERROR;

  // Register 'BLiteral' State for handling characters during `...` even though not valid for C/C++
  state.Reset();
  state.name = "BLiteral";
  MemoryFillRaw(&state.byteActions[0], (Byte)Lex::ActionAppend,   0x80);
  state.byteActions['`'] = (Byte)Lex::ActionAppendFlushMain;
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
  state.name = "WaitForEOC";
  MemoryFillRaw(&state.byteActions[0], (Byte)Lex::ActionAppend,   0x80);
  state.byteActions['*']  = lang.GetActionIndex("CheckForEOC");
  state.byteActions['\n'] = lang.GetActionIndex("EOCNewLine");
  status = lang.NewState(state);
  if (status) goto SOMEERROR;
  
  // TEMP...
  if (true) {
    status = lang.GetState("Main", state);
    if (status) goto SOMEERROR;
    status = state.PopulateRangesFromBytes();
    if (status) goto SOMEERROR;
    //state.type = LexState::TypeChar;
    status = lang.UpdateStateById(state);
    if (status) goto SOMEERROR;
  }
  // ...TEMP
  
  // Handle errors
  while (false) {
    BADSPEC:   status = Error::LexLangSpecErrors; break;
    SOMEERROR:                                    break;
  }
  
  return status;
  
} // ...Function LexLangCpp

} // ...Namespace BEFE
