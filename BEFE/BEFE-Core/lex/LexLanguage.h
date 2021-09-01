//----------------------------------------------------------------------
// File: LexLanguage.h - Declarations for the BEFE LexLanguage Class
//----------------------------------------------------------------------
//
// This class is a <Foundation Class> for BEFE Lexical processing.
//
// A LexLanguage instance is the Lexical specifications of a given
// language to be processed.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LEXLANGUAGE_H // LEXLANGUAGE_H...
#define LEXLANGUAGE_H

#include "Primitives.h"
#include "Array.h"
#include "Stream.h"
#include "Ids.h"

#include "LexState.h"
#include "LexAction.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Typedefs
//

typedef UIntMapPrimitive<Int> Id2Idx;

//----------------------------------------------------------------------
//
// Class LexLanguage
//

class LexLanguage { // Class LexLanguage...

  // Friends
  friend struct LexStream;
  friend class  LexFormatOut;
  friend struct Lex;
  
	// C++ Lifecycle
	public: LexLanguage();
  public: LexLanguage(LexLanguage const &that);
	public: ~LexLanguage();
  public: LexLanguage &operator=(LexLanguage const &that);
	
	// BEFE Lifecycle
  public: Status       StartUp();
  public: Status       ShutDown();
  public: Status       Reset();
  public: Boolean      IsNull() const;
  public: Status       SetNull();
  public: Boolean      IsEmpty();
  public: Status       SetEmpty();
  public: Status       MoveFrom(LexLanguage const &that);
  public: Status       CopyFrom(LexLanguage const &that);
  public: Boolean      IsConsumable() const;
  public: LexLanguage &Consumable() const;
  public: LexLanguage &NotConsumable() const;

  // Member Get/Set (if you want it to build initial State for you)
  public: Id      GetId() const;
  public: Status  SetId(Id newId);
  public: String  GetName() const;
  public: Strings GetErrors() const;
  public: Status  SetName(String const &newName);
  public: Status  SetWhiteChars(String const &chars);
  public: String  GetWhiteChars() const;
  public: Status  SetNameStart(String const &chars);
  public: String  GetNameStart() const;
  public: Status  SetNameChars(String const &chars);
  public: String  GetNameChars() const;
  public: Status  SetOperators(Strings const &ops);
  public: Strings GetOperators() const;
  public: Status  SetBrackPairs(Strings const &pairs);
  public: Strings GetBrackPairs() const;
  public: Status  SetSepChars(String const &theChars);
  public: String  GetSepChars() const;
  public: String  GetQuoteChars() const;
  public: Status  SetQuoteChars(String const &theChars);
  public: Strings GetComments() const;
  public: Status  SetComments(Strings const &theComments);
  public: Char    GetPreprocessorChar() const;
  public: Status  SetPreprocessorChar(Char theChar);
  public: Boolean GetUtfInNames() const;
  public: Status  SetUtfInNames(Boolean theValue);
  
  // States
  public: Status    ClearStates();
  public: UInt      NumberOfStates() const;
  public: Boolean   StateExists(String const &name) const;
  public: Id32      GetStateId(String const &name) const;
  public: Strings   GetStateNames() const;
  public: Ids       GetStateIds() const;
  public: LexState  GetState(String const &name) const;
  public: Status    GetState(String const &name, LexState &state) const;
  public: LexState  GetState(Id stateId) const;
  public: Status    GetState(Id stateId, LexState &state) const;
  public: Int       GetStateIndex(Id stateId);
  public: Int       GetStateIndex(String const &stateName);
  public: Status    NewState(LexState &state);
  public: Status    UpdateStateById(LexState &state);
  public: Status    UpdateStateByName(LexState &state);
  
  // Actions
  public: Status    ClearActions();
  public: UInt      NumberOfActions() const;
  public: Boolean   ActionExists(String const &name) const;
  public: Strings   GetActionNames() const;
  public: Ids       GetActionIds() const;
  public: Id32      GetActionId(String const &name) const;
  public: LexAction GetAction(String const &name) const;
  public: Status    GetAction(String const &name, LexAction &action) const;
  public: LexAction GetAction(Id actionId) const;
  public: Status    GetAction(Id actionId, LexAction &action) const;
  public: Int       GetActionIndex(Id actionId);
  public: Int       GetActionIndex(String const &actionName);
  public: Status    NewAction(LexAction &action);
  public: Status    UpdateActionById(LexAction &action);
  public: Status    GetActionHandler(String const &name, LexCharAction &handler);
  public: Status    SetActionHandler(String const &name, LexCharAction handler);
  
  // Validation
  public: Boolean   IsValid() const;
  public: Status    Validate();
  
  // Generation
  public: Status    Generate();
  
  // Protected Interenal methods
  protected: Status PopulateDefaults();
  protected: Status PopulateDefaultOps();
  protected: Status RebuildIndexes();
  
  //
  // Protected Members
  //
  // Note: contigMax = Maximum number of contiguous Bytes needed for
  //                   token recognition. Typically this is the length
  //                   (in Bytes) of the longest operator, but this can
  //                   be set larger if someone up the line, like a Semantic
  //                   Analyser, wishes this to be larger so they can be
  //                   assured a single keyword, etc., is contiguous in
  //                   memory for recognition purposes.
  //
  
  protected: Boolean isNull:1;
  protected: Boolean isConsumable:1;
  protected: Boolean isDirty:1;
  protected: Boolean isValid:1;

  protected: Id      id;
  protected: String  name;
  public:    Strings errors;
  
  protected: Byte             contigMax;      // Maximum number of contiguous bytes needed (0="Unknown")
  protected: String           whiteChars;     // Single WhiteSpace characters (like ' ', '\r', etc.)
  protected: String           nameStart;      // Name Start character (typically '_', 'a'..'z', and 'A'..'Z')
  protected: String           nameChars;      // Valid Name characters (typically '_', '0'..'9', 'a'..'z', 'A'..'Z')
  protected: String           numberChars;    // Valid Number characters (typically '0'..'9', 'x','b','o' (2nd pos), and 'e', '+', '-' for floating point)
  protected: Strings          operators;      // Single and multiple character operators
  protected: Strings          brackPairs;     // Bracket pairs (like '()','[]', etc.)
  protected: String           sepChars;       // Single character separator characters (like ';', ',', etc.)
  protected: String           quoteChars;     // Single character quotation characters
  protected: Strings          comments;       // Comment markers (like '#' and '//', '/*' and '*/')
  protected: Char             preProcChar;    // Preprocessor Char (if any, typically '#')
  protected: Boolean          utfInNames;     // "UTF-8 okay in names?"
  
  protected: Id               stateHiId;      // High State Id so far
  protected: LexStates        states;         // Array of LexStates
  protected: Id2Idx           stateId2Idx;    // LexState Id->Idx map
  protected: LexStateName2Id  stateName2Id;   // LexState Name->Id map
  protected: LexStateId2Name  stateId2Name;   // LexState Id->Name map
  protected: Id               actionHiId;     // High Action Id so far
  protected: LexActions       actions;        // Array of LexActions
  protected: Id2Idx           actionId2Idx;   // LexAction Id->Idx map
  protected: LexActionName2Id actionName2Id;  // LexState Name->Id map
  protected: LexActionId2Name actionId2Name;  // LexState Idx->Name map
  
}; // ...Class LexLanguage

//----------------------------------------------------------------------
//
// LexLanguage Typedefs
//

typedef ArrayValue<LexLanguage> LexLanguages;

} // ...Namespace BEFE

#endif // ...LEXLANGUAGE_H
