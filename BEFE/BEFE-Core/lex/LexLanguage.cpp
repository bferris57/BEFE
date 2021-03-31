//----------------------------------------------------------------------
// File: LexLanguage.cpp - Implementation of the BEFE LexLanguage Struct
//----------------------------------------------------------------------
//
// This class contains the C++ implementation of the BEFE LexLanguage Struct
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Id.h"
#include "Lex.h"
#include "LexLanguage.h"
#include "LexLang.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LexLanguage::LexLanguage() {
  StartUp();
}

LexLanguage::LexLanguage(LexLanguage const &that) {
  StartUp();
  CopyFrom(that);
}

LexLanguage::~LexLanguage() {
  ShutDown();
}

LexLanguage &LexLanguage::operator=(LexLanguage const &that) {
	
  CopyFrom(that);
  
  return *this;

}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status LexLanguage::StartUp() {

  Status retStatus;
	Status status;
  
  isNull       = true;
  isDirty      = true;
  isValid      = false;
  isConsumable = false;

  id = UNaN;
  retStatus = name.StartUp();
  status = errors.StartUp();
    if (status && !retStatus) retStatus = status;
  
  contigMax    = 0;
  status = whiteChars.StartUp();
    if (status && !retStatus) retStatus = status;
  status = nameStart.StartUp();
    if (status && !retStatus) retStatus = status;
  status = nameChars.StartUp();
    if (status && !retStatus) retStatus = status;
  numberChars = String("0123456789abcdefABCDEFxXoObBeE+-");
  status    = operators.StartUp();
    if (status && !retStatus) retStatus = status;
  status    = brackPairs.StartUp();
    if (status && !retStatus) retStatus = status;
  status    = sepChars.StartUp();
    if (status && !retStatus) retStatus = status;
  status    = quoteChars.StartUp();
    if (status && !retStatus) retStatus = status;
  status    = comments.StartUp();
    if (status && !retStatus) retStatus = status;
  preProcChar = Char();
  utfInNames  = false;
  
  stateHiId = 0;
  status = states.StartUp();
    if (status && !retStatus) retStatus = status;
  status = stateId2Idx.StartUp();
    if (status && !retStatus) retStatus = status;
  status = stateName2Id.StartUp();
    if (status && !retStatus) retStatus = status;
  status = stateId2Name.StartUp();
    if (status && !retStatus) retStatus = status;
  actionHiId = 0;
  status = actions.StartUp();
    if (status && !retStatus) retStatus = status;
  status = actionId2Idx.StartUp();
    if (status && !retStatus) retStatus = status;
  status = actionName2Id.StartUp();
    if (status && !retStatus) retStatus = status;
  status = actionId2Name.StartUp();
    if (status && !retStatus) retStatus = status;
  
  return retStatus;
	
}

Status LexLanguage::ShutDown() {
	
  Status retStatus;
	Status status;
  
  retStatus = name.ShutDown();
  status = errors.ShutDown();
    if (status && !retStatus) retStatus = status;

  status = whiteChars.ShutDown();
    if (status && !retStatus) retStatus = status;
  status = nameStart.ShutDown();
    if (status && !retStatus) retStatus = status;
  status = numberChars.ShutDown();
    if (status && !retStatus) retStatus = status;
  status = nameChars.ShutDown();
    if (status && !retStatus) retStatus = status;
  status    = operators.ShutDown();
    if (status && !retStatus) retStatus = status;
  status    = brackPairs.ShutDown();
    if (status && !retStatus) retStatus = status;
  status    = sepChars.ShutDown();
    if (status && !retStatus) retStatus = status;
  status    = quoteChars.ShutDown();
    if (status && !retStatus) retStatus = status;
  status    = comments.ShutDown();
    if (status && !retStatus) retStatus = status;

  status = states.ShutDown();
    if (status && !retStatus) retStatus = status;
  status = stateId2Idx.ShutDown();
    if (status && !retStatus) retStatus = status;
  status = stateName2Id.ShutDown();
    if (status && !retStatus) retStatus = status;
  status = stateId2Name.ShutDown();
    if (status && !retStatus) retStatus = status;
  status = actions.ShutDown();
   if (status && !retStatus) retStatus = status;
  status = actionId2Idx.ShutDown();
    if (status && !retStatus) retStatus = status;
  status = actionName2Id.ShutDown();
    if (status && !retStatus) retStatus = status;
  status = actionId2Name.ShutDown();
    if (status && !retStatus) retStatus = status;
  
  return retStatus;
	
}

Status LexLanguage::Reset() {
	
  Status retStatus;
  Status status;
	
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
	
  return retStatus;
	
}

Boolean LexLanguage::IsNull() const {
  return isNull;
}

Status LexLanguage::SetNull() {
  
  Status status;
  
  status = Reset();
  
  return status;
  
}

Boolean LexLanguage::IsEmpty() {
  
  Boolean answer;
  
  answer = errors.IsEmpty()        &&
           states.IsEmpty()        &&
           stateName2Id.IsEmpty()  &&
           stateId2Name.IsEmpty()  &&
           actions.IsEmpty()       &&
           actionName2Id.IsEmpty() &&
           actionId2Name.IsEmpty();
  
  return answer;

}

Status LexLanguage::SetEmpty() {

  Status retStatus;
	Status status;

  isDirty = true;
  isValid = false;
  retStatus = errors.SetEmpty();
  
  stateHiId = 0;
  status = states.SetEmpty();
    if (status && !retStatus) retStatus = status;
  status = stateId2Idx.SetEmpty();
    if (status && !retStatus) retStatus = status;
  status = stateName2Id.SetEmpty();
    if (status && !retStatus) retStatus = status;
  status = stateId2Name.SetEmpty();
    if (status && !retStatus) retStatus = status;
  actionHiId = 0;  
  status = actions.SetEmpty();
    if (status && !retStatus) retStatus = status;
  status = actionId2Idx.SetEmpty();
    if (status && !retStatus) retStatus = status;
  status = actionName2Id.SetEmpty();
    if (status && !retStatus) retStatus = status;
  status = actionId2Name.SetEmpty();
    if (status && !retStatus) retStatus = status;
  	
  return retStatus;

}

Status LexLanguage::MoveFrom(LexLanguage const &that) {
	
	Status  retStatus;
	Status  status;
	
  if (this != (LexLanguage *)&that) {	
    retStatus = Reset();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexLanguage));
    status = ((LexLanguage *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
    NotConsumable();
  }
  else
    retStatus = Error::None;
		
  return retStatus;
	
}

Status LexLanguage::CopyFrom(LexLanguage const &that) {
	
	Status  retStatus;
	Status  status;
	
  if (this != (LexLanguage *)&that) {	
    if (IsConsumable()) 
      retStatus = MoveFrom(that);
    else {
      retStatus = Reset();
      isNull       = that.isNull;
      isConsumable = false;
      isDirty      = that.isDirty;
      isValid      = that.isValid;
      utfInNames   = that.utfInNames;

      id = that.id;
      status = name.CopyFrom(that.name);
        if (status && !retStatus) retStatus = status;
      status = errors.CopyFrom(that.errors);
        if (status && !retStatus) retStatus = status;
      
      contigMax = that.contigMax;
      status = whiteChars.CopyFrom(that.whiteChars);
        if (status && !retStatus) retStatus = status;
      status = nameChars.CopyFrom(that.nameChars);
        if (status && !retStatus) retStatus = status;
      status = operators.CopyFrom(that.operators);
        if (status && !retStatus) retStatus = status;
      status = brackPairs.CopyFrom(that.brackPairs);
        if (status && !retStatus) retStatus = status;
      status = sepChars.CopyFrom(that.sepChars);
        if (status && !retStatus) retStatus = status;
      status = quoteChars.CopyFrom(that.quoteChars);
        if (status && !retStatus) retStatus = status;
      status = comments.CopyFrom(that.comments);
        if (status && !retStatus) retStatus = status;
        
      stateHiId = that.stateHiId;
      status = states.CopyFrom(that.states);
        if (status && !retStatus) retStatus = status;
      status = stateId2Idx.CopyFrom(that.stateId2Idx);
        if (status && !retStatus) retStatus = status;
      status = stateName2Id.CopyFrom(that.stateName2Id);
        if (status && !retStatus) retStatus = status;
      status = stateId2Name.CopyFrom(that.stateId2Name);
        if (status && !retStatus) retStatus = status;
      actionHiId = that.actionHiId;
      status = actions.CopyFrom(that.actions);
        if (status && !retStatus) retStatus = status;
      status = actionId2Idx.CopyFrom(that.actionId2Idx);
        if (status && !retStatus) retStatus = status;
      status = actionName2Id.CopyFrom(that.actionName2Id);
        if (status && !retStatus) retStatus = status;
      status = actionId2Name.CopyFrom(that.actionId2Name);
        if (status && !retStatus) retStatus = status;
      
    }
  }
  else
    retStatus = Error::None;
		
  return retStatus;
	
}

Boolean LexLanguage::IsConsumable() const {
  return isConsumable;
}

LexLanguage &LexLanguage::Consumable() const {
  ((LexLanguage *)this)->isConsumable = true;
  return *((LexLanguage *)this);
}

LexLanguage &LexLanguage::NotConsumable() const {
  ((LexLanguage *)this)->isConsumable = false;
  return *((LexLanguage *)this);
}

//----------------------------------------------------------------------
//
// Member Get/Set
//

Id LexLanguage::GetId() const {
  return id;
}

Status LexLanguage::SetId(Id newId) {
  
  Status status;
  
  if (!BEFE::IsNull(id)) goto ALREADYSET;
  id = newId;
  
  // Handle errors
  status = Error::None;
  while (false) {
    ALREADYSET: status = Error::LexLangIdAlreadySet; break;
  }
  
  return status;
  
}

String LexLanguage::GetName() const {  
  String localName;
  localName = name;  
  return localName.Consumable();
}

Strings LexLanguage::GetErrors() const {  
  Strings localErrors;
  localErrors = errors;  
  return localErrors.Consumable();
}

Status LexLanguage::SetName(String const &newName) {
  return name.CopyFrom(newName);
}

Status LexLanguage::SetWhiteChars(String const &chars)  {
  
  Status  status;

  isDirty = true;
  status = whiteChars.CopyFrom(chars);
  
  return status;
  
}

String LexLanguage::GetWhiteChars() const  {
  
  String theString;
  
  theString.CopyFrom(whiteChars);
  
  return theString.Consumable();
  
}

Status LexLanguage::SetNameStart(String const &chars)  {
  
  Status  status;

  isDirty = true;
  status = nameStart.CopyFrom(chars);
  
  return status;
  
}

String LexLanguage::GetNameStart() const  {
  
  String theString;
  
  theString.CopyFrom(nameStart);
  
  return theString.Consumable();
  
}

Status LexLanguage::SetNameChars(String const &chars)  {
  
  Status  status;

  isDirty = true;
  status = nameChars.CopyFrom(chars);
  
  return status;
  
}

String LexLanguage::GetNameChars() const  {
  
  String theString;
  
  theString.CopyFrom(nameChars);
  
  return theString.Consumable();
  
}

Status LexLanguage::SetOperators(Strings const &ops) {
  
  Status  retStatus;
  Status  status;

  isDirty = true;
  retStatus = operators.CopyFrom(ops);
  status = operators.Sort();
  if (status && retStatus) retStatus = status;
  
  return retStatus;
  
}

Strings LexLanguage::GetOperators() const {
  
  Strings theOps;
  
  theOps.CopyFrom(operators);
  
  return theOps.Consumable();
  
}

Status LexLanguage::SetBrackPairs(Strings const &pairs) {
  
  Status  status;
  
  isDirty = true;
  status = brackPairs.CopyFrom(pairs);
  
  return status;
  
}

Strings LexLanguage::GetBrackPairs() const {

  Strings thePairs;
  
  thePairs.CopyFrom(brackPairs);
  
  return thePairs.Consumable();
  
}

Status LexLanguage::SetSepChars(String const &theChars) {
  
  Status  status;
  
  isDirty = true;
  status = sepChars.CopyFrom(theChars);

  return status;
  
}

String LexLanguage::GetSepChars() const {

  String theChars;
  
  theChars.CopyFrom(sepChars);
  
  return theChars.Consumable();

}

String LexLanguage::GetQuoteChars() const {

  String theChars;
  
  theChars.CopyFrom(quoteChars);
  
  return theChars.Consumable();

}

Status LexLanguage::SetQuoteChars(String const &theChars) {
  
  Status  status;
  
  isDirty = true;
  status = quoteChars.CopyFrom(theChars);

  return status;
  
}

Strings LexLanguage::GetComments() const {

  Strings theComments;
  
  theComments.CopyFrom(comments);
  
  return theComments.Consumable();

}

Status LexLanguage::SetComments(Strings const &theComments) {
  
  Status  status;
  
  isDirty = true;
  status = comments.CopyFrom(theComments);

  return status;
  
}

Char LexLanguage::GetPreprocessorChar() const {
  return preProcChar;
}

Status LexLanguage::SetPreprocessorChar(Char theChar) {
  
  Status status;
  
  if (theChar.IsBadUnicode())
    status = Error::UTF8InvalidChar;
  else {
    preProcChar = theChar;
    status = Error::None;
  }
  
  return status;
  
}

Boolean LexLanguage::GetUtfInNames() const {
  return utfInNames;
}

Status LexLanguage::SetUtfInNames(Boolean theValue) {

  utfInNames = theValue;

  return Error::None;

}

//----------------------------------------------------------------------
//
// States
//

Status LexLanguage::ClearStates() {
  return Error::NotImplemented;
}

UInt LexLanguage::NumberOfStates() const {
  return stateId2Idx.Length();
}

Boolean LexLanguage::StateExists(String const &name) const {
  return stateName2Id.HasKey(name);
}

Id32 LexLanguage::GetStateId(String const &name) const {
  
  Status status;
  Id32   id;
  
  BEFE::SetNull(id);
  if (stateName2Id.HasKey(name)) {
    status = stateName2Id.Get(name, id);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return id;

}

Strings LexLanguage::GetStateNames() const {
  
  Strings theNames;
 
  theNames = stateName2Id.Keys();
  theNames.Sort();
  
  return theNames.Consumable();
  
}

Ids LexLanguage::GetStateIds() const {
  
  Ids   theIds;
  UInts theInts;
  
  theInts = stateId2Name.Keys();
  theIds._ids.MoveFrom(theInts);
  
  return theIds.Consumable();
  
}
  
LexState LexLanguage::GetState(String const &name) const {
  
  //Status    status;
  LexState theState;
  
  /*status =*/ GetState(name, theState);

  return theState.Consumable();

}
  
Status LexLanguage::GetState(String const &name, LexState &state) const {
  
  Status status;
  Id32   stateId;
  UInt   stateIdx;
  String theName;

  theName = name;   // ◄── Need to do this in case name parameter is in state itself  
  state.Reset();
  
  if (!stateName2Id.HasKey(theName)) goto NOTEXIST;  
  BEFE::SetNull(stateId);
  status = stateName2Id.Get(theName, stateId);
  if (!stateId2Idx.HasKey((Int)stateId)) goto NOTEXIST;
  stateIdx = stateId2Idx.Get((UInt)stateId);
  if (stateIdx > states.Length()) goto NOTEXIST;
  status = states.Get(stateIdx, state);
  if (status) goto SOMEERROR;
  
  // Handle errors...
  while (false) {
    NOTEXIST:  status = Error::LexStateNotExist; break;
    SOMEERROR:                                   break;
  }
  
  return status;
  
}

LexState LexLanguage::GetState(Id stateId) const {
  
  //Status    status;
  LexState theState;
  
  /*status =*/ GetState(stateId, theState);

  return theState.Consumable();

}

Status LexLanguage::GetState(Id stateId, LexState &state) const {
  
  Status status;
  UInt   stateIdx;
  state.Reset();
  
  if (!actionId2Idx.HasKey(stateId)) goto NOTEXIST;
  stateIdx = stateId2Idx.Get((UInt)stateId);
  if (stateIdx > states.Length()) goto NOTEXIST;
  status = states.Get(stateIdx, state);
  if (status) goto SOMEERROR;
  
  // Handle errors...
  while (false) {
    NOTEXIST:  status = Error::LexStateNotExist; break;
    SOMEERROR:                                   break;
  }
  
  return status;
  
}

Int LexLanguage::GetStateIndex(Id stateId) {
  Int theIndex;
  if (!stateId2Idx.HasKey(stateId))
    BEFE::SetNull(theIndex);
  else
    theIndex = stateId2Idx.Get((UInt)stateId);
  return theIndex;
}

Int LexLanguage::GetStateIndex(String const &stateName) {
  
  //Status status;
  Id     theId;
  Int    theIndex;

  BEFE::SetNull(theIndex);
  BEFE::SetNull(theId);

  if (stateName2Id.HasKey(stateName)) {
    /*status =*/ stateName2Id.Get(stateName, theId);
    theIndex = GetStateIndex(theId);
  }

  return theIndex;
}

Status LexLanguage::NewState(LexState &state) {
  
  Status status;
  UInt   stateIdx;
  
  // Do "New Validation" first...
  if (!BEFE::IsNull(state.id)) goto IDNOTNULL;
  if (state.name.IsNull()) goto NAMEEMPTY;
  if (stateName2Id.HasKey(state.name)) goto DUPNAME;
  if (states.Length() >= 0x7f) goto TOOMANY;
  
  // Append it...
  state.id = ++stateHiId;
  status = states.Append(state);
  if (status) goto SOMEERROR;
  stateIdx = states.Length() - 1;
  
  // Update Indexes...
  status = stateId2Idx.Set(state.id, stateIdx);
  if (status) goto SOMEERROR;
  status = stateId2Name.Set(state.id, state.name);
  if (status) goto SOMEERROR;
  status = stateName2Id.Set(state.name, state.id);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    IDNOTNULL: status = Error::LexStateIdNotNull;  break;
    NAMEEMPTY: status = Error::LexStateNoName;     break;
    DUPNAME:   status = Error::LexStateExists;     break;
    TOOMANY:   status = Error::LexStateTooMany;    break;
    SOMEERROR:                                     break;
  }
  
  return status;
  
}

Status LexLanguage::UpdateStateById(LexState &state) {
  
  Status   status;
  Int      idx;
  LexState oldState;
  
  if (BEFE::IsNull(state.id)) goto NULLID;
  if (!stateId2Idx.HasKey(state.id)) goto NOTEXIST;
  idx = stateId2Idx.Get(state.id);
  status = states.Get(idx, oldState);
  
  // Update it...
  status = states.Set(idx, state);
  if (status) goto SOMEERROR;
  
  // If name is different, update indexes (but check for duplicate)...
  if (state.name != oldState.name) {
    if (GetStateIndex(state.name) != idx) goto DUPNAME;
    status = stateName2Id.Remove(oldState.name);
    if (status) goto SOMEERROR;
    status = stateId2Name.Remove(oldState.id);
    if (status) goto SOMEERROR;
    status = stateName2Id.Set(state.name, state.id);
    if (status) goto SOMEERROR;
    status = stateId2Name.Set(state.id, state.name);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors;
  while (false) {
    NULLID:    status = Error::LexStateIdNull;        break;
    DUPNAME:   status = Error::LexStateDuplicateName; break;
    NOTEXIST:  status = Error::LexStateNotExist;      break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

Status LexLanguage::UpdateStateByName(LexState &state) {
  
  Status   status;
  Int      idx;
  LexState oldState;
  
  if (!stateName2Id.HasKey(state.name)) goto NOTEXIST;
  status = stateName2Id.Get(state.name, state.id);
  if (status) goto SOMEERROR;
  status = stateId2Idx.Get(state.id, idx);
  if (status) goto SOMEERROR;
  status = states.Get(idx, oldState);
  if (status) goto SOMEERROR;
  
  // Update it...
  status = states.Set(idx, state);
  if (status) goto SOMEERROR;
  
  // Handle errors;
  while (false) {
    NOTEXIST:  status = Error::LexStateNotExist; break;
    SOMEERROR:                                   break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Actions
//

Status LexLanguage::ClearActions() {
  return Error::NotImplemented;
}

UInt LexLanguage::NumberOfActions() const {
  return actionId2Idx.Length();
}

Boolean LexLanguage::ActionExists(String const &name) const {
  return actionName2Id.HasKey(name);
}

Id32 LexLanguage::GetActionId(String const &name) const {
  
  //Status status;
  Id32   id;
 
  BEFE::SetNull(id); 
  if (actionName2Id.HasKey(name))
    /*status =*/ actionName2Id.Get(name, id);
  return id;
}

Strings LexLanguage::GetActionNames() const {
  
  Strings theNames;
 
  theNames = actionName2Id.Keys();
  theNames.Sort();
  
  return theNames.Consumable();
  
}

Ids LexLanguage::GetActionIds() const {
  
  Ids   theIds;
  UInts theUInts;
  
  theUInts = actionId2Name.Keys();
  theIds._ids.MoveFrom(theUInts);
  
  return theIds.Consumable();
  
}

LexAction LexLanguage::GetAction(String const &name) const {
  
  //Status    status;
  LexAction theAction;
  
  /*status =*/ GetAction(name, theAction);

  return theAction.Consumable();

}
  
Status LexLanguage::GetAction(String const &name, LexAction &action) const {
  
  Status status;
  Id     actionId;
  UInt   actionIdx;
  String theName;
  
  theName = name;   // ◄── Need to do this in case name parameter is in action itself
  action.Reset();
  
  if (!actionName2Id.HasKey(theName)) goto NOTEXIST;
  status = actionName2Id.Get(theName, actionId);
  if (status) goto SOMEERROR;
  if (!actionId2Idx.HasKey(actionId)) goto NOTEXIST;
  actionIdx = actionId2Idx.Get((UInt)actionId);
  if (actionIdx > actions.Length()) goto NOTEXIST;
  status = actions.Get(actionIdx, action);
  if (status) goto SOMEERROR;
  
  // Handle errors...
  while (false) {
    NOTEXIST:  status = Error::LexActionNotExist; break;
    SOMEERROR:                                    break;
  }
  
  return status;
  
}

LexAction LexLanguage::GetAction(Id actionId) const {
  
  //Status    status;
  LexAction theAction;
  
  /*status =*/ GetAction(actionId, theAction);

  return theAction.Consumable();

}

Status LexLanguage::GetAction(Id actionId, LexAction &action) const {
  
  Status status;
  UInt   actionIdx;
  
  action.Reset();
  
  if (!actionId2Idx.HasKey(actionId)) goto NOTEXIST;
  actionIdx = actionId2Idx.Get((UInt)actionId);
  if (actionIdx > actions.Length()) goto NOTEXIST;
  status = actions.Get(actionIdx, action);
  if (status) goto SOMEERROR;
  
  // Handle errors...
  while (false) {
    NOTEXIST:  status = Error::LexActionNotExist; break;
    SOMEERROR:                                    break;
  }
  
  return status;
  
}

Int LexLanguage::GetActionIndex(Id actionId) {
  Int theIndex;
  if (!actionId2Idx.HasKey(actionId))
    BEFE::SetNull(theIndex);
  else
    theIndex = actionId2Idx.Get((UInt)actionId);
  return theIndex;
}

Int LexLanguage::GetActionIndex(String const &actionName) {

  Status status;
  Id     theId;
  Int    theIndex;
  
  BEFE::SetNull(theIndex);
  BEFE::SetNull(theId);
  if (actionName2Id.HasKey(actionName)) {
    status = actionName2Id.Get(actionName, theId);
    if (status) goto SOMEERROR;
    status = actionId2Idx.Get(theId, theIndex);
    if (status) goto SOMEERROR;
  }
    
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return theIndex;
}

Status LexLanguage::NewAction(LexAction &action) {
  
  Status status;
  
  // Do "New Validation" first...
  if (!BEFE::IsNull(action.id)) goto IDNOTNULL;
  if (action.name.IsNull()) goto NAMEEMPTY;
  if (stateName2Id.HasKey(action.name)) goto DUPNAME;
  if (actions.Length() >= 0x7f) goto TOOMANY;
  
  // Append it...
  action.id = ++actionHiId;
  status = actions.Append(action);
  if (status) goto SOMEERROR;

  // Update Indexes...
  status = actionId2Idx.Set(action.id, actions.Length()-1);
  if (status) goto SOMEERROR;
  status = actionId2Name.Set(action.id, action.name);
  if (status) goto SOMEERROR;
  status = actionName2Id.Set(action.name, action.id);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    IDNOTNULL: status = Error::LexActionIdNotNull;   break;
    NAMEEMPTY: status = Error::LexActionMissingName; break;
    DUPNAME:   status = Error::LexActionExists;      break;
    TOOMANY:   status = Error::LexActionTooMany;     break;
    SOMEERROR:                                       break;
  }
  
  return status;
  
}

Status LexLanguage::UpdateActionById(LexAction &action) {
  
  Status    status;
  UInt      idx;
  LexAction oldAction;
  
  if (BEFE::IsNull(action.id)) goto NULLID;
  if (!actionId2Idx.HasKey(action.id)) goto NOTEXIST;
  idx = actionId2Idx.Get(action.id);
  status = actions.Get(idx, oldAction);
  
  // Update it...
  status = actions.Set(idx, action);
  if (status) goto SOMEERROR;
  
  // If name is different, update indexes...
  if (action.name != oldAction.name) {
    status = actionName2Id.Remove(oldAction.name);
    if (status) goto SOMEERROR;
    status = actionId2Name.Remove(oldAction.id);
    if (status) goto SOMEERROR;
    status = actionName2Id.Set(action.name, action.id);
    if (status) goto SOMEERROR;
    status = actionId2Name.Set(action.id, action.name);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors;
  while (false) {
    NULLID:    status = Error::LexActionIdNull;   break;
    NOTEXIST:  status = Error::LexActionNotExist; break;
    SOMEERROR:                                    break;
  }
  
  return status;
  
}

Status LexLanguage::GetActionHandler(String const &name, LexCharAction &handler) {
  
  Status    status;
  LexAction action;

  status = GetAction(name, action);
  if (status) goto SOMEERROR;
  
  handler = action.charHandler;
  
  // Handle errors;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

Status LexLanguage::SetActionHandler(String const &name, LexCharAction handler) {
  
  Status    status;
  UInt      idx;
  LexAction action;

  status = GetAction(name, action);
  if (status) goto SOMEERROR;  
  idx = actionId2Idx.Get(action.id);
  if (BEFE::IsNull(idx)) goto NOTEXIST;
  
  // Update it...
  action.charHandler = handler;
  status = actions.Set(idx, action);
  if (status) goto SOMEERROR;
  
  // Handle errors;
  while (false) {
    NOTEXIST:  status = Error::LexActionNotExist; break;
    SOMEERROR:                                    break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Validation
//

Boolean LexLanguage::IsValid() const {
  
  Status status;
  
  status = ((LexLanguage *)this)->Validate();

  return status == Error::None;

}

Status LexLanguage::Validate() {

  Status  status;
  UInt    i;
  UInt    j;
  Char    c;
  Char    c1;
  Char    c2;
  String  str;
  String  msg;
  Strings curOps;
  String  curChars;
  UInt    numErrors;
  Int     pos;
  
  // Nothing to do if it's not dirty or it's already valid...
  if (!isDirty || isValid) goto OK;
  
  // Clear generated info first
  status = SetEmpty();
  if (status) goto SOMEERROR;
  
  // Validate whiteChars
  for (i=0; i < whiteChars.Length(); i++) {
    c = whiteChars.Get(i);
    if (!LexIsWhiteSpace(c)) {
      msg = LexCharMessage(Error::LexInvalidWhite, c);
      errors.Append(msg.Consumable());
    }
    if ( !BEFE::IsNull(whiteChars.Find(c, Span(i+1,NaN))) ) {
      msg = LexCharMessage(Error::LexDuplicateWhite, c);
      errors.Append(msg.Consumable());
    }
    curChars += c;
  } 

  // Validate nameStart
  for (i=0; i < nameStart.Length(); i++) {
    c = nameStart.Get(i);
    if ( !BEFE::IsNull(curChars.Find(c)) || ((UInt)c > 0x7f && !utfInNames)) {
      msg = LexCharMessage(Error::LexBadNameChar, c);
      errors.Append(msg.Consumable());
    }
    pos = nameStart.Find(c, Span(i+1,NaN));
    if ( !BEFE::IsNull(pos) ) {
      msg = LexCharMessage(Error::LexDuplicateName, c);
      errors.Append(msg.Consumable());
    }
    curChars += c;
  } 

  // Validate brackPairs
  for (i=0; i < brackPairs.Length(); i++) {
    str = brackPairs.Get(i);
    numErrors = errors.Length();
    if (str.IsNull() || str.Length() != 2)
      errors.Append( LexStringMessage(Error::LexBadPairSize, str).Consumable() );
    c1 = str.Get(0);
    c2 = str.Get(1);
    if (!LexIsSpecial(c1))
      errors.Append( LexCharMessage(Error::LexBadPairChar, c1).Consumable() );
    if (!LexIsSpecial(c2))
      errors.Append( LexCharMessage(Error::LexBadPairChar, c2).Consumable() );      
    if ( !BEFE::IsNull(curChars.Find(c1)) || c1 == c2 )
      errors.Append( LexCharMessage(Error::LexDupPairChar, c1).Consumable() );
    if ( !BEFE::IsNull(curChars.Find(c2)) && c1 != c2 )
      errors.Append( LexCharMessage(Error::LexDupPairChar, c2).Consumable() );
    if (numErrors == errors.Length()) {
      curChars += c1;
      curChars += c2;
    }
  }
  
  // Validate sepChars...
  for (i=0; i < sepChars.Length(); i++) {
    c = sepChars.Get(i);
    if ( !LexIsSpecial(c) ) {
      msg = LexCharMessage(Error::LexBadSepChar, c);
      errors.Append(msg.Consumable());
    }
    if ( !BEFE::IsNull(sepChars.Find(c,Span(i+1,NaN))) ) {
      msg = LexCharMessage(Error::LexDupSepChar, c);
      errors.Append(msg.Consumable());
    }
  }

  // Validate quoteChars...
  for (i=0; i < quoteChars.Length(); i++) {
    c = quoteChars.Get(i);
    if ( !LexIsSpecial(c) ) {
      msg = LexCharMessage(Error::LexBadQuoteChar, c);
      errors.Append(msg.Consumable());
    }
    if ( !BEFE::IsNull(quoteChars.Find(c,Span(i+1,NaN))) ) {
      msg = LexCharMessage(Error::LexDupQuoteChar, c);
      errors.Append(msg.Consumable());
    }
  }
  
  // Validate operators
  for (i=0; i < operators.Length(); i++) {
    str = operators.Get(i);
    numErrors = errors.Length();
    if (str.Length() > 4)
      errors.Append( LexStringMessage(Error::LexBadOperatorSize, str).Consumable() );
    for (j=0; j < str.Length(); j++) {
      c = str.Get(j);
      if (!LexIsSpecial(c)) {
        errors.Append( LexStringMessage(Error::LexBadOperatorChar, str).Consumable() );
        break;
      }
    }
    if (curOps.Contains(str))
      errors.Append( LexStringMessage(Error::LexDupOperator, str).Consumable() );
    if ( !BEFE::IsNull(curChars.Find(str.Get(0))) )
      errors.Append( LexStringMessage(Error::LexDupOperatorChar, str).Consumable() );
    c = str.Get(0);
    if (numErrors == errors.Length()) {
      status = curOps.Append(str);
      if (status) goto SOMEERROR;
    }
  }
  
  // Validate comments
  for (i=0; i < comments.Length(); i++) {
    str = comments.Get(i);
    numErrors = errors.Length();
    if (str.Length() > 4)
      errors.Append( LexStringMessage(Error::LexBadCommentSize, str).Consumable() );
    for (j=0; j < str.Length(); j++) {
      c = str.Get(j);
      if (!LexIsSpecial(c)) {
        errors.Append( LexStringMessage(Error::LexBadCommentChar, str).Consumable() );
        break;
      }
    }
    if (curOps.Contains(str))
      errors.Append( LexStringMessage(Error::LexDupComment, str).Consumable() );
    if ( !BEFE::IsNull(curChars.Find(str.Get(0))) )
      errors.Append( LexStringMessage(Error::LexDupCommentChar, str).Consumable() );
    c = str.Get(0);
    if (numErrors == errors.Length()) {
      status = curOps.Append(str);
      if (status) goto SOMEERROR;
    }
  }
  
  // Validate Preprocessor Character (if any)...
  if (!preProcChar.IsBadUnicode()) {
    if ( !BEFE::IsNull(curChars.Find(preProcChar)) )
      errors.Append( LexCharMessage(Error::LexDupPreprocessorChar, preProcChar) );
    if (numErrors == errors.Length()) {
      status = curChars.Append(preProcChar);
      if (status) goto SOMEERROR;
    }
  }
  
  // Reserve first char for all operators/comments...
  for (i=0; i < curOps.Length(); i++) {
    c = curOps.Get(i).Get(0);
    if ( c != NUL &&
         LexIsSpecial(c) &&
         BEFE::IsNull(curChars.Find(c))
       )
      curChars += c;
  }
  
  // Handle errors
  if (errors.Length()) goto ERRORS;
  status = Error::None;
  while (false) {
    OK:        status = Error::None;              break;
    ERRORS:    status = Error::LexLangSpecErrors; break;
    SOMEERROR:                                    break;
  }

  // Make sure errors is set because we may have a generic status...
  if (status && status != Error::LexLangSpecErrors)
    errors.Append(ErrorString(status));

  // Tidy up flags based on status
  if (status) {
    isDirty = true;
    isValid = false;
  }
  else {
    isDirty = false;
    isValid = true;
  }
  
  return status;
  
}
  
//----------------------------------------------------------------------
//
// Generation
//

Status LexLanguage::Generate() {
  
  Status   status;
  UInt     i;
  Char     c;
  String   str;
  LexState state;
  Byte     b;
  Int      actionIdxOperator;
  Int      actionIdxComment;
  Int      actionIdxPreprocessor;
  Int      actionIdx;
  Strings  curOps;
  UInt     numOps;
  
  // Can't do it if not valid...
  if (!IsValid()) goto NOTVALID;

  // Populate the defaults
  status = PopulateDefaults();
  if (status) goto SOMEERROR;

  //
  // Build 'Main' state...
  //

  status = states.Get(Lex::StateMain, state);
  if (status) goto SOMEERROR;
  status = state.SetName("Main");
  if (status) goto SOMEERROR;
  
  state.byteActions[0x00] = Lex::ActionUTF;
  
  state.byteActions['\n'] = Lex::ActionNewLine;

  for (i=0; i < whiteChars.Length(); i++)
    state.byteActions[whiteChars.Get(i)] = Lex::ActionWhiteSpace;
  for (i=0; i < nameStart.Length(); i++)
    state.byteActions[nameStart.Get(i)] = Lex::ActionName;
  
  actionIdxOperator      = GetActionIndex("Operator");
  actionIdxComment       = GetActionIndex("Comment");
  actionIdxPreprocessor  = GetActionIndex("Preprocessor");
  if (BEFE::IsNull(actionIdxOperator)) goto INTERNAL;

  for (i=0; i < operators.Length(); i++) {
    status = curOps.Append(operators.Get(i));
    if (status) goto SOMEERROR;
  }

  for (i=0; i < comments.Length(); i++) {
    status = curOps.Append(comments.Get(i));
    if (status) goto SOMEERROR;
  }

  curOps.Sort();
  numOps = curOps.Length();
  for (i=0; i < numOps; i++) {
    str = curOps.Get(i);
    if (str.Length() > 1)
      actionIdx = 0xFF;
    else
      actionIdx = (operators.Contains(str)) ? actionIdxOperator : actionIdxComment;
    b = str.Get(0);
    if (b > 0x7f)
      BEFE_ERROR("LexLanguage.Generate: byte > 0x7f!!!", Error::Internal);
    else
      state.byteActions[(Int)b] = actionIdx;
  }
  
  if (!preProcChar.IsBadUnicode())
    state.byteActions[(Int)preProcChar] = actionIdxPreprocessor;
  
  for (i=0; i < brackPairs.Length(); i++) {
    str = brackPairs.Get(i);
    b = str.Get(0);
    if (b > 0x7f)
      BEFE_ERROR("LexLanguage.Generate: byte > 0x7f!!!", Error::Internal);
    else
      state.byteActions[(Int)b] = Lex::ActionBracketLeft;
    b = str.Get(1);
    if (b > 0x7f)
      BEFE_ERROR("LexLanguage.Generate: byte > 0x7f!!!", Error::Internal);
    else
      state.byteActions[(Int)b] = Lex::ActionBracketRight;    
  }

  for (i=0; i < sepChars.Length(); i++)
    state.byteActions[sepChars.Get(i)] = Lex::ActionSeparator;
  
  for (i=0; i < quoteChars.Length(); i++)
    state.byteActions[quoteChars.Get(i)] = Lex::ActionQuote;
  
  // NOTE: We probably shouldn't hard code these don't think it'll hurt
  //       for now...
  for (i='0'; i <= '9'; i++)
    state.byteActions[i] = Lex::ActionNumber;
  
  // Flush Main state...
  status = states.Set(Lex::StateMain, state);
  if (status) goto SOMEERROR;
  
  //
  // Next, Build the 'Name' state...
  //
  
  status = states.Get(Lex::StateName, state);
  if (status) goto SOMEERROR;
  
  status = state.SetName("Name");
  if (status) goto SOMEERROR;
  
  MemoryFillRaw(&state.byteActions[0], Lex::ActionFlushMain, sizeof(state.byteActions));
  for (i=0; i < nameChars.Length(); i++)
    state.byteActions[nameChars.Get(i)] = Lex::ActionAppend;
  
  // Flush Name state...
  status = states.Set(Lex::StateName, state);
  if (status) goto SOMEERROR;
  
  //
  // Next, Build the 'Number' state...
  //
  
  status = states.Get(Lex::StateNumber, state);
  if (status) goto SOMEERROR;
  
  status = state.SetName("Number");
  if (status) goto SOMEERROR;
  
  MemoryFillRaw(&state.byteActions[0], Lex::ActionFlushMain, sizeof(state.byteActions));
  for (i='0'; i <= '9'; i++)
    state.byteActions[i] = Lex::ActionAppend;
  for (i=0; i < numberChars.Length(); i++)
    state.byteActions[numberChars.Get(i)] = Lex::ActionAppend;
  
  // Flush Name state...
  status = states.Set(Lex::StateNumber, state);
  if (status) goto SOMEERROR;
  
  //
  // Finally, populate the default multi-Byte operators...
  //
  
  status = PopulateDefaultOps();
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    NOTVALID:  status = Error::LexLangSpecErrors; break;
    INTERNAL:  status = Error::Internal;          break;
    SOMEERROR:                                    break;
  }
  
  return status;
  
}
  
//----------------------------------------------------------------------
//
// Protected Internal Methods
//

Status LexLanguage::PopulateDefaults() { // LexLanguage.PopulateDefaults...
  
  Status status;
  UInt   i;
  
  // Nothing to do if dirty or already populated...
  if (isDirty || (UInt)stateHiId || (UInt)actionHiId) goto OK;
  
  // Clear all generated stuff
  status = SetEmpty();
  if (status) goto SOMEERROR;
  
  //
  // Populate default states
  //
  {
    
    LexState           state;
    static char const *stateNames[] = {
      "Main",
      "Name",
      "Number",
      0};
      
    stateHiId = 0;
    for (i=0; stateNames[i]; i++) {
      state.Reset();
      state.id = ++stateHiId;
      state.name = stateNames[i];
      state.type = LexState::TypeByte;
      status = states.Append(state);
      if (status) goto SOMEERROR;
    }
    
  }
  
  //
  // Populate default actions
  //
  {
    LexAction           action;
    static struct {char const *name; LexCharAction handler;} actionDefs[] = {
      {"UTF",             LexLangDefaultUTF},
      {"NewLine",         LexLangDefaultNewLine},
      {"WhiteSpace",      LexLangDefaultWhiteSpace},
      {"Name",            LexLangDefaultName},
      {"Number",          LexLangDefaultNumber},
      {"Quote",           LexLangDefaultQuote},
      {"Operator",        LexLangDefaultOperator},
      {"Comment",         LexLangDefaultComment},
      {"Preprocessor",    LexLangDefaultPreprocessor},
      {"BracketLeft",     LexLangDefaultBracketLeft},
      {"BracketRight",    LexLangDefaultBracketRight},
      {"Separator",       LexLangDefaultSeparator},
      {"Escape",          LexLangDefaultEscape},
      {"FlushMain",       LexLangDefaultFlushMain},
      {"Append",          LexLangDefaultAppend},
      {"AppendFlushMain", LexLangDefaultAppendFlushMain},
      {0,0}};
      
    actionHiId = 0;
    for (i=0; actionDefs[i].name; i++) {
      action.id          = ++actionHiId;
      action.name        = actionDefs[i].name;
      action.charHandler = actionDefs[i].handler;
      status = actions.Append(action);
      if (status) goto SOMEERROR;
    }
  }
  
  // Now, rebuild the indexes...
  status = RebuildIndexes();
  if (status) goto SOMEERROR;
  
  // And clear isDirty...
  isDirty = false;
  
  // Handle errors
  while (false) {
    OK:        status = Error::None;
    SOMEERROR: break;
  }

  return status;
  
} // ...LexLanguage.PopulateDefaults

Status LexLanguage::PopulateDefaultOps() { // LexLanguage.PopulateDefaultOps...

  Status    status;
  Strings   ops;
  String    op;
  UInt      opLen;
  UInt      maxLen;
  UInt      i;
  UInt      j;
  LexState  mainState;
  UInt      numMainUpdates;
  Byte      firstByte;
  String    stateName;
  LexState  state;
  LexAction action;
  Int       stateIdx;
  Int       nextStateIdx;
  Int       actionIdx;
  
  // Figure out maximum operator length...
  ops = operators;
  for (i=0; i < comments.Length(); i++) {
    status = ops.Append(comments.Get(i));
    if (status) goto SOMEERROR;
  }
  maxLen = 0;
  for (i=0; i < ops.Length(); i++)
    maxLen = Max(maxLen, ops.Get(i).Length());
    
  // Nothing to do if no multi-Byte operators...
  if (maxLen < 1) goto OK;

  //
  // First pass makes all the states needed
  //
  
  for (i=0; i < ops.Length(); i++) { // For each operator...
    
    op = ops.Get(i);
    if (op.Length() < 2) continue;

    opLen = op.Length();
    // TEMP...
    //for (j=2; j < opLen-1; j++) { // For each depth...
    for (j=0; j < opLen-1; j++) { // For each depth...
    // ...TENP
      state.Reset();
      state.type = LexState::TypeByte;
      state.name = String("Op")+op.Get(Range(0,j));
      if (!StateExists(state.name)) {
        MemoryFillRaw(&state.byteActions[0], Lex::ActionFlushMain, sizeof(state.byteActions));
        status = NewState(state);
        if (status) goto SOMEERROR;
      }
    } // ...For each depth

  } // ...For each operator
  
  //
  // Second pass updates all the states
  //
  
  // TEMP...
  /**/
  // ...TEMP
  for (i=0; i < ops.Length(); i++) { // For each operator...
  
    op = ops.Get(i);
    if (op.Length() < 2) continue;
    
    opLen = op.Length();
    for (j=0; j < opLen-1 ;j++) { // For each depth...
      
      // Calculate State and Action names
      state.Reset();
      action.Reset();
      state.name  = String("Op")+op.Get(Range(0,j));
      
      if (j == opLen-2) {
        if (comments.Contains(op))
          action.name = "Comment";
        else
          action.name = "AppendFlushMain";
      }
      else
        action.name = String();
        
      // Get the State and Action indexes
      stateIdx = GetStateIndex(state.name);
      if (BEFE::IsNull(stateIdx)) goto STATEMISS;
      if (action.name.IsNull()) {
        nextStateIdx = GetStateIndex(String("Op")+op.Get(Range(0,j+1)));
        if (BEFE::IsNull(nextStateIdx)) goto STATEMISS;
        actionIdx = 0x80 | nextStateIdx;
      }
      else {
        actionIdx = GetActionIndex(action.name);
        if (BEFE::IsNull(actionIdx)) goto ACTIONMISS;
      }
      
      // Update the state
      status = GetState(state.name, state);
      if (status) goto SOMEERROR;
      state.byteActions[op.Get(j+1)] = actionIdx;
      status = UpdateStateById(state);
      if (status) goto SOMEERROR;
      
    } // ...For each depth

  } // ...For each operator

  // TEMP...
  /**/
  // ...TEMP

  //
  // Finally, whip through all the operators and fill in the "Main" state...
  //
  
  // Sort the operators so the shortest duplicate first char come first...
  status = ops.CopyFrom(operators);
  if (status) goto SOMEERROR;
  status = ops.Sort();
  if (status) goto SOMEERROR;

  // Get the "Main" state
  status = GetState("Main", mainState);
  if (status) goto SOMEERROR;
  numMainUpdates = 0;
  
  // For each operator...  
  for (i=0; i < ops.Length(); i++) {
  //if (false) {
    
    op = ops.Get(i);
    firstByte = (Byte)op.Get(0);
    if (firstByte >= 0x80) goto INTERNAL;
    
    // If single character operator without multiple ones that start with
    // the same character...
    if (i == ops.Length()-1 || (Byte)ops.Get(i+1).Get(0) == firstByte) {
      mainState.byteActions[firstByte] = Lex::ActionOperator;
      numMainUpdates++;
    }
    
    // Else, it's got multiple operators that start with same character...
    else {
      stateName = "Op";
      stateName += Char(firstByte);
      stateIdx = GetStateIndex(stateName);
      if (!BEFE::IsNull(stateIdx) /*&& mainState.byteActions[firstByte] == 0xFF*/)
        mainState.byteActions[firstByte] = 0x80 | (Byte)stateIdx;
    }
    
  }
  
  // Update the "Main" state...
  if (numMainUpdates) {
    status = UpdateStateById(mainState);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:         status = Error::None;              break;
    STATEMISS:  status = Error::LexStateNotExist;  break;
    ACTIONMISS: status = Error::LexActionNotExist; break;
    INTERNAL:   status = Error::Internal;          break;
    SOMEERROR:                                     break;
  }
  
  return status;

} // ...LexLanguage.PopulateDefaultOps

Status LexLanguage::RebuildIndexes() {
  
  Status     status;
  LexState  *state;
  LexAction *action;
  UInt       i;
  
  // Clear them first...
  status = stateId2Idx.Reset();
  if (status) goto SOMEERROR;
  status = stateName2Id.Reset();
  if (status) goto SOMEERROR;
  status = stateId2Name.Reset();
  if (status) goto SOMEERROR;
  status = actionId2Idx.Reset();
  if (status) goto SOMEERROR;
  status = actionName2Id.Reset();
  if (status) goto SOMEERROR;
  status = actionId2Name.Reset();
  if (status) goto SOMEERROR;

  // Populate state indexes...
  for (i=0; i < states.Length(); i++) {
    state = (LexState *)states._GetAddress(i);
    if (BEFE::IsNull(state)) goto NULLPOINTER;
    if (!BEFE::IsNull((UInt)state->id)) {
      status = stateId2Idx.Set((UInt)state->id, i);
      if (status) goto SOMEERROR;
      if (!state->name.IsNull()) {
        status = stateName2Id.Set(state->name, state->id);
        if (status) goto SOMEERROR;
        status = stateId2Name.Set((UInt)state->id, state->name);
        if (status) goto SOMEERROR;
      }
    }
  }
  
  // Populate action indexes...
  for (i=0; i < actions.Length(); i++) {
    action = (LexAction *)actions._GetAddress(i);
    if (BEFE::IsNull((void *)action)) goto NULLPOINTER;
    if (!BEFE::IsNull((UInt)action->id)) {
      status = actionId2Idx.Set((UInt)action->id, i);
      if (status) goto SOMEERROR;
      if (!action->name.IsNull() ) {
        status = actionName2Id.Set(action->name, action->id);
        if (status) goto SOMEERROR;
        status = actionId2Name.Set((UInt)action->id, action->name);
        if (status) goto SOMEERROR;
      }
    }
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }

  return status;
  
}

} // ...Namespace BEFE
