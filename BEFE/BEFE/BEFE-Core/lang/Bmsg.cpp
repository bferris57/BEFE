//----------------------------------------------------------------------
// File: Bmsg.h - Implementation of the Bmsg Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Bmsg.h"
#include "LexLanguage.h"
#include "LexLang.h"
#include "LexFormat.h"
#include "IntFuncs.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// BEFE Lifecycle...
//

Status Bmsg::StartUp() {

  Status retStatus;  
  Status status;
  
  retStatus = Blang::StartUp();

  status = ranges.StartUp();
  if (status && !retStatus) retStatus = status;
  status = messages.StartUp();
  if (status && !retStatus) retStatus = status;
  status = msgIdIndexes.StartUp();
  if (status && !retStatus) retStatus = status;
  status = msgNameIndexes.StartUp();
  if (status && !retStatus) retStatus = status;
  status = errors.StartUp();
  if (status && !retStatus) retStatus = status;
  status = warnings.StartUp();
  if (status && !retStatus) retStatus = status;

  BEFE::SetNull(numRanges);
  BEFE::SetNull(numMessages);
  BEFE::SetNull(numWarnings);
  BEFE::SetNull(numErrors);
  
  status = nextBmsgToken.StartUp();
  if (status && !retStatus) retStatus = status;
    
  return retStatus;
  
}

Status Bmsg::ShutDown() {

  Status retStatus;  
  Status status;
  
  retStatus = Blang::ShutDown();

  status = ranges.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = messages.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = msgIdIndexes.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = msgNameIndexes.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = errors.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = warnings.ShutDown();
  if (status && !retStatus) retStatus = status;

  status = nextBmsgToken.ShutDown();
  if (status && !retStatus) retStatus = status;

  return retStatus;
  
}

Status Bmsg::Reset() {

  Status retStatus;
  Status status;

  retStatus = ShutDown();
  status    = StartUp();

  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status Bmsg::MoveFrom(Bmsg const &that) {
  
  Status retStatus;
  Status status;

  if ((Bmsg *)&that == this)
    retStatus = Error::None;
  else {
    retStatus = ShutDown();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(Bmsg));
    status = ((Bmsg *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
  }
  NotConsumable();
  
  return retStatus;
  
}

Status Bmsg::CopyFrom(Bmsg const &that) {

  Status retStatus;  
  Status status;
  
  if (this == (Bmsg *)&that)
    retStatus = Error::None;
  else {
    if (IsConsumable())
      retStatus = MoveFrom(that);
    else {

      isConsumable = false;
      isStarted    = that.isStarted;
      isAtEnd      = that.isAtEnd;
      skipToEOL    = that.skipToEOL;
      
      retStatus = Blang::CopyFrom(that);

      status = ranges.CopyFrom(that.ranges);
      if (status && !retStatus) retStatus = status;
      status = messages.CopyFrom(that.messages);
      if (status && !retStatus) retStatus = status;
      status = msgIdIndexes.CopyFrom(that.msgIdIndexes);
      if (status && !retStatus) retStatus = status;
      status = msgNameIndexes.CopyFrom(that.msgNameIndexes);
      if (status && !retStatus) retStatus = status;
      status = errors.CopyFrom(that.errors);
      if (status && !retStatus) retStatus = status;
      status = warnings.CopyFrom(that.errors);
      if (status && !retStatus) retStatus = status;

      status = nextBmsgToken.CopyFrom(that.nextBmsgToken);
      if (status && !retStatus) retStatus = status;
      
    }

  }
  
  return retStatus;
  
}

Boolean Bmsg::IsNull() const {
  return (
    Blang::IsNull()         &&
    ranges.IsNull()         &&
    messages.IsNull()       &&
    msgIdIndexes.IsNull()   &&
    msgNameIndexes.IsNull() &&
    nextBmsgToken.IsNull()  &&
    errors.IsNull()
  );
}

Status Bmsg::SetNull() {
  return Reset();
}

Boolean Bmsg::IsEmpty() const {
  return IsNull();
}

Status Bmsg::SetEmpty() {
  return Reset();
}

UInt Bmsg::Length() const {
  return 0;
}

UInt Bmsg::Size() const {
  return 0;
}

//----------------------------------------------------------------------
//
// Operational Methods
//

Status Bmsg::ProcessFile(String const &pFileName) {
  
  Status       status;
  BmsgLine     line;
  LexFormatOut fmt;
  UInt         errNo;
  BmsgToken    err;
  String       msg;
  
  inputFileName = pFileName;  
  
  // Pass 1: Process the lines to get array sizes...
  status = _StartParsing();
  if (status) goto SOMEERROR;
  status = _ProcessLines(false);
  if (status) goto SOMEERROR;
  status = _StopParsing();
  if (status) goto SOMEERROR;
  
  // Pass 2: Process the lines to allocate correctly...
  status = _StartParsing();
  if (status) goto SOMEERROR;
  status = _ProcessLines(true);
  if (status) goto SOMEERROR;
  status = _StopParsing();
  if (status) goto SOMEERROR;
  
  // Display any errors and warnings...
  if (gShowWarnings) {
    for (errNo=0; errNo < numErrors; errNo++) {
      err = errors.Get(errNo);
      msg = String("File '") << pFileName
                             << "' (" << err.lineNo+1
                             << '/' << err.colNo+1
                             << "): " << err.value;
      BEFE_WARN(msg);
    }
      msg = String("File '") << pFileName
                             << "' (" << err.lineNo+1
                             << '/' << err.colNo+1
                             << ": " << err.value;
    for (errNo=0; errNo < numWarnings; errNo++) {
      err = warnings.Get(errNo);
      msg = String("File '") << pFileName
                             << "' (" << err.lineNo+1
                             << '/' << err.colNo+1
                             << "): " << err.value;
      BEFE_WARN(msg);
    }
  }

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  if (status)
    _StopParsing();
    
  return status;
  
}

Status Bmsg::GetCounts(UInt &rangeCount,
                       UInt &msgCount,
                       UInt &errorCount,
                       UInt &warningCount) {

  rangeCount   = ranges.Length();
  msgCount     = messages.Length();
  errorCount   = errors.Length();
  warningCount = warnings.Length();
  
  return Error::None;
  
}

Status Bmsg::GetErrors(BmsgTokens &theErrors) {  
  Status status;
  status = theErrors.CopyFrom(errors);
  return status;
}

Status Bmsg::TakeErrors(BmsgTokens &theErrors) {
  Status status;
  status = theErrors.MoveFrom(errors);
  return status;
}

Status Bmsg::GetWarnings(BmsgTokens &theWarnings) {
  Status status;
  status = theWarnings.CopyFrom(warnings);
  return status;
}

Status Bmsg::TakeWarnings(BmsgTokens &theWarnings) {
  Status status;
  status = theWarnings.MoveFrom(warnings);
  return status;
}
  
Status Bmsg::GetResults(MessageRanges &theRngArray,
                        Messages      &theMsgArray,
                        MessageIds    &theIdIndexes,
                        MessageNames  &theNameIndexes) {

  Status retStatus;
  Status status;

  // Reset first...  
  theRngArray.Reset();
  theMsgArray.Reset();
  theIdIndexes.Reset();
  theNameIndexes.Reset();

  // Copy 'em...
  retStatus = theRngArray.CopyFrom(ranges);
  status    = theMsgArray.CopyFrom(messages);
  if (status && !retStatus) retStatus = status;
  status    = theIdIndexes.CopyFrom(msgIdIndexes);
  if (status && !retStatus) retStatus = status;
  status    = theNameIndexes.CopyFrom(msgNameIndexes);
  if (status && !retStatus) retStatus = status;

  return retStatus;
  
}

Status Bmsg::TakeResults(MessageRanges &theRngArray,
                         Messages      &theMsgArray,
                         MessageIds    &theIdIndexes,
                         MessageNames  &theNameIndexes) {

  Status retStatus;
  Status status;

  // Reset first...  
  theRngArray.Reset();
  theMsgArray.Reset();
  theIdIndexes.Reset();
  theNameIndexes.Reset();

  // Move 'em...
  retStatus = theRngArray.MoveFrom(ranges);
  status    = theMsgArray.MoveFrom(messages);
  if (status && !retStatus) retStatus = status;
  status    = theIdIndexes.MoveFrom(msgIdIndexes);
  if (status && !retStatus) retStatus = status;
  status    = theNameIndexes.MoveFrom(msgNameIndexes);
  if (status && !retStatus) retStatus = status;

  return retStatus;
  
}
                         
//----------------------------------------------------------------------
//
// Low level Under-the-cover Methods
//

Status Bmsg::_StartParsing() { // Bmsg._StartParsing...
  
  Status status;
  
  // Nothing to do if already parsing...
  if (isStarted) goto OK;

  // Some sanity checks
  if (BEFE::IsNull(TheBefe)) goto NOBEFE;
  
  // Reset stuff first...
  status = ranges.Reset();
  if (status) goto SOMEERROR;
  status = messages.Reset();
  if (status) goto SOMEERROR;
  status = msgIdIndexes.Reset();
  if (status) goto SOMEERROR;
  status = msgNameIndexes.Reset();
  if (status) goto SOMEERROR;
  status = errors.Reset();
  if (status) goto SOMEERROR;
  status = warnings.Reset();
  if (status) goto SOMEERROR;

  // Let Blang do most of it...
  status = Blang::_StartParsing();
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    OK:        status = Error::None;   break;
    NOBEFE:    status = Error::NoBefe; break;
    SOMEERROR:                         break;
  }
  
  return status;
  
} // ...Bmsg._StartParsing

Status Bmsg::_StopParsing() {

  Status retStatus;
  Status status;
  
  retStatus = Error::None;

  if (isStarted) {
  
    status = Blang::_StopParsing();
    if (status && !retStatus) retStatus = status;
    
    status = nextBmsgToken.Reset();
    if (status && !retStatus) retStatus = status;
    
    if (!retStatus)
      isStarted = false;
      
  }
  
  return retStatus;
  
}

Status Bmsg::_NextBmsgToken(BmsgToken &token) { // Bmsg._NextBmsgToken...

  Status  status;
  Int32   anInt32;
  Byte    tokType;
  UInt    lfCount;
  
  status = token.Reset();
  if (status) goto SOMEERROR;
  
  // If no curLineNo, always start at zero...
  if (BEFE::IsNull(curLineNo))
    curLineNo = 0;
    
  // If we're already at end, return EndOfStream token...
  if (isAtEnd) {
    token.type   = BmsgToken::TypeEndOfStream;
    token.lineNo = curLineNo;
    token.colNo  = token0->span.idx1 - (Int)curLineStart;
    goto OK;
  }
  
  //
  // Until we can form a proper BmsgToken...
  //
  
  do {
    
    // Make sure the LexTokens are up to date...
    status = _GetLexTokens();
    if (status) goto SOMEERROR;
    if (BEFE::IsNull(token0)) goto NULLPOINTER;
    
    // Handle EndOfInput...
    if (token0[curTokNo].type == LexToken::EndOfInput) {
      token.type   = BmsgToken::TypeEndOfStream;
      isAtEnd = true;
      goto OK;
    }
    
    // Ignore if not EndOfLine and we're skipping...
    if (skipToEOL && token0[curTokNo].type != LexToken::EndOfLine) {
      curTokNo++;
      continue;
    }
      
    // Initialise this token's lineNo/colNo
    if (BEFE::IsNull(curLineStart))
      curLineStart = token0[curTokNo].span.idx1;
    token.lineNo = curLineNo;
    token.colNo  = token0[curTokNo].span.idx1 - curLineStart;

    // Handle comments (in case they contain '\n')...
    tokType = token0[curTokNo].type;
    if (tokType == LexToken::Comment) {
      if (token0[curTokNo].value.Get(Range(0,1)) == "/*") {
        status = stream.GetInputSpan(token0[curTokNo].span, token.value);
        if (status) goto SOMEERROR;
        lfCount = token.value.Count('\n');
        curLineNo += lfCount;
      }
      curTokNo++;
      continue;
    }      
     
    // Skip ones we don't want...
    if (tokType == LexToken::WhiteSpace ||
        tokType == LexToken::Preprocessor
       ) {
      curTokNo++;
      continue;
    }
    
    // Handle EndOfLine...
    if (tokType == LexToken::EndOfLine) {
      skipToEOL = false;
      curLineNo++;
      BEFE::SetNull(curLineStart);
      curTokNo++;
      continue;
    }
    
    // If this is a Name and next is '...', form it...
    if (tokType == LexToken::Name && token0[curTokNo+1].type == LexToken::Operator && token0[curTokNo+1].value == "...") {
      token.type  = BmsgToken::TypeBeginEllipsis;
      status = stream.GetInputSpan(token0[curTokNo].span, token.value);
      if (status) goto SOMEERROR;
      curTokNo += 2;
      goto OK;
    }
    
    // If this is a '...' and next is a Name, form it...
    if (token0[curTokNo].type == LexToken::Operator && token0[curTokNo].value == "..." && token0[curTokNo+1].type == LexToken::Name) {
      token.type  = BmsgToken::TypeEndEllipsis;
      token.colNo = token0[curTokNo+1].span.idx1-curLineStart;
      status = stream.GetInputSpan(token0[curTokNo+1].span, token.value);
      if (status) goto SOMEERROR;
      curTokNo += 2;
      goto OK;
    }
    
    // If this is a number...
    if (tokType == LexToken::Number) { // Number...
      
      // Build it first, assuming it's okay...
      token.type  = BmsgToken::TypeNumber;
      status = stream.GetInputSpan(token0[curTokNo].span, token.value);
      if (status) goto SOMEERROR;
      
      // Validate it
      status = Int32FromString(token.value, anInt32);
      if (status) {
        token.type = BmsgToken::TypeWarning;
        token.value = ErrorString(status);
        skipToEOL = true;
        curTokNo++;
        break;
      }
      
      // It's okay...
      token.type  = BmsgToken::TypeNumber;
      curTokNo++;
      break;
      
    } // ...Number
    
    // Okay if it's a '..'
    if (tokType == LexToken::Operator && token0[curTokNo].value == "..") {
      token.type = BmsgToken::TypeRange;
      token.value = "..";
      curTokNo++;
      break;
    }
    
    // Okay if it's a '='
    if (tokType == LexToken::Operator && token0[curTokNo].value == "=") {
      token.type = BmsgToken::TypeEquals;
      token.value = '=';
      curTokNo++;
      break;
    }
    
    // Okay if it's a ','
    if (tokType == LexToken::Separator && token0[curTokNo].value == ",") {
      token.type = BmsgToken::TypeSeparator;
      token.value = ',';
      curTokNo++;
      break;
    }
    
    // Okay if it's a 'Name'
    if (tokType == LexToken::Name) {
      token.type  = BmsgToken::TypeName;
      status = stream.GetInputSpan(token0[curTokNo].span, token.value);
      if (status) goto SOMEERROR;
      curTokNo++;
      break;
    }
    
    // If it's a String Literal...
    if (tokType == LexToken::SLiteral || tokType == LexToken::DLiteral || tokType == LexToken::BLiteral) {
      token.type = BmsgToken::TypeString;
      status = stream.GetInputSpan(token0[curTokNo].span, token.value);
      if (status) goto SOMEERROR;
      curTokNo++;
      break;
    }
    
    // Handle Error, Unexpected, and Warning tokens...
    if (tokType == LexToken::Error ||
        tokType == LexToken::Unexpected || 
        tokType == LexToken::Warning) {
      if (tokType == LexToken::Error)
        token.type = BmsgToken::TypeError;
      else if (tokType == LexToken::Warning)
        token.type = BmsgToken::TypeWarning;
      else {
        token.type  = BmsgToken::TypeError;
        token.value = ErrorString(Error::LangBefeUnexpectedCharacter);
      }
      skipToEOL = true;
      curTokNo++;
      goto OK;
    } // ...Error/Warning

    // Don't know what it is...
    token.type = BmsgToken::TypeError;
    token.value = ErrorString(Error::LangBefeUnexpectedToken);
    curTokNo++;
    skipToEOL = true;
    goto OK;
    
  } while (true);
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    OK:          status = Error::None;                break;
    SOMEERROR:                                        break;
  }
  if (!status)
    totOutTokens++;
    
  return status;
  
} // ...Bmsg._NextBmsgToken

//----------------------------------------------------------------------
//
// High Level Under-the-cover Methods
//

Status Bmsg::_NextBmsgLine(BmsgLine &line) { // Bmsg._NextBmsgLine...
  
  Status status;

  // First off, reset the passed line...
  status = line.Reset();
  if (status) goto SOMEERROR;
  
  // Get the next BmsgToken if we don't already have one...
  if (nextBmsgToken.IsNull()) {
    status = _NextBmsgToken(nextBmsgToken);
    if (status) goto SOMEERROR;
  }
  
  // If it's Null or "EndOfStream", nothing else to get
  if (nextBmsgToken.IsNull() || nextBmsgToken.type == BmsgToken::TypeEndOfStream)
    goto OK;
    
  // This is the first one, set the line info...
  line.lineNo = curLineNo;

  // Until we reach a different line...
  do {
    
    // Append the token...
    status = line.tokens.Append(nextBmsgToken);
    if (status) goto SOMEERROR;
    
    // Get the next token...
    status = _NextBmsgToken(nextBmsgToken);
    if (status) goto SOMEERROR;
    
  } while (!BEFE::IsNull(nextBmsgToken.lineNo)              &&
           nextBmsgToken.type != BmsgToken::TypeEndOfStream &&
           nextBmsgToken.lineNo == line.lineNo);
  
  line.numTokens = line.tokens.Length();
  
  // Handle errors
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;
  
} // ...Bmsgstream._NextBmsgLine

enum {
  StateNull,
  StateRanges,
  StateMessages
};

Status Bmsg::_ProcessLines(Boolean doit) {
  
  Status       status;
  BmsgLine     line;        // Current line we're processing
  UInt         state;       // Processing state (see above)
  BmsgToken    token;       // Transient: Local copy of a token
  Int          rangeIdx;    // Range's Index
  MessageRange range;       // Range we're building
  UInt         nextInRange; // Next message Id value to emit in range..
  Int          messageIdx;  // Message's Index
  Message      message;     // Message we're building
  Boolean      skipToNull;  // "Skip until after ...<name> found"
  Alloc        alloc;       // TEMP: Used for array allocating
  UInt         i;
  
  //
  // Pre-allocate the arrays if applicable...
  //
  
  if (messages.Length() == 0 && !BEFE::IsNull(numMessages) && numMessages != 0) {
    alloc = Alloc(numMessages, 16);
    status = messages.Allocate(alloc);
    if (status) goto SOMEERROR;
    status = messages.SetLength(0);
    if (status) goto SOMEERROR;
  }
  
  //
  // Process the lines...
  //
  
  state       = StateNull;
  skipToNull  = false;
  numRanges   = 0;
  numMessages = 0;
  numWarnings = 0;
  numErrors   = 0;
  
  while (true) { // Until we're finished...
  
    // Get the next line
    status = _NextBmsgLine(line);
    if (status) goto SOMEERROR;
    
    // If no line number, it's time for the next pass...
    if (BEFE::IsNull(line.lineNo))
      break;
    
    // If no tokens, nothing to do
    if (line.numTokens == 0) continue;
    
    // Denormalise line stuff before we start...
    line.token0 = (BmsgToken *)line.tokens._GetAddress(0);
    if (BEFE::IsNull(line.token0)) goto NULLPOINTER;

    // If we're skipping until ...<name>, do so...
    if (skipToNull) {
      if (line.token0->type == BmsgToken::TypeEndEllipsis)
        skipToNull = false;
      continue;
    }
    
    // Assume we've got an error on first token...
    token = *line.token0;
    token.type = BmsgToken::TypeError;
    
    //
    // StateNull - Waiting for <name>...
    //
    
    if (state == StateNull) {
      
      // If first token isn't a "...<name>", something's wrong...
      if (line.token0->type == BmsgToken::TypeEndEllipsis) {
        token.value = "Expected <name>... - Ignored";
        goto APPENDWARNING;
      }

      // If "MessageBase...", put into StateRanges state...
      line.token0->value.Insensitive();
      if (line.token0->value == "MessageBase") {
        
        state = StateRanges;
        continue;
        
      }
      
      // If "doit"...
      if (doit) { // "doit"...
        
        // Make sure the range exists...
        rangeIdx = ranges.Find(line.token0->value);
        if (rangeIdx < 0) {
          token.value = String("Message Range '") + line.token0->value + "' does not exist";
          token.value << " (Skipping input until after next ..." << line.token0->value << ")";
          skipToNull = true;
          goto APPENDERROR;
        }
      
        // Get the range and change state...
        status = ranges.Get(rangeIdx, range);
        if (status) goto SOMEERROR;
      
        // Make sure there's not already some defined here...
        for (i=0; i < messages.Length(); i++) {
          status = messages.Get(i, message);
          if (status) goto SOMEERROR;
          if (range.range.InRange(message.id)) {
            token.type = BmsgToken::TypeWarning;
            token.value = String("Range '") + line.token0->value + "' already declared (skipping)";
            status = warnings.Append(token);
            if (status) goto SOMEERROR;
            skipToNull = true;
            break;
          }
        }
     
      } // ..."doit"
 
      // Everything's okay (or we've set ourselve up),
      // so change state to StateMessages...
      state = StateMessages;
      nextInRange = range.range.idx1;
      
      continue;
      
    }
    
    //
    // StateRanges - Expecting Range specs...
    //

    else if (state == StateRanges) {
      
      // If token[0] is a ...<name>, validate and change state...
      if (line.token0[0].type == BmsgToken::TypeEndEllipsis) {
        
        // Warn if not proper end ellipsis...
        if (line.token0[0].value != "MessageBase") {
          token.type = BmsgToken::TypeWarning;
          token.value = "Expected '...MessageBase' here, entering Null state anyway";
          status = warnings.Append(token);
          if (status) goto SOMEERROR;
        }
        
        // Do it
        state = StateNull;
        continue;
        
      }
      
      // Error if token[0] isn't a Name...
      if (line.token0[0].type != BmsgToken::TypeName) {
        token.value = "Expected <rangeName> here";
        goto APPENDERROR;
      }
      
      // Error if token[1] isn't a '='...
      if (line.numTokens < 2                           ||
          line.token0[1].type != BmsgToken::TypeEquals ||
          line.token0[1].value != "=") {
        if (line.numTokens >= 2)
          token.colNo = line.token0[1].colNo;
        else
          token.colNo = line.token0[0].colNo + line.token0[0].value.Length();
        token.value = "Expected '=' here";
        goto APPENDERROR;
      }
      
      // Error if token[2] isn't a Number...
      if (line.numTokens < 3 || line.token0[2].type != BmsgToken::TypeNumber) {
        if (line.numTokens >= 3)
          token.colNo = line.token0[2].colNo;
        else
          token.colNo = line.token0[1].colNo + line.token0[1].value.Length();
        token.value = "Expected '=' here";
        goto APPENDERROR;
      }

      // Error if token[3] isn't a ".."...
      if (line.numTokens < 4 || 
          line.token0[3].type != BmsgToken::TypeRange ||
          line.token0[3].value != "..") {
        if (line.numTokens >= 4)
          token.colNo = line.token0[3].colNo;
        else
          token.colNo = line.token0[2].colNo + line.token0[2].value.Length();
        token.value = "Expected '..' here";
        goto APPENDERROR;
      }

      // Error if token[4] isn't a Number...
      if (line.numTokens < 5 || line.token0[4].type != BmsgToken::TypeNumber) {
        if (line.numTokens >= 5)
          token.colNo = line.token0[4].colNo;
        else
          token.colNo = line.token0[3].colNo + line.token0[3].value.Length();
        token.value = "Expected '=' here";
        goto APPENDERROR;
      }

      // If we've got more than 5 tokens, make sure next one is a ','
      if (line.numTokens > 5 && line.token0[5].type != BmsgToken::TypeSeparator) {
        token.colNo = line.token0[5].colNo;
        token.value = "Expected ',' here (if anything)";
        goto APPENDERROR;
      }
      
      // If we've got more than 6 tokens, make sure next one is a name or a string
      if (line.numTokens > 6                         &&
          line.token0[6].type != BmsgToken::TypeName &&
          line.token0[6].type != BmsgToken::TypeString) {
        token.colNo = line.token0[6].colNo;
        token.value = "Expected range Short Name here (if anything)";
        goto APPENDERROR;
      }

      // Warning if there's more tokens left...
      if (line.numTokens > 7) {
        token.type  = BmsgToken::TypeWarning;
        token.colNo = line.token0[7].colNo;
        token.value = "Don't know what this stuff is!!! (ignoring it)";
        if (doit) {
          status = warnings.Append(token);
          if (status) goto SOMEERROR;
        }
        numWarnings++;
        
      }
      
      // Right, the line's okay, do it...
      if (doit) {
        range.name = line.token0[0].value;
        if (line.numTokens == 7)
          range.shortName = line.token0[6].value;
        else
          range.shortName.Reset();
        status = Int32FromString(line.token0[2].value, *(Int32 *)&range.range.idx1);
        if (status) goto SOMEERROR;
        status = Int32FromString(line.token0[4].value, *(Int32 *)&range.range.idx2);
        if (status) goto SOMEERROR;
        status = ranges.Append(range);
        if (status) goto SOMEERROR;
      }
      numRanges++;
        
    }
    
    //
    // StateMessage - Expecting Message specs...
    //
    
    else {
      
      // If token[0] is a ...<name>, validate and change state...
      if (line.token0[0].type == BmsgToken::TypeEndEllipsis) {
        
        // Warn if not proper end ellipsis...
        if (line.token0[0].value != range.name) {
          token.type = BmsgToken::TypeWarning;
          token.value = String("Expected '...") + range.name + "' here, entering Null state anyway";
          if (doit) {
            status = warnings.Append(token);
            if (status) goto SOMEERROR;
          }
          numWarnings++;
        }
        
        // Do it
        state = StateNull;
        continue;
        
      }
      
      // Error if token[0] isn't a Name...
      if (line.token0[0].type != BmsgToken::TypeName) {
        token.value = "Expected <rangeName> here";
        goto APPENDERROR;
      }
      
      // Error if token[1] isn't a '='...
      if (line.numTokens < 2                           ||
          line.token0[1].type != BmsgToken::TypeEquals ||
          line.token0[1].value != "=") {
        if (line.numTokens >= 2)
          token.colNo = line.token0[1].colNo;
        else
          token.colNo = line.token0[0].colNo + line.token0[0].value.Length();
        token.value = "Expected '=' here";
        goto APPENDERROR;
      }
      
      // Error if token[2] isn't a string...
      if (line.numTokens < 3 || line.token0[2].type != BmsgToken::TypeString) {
        if (line.numTokens >= 3)
          token.colNo = line.token0[2].colNo;
        else
          token.colNo = line.token0[1].colNo + line.token0[1].value.Length();
        token.value = "Expected String literal here";
        goto APPENDERROR;
      }

      // Warning if there's more tokens left...
      if (line.numTokens > 3) {
        token.type  = BmsgToken::TypeWarning;
        token.colNo = line.token0[3].colNo;
        token.value = "Don't know what this stuff is!!! (ignoring it)";
        if (doit) {
          status = warnings.Append(token);
          if (status) goto SOMEERROR;
        }
        numWarnings++;
      }
      
      // Error if the range is full...
      if (doit && nextInRange > range.range.idx2) {
        token.type = BmsgToken::TypeError;
        token.value = "Range is full (ignoring remaining messages in this range)";
        if (doit) {
          status = errors.Append(token);
          if (status) goto SOMEERROR;
        }
        numErrors++;
        skipToNull = true;
        continue;
      }

      // All okay, just do it...
      message.id      = nextInRange++;
      message.name    = line.token0[0].value.Consumable();
      message.message = line.token0[2].value.Consumable();
      
      if (doit) {
        Char c;
        // Remove begin/end quotation marks, if any
        c = message.message.Get(0);
        if ((c == '"' || c == '\'') && message.message.Get(-1) == c) {
          message.message.Remove(0);
          message.message.Remove(-1);
        }
        // Append to message array
        messageIdx = messages.Length();
        status = messages.Append(message);
        if (status) goto SOMEERROR;
        // Update Id->Index map
        status = msgIdIndexes.Set(message.id, messageIdx);
        if (status) goto SOMEERROR;
        // Update Name->Index map
        status = msgNameIndexes.Set(message.name, messageIdx);
        if (status) goto SOMEERROR;
      }
      numMessages++;
        
      continue;
      
    }
    
    //
    // Handle generated errors...
    //

    while (false) {
      APPENDERROR:
        token.type = BmsgToken::TypeError;
        if (doit) {
          status = errors.Append(token);
          if (status) goto SOMEERROR;
        }
        numErrors++;
        break;
      APPENDWARNING:
        token.type = BmsgToken::TypeWarning;
        if (doit) {
          status = warnings.Append(token);
          if (status) goto SOMEERROR;
        }
        numWarnings++;          
        break;
    }    
    
  } // ...Until we're finished
  
  // Handle errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

} // ...Namespace BEFE
