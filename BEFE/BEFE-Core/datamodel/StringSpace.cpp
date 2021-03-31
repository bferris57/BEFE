//----------------------------------------------------------------------
// File: StringSpace.cpp - Implementation of the StringSpace class
//----------------------------------------------------------------------
//
// The StringSpace class is used to maintain a "dictionary" of distinct
// strings, each identified by an Id.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "StringSpace.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

StringSpace::StringSpace() {
  StartUp();
}

StringSpace::StringSpace(String const desc) {
  StartUp(desc);
}

StringSpace::StringSpace(StringSpace const &that) {
  BEFE_WARN("StringSpace.StringSpace(const &that) not implemented yet");
}

StringSpace &StringSpace::operator= (StringSpace const &that) {
  BEFE_WARN("StringSpace.operator= not implemented yet");
  return *this;
}

StringSpace::~StringSpace() {
  Status status;
  if (TheBefe->TheIdSpace && !IsNull(spaceId) ) {
    status = TheBefe->TheIdSpace->KillId(spaceId);
    if (status) BEFE_ERROR("TheIdSpace.KillId", status);
  }
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status StringSpace::StartUp() {

  Status status;
  String message;

  desc = "";
  spaceId = UNaN;
  status = IdSpace::New(String("StringSpace"), desc, spaceId);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR: BEFE_ERROR("IdSpace.New",status); break;
  }

  return status;

}

Status StringSpace::StartUp(String const &desc) {

  Status status;

  this->desc = desc;
  status = IdSpace::New(String("StringSpace"), desc, spaceId);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR: BEFE_ERROR("IdSpace.New",status); break;
  }

  return status;

}

Status StringSpace::MoveFrom(StringSpace const &that) {
  BEFE_WARN("StringSpace.MoveFrom not implemented yet");
  return Error::NotImplemented;
}

Status StringSpace::CopyFrom(StringSpace const &that)  {
  BEFE_WARN("StringSpace.ShutDown not implemented yet");
  return Error::NotImplemented;
}

Status StringSpace::ShutDown() {
  BEFE_WARN("StringSpace.ShutDown not implemented yet");
  return Error::NotImplemented;
}

//----------------------------------------------------------------------
// StringSpace Query methods
//

Id StringSpace::GetSpaceId() {
  BEFE_WARN("StringSpace.GetSpaceId not implemented yet");
  return Id();
}

String StringSpace::GetSpaceName() {
  BEFE_WARN("StringSpace.GetSpaceName not implemented yet");
  return String();
}

String StringSpace::GetSpaceDescription() {
  BEFE_WARN("StringSpace.GetSpaceDescription not implemented yet");
  return String();
}

//----------------------------------------------------------------------
//
// String Query methods
//

Id StringSpace::FindString(String const &that) {
  BEFE_WARN("StringSpace.FindString not implemented yet");
  return Id();
}

//----------------------------------------------------------------------
//
// Active Invasive methods (counts as "Update")
//

Id StringSpace::NewString(String const &that) {
  BEFE_WARN("StringSpace.NewString not implemented yet");
  return Id();
}

Id StringSpace::KillId(Id theid) {
  BEFE_WARN("StringSpace.KillId not implemented yet");
  return Id();
}

} // ...Namespace BEFE
