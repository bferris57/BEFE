//----------------------------------------------------------------------
// File: LexLang.h - Declarations for the BEFE LexLang<lang> Functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LEXLANG_H // LEXLANG_H...
#define LEXLANG_H

#include "Primitives.h"
#include "LexLanguage.h"
#include "LexStream.h"

namespace BEFE { // Namespace BEFE...

//
// Default Action Handlers
//   Implemented in LexLangActions.cpp
//

Status LexLangDefaultUTF(LexStream &stream, Char theChar);
Status LexLangDefaultNewLine(LexStream &stream, Char theChar);
Status LexLangDefaultWhiteSpace(LexStream &stream, Char theChar);
Status LexLangDefaultName(LexStream &stream, Char theChar);
Status LexLangDefaultNumber(LexStream &stream, Char theChar);
Status LexLangDefaultQuote(LexStream &stream, Char theChar);
Status LexLangDefaultOperator(LexStream &stream, Char theChar);
Status LexLangDefaultComment(LexStream &stream, Char theChar);
Status LexLangDefaultPreprocessor(LexStream &stream, Char theChar);
Status LexLangDefaultBracketLeft(LexStream &stream, Char theChar);
Status LexLangDefaultBracketRight(LexStream &stream, Char theChar);
Status LexLangDefaultSeparator(LexStream &stream, Char theChar);
Status LexLangDefaultEscape(LexStream &stream, Char theChar);
Status LexLangDefaultFlushMain(LexStream &stream, Char theChar);
Status LexLangDefaultAppend(LexStream &stream, Char theChar);
Status LexLangDefaultAppendFlushMain(LexStream &stream, Char theChar);

//
// C/C++ type Action Handlers...
//   Implemented in LexLangActions.cpp
//

Status LexActionCppQuote(LexStream &stream, Char theChar);
Status LexActionCheckQuote(LexStream &stream, Char theChar);
Status LexActionCheckEscape(LexStream &stream, Char theChar);
Status LexActionCppComment(LexStream &stream, Char theChar);
Status LexActionCppPreprocessor(LexStream &stream, Char theChar);
Status LexActionWaitForEOL(LexStream &stream, Char theChar);
Status LexActionCheckForEOC(LexStream &stream, Char theChar);
Status LexActionEOCNewLine(LexStream &stream, Char theChar);
Status LexActionOpSlash(LexStream &stream, Char theChar);
Status LexActionOpStar(LexStream &stream, Char theChar);
Status LexActionOpPlus(LexStream &stream, Char theChar);
Status LexActionOpHyphen(LexStream &stream, Char theChar);
Status LexActionOpLess(LexStream &stream, Char theChar);
Status LexActionOpGreater(LexStream &stream, Char theChar);
Status LexActionOpEqual(LexStream &stream, Char theChar);
Status LexActionOpPling(LexStream &stream, Char theChar);
Status LexActionOpAmper(LexStream &stream, Char theChar);
Status LexActionOpOr(LexStream &stream, Char theChar);
Status LexActionOpColon(LexStream &stream, Char theChar);
Status LexActionBracketLeft(LexStream &stream, Char theChar);
Status LexActionBracketRight(LexStream &stream, Char theChar);
Status LexActionOpDot(LexStream &stream, Char theChar);
Status LexActionOpDotDot(LexStream &stream, Char theChar);

//
// Python type Action Handlers...
//   Implemented in LexLangActions.cpp
//

Status LexActionQuote(LexStream &stream, Char theChar);
Status LexActionCheckEOQ(LexStream &stream, Char theChar);

//----------------------------------------------------------------------
//
// Language Functions
//

Status LexLangCpp(LexLanguage &lang);
Status LexLangBefe(LexLanguage &lang);

} // ...Namespace BEFE

#endif // ...LEXLANG_H
