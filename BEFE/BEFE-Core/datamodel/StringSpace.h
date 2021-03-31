//----------------------------------------------------------------------
// File: StringSpace.h - Declarations for the StringSpace Class
//----------------------------------------------------------------------
//
// The purpose of the StringSpace class is to maintain a list of distinct
// (eg, never reused) identifiers for instances of Strings.
//
// As such, it is an IdSpace.  It is, however, built using extra
// functionality on top of an IdSpace.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef STRINGSPACE_H // STRINGSPACE_H...
#define STRINGSPACE_H

#include "Primitives.h"
#include "IdSpace.h"

namespace BEFE { // Namespace BEFE...

class StringSpace { // Class StringSpace...

  //
  // C++ Lifecycle
  //

  public: StringSpace();
  public: StringSpace(String const desc);
  public: StringSpace(StringSpace const &that);
  public: StringSpace &operator= (StringSpace const &that);
  public: ~StringSpace();

  //
  // BEFE Lifecycle
  //

  public: Status StartUp();
  public: Status StartUp(String const &desc);
  public: Status MoveFrom(StringSpace const &thatspace);
  public: Status CopyFrom(StringSpace const &thatspace);
  public: Status ShutDown();

  //
  // Query/Update methods
  //

  // StringSpace Query methods
  public: Id     GetSpaceId();
  public: String GetSpaceName();
  public: String GetSpaceDescription();

  // String Query methods
  public: Id     FindString(String const &that);

  // Active Invasive methods (counts as "Update")
  public: Id NewString(String const &that);
  public: Id KillId(Id theid);

  //
  // Private members
  //

  private: Id     spaceId;
  private: String desc;

}; // ...Class StringSpace

} // ...Namespace BEFE

#endif // ...STRINGSPACE_H
