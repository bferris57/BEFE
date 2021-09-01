//!befe-publish inc
//----------------------------------------------------------------------
// File: Storage.h - Storage related declarations
//----------------------------------------------------------------------
//
// This header declares storage related namespace values and classes.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef STORAGE_H // STORAGE_H...
#define STORAGE_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Namespace: StorageUnit - Various named storage unit sizes (in bits)
//
// Note: This will be turning into a  ULong enum shortly so don't
//       depend on it being a "normal enum" for long.
//

namespace StorageUnit { // Namespace StorageUnit...

  enum {

     None         = 0
    ,Bit          = 1
    ,B            = 8
    ,Byte         = 8
    ,Short        = 16
    ,Int          = 32
    ,Long         = 64
    ,LongLong     = 128
    ,Kb           = 0x400                       // KiloBits
    ,KB           = 0x400*8                     // KiloBytes
    ,Mb           = 0x400*0x400                 // MegaBits
    ,MB           = 0x400*0x400*8               // MegaBytes
    ,Gb           = 0x400*0x400*0x400           // GigaBits
  //,GB           = 0x400*0x400*0x400*8         // GigaBytes <--+--- Commented out because
  //,Tb           = 0x400*0x400*0x400*0x400     // TeraBits     |    it won't fit into a
  //,TB           = 0x400*0x400*0x400*0x400*8   // TeraBytes <--+    signed int and only
  //                                                                 here for descriptive
  //                                                                 purposes.

  };

} // ...Namespace StorageUnit

//----------------------------------------------------------------------
//
// Storage types and classes
//

typedef Byte *Address;      // Address (Index) in Memory
typedef UInt  StoreAddress; // Address (Index) in other Storage

} // ...Namespace BEFE

#endif // ...STORAGE_H