//----------------------------------------------------------------------
// File: Crc32.cpp - Implementation of the Crc32 Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// Note: I forget where I knicked this from, but it was somewhere on the
//       web and didn't seem to have a licence like GPL associated with it.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#define CRC32_CPP

#include "BEFE.h"
#include "Crc32.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class: Crc32 - 32 bit Cyclical Redundancy Check (CRC)
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Crc32::Crc32() {
  Reset();
}

Crc32::Crc32(UInt32 seed) {
  Reset(seed);
}

Crc32::operator Int32() {
  return value;
}

//----------------------------------------------------------------------
//
// Seeding
//

void Crc32::Reset() {
  value = 0;
}

void Crc32::Reset(UInt32 seed) {
  value = seed;
}

//----------------------------------------------------------------------
//
// Value
//

UInt32 Crc32::GetValue() {
  return value;
}

UInt32 Crc32::Update(UInt32 newvalue) {
  value = IntCrc32(value,newvalue);
  return value;
}

UInt32 Crc32::Update(Byte *bytes, UInt len) {
  
  if (!IsNull(bytes) && len != 0)
    while (len-- > 0)
      value = IntCrc32(value,*bytes++);
    
  return value;
}

UInt32 Crc32::Update(const char *chars) {
  Reset();
  while (!IsNull(chars) && *chars != 0)
    value = IntCrc32(value,*chars++);
  return value;
}

//----------------------------------------------------------------------
//
// Class: Crc32Ring - Circular ring of Crc32 values
//
//----------------------------------------------------------------------

Crc32Ring::Crc32Ring(Int ringsize) {

  size  = 0;
  bot   = 0;
  top   = 0;
  ring  = NULL;

  if (ringsize < 0)
    size = 0;
  else
    size = ringsize;

  SetEmpty();

}

Crc32Ring::~Crc32Ring() {

  if (!BEFE::IsNull(ring)) {
    delete[] ring;
    ring = NULL;
  }

}

Status Crc32Ring::StartUp() {

  BEFE::SetNull(size);
  BEFE::SetNull(bot);
  BEFE::SetNull(top);
  ring = NULL;

  return Error::None;
  
}
  
Status Crc32Ring::ShutDown() {

  if (!BEFE::IsNull(ring))
    delete ring;
  
  return Error::None;
  
}

Status Crc32Ring::Reset() {

  Status retStatus;
  Status status;

  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Boolean Crc32Ring::IsNull() const {
  return (BEFE::IsNull(ring));
}

Status Crc32Ring::SetNull() {
  return Reset();
}

Boolean Crc32Ring::IsEmpty() const {
  return (BEFE::IsNull(ring));
}
          
Status Crc32Ring::SetEmpty() {

  Int    i;

  // Allocate ring if not already
  if (BEFE::IsNull(ring) && size > 0) {
    ring = new RingEntry[size+1];
  }

  // Clear ring if any
  if (!BEFE::IsNull(ring)) {
    for (i=0; i < size; i++)
      ring[i].crc.Reset();
  }

  // Clear other stuff
  top = 0;
  bot = 0;

  return Error::None;

}

Int Crc32Ring::GetSize() {
  return size;
}

Int Crc32Ring::GetDepth() {

  Int depth;

  if (size <= 0)
    depth = 0;
  else
    depth = (top-bot)%size;

  return depth;

}

Int Crc32Ring::GetCrc(Int index) {

  Int   idx;
  Int32 result;

  result = 0;

  if (size != 0 && !IsNull()) {
    idx = (top - 1 - index) % size;
    result = ring[idx].crc.GetValue();
  }

  return result;

}

Int Crc32Ring::GetValue(Int index) {

  Int   idx;
  Int32 result;

  result = 0;

  if (size != 0 && !IsNull()) {
    idx = (top - 1 - index) % size;
    result = ring[idx].value;
  }

  return result;

}

Int32 Crc32Ring::Push(Byte abyte) {

  Int32 result;
  Int   i;

  if (size <= 0 || IsNull())
    result = 0;
  else {
    for (i=bot; i != top; i=(i+1)%size)
      ring[i].crc.Update(abyte);
    ring[top].value = abyte;
    result = ring[top].crc.Update(abyte);
    top = (top+1)%size;
  }

  return result;
}

Int32 Crc32Ring::Push(Int avalue) {

  Int32 result;
  Int   i;

  if (size <= 0 || IsNull())
    result = 0;
  else {
    for (i=bot; i != top; i = (i+1)%size)
      ring[i].crc.Update(avalue);
    ring[top].value = avalue;
    result = ring[top].crc.Update(avalue);
    top = (top+1)%size;
  }

  return result;
}

} // ...Namespace BEFE
