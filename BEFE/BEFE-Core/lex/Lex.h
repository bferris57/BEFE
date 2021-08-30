//----------------------------------------------------------------------
// File: Lex.h - Declarations for the BEFE Lex Class
//----------------------------------------------------------------------
//
// This file contains the main declarations for the BEFE Lexical Processing
// (Lex for short) Classes, Types, etc.
//
// The following is a summary of the Types discussed here...
//
//   Lex         - A Class providing the Lex 
//   LexToken    - A Struct indicating a single Lexical Token
//   LexState    - A Struct providing 128 possible Actions to perform
//   LexLanguage - A Class providing a Language specification
//   LexStream   - A Struct providing a Stream of Output LexTokens
//
// The Lex class provides a context to pull all the processing for
// many languages into a single place.  It contains the namespace for
// the following declarations...
//
//   o Enum    ActionNo - Action index (see below)
//   o Typedef Action   - Action Function (see below)
//   o Typedef State    - State Transition Table (see below)
//
//   A Lex.ActionNo is an index into the Lex's list of Sparse LexActions,
//   identified by a UInt (or, in the current setup, a Byte).  The Lex
//   reserves a set of predefined actions which are not to be used for
//   specialised actions, as they are implemented "in-house" for
//   efficiency's sake and so, as such, they don't have to go through
//   the run-time overhead of invoking a Function Pointer that would be
//   required to invoke a Lex.Action.
//
//   A Lex.Action is a Typedef which is a pointer to a C++ function as
//   declared below in this header.  It is the responsibility of the
//   Action Function to interpret the byte, in the current context,
//   and may change the current State Index in the LexStream, modify
//   the current LexToken being produced, produce an Error/Warning
//   message, continue processing, abort processing, or any of these
//   things.
// 
//   NOTE: Should you be wishing to implement a language that is
//         expressed heavily in UTF-8, you may want to consider
//         using a different base Lexical Analyser.
//
//   *** More on Lex.Action elsewhere **
//
//   A Lex.State is simply a State/Action table used for performing
//   actions based on a Byte encountered.  Currently, this is
//   implemented as 128 Bytes. For each input Byte in the "State"
//   identified by the LexState's index (more on this later), it
//   contains a single ActionNo (see below).  For Byte B in the range
//   0x00..0x7F (eg, ASCII), the input Byte is processed according to
//   the action in Byte[B].  For Byte B in the range 0x80..0xFF
//   (eg, UTF-8), the input Byte is processed according to the action
//   specified in Byte[0].  This means that NUL and UTF-8 are handled
//   by the same Lex.Action.  In the "real world" this does not cause
//   any significant performance issues for most "reasonable" input
//   because, at least so far, most programming languages are specified
//   mainly in ASCII so NUL and UTF-8 are considered special cases.
//
// A LexToken Struct is the main purpose of Lexical Processing...
// It identifies a single coherant/cohesive Lexical Token in whatever
// language, and is produced as the output of a LexStream (see below).
//
// A LexLanguage is a Class used to declare the static Lexical
// processing to be performed for a given input language.  It contains
// the following information and, once processing has begun, should
// remain static...
//
//   o State table - Array of N Lex.State instances, numbered 0..N-1
//   o Actions     - Sparse Array of LexAction function pointers
//
// A LexLanguage, although its main purpose is to deliver a Lex.State
// array (Lex.States) its main functional purpose is to provide a
// way of building and validating the States and Actions before
// performing Lexical Analysis.
//
// NOTE: Although we do provide language naming, this is handled by the
// Lex Class itself as it keeps a MapValue<LexLanguage> list to locate
// languages for use by all LexStreams processing on the Lex.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LEX_H // LEX_H...
#define LEX_H

#include "Primitives.h"
#include "Cache.h"
#include "Sparse.h"
#include "Map.h"
#include "Id.h"

#include "LexState.h"
#include "LexRange.h"
#include "LexWorkspace.h"

namespace BEFE { // Namespace BEFE...

// Forward declarations
struct LexStream;
class  LexLanguage;
struct Lex;

//----------------------------------------------------------------------
//
// Lex Typedefs
//

// Various miscellaneous ones we use to make the code more readable...
typedef SparsePrimitive<Id>          LexWorkspaceIds;
typedef SparsePrimitive<Id>          IdLexWorkspaces;
typedef SparsePrimitive<Id>          StreamIds;
typedef SparsePrimitive<Id>          IdStreams;
typedef StringMapValue<LexLanguage>  NamedLexLanguages;

//----------------------------------------------------------------------
//
// Struct Lex - A Lexical Analyser
//

struct Lex {
	
	// Friends
	friend struct LexStream;
  friend class  LexWorkspace;
  
  enum {
    StateMain,
    StateName,
    StateNumber,
  };

  enum : Byte {
     ActionUTF              // index:0x00 - NUL and UTF Action
    ,ActionNewLine          // index:0x01 - NewLine Action
    ,ActionWhiteSpace       // index:0x02 - WhiteSpace Action
    ,ActionName             // index:0x03 - Name Action
    ,ActionNumber           // index:0x04 - Number Action
    ,ActionQuote            // index:0x05 - Quote Action
    ,ActionOperator         // index:0x06 - Single Byte operator
    ,ActionComment          // index:0x07 - Append and set Comment
    ,ActionPreprocessor     // index:0x08 - Set Preprocessor, Append to NewLine
    ,ActionBracketLeft      // index:0x09 - Left Bracket
    ,ActionBracketRight     // index:0x0A - Right Bracket
    ,ActionSeparator        // index:0x0B - Flush, start 'Separator' Token, enter Main State
    ,ActionEscape           // index:0x0C - Escape processing (typically '\' in string literal)
    ,ActionFlushMain        // index:0x0D - Flush, switch to Main State (and process Byte)
    ,ActionAppend           // index:0x0E - Append (keep same State)
    ,ActionAppendFlushMain  // index:0x0F - Append,  Flush, Switch to Main State
    ,ActionNumDefault
    
    ,ActionHigh      = 0x7F // index:0xFF - Highest valid Action No
    ,ActionUnhandled = 0xFF // index:0xFF - Unhandled Action
    
  };
  
  // Public Lex Enums and Typedefs...
  //public: typedef union {Byte actions[128];} State;
  //public: typedef ArrayPrimitive<State>      States;
	
	// C++ Lifecycle
	public: Lex();
  public: Lex(Lex const &that);
	public: ~Lex();
  public: Lex &operator=(Lex const &that);
	
	// BEFE Lifecycle
  public: Status   StartUp();
  public: Status   ShutDown();
  public: Status   Clear();
  public: Status   Reset();
  public: Boolean  IsNull() const;
  public: Status   SetNull();
  public: Boolean  IsEmpty();
  public: Status   SetEmpty();
  public: Status   MoveFrom(Lex const &that);
  public: Status   CopyFrom(Lex const &that);
	public: Boolean  IsConsumable() const;
  public: Lex     &Consumable() const;
  public: Lex     &NotConsumable() const;

  // LexWorkspace Management
  public: Status   IsAttached(LexWorkspace &ws) const;
  public: Status   Attach(LexWorkspace &ws);
  public: Status   Detach(LexWorkspace &ws);
  
  // LexStream Management
  public: Boolean  IsAttached(LexStream &stream) const;
  public: Status   Attach(LexStream &stream);
  public: Status   Detach(LexStream &stream);
  
  // LexLanguage Management
  public: UInt     GetLanguageCount();
  public: Id       GetLanguageId(String const &name);
  public: Status   GetLanguageId(String const &name, Id &langId);
  public: UInt     GetLanguageIndex(String const &name);
  public: Status   GetLanguageIndex(String const &name, UInt &langIndex);
  public: Status   GetLanguage(String const &name, LexLanguage &lang);
  public: Status   RegisterLanguage(LexLanguage &lang);
  public: Status   UnRegisterLanguage(String const &langName);
  public: Strings  GetLanguageNames() const;
  
  //
	// Protected methods
	//
	
	protected: Status DetachWorkspaces();
	protected: Status DetachStreams();
	
  //
  // Members
  //
	
	// Protected Members...
  protected: Boolean           isConsumable:1;  // "IsConsumable" flag
  protected: Id                highWorkspaceId; // Highest LexWorkspaceId so far
  protected: LexWorkspaceIds   workspaceIds;    // Sparse of LexWorkspace Ids, indexed by LexWorkspace Address
  protected: IdLexWorkspaces   idWorkspaces;    // Sparse of LexWorkspace Addresses index by Id
	protected: Id                highStreamId;    // Highest LexStreamId so far
	protected: StreamIds         streamIds;       // Sparse of LexStream Ids, indexed by LexStream Address
	protected: IdStreams         idStreams;       // Sparse of LexStream Addresses indexed by Id
  protected: Id                highLanguageId;  // Highest LexWorkspaceId so far
	protected: NamedLexLanguages languages;       // Language Specifications
  
};

//
// Lex Utility Functions
//  Implemented in LexFuncs.cpp
//

Boolean LexIsAlphabetic(Char c);
Boolean LexIsSpecial(Char c);
Boolean LexIsControl(Char c);
Boolean LexIsWhiteSpace(Char c);
Boolean LexIsQuote(Char c);

Boolean LexIsBase2(Char c);
Boolean LexIsBase8(Char c);
Boolean LexIsBase10(Char c);
Boolean LexIsBase16(Char c);

String LexCharMessage(Status status, Char c);
String LexStringMessage(Status status, String const &s);

} // ...Namespace BEFE

#endif // ...LEX_H
