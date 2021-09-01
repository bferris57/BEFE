//----------------------------------------------------------------------
// File: LexStream.h - Declarations for the BEFE LexStream Class
//----------------------------------------------------------------------
//
// This class is a <Foundation Class> for BEFE Lexical processing.
//
// A LexStream is a IO Stream that takes source code, in character form
// as input and produces a Stream of LexTokens as output.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LEXSTREAM_H // LEXSTREAM_H...
#define LEXSTREAM_H

#include "Primitives.h"
#include "Lex.h"
#include "LexLanguage.h"
#include "LexToken.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Struct LexStream...
//

struct LexStream {
	
  // Enum for LexStream Input Types...
  enum : Byte {
    InputTypeNone,
    InputTypeFile,
    InputTypeString,
    NumInputTypes
  };
  
	// C++ Lifecycle
  //   Implemented in LexStream.cpp
	BEFE_inline LexStream()                                 {StartUp();}
  BEFE_inline LexStream(LexStream const &that)            {StartUp(); CopyFrom(that);}
	BEFE_inline ~LexStream()                                {ShutDown();}
  BEFE_inline LexStream &operator=(LexStream const &that) {CopyFrom(that); return *this;}
	
	// BEFE Lifecycle
  //   Implemented in LexStream.cpp
  Status     StartUp();
  Status     ShutDown();
  Status     Clear();
  Status     Reset();
  Boolean    IsNull() const;
  Status     SetNull();
  Boolean    IsEmpty();
  Status     SetEmpty();
  Status     MoveFrom(LexStream const &that);
  Status     CopyFrom(LexStream const &that);
	Boolean    IsConsumable() const;
  LexStream &Consumable() const;
  LexStream &NotConsumable() const;

  //
  // LexStream functionality
  //

  // Lex Attachment
  //   Implemented in LexStreamLex.cpp
  Boolean IsAttached() const;	
  Status  _Attach(Lex const &theLex);
  Status  _Detach();

  // Input Settings
  Status  SetLanguageName(String const &name);  // Set the input language name
  Status  SetInputFileName(String const &name); // Set the input file name
  Status  SetInputString(String const &str);    // Set the input string
  UInt    GetInputSize();                       // Get the input size (UNaN if not parsing)
	Status  GetInputSize(UInt &size);             // Get the input size (UNaN if not parsing)
  Status  CountInputLines(UInt &count);         // Get the input '\n' count
  
  // Buffer Management
  Status  ResetBuffer();                        // Reset/Initialise/Allocate buffer
  Status  SetInputPosition(UInt pos);           // Position to given location in input file
  Status  _AllocateBuf();                       // ◄── Internal: Allocate buffer if not already allocated
  
  // Parsing Internal
  Status  _StartParsing();                      // ◄── Internal: Start Parsing (validation, init, etc.)
  Status  _FinishParsing();                     // ◄── Internal: Finish Parsing (clean up)
  Status  _PopulateBuffer();                    // ◄── Internal: Populate Buffer if need be (see bufTrig)

  // Parsing Query
  UInt    LineToOffset(UInt lineNo);            // Convert a line number to a input Offset (0..inputSize-1)
  UInt    OffsetToLine(UInt inputOffset);       // Convert a file offset to a line number (0..numLines-1)
  Status  OffsetToLineCol(UInt inputOffset, UInt &lineNo, UInt &colNo);
  String  GetInputSpan(Span span);
  Status  GetInputSpan(Span span, String &value);

  // Single Character handling
  Status  _Consume();                           // Consume current character
  
  // Parsing
  Status  NextToken(LexToken &theToken);        // Get the next Input LexToken
  
  //
  // Members
  //
	
  Boolean      isConsumable:1;
  Boolean      isNull:1;
  Boolean      isParsing:1;
  Byte         inputType;
  
  Lex         *lex;             // The Lex we're attached to (if any)
  
  // Input Buffer
  String       languageName;    // Language Name ("C++", etc.)
  String       inputFileName;   // Input file name
  String       inputString;     // Input String

  // Main Parsing State
  LexLanguage  lang;            // Copy of the Lex's language (for read consistency)
  UInt         contigMax;       // Denormalised copy of lang.contigMax
  LexState    *states;          // Denormalised pointer to Array of language LexStates
  UInt         numStates;       // Number of LexStates
  LexAction   *actions;         // Denormalised pointer to Array of language LexActions
  UInt         numActions;      // Number of LexActions
  Id           fileId;          // Cache FileId
  UInt         inputSize;       // Input Size (in Bytes)
  UInt         numLines;        // Number of lines so far
  UInts        lineOffsets;     // Array of file Line offsets
  UInt         nonWhiteCount;   // Count of non-Whitespace on current line (including curTok)
  UInt         curState;        // Current State (index)
  LexToken     curTok;          // Current Token we're working on
  UInt         blkSize;         // Number of bytes in each input block
  UInt         bufSize;         // Number of bytes in allocated buffer (should be blkSize+lang.contigMax)
  UInt         bufBlkNo;        // Current File block number (1st block in buffer)
  Byte        *buf;             // Double block Buffer
  Byte        *curByte;         // Current (next) byte in buffer
  Char         curChar;         // Current (next) character in buffer
  UInt         curCharLen;      // Current (next) character length in bytes
  UInt         curOffset;       // Current (next) character Stream offset in bytes
  UInt         curLineStart;    // Current Line start offset
  UInt         curLineNo;       // Current (next) Input Line Number (0..)
  UInt         curColNo;        // Current (next) Input Line Column Number (0..)
  UInt         curLineLen;      // Current Input Line length (up to next Token)
  UInt         bufUsed;         // Number of input bytes used in buffer
  UInt         bufRem;          // Remaining used bytes in buffer
  UInt         bufTrig;         // Remaining used bytes in buffer before "load next" trigger
  Boolean      cmtCounting:1;   // Counting comment depth?
  UInt         cmtDepth;        // Comment depth (if counting)
  
};

//----------------------------------------------------------------------
//
// Handy Macros (yuck)
//

// For use in LexStream method code...
#define LexStreamCONSUME      \
  status = _Consume();        \
  if (status) goto SOMEERROR;

// For use outside LexStream method code...
#define LexStreamCONSUMEX(s) { \
  Status status;               \
  status = s._Consume();       \
  if (status) return status;   \
  }

} // ...Namespace BEFE

#endif // ...LEXSTREAM_H
