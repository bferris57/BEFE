//!befe-publish inc
//----------------------------------------------------------------------
// File: ArrayPrimitive.h - Declarations for the ArrayPrimitive template
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef ARRAYPRIMITIVE_H // ARRAYPRIMITIVE_H...
#define ARRAYPRIMITIVE_H

#include "Primitives.h"
#include "Array.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// ArrayPrimitive Template Class
//

template <typename T> class ArrayPrimitive : public _ArrayBase {  // Template ArrayPrimitive<V>...

	// Special members...
	public: T _bogusReference; // Used as target when bad indexes are given. All we're
	                           // saying is indeterminate behaviour is better than GPFs.

  // C++ Lifecycle overrides
  public: ArrayPrimitive()  {StartUp();}
  public: ~ArrayPrimitive() {_ArrayBase::ShutDown();}
  public: T operator[](Int index) const {
    Status status;
    T      answer;
    status = Get(index, answer);
    if (status)
      BEFE::SetNull(answer);
    return answer;
  }

  // BEFE Lifecycle overrides
  public: Status StartUp() {
    Status status;
    status = _ArrayBase::StartUp();
    elmSize = sizeof(T);
    return status;
  }

  public: ArrayPrimitive<T> &Consumable() const {
    _ArrayBase::Consumable();
    return *(ArrayPrimitive<T> *)this;
  }

  public: ArrayPrimitive<T> &NotConsumable() const {
    _ArrayBase::NotConsumable();
    return *(ArrayPrimitive<T> *)this;
  }
  
  public: Boolean Contains(T const &that) const {
    return _ArrayBase::_Contains((ArrayCompareFunc)&ArrayPrimitive<T>::_CompareElements, (Byte *)&that);
  }
  
  public: Status Contains(T const &that, Int &answer) const {
    answer = _ArrayBase::_Contains((ArrayCompareFunc)&ArrayPrimitive<T>::_CompareElements, (Byte *)&that);
    return 0;
  }
  
  // Get Methods
  public: T       Get(Int index) const {
    Status status;
    T      local;
    status = _Get(index, (Byte *)&local);
    if (status)
      local = NULL;
    return local;
  }
  
  public: Status Get(Int index, T &that) const {
    return _Get(index, (Byte *)&that);
    }
    
  // Set/Append/Insert Methods
  public: Status Set(Int index, T that) {
    return _Set(index, (Byte *)&that);
  }

  public: T &operator[](Int index) {
  	T *that = (T *)_GetAddress(index);
  	if (!that)
  		that = &_bogusReference;
  	return *that;
  }

  public: Status Append(T that) {
    return _Append((Byte *)&that);
  }

  public: Status Insert(Int index, T that) {
    return _Insert(index, (Byte *)&that);
  }

  // Remove Methods
  public: Status Remove(Int index) {
    return _Remove(index);
    }
    
  public: Status Remove(Int index, UInt removeLen) {
    return _Remove(index, removeLen);
  }
  
  public: Status Remove(Span const &span) {
    return _Remove(span);
  }
  
  public: Status Remove(USpan const &span) {
    return _Remove(span);
  }
  
  public: Status Remove(Range const &range) {
    return _Remove(range);
  }
  
  public: Status Remove(URange const &range) {
    return _Remove(range);
  }
  
  public: Status Remove(Slice const &slice) {
    return _Remove(slice);
  }
  
  public: Status Sort() {
    return _Sort(ArrayPrimitive<T>::_CompareElements);
  }
  
  static Status _CompareElements(Byte *elm1, Byte *elm2, Int &answer) {
    Int ourAnswer;
    ourAnswer = 0;
    if (elm1 != NULL && elm2 != NULL) {
      if (*(T *)elm1 == *(T *)elm2 )
        ourAnswer = 0;
      else if (*(T *)elm1 <  *(T *)elm2 )
        ourAnswer = -1;
      else
        ourAnswer = 1;
    }
    answer = ourAnswer;
    return Error::None;
  }

}; // ...Template ArrayPrimitive<V>

//----------------------------------------------------------------------
//
// Commonly used ArrayPrimitive Template Uses (and Null/Empty functions)
//

// Shorts
typedef ArrayPrimitive<Short> Shorts;
BEFE_INLINE Boolean IsNull  (Shorts const &t) {return ((Shorts *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (Shorts &t)       {return ((Shorts *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (Shorts const &t) {return ((Shorts *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(Shorts &t)       {return ((Shorts *)&t)->SetEmpty();}

// UShorts
typedef ArrayPrimitive<UShort> UShorts;
BEFE_INLINE Boolean IsNull  (UShorts const &t) {return ((UShorts *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (UShorts &t)       {return ((UShorts *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (UShorts const &t) {return ((UShorts *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(UShorts &t)       {return ((UShorts *)&t)->SetEmpty();}

// Ints
typedef ArrayPrimitive<Int> Ints;
BEFE_INLINE Boolean IsNull  (Ints const &t) {return ((Ints *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (Ints &t)       {return ((Ints *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (Ints const &t) {return ((Ints *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(Ints &t)       {return ((Ints *)&t)->SetEmpty();}

// UInts
typedef ArrayPrimitive<UInt> UInts;
BEFE_INLINE Boolean IsNull  (UInts const &t) {return ((UInts *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (UInts &t)       {return ((UInts *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (UInts const &t) {return ((UInts *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(UInts &t)       {return ((UInts *)&t)->SetEmpty();}

// UInt32s
typedef ArrayPrimitive<UInt32> UInt32s;

// Chars
typedef ArrayPrimitive<Char> Chars;
BEFE_INLINE Boolean IsNull  (Chars const &t) {return ((Chars *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (Chars &t)       {return ((Chars *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (Chars const &t) {return ((Chars *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(Chars &t)       {return ((Chars *)&t)->SetEmpty();}

// Longs
typedef ArrayPrimitive<Long> Longs;
BEFE_INLINE Boolean IsNull  (Longs const &t) {return ((Longs *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (Longs &t)       {return ((Longs *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (Longs const &t) {return ((Longs *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(Longs &t)       {return ((Longs *)&t)->SetEmpty();}

// ULongs
typedef ArrayPrimitive<ULong> ULongs;
BEFE_INLINE Boolean IsNull  (ULongs const &t) {return ((ULongs *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (ULongs &t)       {return ((ULongs *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (ULongs const &t) {return ((ULongs *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(ULongs &t)       {return ((ULongs *)&t)->SetEmpty();}

// Floats
typedef ArrayPrimitive<Float> Floats;
BEFE_INLINE Boolean IsNull  (Floats const &t) {return ((Floats *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (Floats &t)       {return ((Floats *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (Floats const &t) {return ((Floats *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(Floats &t)       {return ((Floats *)&t)->SetEmpty();}

// Doubles
typedef ArrayPrimitive<Double> Doubles;
BEFE_INLINE Boolean IsNull  (Doubles const &t) {return ((Doubles *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (Doubles &t)       {return ((Doubles *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (Doubles const &t) {return ((Doubles *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(Doubles &t)       {return ((Doubles *)&t)->SetEmpty();}

// Ptrs
typedef ArrayPrimitive<Ptr> Ptrs;
//BEFE_INLINE Boolean IsEmpty (Ptr const &t) {return ((Ptr *)&t)->IsEmpty();}
//BEFE_INLINE Status  SetEmpty(Ptr &t)       {return ((Ptr *)&t)->SetEmpty();}

// Ranges
typedef ArrayPrimitive<Range> Ranges;
BEFE_INLINE Boolean IsNull  (Ranges const &t) {return ((Ranges *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (Ranges &t)       {return ((Ranges *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (Ranges const &t) {return ((Ranges *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(Ranges &t)       {return ((Ranges *)&t)->SetEmpty();}

// URanges
typedef ArrayPrimitive<URange> URanges;
BEFE_INLINE Boolean IsNull  (URanges const &t) {return ((URanges *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (URanges &t)       {return ((URanges *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (URanges const &t) {return ((URanges *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(URanges &t)       {return ((URanges *)&t)->SetEmpty();}

// Spans
typedef ArrayPrimitive<Span> Spans;
BEFE_INLINE Boolean IsNull  (Spans const &t) {return ((Spans *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (Spans &t)       {return ((Spans *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (Spans const &t) {return ((Spans *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(Spans &t)       {return ((Spans *)&t)->SetEmpty();}

// USpans
typedef ArrayPrimitive<USpan> USpans;
BEFE_INLINE Boolean IsNull  (USpans const &t) {return ((USpans *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (USpans &t)       {return ((USpans *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (USpans const &t) {return ((USpans *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(USpans &t)       {return ((USpans *)&t)->SetEmpty();}

// Slices
typedef ArrayPrimitive<Slice> Slices;
BEFE_INLINE Boolean IsNull  (Slices const &t) {return ((Slices *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (Slices &t)       {return ((Slices *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (Slices const &t) {return ((Slices *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(Slices &t)       {return ((Slices *)&t)->SetEmpty();}

} // ...Namespace BEFE

#endif // ...ARRAYPRIMITIVE_H
