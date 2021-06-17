//!befe-publish inc
//----------------------------------------------------------------------
// File: IdSpace.h - Declarations for the IdSpace Class
//----------------------------------------------------------------------
//
// This class is a <Foundation Class> for <Id Spaces>. As such, it
// appears throughout the product when manipulating <Ids> and
// <Id Spaces>.
//
// The purpose of the IdSpace class is to maintain a list of distinct
// (eg, never reused) identifiers for instances of things.
//
// *** See technical documentation for a more detailed description ***
//
// Note: At the risk of being redudant (it's in the tech docs), the
//       semantics of the zero Id and the NaN Id are reserved for use
//       and defined by each specific IdSpace.
//
//       In the "IdSpace" Idspace...
//
//         0         => The "IdSpace" IdSpace
//         1..UNaN-1 => And Id in the "IdSpace" IdSpace
//         UNaN      => Not defined in the "IdSpace" IdSpace
//
//       But, it's up to each IdSpace instance to declare the meaning
//       of these "special Ids" in the context of that IdSpace.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef IDSPACE_H // IDSPACE_H...
#define IDSPACE_H

#include "Primitives.h"
#include "Id.h"
#include "IdSpaceInfo.h"
#include "IdInfo.h"

namespace BEFE { // Namespace BEFE...

// Playing around with persistence thinking here...
namespace IdScope { // Namespace IdScope...

  // Persistence (and ID space) scope
  enum {
     None         // Probably on the stack, comes and goes
    ,Thread       // Thread specific (exclusive)
    ,Process      // For all threads in this process (available in shared memory)
    ,Temporary    // Maybe on the hard disk to pick up later if it's there
    ,Host         // The host it was created on
    ,Cluster      // Cluster of hosts (probably accessing it in parallel)
    ,Local        // Local use only, like LAN, company, etc.
    ,Community    // Within an identified community
    ,World        // For the whole world
    ,Universal    // Fat chance!
  };

} // ...Namespace IdScope

//----------------------------------------------------------------------
//
// Class IdSpace
//

class IdSpace { // Class IdSpace...

  // Friend classes
  friend class Process;

  //
  // Public Class methods
  //

  // Class BEFE Lifecycle
  public: static Status  ClassStartUp();
  public: static Status  New(String shortname, String Description, Id &spaceid);
  public: static Status  Free(Id spaceid);
  public: static Status  ClassShutDown();

  // Non-Active (Doesn't count as "Access" or "Update")
  public: static Status  GetIdSpaceInfo(Id spaceid,IdSpaceInfo &info);

  //
  // Public Instance members
  //

  public: IdSpaceInfo info;

  //
  // Public Instance methods
  //

  // Active non-Invasive methods (counts as "Access")
  public: UInt    Length();
  public: URange  IdRange();          // <-- Typically this would be "IndexRange"
  public: Boolean Exists(Id theid);
  public: Status  Info(IdSpaceInfo &info);
  public: IdInfo  GetInfo(Id theid);

  // Active Invasive methods (counts as "Update")
  public: Id      NewId();
  public: Status  KillId(Id theid);

  //
  // Protected Instance Methods
  //

  // C/C++ Lifecycle methods
  //
  // Note: We actively disable use of copy constructor and operator=
  protected: IdSpace();
  protected: IdSpace(IdSpace const &that);
  protected: IdSpace &operator= (const IdSpace &that);
  protected: ~IdSpace();

  // BEFE Lifecycle methods
  protected: Status StartUp();
  protected: Status MoveFrom(IdSpace const &that);
  protected: Status CopyFrom(IdSpace const &that);
  protected: Status ShutDown();

}; // ...Class IdSpace

} // ...Namespace BEFE

#endif // ...IDSPACE_H