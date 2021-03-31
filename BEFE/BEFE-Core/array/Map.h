//!befe-publish inc
//----------------------------------------------------------------------
// File: Map.h - Declarations for the _MapBase class
//----------------------------------------------------------------------
//
// The _MapBase Class is a base class for Map-like implementations of
// value->value mappings.
//
// Built on the _MapBase Class are the following templates...
//
//   o UIntMapPrimitive<T>   - UInt indexed Map of Primitives
//   o UIntMapValue<V>       - UInt indexed Map of Values
//   o StringMapPrimitive<T> - String indexed Map of Primitives
//   o StringMapValue<V>     - String indexed Map of Values
//
// Basically, a Map is indexed exactly like a Sparse Array BUT the
// StringMap Templates use a String as the "index" instead of an Int.
//
//   Note: Actually, the Templates use the String's Hash (CRC) for an
//         index into the underlying Sparse Array.
//
// Even though the UIntMap Template Instances are pretty much equivalent
// to a Sparse Template instance, they are provided here so they can
// be through of as a "Map" instead of a "Sparse Array".
//
// As such, the indexes are forced to be Distinct, whereas a Sparse
// Array doesn't enforce such a restriction by default.
//
// Basically, an UIntMap uses an Int as the Map Index whereas a StringMap
// uses a String as an index.
//
// As mentioned above, the StringMap templates do a little trickery by
// making the underlying SparseArray "Element" actually be a
// concatenation of a String and whatever <T> or <V> is specified in the
// associated template.
//
// This means that StringMap instances are implemented such that
// the String's Hash is the Index but since a Hash, by definition, is
// not guaranteed to be unique.  So, we take full advantage of
// SparseBase's cool ability to have multiple values at the same index
// and enforce Distinctness on the String value itself NOT its Hash value.
//
// But, enough of that and "On With The Show"...
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef MAP_H  // MAP_H...
#define MAP_H

#include "Primitives.h"
#include "Null.h"
#include "LifeCycle.h"
#include "Sparse.h"

namespace BEFE {

//----------------------------------------------------------------------
//
// Namespace MapKeyType - For enumerating Map Key Types
//

namespace Map { // Namespace Map...

  enum { // Key Types...
     KeyTypeInt
    ,KeyTypeString
  }; // ... Key Types
  
} // ...Namespace Map

//----------------------------------------------------------------------
//
// _MapBase Class
//
// This is the Base class used for the UIntMap and StringMap templates
// declared after it.
//

class _MapBase { // _MapBase Class...

  // C++ Lifecycle methods
  public: _MapBase();                           // Default constructor
  public: _MapBase(_MapBase const &that);       // Copy constructor
  public: ~_MapBase();                          // Deconstructor
  public: _MapBase &operator=(_MapBase const &that);
  
  // BEFE Lifecycle methods
  public: Status    StartUp();
  public: Status    ShutDown();
  public: Status    Reset();
  public: Boolean   IsNull()  const { return sparse.IsNull();      };
  public: Status    SetNull()       { return sparse.Reset();       };  
  public: Boolean   IsEmpty() const { return sparse.Length() == 0; };
  public: Status    SetEmpty()      { return sparse.SetEmpty();    };
  public: Status    MoveFrom(_MapBase const &that);
  public: Status    CopyFrom(_MapBase const &that);
  //public: Status    &NewIterator(UInt iterType, Iterator *&newIter); // ◄── Not implemented yet
  public: _MapBase &Consumable() const;
  public: _MapBase &NotConsumable() const;
  public: Boolean   IsConsumable() const;
  public: Status    SetLength(UInt newElms) const;
  public: Boolean   IsMutable() const;
  public: Status    Mutable();
  public: Status    Immutable();
  public: Boolean   _Contains(Byte *that) const;
  public: Status    _Contains(Byte *that, Boolean &answer) const;
  
  // Contents Info Methods
  public: UInt     Length() const;                // ◄── Number of Elements (at both levels)
  public: UInt     Size() const;                  // ◄── Number of Storage Units used
  public: UInt     ElementSize() const;           // ◄── Storage Units per Element
  public: UInt     UnitSize() const;              // ◄── Size of Storage Unit in bits (see StorageUnit)
  public: Status   _FindKey(UInt index, UInt &phyIndex) const;        // ◄── "Does Int Index exist?"
  public: Status   _FindKey(String const &key, UInt &phyIndex) const; // ◄── "Does Striong Index exist?"

  // Keys...
  public: Boolean  IsSensitive() const;           // "Is this StringMap case sensitive?"
  public: Status   SetSensitivity(Boolean sense); // Set StringMap key case sensitivity
  public: Status   SetSensitive();                // Set StringMap key search to "case sensitive"
  public: Status   SetInsensitive();              // Set Stringmap key search to "case insensitive"
  public: Status   Keys(UInts &keys) const;
  public: Status   Keys(Strings &keys) const;
  public: Boolean  HasKey(UInt index) const;
  public: Boolean  HasKey(String const &key) const;
  
  //
  // Protected (mostly) Instance members
  //
  // Note: In the public methods that return "Byte const *", you
  //       shouldn't modify the contents unless you REALLY know
  //       what you're doing.  In fact, you shouldn't really be
  //       calling them unless you're in the BEFE team.  ;-)
  
  // Members
  public:    _SparseBase sparse;      // The underlying Sparse implementation 
  //protected: Id         _firstIter;   // ◄── ***Reserved for possible future use by Iterators***
  
  // Methods
  protected: void        _SetMapInfo(UInt keyType, UInt elmSize, Boolean isDistinct, Boolean isInsensitive);
  public:    Byte const *_GetPhyAddress(UInt index) const;        // ◄── Don't modify contents!!!
  public:    Byte const *_GetPhyAddress(String const &key) const; // ◄── Don't modify contents!!!
  public:    Byte const *_GetLogAddress(UInt index) const;        // ◄── Don't modify contents!!!
  protected: UInt        _GetElementSize() const;
  protected: UInt        _GetKeySize() const;
  protected: Boolean     _IsInsensitive() const;
  protected: Status      _GetKeySpan(String const &key, USpan &span) const;
  protected: Status      _FindOrCreatePhyValueSpace(String const &key, Byte *&value, Boolean &created);
  protected: Status      _Remove(String const &key);
  protected: Status      _RemovePhyIndex(UInt phyIndex);
  protected: Status      _ResetKeys();
  
}; // ..._MapBase Class

//----------------------------------------------------------------------
//
// General _MapBase Functions...
//

Boolean IsNull(_MapBase const &v);

//----------------------------------------------------------------------
//
// UIntMapPrimitive<T> Template Class
//
// Note: This is essentially the same as SparsePrimitive except that
//       it's presented as a Map so it's wrapped differently
//

template <typename T> class UIntMapPrimitive : public _MapBase { // UIntMapPrimitive<T> Template...

  // C++ Lifecycle overrides
  public: UIntMapPrimitive()  {StartUp();}
  public: ~UIntMapPrimitive() {_MapBase::ShutDown();}
  
  // BEFE Lifecycle overrides
  public: Status StartUp() {
    Status status;
    status = _MapBase::StartUp();
    _SetMapInfo(Map::KeyTypeInt, sizeof(T), true, false);
    
    return status;
  }

  public: UIntMapPrimitive<T> &Consumable() const {
    sparse.Consumable();
    return *(UIntMapPrimitive<T> *)this;
  }

  public: UIntMapPrimitive<T> &NotConsumable() const {
    sparse.NotConsumable();
    return *(UIntMapPrimitive<T> *)this;
  }
  
  public: Boolean Contains(T const &that) const {
    return _Contains((Byte *)&that);
  }
  
  // Map Template Specific Methods
  public: Boolean Exists(UInt index) {
    
    Status  status;
    UInt    phyIndex;
    Boolean answer;
    
    status = _FindKey(index, phyIndex);
    answer = !(status || BEFE::IsNull(phyIndex));
    
    return answer;
    
  }
  
  public: Status Keys(UInts &keys) const {
    return _MapBase::Keys(keys);
  }
  
  public: UInts Keys() const {
    
    //Status status;
    UInts   keys;
    
    /*status =*/ Keys(keys);
    
    return keys.Consumable();
    
  }

  // Note: This one's a bit dodgy because it doesn't/can't initialise local  
  public: T Get(UInt index) const {
    Status status;
    T      local;
    status = sparse._Get(index, (Byte *)&local);
    if (status)
      BEFE::SetNull(local);
    return local;
  }

  public: Status Get(UInt index, T &that) const {
    return sparse._Get(index, (Byte *)&that);
    }
      
  public: Status Set(UInt index, T const &value) {
    
    Status status;
    UInt   phyIndex;
    
    status = _FindKey(index, phyIndex);
    if (status && status != Error::IndexNotFound) goto SOMEERROR;
    if (status == Error::IndexNotFound)
      status = sparse._Append(index, (Byte *)&value);
    else
      status = sparse._Set(index, (Byte *)&value);
    
    // Handle errors
    while (false) {
      SOMEERROR: break;
    }
    
    return status;
    
  }

  public: Status Remove(UInt index) {
    return sparse._Remove(index);
  }

  public: Status Remove(UInt index, Int subindex) {
    return sparse._Remove(index);
  }
    
}; // ...UIntMapPrimitive<T> Template

//----------------------------------------------------------------------
//
// UIntMapValue<V> Template Class
//
// Note: This is essentially the same as SparseValue except that
//       it's presented as a Map so it's wrapped differently
//

template <typename V> class UIntMapValue : public _MapBase { // UIntMapValue<V> Template...

  // C++ Lifecycle overrides
  public: UIntMapValue()  {StartUp();}
  public: UIntMapValue(UIntMapValue const &that) {StartUp(); CopyFrom(that);}
  public: ~UIntMapValue() {ShutDown();}
  public: UIntMapValue &operator=(UIntMapValue const &that) {Reset(); CopyFrom(that); return *this;}  

  // BEFE Lifecycle overrides
  public: Status StartUp() {
    Status status;
    status = _MapBase::StartUp();
    _SetMapInfo(Map::KeyTypeInt, sizeof(V), true, false);
    
    return status;
  }

  public: Status ShutDown() {
 
    Status retStatus;
    Status status;
 
    retStatus = _ResetElements();
    status = _MapBase::ShutDown();
    if (status && !retStatus) retStatus = status;

    return retStatus;
 
  }
    
  public: Status Reset() {
    
    Status retStatus;
    Status status;
    
    retStatus = _ResetElements();
    status = sparse.ShutDown();
    if (status && !retStatus) retStatus = status;
    status = sparse.StartUp();
    if (status && !retStatus) retStatus = status;    
    _SetMapInfo(Map::KeyTypeInt, sizeof(V), true, false);
    
    return retStatus;
    
  }

  public: Status CopyFrom(UIntMapValue<V> const &that) {
    
    Status status;
    UInt   numElms;
    UInt   curIdx;
    V     *thisElm;
    V     *thatElm;
    
    if (that.IsConsumable())
      status = _MapBase::MoveFrom(that);
    else {
      status = _MapBase::CopyFrom(that);
      if (status) goto SOMEERROR;
      numElms = Length();
      for (curIdx=0; curIdx < numElms; curIdx++) {
        thisElm = (V *)_GetPhyAddress(curIdx);
        if (thisElm != NULL)
          thisElm->StartUp();
      }
      for (curIdx=0; curIdx < numElms; curIdx++) {
        thisElm = (V *)_GetPhyAddress(curIdx);
        thatElm = (V *)((UIntMapValue<V> *)&that)->_GetPhyAddress(curIdx);
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

  public: UIntMapValue<V> &Consumable() const {
    sparse.Consumable();
    return *(UIntMapValue<V> *)this;
  }

  public: UIntMapValue<V> &NotConsumable() const {
    sparse.NotConsumable();
    return *(UIntMapValue<V> *)this;
  }

  public: Boolean Contains(V const &that) const {
  
    Boolean  answer;
    Byte    *buf;
    UInt     bufSize;
    UInt     phyElmSize;
    UInt     phyHdrSize;
    UInt     numElms;
    UInt     curIdx;
  
    answer = false;
    ((UIntMapValue<V> *)this)->sparse._BufAndSize(buf, bufSize);
    phyElmSize = ((UIntMapValue<V> *)this)->sparse._GetPhyElementSize();
    phyHdrSize = ((UIntMapValue<V> *)this)->sparse._GetPhyHeaderSize();
    if (buf && bufSize && phyElmSize) {
      numElms = bufSize/phyElmSize;
      for (curIdx=0; curIdx < numElms; curIdx++) {
        if (that.Compare(*((V *)(buf+curIdx*phyElmSize+phyHdrSize))) == 0) break;
      }
      answer = curIdx < numElms;
    }
    
    return answer;
  
  }

  // UIntMap specific methods...
  public: Boolean Exists(UInt index) {
    
    Status  status;
    UInt    phyIndex;
    Boolean answer;
    
    status = _FindKey(index, phyIndex);
    answer = !(status || BEFE::IsNull(phyIndex));
    
    return answer;
    
  }
  
  public: Status Keys(UInts &keys) const {
    return _MapBase::Keys(keys);
  }
  
  public: UInts Keys() const {
    
    //Status status;
    UInts   keys;
    
    /*status =*/ Keys(keys);
    
    return keys.Consumable();
    
  }
  
  public: V Get(UInt index) const {
    
    Status status;
    V     *valPtr;
    V      local;

    valPtr = (V *)sparse._GetLogAddress(index);
    if (valPtr)
      local = *valPtr;
      
    return local.Consumable();

  }

  public: Status Get(UInt index, V &that) const {

    Status  retStatus;
    Status  status;
    V      *valPtr;
    V       local;

    retStatus = that.Reset();
    valPtr = (V *)_GetLogAddress(index);
    if (valPtr) {
      status = local.CopyFrom(*valPtr);
      if (status && !retStatus) retStatus = status;
      that = local.Consumable();
      status = local.StartUp();
      if (status && !retStatus) retStatus = status;
    }
    else
      if (!retStatus) retStatus = Error::IndexNotFound;
      
    return retStatus;

  }
    
  public: Status Set(UInt index, V const &that) {
    
    Status status;
    UInt   phyIndex;
    V      local;
    V     *element;
    
    status = _FindKey(index, phyIndex);
    if (status && status != Error::IndexNotFound) goto SOMEERROR;
    if (status == Error::IndexNotFound) {
      status = sparse._Append(index, (Byte *)&local);
      if (status) goto SOMEERROR;
      status = _FindKey(index, phyIndex);
      if (status) goto SOMEERROR;
    }
 
    element = (V *)_GetPhyAddress(phyIndex);
    if (element == NULL) goto NULLPOINTER;
    local = that;
    MemoryCopyRaw((Byte *)&local, (Byte *)element, sizeof(V));
    status = local.StartUp();

    // Handle errors
    while (false) {
      SOMEERROR:                                        break;
      NULLPOINTER: status = Error::InternalNullPointer; break;
    }
    
    return status;
    
  }

  public: Status Remove(UInt index) {

    Status  status;
    UInt    phyIndex;
    V      *element;
    
    status = _FindKey(index, phyIndex);
    if (status) goto SOMEERROR;
    element = (V *)_GetPhyAddress(phyIndex);
    if (element == NULL) goto NULLPOINTER;
    status = element->ShutDown();
    if (status) goto SOMEERROR;
    status = sparse._Remove(index);
    
    // Handle errors
    while (false) {
      SOMEERROR:                                        break;
      NULLPOINTER: status = Error::InternalNullPointer; break;
    }
    
    return status;
    
  }
  
  //
  // Private maintenance metnods
  //
  
  protected: Status _ResetElements() {
    
    Status retStatus;
    Status status;
    UInt   numElms;
    UInt   curIdx;
    V     *curElm;
    
    retStatus = Error::None;
    numElms = Length();
    for (curIdx=0; curIdx < numElms; curIdx++) {
      curElm = (V *)_GetPhyAddress(curIdx);
      if (curElm) {
        status = curElm->Reset();
        if (status && !retStatus) retStatus = status;
      }
    }
    
    status = _MapBase::Reset();
    if (status && !retStatus) retStatus = status;
    
    return retStatus;

  }
  
}; // ...UIntMapValue<V> Template

//----------------------------------------------------------------------
//
// StringMapPrimitive<T> Template Class
//

template <typename T> class StringMapPrimitive : public _MapBase { // StringMapPrimitive<T> Template...

  // C++ Lifecycle overrides
  public: StringMapPrimitive()  {StartUp();}
  public: ~StringMapPrimitive() {ShutDown();}
  
  // BEFE Lifecycle overrides
  public: Status StartUp() {
    Status status;
    status = _MapBase::StartUp();
    _SetMapInfo(Map::KeyTypeString, sizeof(String)+sizeof(T), true, false);
    
    return status;
  }

  public: Status ShutDown() {
    Status status;
    status = _MapBase::ShutDown();
    return status;
  }
    
  public: StringMapPrimitive<T> &Consumable() const {
    sparse.Consumable();
    return *(StringMapPrimitive<T> *)this;
  }

  public: StringMapPrimitive<T> &NotConsumable() const {
    sparse.NotConsumable();
    return *(StringMapPrimitive<T> *)this;
  }
  
  public: Boolean Contains(T const &that) const {
  
    Boolean  answer;
    Byte    *buf;
    UInt     bufSize;
    UInt     phyElmSize;
    UInt     phyHdrSize;
    UInt     numElms;
    UInt     curIdx;
  
    answer = false;
    ((StringMapPrimitive<T> *)this)->sparse._BufAndSize(buf, bufSize);
    phyElmSize = ((StringMapPrimitive<T> *)this)->sparse._GetPhyElementSize();
    phyHdrSize = ((StringMapPrimitive<T> *)this)->sparse._GetPhyHeaderSize() + sizeof(String);
    if (buf && bufSize && phyElmSize) {
      numElms = bufSize/phyElmSize;
      for (curIdx=0; curIdx < numElms; curIdx++) {
        if (that == *((T *)(buf+curIdx*phyElmSize+phyHdrSize))) break;
      }
      answer = curIdx < numElms;
    }
    
    return answer;
  
  }

  // Map Template Specific Methods  
  public: Boolean Exists(UInt key) {
    return Error::MapNotStringIndexed;
  }
  
  public: Boolean Exists(String const &key) {
    
    //Status   status;
    Byte    *phyKey;
    Boolean  answer;
    
    phyKey = (Byte *)_GetPhyAddress(key);
    answer = (phyKey != NULL);

    return answer;
    
  }
  
  public: Status Keys(Strings &keys) const {
    return _MapBase::Keys(keys);
  }
  
  public: Strings Keys() const {
    
    //Status  status;
    Strings keys;
    
    /*status =*/ Keys(keys);
    
    return keys.Consumable();
    
  }
  
  public: T Get(UInt index) const {
    return Error::MapNotIntIndexed;
  }
  
  public: T Get(String const &key) const {

    Byte *phyKey;
    T     local;
    
    phyKey = (Byte *)_GetPhyAddress(key);
    if (phyKey)
      local = *((T *)(phyKey + sizeof(String)));
    //else
    //  BEFE::SetNull(&local);
      
    return local;

  }

  public: Status Get(UInt index, T &that) const {
    return Error::MapNotIntIndexed;
  }
  
  public: Status Get(String const &key, T &that) const {

    Status  status;
    Byte   *phyKey;
    
    phyKey = (Byte *)_GetPhyAddress(key);
    if (phyKey) {
      that = *((T *)(phyKey + sizeof(String)));
      status = Error::None;
    }
    else
      status = Error::IndexNotFound;
      
    return status;

  }

  public: Status Set(UInt index, T const &value) const {
    return Error::MapNotIntIndexed;
  }
  
  public: Status Set(String const &key, T const &value) {
    
    Status   status;
    Byte    *phyValue;
    Boolean  created;

    status = _FindOrCreatePhyValueSpace(key, phyValue, created);
    if (status) goto SOMEERROR;
    *(T *)phyValue = value;
    
    // Handle errors
    while (false) {
      SOMEERROR: break;
    }
    
    return status;
    
  }

  public: Status Remove(UInt index) {
    return Error::MapNotIntIndexed;
  }
  
  public: Status Remove(String const &key) {
    
    Status status;
    
    status = _Remove(key);
    
    return status;
    
  }

}; // ...StringMapPrimitive<T> Template

//----------------------------------------------------------------------
//
// StringMapValue<V> Template Class
//

template <typename V> class StringMapValue : public _MapBase { // StringMapValue<V> Template...

  // C++ Lifecycle overrides
  public: StringMapValue()  {StartUp();}
  public: StringMapValue(StringMapValue const &that) {StartUp(); CopyFrom(that);}
  public: ~StringMapValue() {ShutDown();}
  public: StringMapValue &operator=(StringMapValue const &that) {Reset(); CopyFrom(that); return *this;}
  
  // BEFE Lifecycle overrides
  public: Status StartUp() {
    Status status;
    status = _MapBase::StartUp();
    _SetMapInfo(Map::KeyTypeString, sizeof(String)+sizeof(V), true, false);    
    return status;
  }
  
  public: Status ShutDown() {
    Status retStatus;
    Status status;    
    retStatus = _ResetElements();
    status = _MapBase::ShutDown();
    if (status && !retStatus) retStatus = status;
    return retStatus;
  }
  
  public: Status CopyFrom(StringMapValue<V> const &that) {
    
    Status status;
    UInt   numElms;
    UInt   curIdx;
    V     *thisElm;
    V     *thatElm;
    
    if (that.IsConsumable())
      status = _MapBase::MoveFrom(that);
    else {
      status = _MapBase::CopyFrom(that);
      if (status) goto SOMEERROR;
      numElms = Length();
      for (curIdx=0; curIdx < numElms; curIdx++) {
        thisElm = (V *)_GetPhyAddress(curIdx);
        if (thisElm != NULL)
          thisElm->StartUp();
      }
      for (curIdx=0; curIdx < numElms; curIdx++) {
        thisElm = (V *)_GetPhyAddress(curIdx);
        thatElm = (V *)((StringMapValue<V> *)&that)->_GetPhyAddress(curIdx);
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
  
  public: StringMapValue<V> &Consumable() const {
    sparse.Consumable();
    return *(StringMapValue<V> *)this;
  }

  public: StringMapValue<V> &NotConsumable() const {
    sparse.NotConsumable();
    return *(StringMapValue<V> *)this;
  }
  
  public: Boolean Contains(V const &that) const {
  
    Boolean  answer;
    Byte    *buf;
    UInt     bufSize;
    UInt     phyElmSize;
    UInt     phyHdrSize;
    UInt     numElms;
    UInt     curIdx;
  
    answer = false;
    ((StringMapValue<V> *)this)->sparse._BufAndSize(buf, bufSize);
    phyElmSize = ((StringMapValue<V> *)this)->sparse._GetPhyElementSize();
    phyHdrSize = ((StringMapValue<V> *)this)->sparse._GetPhyHeaderSize()+sizeof(String);
    if (buf && bufSize && phyElmSize) {
      numElms = bufSize/phyElmSize;
      for (curIdx=0; curIdx < numElms; curIdx++) {
        if (that.Compare(*((V *)(buf+curIdx*phyElmSize+phyHdrSize))) == 0) break;
      }
      answer = curIdx < numElms;
    }
    
    return answer;
  
  }

  // Map Template Specific Methods  
  public: Boolean Exists(UInt key) {
    return Error::MapNotStringIndexed;
  }
  
  public: Boolean Exists(String const &key) {
    
    //Status   status;
    Byte    *phyKey;
    Boolean  answer;
    
    phyKey = (Byte *)_GetPhyAddress(key);
    answer = (phyKey != NULL);

    return answer;
    
  }
  
  public: Status Keys(Strings &keys) const {
    return _MapBase::Keys(keys);
  }
  
  public: Strings Keys() const {
    
    //Status  status;
    Strings keys;
    
    /*status =*/ Keys(keys);
    
    return keys.Consumable();
    
  }
  
  public: Status Get(UInt index) const {
    return Error::MapNotIntIndexed;
  }
  
  public: V Get(String const &key) const {

    Byte *phyKey;
    V     local;
    
    phyKey = (Byte *)_GetPhyAddress(key);
    if (phyKey)
      local = *((V *)(phyKey + sizeof(String)));

    return local.Consumable();

  }

  public: Status Get(UInt index, V &that) const {
    return Error::MapNotIntIndexed;
  }
  
  public: Status Get(String const &key, V &that) const {

    Status  status;
    Byte   *phyKey;
    
    phyKey = (Byte *)_GetPhyAddress(key);
    if (phyKey) {
      that = *((V *)(phyKey + sizeof(String)));
      status = Error::None;
    }
    else
      status = Error::IndexNotFound;
      
    return status;

  }

  public: Status Set(UInt index, V const &value) const {
    return Error::MapNotIntIndexed;
  }

  public: Status Set(String const &key, V const &value) {
    
    Status   status;
    Byte    *phyValue;
    Boolean  created;

    status = _FindOrCreatePhyValueSpace(key, phyValue, created);
    if (status) goto SOMEERROR;
    if (created) {
      status = ((V *)phyValue)->StartUp();
      if (status) goto SOMEERROR;
    }
    *(V *)phyValue = value;
    
    // Handle errors
    while (false) {
      SOMEERROR: break;
    }
    
    return status;
    
  }

  public: Status Remove(UInt index) {
    return Error::MapNotIntIndexed;
  }
  
  public: Status Remove(String const &key) {
    
    Status status;
    UInt   phyIndex;

    status =  _FindKey(key, phyIndex);
    if (!status)
      status = _RemovePhyIndex(phyIndex);
    
    return status;
    
  }

  protected: Status _ResetElements() {
    
    Status retStatus;
    Status status;
    UInt   numElms;
    UInt   phyElmSize;
    UInt   keySize;
    UInt   curIdx;
    Byte  *curElm;
    
    retStatus = Error::None;
    numElms = Length();
    if (numElms) {
      curElm = (Byte *)_GetPhyAddress(0);
      if (curElm == NULL)
        retStatus = Error::InternalNullPointer;
      phyElmSize = sparse._GetPhyElementSize();
      keySize = _GetKeySize();
      curElm += keySize;
      for (curIdx=0; curIdx < numElms; curIdx++) {
        status = ((V *)curElm)->Reset();
        if (status && !retStatus) retStatus = status;
        curElm += phyElmSize;
      }
    }
    
    status = _MapBase::Reset();
    if (status && !retStatus) retStatus = status;
    
    return retStatus;

  }

}; // ...Template StringMapValue<V>

} // ...Namespace BEFE

#endif // ...MAP_H
