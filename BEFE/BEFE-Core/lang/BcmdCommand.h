//----------------------------------------------------------------------
// File: Bcmd.h - Declarations for the BcmdCommand Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// Notes: The BcmdCommand represents a single command for the .bcmd
//        language. In essense, it's an ordered list of BcmdArgs.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BCMDCOMMAND_H // BCMDCOMMAND_H...
#define BCMDCOMMAND_H

#include "Primitives.h"
#include "BcmdArg.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Struct BcmdCommand - A .bcmd Input Command
//
// Note: A single BcmdCommand has a list of arguments.  Each command
//       starts at a given line/col in the input but may span multiple
//       lines in case of continuation (tailing '\') and multi-line
//       comments ("/*...*/").
//
// Note: A single argument may span multiple input lines in the cases
//       of '''...''' and """...""".
//

struct BcmdCommand { // Struct BcmdCommand...

  // Members
  Boolean     isConsumable:1;
  UInt        lineNo;       // Starting Line Number (0..numLines-1)
  Span        fullSpan;     // Full Input Span
  Span        nonWhiteSpan; // Input Span with leading/trailing Whitespace trimmed
  BcmdArgs    args;         // Array of arguments
  UInt        numArgs;      // Denormalised: args.Length()
  UInt        numOptions;   // Number of arg[n].type == TypeOption
  UInt        numMessages;  // Denormalised messages.Length()
  UInt        numErrors;    // Number of arg[n].type == TypeError OR in messages
  UInt        numWarnings;  // Number of arg[n].type == TypeWarning OR in messages
  BcmdArgs    messages;     // Error/Warning messages

  // C++ Lifecycle
	BEFE_inline BcmdCommand()                                   {StartUp();}
  BEFE_inline BcmdCommand(BcmdCommand const &that)            {StartUp(); CopyFrom(that);}
	BEFE_inline ~BcmdCommand()                                  {ShutDown();}
  BEFE_inline BcmdCommand &operator=(BcmdCommand const &that) {CopyFrom(that); return *this;}
              String operator[](Int index);

  // BEFE Lifecycle
  Status       StartUp();
  Status       ShutDown();
  Status       Reset();
  Status       MoveFrom(BcmdCommand const &that);
  Status       CopyFrom(BcmdCommand const &that);
  Boolean      IsNull() const;
  Status       SetNull();
  Boolean      IsEmpty() const;
  Status       SetEmpty();
  UInt         Length() const;
  UInt         Size() const;
	Boolean      IsConsumable() const;
  BcmdCommand &Consumable() const;
  BcmdCommand &NotConsumable() const;

  // Error reporting
  Status       NormaliseErrors();
  Status       SetError(Status pStatus);
  Status       SetError(Int argNo, Status pStatus);
  Status       SetError(Int argNo, Status pStatus, String const &errMsg);
  Status       SetError(UInt lineNo, UInt colNo, Status pStatus);
  Status       SetError(UInt lineNo, UInt colNo, Status pStatus, String const &errMsg);
  Status       AppendError(Status pStatus);
  Status       AppendError(Status pStatus, String const &errMsg);

  // Messages
  Status       DisplayMessages(String const &prefix);
  Status       DisplayMessage(String const &prefix, BcmdArg &arg);

  // Options
  Boolean      IsOption(Int argNo) const;
  Status       ExecuteOptions();
  Status       ExecuteOption(Int argNo);
  Status       RemoveOptions();
  Status       _SplitOption(Int argNo, Char &flag, String &name, String &value);
  Status       _ExecuteBooleanOption(Int argNo, Char&flag, String &name, String &value);
  Status       _ExecuteEnvOption(Int argNo);
  Status       _ExecuteEnvOption(Int argNo, Char flag, String &name, String &value);
  Status       _QueryOption(Int argNo, String &name, String &value);
  Status       _QueryEnv(Int argNo, String &name, String &value);

}; // ...struct BcmdCommand

typedef ArrayValue<BcmdCommand> BcmdCommands;

} // ...Namespace BEFE

#endif // ...BCMDCOMMAND_H
