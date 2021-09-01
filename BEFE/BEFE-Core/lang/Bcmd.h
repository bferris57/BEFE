//----------------------------------------------------------------------
// File: Bcmd.h - Declarations for the BEFE Language ".bmsg"
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// Notes: The .bcmd language is built on the .befe lexical language and
//        is Line based.  A trailing '\' (except when in comments) can
//        be used for command line continuation.
//
//        The Bcmd Class is expected to be specialised so that the
//        GetLine method gets the input from wherever.  The default
//        behaviour of this method (as implemented in an unspecialised
//        Bcmd instance) is to return Error::StreamAtEnd.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BCMD_H // BCMD_H...
#define BCMD_H

#include "Primitives.h"
#include "BEFEWrappers.h"
#include "Blang.h"
#include "Lex.h"
#include "LexToken.h"
#include "LexStream.h"
#include "LexLanguage.h"
#include "BcmdArg.h"
#include "BcmdCommand.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Struct Bcmd - The .bcmd Language Class
//

class Bcmd : public Blang { // Class Bcmd...

  //
  // Class (static) methods
  //

  public: static String GetOptionString();
  
  //
  // Methods
  //
  
  // C++ Lifecycle
	public: BEFE_inline Bcmd()                            {StartUp();}
  public: BEFE_inline Bcmd(Bcmd const &that)            {StartUp(); CopyFrom(that);}
	public: BEFE_inline ~Bcmd()                           {ShutDown();}
  public: BEFE_inline Bcmd &operator=(Bcmd const &that) {CopyFrom(that); return *this;}
  
  // BEFE Lifecycle
  public: Status   StartUp();
  public: Status   ShutDown();
  public: Status   Reset();
  public: Status   MoveFrom(Bcmd const &that);
  public: Status   CopyFrom(Bcmd const &that);
  public: Boolean  IsNull() const;
  public: Status   SetNull();
  public: Boolean  IsEmpty() const;
  public: Status   SetEmpty();
  public: UInt     Length() const;
  public: UInt     Size() const;
  
  // Operational Methods
  //
  // Note: These can be used in two ways, 1) by Command, or
  //       2) by Argument.
  //
  //       It doesn't matter which way you use it, either
  //       method is fine by us.
  //
  
  public: Status   SetInputFile      (String const &fileName);
  public: String   GetInputFile      () const;
  public: Status   SetInputString    (String const &string);
  public: String   GetInputString    () const;
  public: Status   SetInputPrompt    (String const &prompt);
  public: Status   SetContinuePrompt (String const &prompt);
  public: Boolean  IsFileInput() const;
  public: Boolean  IsStringInput() const;
  public: Boolean  IsConsoleInput() const;

  public: Status   PromptCommand     (BcmdCommand &theCommand);
  public: Status   GetNextCommand    (BcmdCommand &theCommand);
  public: Status   DisplayMessages   (BcmdCommand &cmd);

  // Low Level Under-the-cover Methods
  private: Status   _StartParsing();
  private: Status   _StopParsing();
  public:  Status   _PromptCommand(BcmdCommand &command); // ◄── Public for debugging/testing purposes only
  public:  Status   _NextCommand(BcmdCommand &command);   // ◄── Public for debugging/testing purposes only
  public:  Status   _NextArgument(BcmdCommand &command, BcmdArg &arg); // ◄── Public for debugging/testing purposes only
  private: Status   _FlushArg(BcmdCommand &command, BcmdArg &arg);
  private: Status   _FinaliseArg(BcmdArg &arg);
  private: String   _FormatErrorOrWarning(Status baseMsg, BcmdArg &arg);
  
  //
  // Members
  //

  // Continue prompt
  protected: String    continuePrompt;

  // Internal: Counts
  protected: UInt      numCommands;   // ◄── Total MessageRanges delivered so far
  protected: UInt      numArgs;       // ◄── Total Messages delivered so far
  protected: UInt      numErrors;     // ◄── Total Errors delivered so far
  protected: UInt      numWarnings;   // ◄── Total Warnings delivered so far
  
  // Internal: Errors/Warnings so far
  protected: BcmdArgs  errors;        // ◄── Errors
  protected: BcmdArgs  warnings;      // ◄── Warnings
  
  // Internal: Next TypeEndOfInput BcmdArg to return
  protected: BcmdArg   nextEOI;       // ◄── Used to defer End Of Input
  
}; // ...Class Bcmd

typedef ArrayValue<Bcmd> Bcmds;

} // ...Namespace BEFE

#endif // ...BCMD_H