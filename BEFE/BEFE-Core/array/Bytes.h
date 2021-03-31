//!befe-publish inc
//----------------------------------------------------------------------
// File: Bytes.h - Declarations for the Bytes class
//----------------------------------------------------------------------
//
// The Bytes class is a "tiny" wrapper for a BytesBuffer instance.
//
// We've made this so that the effort required to maintain the pointer
// to the BytesBuffer is hidden from the caller.
//
// Unfortunate side effect is that the function calls take a teeny
// bit longer but it's worth it for ease of use.
//
// Basically, Bytes are a variable length array of N bytes.
//
// However, Bytes maintains an ElementSize member so it can be reused
// by other Array-like things like Ints, Ranges, etc.
//
// We also have two flavours of most functions that are passed
// Indexes...
//
//   o UInt flavour - Low level "that's the index" basic form
//   o Int  flavour - Higher level "interpret based on setting"
//
// It's the second form that's the most interesting because it allows 
// us, depending on the SetIndexType method call, to change how we
// interpret positive versus negative indexes.
//
// *** See Sequence.h for a more robust description of each of the Index
// ***   types.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BYTES_H // BYTES_H...
#define BYTES_H

#include "Primitives.h"
#include "Buffer.h"
#include "Alloc.h"

namespace BEFE { // Namespace BEFE...

class Bytes { // Class Bytes...

  //
  // Public members
  //

  public: Boolean isLimited:1;        // "Is this a limited size?"
  public: Boolean isFixed:1;          // "Is this a fixed size?"
  public: UInt    indexType:3;        // Sequence IndexType (see Sequence.h)
  public: UInt    reserved:11;        // ◄── ***Reserved for future use***
  public: UInt    elmSize:16;         // Size of each element (in bytes)
  public: Buffer  buffer;             // Buffer containing data

  //
  // Public instance methods
  //

  // C++ Lifecycle
  public: Bytes();                    // Default constructor
  public: Bytes(Alloc alloc);         // Alloc constructor
  public: Bytes(Bytes const &that);   // Copy constructor
  public: ~Bytes();                   // Deconstructor
  public: Bytes &operator=(Bytes const &that); // Operator =

  // BEFE Lifecycle
  public: Status  StartUp();
  public: Status  ShutDown();
  public: Status  Reset();
  public: Boolean IsNull() const { return buffer.IsNull(); };
  public: Status  SetNull();
  public: Boolean IsEmpty() const { return buffer.IsEmpty(); };
  public: Status  SetEmpty();
  public: Status  MoveFrom(Bytes const &that);
  public: Status  CopyFrom(Bytes const &that);
  
  public: Bytes  &Consumable() const;
  public: Bytes  &NotConsumable() const;
  public: Boolean IsConsumable()const;// "Is this consumable?"
  
  public: UInt    Length() const;     // Get length in Elements (same thing as size for Base Class)
  public: UInt    Size() const;       // Get size in Bytes
  public: UInt    Hash() const;       // Get Hash of Bytes
  public: Status  Allocate(Alloc const &alloc);
  public: Status  SetLength(UInt newElms);

  // High level Base Buffer methods
  public: Status  Get(UInt index, Byte *buf) const;
  public: Status  Get(UInt index, Byte *buf, UInt bufMaxSize, UInt &usedSize) const;
  public: Status  Get(Int  index, Byte *buf) const;
  public: Status  Get(Int  index, Byte *buf, UInt bufMaxSize, UInt &usedSize) const;

  public: Status  Set(UInt index, Byte *buf);
  public: Status  Set(UInt index, Byte *buf, UInt size);
  public: Status  Set(Int  index, Byte *buf);
  public: Status  Set(Int  index, Byte *buf, UInt size);

  public: Status  Append(Byte *buf);
  public: Status  Append(Byte *buf, UInt size);

  public: Status  Insert(UInt index, Byte *buf);
  public: Status  Insert(UInt index, Byte *buf, UInt size);
  public: Status  Insert(Int  index, Byte *buf);
  public: Status  Insert(Int  index, Byte *buf, UInt size);

  public: Status  Remove(UInt index);
  public: Status  Remove(UInt index, UInt size);
  public: Status  Remove(Int index);
  public: Status  Remove(Int index, UInt size);
  public: Status  Remove(Span   const &span);
  public: Status  Remove(USpan  const &span);
  public: Status  Remove(Range  const &range);
  public: Status  Remove(URange const &range);
  public: Status  Remove(Slice  const &slice);
  
  // Low level Buffer info
  public: UInt    GetAllocBytes();     // Get allocated bytes
  public: UInt    GetIncrementBytes(); // Get byte increment count
  public: UInt    GetUsedBytes();      // Get number of used bytes
  public: Status  SetUsedBytes(UInt newBytes); // Set number of used bytes
  public: Byte   *GetBuf();            // Get pointer to the bytes (transient)
  public: void    BufAndSize(Byte *&buf, UInt &size);
  public: Status  BufAlloc();          // Allocate Buffer if none, with default Alloc
  public: Status  BufAlloc(Alloc def); // Allocate Buffer if none, with specified Alloc

  // Morphing
  public: String  MorphToString();
  public: String  MorphToString(Boolean makeMutable);

  // Bytes specific methods
  public: Status  Get(Int index, Byte &byte);
  public: Bytes   Get(Slice const &slice) const;
  public: Status  Get(Slice const &slice, Bytes &bytes) const;
  public: Bytes   Get(Range const &range) const;
  public: Status  Get(Range const &range, Bytes &bytes) const;
  public: Status  Set(Int index, Byte byte);
  
  // Index resolving
  public: UInt    GetIndexType();
  public: UInt    SetIndexType(UInt type);
  public: UInt    ResolveIndex(Int value);
  public: UInt    ResolveIndex(UInt type, Int value);
  
}; // ...Class Bytes

//----------------------------------------------------------------------
//
// General Bytes Functions...
//

Boolean IsNull(Bytes const &v);

} // ...Namespace BEFE

#endif // ...BYTES_H