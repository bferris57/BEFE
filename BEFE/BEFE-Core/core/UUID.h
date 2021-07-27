//!befe-publish inc
//----------------------------------------------------------------------
// File: UUID.h - UUID Declarations
//----------------------------------------------------------------------
//
// These definitions are for BEFE UUIDs and related Functions.
//
// Note: Unlike Microsoft who calls it a "GUID", we stick with the IETF
//       RFC 4122 Standard and call it a UUID.
//
// Note: For now, we only need version 4 (random) UUIDs, so we've
//       designed the struct members and methods to follow that.
//
//       But, because we may change our mind, we've declared the 
//       members with "_" names to WARN you that they may change in 
//       the future.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef UUID_H // UUID_H...
#define UUID_H

#include "Primitives.h"
#include "Stream.h"

namespace BEFE { // Namespace BEFE...

struct UUID { // Struct UUID...

  // Members
  //
  // Note: We do this in TWO WAYS... Microsoft's and the "sane way".
  //       This is because there's nothing we can find in RFC4122 that
  //       says you have to treat these sub-parts of a UUID in a
  //       little-endian (Intel) way as "Native Integers".
  //
  //       BUT, RFC4122 says you have to present it to the "outside world"
  //       in a big-endian way.  So, we leave it up to the Platform
  //       specific code to dick about with it if it really needs to
  //       needs to and present it PROPERLY once generated.
  //
  //       Sure, we could argue about it day in and day out but, quite
  //       honestly, the only real need for a UUID, to begin with, is to
  //       persistently identify things uniquely.  So, this means things
  //       that are likely to be stored on disk, transmitted across a
  //       network, and/or pulled together from disparate locations.
  //
  //       "Things" that are manipulated that way are already used to
  //       suffering time delays, so why not just add a teeny weeny
  //       bit more overhead ONCE when generating the UUID instead
  //       of EVERY TIME you want to store/transmit them?
  //
  //       Duh!!!
  //
  union {
    struct {                
    	UInt32 data1;
	    UInt16 data2;
	    UInt16 data3;
	    Byte   data4[8];
    } microsoft;            // ◄── Microsoft's weird GUID way
    Byte data[16];          // ◄── Sane way specified in RFC4122
  } value;
  
   // C++ Lifecycle
  UUID();
  UUID(UUID const &that);
  ~UUID();
  UUID &operator = (UUID const &that);
  
  // BEFE Lifecycle
  Status   StartUp();
  Status   ShutDown();
  Status   Reset();
  Boolean  IsNull() const;
  Status   SetNull();
  Boolean  IsEmpty() const;
  Status   SetEmpty();
  Status   MoveFrom(UUID const &that);
  Status   CopyFrom(UUID const &that);
  UUID    &Consumable() const;
  UUID    &NotConsumable() const;
  Boolean  IsConsumable() const;
  UInt     Length() const;
  UInt     Size() const;
  Status   WriteTo(Stream &out);  
  Status   ReadFrom(Stream &in);

  // Operational Methods
  Boolean  IsValid() const;
  String   ToString() const;
  Status   NewRandom();
  
}; // ...Struct UUID

//----------------------------------------------------------------------
//
// General UUID Functions
//

BEFE_INLINE Boolean IsNull(UUID const &that)   {return that.IsNull();}
BEFE_INLINE Status  SetNull(UUID &that)        {return that.SetNull();}
BEFE_INLINE String &ToString(UUID const &that) {return that.ToString().Consumable();}

} // ...Namespace BEFE

#endif // ...UUID_H