//!befe-publish inc
//----------------------------------------------------------------------
// File: Sparse.h - Declarations for the Sparse Base class
//----------------------------------------------------------------------
//
// The _SparseBase is a base class for a Sparse Array-like
// implementations of various fixed sized elements, but not all indexes
// in the Sparse Array need be present.
//
// Built on the _SparseBase Class are the following Templates...
//
//   o SparsePrimitive<T> - For Sparse Arrays of C/C++ primitive values
//   o SparseValue<V>     - For Sparse Arrays of BEFE Lifecycle Classes
//
// At the end of this header file you will find several commonly used
// implementations of these Templates.
//
// ...
//
// Basically, _SparseBase is indexed exactly like an Array but the
// indexes mean something slightly different...
//
//   o Given index N, it may or may not be present
//
//   o Negative indexes don't mean anything in a Sparse, so indexes
//       are Uints unlike Array which uses Ints
//
//   o Length indicates the number of present entries in the Sparse
//       array, unlike Array where it indicates the highest index + 1
//
//   o Length zero indicates the Sparse is "empty" or devoid of
//       elements
//
//   o Slicing DOES NOT make sense at the top Index Level in a Sparse
//       because "step" isn't meaningful or, at least, useful.  Instead,
//       Sparse implements "USpan" at the index level, meaning it's an
//       inclusive unsigned Span unlike a Slice, which is exclusive
//       signed range with a step
//
//   o ...more to come here, perhaps...
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef SPARSE_H // SPARSE_H...
#define SPARSE_H

#include "Primitives.h"
#include "USpan.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// _SparseBase Class
//
// This is the Base class used for the SparsePrimitive<T> and 
// SparseValue<T> templates below.
//

class _SparseBase { // Class _SparseBase...

  // Friend Classes/templates
  friend class _MapBase;
  template <typename T> friend class UIntMapPrimitive;
  template <typename V> friend class UIntMapValue;
  template <typename T> friend class StringMapPrimitive;
  template <typename V> friend class StringMapValue;
  
  //
  // Public Instance methods
  //

  // C++ Lifecycle methods
  public: _SparseBase();                        // Default constructor
  public: _SparseBase(_SparseBase const &that); // Copy constructor
  public: ~_SparseBase();                       // Deconstructor
  public: _SparseBase &operator=(_SparseBase const &that);
  
  // BEFE Lifecycle methods
  public: Status      StartUp();
  public: Status      ShutDown();
  public: Status      Reset();
  public: Boolean     IsNull()  const { return buffer.IsNull();      };
  public: Status      SetNull()       { return Reset();              };  
  public: Boolean     IsEmpty() const { return buffer.Length() == 0; };
  public: Status      SetEmpty();
  public: Status      MoveFrom(_SparseBase const &that);
  public: Status      CopyFrom(_SparseBase const &that);

  public: _SparseBase &Consumable() const;
  public: _SparseBase &NotConsumable() const;
  public: Boolean     IsConsumable() const;
  public: Status      SetLength(UInt newElms) const;
  public: Boolean     IsMutable() const;
  public: Status      Mutable();
  public: Status      Immutable();
  public: Boolean     _Contains(Byte *that) const;
  public: Status      _Contains(Byte *that, Boolean &answer) const;
  
  // Contents Info Methods
  public: UInt        Length() const;                 // ◄── Number of Elements (at both levels)
  public: UInt        Length(UInt index) const;       // ◄── Number of entries (at given Index)
  public: UInt        DistinctLength() const;         // ◄── Number of distinct index entries
  public: UInt        Size() const;                   // ◄── Number of Storage Units used
  public: UInt        ElementSize() const;            // ◄── Storage Units per Element
  public: UInt        UnitSize() const;               // ◄── Size of Storage Unit in bits (see StorageUnit)
  public: USpan       IndexSpan() const;              // ◄── Span of valid Index values
  public: USpan       SubIndexSpan(UInt index) const; // ◄── Span of valid SubIndex values
  public: Boolean     Exists(UInt index) const;       // ◄── "Does Index N exist?"

  // Abstract Element Methods
  public: Status      _Get(UInt index, void *buf) const;
  public: Status      _Get(UInt index, Int subindex, void *buf) const;
  public: Status      _Set(PtrInt index, void *buf);
  public: Status      _Set(PtrInt index, Int subindex, void *buf);
  public: Status      _Append(void *buf);
  public: Status      _Append(UInt index, void *buf);
  public: Status      _Remove(UInt index);
  public: Status      _Remove(UInt index, Int subindex);

  // Physical Element Methods
  public: Status      _GetPhysicalElement(UInt &index, void **buf) const;

  //
  // Protected methods
  //
  
  // Allocate Buffer if not already allocated
  protected: Status AllocateBuffer();
  
  // Get range of valid Array style indexes
  protected: USpan  LogicalIndexSpan() const;       // ◄── Logical Index Span of all elements
  protected: USpan  PhysicalIndexSpan() const;      // ◄── Physical index Span of all elements
  protected: USpan  PhysicalIndexSpan(UInt i) const;// ◄── Physical index Span of given Index

  // Given an index (which may be Null) and subindex (which may be Null),
  //   Resolve where in the Array to insert/append it
  protected: Status ResolveInsert(UInt i, UInt s, UInt &resolved) const;

  // Given an index and subindex (which may be NaN),
  //   Resolve where in the Array to get/remove it from
  protected: Status ResolveGet(UInt i, UInt s, UInt &resolved) const;
  
  //
  // Protected Instance members and methods
  //
  
  // Members
  protected: Boolean isDistinct:1;    // "Are distinct indexes required?"
  protected: Boolean isMap:1;         // ◄── Used by MapBase Class
  protected: Boolean isInsensitive:1; // ◄── Used by MapBase Class
  protected: Boolean isMutable:1;     // ◄── Used by MapBase Class
  protected: UInt    _reserved:4;     // ***Reserved for future use***
  protected: UInt8   mapKeyType;      // ◄── Used by MapBase Class
  protected: UInt16  elmSize;         // Element size (excluding header)
  protected: PtrInt  _firstIter;      // ***Reserved for possible future use by Iterators***
  public:    Buffer  buffer;          // ◄── This is where we store everything
  
  // Note: The following are only for use by Sparse/Map templates and
  //       friend Classes...
  //
  // Note: with the public methods that return "Byte const *", you
  //       shouldn't modify the contents unless you REALLY know
  //       what you're doing.  In fact, you shouldn't really be
  //       calling them unless you're in the BEFE team.  ;-)
  protected: void        _BufAndSize(Byte *&buf, UInt &len) const;
  protected: Status      _BufAlloc();          // Allocate Buffer if none, with default Alloc
  protected: Status      _BufAlloc(Alloc def); // Allocate Buffer if none, with specified Alloc
  public:    Byte const *_GetPhyAddress (UInt phyIndex) const;
  protected: UInt        _GetPhyElementSize() const;
  protected: UInt        _GetPhyHeaderSize() const;
  public:    Byte const *_GetLogAddress (UInt logIndex) const;
  public:    Byte const *_GetLogAddress (UInt logIndex, Int subIndex) const;
  protected: Status      _PhyInsert(UInt phyIndex, UInt logIndex, Byte *&phyElm);
  protected: Status      _PhyRemove(UInt phyIndex);
  
}; // ...Class _SparseBase

//----------------------------------------------------------------------
//
// General _SparseBase Functions...
//

Boolean IsNull(_SparseBase const &v);

//----------------------------------------------------------------------
//
// SparsePrimitive Template Class
//

template <typename T> class SparsePrimitive : public _SparseBase { // Template SparsePrimitive<T>...

  // C++ Lifecycle overrides
  public: SparsePrimitive()  {StartUp();}
  public: ~SparsePrimitive() {_SparseBase::ShutDown();}
  
  // BEFE Lifecycle overrides
  public: Status StartUp() {
    Status status;
    status = _SparseBase::StartUp();
    elmSize = sizeof(T);
    return status;
  }

  public: SparsePrimitive<T> &Consumable() const {
    _SparseBase::Consumable();
    return *(SparsePrimitive<T> *)this;
  }

  public: SparsePrimitive<T> &NotConsumable() const {
    _SparseBase::NotConsumable();
    return *(SparsePrimitive<T> *)this;
  }
  
  public: UInt Find(T const that,UInt startOffset) const {
   
    USpan span;
    UInt  idx;
    
    span = LogicalIndexSpan();
    for (idx = startOffset; idx < span.idx2; idx++)
      if (((T *)buffer)[idx] == that)
        break;
    if (idx >= span.idx2)
      return UIntNaN;
    return idx;

  }

  public: UInt Find(T that) {
    return Find(that,0);
  }

  public: Boolean Contains(T const &that) const {
    return _Contains((Byte *)&that);
  }
  
  // Get Methods
  public: T Get(UInt index) const {
    Status status;
    T      local;
    status = _Get(index, (Byte *)&local);
    if (status)
      BEFE::SetNull(local);
    return local;
  }

  public: Status Get(UInt index, T *that) const {
    return _Get(index, (Byte *)&that);
    }
    
  // Set/Append/Insert Methods
  public: Status Set(UInt index, T that) {
    return _Set(index, (Byte *)&that);
  }

  public: Status Set(UInt index, Int subindex, T that) {
    return _Set(index, subindex, (Byte *)&that);
  }
  
  public: Status Append(T const &that) {
    return _Append((Byte *)&that);
  }

  public: Status Append(UInt index, T const &that) {
    return _Append(index, (Byte *)&that);
  }

  public: Status Remove(PtrInt index) {
    return _Remove(index);
  }
  
  public: Status Remove(UInt index, Int subindex) {
    return _Remove(index, subindex);
  }
  
  // Physical Element Methods
  public: Status GetPhysical(UInt &index, T *that) const {
    return _GetPhysicalElement(index, (void **)that);
  }

}; // ...Template SparsePrimitive<T>

//----------------------------------------------------------------------
//
// SparseValue Template Class
//

template <typename V> class SparseValue : public _SparseBase { // Template SparseValue<V>...

  // C++ Lifecycle overrides
  public: SparseValue()  {StartUp();}

  public: ~SparseValue() {ShutDown();}
  
  // BEFE Lifecycle overrides
  public: Status StartUp() {
    Status status;
    status = _SparseBase::StartUp();
    elmSize = sizeof(V);
    return status;
  }

  public: Status ShutDown() {
 
    Status retStatus;
    Status status;
 
    retStatus = _ResetElements();
    status = _SparseBase::ShutDown();
    if (status && !retStatus) retStatus = status;

    return retStatus;
 
  }
    
  public: Status Reset() {
    
    Status retStatus;
    Status status;
    UInt   savElmSize;
    
    savElmSize = elmSize;
    retStatus = _ResetElements();
    status = ShutDown();
    if (status && !retStatus) retStatus = status;
    status = StartUp();
    if (status && !retStatus) retStatus = status;    
    elmSize = savElmSize;
    
    return retStatus;
    
  }

  public: Status MoveFrom(SparseValue<V> const &that) {

    Status retStatus;
    Status status;

    retStatus = Reset();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(SparseValue<V>));
    status = ((V *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;

    return retStatus;

  }
  
  public: Status CopyFrom(SparseValue<V> const &that) {
    
    Status status;
    UInt   numElms;
    PtrInt curIdx;
    V     *thisElm;
    V     *thatElm;
    
    if (that.IsConsumable())
      status = MoveFrom(that);
    else {
      status = _SparseBase::CopyFrom(that);
      if (status) goto SOMEERROR;
      numElms = Length();
      for (curIdx=0; curIdx < numElms; curIdx++) {
        thisElm = (V *)_GetPhyAddress(curIdx);
        if (thisElm != NULL)
          thisElm->StartUp();
      }
      for (curIdx=0; curIdx < numElms; curIdx++) {
        thisElm = (V *)_GetPhyAddress(curIdx);
        thatElm = (V *)((SparseValue<V> *)&that)->_GetPhyAddress(curIdx);
        if (thisElm != NULL && thatElm != NULL)
          thisElm->CopyFrom(*thatElm);
      }
    }
    
    // Handle errors
    while (false) {
      SOMEERROR: break;
    }
    
    return status;

  }

  public: SparseValue<V> &Consumable() const {
    _SparseBase::Consumable();
    return *(SparseValue<V> *)this;
  }

  public: SparseValue<V> &NotConsumable() const {
    _SparseBase::NotConsumable();
    return *(SparseValue<V> *)this;
  }
  
  public: Boolean Contains(V const &that) const {
  
    Boolean  answer;
    Byte    *buf;
    UInt     bufSize;
    UInt     phyElmSize;
    UInt     phyHdrSize;
    UInt     numElms;
    PtrInt   curIdx;
  
    answer = false;
    ((SparseValue<V> *)this)->_BufAndSize(buf, bufSize);
    phyElmSize = ((SparseValue<V> *)this)->_GetPhyElementSize();
    phyHdrSize = ((SparseValue<V> *)this)->_GetPhyHeaderSize();
    if (buf && bufSize && phyElmSize) {
      numElms = bufSize/phyElmSize;
      for (curIdx=0; curIdx < numElms; curIdx++) {
        if (that.Compare(*((V *)(buf+curIdx*phyElmSize+phyHdrSize))) == 0) break;
      }
      answer = curIdx < numElms;
    }
    
    return answer;
  
  }

  // Get Methods
  public: V Get(UInt index) const {
    
    //Status status;
    V     *valPtr;
    V      local;

    valPtr = (V *)_GetLogAddress(index);
    if (valPtr)
      local = *valPtr;
      
    return local.Consumable();

  }

  public: Status Get(UInt index, V &that) const {

    Status  retStatus;
    Status  status;
    V      *valPtr;

    retStatus = that.Reset();
    valPtr = (V *)_GetLogAddress(index);
    if (valPtr) {
      status = valPtr->CopyFrom(*valPtr);
      if (status && !retStatus) retStatus = status;
    }
    else
      if (!retStatus) retStatus = Error::IndexNotFound;
      
    return retStatus;

  }
    
  // Set/Append/Insert Methods
  public: Status Set(UInt index, V const &that) {
    
    Status  status;
    V      *element;
    
    element = (V *)_GetLogAddress(index);
    if (element == NULL)
      status = Append(index, that);
    else
      status = element->CopyFrom(that);
    
    return status;
    
  }

  public: Status Set(UInt index, Int subIndex, V const &that) {
    
    Status  status;
    V      *element;
    
    element = (V *)_GetLogAddress(index, subIndex);
    if (element == NULL)
      status = Append(index, subIndex, that);
    else
      status = element->CopyFrom(that);
    
    return status;
    
  }
  
  public: Status Append(V const &that) {
    
    Status status;
    V      local;
    
    local = that;
    status = _Append((Byte *)&local);
    if (!status)
      local.StartUp();
      
    return status;
    
  }

  public: Status Append(UInt index, V const &that) {

    Status status;
    V      local;
    
    local = that;
    status = _Append(index, (Byte *)&local);
    if (!status)
      local.StartUp();
      
    return status;
    
  }

  public: Status Remove(UInt index) {
    return _Remove(index);
  }
  
  public: Status Remove(UInt index, UInt subindex) {
    return _Remove(index, subindex);
  }
  
  // Physical Element Methods
  public: V *GetPhysical(UInt &index) const {
    V *that;
    that = (V *)_GetPhyAddress(index);
    return that;
  }

  //
  // Private maintenance metnods
  //
  
  protected: Status _ResetElements() {
    
    Status  retStatus;
    Status  status;
    UInt    numElms;
    PtrInt  curIdx;
    V      *curElm;
    
    retStatus = Error::None;
    numElms = Length();
    for (curIdx=0; curIdx < numElms; curIdx++) {
      curElm = (V *)_GetPhyAddress(curIdx);
      if (curElm) {
        status = curElm->Reset();
        if (status && !retStatus) retStatus = status;
      }
    }
    
    status = _SparseBase::Reset();
    if (status && !retStatus) retStatus = status;
    
    return retStatus;

  }
  
}; // ...Template SparseValue<V>

//----------------------------------------------------------------------
//
// Commonly used Sparse Template Uses
//

typedef SparsePrimitive<Short>  SparseShorts;
typedef SparsePrimitive<UShort> SparseUShorts;
typedef SparsePrimitive<Int>    SparseInts;
typedef SparsePrimitive<UInt>   SparseUInts;
typedef SparsePrimitive<Char>   SparseChars;
typedef SparsePrimitive<Long>   SparseLongs;
typedef SparsePrimitive<ULong>  SparseULongs;

typedef SparseValue<String>     SparseStrings;

} // ...Namespace BEFE

#endif // ...SPARSE_H
