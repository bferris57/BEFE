//----------------------------------------------------------------------
// File: IdSpace.cpp - Default implementation of the IdSpace Class
//----------------------------------------------------------------------
//
// The IdSpace class is a base clase for all IdSpaces.  As such, it
// implements default behaviour which is expected to be overriden by
// specialisations of this class.
//
// NOTE: For now, we don't handle Dead Ids.  That will come when we
//       really need them.  I haven't convinced myself it's readlly
//       needed at this point or if it's just a theoretical nicety.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "IdSpace.h"
#include "IdSpaceInfoExt.h"

namespace BEFE { // Namepace BEFE...

//----------------------------------------------------------------------
//
// Class (static) members and methods
//

static IdSpaceInfoExt theIdSpace;

//----------------------------------------------------------------------
//
// Published (public) Class Methods
//
// The Class Methods are considered "Non-Passive", signifying that they
// do NOT update any timestamps in the IdSpaces themselves.
//

IdSpace *TheIdSpace;

Status IdSpace::ClassStartUp() {

  Status status;
  String name;
  String desc;

  if (TheIdSpace) goto STARTUPALREADY;

  //
  // Allocate and populate initial IdSpace
  //

  theIdSpace.StartUp();
  theIdSpace.info.shortName = "IdSpace";
  theIdSpace.info.description = "The BEFE IdSpace of IdSpaces";
  status = IdSpace::New(theIdSpace.info.shortName,theIdSpace.info.description,theIdSpace.info.id);
  if (status) goto SOMEERROR;
  status = GetIdSpaceInfo((Int)0,theIdSpace.info);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    STARTUPALREADY: status = Error::AlreadyClassStartUp; break;
    SOMEERROR:                                           break;
  }

  return status;

}

Status IdSpace::New(String shortname, String description, Id &newspaceid) {

  Status          status;
  IdSpaceInfoExt *that;

  newspaceid = UNaN;

  // Make sure we're not at the last Id...
  if ((UInt)theIdSpace.info.nextId == UNaN-1) goto SPACEFULL;

  // Get the next Id
  newspaceid = theIdSpace.info.nextId;
  theIdSpace.info.nextId = (UInt)newspaceid + 1;

  // If not IdSpace 0, Append it and get it's address...
  if ((Int)newspaceid != 0) {
    status = theIdSpace.liveIds._Append((UInt)newspaceid,(Byte *)NULL);
    if (status) goto SOMEERROR;
    that = (IdSpaceInfoExt *)theIdSpace.liveIds._GetLogAddress(newspaceid);
    if (!that) goto NULLPOINTER;
    // Start it up...
    status = that->StartUp();
    if (status) goto SOMEERROR;
    that->info.shortName   = shortname;
    that->info.description = description;
  }
  else
    that = &theIdSpace;

  // Handle errors
  status = Error::None;
  while (false) {
    SPACEFULL:   status = Error::IdSpaceFull;         break;
    SOMEERROR:                                        break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
  }

  return status;

}

Status IdSpace::Free(Id spaceid) {
  BEFE_WARN("IdSpace.Free not implemented yet");
  return Error::NotImplemented;
}

Status IdSpace::ClassShutDown() {

  Status status;

  if (!TheIdSpace) goto ALREADYSHUTDOWN;

  theIdSpace.ShutDown();
  TheIdSpace = NULL;

  // Handle errors
  status = Error::None;
  while (false) {
    ALREADYSHUTDOWN: status = Error::AlreadyClassShutDown; break;
  }

  return status;

}

Status IdSpace::GetIdSpaceInfo(Id spaceid,IdSpaceInfo &info) {

  Status          status;
  _SparseBase    *ids[] = {&theIdSpace.liveIds,/*&theIdSpace.deadIds*/NULL,NULL};
  Int             i;
  IdSpaceInfoExt *theinfo;

  if (!TheIdSpace) goto UNKNOWNID;

  // Special case for Id 0 (the IdSpace of IdSpaces)...
  if ((Int)spaceid == 0) {
    info = theIdSpace.info;
    goto OK;
  }

  // Try each list
  for (i=0; ids[i]; i++) {
    if (ids[i]->Exists((Int)spaceid))
      break;
  }
  if (!ids[i]) goto UNKNOWNID;

  // Get the info
  theinfo = (IdSpaceInfoExt *)ids[i]->_GetLogAddress((Int)spaceid);
  if (!theinfo) goto NULLPOINTER;

  // Give a copy to the caller
  info = theinfo->info;

  // Handle errors
  status = Error::None;
  while (false) {
    UNKNOWNID: status = Error::IdUnknown;             break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    OK:          status = Error::None;                break;
  }

  return status;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//
// Published (public) Instance methods
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// Active non-Invasive methods
//

UInt IdSpace::Length() {
  BEFE_WARN("IdSpace.GetCount not implemented yet");
  return UNaN;
}

URange IdSpace::IdRange() {
  BEFE_WARN("IdSpace.IdRange not implemented yet");
  return URange();
}


Boolean IdSpace::Exists(Id theid) {
  BEFE_WARN("IdSpace.Exists not implemented yet");
  return false;
}

Status IdSpace::Info(IdSpaceInfo &info) {
  BEFE_WARN("IdSpace.Info not implemented yet");
  return Error::NotImplemented;
}

//----------------------------------------------------------------------
//
// Active Invasive methods
//

Id IdSpace::NewId() {
  BEFE_WARN("IdSpace.GetNextSpaceId not implemented yet");
  return Id();
}

Status IdSpace::KillId(Id theid) {
  BEFE_WARN("IdSpace.KillId not implemented yet");
  return Error::NotImplemented;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//
// Unpublished (protected) Instance methods
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// IdSpace C++ Lifecycle
//

IdSpace::IdSpace() {
  if (TheIdSpace) StartUp();
}

IdSpace::IdSpace(IdSpace const &that) {
  BEFE_WARN("IdSpace.IdSpace copy constructor invoked");
}

IdSpace &IdSpace::operator= (IdSpace const &that) {
  BEFE_WARN("IdSpace C++ operator= invoked");
  return *this;
}

IdSpace::~IdSpace() {
  BEFE_WARN("IdSpace C++ Destructor invoked");
}

//----------------------------------------------------------------------
//
// IdSpace BEFE Lifecycle
//

Status IdSpace::StartUp() {
  if (!TheIdSpace)
    TheIdSpace = this;
  //info.id = 0;
  info.creationTime.Set();
  info.updateTime.Set();
  info.nextId = 0;
  return Error::None;
}

Status IdSpace::MoveFrom(IdSpace const &that) {
  BEFE_WARN("IdSpace.MoveFrom Not Implemented Yet");
  return Error::NotImplemented;
}

Status IdSpace::CopyFrom(IdSpace const &that) {
  BEFE_WARN("IdSpace.CopyFrom Not Implemented Yet");
  return Error::NotImplemented;
}

Status IdSpace::ShutDown() {

  if (TheIdSpace == this)
    TheIdSpace = NULL;

  return Error::None;

}

} // Namespace BEFE
