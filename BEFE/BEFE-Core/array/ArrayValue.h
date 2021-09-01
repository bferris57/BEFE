//!befe-publish inc
//----------------------------------------------------------------------
// File: ArrayValue.h - Declarations for the ArrayValue template
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef ARRAYVALUE_H // ARRAYVALUE_H...
#define ARRAYVALUE_H

#include "Primitives.h"
#include "Array.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// ArrayValue Template Class
//

template <typename V> class ArrayValue : public _ArrayBase { // Template ArrayValue<V>...

	// Special members...
	//public: V _bogusReference; // Used as target when bad indexes are given. All we're
	//                           // saying is indeterminate behaviour is better than GPFs.

  // C++ Lifecycle overrides
  public: ArrayValue()  {StartUp();}
  public: ArrayValue(ArrayValue<V> const &that) {CopyFrom(that);}
  public: ~ArrayValue() {ShutDown();}
  public: ArrayValue<V> &operator=(ArrayValue <V> const &that) {
    CopyFrom(that);
    NotConsumable();
	return *this;
  }
  public: V &operator[](Int index) {
  	static V bogus;
  	V *that = (V *)_GetAddress(index);
  	if (!that)
  		that = &bogus;
  	return *that;
  }

  // BEFE Lifecycle overrides
  public: Status StartUp() {
    Status status;
    status = _ArrayBase::StartUp();
    elmSize = sizeof(V);
    return status;
  }
  
  public: Status ShutDown() {

    Status retStatus;
    Status status;
    UInt   numElms;
    V     *curElm;
    
    retStatus = Error::None;
    _BufAndLength(*(Byte **)&curElm,numElms);
    while (numElms--) {
      status = curElm->ShutDown();
      if (status && !retStatus) retStatus = status;      
      curElm++;
    }
    
    status = _ArrayBase::ShutDown();
    if (status && !retStatus) retStatus = status;
    
    return retStatus;

  }

  public: Status Reset() {
    
    Status retStatus;
    Status status;
    
    retStatus = ShutDown();
    status = StartUp();
    if (status && !retStatus) retStatus = status;
    
    return retStatus;
    
  }

  public: Status SetNull() {
    return Reset();
  }
  
  public: Status SetEmpty() {
    return SetLength(0);
  }
  
  public: Status CopyFrom(ArrayValue<V> const &that) {
    
    Status status;
    UInt   numElms;
    UInt   curIdx;
    V     *thisElm;
    V     *thatElm;
    
    status = Reset();
    if (status) goto SOMEERROR;
    if (that.IsConsumable())
      status = MoveFrom(that);
    else {
      status = _ArrayBase::CopyFrom(that);
      if (status) goto SOMEERROR;
      numElms = Length();
      thisElm = (V *)_GetAddress(0);
      for (curIdx=0; curIdx < numElms; curIdx++)
        (thisElm++)->StartUp();
      thisElm = (V *)_GetAddress(0);
      thatElm = (V *)((_ArrayBase *)&that)->_GetAddress(0);
      for (curIdx=0; curIdx < numElms; curIdx++)
        (thisElm++)->CopyFrom(*(thatElm++));
    }
    
    // Handle errors
    while (false) {
      SOMEERROR: break;
    }
    
    return status;

  }

  public: ArrayValue<V> &Consumable() const {
    _ArrayBase::Consumable();
    return *(ArrayValue<V> *)this;
  }
  
  public: ArrayValue<V> &NotConsumable() const {
    _ArrayBase::NotConsumable();
    return *(ArrayValue<V> *)this;
  }

  public: Status Allocate(Alloc const &alloc) {

    Status  retStatus;
    Status  status;
    V      *curVal;
    UInt    numElms;
    
    retStatus = _ArrayBase::Allocate(alloc);
    if (retStatus) return retStatus;
    _BufAndLength(*(Byte **)&curVal, numElms);
    if (curVal && elmSize) {
      while (numElms) {
        status = curVal->StartUp();
        if (status && !retStatus) retStatus = status;
        ++curVal;
        --numElms;
      } 
    }
    else
      retStatus = Error::None;
      
    return retStatus;
  }
  
  public: Status SetLength(UInt newElms) {

    Status retStatus;
    Status status;
    UInt   numElms;
    V     *firstElm;
    Int    curIdx;
    
    numElms = Length();
    retStatus = Error::None;
    if (newElms < numElms) {
      firstElm = (V *)_GetAddress(0);
      for (curIdx=(Int)numElms-1; curIdx >= (Int)newElms; curIdx--) {
        status = (firstElm+curIdx)->ShutDown();
        if (status && !retStatus) retStatus = status;
      }
    }
    status = _ArrayBase::SetLength(newElms);
    if (status && !retStatus) retStatus = status;
    
    return retStatus;
    
  }
  
  public: Boolean Contains(V const &that) const {
    return _Contains(_CompareElements, (Byte *)&that);
  }
  
  public: Status Contains(V const &that, Int &answer) const {
    answer = _Contains(_CompareElements, (Byte *)&that);
    return 0;
  }
  
  // ArrayValue<V> specific methods
  public: V Get(Int index) const {
    
    //Status  status;
    V      *elmPtr;
    V       local;

    elmPtr = (V *)((ArrayValue<V> *)this)->_GetAddress(index);
    if (elmPtr == NULL) goto OUTOFBOUNDS;
    local = *elmPtr;
    //status = Error::None;
    while (false) {
      OUTOFBOUNDS: /*status = Error::IndexOutOfRange;*/ break;
    }

    return local.Consumable();

  }
  
  public: Status Get(Int index, V &that) const {

    Status  status;
    V      *elmPtr;

    elmPtr = (V *)((ArrayValue<V> *)this)->_GetAddress(index);
    if (elmPtr == NULL) goto OUTOFBOUNDS;
    that = *elmPtr;
    status = Error::None;
    while (false) {
      OUTOFBOUNDS: status = Error::IndexOutOfRange; break;
    }

    return status;

  }

  public: Status Set(Int index, V that) {
    Status  status;
    V      *local;
    local = (V *)_GetAddress(index);
    if (local == NULL)
      status = Error::IndexOutOfRange;
    else {
      *local = that;
      local->NotConsumable();
      status = Error::None;
    }
    return status;
  }

  public: Status Append(V const &that) {
    Status  status;
    V       empty;
    V      *local;
    UInt    numElms;
    numElms = Length();
    status = _Append((Byte *)&empty);
    if (status == Error::None) {
      local = (V *)_GetAddress(numElms);
      if (local) {
        *local = that;
        local->NotConsumable();
      }
    }
    return status;
  }

  public: Status Append(ArrayValue<V> const &that) {
  	Status status = Error::None;
  	UInt   numElms;
  	ArrayValue<V> localThat = that;
  	numElms = that.Length();
  	for (Int i = 0; i < numElms; i++) {
  		status = Append(localThat.Consumable());
  		if (status) break;
  	}
  	return status;
  }

  //public: Status Append(T *those, UInt size)
  //  { return _Append((Byte *)those, size); }

  public: Status Insert(Int index, V const &that) {
    
    Status status;
    V      local;
    
    status = local.CopyFrom(that);
    if (status) goto SOMEERROR;
    local.NotConsumable();
    status = _Insert(index, (Byte *)&local);
    if (status) goto SOMEERROR;
    status = local.StartUp();
    if (status) goto SOMEERROR;
    
    // Handle errors
    while (false) {
      SOMEERROR: break;
    }
    
    return status;
  }

  //public: Status Insert(Int index, T *those, UInt size)
  //  { return _Insert(index, (Byte *)those, size); }
  
  public: Status Remove(Int index) {
    Status  status;
    V      *that;
    that = (V *)_GetAddress(index);
    if (that == NULL) goto OUTOFBOUNDS;
    status = that->ShutDown();
    if (status) goto SOMEERROR;
    status = _Remove(index);
    if (status) goto SOMEERROR;
    while (false) {
      OUTOFBOUNDS: status = Error::IndexOutOfRange; break;
      SOMEERROR:                                     break;
    }
    return status;
  }
  
  public: Status Sort() {
    return _Sort((ArrayCompareFunc)&ArrayValue<V>::_CompareElements);
  }
  
  protected: Status _ResetElements() {
    
    Status retStatus;
    Status status;
    UInt   numElms;
    V     *curElm;
    
    retStatus = Error::None;
    _BufAndLength(*(Byte **)&curElm,numElms);
    while (numElms--) {
      status = curElm->Reset();
      if (status && !retStatus) retStatus = status;      
      curElm++;
    }
    
    status = _ArrayBase::ShutDown();
    if (status && !retStatus) retStatus = status;
    
    return retStatus;

  }
  
  public: static Status _CompareElements(Byte *elm1, Byte *elm2, Int &answer) {
    Status status;
    Int    ourAnswer;
    if (!elm1 || !elm2) {
      status = Error::InternalNullPointer;
      ourAnswer = 0;
    }
    else {
      ourAnswer = ((V *)elm1)->Compare(*(V *)elm2);
      status = Error::None;
    }
    answer = ourAnswer;
    return status;
  }

}; // ...Template ArrayValue<V>

} // ...Namespace BEFE

#endif // ...ARRAYVALUE_H
