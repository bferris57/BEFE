//----------------------------------------------------------------------
// File: LexFormatOut.cpp - Implementation of the BEFE LexFormatOut Struct
//----------------------------------------------------------------------
//
// This file contains the declarations for the LexFormat Classes.
//
// LexFormatOut is responsible for output formatting of various Lexical
// Analysis Classes, while LexFormatIn is responsible for input
// formatting.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEWrappers.h"
#include "BEFEMacros.h"
#include "IntFuncs.h"
#include "ToStrings.h"
#include "LexFormat.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// LexToken type names
//

static char const *lexTokenTypes[] = {
  "Unknown",
	"EndOfInput",     // Input exhausted
	"EndOfLine",      // Typically '\n'
  "Error",          // Typically Error::None
  "Unexpected",     // Character not recognised
  "Warning",        // Typically Error::None
  "WhiteSpace",     // Whitespace (typically 0x00..0x20)
	"Operator",       // Some operator
	"BracketLeft",    // Beginning of some grouping
  "BracketRight",   // End of some grouping
	"Separator",      // List separator
  "Comment",        // Comment
  "Name",           // The "Name" of something
  "Number",         // Number literal
  "DLiteral",       // Double-quote literal
  "SLiteral",       // Single-quote literal
	"BLiteral",       // Back-quote   literal
  "MultiQuote",     // MultiQuote   literal
  "Preprocessor",   // C/C++ preprocessor
  0
  };
static UInt maxLexTokenLen = 0;  

//----------------------------------------------------------------------
//
// LexToken type names
//

static char const *bmsgTokenTypes[] = {
  "Unknown",
  "Error",            // Error
  "Warning",          // Warning
  "Begin...",         // Named Start Ellipsis (xxx...)
  "...End",           // Named End Ellipsis (...xxx)
  "Name",             // Name of something
  "Number",           // Will need validating though
  "Equals",           // "="
  "Range",            // ".."
  "Separator",        // ","
  "String",           // "..." and '...'
  0
  };
static UInt maxBmsgTokenLen = 0;  

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LexFormatOut::LexFormatOut()                                    { StartUp(); }
LexFormatOut::LexFormatOut(LexFormatOut const &that)            { CopyFrom(that); }
LexFormatOut::LexFormatOut::~LexFormatOut()                     { ShutDown(); } 
LexFormatOut &LexFormatOut::operator=(LexFormatOut const &that) { CopyFrom(that); return *this;}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status LexFormatOut::StartUp() {
  
  isConsumable = false;
  indentCols   = 0;
  linesBefore  = 0;
  linesAfter   = 0;
  
  return Error::None;

}
  
Status LexFormatOut::ShutDown() {
  return Error::None;
}
  
Status LexFormatOut::Clear() {
  return Reset();
}

Status LexFormatOut::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}


Boolean LexFormatOut::IsNull() const {
  return BEFE::IsNull(indentCols);
}
    
Status LexFormatOut::SetNull() {
  return Error::NotCapable;
}

Boolean LexFormatOut::IsEmpty() {
  return (indentCols == 0);
}

Status LexFormatOut::SetEmpty() {
  return Clear();
}

Status LexFormatOut::MoveFrom(LexFormatOut const &that) {
  
  Status retStatus;
  Status status;
  
  retStatus = Reset();
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexFormatOut));
  isConsumable = false;
  status = ((LexFormatOut *)&that)->StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Status LexFormatOut::CopyFrom(LexFormatOut const &that) {
  
  Status retStatus;
  
  if (that.isConsumable)
    retStatus = MoveFrom(that);
  else {
    indentCols = that.indentCols;
    NotConsumable();
    retStatus = Error::None;
  }

  return retStatus;

}
            
Boolean LexFormatOut::IsConsumable() const {
  return isConsumable;
}

LexFormatOut &LexFormatOut::Consumable() const {
  ((LexFormatOut *)this)->isConsumable = true;
  return *((LexFormatOut *)this);
}

LexFormatOut &LexFormatOut::NotConsumable() const {
  ((LexFormatOut *)this)->isConsumable = false;
  return *((LexFormatOut *)this);
}  

//----------------------------------------------------------------------
//
// Output settings
//

Status LexFormatOut::SetIndent(UInt numCols) {

  Status status;

  if (!BEFE::IsNull(numCols)) {
    indentCols = numCols;
    status = Error::None;
  }
  else
    status = Error::InvalidParameter;
    
  return status;
  
}

//----------------------------------------------------------------------
//
// Output utilities
//

Status LexFormatOut::EmitLinesBefore() {
  if (linesBefore != UNaN)
    Cout << String('\n')*linesBefore;
  return Error::None;
}

Status LexFormatOut::EmitLinesAfter() {
  if (linesAfter != UNaN)
    Cout << String('\n')*linesAfter;
  return Error::None;
}
  
//----------------------------------------------------------------------
//
// Cache Formatting
//

Status LexFormatOut::DisplayCache() {
  
  Status status;
  
  NEED_TheBefe
  NEED_TheOS

  status = TheBefe->TheCache.DisplayOverview(indentCols);
  
  return status;
  
}

Status LexFormatOut::DisplayBlock(LexWorkspace &ws, UInt blockNo) {
  
  Status status;
  UInt   fileSize;
  String indent;
  
  NEED_TheBefe
  NEED_TheOS
  
  EmitLinesBefore();
  
  // Make sure it's a valid block
  status = TheBefe->TheCache.GetFileSize(ws.fileId, fileSize);
  if (status) goto SOMEERROR;
  if (BEFE::IsNull(ws.blkSize)) goto BADBLOCK;
  if (blockNo >= (fileSize + ws.blkSize -1)/ws.blkSize) goto BADBLOCK;
  
  // Display it...
  indent = String(' ')*indentCols;
  Cout << indent << "File block 0x" << ToHexString(blockNo,8) << "...\n";
  status = TheBefe->TheCache.DisplayFileBlock(ws.fileId, blockNo, indentCols+2);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    BADBLOCK:  status = Error::CacheBadBlockIdx; break;
    SOMEERROR:                                   break;
  }

  if (!status) EmitLinesAfter();
  
  return status;
  
}

//----------------------------------------------------------------------
//
// LexWorkspace formatting
//

Status LexFormatOut::Display(LexWorkspace &ws) {
  
  Status status;
  String leadIn;
  String leadOut;
  String indent;
  UInt   fileSize;
  UInt   numBlocks;
  
  NEED_TheBefe
  NEED_TheOS
  
  EmitLinesBefore();
  
  leadIn = GetEnvironmentValue("BEFE_QuoteLeadIn");
  leadOut = GetEnvironmentValue("BEFE_QuoteLeadOut");
  indent = String(' ')*indentCols;
  
  Cout << indent << "LexWorkspace @ 0x" << ToHexString((PtrInt)&ws,8);
  if (gVerbose) Cout << "...";
  Cout << '\n';

  if (gVerbose) {

    Cout << indent << "  File Name:    " << ws.fileName << '\n';

    Cout << indent << "  File Id:      0x" << ToHexString(ws.fileId,8) << '\n';
    Cout << indent << "  Block Size:   " << ws.blkSize << '\n';

    BEFE::SetNull(numBlocks);
    if (!BEFE::IsNull(ws.blkSize)) {
      BEFE::SetNull(fileSize);
      TheBefe->TheCache.GetFileSize(ws.fileId, fileSize);
      numBlocks = (fileSize+ws.blkSize-1)/ws.blkSize;
    }
    Cout << indent << "  Total Blocks: " << numBlocks << '\n';
    
    Cout << indent 
         << "  Blobs:        "
         << ws.wsHead.blobs.numEntries
         << '\n';    
    Cout << indent << "  Strings:      " << ws.wsHead.strings.numEntries << '\n';
    Cout << indent << "  Files:        " << ws.wsHead.files.numEntries << '\n';
    
    if (gVeryVerbose) {

      indentCols += 4;

      Cout << indent << "  Blob Array...\n";
      status = Display(ws.wsHead.blobs);
      if (status) goto SOMEERROR;
      
      Cout << indent << "  File Array...\n";
      status = Display(ws.wsHead.files);
      if (status) goto SOMEERROR;

      Cout << indent << "  String Array...\n";
      status = Display(ws.wsHead.strings);
      if (status) goto SOMEERROR;
      Cout << indent << "  String Blob Id:      0x"
           << ToHexString(ws.wsHead.stringsBlobId,8) << '\n';

      indentCols -= 4;
      
    }
    
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }

  if (!status) EmitLinesAfter();
  
  return status;

}

//----------------------------------------------------------------------
//
// LexWsArrayHead formatting
//

Status LexFormatOut::Display(LexWsArrayHead &ah) {
  
  String indent;
  
  NEED_TheBefe
  NEED_TheOS

  EmitLinesBefore();

  indent = String(' ')*indentCols;
  
  Cout << indent << "Block Type: " << ah.blockType << '\n';
  Cout << indent << "Name Id:           0x" << ToHexString(ah.nameId,8) << '\n';
  Cout << indent << "High Id:           0x" << ToHexString(ah.hiId,8) << '\n';
  Cout << indent << "Entry Size:        0x" << ToHexString((UInt)ah.entrySize,4) << '\n';
  Cout << indent << "Entries Per Block: ";
  if (BEFE::IsNull(ah.entriesPerBlock))
    Cout << "NaN";
  else
    Cout << ah.entriesPerBlock;
  Cout << '\n';
  Cout << indent << "Total Entries:     " << ah.numEntries << '\n';
  Cout << indent << "First Low Block:   0x" << ToHexString(ah.fstLowBlockNo,8) << '\n';
  Cout << indent << "Last  Low Block:   0x" << ToHexString(ah.lstLowBlockNo,8) << '\n';
  Cout << indent << "Top       Block:   0x" << ToHexString(ah.topBlockNo,8) << '\n';

  EmitLinesAfter();
  
  return Error::None;
  
}

//----------------------------------------------------------------------
//
// LexLanguage formatting
//

Status LexFormatOut::DisplayLanguage(LexLanguage &lang) {
  
  Status  status;
  UInt    saveIndentCols;
  String  indent;
  UInt    oldColour;
  String  leadIn;
  String  leadOut;
  Strings errors;
  UInt    i;
  UInt    numOut;
  String  outStr;
  
  NEED_TheBefe
  NEED_TheOS

  EmitLinesBefore();

  saveIndentCols = indentCols;
  indent         = String(' ')*indentCols;
  oldColour      = Cout.GetForegroundColour();
  leadIn         = GetEnvironmentValue("BEFE_QuoteLeadIn");
  leadOut        = GetEnvironmentValue("BEFE_QuoteLeadOut");
 
  Cout <<  indent
       << "LexLanguage '" << lang.name << "'" 
       << " (&0x" << ToHexString((PtrInt)&lang,8) << ')';
       
  if (!gVerbose) {
    Cout << '\n';
    goto OK;
  }
  Cout << "...\n\n";
  
  indentCols += 2;
  indent *= indentCols;
  
  // Display language stuff...
  Cout << indent << "id:         0x" << ToHexString(lang.id,8) << '\n';
  Cout << indent << "contigMax:  " << lang.contigMax << '\n';
  Cout << indent << "whiteChars: " << lang.whiteChars.Escape() << '\n';
                    
  numOut = 0;
  outStr.SetEmpty();
  for (i=0; i < lang.operators.Length(); i++) {
    if (outStr.Length() > 40) {
      Cout << indent << ((numOut==0)?"operators:  ":"            ") << outStr << '\n';
      outStr.SetEmpty();
      numOut++;
    }
    if (outStr.Length()) outStr += ' ';
    outStr += lang.operators.Get(i);
  }
  if (outStr.Length())
    Cout << indent << ((numOut==0)?"operators:  ":"            ") << outStr << '\n';
  
  Cout << indent << "brackPairs: ";
  for (i=0; i < lang.brackPairs.Length(); i++) {
    if (i != 0) Cout << ' ';
    Cout << lang.brackPairs.Get(i);
  }
  Cout << '\n';
  
  Cout << indent << "sepChars:   " << lang.sepChars << '\n';
  Cout << indent << "quoteChars: "  << lang.quoteChars  << '\n';

  Cout << indent << "comments:   ";
  for (i=0; i < lang.comments.Length(); i++) {
    if (i != 0) Cout << ' ';
    Cout << lang.comments.Get(i);
  }
  Cout << '\n';

  Cout << indent << "stateHiId:  0x" << ToHexString(lang.stateHiId,8) << '\n';
  Cout << indent << "actionHiId: 0x" << ToHexString(lang.actionHiId,8) << '\n';
  
  //
  // Display States
  //

  if (!gVeryVerbose || lang.states.Length() == 0)
    Cout << '\n' << indent << "Total States:  " << lang.states.Length() << '\n';
  else {
    for (i=0; i < lang.states.Length(); i++) {
      status = DisplayState(lang, i);
      if (status) goto SOMEERROR;
    }
  }

  //
  // Display Actions
  //

  if (!gVeryVerbose || lang.actions.Length() == 0)
    Cout << '\n' << indent << "Total Actions: " << lang.actions.Length() << '\n';
  else {
    for (i=0; i < lang.actions.Length(); i++) {
      status = DisplayAction(lang, i);
      if (status) goto SOMEERROR;
    }
  }

  //
  // Display errors
  //
  
  if (gVeryVerbose && !lang.IsValid()) {
    errors = lang.GetErrors();
    Cout << indent << "Errors (" << errors.Length() << ")...\n";
    Cout.SetForegroundColour(Console::Red);
    for (i=0; i < errors.Length(); i++)
        Cout << "  " << errors.Get(i) << '\n';
    Cout.SetForegroundColour(oldColour);
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  // Restore stuff as it was on entry...
  indentCols = saveIndentCols;
  Cout.SetForegroundColour(oldColour);

  if (!status) EmitLinesAfter();
  
  return status;
  
}

Status LexFormatOut::DisplayState(LexLanguage &lang, UInt stateNo) {
  
  Status   status;
  String   indent;
  LexState state;
  UInt     intLen;
  UInt     idLen;
  String   typeName;
  UInt     numStates;
  UInt     numRanges;
  LexRange range;
  UInt     i;
  UInt     maxRangeLen;
  String   rangeStr;
  
  char const *typeNames[] = {"Null","Byte","Char"};
  
  indent = String(' ')*indentCols;
  
  numStates = lang.states.Length();
  if (stateNo >= numStates) goto INTERNAL;
  
  status = lang.states.Get(stateNo, state);
  if (status) goto SOMEERROR;
 
  //intLen = IntLog10(lang.states.Length()-1)+1;
  idLen  = IntLog2(lang.stateHiId+255)/8+1;
  if (state.type >= LexState::NumTypes)
    typeName = "????";
  else
    typeName = typeNames[state.type];
  Cout << indent << "state [" << ToHexString(stateNo,2) << "]:"
       << " id = 0x" << ToHexString(state.id,idLen)
       << ", type = " << typeName
       << ", name = '" << state.name << "'";
  if (gVeryVerbose && state.type < LexState::NumTypes && state.type != LexState::TypeUnknown )
    Cout << " ...";
  Cout << '\n';
  
  // Display Byte Contents...
  if (gVeryVeryVerbose && state.type == LexState::TypeByte) {
    DisplayMemory(state.byteActions, sizeof(state.byteActions), 0, indentCols+2);
  }

  // Display Char Contents...
  if (gVeryVerbose && state.ranges.Length() != 0) {

    indent = String(' ')*(indentCols+2);
    numRanges = state.ranges.Length();

    maxRangeLen = 0;
    for (i=0; i < numRanges; i++) {
      range = state.ranges.Get(i);
      maxRangeLen = Max(maxRangeLen, range.range.ToString().Length());
    }

    intLen = IntLog10(numRanges-1) + 1;
    for (i=0; i < numRanges; i++) {
      range = state.ranges.Get(i);
      rangeStr = range.range.ToString();
      Cout << indent
           << "Range[" << BEFE::ToString(i,intLen) << "] = "
           << rangeStr << String(' ')*(maxRangeLen-rangeStr.Length())
           << " actionNo = 0x" << ToHexString((UInt)range.actionNo,2)
           << '\n';
    }

  } 
  
  // Handle errors
  while (false) {
    INTERNAL:  status = Error::Internal; break;
    SOMEERROR:                           break;
  }

  return status;

}

Status LexFormatOut::DisplayAction(LexLanguage &lang, UInt actionNo) {
  
  Status     status;
  String     indent;
  LexAction *actions;
  LexAction  action;
  UInt       idLen;
  UInt       maxNameLen;
  UInt       i;
  //UInt       intLen;

  indent = String(' ')*indentCols;

  // Figure out longest name length...
  actions = (LexAction *)lang.actions._GetAddress(0);
  if (BEFE::IsNull(actions)) goto NULLPOINTER;
  maxNameLen = 0;
  for (i=0; i < lang.actions.Length(); i++)
    maxNameLen = Max(maxNameLen, actions[i].name.Length());
    
  // Get this action
  status = lang.actions.Get(actionNo, action);
  if (status) goto SOMEERROR;
  
  //intLen = IntLog10(lang.actions.Length()-1)+1; 
  idLen  = IntLog2(lang.stateHiId+255)/8+1;
  Cout << indent << "action[0x" << ToHexString(actionNo,2) << "]:"
       << " id = 0x" << ToHexString(action.id, idLen)
       << ", name = '" << action.name << "'";
  if (!BEFE::IsNull(action.charHandler))
    Cout << String(' ')*(maxNameLen-action.name.Length())
         << " handler = 0x"
         << ToHexString((PtrInt)action.charHandler, 8);
  Cout << '\n';
  
  // Handle errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }

  return status;

}

Status LexFormatOut::DisplayToken(UInt tokenNo, LexToken &token) {
  
  Status    status;
  String    indent;
  UInt      i;
  String    typeName;
  String    tokVal;
  
  indent = String(' ')*indentCols;

  if (token.type > LexToken::NumBaseTypes) goto BADTYPE;
  
  // Calculate maximum name length if first time...
  if (maxLexTokenLen == 0) {
    for (i=0; lexTokenTypes[i]; i++)
      maxLexTokenLen = Max(maxLexTokenLen, Strlen(lexTokenTypes[i]));
  }

  // Output it
  Cout << indent;
  if (!BEFE::IsNull(tokenNo))
    Cout << "Token[" << tokenNo << "]: ";
  typeName = lexTokenTypes[token.type];
  Cout << typeName << String(' ')*(maxLexTokenLen-typeName.Length());
  tokVal = token.value.TruncateOrPad(4);
  for (i=0; i < tokVal.Length(); i++) {
    if (tokVal.Get(i) < 0x20u)
      tokVal.Replace(i,' ');
  }
  Cout << " " << tokVal;
  Cout << " line/col = "
       << ((BEFE::IsNull(token.lineNo))?token.lineNo:token.lineNo+1)
       << '/' 
       << ((BEFE::IsNull(token.colNo))?token.colNo:token.colNo+1);
  if (gVeryVeryVerbose) {
    Cout << " span = " << token.span.ToString();
    Cout << " (value.Size() = "
         << token.value.Size()
				 << ", value.Length() = "
				 << token.value.Length()
         << ")";
  }
  Cout << '\n';

  // Handle errors
  status = Error::None;
  while (false) {
    BADTYPE:   status = Error::LexInvalidTokenType; break;
  }

  return status;

}

// CAUTION: Make damn sure this matches LexToken::LexTokenType in LexToken.h!!!!
static char const *LexFormatOutTokenTypeNames[] = {
    "Unknown"        // 00: Unknown (initial value)
   ,"EndOfInput"     // 01: Input exhausted
   ,"EndOfLine"      // 02: Typically '\n'
   ,"Error"          // 03: Typically Error::None
   ,"Unexpected"     // 04: Character not recognised
   ,"Warning"        // 05: Typically Error::None
   ,"WhiteSpace"     // 06: Whitespace (typically 0x00..0x20)
   ,"Operator"       // 07: Some operator
   ,"BracketLeft"    // 08: Beginning of some grouping
   ,"BracketRight"   // 09: End of some grouping
   ,"Separator"      // 10: List separator
   ,"Comment"        // 11: Comment
   ,"Name"           // 12: The "Name" of something
   ,"Number"         // 13: Number literal
   ,"DLiteral"       // 14: Double-quote literal
   ,"SLiteral"       // 15: Single-quote literal
	 ,"BLiteral"       // 16: Back-quote literal
   ,"MultiQuote"     // 17: Multi-line String literal
	 ,"NameLiteral"    // 18: Befe Name literal
   ,"Preprocessor"   // 19: C/C++/Befe Preprocessor Directive
};

Status LexFormatOut::DisplayToken(UInt tokenNo, BmsgToken &token) {
  
  Status    status;
  String    indent;
  UInt      i;
  String    typeName;
  
  indent = String(' ')*indentCols;

  // Calculate maximum name length if first time...
  if (maxBmsgTokenLen == 0) {
    for (i=0; bmsgTokenTypes[i]; i++)
      maxBmsgTokenLen = Max(maxBmsgTokenLen, Strlen(bmsgTokenTypes[i]));
  }

  // Output it
  Cout << indent;
  if (!BEFE::IsNull(tokenNo))
    Cout << "Token[" << tokenNo << "]: ";
  if (token.type >= LexToken::LexTokenType::NumBaseTypes)
    typeName = String("0x") << ToHexString((UInt)token.type,2);
  else
    typeName = LexFormatOutTokenTypeNames[token.type];
  Cout << typeName << String(' ')*(maxBmsgTokenLen-typeName.Length());
  Cout << " Line/Col " << token.lineNo+1 << '/' << token.colNo+1;
  Cout << " " << token.value.Pad(4);
  Cout << '\n';

  // Handle errors
  status = Error::None;

  return status;

}

Status LexFormatOut::DisplayLine(BmsgLine &line) {
  
  Status     status;
  String     indent;
  UInt       saveIndentCols;
  UInt       i;
  UInt       numTokens;
  
  indent = String(' ')*indentCols;
  saveIndentCols = indentCols;
  indentCols += 2;

  // Output header line
  numTokens = line.tokens.Length();
  Cout << "Line " << BEFE::ToString(line.lineNo+1) << ": "  << numTokens << " BmsgTokens";
  if (numTokens && gVeryVerbose)
    Cout << "...";
  Cout << '\n';
  
  // If there's tokens and we don't have a denormalised BmsgToken pointer
  // in the line, set it...
  if (numTokens && BEFE::IsNull(line.token0)) {
    line.token0 = (BmsgToken *)line.tokens._GetAddress(0);
    if (BEFE::IsNull(line.token0)) goto NULLPOINTER;
  }
  
  // Output the tokens...
  if (gVeryVerbose) {
    for (i=0; i < numTokens; i++) {
      status = DisplayToken(i, *(line.token0+i));
      if (status) goto SOMEERROR;
    }
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  indentCols = saveIndentCols;

  return status;

}

String LexFormatOut::ToString(LexToken &token) {

  String typeName;
  String out;
  String dquote = String('"');

  if (token.type >= LexToken::LexTokenType::NumBaseTypes)
    typeName = String("0x") << ToHexString((UInt)token.type,2);
  else
    typeName = dquote + LexFormatOutTokenTypeNames[token.type] + dquote;

  out << "{";
  out << dquote+"type"+dquote      << ':' << typeName;
  out << ',';
  out << dquote+"lineCount"+dquote << ':' << token.lineCount;
  out << ',';
  out << dquote+"status"+dquote    << ':' << String("0x")+ToHexString(token.status);
  out << ',';
  out << dquote+"value"+dquote     << ':' << dquote+token.value.Escape()+dquote;
  out << ',';
  out << dquote+"lineNo"+dquote    << ':' << token.lineNo;
  out << ',';
  out << dquote+"colNo"+dquote     << ':' << token.colNo;
  out << ',';
  out << dquote+"span"+dquote      << ':' << dquote+token.span.ToString()+dquote;
  out << '}';

  return out.Consumable();

}

Strings LexFormatOut::ToStrings(LexStream &stream) {

	Status   status;
	Strings  outs;
	LexToken token;
  String   dquote = String('"');
  Int      idx;
  Boolean  first = True;

  outs.Append("#!/usr/bin/python");
  outs.Append("#ecoding=utf8");
  outs.Append(String("befeTokens = {\"file\":\"") << stream.inputFileName.Escape() << "\", \"tokens\":");

  // Start it off...
	outs.Append(String('['));

	// For each token...
	idx = -1;
	while (true) {
		String out = "  ";
		idx++;

		status = stream.NextToken(token);
		if (status) break;
  	out << ((!first)?" ,":"  ");
		out += ToString(token);
		outs.Append(out.Consumable());
		first = False;
		if (token.type == LexToken::LexTokenType::EndOfInput) break;
	}
  // Finish it off...
	outs.Append("  ]");
  outs.Append("}");
	return outs.Consumable();

}

} // ...Namespace BEFE
