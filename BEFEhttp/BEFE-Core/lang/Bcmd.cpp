//----------------------------------------------------------------------
// File: Bcmd.h - Implementation of the Bcmd Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Bcmd.h"
#include "BcmdArg.h"
#include "cmdutils.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class (static) methods
//

String Bcmd::GetOptionString() {
  return "+-?!";
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle...
//

Status Bcmd::StartUp() {

  Status retStatus;  
  Status status;
  
  retStatus = Blang::StartUp();

  status = continuePrompt.StartUp();
  if (status && !retStatus) retStatus = status;

  numCommands = 0;
  numArgs     = 0;
  numErrors   = 0;
  numWarnings = 0;

  status = errors.StartUp();
  if (status && !retStatus) retStatus = status;
  status = warnings.StartUp();
  if (status && !retStatus) retStatus = status;

  status = nextEOI.StartUp();
  if (status && !retStatus) retStatus = status;

  return retStatus;
  
}

Status Bcmd::ShutDown() {

  Status retStatus;  
  Status status;
  
  retStatus = Blang::ShutDown();

  status = continuePrompt.ShutDown();
  if (status && !retStatus) retStatus = status;

  status = errors.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = warnings.ShutDown();
  if (status && !retStatus) retStatus = status;

  status = nextEOI.ShutDown();
  if (status && !retStatus) retStatus = status;

  return retStatus;
  
}

Status Bcmd::Reset() {

  Status retStatus;
  Status status;

  retStatus = ShutDown();
  status    = StartUp();

  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status Bcmd::MoveFrom(Bcmd const &that) {
  
  Status retStatus;
  Status status;

  if ((Bcmd *)&that == this)
    retStatus = Error::None;
  else {
    retStatus = ShutDown();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(Bcmd));
    status = ((Bcmd *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
  }
  NotConsumable();
  
  return retStatus;
  
}

Status Bcmd::CopyFrom(Bcmd const &that) {

  Status retStatus;  
  Status status;
  
  if (this == (Bcmd *)&that)
    retStatus = Error::None;
  else {
    if (IsConsumable())
      retStatus = MoveFrom(that);
    else {
      retStatus = Blang::CopyFrom(that);
      status = continuePrompt.CopyFrom(that.continuePrompt);
      if (status && !retStatus) retStatus = status;

      numCommands = that.numCommands;
      numArgs     = that.numArgs;
      numErrors   = that.numErrors;
      numWarnings = that.numWarnings;
      status = errors.CopyFrom(that.errors);
      if (status && !retStatus) retStatus = status;
      status = warnings.CopyFrom(that.warnings);
      if (status && !retStatus) retStatus = status;
      status = nextEOI.CopyFrom(that.nextEOI);
      if (status && !retStatus) retStatus = status;
    }
  }
  
  return retStatus;
  
}

Boolean Bcmd::IsNull() const {
  return (
         Blang::IsNull()  &&
         numCommands == 0 &&
         numArgs     == 0 &&
         numErrors   == 0 &&
         numWarnings == 0 &&
         errors.IsNull()  &&
         warnings.IsNull()
         );
}

Status Bcmd::SetNull() {
  return Reset();
}

Boolean Bcmd::IsEmpty() const {
  return IsNull();
}

Status Bcmd::SetEmpty() {
  return Reset();
}

UInt Bcmd::Length() const {
  return 0;
}

UInt Bcmd::Size() const {
  return 0;
}

//----------------------------------------------------------------------
//
// Operational Methods
//

Status Bcmd::SetInputFile(String const &fileName) {
  
  inputFileName = fileName;
  inputString.Reset();
  
  return Error::None;
  
}

String Bcmd::GetInputFile() const {
  return inputFileName;
}

Status Bcmd::SetInputString(String const &string) {
  
  inputString = string;
  inputFileName.Reset();
  
  return Error::None;
  
}

String Bcmd::GetInputString() const {
  return inputString;
}

Status Bcmd::SetInputPrompt(String const &prompt) {
  
  inputPrompt = prompt;
  inputFileName.Reset();
  inputString.Reset();
  
  return Error::None;
  
}

Boolean Bcmd::IsFileInput() const {
  return !inputFileName.IsNull();
}

Boolean Bcmd::IsStringInput() const {
  return inputFileName.IsNull() && inputPrompt.IsNull();
}

Boolean Bcmd::IsConsoleInput() const {
  return !inputPrompt.IsNull();
}

Status Bcmd::SetContinuePrompt(String const &prompt) {
  
  continuePrompt = prompt;
  
  return Error::None;
  
}

Status Bcmd::PromptCommand(BcmdCommand &command) {

  Status status;
  
  if (inputPrompt.IsNull()) goto NOPROMPT;

  status = _PromptCommand(command);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    NOPROMPT:  status = Error::LangBefeNoConsolePrompt; break;
    SOMEERROR:                                          break;
  }

  return status;

}

Status Bcmd::GetNextCommand(BcmdCommand  &theCommand) {
  
  Status   status;
  
  // Get it...
  status = _NextCommand(theCommand);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

Status Bcmd::DisplayMessages(BcmdCommand &cmd) {

  Status status;
  String errPrefix;

  if (IsFileInput())
    errPrefix = CmdStringMessage(GetMessage(Error::CommandFileHead), inputFileName);
  else if (IsConsoleInput())
    errPrefix = GetMessage(Error::CommandConsoleHead);
  else if (IsStringInput())
    errPrefix = CmdStringMessage(GetMessage(Error::CommandStringHead), inputString);
  status = cmd.DisplayMessages(errPrefix);

  return status;

}

//----------------------------------------------------------------------
//
// Low level Under-the-cover Methods
//

Status Bcmd::_StartParsing() { // Bcmd._StartParsing...
  
  Status status;
  
  // Nothing to do if already parsing...
  if (isStarted) goto OK;

  // Some sanity checks
  if (BEFE::IsNull(TheBefe)) goto NOBEFE;
  
  // Let Blang do most of it...
  status = Blang::_StartParsing();
  if (status) goto SOMEERROR;
  
  // IMPORTANT!!! Initialise nextEOI so we don't keep reporting EOI...
  status = nextEOI.Reset();
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    OK:        status = Error::None;   break;
    NOBEFE:    status = Error::NoBefe; break;
    SOMEERROR:                         break;
  }
  
  return status;
  
} // ...Bcmd._StartParsing

Status Bcmd::_StopParsing() {

  Status retStatus;
  Status status;

  retStatus = Error::None;

  if (isStarted) {
  
    status = Blang::_StopParsing();
    if (status && !retStatus) retStatus = status;

    if (!retStatus)
      isStarted = false;
      
  }
  
  return retStatus;
  
}

Status Bcmd::_PromptCommand(BcmdCommand &command) { // Bcmd._PromptCommand...

  Status  status;
  BcmdArg arg;
  UInt    lineNo;
  String  line;
  UInt    fg;
  Boolean keepGoing;
  
  lineNo = 0;
  status = inputString.Reset();
  if (status) goto SOMEERROR;

  do {

    // Reset the command
    status = command.Reset();
    if (status) goto SOMEERROR;
    keepGoing = false;

    // Prompt...
    if (gConsoleInteractive || Cin.IsInteractive()) {
      fg = Cout.SetForegroundColour(Console::Yellow);
      if (lineNo == 0)
        Cout << inputPrompt;
      else if (!continuePrompt.IsNull())
        Cout << continuePrompt;
      else
        Cout << String('.')*inputPrompt.Length();
      Cout.SetForegroundColour(fg);
    }

    // Get the line...
    status = Cin.GetLine(line);
    if (status == Error::StreamAtEnd) goto ATEND;
    if (status) goto SOMEERROR;
    if (line.Get(-1) != '\n')
      line += '\n';
    inputString += line;

    // Parse it...
    status = _StopParsing();
    if (status) goto SOMEERROR;
    status = _StartParsing();
    if (status) goto SOMEERROR;
    status = GetNextCommand(command);
    if (status == Error::LangBefeEndOfStream) break;

    // If last one is TypeCommandContinue, we want to keep going
    arg = command.args.Get(-1);
    if (arg.type == BcmdArg::TypeCommandContinue)
      keepGoing = true;

    // If only one error and it's an "Unterminated...", keep going
    arg = command.messages.Get(-1);
    if (arg.status == Error::LangBefeUntermMultiQuote ||
        arg.status == Error::LangBefeUntermComment) {

      status = command.messages.Remove(-1);
      if (status) goto SOMEERROR;
      command.numErrors--;      
      keepGoing = true;
    }

    // If still some errors, we're finished
    if (command.numErrors) break;

    lineNo++;
    
  } while (keepGoing);

  // Handle errors
  while (false) {
    ATEND:     status = Error::LangBefeEndOfStream; break;
    SOMEERROR:                                      break;
  }

  return status;

} // ...Bcmd._PromptCommand

Status Bcmd::_NextCommand(BcmdCommand &command) { // Bcmd._NextCommand...

  Status   status;
  BcmdArg  curArg;
  UInt     argNo;
  BcmdArg *arg0;      // Command's arg[0]
  BcmdArg *arg;       // Command's arg[n]
  String   tStr;
  Char     tChar;
  Status   baseMsg;
  UInt     numQuoteComments;

  // Reset the command first
  status = command.Reset();
  if (status) goto SOMEERROR;

  //
  // Pass 1: Get the arguments...
  //

  do {
    
    // Get the next argument
    status = _NextArgument(command, curArg);
    if (status) goto SOMEERROR;
    
    // Finished if Error or TypeEndOfCommand or TypeEndOfInput...
    if (curArg.type == BcmdArg::TypeEndOfCommand ||
        curArg.type == BcmdArg::TypeEndOfInput)
      break;
    
    // Handle embedded errors/warnings...
    if (curArg.type >= BcmdArg::TypeError && curArg.type <= BcmdArg::TypeWarning) {

      status = command.messages.Append(curArg);
      if (status) goto SOMEERROR;
      if (curArg.type != BcmdArg::TypeWarning)
        command.numErrors++;
      else
        command.numWarnings++;
      break;

    }

    // Save in nextEOI if we're at end of input...
    if (curArg.type == BcmdArg::TypeEndOfInput)
      nextEOI = curArg;

    // Dereference the argument list...
    arg0 = (BcmdArg *)command.args._GetAddress(0);
    if (BEFE::IsNull(arg0) && command.numArgs != 0) goto INTERNAL;
    curArg.Consumable();
    
    // If last one isn't TypeCommandContinue , append it...
    if (command.numArgs == 0 || arg0[command.numArgs-1].type != BcmdArg::TypeCommandContinue) {
      status = command.args.Append(curArg);
      if (status) goto SOMEERROR;
      command.numArgs++;
      continue;
    }
    
    // Last one is TypeCommandContinue, replace it...
    status = command.args.Set(command.numArgs-1, curArg);
    if (status) goto SOMEERROR;
    
  } while (true);
  
  // Handle EndOfInput if command empty...
  if (curArg.type == BcmdArgType::TypeEndOfInput) {
    nextEOI = curArg;
    if (command.numArgs == 0) goto ENDOFINPUT;
  }
  
  //
  // Pass 2: Validate the command arguments...
  //

  argNo = 0;
  arg0 = (BcmdArg *)command.args._GetAddress(0);
  if (BEFE::IsNull(arg0) && command.numArgs != 0) goto INTERNAL;
  numQuoteComments = 0;
  if (!inputFileName.IsNull())
    baseMsg = Error::LangBefeHeadFileLineCol;
  else if (!inputPrompt.IsNull())
    baseMsg = Error::LangBefeHeadConsoleLineCol;
  else
    baseMsg = Error::LangBefeHeadStringLineCol;

  while (argNo < command.numArgs) { // For each BcmdArg...
    
    arg = arg0+argNo;

    // Handle unterminated Literals/Comments
    if ( arg->value.Get(0)         == '"'  ||
         arg->value.Get(0)         == '\'' ||
				 arg->value.Get(0)         == '`' ||
         arg->value.Get(Span(0,2)) == "/*"
       ) { // Is a Literal/Comment...
      
      numQuoteComments++;
      if (arg->value.Get(Span(0,3)) == "'''")
        tStr = "'''";
      else if (arg->value.Get(Span(0,3)) == "\"\"\"")
        tStr = "\"\"\"";
      else if (arg->value.Get(0) == '\'')
        tStr = "'";
      else if (arg->value.Get(0) == '"')
        tStr = '"';
      else if (arg->value.Get(0) == '`')
        tStr = '`';
      else if (arg->value.Get(Span(0,2)) == "/*") {
        tStr = "*/";
      }

      if (arg->value.Get(Range(-(Int)tStr.Length(), -1)) != tStr) { // Unterminated...

        // Change it to an Error...
        if (tStr == "*/")
          arg->status = Error::LangBefeUntermComment;
        else
          arg->status = Error::LangBefeUntermStringLiteral;
        arg->type   = BcmdArg::TypeError;
        tStr = ErrorString(arg->status);
        arg->value = _FormatErrorOrWarning(baseMsg, *arg);

        // Append it to error list...
        status = command.messages.Append(arg->Consumable());
        if (status) goto SOMEERROR;
        command.numMessages++;
        command.numErrors++;

        // Remove it from the arguments
        status = command.args.Remove(argNo);
        if (status) goto SOMEERROR;
        command.numArgs--;
        arg0 = (BcmdArg *)command.args._GetAddress(0);
        if (BEFE::IsNull(arg0) && command.numArgs != 0) goto INTERNAL;
        numQuoteComments--;

        continue;

      } // ...Unterminated

    } // ...Is a Literal/Comment

    // Position to next argument
    argNo++;

  } // ...For each BcmdArg
  
  //
  // Pass 3: Remove quotes and multi-line comments...
  //

  argNo = 0;
  arg0 = (BcmdArg *)command.args._GetAddress(0);
  if (BEFE::IsNull(arg0) && command.numArgs != 0) goto INTERNAL;
  baseMsg = (inputFileName.Length()) ? Error::LangBefeHeadStringLineCol : Error::LangBefeHeadStringLineCol;

  while (numQuoteComments && !command.numMessages && argNo < command.numArgs) { // For each BcmdArg...
    
    arg = arg0+argNo;

    if ( arg->value.Get(Span(0,3)) == "'''" ||
         arg->value.Get(Span(0,3)) == "\"\"\"") {
      tStr = arg->value.Get(Span(0,3));
      status = arg->value.Remove(Span(0,3));
      if (status) goto SOMEERROR;
      if(arg->value.Get(Range(-3,-1)) == tStr) {
        status = arg->value.Remove(Range(-3,-1));
        if (status) goto SOMEERROR;
      }
      numQuoteComments--;
      /* Uncomment this if we decide to strip off leading stuff in multi-quotes...
      if (arg->value.Get(Span(0,2)) == "\r\n") {
        status = arg->value.Remove(Span(0,2));
        if (status) goto SOMEERROR;
      }
      else if (arg->value.Get(0) == '\n') {
        status = arg->value.Remove(0);
        if (status) goto SOMEERROR;
      }
      */
    }
    else if (arg->value.Get(0) == '\'' || arg->value.Get(0) == '"' || arg->value.Get(0) == '`') {
      tChar = arg->value.Get(0);
      status = arg->value.Remove(0);
      if (status) goto SOMEERROR;
      if (arg->value.Get(-1) == tChar) {
        status = arg->value.Remove(-1);
        if (status) goto SOMEERROR;
      }
      numQuoteComments--;
    }
    else if (arg->value.Get(Span(0,2)) == "/*") {
      tChar = arg->value.Get(0);
      status = command.args.Remove(argNo);
      if (status) goto SOMEERROR;
      command.numArgs--;
      arg0 = (BcmdArg *)command.args._GetAddress(0);
      if (BEFE::IsNull(arg0) && command.numArgs != 0) goto INTERNAL;
      numQuoteComments--;
      continue; 
    }    

    argNo++;

  } // ...For each BcmdArg

  //
  // Finally, update statistics
  //

  numCommands++;
  numArgs += command.numArgs;
  
  //
  // Handle errors
  //

  while (false) {
    INTERNAL:   status = Error::Internal;            break;
    ENDOFINPUT: status = Error::LangBefeEndOfStream; break;
    SOMEERROR:                                       break;
  }
  
  return status;

} // ...Bcmd._NextCommand

Status Bcmd::_NextArgument(BcmdCommand &command, BcmdArg &arg) { // Bcmd._NextBcmdArgument...

  Status       status;
  Boolean      appendIt;      // "Append this LexToken to current argumment?"
  Boolean      isContinue;    // "Is this LexToken a trailing '\'?"
  UInt         contSkip;      // Number of tokens to skip if trailing '\'
  LexTokenType tokType;       // Current LexToken type
  String       tokValue;      // Current LexToken full value
  UInt         i;             // Temp: Loop counter
  String       tStr;          // Temp: A String
  Char         tChar;         // Temp: A Char
  Boolean      unTerminated;  // "Is this token an unterminated String literal?"
  
  // Reset the argument...
  status = arg.Reset();
  if (status) goto SOMEERROR;
  
  // It's easy if we're at the end...
  if (nextEOI.type == BcmdArg::TypeEndOfInput) {
    arg = nextEOI;
    goto OK;
  }
  
  // Start parsing if not already...
  status = _StartParsing();
  if (status) goto SOMEERROR;
  
  //
  // Until we get one...
  //
  
  do {

    // Make sure the LexTokens are up to date...
    status = _GetLexTokens();
    if (status) goto SOMEERROR;
    if (BEFE::IsNull(token0)) goto NULLPOINTER;
    
    // Populate nextEOI if EndOfInput...
    tokType = token0[curTokNo].type;
    if (tokType == LexToken::EndOfInput) {
      nextEOI.type = BcmdArg::TypeEndOfInput;
      nextEOI.lineNo = token0[curTokNo].lineNo;
      nextEOI.colNo  = token0[curTokNo].colNo;
    }
    
    // Update BcmdCommand's Spans
    if (!BEFE::IsNull(token0[curTokNo].span.idx1)) {
      if (BEFE::IsNull(command.fullSpan.idx1))
        command.fullSpan.idx1 = token0[curTokNo].span.idx1;
      if (BEFE::IsNull(command.nonWhiteSpan.idx1) && token0[curTokNo].type != LexToken::WhiteSpace)
        command.nonWhiteSpan.idx1 = token0[curTokNo].span.idx1;
      if (BEFE::IsNull(command.fullSpan.idx2))
        command.fullSpan.idx2 = command.fullSpan.idx1;
      if (tokType != LexToken::WhiteSpace && 
          tokType != LexToken::EndOfLine  &&
          tokType != LexToken::EndOfInput)
        command.nonWhiteSpan.idx2 = command.fullSpan.idx2;
    }
    if (!BEFE::IsNull(token0[curTokNo].span.idx2) &&
        tokType != LexToken::EndOfInput           &&
        tokType != LexToken::EndOfLine) {
      command.fullSpan.idx2 = Max(command.fullSpan.idx2, token0[curTokNo].span.idx2);
      if (tokType != LexToken::WhiteSpace)
        command.nonWhiteSpan.idx2 = Max(command.nonWhiteSpan.idx2, token0[curTokNo].span.idx2);
    }
   
    // Ignore if not EndOfLine/EndOfInput and we're skipping...
    if (skipToEOL && tokType != LexToken::EndOfLine && tokType != LexToken::EndOfInput) {
      curTokNo++;
      continue;
    }

    // Get the token's full value
    status = stream.GetInputSpan(token0[curTokNo].span, tokValue);
    if (status) goto SOMEERROR;
    
    // See if '\' continuation and keep track of how many to skip...
    isContinue = false;
    if (tokType == LexToken::Operator &&
        token0[curTokNo].value == "\\" &&
        (curTokNo == 0 || token0[curTokNo-1].type == LexToken::WhiteSpace)
       ) {
      
      // If next is optional whitespace, followed by EndOfLine/EndOfInput, skip 'em...
      i = curTokNo+1;
      if (token0[i].type == LexToken::WhiteSpace) {
        i++;
        contSkip++;
      }
      if (token0[i].type == LexToken::EndOfLine) {
        i++;
        contSkip++;
        isContinue = true;
      }
      contSkip = i-curTokNo;  
      
    }

    // Turn LexToken::Separator ';' into BcmdArg::TypeEndOfCommand...
    if (tokType == LexToken::Separator && tokValue == ";") {

      // Use current arg (if any)
      if (!arg.IsNull())
        break;
      
      // Do it...
      arg.type   = BcmdArg::TypeEndOfCommand;
      arg.lineNo = token0[curTokNo].lineNo;
      arg.colNo  = token0[curTokNo].colNo;
      curTokNo++;
      break;

    }

    // Detect and handle unterminated String literals...
    unTerminated = false;
    if (tokType == LexToken::SLiteral || tokType == LexToken::DLiteral || tokType == LexToken::BLiteral)
      unTerminated = (tokValue.Length() < 2 || tokValue[0] != tokValue[-1]);
    else if (tokType == LexToken::MultiQuote)
      unTerminated = (tokValue.Length() <= 3 || tokValue[Range(0,2)] != tokValue[Range(-3,-1)]);

    if(unTerminated) {
      if (!arg.IsNull())
        break;
      arg.type = BcmdArg::TypeError;
      arg.lineNo = token0[curTokNo].lineNo;
      arg.colNo  = token0[curTokNo].colNo;
      if (tokType == LexToken::MultiQuote)
        arg.status = Error::LangBefeUntermMultiQuote;
      else
        arg.status = Error::LangBefeUntermStringLiteral;
      arg.value = String();
      curTokNo++;
      break;
    }

    // Figure out if we're supposed to append it to current argument...
    appendIt = (tokType == LexToken::Operator && !isContinue) ||
                tokType == LexToken::BracketLeft              ||
                tokType == LexToken::BracketRight             ||
                tokType == LexToken::Name                     ||
                tokType == LexToken::Number                   ||
                tokType == LexToken::SLiteral                 ||
                tokType == LexToken::DLiteral                 ||
								tokType == LexToken::BLiteral                 ||
                tokType == LexToken::MultiQuote;

    // If this isn't contiguous with previous token, we NEVER append it...
    if (!arg.IsNull() && curTokNo && token0[curTokNo].span.idx1 != token0[curTokNo-1].span.idx2)
      appendIt = false;

    // Append if we're supposed to...
    if (appendIt) { // Append to arg...
      
      // Initialise this token's lineNo/colNo if not already set
      if (BEFE::IsNull(curLineStart))
        curLineStart = token0[curTokNo].span.idx1;
      if (BEFE::IsNull(arg.lineNo)) {
        arg.lineNo = token0[curTokNo].lineNo;
        arg.colNo  = token0[curTokNo].colNo;
      }
      
      // Get the full value into tStr...
      if (token0[curTokNo].span.Length() > 4) {
        status = stream.GetInputSpan(token0[curTokNo].span, tStr);
        if (status) goto SOMEERROR;
      }
      else
        tStr = token0[curTokNo].value;
      
      // Append to the argument
      status = arg.value.Append(tStr);
      if (status) goto SOMEERROR;
      
    }  // ...Append to arg

    // Handle non-Append cases...
    if (!appendIt) {
      
      // Use current arg
      if (!arg.IsNull())
        break;
      
      // If EndOfInput, do it...
      if (tokType == LexToken::EndOfInput) {
        arg.type   = BcmdArg::TypeEndOfInput;
        arg.lineNo = token0[curTokNo].lineNo;
        arg.colNo  = token0[curTokNo].colNo;
        curTokNo++;
        break;
      }
    
      // If EndOfLine, do it...
      if (tokType == LexToken::EndOfLine) {
        arg.type   = BcmdArg::TypeEndOfCommand;
        arg.lineNo = token0[curTokNo].lineNo;
        arg.colNo  = token0[curTokNo].colNo;
        curTokNo++;
        break;
      }
    
      // If Comment and it's a /*...*/, we've got one...
      if (tokType == LexToken::Comment && tokValue.Get(Span(0,2)) == "/*") {
        arg.type = BcmdArgType::TypeComment;
        arg.lineNo = token0[curTokNo].lineNo;
        arg.colNo  = token0[curTokNo].colNo;
        arg.value  = tokValue.Consumable();
        curTokNo++;
        break;
      }

      // If Error..Warning, we've got one...
      if (tokType >= LexToken::Error && tokType <= LexToken::Warning) {
        arg.type = (tokType < LexToken::Warning) ? BcmdArg::TypeError : BcmdArg::TypeWarning;
        arg.lineNo = token0[curTokNo].lineNo;
        arg.colNo  = token0[curTokNo].colNo;
        arg.status = token0[curTokNo].status;
        if (tokType == LexToken::Unexpected) {
          tStr = stream.GetInputSpan(token0[curTokNo].span);
          tChar = tStr.Get(0);
          arg.value = LexCharMessage(Error::LexUnexpectedChar, tChar);
        }
        else
          arg.value  = token0[curTokNo].value.Consumable();
        curTokNo++;
        break;
      }
      
      // If it's a non-continuation LexToken::EndOfLine or a separator, do it
      if ((tokType == LexToken::EndOfLine && !isContinue) ||
          tokType == LexToken::Separator) {
        arg.type   = BcmdArg::TypeEndOfCommand;
        arg.lineNo = token0[curTokNo].lineNo;
        arg.colNo  = token0[curTokNo].colNo;
        curTokNo++;
        break;
      }
      
      // If it's a continuation, do it
      if (isContinue) {
        arg.type   = BcmdArg::TypeCommandContinue;
        arg.lineNo = token0[curTokNo].lineNo;
        arg.colNo  = token0[curTokNo].colNo;
        curTokNo += contSkip;
        break;
      }
      
      /*
      // Else, it's not a comment, it's an error...
      if (tokType != LexToken::Preprocessor && tokType != LexToken::Comment) {
        arg.type = BcmdArg::TypeError;
        arg.lineNo = token0[curTokNo].lineNo;
        arg.colNo  = token0[curTokNo].colNo;
        arg.status = Error::LangBefeUnexpectedToken;
        arg.value  = CmdStringMessage(arg.status, token0[curTokNo].value);
        curTokNo++;
        break;
      }
      */

    }
    
    // Skip to next token
    curTokNo += 1;
    
  } while (true);

  // Figure out what type it is...
  status = _FinaliseArg(arg);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    OK:          status = Error::None;                break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }

  // Final clean up...
  if (status || arg.type == BcmdArg::TypeEndOfInput)
    _StopParsing();
    
  return status;

} // ...Bcmd._NextBcmdArgument

Status Bcmd::_FinaliseArg(BcmdArg &arg) { // Bcmd._FinaliseArg...
  
  Status status;
  String optStr;

  status = Error::None;
  
  // Nothing to do if the argument is Null...
  if (arg.IsNull()) goto OK;
  
  // Figure out the type (if we don't know yet)
  optStr = GetOptionString();
  if (arg.type == BcmdArg::TypeUnknown || arg.type >= BcmdArg::NumTypes) {
    arg.type = BcmdArg::TypeArgument;
    if ( !BEFE::IsNull(optStr.Find(arg.value.Get( 0))) )
      arg.type = BcmdArg::TypeOption;
  }
  
  // Handle errors
  while (false) {
    OK: break;
  }
  
  return status;
  
}  // ...Bcmd._FinaliseArg

String Bcmd::_FormatErrorOrWarning(Status baseMsg, BcmdArg &arg) {
  
  String msg;
  Int    pos;
  
  if (arg.type != BcmdArg::TypeError && arg.type != BcmdArg::TypeWarning)
    msg = Error::LangBefeExpectedErrorOrWarn;

  else {
  
    if (baseMsg != Error::None)
      msg = GetMessage(baseMsg);
    msg += ErrorString(arg.status);
  
    pos = msg.Find("${file}");
    if (!BEFE::IsNull(pos))
      msg.Replace(Span(pos,pos+7), inputFileName);
    pos = msg.Find("${line}");
    if (!BEFE::IsNull(pos))
      msg.Replace(Span(pos,pos+7), String((Int)arg.lineNo+1));
    pos = msg.Find("${col}");
    if (!BEFE::IsNull(pos))
      msg.Replace(Span(pos,pos+6), String((Int)arg.colNo+1));

  }
  
  return msg.Consumable();
  
}

} // ...Namespace BEFE
