//----------------------------------------------------------------------
// File: Bytes.cpp - Implementation of the Bytes class
//----------------------------------------------------------------------
//
// This class is used as a movable wrapper around a dynamic Buffer
// instance.  We use the phrase "dynamic" meaning the Buffer acts
// on its own and moves itself around in memory without as required.
//
// As such, it demands a more usable "front end" class to wrap it and
// act more like a normal "value" class.  So, we've implemented
// the standard <Movability> methods.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Bytes.h"
#include "Buffer.h"
#include "Crc32.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// General Bytes Functions...
//

Boolean IsNull(Bytes const &v) {
  return v.IsEmpty();
}
//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Bytes::Bytes()                  {StartUp();                 };
Bytes::Bytes(Bytes const &that) {StartUp(); CopyFrom(that); }
Bytes::Bytes(Alloc alloc)       {StartUp(); BufAlloc(alloc);}
Bytes::~Bytes()                 {ShutDown();                };

// C++ copy constructor
Bytes &Bytes::operator=  (Bytes const &that) {
  CopyFrom(that);
  NotConsumable();
  return *this;
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle methods
//

Status Bytes::StartUp() {
  Status status;
  isFixed   = false;
  isLimited = false;
  indexType = Sequence::IndexType::LengthWrap;
  elmSize   = 1;
  status = buffer.StartUp();
  return status;
}

Status Bytes::ShutDown() {
  Status status;
  status = buffer.ShutDown();
  return status;
}

Status Bytes::Reset() {

  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status Bytes::SetEmpty() {

  Status status;

  status = buffer.SetSize(0);

  return status;

}

Status Bytes::MoveFrom(Bytes const &that) {
  Status status;
  if (this != &that) {
    isLimited = that.isLimited;
    isFixed   = that.isFixed;
    indexType = that.indexType;
    elmSize   = that.elmSize;
    status = buffer.MoveFrom(that.buffer);
  }
  else
    status = Error::None;
  return status;
}

Status Bytes::CopyFrom(Bytes const &that) {

  Status status;

  if (this != &that) { 
    if (IsConsumable()) {
      status = MoveFrom(that);
      if (status) goto SOMEERROR;
    }
    else {
      status = Reset();
      if (status) goto SOMEERROR;
      isLimited = that.isLimited;
      isFixed   = that.isFixed;
      indexType = that.indexType;
      elmSize   = that.elmSize;
      status = buffer.CopyFrom(that.buffer);
      if (status) goto SOMEERROR;
    }
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
}

Bytes &Bytes::Consumable() const {
  buffer.Consumable();
  return *(Bytes *)this;
}

Bytes &Bytes::NotConsumable() const {
  buffer.NotConsumable();
  return *(Bytes *)this;
}

Boolean Bytes::IsConsumable() const {
  return buffer.IsConsumable();
}

UInt Bytes::Length() const {
  return buffer.GetUsedBytes()/elmSize;
}

UInt Bytes::Size() const {
  return buffer.GetUsedBytes()/elmSize;
}

UInt Bytes::Hash() const {

  Crc32    crc;
  UInt     hash;

  crc.Update(buffer.GetBuf(),buffer.Size());
  hash = crc.GetValue();
  
  return hash;

}


Status Bytes::Allocate(Alloc const &alloc) {
  
  Status status;
  Alloc  ourAlloc;
  
  if (buffer.pointer && buffer.GetUsedBytes() != 0) goto NOTEMPTY;

  // If we already have an empty buffer, free it
  if (buffer.pointer) {
    status = buffer.Reset();
    if (status) goto SOMEERROR;
  }
  
  // Take a copy and normalise it based on element size
  ourAlloc = alloc.Normalise(elmSize);
  if ( BEFE::IsNull(ourAlloc.MaxElms()) )
    status = buffer.Allocate(ourAlloc.MaxElms(), ourAlloc.IncElms());
  else
    status = buffer.Allocate(ourAlloc.MinElms(), ourAlloc.IncElms());
  if (status) goto SOMEERROR;
  
  // Set fixed/limited flags
  isFixed   = ourAlloc.IsFixed();
  isLimited = ourAlloc.IsLimited();
  
  // Set minimum elements
  status = buffer.SetUsedBytes(ourAlloc.MinElms());
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOTEMPTY:  status = Error::ArrayNotEmpty; break;
    SOMEERROR:                                break;
  }
  
  return status;
  
}

Status Bytes::SetLength(UInt newSize) {
  return buffer.SetSize(newSize*elmSize);
}

//----------------------------------------------------------------------
//
// High Level Base methods
//

Status Bytes::Get(UInt index, Byte *buf) const {
  
  Status status;
  UInt   usedLen;
  
  status = Get(index, buf, 1, usedLen);

  return status;
  
}

Status Bytes::Get(UInt index, Byte *buf, UInt bufMaxLen, UInt &usedLen) const {

  Status  status;
  UInt    usedElms;
  UInt    usedBytes;
  
  usedElms = ((Buffer *)&buffer)->GetUsedBytes() / elmSize;
  if (index >= usedElms) goto OUTOFRANGE;
  
  status = ((Buffer *)&buffer)->GetBytes(index*elmSize, buf, bufMaxLen*elmSize, usedBytes);
  if (status) goto SOMEERROR;
  usedLen = usedBytes/elmSize;
  
  // Handle errors
  while (false) {
    OUTOFRANGE: status = Error::IndexOutOfRange; break;
    SOMEERROR:                                   break;
  }
  
  return status;

}

Status Bytes::Get(Int index, Byte *buf) const {
  
  Status status;
  UInt   usedLen;
  
  status = Get(index, buf, 1, usedLen);

  return status;
  
}

Status Bytes::Get(Int index, Byte *buf, UInt bufMaxLen, UInt &usedLen) const {

  Status status;
  UInt   resIndex;
  
  resIndex = ((Bytes *)this)->ResolveIndex(index);
  status   = Get(resIndex, buf, bufMaxLen, usedLen);
  
  return status;

}

Status Bytes::Set(UInt index, Byte *buf) {
  
  Status status;
  
  status = Set(index, buf, elmSize);
  
  return status;

}

Status Bytes::Set(UInt index, Byte *buf, UInt size) {

  Status status;
  UInt   usedElms;
  UInt   usedBytes;
  
  usedElms = ((Buffer *)&buffer)->GetUsedBytes() / elmSize;
  if (index >= usedElms) goto OUTOFRANGE;
  
  usedBytes = 0;
  status = buffer.SetBytes(index*elmSize, buf, size*elmSize, usedBytes);
  if (status) goto SOMEERROR;
  usedElms = usedBytes/elmSize;
  if (usedElms != size) goto INTERNALERROR;

  // Handle errors
  while (false) {
    OUTOFRANGE:    status = Error::IndexOutOfRange; break;
    SOMEERROR:                                      break;
    INTERNALERROR: status = Error::Internal;        break;
  }
  
  return status;

}

Status Bytes::Set(Int index, Byte *buf) {
  
  Status status;
  
  status = Set(index, buf, elmSize);
  
  return status;

}

Status Bytes::Set(Int index, Byte *buf, UInt size) {

  Status  status;
  UInt    resIndex;
  
  resIndex = ResolveIndex(index);
  status   = Set(resIndex, buf, size);
  
  return status;

}

Status Bytes::Append(Byte *buf) {

  Status status;
  
  status = Append(buf, 1);
  
  return status;
  
}

Status Bytes::Append(Byte *buf, UInt size) {

  Status status;

  status = BufAlloc();
  if (status) goto SOMEERROR;

  status = buffer.AppendBytes(buf,size*elmSize);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR:                               break;
  }

  return status;

}

Status Bytes::Insert(UInt index, Byte *buf) {
  
  Status status;
  
  status = Insert(index, buf, 1);
  
  return status;
  
}

Status Bytes::Insert(UInt index, Byte *buf, UInt size) {

  Status status;
  UInt   usedElms;

  usedElms = ((Buffer *)&buffer)->GetUsedBytes() / elmSize;
  if (index > usedElms) goto OUTOFRANGE;
  
  status = BufAlloc();
  if (status) goto SOMEERROR;

  status = buffer.InsertBytes(index*elmSize,buf,size*elmSize);
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFRANGE: status = Error::IndexOutOfRange; break;
    SOMEERROR:                                   break;
  }

  return status;

}

Status Bytes::Insert(Int index, Byte *buf) {
  
  Status status;
  
  status = Insert(index, buf, 1);
  
  return status;
  
}

Status Bytes::Insert(Int index, Byte *buf, UInt size) {

  Status status;
  UInt   resIndex;
  
  resIndex = ResolveIndex(index);
  status   = Insert(resIndex, buf, size);
  
  return status;

}

Status Bytes::Remove(UInt index) {
  
  Status status;
  
  status = Remove(index, 1);
  
  return status;
  
}

Status Bytes::Remove(UInt index, UInt size) {

  Status status;
  UInt   usedElms;

  if ( size == 0 || BEFE::IsNull(size) ) goto OK;
  usedElms = ((Buffer *)&buffer)->GetUsedBytes() / elmSize;
  if (index > usedElms) goto OUTOFRANGE;
  
  status = BufAlloc();
  if (status) goto SOMEERROR;

  status = buffer.RemoveBytes(index*elmSize,size*elmSize);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    OK:         status = Error::None;            break;
    OUTOFRANGE: status = Error::IndexOutOfRange; break;
    SOMEERROR:                                   break;
  }

  return status;

}

Status Bytes::Remove(Int index) {
  
  Status status;
  
  status = Remove(index, 1);
  
  return status;
  
}

Status Bytes::Remove(Int index, UInt size) {

  Status status;
  UInt   resIndex;
  
  resIndex = ResolveIndex(index);
  status   = Remove(resIndex, size);

  return status;

}

Status Bytes::Remove(Span const &span) {
  
  Status status;
  UInt   len;
  Span   rSpan;
  UInt   rLen;
  UInt   start;
  
  len = Length();
  if (len == 0) goto OK;
  rSpan = span.Resolve(len);
  rLen = rSpan.Length();
  if (len == 0) goto OK;
  if (rSpan.idx1 < rSpan.idx2)
    start = rSpan.idx1;
  else
    start = rSpan.idx2+1;
  if (start >= len) goto OUTOFRANGE;
  status = Remove(start, rLen);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    OK:         status = Error::None;            break;
    OUTOFRANGE: status = Error::IndexOutOfRange; break;
    SOMEERROR:                                   break;
  }
  
  return status;

}

Status Bytes::Remove(USpan const &span) {
  
  Status status;
  UInt   len;
  USpan  rSpan;
  UInt   start;
  
  len = Length();
  if (len == 0) goto OK;
  rSpan = span.Resolve(len);
  len = rSpan.Length();
  if (len == 0) goto OK;
  start = Min(rSpan.idx1, rSpan.idx2);
  if (start >= len) goto OUTOFRANGE;
  status = Remove(Min(rSpan.idx1, rSpan.idx2), len);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    OK:         status = Error::None;            break;
    OUTOFRANGE: status = Error::IndexOutOfRange; break;
    SOMEERROR:                                   break;
  }
  
  return status;

}

Status Bytes::Remove(Range const &range) {
  
  Status status;
  Span   span;
  
  span.idx1 = range.idx1;
  span.idx2 = range.idx2+1;
  status = Remove(span);

  return status;
  
}

Status Bytes::Remove(URange const &range) {

  Status status;
  USpan  span;
  
  span.idx1 = range.idx1;
  span.idx2 = range.idx2+1;
  status = Remove(span);

  return status;
  
}

Status Bytes::Remove(Slice const &slice) {
  
  Status status;
  Slice  rSlice;
  Span   span;
  UInt   len;
  UInt   curIdx;

  // Validate step
  if ( slice.step == 0 || BEFE::IsNull(slice.step) ) goto INVALIDPARAMETER;
  
  // Resolve the slice
  len = Length();
  rSlice = slice.Resolve(len);
  len = rSlice.Length();
  if ( len == 0 || BEFE::IsNull(len) ) goto OK;
  
  // Build the span for the removal
  span.idx1 = Min(rSlice.idx1, rSlice.idx2);
  span.idx2 = Max(rSlice.idx1, rSlice.idx2);
  if (rSlice.idx1 > rSlice.idx2) {
    span.idx1++;
    span.idx2++;
  }
  
  // Special case for step 1/-1...
  if (rSlice.step == 1 || rSlice.step == -1) {
    status = Remove(span.idx1,len);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Remove one by one...
  rSlice.step = Abs(rSlice.step); 
  for (curIdx=span.idx1; curIdx < (UInt)span.idx2; curIdx += rSlice.step-1) {
    status = Remove(curIdx,1);
    if (status) goto SOMEERROR;
    span.idx2--;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    INVALIDPARAMETER: status = Error::InvalidParameter; break;
    OK:               status = Error::None;             break;
    SOMEERROR:                                          break;
  }
  
  return status;
  
}


//----------------------------------------------------------------------
//
// Low level Buffer info
//

UInt Bytes::GetAllocBytes() {

  UInt size;

  size = buffer.GetAllocBytes();

  return size;
  
}

UInt Bytes::GetIncrementBytes() {

  UInt size ;

  size = buffer.GetIncrementBytes();

  return size;
  
}

UInt Bytes::GetUsedBytes() {

  UInt size;

  size = buffer.GetUsedBytes();

  return size;
  
}

Status Bytes::SetUsedBytes(UInt newBytes) {
  
  Status status;
  
  status = buffer.SetUsedBytes(newBytes);
  
  return status;

}

Byte *Bytes::GetBuf() {

  Byte *buf;

  buf = buffer.GetBuf();

  return buf;
  
}

void Bytes::BufAndSize(Byte *&buf, UInt &size) {

  buf  = buffer.GetBuf();
  size = buffer.GetUsedBytes();

  return;

}

Status Bytes::BufAlloc() {

  Status status;
  Alloc  alloc("0..");
  UInt   minElms;
  UInt   incElms;
  if (buffer.pointer)
    status = Error::None;
  else {
    alloc.Normalise(elmSize);
    minElms = alloc.MinElms();
    if ( BEFE::IsNull(minElms) )
      minElms = minElms*elmSize;
    incElms = alloc.IncElms();
    if ( BEFE::IsNull(incElms) )
      incElms = incElms*elmSize;
    status = buffer.Allocate(minElms,incElms);
  }

  return status;

}

Status Bytes::BufAlloc(Alloc alloc) {

  Status status;
  UInt   minElms;
  UInt   incElms;

  if (buffer.pointer)
    status = Error::None;
  else {
    minElms = alloc.MinElms();
    if ( BEFE::IsNull(minElms) )
      minElms = 0;
    incElms = alloc.IncElms();
    if ( BEFE::IsNull(incElms) )
      incElms = 0;
    status = buffer.Allocate(minElms*elmSize,incElms*elmSize);
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Morphing
//

String Bytes::MorphToString() {

  String that;

  that.isMutable    = true;
  that.isByte       = false;
  that.isChar       = false;
  that.type         = String::Buffer;
  that.mult         = 1;
  that.data         = buffer.pointer;

  StartUp();

  that.Consumable();
  return that;

}

String Bytes::MorphToString(Boolean makeMutable) {

  String that;

  that.isMutable    = makeMutable;
  that.isByte       = false;
  that.isChar       = false;
  that.type         = String::Buffer;
  that.mult         = 1;
  that.data         = buffer.pointer;

  StartUp();

  that.Consumable();
  return that;

}

//----------------------------------------------------------------------
//
// Bytes specific methods
//

Status Bytes::Get(Int index, Byte &byte) {
  
  Status status;
  
  status = Get(index, &byte);
  
  return status;
  
}  

Bytes Bytes::Get(Slice const &slice) const {

  //Status status;
  Bytes  bytes;

  /*status =*/ Get(slice,bytes);

  return bytes.Consumable();

}

Status Bytes::Get(Slice const &slice, Bytes &bytes) const {
  
  Status status;
  
  status = bytes.SetEmpty();
  if (!status) status = Error::NotImplemented;
  BEFE_WARN("Bytes.Get(Slice const &) not implemented yet");
  
  return status;
  
}

Bytes Bytes::Get(Range const &range) const {

  //Status status;
  Bytes  bytes;

  /*status =*/ Get(range,bytes);

  return bytes.Consumable();

}


Status Bytes::Get(Range const &range, Bytes &bytes) const {
  
  Status status;
  
  status = bytes.SetEmpty();
  if (!status) status = Error::NotImplemented;
  BEFE_WARN("Bytes.Get(Range const &) not implemented yet");
  
  return status;
  
}

Status Bytes::Set(Int index, Byte byte) {

  Status status;
  UInt   usedElms;
  UInt   usedBytes;

  usedElms = ((Buffer *)&buffer)->GetUsedBytes() / elmSize;
  if (index < 0) index = usedElms + index;
  if (index < 0 || index >= (Int)usedElms) goto OUTOFRANGE;
  
  status = buffer.SetBytes(index*elmSize, &byte, elmSize, usedBytes);
  if (status) goto SOMEERROR;
  if (usedBytes/elmSize != 1) goto INTERNALERROR;

  // Handle errors
  while (false) {
    OUTOFRANGE:    status = Error::IndexOutOfRange; break;
    SOMEERROR:                                      break;
    INTERNALERROR: status = Error::Internal;        break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Index resolving
//

UInt Bytes::GetIndexType() {
  return indexType;
}

UInt Bytes::SetIndexType(UInt type) {
  
  UInt oldType;
  
  oldType = indexType;
  if (type < Sequence::IndexType::NumTypes)
    indexType = type;
  
  return oldType;

}

UInt Bytes::ResolveIndex(Int value) {
  return ResolveIndex(indexType, value);  
}

UInt Bytes::ResolveIndex(UInt type, Int value) {

  Int  newValue;
  UInt usedElms;
  
  switch (type) {
    case Sequence::IndexType::None:
      newValue = value;
      break;
    case Sequence::IndexType::LengthWrap:
      newValue = value;
      if (newValue < 0) {
        usedElms = buffer.GetUsedBytes()/elmSize;
        newValue = usedElms + newValue;
      }
      if (newValue < 0)
        BEFE::SetNull(newValue);
      break;
    case Sequence::IndexType::Ring:
      usedElms = buffer.GetUsedBytes()/elmSize;
      if (usedElms == 0)
        newValue = 0;
      else if (value >= 0)
        newValue = value%usedElms;
      else
        newValue = usedElms + value%usedElms;
      break;
    case Sequence::IndexType::Stack:
      usedElms = buffer.GetUsedBytes()/elmSize;
      if (usedElms == 0)
        newValue = 0;
      else if (value >= 0 && value < (Int)usedElms)
        newValue = usedElms-1 - value;
      else
        newValue = -value - 1;
      break;    
    case Sequence::IndexType::Queue:
    default:
      BEFE::SetNull(newValue);
      break;
  }
  
  return (UInt)newValue;

}
  
}  // ...Namespace BEFE
