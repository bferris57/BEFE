//!befe-publish inc
//----------------------------------------------------------------------
// File: IdSpaceInfo.h - Declarations for the IdSpaceInfo Class
//----------------------------------------------------------------------
//
// This class is a <Foundation Class> for <Id Spaces>. As such, it
// appears throughout the product when manipulating <Ids> and
// <Id Spaces>.
//
// The purpose of the IdSpaceInfo class is to maintain a set of
// <Descriptive> values of an <IdSpace>.
//
// These values are descriptive purposes only.  As such, and at most,
// they contain a "snapshot" of an IdSpace at a given point in time.
//
// A "hidden" copy of these values are maintained under the covers
// by the IdSpace implementation itself.  So, all the consumer ever
// sees is a passive descriptive snapshot of these values.  So, they
// can change them all they like but all they're doing is damaging
// the snapshot, NOT the real thing.  ;-)
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef IDSPACEINFO_H // IDSPACEINFO_H...
#define IDSPACEINFO_H

#include "Primitives.h"
#include "Id.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class IdSpaceInfo
//

class IdSpaceInfo { // Class IdSpaceInfo...

  //
  // Public members
  //

  public: Id     id;
  public: String shortName;
  public: String description;

  public: Time   creationTime;
  public: Time   accessTime;
  public: Time   updateTime;

  public: Id     nextId;
  public: URange idRange;
  public: Int    idCount;

}; // ...Class IdSpaceInfo

} // ...Namespace BEFE

#endif // ...IDSPACEINFO_H