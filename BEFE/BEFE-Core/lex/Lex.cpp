//----------------------------------------------------------------------
// File: Lex.cpp - Implementation of the BEFE Lex Class
//----------------------------------------------------------------------
//
// This file contains the C++ implementation of the BEFE Lex Class
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Lex.h"
#include "LexWorkspace.h"
#include "LexStream.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Lex::Lex() {
	StartUp();
}

Lex::Lex(Lex const &that) {
	ShutDown();
}

Lex::~Lex() {
	ShutDown();
}

Lex &Lex::operator=(Lex const &that) {
	return *this;
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Lex::StartUp() {
	
	Status retStatus;
	Status status;

  isConsumable = false;
  highWorkspaceId = 0;
	retStatus = workspaceIds.StartUp();
	status    = idWorkspaces.StartUp();
  highStreamId = 0;
	retStatus = streamIds.StartUp();
	status    = idStreams.StartUp();
	if (status && !retStatus) retStatus = status;
  highLanguageId = 0;
  status    = languages.StartUp();
  if (status && !retStatus) retStatus = status;
  status    = languages.SetSensitivity(false);
  if (status && !retStatus) retStatus = status;

  return retStatus;
	
}

Status Lex::ShutDown() {
	
	Status retStatus;
	Status status;

  retStatus = DetachWorkspaces();
  status = DetachStreams();
	if (status && !retStatus) retStatus = status;
  status = streamIds.ShutDown();
	if (status && !retStatus) retStatus = status;
  status = idStreams.ShutDown();
	if (status && !retStatus) retStatus = status;
  status = languages.ShutDown();
	if (status && !retStatus) retStatus = status;
	
	return retStatus;
	
}

Status Lex::Clear() {
	
	Status status;
	
	status = Reset();
	
	return status;
	
}

Status Lex::Reset() {
	
	Status retStatus;
	Status status;
	
	retStatus = ShutDown();
	status = StartUp();
	if (status && !retStatus) retStatus = status;
	
	return retStatus;
	
}

Boolean Lex::IsNull() const {
  Boolean result;
  if (!BEFE::IsNull(highStreamId) ||
      !streamIds.IsNull()         ||
      !idStreams.IsNull()         ||
      !languages.IsNull()         
     )
    result = false;
  else
    result = true;
  
	return result;
  
}

Status Lex::SetNull() {
	return Error::NotApplicable;
}

Boolean Lex::IsEmpty() {
	return false;
}

Status Lex::SetEmpty() {
	return Error::NotApplicable;
}

Status Lex::MoveFrom(Lex const &that) {
	return Error::NotCapable;
}

Status Lex::CopyFrom(Lex const &that) {
	return Error::NotCapable;	
}

Boolean Lex::IsConsumable() const {
	return isConsumable;
}

Lex &Lex::Consumable() const {
  ((Lex *)this)->isConsumable = true;
	return *((Lex *)this);
}

Lex &Lex::NotConsumable() const {
	((Lex *)this)->isConsumable = false;
	return *((Lex *)this);
}

//----------------------------------------------------------------------
//
// LexWorkspace Management
//

Status Lex::IsAttached(LexWorkspace &theWs) const {
  
  if (workspaceIds.Exists(&theWs))
    return true;
  else
    return false;

}

Status Lex::Attach(LexWorkspace &theWs) {

  Status status;

  if (!BEFE::IsNull(*(void **)&theWs.lex) && theWs.lex != this) goto ATTACHEDOTHER;
  if (!BEFE::IsNull((void *)&theWs.lex) && workspaceIds.Exists((UInt)&theWs)) goto ALREADYATTACHED;
  status = theWs._Attach(*this);
  
  status = workspaceIds.Set((UInt)&theWs, ++highWorkspaceId);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    ALREADYATTACHED: status = Error::LexWorkspaceIsAttached;    break;
    ATTACHEDOTHER:   status = Error::LexWorkspaceAttachedOther; break;
    SOMEERROR:                                                  break;
  }
  
	return status;
  
}

Status Lex::Detach(LexWorkspace &theWs) {
  
  Status status;
  
  if (!BEFE::IsNull((void *)theWs.lex) && theWs.lex != this) goto ATTACHEDOTHER;
  if (!workspaceIds.Exists((UInt)&theWs)) goto NOTATTACHED;
  status = workspaceIds.Remove((UInt)&theWs);
  if (status) goto SOMEERROR;
  status = theWs._Detach();
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    ATTACHEDOTHER: status = Error::LexWorkspaceAttachedOther; break;
    NOTATTACHED:   status = Error::LexWorkspaceNotAttached;   break;
    SOMEERROR:                                                break;
  }
  
	return status;
  
}
  
//----------------------------------------------------------------------
//
// LexStream Management Methods...
//

Boolean Lex::IsAttached(LexStream &theStream) const {
  
  if (streamIds.Exists((UInt)&theStream))
    return true;
  else
    return false;

}

Status Lex::Attach(LexStream &theStream) {
  
  Status status;

  if (!BEFE::IsNull(*(void **)&theStream.lex) && theStream.lex != this) goto ATTACHEDOTHER;
  if (!BEFE::IsNull((void *)&theStream.lex) && streamIds.Exists((UInt)&theStream)) goto ALREADYATTACHED;
  status = theStream._Attach(*this);
  
  status = streamIds.Set((UInt)&theStream, ++highStreamId);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    ALREADYATTACHED: status = Error::LexStreamIsAttached;    break;
    ATTACHEDOTHER:   status = Error::LexStreamAttachedOther; break;
    SOMEERROR:                                               break;
  }
  
	return status;
  
}

Status Lex::Detach(LexStream &theStream) {
  
  Status status;
  
  if (!BEFE::IsNull((void *)theStream.lex) && theStream.lex != this) goto ATTACHEDOTHER;
  if (!streamIds.Exists((UInt)&theStream)) goto NOTATTACHED;
  status = streamIds.Remove((UInt)&theStream);
  if (status) goto SOMEERROR;
  status = theStream._Detach();
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    ATTACHEDOTHER: status = Error::LexStreamAttachedOther; break;
    NOTATTACHED:   status = Error::LexStreamNotAttached;   break;
    SOMEERROR:                                             break;
  }
  
	return status;
  
}

//----------------------------------------------------------------------
//
// Protected Methods...
//

Status Lex::DetachWorkspaces() {
	
	Status status;
	Id     workspaceId;
  union {
  	LexWorkspace *theWorkspace;
    UInt          theUIntWorkspace;
  };

  while (workspaceIds.Length()) {
    //theUIntWorkspace = (UInt)&theWorkspace;
		status = workspaceIds.GetPhysical(0, theUIntWorkspace, workspaceId);
		if (status) goto SOMEERROR;
		if (!BEFE::IsNull((void *)theWorkspace)) {
			status = theWorkspace->_Detach();
			if (status) goto SOMEERROR;
			status = workspaceIds.Remove(0);
			if (status) goto SOMEERROR;
		}
	}
	
	// Handle errors
	status = Error::None;
	while (false) {
		SOMEERROR: break;
	}
	
	return status;
	
}

Status Lex::DetachStreams() {
	
	Status status;
	Id     streamId;
  union {
  	LexStream *theStream;
    UInt       theUIntStream;
  };

  while (streamIds.Length()) {
    //theUIntStream = (UInt)&theStream;
		status = streamIds.GetPhysical(0, theUIntStream, streamId);
		if (status) goto SOMEERROR;
		if (!BEFE::IsNull((void *)theStream)) {
			status = theStream->_Detach();
			if (status) goto SOMEERROR;
			status = streamIds.Remove((UInt)theStream);
			if (status) goto SOMEERROR;
		}
	}
	
	// Handle errors
	status = Error::None;
	while (false) {
		SOMEERROR: break;
	}
	
	return status;
	
}

//----------------------------------------------------------------------
//
// LexLanguage Management
//

UInt Lex::GetLanguageCount() {
  return languages.Length();
}

Id Lex::GetLanguageId(String const &name) {
  
  Id theId;
  
  BEFE::SetNull(theId);
  
  return theId;
}

Status Lex::GetLanguageId(String const &name, Id &langId) {
  return Error::NotImplemented;
}

UInt Lex::GetLanguageIndex(String const &name) {
  return UNaN;
}

Status Lex::GetLanguageIndex(String const &name, UInt &langIndex) {
  return Error::NotImplemented;
}

Status Lex::GetLanguage(String const &name, LexLanguage &lang) {
  
  Status status;
  
  if (!languages.HasKey(name)) {
    lang.Reset();
    status = Error::LexLangNotRegistered;
  }
  else {
    lang = languages.Get(name);
    status = Error::None;
  }
  
  return status;
  
}

Status Lex::RegisterLanguage(LexLanguage &lang) {
  
  Status       status;
  String       langName;
  
  langName = lang.GetName();
  if (BEFE::IsEmpty(langName)) goto NONAME;
  if (languages.HasKey(langName)) goto EXISTS;
  if (!lang.IsValid()) goto NOTVALID;
  
  // Give it an Id if it doesn't have one...
  if (BEFE::IsNull(lang.GetId())) {
     status = lang.SetId(++highLanguageId);
     if (status) goto SOMEERROR;
  }
  
  // Save it
  status = languages.Set(langName, lang);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NONAME:      status = Error::LexStreamNoLangName;  break;
    EXISTS:      status = Error::LexLangIsRegistered;  break;
    NOTVALID:    status = Error::LexLangSpecErrors;    break;
    SOMEERROR:                                             break;
  }
  
  return status;
  
}

Status Lex::UnRegisterLanguage(String const &langName) {
  
  Status status;
  
  if (!languages.HasKey(langName)) goto NOTEXIST;
  status = languages.Remove(langName);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOTEXIST:  status = Error::LexLangNotRegistered; break;
    SOMEERROR:                                       break;
  }
  
  return status;
  
}

Strings Lex::GetLanguageNames() const {
  
  Strings keys;
  
  keys = languages.Keys();
  
  return keys.Consumable();
  
}

} // ...Namespace BEFE
