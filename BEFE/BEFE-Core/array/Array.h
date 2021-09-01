//!befe-publish inc
//----------------------------------------------------------------------
// File: Array.h - Declarations for the Array base class
//----------------------------------------------------------------------
//
// The _ArrayBase class is an implementation intended to be used by
// Template Class specialisations.
//
// The ArrayPrimitive<T> template is for managing arrays of non-dynamic
// Values such as Ints, etc. that don't have a proper BEFE lifecycle.
//
// The ArrayValue<V> template is for managing array of dynamic Values
// such as Strings, etc. that do have a proper BEFE lifecycle.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef ARRAY_H // ARRAY_H...
#define ARRAY_H

#include "Primitives.h"
#include "Buffer.h"
#include "Alloc.h"
#include "Slice.h"
//#include "Id.h"             // For Iterator's Id

namespace BEFE { // Namespace BEFE...

// General Array Typedefs
typedef Status (*ArrayCompareFunc)(Byte *, Byte *, Int &);

//----------------------------------------------------------------------
//
// _ArrayBase Class
//
// Note: This is rarely used by itself and is intended to be used mainly
//       in the context of the Array Templates declared here and elsewhere
//

class _ArrayBase { // Class _ArrayBase...
  
  //
  // Public members
  //

  public: Boolean isLimited:1;  // "Is this a limited size?"
  public: Boolean isFixed:1;    // "Is this a fixed size?"
  public: Boolean isDynamic:1;  // "Are elements dynamic values?"
  public: Boolean isSorted:1;   // "Are elements sorted?"
  public: UInt    indexType:3;  // Sequence IndexType (see Sequence.h)
  public: UInt    _reserved:9;  // ◄── ***Reserved for future/sub-class use***
  //public: Id      _firstIter;   // ◄── ***Reserved for possible future use by Iterators***
  public: UInt16  elmSize;      // Size of each element (in bytes)
  public: Buffer  buffer;       // Buffer containing data

  //
  // Public instance methods
  //

  // C++ Lifecycle
  public: _ArrayBase();                          // Default constructor
  public: _ArrayBase(Alloc alloc);               // Alloc constructor
  public: _ArrayBase(_ArrayBase const &that);    // Copy constructor
  public: ~_ArrayBase();                         // Deconstructor
  
  // C++ "Handy Methods"
  public: _ArrayBase &operator=(_ArrayBase const &that);// Operator =

  // BEFE Lifecycle
  public: Status  StartUp();
  public: Status  ShutDown();
  public: Status  Reset();
  public: Boolean IsNull() const  { return buffer.IsNull();      }
  public: Status  SetNull()       { return Reset();              }  
  public: Boolean IsEmpty() const { return buffer.Length() == UNaN || buffer.Length() == 0; }
  public: Status  SetEmpty();
  public: Status  MoveFrom(_ArrayBase const &that);
  public: Status  CopyFrom(_ArrayBase const &that);
  //public: Status     &NewIterator(UInt iterType, Iterator *&newIter); // ◄── Not implemented yet
  public: Status  Consumable() const;           // "This IS Consumable"
  public: Status  NotConsumable() const;        // "This IS NOT Consumable"
  public: Boolean IsConsumable()const;          // "Is this Consumable?"

  public: UInt    Length() const;               // Get Length in Elements
  public: UInt    Size() const;                 // Get Size in Bytes
  public: UInt    ElementSize() const;          // Get Element Size (in Bytes)
  public: Status  Allocate(Alloc const &alloc);
  public: Status  SetLength(UInt newElms);
  public: Boolean _Contains(ArrayCompareFunc cmpFunc, Byte *that) const;
  public: Status  _Contains(ArrayCompareFunc cmpFunc, Byte *that, Boolean &answer) const;
  
  // Index resolving
  public: UInt    GetIndexType();
  public: UInt    SetIndexType(UInt type);
  public: UInt    ResolveIndex(Int value);
  public: UInt    ResolveIndex(UInt type, Int value);
  
  // High level Buffer Management
  public: Status  _Get(UInt index, Byte *buf) const;
  public: Status  _Get(UInt index, Byte *buf, UInt bufMaxLen, UInt &usedLen) const;
  public: Status  _Get(Int  index, Byte *buf) const;
  public: Status  _Get(Int  index, Byte *buf, UInt bufMaxLen, UInt &usedLen) const;
  public: Status  _Get(Range const &range, _ArrayBase &theArray) const;
  public: Status  _Get(Span  const &span,  _ArrayBase &theArray) const;
  public: Status  _Get(Slice const &slice, _ArrayBase &theArray) const;

  public: Status  _Set(UInt index, Byte *buf);
  public: Status  _Set(UInt index, Byte *buf, UInt bufLen);
  public: Status  _Set(Int  index, Byte *buf);
  public: Status  _Set(Int  index, Byte *buf, UInt bufLen);

  public: Status  _Append(Byte *buf);
  public: Status  _Append(Byte *buf, UInt bufLen);

  public: Status  _Insert(UInt index, Byte *buf);
  public: Status  _Insert(UInt index, Byte *buf, UInt bufLen);
  public: Status  _Insert(Int  index, Byte *buf);
  public: Status  _Insert(Int  index, Byte *buf, UInt bufLen);

  public: Status  _Remove(UInt index);
  public: Status  _Remove(UInt index, UInt removeLen);
  public: Status  _Remove(Int index);
  public: Status  _Remove(Int index, UInt removeLen);
  public: Status  _Remove(Span   const &span);
  public: Status  _Remove(USpan  const &span);
  public: Status  _Remove(Range  const &range);
  public: Status  _Remove(URange const &range);
  public: Status  _Remove(Slice  const &slice);

  // Elements as a whole
  public: Status  Reverse();                    // Reverse order of existing elements
  public: Status  _Sort(ArrayCompareFunc cmpFunc);   // Sort Elements
  
  // Low Level Buffer Mangement
  public: UInt    GetAllocElements();           // Get allocated bytes
  public: UInt    GetIncrementElements();       // Get byte increment count
  public: UInt    GetUsedElements();            // Get number of used bytes
  public: UInt    GetAllocBytes();              // Get allocated bytes
  public: UInt    GetIncrementBytes();          // Get byte increment count
  public: UInt    GetUsedBytes();               // Get number of used bytes
  public: Byte   *_GetBuf();                    // Get pointer to the bytes (transient)
  public: Byte   *_GetAddress(Int Index);       // Get address of specified index (transient)
  public: void    _BufAndLength(Byte *&buf, UInt &length);
  public: void    _BufAndSize(Byte *&buf, UInt &size);
  public: Status  _BufAlloc();          // Allocate Buffer if none, with default Alloc
  public: Status  _BufAlloc(Alloc def); // Allocate Buffer if none, with specified Alloc
  public: static Status _CompareElements(Byte *elm1, Byte *elm2, Int &answer);
  
}; // ...Class _ArrayBase

} // ...Namespace BEFE

#include "ArrayPrimitive.h"
#include "ArrayValue.h"

#endif // ...ARRAY_H
