//----------------------------------------------------------------------
// File: BcmdArg.h - Declarations for the BcmdArg Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// Notes: The BcmdArg struct serves as a Lexical Token for the .bcmd
//        language.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BCMDARG_H // BCMDARG_H...
#define BCMDARG_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// BcmdArg - A single Bcmd Argument (a .bcmd Lexical Token)
//

struct BcmdArg { // Struct BcmdArg...

  // Enum BcmdArgType...
  enum BcmdArgType : Byte { // Enum BcmdArgType...
  
     TypeUnknown
    ,TypeError            // Error
    ,TypeWarning          // Warning
    ,TypeComment          // Comment (multi-line)
    ,TypeCommand          // Command Name
    ,TypeOption           // Command Option
    ,TypeArgument         // Command Argument
    ,TypeCommandContinue  // Command Continuation
    ,TypeEndOfCommand     // End Of Command
    ,TypeEndOfInput       // End Of Input
    
    ,NumTypes

  }; // ...Enum BcmdArgType

  // Members
  BcmdArgType type;
  UInt        lineNo;       // Line Number (0..N) of start of Argument
  UInt        colNo;        // Column Number (0..N) of start of Argument
  Status      status;       // Status (can use instead of value for Errors/Warnings)
  String      value;        // Full Argument value (including '"', etc.)
  
  // C++ Lifecycle
  BEFE_inline BcmdArg()                               {StartUp();}
  BEFE_inline BcmdArg(BcmdArg const &that)            {StartUp(); CopyFrom(that);}
  BEFE_inline ~BcmdArg()                              {ShutDown();}
  BEFE_inline BcmdArg &operator=(BcmdArg const &that) {CopyFrom(that); return *this;}
  
  // BEFE Lifecycle
  Status   StartUp();
  Status   ShutDown();
  Status   Reset();
  Status   MoveFrom(BcmdArg const &that);
  Status   CopyFrom(BcmdArg const &that);
  Boolean  IsNull() const;
  Status   SetNull();
  Boolean  IsEmpty() const;
  Status   SetEmpty();
  UInt     Length();
  UInt     Size();
	Boolean  IsConsumable() const;
  BcmdArg &Consumable() const;
  BcmdArg &NotConsumable() const;
  
}; // ...Struct BCmdArg

typedef ArrayValue<struct BcmdArg> BcmdArgs;
typedef BcmdArg::BcmdArgType       BcmdArgType;

} // ...Namespace BEFE

#endif // ...BCMDARG_H
