//----------------------------------------------------------------------
// File: _ArrayBase.cpp - Implementation of the _ArrayBase class
//----------------------------------------------------------------------
//
// The _ArrayBase class is used as the Base implementation for the
// Array<T> and ArrayValue<V> template classes.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Array.h"

#include "Array.h"
#include "Buffer.h"
#include "Sort.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// General _ArrayBase Functions...
//

Boolean IsNull(_ArrayBase const &v) {
  return v.IsNull();
}
  
//----------------------------------------------------------------------
//
// C++ Lifecycle
//

_ArrayBase::_ArrayBase() {
  StartUp();
};

_ArrayBase::_ArrayBase(_ArrayBase const &that) {
  StartUp();
  CopyFrom(that);
}

_ArrayBase::_ArrayBase(Alloc alloc) {
  StartUp();
  _BufAlloc(alloc);
}

_ArrayBase::~_ArrayBase() {
  ShutDown();
};

//----------------------------------------------------------------------
//
// C++ "Handy Methods"
//

_ArrayBase &_ArrayBase::operator = (_ArrayBase const &that) {
  CopyFrom(that);
  NotConsumable();
  return *this;
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle methods
//

Status _ArrayBase::StartUp() {
  Status status;
  isFixed    = false;
  isLimited  = false;
  isDynamic  = false;
  isSorted   = false;
  indexType  = Sequence::IndexType::LengthWrap;
  elmSize    = 1;
  _reserved  = 0x1FF; // 9 bits
  //_firstIter = 0;
  status = buffer.StartUp();
  return status;
}

Status _ArrayBase::ShutDown() {
  Status status;
  status = buffer.ShutDown();
  return status;
}

Status _ArrayBase::Reset() {

  Status retStatus;
  Status status;
  
  retStatus = buffer.ShutDown();
  status = buffer.StartUp();
  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status _ArrayBase::SetEmpty() {

  Status status;

  status = _BufAlloc();
  if (!status)
    //status = buffer.SetSize(0);
    status = buffer.SetUsedBytes(0);
  return status;

}

Status _ArrayBase::MoveFrom(_ArrayBase const &that) {
  Status status;
  if (this != &that) {
    isLimited = that.isLimited;
    isFixed   = that.isFixed;
    isDynamic = that.isDynamic;
    indexType = that.indexType;
    elmSize   = that.elmSize;
    status = buffer.MoveFrom(that.buffer);
  }
  else
    status = Error::None;
  return status;
}

Status _ArrayBase::CopyFrom(_ArrayBase const &that) {

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
      isDynamic = that.isDynamic;
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

Status _ArrayBase::Consumable() const {
  buffer.Consumable();
  return Error::None;
}

Status _ArrayBase::NotConsumable() const {
  buffer.NotConsumable();
  return Error::None;
}

Boolean _ArrayBase::IsConsumable() const {
  return buffer.IsConsumable();
}

UInt _ArrayBase::Length() const {
  return buffer.GetUsedBytes()/elmSize;
}

UInt _ArrayBase::Size() const {
  return buffer.GetUsedBytes()/elmSize;
}

UInt _ArrayBase::ElementSize() const {
  return elmSize;
}

Status _ArrayBase::Allocate(Alloc const &alloc) {
  
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
  if ( !BEFE::IsNull(ourAlloc.MaxElms()) )
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

Status _ArrayBase::SetLength(UInt newSize) {
  return buffer.SetSize(newSize*elmSize);
}

Boolean _ArrayBase::_Contains(ArrayCompareFunc cmpFunc, Byte *that) const {
  //Status  status;
  Boolean answer;
  /*status =*/ _Contains(cmpFunc, that, answer);
  return answer;
}

Status _ArrayBase::_Contains(ArrayCompareFunc cmpFunc, Byte *that, Boolean &answer) const {
  
  Status  status;
  Byte   *buf;
  UInt    bufSize;
  UInt    numElms;
  UInt    curIdx;
  Int     cmpAnswer;
  
  answer = false;
  ((_ArrayBase *)this)->_BufAndSize(buf, bufSize);
  if (buf && bufSize && elmSize) {
    numElms = bufSize/elmSize;
    for (curIdx=0; curIdx < numElms; curIdx++) {
      status = cmpFunc(that, buf+curIdx*elmSize, cmpAnswer);
      if (status || cmpAnswer == 0) break;
    }
    answer = !status && curIdx < numElms;
  }
    
  return Error::None;
  
}

//----------------------------------------------------------------------
//
// Index resolving
//

UInt _ArrayBase::GetIndexType() {
  return indexType;
}

UInt _ArrayBase::SetIndexType(UInt type) {
  
  UInt oldType;
  
  oldType = indexType;
  if (type < Sequence::IndexType::NumTypes)
    indexType = type;
  
  return oldType;

}

UInt _ArrayBase::ResolveIndex(Int value) {
  return ResolveIndex(indexType, value);  
}

UInt _ArrayBase::ResolveIndex(UInt type, Int value) {

  Int   newValue;
  Byte *that;
  UInt  usedElms;
  
  _BufAndSize(that,usedElms);
  if (elmSize)
    usedElms = usedElms/elmSize;
  else
    usedElms = 0;
    
  switch (type) {
    case Sequence::IndexType::None:    
      newValue = value;
      break;
    case Sequence::IndexType::LengthWrap:
      newValue = value;
      if (newValue < 0)
        newValue = usedElms + newValue;
      if (newValue < 0 || newValue >= (Int)usedElms)
        BEFE::SetNull(newValue);
      break;
    case Sequence::IndexType::Ring:
      if (usedElms == 0)
        newValue = 0;
      else if (value >= 0)
        newValue = value%usedElms;
      else
        newValue = usedElms + value%usedElms;
      if (newValue < 0 || newValue >= (Int)usedElms)
        BEFE::SetNull(newValue);
      break;
    case Sequence::IndexType::Stack:
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
  
  if (newValue < 0 || newValue >= (Int)usedElms)
    BEFE::SetNull(newValue);
  if ( BEFE::IsNull(newValue) )
    newValue = UNaN;
    
  return (UInt)newValue;

}

//----------------------------------------------------------------------
//
// High level Buffer Management
//

Status _ArrayBase::_Get(UInt index, Byte *buf) const {

  Status status;
  UInt   usedLen;
  
  status = _Get(index, buf, 1, usedLen);

  return status;

}

Status _ArrayBase::_Get(UInt index, Byte *buf, UInt bufMaxLen, UInt &usedLen) const {

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

Status _ArrayBase::_Get(Int index, Byte *buf) const {
  
  Status status;
  UInt   usedLen;
  
  status = _Get(index, buf, 1, usedLen);

  return status;
  
}

Status _ArrayBase::_Get(Int index, Byte *buf, UInt bufMaxLen, UInt &usedLen) const {

  Status status;
  UInt   resIndex;
  
  resIndex = ((_ArrayBase *)this)->ResolveIndex(index);
  status   = _Get(resIndex, buf, bufMaxLen, usedLen);
  
  return status;

}

Status _ArrayBase::_Get(Range const &range, _ArrayBase &theArray) const {
  BEFE_WARN("Array._Get(Range &, _ArrayBase &) not implemented yet");
  return Error::NotImplemented;
}

Status _ArrayBase::_Get(Span  const &span,  _ArrayBase &theArray) const {
  BEFE_WARN("Array._Get(Span &, _ArrayBase &) not implemented yet");
  return Error::NotImplemented;
}

Status _ArrayBase::_Get(Slice const &slice, _ArrayBase &theArray) const {
  BEFE_WARN("Array._Get(Slice &, _ArrayBase &) not implemented yet");
  return Error::NotImplemented;
}

Status _ArrayBase::_Set(UInt index, Byte *buf) {
  
  Status status;
  
  status = _Set(index, buf, 1);
  
  return status;

}

Status _ArrayBase::_Set(UInt index, Byte *buf, UInt bufLen) {

  Status status;
  UInt   usedElms;
  UInt   usedBytes;
  
  usedElms = ((Buffer *)&buffer)->GetUsedBytes() / elmSize;
  if (index >= usedElms) goto OUTOFRANGE;
  
  usedBytes = 0;
  status = buffer.SetBytes(index*elmSize, buf, bufLen*elmSize, usedBytes);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    OUTOFRANGE:    status = Error::IndexOutOfRange; break;
    SOMEERROR:                                      break;
    //INTERNALERROR: status = Error::Internal;        break;
  }
  
  return status;

}

Status _ArrayBase::_Set(Int index, Byte *buf) {
  
  Status status;
  
  status = _Set(index, buf, 1);
  
  return status;

}

Status _ArrayBase::_Set(Int index, Byte *buf, UInt bufLen) {

  Status  status;
  UInt    resIndex;
  
  resIndex = ResolveIndex(index);
  status   = _Set(resIndex, buf, bufLen);
  
  return status;

}

Status _ArrayBase::_Append(Byte *buf) {

  Status status;

  status = _BufAlloc();
  if (status) goto SOMEERROR;

  status = buffer.AppendBytes(buf,elmSize);
  if (status) goto SOMEERROR;

  // Presume not sorted now
  isSorted = false;

  // Handle errors
  while (false) {
    SOMEERROR:                               break;
  }

  return status;

}

Status _ArrayBase::_Append(Byte *buf, UInt bufLen) {

  Status status;

  status = _BufAlloc();
  if (status) goto SOMEERROR;

  status = buffer.AppendBytes(buf,bufLen*elmSize);
  if (status) goto SOMEERROR;

  // Presume not sorted now
  isSorted = false;

  // Handle errors
  while (false) {
    SOMEERROR:                               break;
  }

  return status;

}

Status _ArrayBase::_Insert(UInt index, Byte *buf) {
  
  Status status;
  
  status = _Insert(index, buf, 1);
  
  return status;
  
}

Status _ArrayBase::_Insert(UInt index, Byte *buf, UInt bufLen) {

  Status status;
  UInt   usedElms;

  usedElms = ((Buffer *)&buffer)->GetUsedBytes() / elmSize;
  if (index > usedElms) goto OUTOFRANGE;
  
  status = _BufAlloc();
  if (status) goto SOMEERROR;

  status = buffer.InsertBytes(index*elmSize,buf,bufLen*elmSize);
  if (status) goto SOMEERROR;

  // Presume not sorted now
  isSorted = false;

  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFRANGE: status = Error::IndexOutOfRange; break;
    SOMEERROR:                                   break;
  }

  return status;

}

Status _ArrayBase::_Insert(Int index, Byte *buf) {
  
  Status status;
  
  status = _Insert(index, buf, 1);
  
  return status;
  
}

Status _ArrayBase::_Insert(Int index, Byte *buf, UInt bufLen) {

  Status status;
  UInt   resIndex;
  
  resIndex = ResolveIndex(index);
  status   = _Insert(resIndex, buf, bufLen);
  
  return status;

}

Status _ArrayBase::_Remove(UInt index) {
  
  Status status;
  
  status = _Remove(index, 1);
  
  return status;
  
}

Status _ArrayBase::_Remove(UInt index, UInt removeLen) {

  Status status;
  UInt   usedElms;

  if ( removeLen == 0 || BEFE::IsNull(removeLen) ) goto OK;
  usedElms = ((Buffer *)&buffer)->GetUsedBytes() / elmSize;
  if (index > usedElms) goto OUTOFRANGE;
  
  status = _BufAlloc();
  if (status) goto SOMEERROR;

  status = buffer.RemoveBytes(index*elmSize,removeLen*elmSize);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    OK:         status = Error::None;            break;
    OUTOFRANGE: status = Error::IndexOutOfRange; break;
    SOMEERROR:                                   break;
  }

  return status;

}

Status _ArrayBase::_Remove(Int index) {
  
  Status status;
  
  status = _Remove(index, 1);
  
  return status;
  
}

Status _ArrayBase::_Remove(Int index, UInt removeLen) {

  Status status;
  UInt   resIndex;
  
  resIndex = ResolveIndex(index);
  status   = _Remove(resIndex, removeLen);

  return status;

}

Status _ArrayBase::_Remove(Span const &span) {
  
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
  status = _Remove(start, rLen);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    OK:         status = Error::None;            break;
    OUTOFRANGE: status = Error::IndexOutOfRange; break;
    SOMEERROR:                                   break;
  }
  
  return status;

}

Status _ArrayBase::_Remove(USpan const &span) {
  
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
  status = _Remove(Min(rSpan.idx1, rSpan.idx2), len);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    OK:         status = Error::None;            break;
    OUTOFRANGE: status = Error::IndexOutOfRange; break;
    SOMEERROR:                                   break;
  }
  
  return status;

}

Status _ArrayBase::_Remove(Range const &range) {
  
  Status status;
  Span   span;
  
  span.idx1 = range.idx1;
  span.idx2 = range.idx2+1;
  status = _Remove(span);

  return status;
  
}

Status _ArrayBase::_Remove(URange const &range) {

  Status status;
  USpan  span;
  
  span.idx1 = range.idx1;
  span.idx2 = range.idx2+1;
  status = _Remove(span);

  return status;
  
}

Status _ArrayBase::_Remove(Slice const &slice) {
  
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
    status = _Remove(span.idx1,len);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Remove one by one...
  rSlice.step = Abs(rSlice.step); 
  for (curIdx=span.idx1; (Int)curIdx < span.idx2; curIdx += rSlice.step-1) {
    status = _Remove(curIdx,1);
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
// Elements as a whole
//

Status _ArrayBase::Reverse() {
  
  Byte   *buf;
  UInt    len;
  UInt    curIdx1;
  UInt    curIdx2;
  
  _BufAndLength(buf, len);
  if ( BEFE::IsNull(buf) || len == 0 || elmSize == 0) goto FINISHED;
  curIdx1 = 0;
  curIdx2 = len-1;
  while (curIdx1 < curIdx2) {
    MemoryExchangeRaw(buf+curIdx1*elmSize, buf+curIdx2*elmSize, elmSize);
    curIdx1++;
    curIdx2--;
  }

  // Presume not sorted now
  isSorted = false;

  // Handle errors
  while (false) {
    FINISHED: break;
  }
  
  return Error::None;
  
}

//----------------------------------------------------------------------
//
// Low level Buffer Management
//

UInt _ArrayBase::GetAllocElements() {
  UInt numBytes;
  numBytes = GetAllocBytes();
  if (elmSize && !BEFE::IsNull(elmSize))
    numBytes /= elmSize;
  return numBytes;
}

UInt _ArrayBase::GetIncrementElements() {
  UInt numBytes;
  numBytes = GetIncrementBytes();
  if (elmSize && !BEFE::IsNull(elmSize))
    numBytes /= elmSize;
  return numBytes;
}

UInt _ArrayBase::GetUsedElements() {
  UInt numBytes;
  numBytes = GetUsedBytes();
  if (elmSize && !BEFE::IsNull(elmSize))
    numBytes /= elmSize;
  return numBytes;
}

UInt _ArrayBase::GetAllocBytes() {

  UInt size;

  size = buffer.GetAllocBytes();

  return size;
  
}

UInt _ArrayBase::GetIncrementBytes() {

  UInt size ;

  size = buffer.GetIncrementBytes();

  return size;
  
}

UInt _ArrayBase::GetUsedBytes() {

  UInt size;

  size = buffer.GetUsedBytes();

  return size;
  
}

Byte *_ArrayBase::_GetBuf() {

  Byte *buf;

  buf = buffer.GetBuf();

  return buf;
  
}

Byte *_ArrayBase::_GetAddress(Int index) {

  Byte *buf;
  UInt  resIndex;

  resIndex = ResolveIndex(index);
  if ( BEFE::IsNull(resIndex) || elmSize == 0 )
    buf = NULL;
  else
    buf = buffer.GetBuf() + resIndex*elmSize;

  return buf;
  
}

void _ArrayBase::_BufAndLength(Byte *&buf, UInt &length) {

  buf    = buffer.GetBuf();
  length = buffer.GetUsedBytes();
  if (elmSize)
    length /= elmSize;
  else
    length = 0;
  return;

}

void _ArrayBase::_BufAndSize(Byte *&buf, UInt &size) {

  buf  = buffer.GetBuf();
  size = buffer.GetUsedBytes();

  return;

}

Status _ArrayBase::_BufAlloc() {

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
    status = buffer.Allocate(minElms, incElms);
  }

  return status;

}

Status _ArrayBase::_BufAlloc(Alloc alloc) {

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

Status _ArrayBase::_CompareElements(Byte *elm1, Byte *elm2, Int &answer) {
  
  answer = 0;

  return Error::None;
  
}

//----------------------------------------------------------------------
//
// Array Sorting
//

class ArraySortTarget : public SortTarget {

  _ArrayBase       *theArray;
  ArrayCompareFunc  theCmpFunc;
  Byte             *theBuf;
  UInt              numElms;
  UInt              theElmSize;
  
  public: virtual ~ArraySortTarget() {};

  public: ArraySortTarget(_ArrayBase *that, ArrayCompareFunc thatCmpFunc) {
    theArray   = that;
    theCmpFunc = thatCmpFunc;
    theElmSize = that->elmSize;
    that->_BufAndLength(theBuf,numElms);
  }
  
  // SortTarget Overrides (required)
  virtual UInt Length () {
    return theArray->Length();
  }
  
  virtual Int Compare (UInt elm1, UInt elm2) {
    Status status;
    Int    answer;
    Byte   *elm1Ptr;
    Byte   *elm2Ptr;
    elm1Ptr = theBuf + elm1*theElmSize;
    elm2Ptr = theBuf + elm2*theElmSize;
    status = theCmpFunc(elm1Ptr, elm2Ptr, answer);
    if (status) answer = 0;
    return answer;
  }
  
  virtual Status Swap (UInt elm1, UInt elm2) {
    Byte *elm1Ptr;
    Byte *elm2Ptr;
    elm1Ptr = theBuf + elm1*theElmSize;
    elm2Ptr = theBuf + elm2*theElmSize;
    MemoryExchangeRaw(elm1Ptr, elm2Ptr, theElmSize);
    return Error::None;
  }
  
  virtual Status  Move (UInt elm1, UInt elm2) {
    return Error::NotImplemented;
  }
  
  virtual Boolean Selected (UInt elm) {
    return true;
  }
  
};

Status _ArrayBase::_Sort(ArrayCompareFunc cmpFunc) {

  Status           status;
  ArraySortTarget  target(this, cmpFunc);
  
  status = SortInsertion(target);
  
  return status;
}

}  // Namespace BEFE
