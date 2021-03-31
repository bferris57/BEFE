//----------------------------------------------------------------------
// File: _SparseBase.cpp - Implementation of the _SparseBase class
//----------------------------------------------------------------------
//
// This is the implementation of the _SparseBase class.  It is intended
// to be used for all the implementations of the SparsePrimitive<T>  and
// SparseValue<V> templates.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Sparse.h"

namespace BEFE { // Namespace BEFE...

//
// Handy local defines
//

#define HEADSIZE sizeof(UInt)

//----------------------------------------------------------------------
//
// General _SparseBase Functions...
//

Boolean IsNull(_SparseBase const &v) {
  return v.IsEmpty();
}
  
//----------------------------------------------------------------------
//
// C++ Lifecycle methods
//

_SparseBase::_SparseBase() {
  StartUp();
}

_SparseBase::_SparseBase(_SparseBase const &that) {
  CopyFrom(that);
}

_SparseBase::~_SparseBase() {
  ShutDown();
}

_SparseBase &_SparseBase::operator = (_SparseBase const &that) {
  if (isMutable) {
    CopyFrom(that);
    NotConsumable();
  }
  return *this;
}

//----------------------------------------------------------------------
//  
// BEFE Lifecycle methods
//

Status _SparseBase::StartUp() {

  Status status;
  
  isDistinct     = false;
  isMap          = false;
  isInsensitive  = false;
  isMutable      = true;
  mapKeyType     = 0;
  elmSize        = 0;
  
  status = buffer.StartUp();
  
  return status;

}

Status _SparseBase::ShutDown() {
  
  Status status;
  
  status = buffer.ShutDown();
  
  return status;

}

Status _SparseBase::Reset() {
  
  Status retStatus;
  Status status;
  
  if (!isMutable)
    retStatus = Error::AccessDenied;
  else {
    retStatus   = buffer.ShutDown();
    status      = buffer.StartUp();
    if (status && !retStatus) retStatus = status;
  }
  
  return retStatus;
  
}

Status _SparseBase::SetEmpty() {

  Status status;

  if (!isMutable)
    status = Error::AccessDenied;
  else {
    status = _BufAlloc();
    if (!status)
      status = buffer.SetUsedBytes(0);
  }
  
  return status;
  
}

Status _SparseBase::MoveFrom(_SparseBase const &that) {
  
  Status status;
  
  if (!isMutable)
    status = Error::AccessDenied;
  else if (this != &that) {
    status = Reset();
    if (!status) {
      MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(_SparseBase));
      status = ((_SparseBase *)&that)->StartUp();
    }
  }
  else
    status = Error::None;
    
  return status;
}

Status _SparseBase::CopyFrom(_SparseBase const &that) {
  
  Status status;
  
  if (!isMutable)
    status = Error::AccessDenied;
  else if (buffer.IsConsumable())
    status = MoveFrom(that);
  else {
    isDistinct     = that.isDistinct;
    isMap          = that.isMap;
    isInsensitive  = that.isInsensitive;    
    isMutable      = that.isMutable;
    mapKeyType     = that.mapKeyType;
    elmSize        = that.elmSize;
    buffer         = that.buffer;
    status = Error::None;
  }
  
 return status;

}

_SparseBase &_SparseBase::Consumable() const {
  ((Buffer *)&buffer)->Consumable();
  return *((_SparseBase *)this);
}

_SparseBase &_SparseBase::NotConsumable() const {
  ((Buffer *)&buffer)->NotConsumable();
  return *((_SparseBase *)this);
}

Boolean _SparseBase::IsConsumable() const {
  return buffer.IsConsumable();
}

Status _SparseBase::SetLength(UInt newElms) const {

  Status status;
  UInt   numBytes;
  
  numBytes = (elmSize + HEADSIZE) * newElms;
  status   = ((Buffer *)&buffer)->SetSize(numBytes);
  
  return status;

}

Boolean _SparseBase::IsMutable() const {
  return isMutable;
}

Status _SparseBase::Mutable() {
  isMutable = true;
  return Error::None;
}

Status _SparseBase::Immutable() {
  isMutable = false;
  return Error::None;
}

Boolean _SparseBase::_Contains(Byte *that) const {
  //Status  status;
  Boolean answer;
  /*status =*/ _Contains(that, answer);
  return answer;
}

Status _SparseBase::_Contains(Byte *that, Boolean &answer) const {
  
  Byte   *buf;
  UInt    bufSize;
  UInt    phySize;
  UInt    numElms;
  UInt    curIdx;
  
  answer = false;
  ((_SparseBase *)this)->_BufAndSize(buf, bufSize);
  if (buf && bufSize && elmSize) {
    phySize = elmSize + sizeof(Int);
    numElms = bufSize/phySize;
    for (curIdx=0; curIdx < numElms; curIdx++) {
      if (MemoryCompareRaw(that, buf+curIdx*phySize+sizeof(Int), elmSize) == 0) break;
    }
    answer = curIdx < numElms;
  }
    
  return Error::None;
  
}

//----------------------------------------------------------------------
//
// Contents info
//

UInt _SparseBase::Length() const {
  
  UInt phySize;
  UInt len;
  
  phySize = elmSize + HEADSIZE;
  if (phySize == 0)
    BEFE_ABORT("Sparse.Length: elmSize == 0");
    
  len = buffer.Length();
  if ( phySize == 0 || BEFE::IsNull(len) )
    len = 0;
  else
    len = len/phySize;
    
  return len;

}

UInt _SparseBase::Length(UInt index) const {
  
  UInt answer;
  UInt len;
  
  len = buffer.Length();
  
  if (elmSize)
    answer = len/(elmSize+HEADSIZE);
  else
    answer = 0;
    
  return answer;
  
}

UInt _SparseBase::DistinctLength() const {
  
  UInt  phySize;
  Byte *buf;
  UInt  numElms;
  UInt  elmNo;
  UInt  prevElm;
  UInt  curElm;
  UInt  count;
  
  phySize = elmSize + HEADSIZE;
  if (phySize == 0)
    BEFE_ABORT("Sparse.DistinctLength: elmSize == 0");

  buf     = buffer.GetBuf();    
  numElms = buffer.Length();
  if ( BEFE::IsNull(numElms) )
    numElms = 0;
  else
    numElms = numElms/phySize;

  BEFE::SetNull(prevElm);
  count   = 0;
  for (elmNo=0; elmNo < numElms; elmNo++) {
    curElm = *(UInt *)(buf + elmNo*phySize);    
    if (curElm != prevElm)
      count++;
    prevElm = curElm;
  }
  
  return count;

}

UInt _SparseBase::Size() const {
  return buffer.Size();
}

UInt _SparseBase::ElementSize() const {
  return elmSize;
}

UInt _SparseBase::UnitSize() const {
  return StorageUnit::Byte;
}

USpan _SparseBase::IndexSpan() const {
  
  UInt    len;
  USpan   uspan;
  Byte   *buf;
  UInt   *header;
  
  len = buffer.GetUsedBytes();
  buf = buffer.GetBuf();
  
  while (len && elmSize) { // If we have elements...
    
    // Get index of first one
    header = (UInt *)buf;
    if (!header) break;
    uspan.idx1 = header[0];
    
    // Get index of last one
    header = (UInt *)(buf + len - (elmSize+HEADSIZE));
    uspan.idx2 = header[0] + 1;
    
    break;
    
  } // ...If we have elements
    
  return uspan;
  
}

USpan _SparseBase::SubIndexSpan(UInt index) const {
  
  USpan uspan;
  USpan retSpan;
  
  uspan = PhysicalIndexSpan(index);
  retSpan.idx1 = 0;
  retSpan.idx2 = uspan.Length();
  
  return retSpan;
  
}

Boolean _SparseBase::Exists(UInt index) const {
  
  USpan subSpan;
  UInt  len;
  
  subSpan = SubIndexSpan(index);
  len = subSpan.Length();
  if ( BEFE::IsNull(len) )
    len = 0;
    
  return len != 0;

}

//----------------------------------------------------------------------
//
// Abstract Element Methods
//

Status _SparseBase::_Get(UInt index, void *buf) const {
  
  Status  status;
  UInt    resIndex;
  Byte   *element;
  Int     subIndex;
  
  // See if we can find it
  BEFE::SetNull(subIndex);
  status = ResolveGet(index, subIndex, resIndex);
  if (status) goto SOMEERROR;

  // Get the element address
  element = (Byte *)_GetPhyAddress(resIndex);
  if (!element) goto NULLPOINTER;
  
  // Get the element...
  MemoryCopyRaw(element, (Byte *)buf, elmSize);
  
  // Handle errors  
  while (false) {
    NULLPOINTER: BEFE_WARN("Sparse.Get(UInt): NULL element pointer");
                 status = Error::InternalNullPointer;
                 break;
    SOMEERROR:   break;
  }
  
  return status;

}

Status _SparseBase::_Get(UInt index, Int subindex, void *buf) const {
  
  Status  status;
  UInt    resIndex;
  Byte   *element;
  
  // See if we can find it
  status = ResolveGet(index,subindex,resIndex);
  if (status) goto SOMEERROR;

  // Get the element address
  element = buffer.GetBuf() + (elmSize+HEADSIZE)*resIndex;
  if (!element) goto NULLPOINTER;
  element += HEADSIZE;
  
  // Get the element...
  MemoryCopyRaw(element, (Byte *)buf, elmSize);
  
  // Handle errors
  while (false) {
    NULLPOINTER: BEFE_WARN("Sparse.Get(UInt,Int): NULL element pointer");
                status = Error::InternalNullPointer;
                break;
    SOMEERROR:  break;
  }
  
  return status;

}

Status _SparseBase::_Set(UInt index, void *buf) {
  
  Status  status;
  UInt    resIndex;
  Byte   *element;
  Int     subIndex;
  
  // See if we can find it
  BEFE::SetNull(subIndex);
  status = ResolveGet(index, subIndex, resIndex);
  if (status == Error::IndexNotFound) {
    status = _Append(index,buf);
    if (status) goto SOMEERROR;
    goto OK;
  }
  if (status) goto SOMEERROR;
  
  // Get the element address
  element = (Byte *)_GetPhyAddress(resIndex);
  if (!element) goto NULLPOINTER;
  
  // Set the element...
  MemoryCopyRaw((Byte *)buf, element, elmSize);
  
  // Handle errors
  while (false) {
    NULLPOINTER: BEFE_WARN("Sparse.Set(UInt): NULL element pointer");
                 status = Error::InternalNullPointer;
                 break;
    OK:          status = Error::None; break;
    SOMEERROR:                         break;
  }
  
  return status;

}

Status _SparseBase::_Set(UInt index, Int subindex, void *buf) {
  
  Status  status;
  UInt    resIndex;
  Byte   *element;

  if (!isMutable) goto IMMUTABLE;
  
  // See if we can find it
  status = ResolveGet(index,subindex,resIndex);
  if (status) goto SOMEERROR;

  // Get the element address
  element = buffer.GetBuf() + (elmSize+HEADSIZE)*resIndex;
  if (!element) goto NULLPOINTER;
  element += HEADSIZE;
  
  // Get the element...
  MemoryCopyRaw((Byte *)buf, element, elmSize);
  
  // Handle errors
  while (false) {
    NULLPOINTER: BEFE_WARN("Sparse.Set(UInt,Int): NULL element pointer");
                 status = Error::InternalNullPointer;
                 break;
    IMMUTABLE:   status = Error::AccessDenied; break;
    SOMEERROR:                                 break;
  }
  
  return status;

}

Status _SparseBase::_Append(void *buf) {
  UInt index;  
  if (!isMutable)
    return Error::AccessDenied;  
  BEFE::SetNull(index);
  return _Append(index, buf);
}

Status _SparseBase::_Append(UInt index, void *buf) {
  
  Status  status;
  UInt    resIndex;
  Byte   *element;
  UInt    phySize;
  USpan   indexSpan;
  UInt    spanLength;
  Int     subIndex;
  
  if (!isMutable) goto IMMUTABLE;
  
  // Get the index if it wasn't given and we can...
  if ( BEFE::IsNull(index) ) {
    indexSpan = IndexSpan();      
    spanLength = indexSpan.Length();
    if ( BEFE::IsNull(spanLength) || spanLength == 0 )
      index = 0;
    else
      index = indexSpan.idx2;
  }
  
  // See if we can find where it insert it
  BEFE::SetNull(subIndex);
  status = ResolveInsert(index, subIndex, resIndex);
  if (status) goto SOMEERROR;

  // Allocate the buffer if it's empty
  status = AllocateBuffer();
  if (status) goto SOMEERROR;
  
  // Insert an empty one
  phySize = elmSize + HEADSIZE;
  status = buffer.InsertBytes(phySize*resIndex,NULL,phySize);
  if (status) goto SOMEERROR;
    
  // Get the element address
  element = buffer.GetBuf() + phySize*resIndex;
  if (!element) goto NULLPOINTER;
  
  // Update the Index value
  *((UInt *)element) = index;
  element += sizeof(index);
  
  // Get the element...
  MemoryCopyRaw((Byte *)buf, element, elmSize);
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: BEFE_WARN("Sparse.Append(UInt): NULL element pointer");
                 status = Error::InternalNullPointer;
                 break;
    IMMUTABLE:   status = Error::AccessDenied; break;
    SOMEERROR:                                 break;
  }
  
  return status;

}

Status _SparseBase::_Remove(UInt index) {
  
  Status  status;
  UInt    resIndex;
  UInt    phySize;
  Int     subIndex;
  
  if (!isMutable) goto IMMUTABLE;

  // See if we can find it
  BEFE::SetNull(subIndex);
  status = ResolveGet(index, subIndex, resIndex);
  if (status) goto SOMEERROR;

  // Remove the element... 
  phySize = elmSize+HEADSIZE;
  status = buffer.RemoveBytes(resIndex*phySize,phySize);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    IMMUTABLE:                                                   status = Error::AccessDenied;        break;
    SOMEERROR:                                                                                        break;
  }
  
  return status;

}

Status _SparseBase::_Remove(UInt index, Int subindex) {
  
  Status  status;
  UInt    resIndex;
  UInt    phySize;
  
  if (!isMutable) goto IMMUTABLE;
  
  // See if we can find it
  status = ResolveGet(index,subindex,resIndex);
  if (status) goto SOMEERROR;

  // Remove the element... 
  phySize = elmSize+HEADSIZE;
  status = buffer.RemoveBytes(resIndex*phySize,phySize);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    IMMUTABLE:                                                   status = Error::AccessDenied;        break;
    SOMEERROR:                                                                                        break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Physical Element Methods
//

Status _SparseBase::_GetPhysicalElement(UInt phyIndex, UInt &index, void *callerBuf) const {

  Status  status;
  Byte   *buf;
  UInt    numBytes;
  UInt    numElms;
  
  // Not sure what to do if it's self-indexing...
  BEFE::SetNull(index);
  if (BEFE::IsNull(callerBuf)) goto INVALIDPARAM;
  
  // Figure out where it is
  buf = buffer.GetBuf();
  numBytes = buffer.GetUsedBytes();
  numElms  = numBytes/elmSize;
  if (phyIndex >= numElms) goto OUTOFRANGE;
  if (BEFE::IsNull(buf)) goto NULLPOINTER;
  buf = buf + (elmSize + HEADSIZE)*phyIndex;
  
  // Get the info
  index = *(UInt *)buf;
  MemoryCopyRaw(buf+HEADSIZE, (Byte *)callerBuf, elmSize);
  
  // Handle errors
  status = Error::None;
  while (false) {
    INVALIDPARAM: status = Error::InvalidParameter;    break;
    OUTOFRANGE:   status = Error::IndexOutOfRange;     break;
    NULLPOINTER:  status = Error::InternalNullPointer; break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Protected methods
//
//----------------------------------------------------------------------

Status _SparseBase::AllocateBuffer() {

  Status status;
  Alloc  alloc;
  UInt   phySize;  
  UInt   minElms;
  UInt   incElms;

  if (buffer.pointer)
    status = Error::None;
  else {
    status = alloc.SetOpen();
    if (status) goto SOMEERROR;
    alloc = alloc.Normalise();
    phySize = elmSize + HEADSIZE;
    minElms = alloc.MinElms();
    if ( !BEFE::IsNull(minElms) )
      minElms = minElms*phySize;
    incElms = alloc.IncElms();
    if ( !BEFE::IsNull(incElms) )
      incElms = incElms*phySize;
    status = buffer.Allocate(minElms, incElms);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

USpan _SparseBase::LogicalIndexSpan() const {

  USpan   uspan;
  Byte   *element;  
  UInt    len;

  uspan.idx1 = 0;
  uspan.idx2 = 0;
  
  // No range if no buffer
  len = buffer.Length();
  if ( BEFE::IsNull(len) || len == 0 ) goto OK;
  
  // Get first one
  element = buffer.GetBuf();
  if (BEFE::IsNull(element)) goto NOBUFFER;
  uspan.idx1 = *(UInt *)element;
    
  // Get last one
  element = element + len - elmSize - HEADSIZE;
  uspan.idx2 = *(UInt *)element + 1;
  
  // Handle errors
  while (false) {
    NOBUFFER:  break;
    OK:        break;
  }
  
  return uspan;
  
}

USpan _SparseBase::PhysicalIndexSpan() const {
  
  Status status;
  UInt   len;               // Buffer length in bytes
  UInt   numElms;           // Number of elements
  UInt   phySize;           // Total element size (including header)
  USpan  phySpan;
  
  len = buffer.Length();
  if ( BEFE::IsNull(len) ) goto FINISHED;
  phySize  = elmSize + HEADSIZE;
  if (phySize == 0) goto BADSTATE;
  if (!len) goto FINISHED;

  numElms = len / phySize;
  phySpan.idx1 = 0;
  phySpan.idx2 = numElms;
  
  // Handle errors
  while (false) {
    FINISHED: break;
    BADSTATE: status = Error::Internal;
              BEFE_ERROR("Sparse.PhysicalIndexSpan",status);
              break;
  }
  
  return phySpan;
  
}

USpan _SparseBase::PhysicalIndexSpan(UInt index) const {
  
  Status  status;
  UInt    ll;               // ◄─┐
  UInt    ul;               // ◄─┼─ For binary search
  UInt    mid;              // ◄─┘
  Byte   *buf;              // ◄─┬─ Physical buffer and length
  UInt    len;              // ◄─┘    (in Bytes)
  
  USpan   phySpan;          // Physical buffer offset range
  UInt    numElms;          // Number of elements in range
  UInt    phySize;          // Total element size (including header)
  UInt    curIdx;           // Current Index
  Byte   *element;          // Current Element (at curIndex in buf)
  Boolean foundit;          // "Did we find it?"
  
  buf = buffer.GetBuf();
  len = buffer.Length();
  if ( BEFE::IsNull(len) || len == 0 ) goto FINISHED;
  phySize = elmSize + HEADSIZE;
  if (phySize == 0) goto BADSTATE;

  numElms = len / phySize;

  phySpan.idx1 = 0;
  phySpan.idx2 = numElms-1;
  
  if ( !BEFE::IsNull(index) ) { // Index specified...
  
    // Binary search for it...
    foundit = false;
    ll = phySpan.idx1;
    ul = phySpan.idx2;
    while (ul >= ll) {
      mid = (ll + ul) >> 1;
      element = buf + mid*phySize;
      curIdx = *(UInt *)element;
      if (curIdx < index)
        ll = mid+1;
      else if (curIdx > index) {
        ul = mid-1;
        if ( BEFE::IsNull(ul) ) break;
      }
      else {
        foundit = true;
        break;
      }
    }
    
    // If we found it...
    if (foundit) { // Found it...

      phySpan.idx1 = mid;
      phySpan.idx2 = mid;

      // No searching to do if is Distinct
      if (isDistinct) {
        phySpan.idx2++;
        goto FINISHED;
      }
      
      // Pre-position to the next one either way
      phySpan.idx1--;
      phySpan.idx2++;
      
      // Find first one...
      foundit = false;
      while ( !BEFE::IsNull(phySpan.idx1) ) {
        element = buf + phySpan.idx1*phySize;
        curIdx = *(UInt *)element;
        if (curIdx < index) {
          foundit = true;
          break;
        }
        phySpan.idx1--;
      }
      if (foundit || BEFE::IsNull(phySpan.idx1) )
        phySpan.idx1++;
        
      // Find last one...
      foundit = false;
      while (phySpan.idx2 < numElms) {
        element = buf + (phySpan.idx2) * phySize;
        curIdx = *(UInt *)element;
        if (curIdx > index) {
          foundit = true;
          break;
        }
        phySpan.idx2++;
      }
      
      goto FINISHED;
      
    } // ...Found it

    // Didn't find it...
    phySpan.idx1 = ll;
    phySpan.idx2 = ul+1;
    
    goto FINISHED;    

  }

  goto FINISHED;
  
  // Handle errors
  while (false) {
    FINISHED: status = Error::None;
              break;
    BADSTATE: BEFE_WARN("Invalid Sparse State");
              status = Error::Internal;
              break;
  }

  // If error, resolve to zero
  if (status != Error::None)
    phySpan = USpan();
    
  return phySpan;
    
}

Status _SparseBase::ResolveInsert(UInt index, Int subindex, UInt &resolved) const {
  
  Status status;
  USpan  phySpan;
  UInt   len;

  resolved = 0;
  phySpan = PhysicalIndexSpan(index);
  if ( BEFE::IsNull(phySpan.idx1) || BEFE::IsNull(phySpan.idx2) ) goto FINISHED;

  // Adjust the resolved index
  resolved = phySpan.idx1;
  len = phySpan.Length();
  if ( !BEFE::IsNull(subindex) )
    resolved += Min((UInt)subindex,len);
  else
    resolved += len;
    
  // Handle errors
  status = Error::None;
  while (false) {
    FINISHED: status = Error::None; break;
  }
  
  return status;
  
}

Status _SparseBase::ResolveGet(UInt index, Int subindex, UInt &resolved) const {
  
  Status status;
  USpan  phySpan;
  UInt   len;

  resolved = 0;
  phySpan = PhysicalIndexSpan(index);
  len = phySpan.Length();
  if ( len == 0 || BEFE::IsNull(len) ) goto NOTEXIST;
  
  // Make sure there's only one there
  if ( len != 1 && BEFE::IsNull(subindex) ) goto AMBIGUOUS;
  
  // If subindex specified, make sure it's in range
  if ( !BEFE::IsNull(subindex) && subindex >= (Int)phySpan.Length() ) goto OUTOFRANGE;
  
  // Adjust the resolved index
  resolved = phySpan.idx1;
  if ( !BEFE::IsNull(subindex) )
    resolved += subindex;
    
  // Handle errors
  status = Error::None;
  while (false) {
    NOTEXIST:   status = Error::IndexNotFound;    break;
    AMBIGUOUS:  status = Error::IndexNotDistinct; break;
    OUTOFRANGE: status = Error::IndexOutOfRange;  break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Template specific methods
//

void _SparseBase::_BufAndSize(Byte *&buf, UInt &len) const {
  
  buf = buffer.GetBuf();
  len = buffer.GetUsedBytes();

  return;
  
}
  
Status _SparseBase::_BufAlloc() {

  Status status;
  Alloc  alloc("0..");
  UInt   minElms;
  UInt   incElms;

  if (buffer.pointer)
    status = Error::None;
  else {
    alloc = alloc.Normalise(elmSize);
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

Status _SparseBase::_BufAlloc(Alloc alloc) {

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

Byte const *_SparseBase::_GetPhyAddress(UInt phyIndex) const {
  
  Byte *phyAddress;
  
  phyAddress = buffer.GetBuf();
  if (phyAddress && phyIndex < Length())
    phyAddress += phyIndex * (elmSize + HEADSIZE) + HEADSIZE;
  else
    phyAddress = NULL;
    
  return phyAddress;
  
}

UInt _SparseBase::_GetPhyElementSize() const {
  return elmSize + HEADSIZE;
}

UInt _SparseBase::_GetPhyHeaderSize() const {
  return HEADSIZE;
}

Byte const *_SparseBase::_GetLogAddress(UInt logIndex) const {
  
  Status  status;
  UInt    resIndex;
  Byte   *element;
  Int     subIndex;
  
  element = NULL;
  BEFE::SetNull(subIndex);
  status = ResolveGet(logIndex, subIndex, resIndex);
  if (status) goto SOMEERROR;
  element = buffer.GetBuf();
  if (element)
    element += resIndex * (elmSize + HEADSIZE) + HEADSIZE;  
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }  
    
  return element;
}

//----------------------------------------------------------------------
//
// _PhyInsert - Perform a low level _SparseBase Insert
//
// Note: Use this at your peril since passing the wrong logindex can
//       really screw up the whole thing!!!
//

Status _SparseBase::_PhyInsert(UInt phyIndex, UInt logIndex, Byte *&phyElm) {

  Status  status;
  UInt    length;
  UInt    phySize;
  
  if (!isMutable) goto IMMUTABLE;

  length = Length();
  if (phyIndex > length)
    phyIndex = length;
  
  // Allocate the buffer if it's empty
  status = AllocateBuffer();
  if (status) goto SOMEERROR;
  
  // Insert an empty one
  phySize = elmSize + HEADSIZE;
  status = buffer.InsertBytes(phySize*phyIndex, NULL, phySize);
  if (status) goto SOMEERROR;
    
  // Get the element address
  phyElm = buffer.GetBuf();
  if (BEFE::IsNull(phyElm)) goto NULLPOINTER;
  phyElm += phySize*phyIndex;
  
  // Update the Index value
  *((UInt *)phyElm) = logIndex;
  phyElm += sizeof(UInt);
  
  // Handle errors
  status = Error::None;
  while (false) {
    IMMUTABLE:   status = Error::AccessDenied;
                 break;
    NULLPOINTER: status = Error::InternalNullPointer;
                 BEFE_ERROR("_SparseBase._PhyInsert: NULL Pointer",status);
    SOMEERROR:   break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// _PhyRemove - Perform a low level _SparseBase Remove
//

Status _SparseBase::_PhyRemove(UInt phyIndex) {

  Status  status;
  UInt    length;
  UInt    phySize;

  if (!isMutable) goto IMMUTABLE;
  
  length = Length();
  if (phyIndex >= length) goto OUTOFRANGE;
  
  // Insert an empty one
  phySize = elmSize + HEADSIZE;
  status = buffer.RemoveBytes(phySize*phyIndex, phySize);
  if (status) goto SOMEERROR;
    
  // Handle errors
  while (false) {
    IMMUTABLE:   status = Error::AccessDenied;    break;
    OUTOFRANGE:  status = Error::IndexOutOfRange; break;
    SOMEERROR:                                    break;
  }
  
  return status;

}

} // ...Namespace BEFE
