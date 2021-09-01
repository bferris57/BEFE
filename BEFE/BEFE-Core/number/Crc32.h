//!befe-publish inc
//----------------------------------------------------------------------
//
// File:    Crc32.h - 32 bit Cyclical Reduncancy Check (CRC) definitions
//
// Classes: Crc32     - 32 bit CRC
//          Crc32Ring - Ring buffer of 32 bit CRCs
//
//
// The Crc32 class can be used to calculate 32 bit CRCs, see below.
//
// The Crc32Ring class provides a "ring" of multiple, sequenced, CRCs.
// This comes in handy when scanning text and searching for words as
// they appear (like when typing, when searching, etc.)
//
//----------------------------------------------------------------------
//
// For those of you unfamiliar with 32 bit CRCs, I'll do a quick
// summary here...
//
// CRCs are handy things that can be used to do fairly good redundancy
// checking on a stream of bits/bytes.
//
// Essentially, its a number between 0 and 2**32-1 and, given a stream
// of bytes, it's a fairly well distributed, reproducable indicator
// of whether or not it's the "original stream" of bytes.
//
// For example, given a file of N bytes, if you calculate it's CRC and
// make sure it's "probably" the same as one calculated earlier on the
// same file, if the CRCs are different AND the files are the same size,
// it's extremely likely nothing changed in it.
//
// This may seem like an odd thing to do but it saves having to keep
// the original hanging around.  With CRCs you can be pretty sure the
// file is the same as the original.  In fact, many disk controllers,
// drives, network transmission, etc. uses CRCs for this same purpose.
//
// Another handy use for CRCs is, since they're pretty well distributed
// between 0 and 2**32-1, they make wonderful string/data hash values.
// You still can have multiple bytes of the same length that have the
// same CRC but it's unlikely so that can be flagged as a special
// case and handled.
//
// For more info, take a look at...
//
//   http://en.wikipedia.org/wiki/Cyclical_redundancy_checking
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CRC32_H // CRC32_H...
#define CRC32_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

class Crc32 {

private:

  UInt32 value;

public:

  // C++ Lifecycle
  Crc32();
  Crc32(UInt32 seed);
  operator Int32();
  
  // Seeding  
  void Reset();
  void Reset(UInt32 seed);

  // Value
  UInt32 GetValue();
  UInt32 Update(UInt32 value);
  UInt32 Update(Byte *value, UInt len);
  UInt32 Update(const char *value);

}; // ...class Crc32

class Crc32Ring { // ClassCrc32Ring...

  class RingEntry {

  public:

    Int   value;
    Crc32 crc;

  };

private:

  Int        size;
  Int        bot;
  Int        top;
  RingEntry *ring;

public:

  Crc32Ring(Int ringsize);
  ~Crc32Ring();

  Status  StartUp();
  Status  ShutDown();
  Status  Reset();
  Boolean IsNull() const;
  Status  SetNull();
  Boolean IsEmpty() const;
  Status  SetEmpty();
  Int     GetSize();
  Int     GetDepth();
  Int     GetCrc(Int index);
  Int     GetValue(Int index);
  Int32   Push(Byte abyte);
  Int32   Push(Int avalue);


}; // ...Class Crc32Ring

} // ...Namespace BEFE

#endif // ...HASHARRAY_H