//----------------------------------------------------------------------
// File: BufferLow.cpp - Implementation of the Low Level Buffer class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BufferLow.h"

namespace BEFE { // Namespace BEFE...

// Default configuration definitions
#define DFLTREDUCEPCT 75  // Default reduction percent (shrink time percent)

//----------------------------------------------------------------------
//
// Static class functions
//
// Functions: New                  - Allocate a new Buffer
//            Delete               - Delete an existing Buffer
//            SetSize              - Set a Buffer's size
//            Resize               - Resize an existing Buffer
//            CopyFrom             - Copy from an existing Buffer
//            GetTotalAllocations  - Get the total number of allocations
//            GetActiveAllocations - Get the number of active allocations
//

Status BufferLow::New(UInt initBytes, UInt incBytes, BufferLow *&newBuf) {

  Status     status;
  BufferLow *that;
  UInt       allocSize;
  Alloc      alloc;
  
  that   = NULL;
  newBuf = NULL;

  // Rationalise parameters
  alloc = Alloc(initBytes, incBytes).Normalise();

  // Allocate the memory
  allocSize = Max(alloc.MinElms(),alloc.IncElms());
  that = (BufferLow *)MemoryAllocRaw(allocSize+sizeof(BufHead32));
  if (IsNull(that)) goto OUTOFMEMORY;

  // Initially fill it with 0xff for debugging purposes
  MemoryFillRaw((Byte *)that + sizeof(BufHead32), 0xff, allocSize);

  // Clear the header
  *(Byte *)that = 0x20;
  that->SetBufSize(allocSize);
  that->SetIsConsumable(0);
  that->SetInitBytes(alloc.MinElms());
  that->SetIncBytes(alloc.IncElms());
  that->SetCurBytes(0);

  // Give it to the caller
  newBuf = that;
  that = NULL;

  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFMEMORY:      status = Error::OutOfMemory;      break;
  }
  
  if (status && that)
    MemoryFreeRaw((Byte *)that);

  return status;
}

Status BufferLow::Delete(BufferLow *&selfp) {

  if (!IsNull(selfp)) {
    MemoryFreeRaw((Byte *)selfp);
    selfp = NULL;
  }

  return Error::None;

}

Status BufferLow::Consumable(BufferLow *&selfp) {

  if (!IsNull(selfp))
    selfp->SetIsConsumable(1);

  return Error::None;
  
}

Status BufferLow::NotConsumable(BufferLow *&selfp) {

  if (!IsNull(selfp))
    selfp->SetIsConsumable(0);

  return Error::None;
  
}

Status BufferLow::SetSize(BufferLow *&selfp, UInt newBytes) {

  Status status;

  // Validate parameters
  if (IsNull(newBytes) ) goto INVALIDPARAMETER;

  // If new bytes < current allocated, just set the size
  if (!IsNull(selfp) && newBytes <= selfp->GetBufSize()) {
    selfp->SetCurBytes(newBytes);
    goto OK;
  }

  // Otherwise, resize it
  status = Resize(selfp,newBytes);

  // Handle errors
  while (false) {
    INVALIDPARAMETER: status = Error::InvalidParameter; break;
    OK:               status = Error::None;             break;
  }

  return status;

}

Status BufferLow::Resize(BufferLow *&selfp, UInt newBytes) {

  Status     status;
  BufferLow *self;
  BufferLow *newSelf;
  UInt       moveBytes;

  newSelf = NULL;

  // If no old buf allocated, just allocate a new one...
  self = selfp;
  if (IsNull(self)) {
    status = New(newBytes, UNaN, newSelf);
    if (status) goto SOMEERROR;
    self = NULL;
    goto OK;
  }

  // Make sure the buffer's resizable
  if (self->GetIncBytes() == 0) goto SIZEFIXED;

  // Nothing to do if it's the same size...
  if (newBytes == self->GetAllocBytes()) goto OK;
  
  // Okay, allocate a new one...
  status = New(newBytes, self->GetIncBytes(), newSelf);
  if (status) goto SOMEERROR;

  // Fiddle with it...
  moveBytes = Min(newBytes,self->GetCurBytes());
  newSelf->SetCurBytes(moveBytes);

  // Copy the bytes from old to new
  if (moveBytes)
    MemoryCopyRaw((Byte *)self+sizeof(BufHead32),(Byte *)newSelf+sizeof(BufHead32),moveBytes);

  // Give new buffer to the caller
  selfp   = newSelf;
  newSelf = NULL;

  // Free up the old buf
  status = Delete(self);
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    SIZEFIXED:        status = Error::SizeFixed;        break;
    SOMEERROR:                                          break;
    OK:               status = Error::None;             break;
  }

  if (!IsNull(newSelf))
    MemoryFreeRaw((Byte *)newSelf);

  return status;

}

Status BufferLow::CopyFrom(BufferLow *&selfp, BufferLow *&thatp) {

  Status     status;
  UInt       newSize;
  UInt       newSize2;

  // Validate parameters
  if (IsNull(thatp)) goto ERRORInvalidParameter;

  // Free current buffer if any
  if (selfp) {
    Delete(selfp);
    selfp = NULL;
  }

  // If that is consumable, just swap pointers...
  if (thatp && thatp->IsConsumable()) {
    *(Int *)&selfp ^= *(Int *)&thatp;
    *(Int *)&thatp ^= *(Int *)&selfp;
    *(Int *)&selfp ^= *(Int *)&thatp;
    goto OK;  
  }
  
  // Allocate a new buffer with the same allocation as them
  newSize = thatp->GetBufSize();
  selfp = (BufferLow *)MemoryAllocRaw(newSize+sizeof(BufHead32));
  if (IsNull(selfp)) goto ERROROutOfMemory;
  
  // NOTE: Keep an eye out on this because newSize2 should be equal to newSize...
  newSize2 = thatp->GetBufSize();
  if (newSize2 != newSize) goto INTERNAL;

  // Copy the contents, if any...
  MemoryCopyRaw((Byte *)thatp, (Byte *)selfp, newSize+sizeof(BufHead32));

  // Handle errors
  status = Error::None;
  while (false) {
    ERRORInvalidParameter: status = Error::InvalidParameter; break;
    ERROROutOfMemory:      status = Error::OutOfMemory;      break;
    INTERNAL:              status = Error::Internal;         break;
    OK:                    status = Error::None;             break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Static class functions
//
// Functions: GetBytes    - Get N bytes from the buffer
//            SetBytes    - Set N bytes into the buffer
//            AppendBytes - Append N bytes to the buffer
//            InsertBytes - Insert N bytes into the buffer
//            RemoveBytes - Remove N bytes from the buffer
//
// NOTE: Technically, we could have made GetBytes a "proper" C++
//       instance method since it doesn' EVER change the buffer pointer,
//       but for clarity's sake it belongs here with the other methods
//       others of its same ilk.
//

Status BufferLow::GetBytes(BufferLow *&selfp, UInt bindex, Byte *buf, UInt bufMaxSize, UInt &usedSize) {

  BufferLow *self;
  Slice      slice;
  UInt       size;
  Byte      *bytes;

  // Initialise output
  usedSize = 0;

  // If everything's lined up okay, do it...
  if (!IsNull(selfp) && !IsNull(buf)) {

    // Initialise our "self" pointer
    self = selfp;

    // Calculate slice and size
    slice = Slice(bindex,bindex+bufMaxSize).Resolve(self->GetUsedBytes());
    size  = slice.Length();

    // Get them
    bytes = self->GetBuf() + slice.idx1;
    usedSize = size;
    if (slice.step == 1)
      MemoryCopyRaw(bytes,buf,size);
    else while (size > 0) {
      *buf++ = *bytes;
       bytes += slice.step;
       size--;
    }

  }

  return Error::None;

}

Status BufferLow::SetBytes(BufferLow *&selfp, UInt bIndex, Byte *buf, UInt bufMaxSize, UInt &usedSize) {

  Status     status;
  BufferLow *self;
  UInt       newSize;
  Slice      slice;
  UInt       size;
  Byte      *bytes;

  // Initialise output
  usedSize = 0;

  // Validate parameters
  if (!selfp || bIndex < 0) goto INVALIDPARAMETER;

  // If nothing to do, don't do it...
  if (IsNull(buf) || bufMaxSize == 0) goto OK;

  // Initialise our "self" pointer
  self = selfp;

  // Calculate slice and size
  newSize = Max(self->GetCurBytes(),bIndex+bufMaxSize);

  // Resize if need be
  if (newSize > self->GetBufSize()) {
    status = SetSize(selfp,newSize);
    if (status) goto SOMEERROR;
    self = selfp;
  }

  // Calculate operating slice
  slice = Slice(bIndex,bIndex+bufMaxSize).Resolve(self->GetBufSize());
  size  = slice.Length();

  // Set them
  bytes = self->GetBuf() + slice.idx1;
  usedSize = size;
  if (slice.step == 1)
    MemoryCopyRaw(buf,bytes,size);
  else while (size > 0) {
    *buf++ = *bytes;
     bytes += slice.step;
     size--;
  }

  // Update curlen if we changed it
  if ((UInt)slice.idx2 > self->GetCurBytes())
    self->SetCurBytes(slice.idx2);

  // Handle errors
  status = Error::None;
  while (false) {
    INVALIDPARAMETER: status = Error::InvalidParameter; break;
    OK:               status = Error::None;             break;
    SOMEERROR:                                          break;
  }

  return Error::None;

}

Status BufferLow::AppendBytes (BufferLow *&selfp, Byte *theBytes, UInt numBytes) {

  Status     status;
  BufferLow *self;
  Byte      *buf;

  self = selfp;

  // If no length, nothing to do
  if (numBytes == 0) goto OK;

  // If no Buffer yet, simply make a new one and populate it...
  if (IsNull(self)) {
    status = New(numBytes,UNaN,selfp);
    if (status) goto SOMEERROR;
    self = selfp;
    buf = self->GetBuf();
    if (!buf) goto NULLPOINTER;
    if (theBytes)
      MemoryCopyRaw(theBytes,buf,numBytes);
    self->SetCurBytes(numBytes);
    goto OK;
  }

  // If not big enough, resize it...
  if (self->GetCurBytes()+numBytes > self->GetBufSize()) {
    status = Resize(selfp,self->GetCurBytes()+numBytes);
    if (status) goto SOMEERROR;
    self = selfp;
  }

  // Copy the bytes in
  buf = self->GetBuf();
  if (!buf) goto NULLPOINTER;
  if (theBytes)
    MemoryCopyRaw(theBytes,buf+self->GetCurBytes(),numBytes);
  self->SetCurBytes(self->GetCurBytes() + numBytes);

  // Handle errors
  status = Error::None;
  while (false) {
    OK:               status = Error::None;                break;
    NULLPOINTER:      status = Error::InternalNullPointer; break;
    SOMEERROR:                                             break;
  }

  return status;

}

Status BufferLow::InsertBytes(BufferLow *&selfp, UInt bIndex, Byte *theBytes, UInt numBytes) {

  Status     status;
  BufferLow *self;
  Byte      *buf;
  Int        numMove;
  UInt       curBytes;
  UInt       bufSize;

  self = selfp;

  // Normalise if negative
  curBytes = self->GetCurBytes();
  bufSize  = self->GetBufSize();
  if (bIndex < 0) bIndex = curBytes + 1;

  // Validate index
  if (bIndex < 0 || (UInt)bIndex > curBytes) goto OUTOFRANGE;

  // If no bytes, nothing to do
  if (numBytes <= 0) goto OK;

  // If no Buffer yet, simply make a new one and populate it...
  if (IsNull(self)) {
    status = New(numBytes,UNaN,selfp);
    if (status) goto SOMEERROR;
    self = selfp;
    buf = self->GetBuf();
    if (theBytes)
      MemoryCopyRaw(theBytes, buf, numBytes);
    self->SetCurBytes(numBytes);
    goto OK;
  }

  // If not big enough, resize it...
  if (curBytes+numBytes > bufSize) {
    status = Resize(self, curBytes+numBytes);
    if (status) goto SOMEERROR;
    selfp = self;
    curBytes = self->GetCurBytes();
    bufSize  = self->GetBufSize();
  }

  // Move the bytes aside past the insertion point
  buf = self->GetBuf();
  numMove = curBytes - bIndex;
  if (numMove != 0)
    MemoryCopyRaw(buf+bIndex, buf+bIndex+numBytes, numMove);

  // Copy the bytes in
  if (theBytes)
    MemoryCopyRaw(theBytes, buf+bIndex, numBytes);
  else
    MemoryFillRaw(buf+bIndex, 0xFF, numBytes);
  self->SetCurBytes(curBytes + numBytes);

  // Handle errors
  status = Error::None;
  while (false) {
    OK:         status = Error::None;             break;
    OUTOFRANGE: status = Error::IndexOutOfRange;  break;
    SOMEERROR:                                    break;
  }

  return status;
}

Status BufferLow::RemoveBytes (BufferLow *&selfp, UInt bIndex, UInt numBytes) {

  Status     status;
  BufferLow *self;
  Byte      *buf;

  self = selfp;

  // Normalise if negative
  if (bIndex < 0) bIndex = self->GetCurBytes() + bIndex;

  // Validate index
  if (bIndex < 0 || (UInt)bIndex > self->GetCurBytes()) goto OUTOFRANGE;

  // If no bytes, nothing to do
  if (numBytes <= 0) goto OK;

  // Only remove up to the end of the buffer
  if (bIndex+numBytes > self->GetCurBytes()) {
    numBytes = self->GetCurBytes() - bIndex;
    if (numBytes <= 0) goto OK;
  }

  // Remove the bytes and fill the space at the end with 0xff
  buf = self->GetBuf();
  MemoryCopyRaw(buf+bIndex+numBytes,buf+bIndex,self->GetCurBytes()-(bIndex+numBytes));
  self->SetCurBytes(self->GetCurBytes() - numBytes);
  MemoryFillRaw(buf+self->GetCurBytes(),0xff,numBytes);

  // If we reduced the size enough, shrink it...
  if (self->GetCurBytes()*100/self->GetBufSize() <= DFLTREDUCEPCT &&
      self->GetCurBytes() > self->GetIncBytes() &&
      self->GetIncBytes() != 0) {
    status = Resize(selfp,(self->GetCurBytes() + self->GetIncBytes()-1) /
                          self->GetIncBytes() * self->GetIncBytes());
    if (status) goto SOMEERROR;
    self = selfp;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    OK:         status = Error::None;             break;
    OUTOFRANGE: status = Error::IndexOutOfRange;  break;
    SOMEERROR:                                    break;
  }

  return status;

}

Status BufferLow::ReplaceBytes (BufferLow *&selfp, UInt bIndex, UInt numBytes, Byte *newBytes, UInt newSize) {

  Status     status;
  BufferLow *self;

  self = selfp;

  // Normalise if negative
  if (bIndex < 0) bIndex = self->GetCurBytes() + 1;

  // Validate index
  if (bIndex < 0 || (UInt)bIndex > self->GetCurBytes()) goto OUTOFRANGE;

  // Cheap and dirty for now, should fix this ASAP and do it in one go...
  status = RemoveBytes(selfp,bIndex,numBytes);
  if (status) goto SOMEERROR;
  self = selfp; // <-- For debugging purposes
  status = InsertBytes(selfp,bIndex,newBytes,newSize);
  if (status) goto SOMEERROR;
  self = selfp; // <-- For debugging purposes

  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFRANGE: status = Error::IndexOutOfRange;  break;
    SOMEERROR:                                    break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Instance methods
//
// Functions: Length            - Get length in bytes
//            Size              - Get size (allocated) in bytes
//            IsConsumable      - "Is this BufferLow consumable?"
//            GetAllocBytes     - Get number of bytes allocated for the Buffer
//            GetIncrementBytes - Get number bytes to increment when resizing
//            GetUsedBytes      - Get number of used bytes in the Buffer
//            SetUsedBytes      - Set number of used bytes in the Buffer
//            GetBuf            - Return address of bytes themselves
//

UInt BufferLow::Length() const {
  return GetCurBytes();
}

UInt BufferLow::Size() const {
  return GetBufSize();
}

Boolean BufferLow::IsConsumable() const {
  return GetIsConsumable();
}

UInt BufferLow::GetAllocBytes() const {
  return GetBufSize();
}

UInt BufferLow::GetIncrementBytes() const {
  return GetIncBytes();
}

UInt BufferLow::GetUsedBytes() const {
  return GetCurBytes();
}

Status BufferLow::SetUsedBytes(UInt newCurBytes) {

  SetCurBytes(newCurBytes);

  return Error::None;

}

Byte *BufferLow::GetBuf() const {
  return ((Byte *)this) + sizeof(BufHead32);
}

} // ...Namespace BEFE
