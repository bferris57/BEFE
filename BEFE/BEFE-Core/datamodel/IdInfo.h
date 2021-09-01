//!befe-publish inc
//----------------------------------------------------------------------
// File: IdInfo.h - Declarations for the IdInfo Class
//----------------------------------------------------------------------
//
// The purpose of the IdInfo class is to maintain the timestamp info
// for each Id managed by an IdSpace.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef IDINFO_H // IDINFO_H...
#define IDINFO_H

#include "Primitives.h"
#include "Id.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class IdInfo
//

class IdInfo { // Class IdInfo...

  //
  // Public members
  //

  public: Time   creationTime;
  public: Time   accessTime;
  public: Time   updateTime;

}; // ...Class IdInfo

} // ...Namespace BEFE

#endif // ...IDINFO_H