//----------------------------------------------------------------------
// File: LexLangActions.cpp - LexLanguage Default State/Action Functions
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
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// LexLanguage Default Action Handlers
//

// Action 0x00 - NUL and UTF
Status LexLangDefaultUTF(LexStream &stream, Char theChar) {
	if (!stream.lang.GetUtfInNames())
    return Error::LexUnexpectedChar;

	return LexLangDefaultName(stream, theChar);
}

// Action 0x01 - NewLine
Status LexLangDefaultNewLine(LexStream &stream, Char theChar) {
  
  Status status;
  
  if (stream.curTok.type == LexToken::Unknown) {
    if (stream.numLines == UNaN)
      stream.numLines = 0;
    stream.numLines++;
    stream.nonWhiteCount = 0;
    // NOTE: Make the next line 'false' if we don't want to keep the
    //       line numbers hanging around anywhere (it's quite a bit
    //       faster that way...
    if (false) {
      status = stream.lineOffsets.Append(stream.curTok.span.idx2+1);
      if (status) goto SOMEERROR;
    }
    stream.curTok.type      = LexToken::EndOfLine;
    stream.curTok.lineCount = 1;
    stream.curTok.value.Reset();
    stream.curTok.lineNo    = stream.curLineNo;
    stream.curTok.colNo     = stream.curColNo;
    LexStreamCONSUMEX(stream);
    stream.curState = Lex::StateMain;
  }

  // Handle errors
  status = Error::LexExitToken;
  
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

// Action 0x02 - Whitespace
Status LexLangDefaultWhiteSpace(LexStream &stream, Char theChar) {

  Status status;

  status = Error::LexExitToken;
  if (stream.curTok.type == LexToken::Unknown) {
    stream.curTok.type = LexToken::WhiteSpace;
    stream.curTok.lineCount = 0;
    stream.curTok.value.Reset();
    stream.curTok.lineNo = stream.curLineNo;
    stream.curTok.colNo  = stream.curColNo;
    LexStreamCONSUMEX(stream);
    status = Error::None;
  }
  else if (stream.curTok.type == LexToken::WhiteSpace) {
    LexStreamCONSUMEX(stream);
    status = Error::None;
  }

  return status;
  
}

// Action 0x03 - Name
Status LexLangDefaultName(LexStream &stream, Char theChar) {

  Status status;

  status = Error::LexExitToken;
  if (stream.curTok.type == LexToken::Unknown) {
    stream.curTok.type      = LexToken::Name;
    stream.curTok.lineCount = 0;
    stream.curTok.value     = String(theChar);
    stream.curTok.lineNo    = stream.curLineNo;
    stream.curTok.colNo     = stream.curColNo;
    LexStreamCONSUMEX(stream);
    stream.curState = Lex::StateName;
    status = Error::None;
  }

  return status;
  
}

// Action 0x04 - Number
Status LexLangDefaultNumber(LexStream &stream, Char theChar) {

  Status status;
  Int    stateNo;
  
  status = Error::LexExitToken;
  if (stream.curTok.type == LexToken::Unknown) {
    stateNo = stream.lang.GetStateIndex("Number");
    stream.curState = stateNo;
    stream.curTok.type      = LexToken::Number;
    stream.curTok.lineCount = 0;
    stream.curTok.value     = theChar;
    stream.curTok.lineNo    = stream.curLineNo;
    stream.curTok.colNo     = stream.curColNo;
    LexStreamCONSUMEX(stream);
    status = Error::None;
  }
  else if (stream.curTok.type == LexToken::Number) {
    LexStreamCONSUMEX(stream);
    status = Error::None;
  }

  return status;
  
}

// Action 0x05 - Quote Action
Status LexLangDefaultQuote(LexStream &stream, Char theChar) {
  return Error::LexActionNotRegistered;
}

// Action 0x06 - Single Byte operator
Status LexLangDefaultOperator(LexStream &stream, Char theChar) {

  if (stream.curTok.type == LexToken::Unknown) {
    stream.curTok.type      = LexToken::Operator;
    stream.curTok.lineCount = 0;
    stream.curTok.value     = String(theChar);
    stream.curTok.lineNo    = stream.curLineNo;
    stream.curTok.colNo     = stream.curColNo;
    LexStreamCONSUMEX(stream);
    stream.curState = Lex::StateMain;
   }
  
  return Error::LexExitToken;

}

// Action 0x07 - Append and set Comment
Status LexLangDefaultComment(LexStream &stream, Char theChar) {

  // DEBUG...
  if (gShowDebug)
  	Cout << "LexLangDefaultComment: theChar = '" << String(theChar).Escape() << "', cmtDepth=" << stream.cmtDepth << '\n';
  // ...DEBUG

  if (stream.curTok.type == LexToken::Unknown) {
    stream.curTok.type      = LexToken::Comment;
    stream.curTok.lineCount = 0;
    stream.curTok.value     = String(theChar);
    stream.curTok.lineNo    = stream.curLineNo;
    stream.curTok.colNo     = stream.curColNo;
    LexStreamCONSUMEX(stream);
   }
  
  return Error::LexExitToken;

}

// Action 0x08 - Set Preprocessor, Append to NewLine
Status LexLangDefaultPreprocessor(LexStream &stream, Char theChar) {
  return Error::LexActionNotRegistered;
}

// Action 0x09 - Left Bracket
Status LexLangDefaultBracketLeft(LexStream &stream, Char theChar) {

  if (stream.curTok.type == LexToken::Unknown) {
    stream.curTok.type      = LexToken::BracketLeft;
    stream.curTok.lineCount = 0;
    stream.curTok.value     = String(theChar);
    stream.curTok.lineNo    = stream.curLineNo;
    stream.curTok.colNo     = stream.curColNo;
    LexStreamCONSUMEX(stream);
    stream.curState = Lex::StateMain;
   }
  
  return Error::LexExitToken;

}

// Action 0x0A - Right Bracket
Status LexLangDefaultBracketRight(LexStream &stream, Char theChar) {

  if (stream.curTok.type == LexToken::Unknown) {
    stream.curTok.type      = LexToken::BracketRight;
    stream.curTok.lineCount = 0;
    stream.curTok.value     = String(theChar);
    stream.curTok.lineNo    = stream.curLineNo;
    stream.curTok.colNo     = stream.curColNo;
    LexStreamCONSUMEX(stream);
    stream.curState = Lex::StateMain;
   }
  
  return Error::LexExitToken;

}

// Action 0x0B - Flush, start 'Separator' Token, enter Main State
Status LexLangDefaultSeparator(LexStream &stream, Char theChar) {

  if (stream.curTok.type == LexToken::Unknown) {
    stream.curTok.type      = LexToken::Separator;
    stream.curTok.lineCount = 0;
    stream.curTok.value     = String(theChar);
    stream.curTok.lineNo    = stream.curLineNo;
    stream.curTok.colNo     = stream.curColNo;
    LexStreamCONSUMEX(stream);
    stream.curState = Lex::StateMain;
   }
  
  return Error::LexExitToken;

}

// Action 0x0C - Escape processing (typically '\' in string literal)
Status LexLangDefaultEscape(LexStream &stream, Char theChar) {
  LexStreamCONSUMEX(stream);
  return Error::None;
}

// Action 0x0D - Flush, switch to Main State (and process Byte)
Status LexLangDefaultFlushMain(LexStream &stream, Char theChar) {

  // DEBUG...
  if (gShowDebug && gVeryVeryVerbose)
  	Cout << "LexLangDefaultFlushMain: theChar = '" << String(theChar).Escape() << "', cmtDepth=" << stream.cmtDepth << '\n';
  // ...DEBUG

  stream.curState = Lex::StateMain;
  return Error::LexExitToken;
}

// Action 0x0E - Append (keep same State)
Status LexLangDefaultAppend(LexStream &stream, Char theChar) {

  UInt valSize;

  // DEBUG...
  if (gShowDebug && gVeryVeryVerbose)
  	Cout << "LexLangDefaultAppend: theChar = '" << String(theChar).Escape() << "', cmtDepth=" << stream.cmtDepth << '\n';
  // ...DEBUG

  valSize = stream.curTok.value.Size();
  if (valSize == 0 || ((UInt)theChar != 0 && (valSize + UTF8::GetByteSize(theChar)) <= 4) )
    stream.curTok.value.Append(theChar);
  LexStreamCONSUMEX(stream);

  return Error::None;

}

// Action 0x0F - Append,  Flush, Switch to Main State
Status LexLangDefaultAppendFlushMain(LexStream &stream, Char theChar) {

  UInt valSize;

  // DEBUG...
  if (gShowDebug && gVeryVeryVerbose)
  	Cout << "LexLangDefaultAppendFlushMain: theChar = '" << String(theChar).Escape() << "', cmtDepth=" << stream.cmtDepth << '\n';
  // ...DEBUG

  stream.curState = Lex::StateMain;
  valSize = stream.curTok.value.Size();
  if (valSize == 0 || ((UInt)theChar != 0 && (valSize + UTF8::GetByteSize(theChar)) <= 4) )
    stream.curTok.value.Append(theChar);
  LexStreamCONSUMEX(stream);

  return Error::LexExitToken;

}

//----------------------------------------------------------------------
//
// Additional Action Functions
//

Status LexActionCppQuote(LexStream &stream, Char theChar) {
  
  Status status;
  Int    stateIdx;
  UInt   valSize;
  
  status = Error::None;

  if (stream.curTok.type == LexToken::DLiteral) {
    LexStreamCONSUMEX(stream);
  }

  else if (stream.curTok.type == LexToken::Unknown) {
    if (theChar == '\'')
      stateIdx = stream.lang.GetStateIndex("SLiteral");
    else if (theChar == '\"')
      stateIdx = stream.lang.GetStateIndex("DLiteral");
    else
    	stateIdx = stream.lang.GetStateIndex("BLiteral");
    if (BEFE::IsNull(stateIdx))
      status = Error::LexStateNotExist;
    else {
      stream.curState = (UInt)stateIdx;
      if (theChar == '\'')
        stream.curTok.type = LexToken::SLiteral;
      else if (theChar == '\"')
        stream.curTok.type = LexToken::DLiteral;
      else if (theChar == '`')
      	stream.curTok.type = LexToken::BLiteral;
      valSize = stream.curTok.value.Size();
      if (valSize == 0 || ((UInt)theChar != 0 && (valSize + UTF8::GetByteSize(theChar)) <= 4) )
        stream.curTok.value.Append(theChar);
      stream.curTok.lineCount = 0;
      stream.curTok.lineNo    = stream.curLineNo;
      stream.curTok.colNo     = stream.curColNo;
      LexStreamCONSUMEX(stream);
    }
  }

  else
    status = Error::LexExitToken;
    
  return status;
  
}
  
Status LexActionCheckQuote(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionCheckEscape(LexStream &stream, Char theChar) {
  
  Status status;
  Byte   nextByte;
  
  nextByte = stream.curByte[1];
  if (nextByte == '\'' || nextByte == '"' || nextByte == '`') {
    LexStreamCONSUMEX(stream);
    if (stream.bufTrig == 0) {
      status = stream._PopulateBuffer();
      if (status) goto SOMEERROR;
    }
  }
  LexStreamCONSUMEX(stream);

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

// C++ compatible comments...
Status LexActionCppComment(LexStream &stream, Char theChar) {

  Status status;
  Int    stateIdx;
  Char   prevChar;

  // Get previous Char...
  prevChar = stream.curTok.value[0];
  
  // DEBUG...
  if (gShowDebug && gVeryVerbose)
  	Cout << "LexActionCppComment: theChar = '" << String(theChar).Escape() << "', prevChar = '" << prevChar << "', cmtDepth=" << stream.cmtDepth << '\n';
  // ...DEBUG

  status = Error::LexExitToken;

  if (stream.curTok.type == LexToken::Comment) {
    LexStreamCONSUMEX(stream);
    status = Error::None;
  }
  else if (stream.curTok.type == LexToken::Unknown ||
           stream.curTok.type == LexToken::Operator) {
    if (stream.curByte[0] == '*') {
    	// Handle comment counting...
    	stream.cmtCounting = (prevChar == '\\')?true:false;
  		stream.cmtDepth = 1;

    	// Get on with it...
      stateIdx = stream.lang.GetStateIndex("WaitForEOC");
      if (BEFE::IsNull(stateIdx))
        status = Error::LexStateNotExist;
      else {
        stream.curState         = (UInt)stateIdx;
        stream.curTok.type      = LexToken::Comment;
        stream.curTok.lineCount = 0;
        stream.curTok.value.Append(theChar);
        // Note: Commented the next two lines because they "really should be there"
        //       for cleanliness reasons but they stomp on the proper line/col already
        //       in stream.curTok...
        //stream.curTok.lineNo    = stream.curLineNo;
        //stream.curTok.colNo     = stream.curColNo;
        LexStreamCONSUMEX(stream);
        status = Error::None;
      }
    }
    else { // It's a EOL comment ('//', etc.)...
      stateIdx = stream.lang.GetStateIndex("WaitForEOL");
      if (BEFE::IsNull(stateIdx))
        status = Error::LexStateNotExist;
      else {
        stream.curState = (UInt)stateIdx;
        status = Error::LexExitToken;
        stream.curTok.type      = LexToken::Comment;
        stream.curTok.lineCount = 0;
        stream.curTok.value.Append(theChar);
        stream.curTok.lineNo    = stream.curLineNo;
        stream.curTok.colNo     = stream.curColNo;
        LexStreamCONSUMEX(stream);
      }
      status = Error::None;
    } // ...It's a '//'
  }

  return status;
  
}

Status LexActionCppPreprocessor(LexStream &stream, Char theChar) {
  
  Status status;
  Int    stateIdx;
  
  status = Error::LexExitToken;

  if (stream.curTok.type == LexToken::Unknown) {
    stateIdx = stream.lang.GetStateIndex("WaitForEOL");
    if (BEFE::IsNull(stateIdx))
      status = Error::LexStateNotExist;
    else {
      stream.curState = (UInt)stateIdx;
      status = Error::LexExitToken;
      stream.curTok.type      = LexToken::Preprocessor;
      stream.curTok.lineCount = 0;
      stream.curTok.value     = String(theChar);
      stream.curTok.lineNo    = stream.curLineNo;
      stream.curTok.colNo     = stream.curColNo;
      LexStreamCONSUMEX(stream);
    }
    status = Error::None;
  }
    
  return status;
  
}

Status LexActionWaitForEOL(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionCheckForEOC(LexStream &stream, Char theChar) {
  
  Status status;
  Byte   nextByte;
  Byte   prevByte;

  status = Error::None;

  nextByte = stream.curByte[1];
  if (stream.curByte != stream.buf)
  	prevByte = stream.curByte[-1];
  else
  	prevByte = 0;

  // DEBUG...
  if (gShowDebug)
  	Cout << "LexActionCheckForEOC: theChar = '" << String(theChar).Escape() << "', cmtCounting = " << ((stream.cmtCounting)?"true":"false") << ", cmtDepth=" << stream.cmtDepth << '\n';
  // ...DEBUG


  // Handle "*/"...
  if (theChar == '*') {
    // If counting and have another "/*", increase the count...
    if (stream.cmtCounting && (prevByte == '/' || prevByte == '\\'))
    	stream.cmtDepth++;
    else {
			if (nextByte == '/') {
				stream.cmtDepth--;
				LexStreamCONSUMEX(stream);
				if (stream.bufTrig == 0) {
					status = stream._PopulateBuffer();
					if (status) goto SOMEERROR;
				}
				if (!stream.cmtCounting || stream.cmtDepth == 0) {
					stream.curState = Lex::StateMain;
					status = Error::LexExitToken;
				}
			}
    }
  }
  LexStreamCONSUMEX(stream);

  // Handle errors
  while (false) {
    SOMEERROR: break;
  }

  return status;
  
}

Status LexActionEOCNewLine(LexStream &stream, Char theChar) {
  
  Status status;
  
  // DEBUG...
  if (gShowDebug)
  	Cout << "LexActionEOCNewLine: theChar = '" << String(theChar).Escape() << "', cmtDepth=" << stream.cmtDepth << '\n';
  // ...DEBUG

  if (stream.numLines == UNaN)
    stream.numLines = 0;
  stream.numLines++;
  status = stream.lineOffsets.Append(stream.curTok.span.idx2+1);
  if (status) goto SOMEERROR;
  LexStreamCONSUMEX(stream);
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
    
}

Status LexActionOpSlash(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionOpStar(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionOpPlus(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionOpHyphen(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionOpLess(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionOpGreater(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionOpEqual(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionOpPling(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionOpAmper(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionOpOr(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionOpColon(LexStream &stream, Char theChar) {
  return Error::NotImplemented;
}

Status LexActionBracketLeft(LexStream &stream, Char theChar) {
	if (stream.curTok.type == LexToken::Unknown) {
		stream.curTok.type      = LexToken::BracketLeft;
		stream.curTok.lineCount = 0;
		stream.curTok.value     = String(theChar);
		stream.curTok.lineNo    = stream.curLineNo;
		stream.curTok.colNo     = stream.curColNo;
		LexStreamCONSUMEX(stream);
		stream.curState = Lex::StateMain;
	}
  return Error::LexExitToken;
}

Status LexActionBracketRight(LexStream &stream, Char theChar) {
	if (stream.curTok.type == LexToken::Unknown) {
		stream.curTok.type      = LexToken::BracketRight;
		stream.curTok.lineCount = 0;
		stream.curTok.value     = String(theChar);
		stream.curTok.lineNo    = stream.curLineNo;
		stream.curTok.colNo     = stream.curColNo;
		LexStreamCONSUMEX(stream);
		stream.curState = Lex::StateMain;
	}
  return Error::LexExitToken;
}  

Status LexActionOpDot(LexStream &stream, Char theChar) {

  Status status;
  Id32   stateId;
  Int    stateIdx;
  
  status = stream.curTok.value.Append(theChar);
  LexStreamCONSUMEX(stream);
  stateId = stream.lang.GetStateId("Op.");
  if (BEFE::IsNull(stateId)) goto MISSINGSTATE;
  stateIdx = stream.lang.GetStateIndex(stateId);
  if (BEFE::IsNull(stateIdx)) goto MISSINGSTATE;
  stream.curState = (UInt)stateIdx;
  
  // Handle errors
  status = Error::None;
  while (false) {
    MISSINGSTATE: status = Error::LexStreamMissingState; break;
  }
  
  return status;

}

Status LexActionOpDotDot(LexStream &stream, Char theChar) {

  Status status;
  Id32   stateId;
  Int    stateIdx;
  
  status = stream.curTok.value.Append(theChar);
  LexStreamCONSUMEX(stream);
  stateId = stream.lang.GetStateId("Op..");
  if (BEFE::IsNull(stateId)) goto MISSINGSTATE;
  stateIdx = stream.lang.GetStateIndex(stateId);
  if (BEFE::IsNull(stateIdx)) goto MISSINGSTATE;
  stream.curState = (UInt)stateIdx;
  
  // Handle errors
  status = Error::None;
  while (false) {
    MISSINGSTATE: status = Error::LexStreamMissingState; break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Function: LexActionQuote - Encountered a Python-like quote
//

Status LexActionQuote(LexStream &stream, Char theChar) {
  
  Status status;
  Int    stateIdx;
  UInt   numConsume;
  
  status = Error::None;

  //
  // If not in "Null" state...
  //
  
  if (stream.curTok.type != LexToken::Unknown) {
    
    // Terminate current token first
    status = Error::LexExitToken;
    
  }
  
  //
  // In "Null" state, enter some appropriate quote state...
  //
  
  else { // In "Null" state...

    numConsume = 1;
    if (stream.curByte[1] == stream.curByte[0] && stream.curByte[2] == stream.curByte[0]) {
      stateIdx = stream.lang.GetStateIndex("MultiQuote");
      numConsume = 3;
    }
    else if (theChar == '\'')
      stateIdx = stream.lang.GetStateIndex("SLiteral");
    else if (theChar == '"')
      stateIdx = stream.lang.GetStateIndex("DLiteral");
    else
    	stateIdx = stream.lang.GetStateIndex("BLiteral");
        
    if (!BEFE::IsNull(stateIdx)) { // Found the LexState...

      stream.curState = (UInt)stateIdx;
      if (theChar == '\'')
        stream.curTok.type = LexToken::SLiteral;
      else if (theChar == '\"')
        stream.curTok.type = LexToken::DLiteral;
      else
      	stream.curTok.type = LexToken::BLiteral;
      if (numConsume > 1)
        stream.curTok.type = LexToken::MultiQuote;
      stream.curTok.lineCount  = 0;
      stream.curTok.lineNo     = stream.curLineNo;
      stream.curTok.colNo      = stream.curColNo;
      
      // Consume the lead characters...
      stream.curTok.value.Reset();
      while (numConsume--) {
        stream.curTok.value.Append(theChar);
        LexStreamCONSUMEX(stream);
      }
      
    } // ...Found the LexState

    else // Misconfigured State Tables...

      status = Error::LexStateNotExist;

  } // ...In "Null" state
  
  return status;
  
}
  
//----------------------------------------------------------------------
//
// Function: LexActionCheckEOQ - LexState "MultiQuote" Character handler
//

Status LexActionCheckEOQ(LexStream &stream, Char theChar) {
  
  Status status;
  
  if (theChar == stream.curTok.value.Get(0) &&
      stream.curByte[1] == stream.curByte[0] &&
      stream.curByte[2] == stream.curByte[0]) { // Found matching ''' or """...
    
    LexStreamCONSUMEX(stream);
    LexStreamCONSUMEX(stream);
    LexStreamCONSUMEX(stream);

    stream.curState = Lex::StateMain;
    
    status = Error::LexExitToken;
    
  } // ...Found matching ''' or """
  
  else if (theChar == '\\') { // It's an '\' (escape) character...
  
    LexStreamCONSUMEX(stream);
    LexStreamCONSUMEX(stream);
    status = Error::None;
    
  }
  
  else { // Else, just consume it...
 
    LexStreamCONSUMEX(stream);
    status = Error::None;
  
  } // ...Else, just consume it
  
  return status;

}

} // ...Namespace BEFE
