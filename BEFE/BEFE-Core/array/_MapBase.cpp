//----------------------------------------------------------------------
// File: _MapBase.cpp - Implementation of the _MapBase class
//----------------------------------------------------------------------
//
// This is the implementation of the _MapBase class.  It is intended
// to be used for all the implementations of the IntMap... and
// StringMap... templates.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Map.h"

namespace BEFE { // BEFE...

//----------------------------------------------------------------------
//
// General _MapBase Functions...
//

Boolean IsNull(_MapBase const &v) {
  return v.IsEmpty();
}
  
//----------------------------------------------------------------------
//
// C++ Lifecycle methods
//

_MapBase::_MapBase() {
  StartUp();
}

_MapBase::_MapBase(_MapBase const &that) {
  StartUp();
  CopyFrom(that);
}

_MapBase::~_MapBase() {
  ShutDown();
}

_MapBase &_MapBase::operator = (_MapBase const &that) {
  CopyFrom(that);
  NotConsumable();
  return *this;
}
  
//----------------------------------------------------------------------
//
// BEFE Lifecycle methods
//

Status _MapBase::StartUp() {
  Status status;
  status = sparse.StartUp();
  return status;
}

Status _MapBase::ShutDown() {
  
  Status  retStatus;
  Status  status;

  // Reset keys if need be...
  retStatus = _ResetKeys();

  // Shutdown the SparseBase
  status = sparse.ShutDown();
  if (status && !retStatus) retStatus = status;
    
  return retStatus;
  
}
  
Status _MapBase::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = _ResetKeys();
  status = sparse.Reset();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Status _MapBase::MoveFrom(_MapBase const &that) {

  Status status;
  
  if (this != &that)
    status = sparse.MoveFrom(that.sparse);
  else
    status = Error::None;

  return status;

}

Status _MapBase::CopyFrom(_MapBase const &that) {
  
  Status  status;
  Status  tStatus;
  UInt    numElms;
  Byte   *srcKey;
  Byte   *dstKey;
  UInt    phyElmSize;
  UInt    phyElm;
  
  if (IsConsumable())

    status = MoveFrom(that);

  else { // Copy it...

    numElms = that.sparse.Length();
    status = sparse.CopyFrom(that.sparse);
    if (status) goto SOMEERROR;

    // Copy the Keys if required...
    if (sparse.mapKeyType && numElms != 0) { // Copy Keys...
    
      // Reset the keys first
      status = _ResetKeys();
      if (status) goto SOMEERROR;

      // Copy them now...
      phyElmSize = sparse._GetPhyElementSize();
      srcKey = (Byte *)that.sparse._GetPhyAddress(0);
      dstKey = (Byte *)sparse._GetPhyAddress(0);
      if (BEFE::IsNull(srcKey) || BEFE::IsNull(dstKey) || phyElmSize == 0) goto NULLPOINTER;

      srcKey = (Byte *)that.sparse._GetPhyAddress(0);
      dstKey = (Byte *)sparse._GetPhyAddress(0);      
      for (phyElm=0; phyElm < numElms; phyElm++) {
        tStatus = ((String *)dstKey)->CopyFrom(*((String *)srcKey));
        if (tStatus && !status) status = tStatus;
        dstKey += phyElmSize;
        srcKey += phyElmSize;
      }
      if (status) goto SOMEERROR;
    
    } // ...Copy Keys
    
  } // ...Copy it
  
  // Handle Errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

_MapBase &_MapBase::Consumable() const {
  sparse.Consumable();
  return *((_MapBase *)this);
}

_MapBase &_MapBase::NotConsumable() const {
  sparse.NotConsumable();
  return *((_MapBase *)this);
}

Boolean _MapBase::IsConsumable() const {
  return sparse.IsConsumable();
}

Status  _MapBase::SetLength(UInt newElms) const {
  
  Status status;
  
  if (Length())
    status = Error::ArrayNotEmpty;
  else
    status = sparse.SetLength(newElms);

  return status;

}

Boolean _MapBase::IsMutable() const {
  return sparse.IsMutable();
}

Status _MapBase::Mutable() {
  return sparse.Mutable();
}

Status _MapBase::Immutable() {
  return sparse.Immutable();
}
  
Boolean _MapBase::_Contains(Byte *that) const {
  //Status  status;
  Boolean answer;
  /*status =*/ _Contains(that, answer);
  return answer;
}

Status _MapBase::_Contains(Byte *that, Boolean &answer) const {

  Status status;

  status = sparse._Contains(that, answer);

  return status;
  
}

//----------------------------------------------------------------------
//
// Contents Info Methods
//

UInt _MapBase::Length() const {
  return sparse.Length();
}

UInt _MapBase::Size() const {
  return sparse.Size();
}

UInt _MapBase::ElementSize() const {
  
  UInt phyElmSize;
  
  phyElmSize  = sparse._GetPhyElementSize();
  phyElmSize -= sparse._GetPhyHeaderSize();
  if (sparse.isMap && sparse.mapKeyType)
    phyElmSize -= sizeof(String);

  return phyElmSize;

}

UInt _MapBase::UnitSize() const {
  return sparse.UnitSize();
}

Status _MapBase::_FindKey(UInt index, UInt &phyIndex) const {
  
  Status status;
  USpan  phyIndexSpan;
  UInt   len;
  
  if (sparse.isMap && sparse.mapKeyType) goto NOTINT;
  
  // Find where it is
  phyIndexSpan = sparse.PhysicalIndexSpan(index);
  len = phyIndexSpan.Length();
  if (len == 0)
    goto NOTFOUND;
  else if (len != 1)
    goto NOTDISTINCT;
  phyIndex = phyIndexSpan.idx1;
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTINT:      status = Error::MapNotIntIndexed;  break;
    NOTFOUND:    status = Error::IndexNotFound;     break;
    NOTDISTINCT: status = Error::IndexNotDistinct;  break;
  }
  
  return status;

}

Status _MapBase::_FindKey(String const &key, UInt &phyIndex) const {
  
  Status  status;
  UInt    hash;
  USpan   phyIndexSpan;
  Byte   *buf;
  UInt    curIdx;
  UInt    phyElmSize;
  String *curKey;
  Boolean equals;
  
  if (!sparse.isMap && sparse.mapKeyType == 0) goto NOTSTRING;
  
  // Get the string hash
  if (sparse.isInsensitive)
    hash = key.HashInsensitive();
  else
    hash = key.HashSensitive();
    
  // Find where it is
  phyIndexSpan = sparse.PhysicalIndexSpan(hash);
  if (phyIndexSpan.Length() == 0) goto NOTFOUND;
  
  // Set up buffer and pointer info
  phyElmSize  = sparse._GetPhyElementSize();
  buf         = sparse.buffer.GetBuf();
  curIdx      = phyIndexSpan.idx1;
  curKey = (String *)(buf + curIdx*phyElmSize + sparse._GetPhyHeaderSize());  
  while (curIdx < phyIndexSpan.idx2) {
    if (sparse.isInsensitive)
      equals = key.CompareEqualsInsensitive(*curKey);
    else
      equals = key.CompareEqualsSensitive(*curKey);
    if (equals) {
      phyIndex = curIdx;
      goto FOUND;
    }
    curIdx++;
    curKey = (String *)((Byte *)curKey + phyElmSize);
  }
  
  goto NOTFOUND;
  
  // Handle errors
  while (false) {
    NOTSTRING: status = Error::MapNotStringIndexed; break;
    FOUND:     status = Error::None;                break;
    NOTFOUND:  status = Error::IndexNotFound;       break;
  }

  if (status)
    BEFE::SetNull(phyIndex);
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Keys...
//

Boolean _MapBase::IsSensitive() const {
  Boolean answer;
  if (!sparse.mapKeyType || sparse.Length() || sparse.isInsensitive)
    answer = false;
  else
    answer = true;
  return answer;
}

Status _MapBase::SetSensitivity(Boolean sense) {
  Status status;
  if (!sparse.mapKeyType)
    status = Error::MapNotStringIndexed;
  else if (!sparse.mapKeyType)
    status = Error::ArrayNotEmpty;
  else {
    status = Error::None;
    sparse.isInsensitive = !sense;
  }
  return status;
}

Status _MapBase::SetSensitive() {
  return SetSensitivity(true);
}

Status _MapBase::SetInsensitive() {
  return SetSensitivity(false);
}

Status _MapBase::Keys(UInts &keys) const {
  
  Status status;
  UInt   numKeys;
  UInt   phyElmSize;
  Byte  *key;
  
  status = keys.SetEmpty();
  if (status) goto SOMEERROR;
  if (sparse.mapKeyType) goto NOTINTMAP;
  
  // Get number of keys...
  sparse._BufAndSize(key, numKeys);
  if (numKeys == 0) goto OK;
  phyElmSize = sparse._GetPhyElementSize();
  if (BEFE::IsNull(key) || phyElmSize == 0) goto NULLPOINTER;
  numKeys /= phyElmSize;
  
  // Get the keys...
  while (numKeys) {
    status = keys.Append(*((UInt *)key));
    if (status) goto SOMEERROR;
    key += phyElmSize;
    numKeys--;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTINTMAP:   status = Error::MapNotIntIndexed;    break;  
    OK:          status = Error::None;                break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}

Status _MapBase::Keys(Strings &keys) const {
  
  Status status;
  UInt   numKeys;
  UInt   phyElmSize;
  Byte  *key;
  
  status = keys.SetEmpty();
  if (status) goto SOMEERROR;
  if (!sparse.mapKeyType) goto NOTSTRMAP;
  
  // Get number of keys...
  sparse._BufAndSize(key, numKeys);
  if (numKeys == 0) goto OK;
  phyElmSize = sparse._GetPhyElementSize();
  if (BEFE::IsNull(key) || phyElmSize == 0) goto NULLPOINTER;
  numKeys /= phyElmSize;
  key = key + sparse._GetPhyHeaderSize();
  
  // Get the keys...
  while (numKeys) {
    status = keys.Append(*((String *)key));
    if (status) goto SOMEERROR;
    key += phyElmSize;
    numKeys--;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTSTRMAP:   status = Error::MapNotStringIndexed; break;  
    OK:          status = Error::None;                break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
}
  
Boolean _MapBase::HasKey(UInt index) const {
  
  Status status;
  UInt   phyIndex;
  
  BEFE::SetNull(phyIndex);
  status = _FindKey(index, phyIndex);
  
  return ( status == Error::None && !BEFE::IsNull(phyIndex) );
  
}

Boolean _MapBase::HasKey(String const &key) const {
  
  Status status;
  UInt   phyIndex;
  
  status = _FindKey(key, phyIndex);
  
  return ( status == Error::None && !BEFE::IsNull(phyIndex) );
  
}

//----------------------------------------------------------------------
//
// Protected Methods
//

void  _MapBase::_SetMapInfo(UInt keyType, UInt elmSize, Boolean isDistinct, Boolean isInsensitive) {
  
  sparse.mapKeyType    = keyType;
  sparse.elmSize       = elmSize;
  sparse.isDistinct    = isDistinct;
  sparse.isInsensitive = isInsensitive;
  
}

Byte const *_MapBase::_GetPhyAddress(UInt index) const {
  return sparse._GetPhyAddress(index);
}

Byte const *_MapBase::_GetPhyAddress(String const &key) const {

  //Status  status;
  USpan   phySpan;
  UInt    remLen;
  Byte   *phyKey;
  UInt    elmSize;
  Boolean isInsensitive;
  Boolean equals;
  
  /*status =*/ _GetKeySpan(key, phySpan);
  remLen = phySpan.Length();

  while (remLen) { // Some Keys with this Hash...

    elmSize = sparse._GetPhyElementSize();
    phyKey  = (Byte *)sparse._GetPhyAddress(phySpan.idx1);
    if ( BEFE::IsNull(phyKey) || elmSize == 0) {
      BEFE_ERROR("_MapBase._GetPhyAddress(String const &)",Error::InternalNullPointer);
      remLen = 0;
      break;
    }
    isInsensitive = _IsInsensitive();

    while (remLen) { // For each sub-key...
      if (isInsensitive)
        equals = key.CompareEqualsInsensitive(*(String *)phyKey);
      else
        equals = key.CompareEqualsSensitive(*(String *)phyKey);
      if (equals) break;
      phyKey += elmSize;
      remLen--;
    } // ...For each sub-key
            
    if (remLen) break;
    
  } // ...Some Keys with this Hash

  if (remLen == 0)
    phyKey = NULL;
    
  return phyKey;
      
}

Byte const *_MapBase::_GetLogAddress(UInt index) const {
  return sparse._GetLogAddress(index);
}

UInt _MapBase::_GetElementSize() const {

  UInt phyElmSize;
  
  phyElmSize  = sparse._GetPhyElementSize();

  return phyElmSize;

}

UInt _MapBase::_GetKeySize() const {
  return (sparse.mapKeyType)?sizeof(String):sizeof(Int);
}

Boolean _MapBase::_IsInsensitive() const {
  return sparse.isInsensitive;
}

Status _MapBase::_GetKeySpan(String const &key, USpan &span) const {
  
  UInt   hash;
  USpan  phySpan;
  
  if (sparse.isInsensitive)
    hash = key.HashInsensitive();
  else
    hash = key.HashSensitive();
  span = sparse.PhysicalIndexSpan(hash);
  
  return Error::None;
  
}

Status _MapBase::_FindOrCreatePhyValueSpace(String const &key, Byte *&value, Boolean &created) {

  Status  status;
  UInt    hash;
  USpan   phySpan;
  UInt    phyIdx;
  UInt    remLen;
  Byte   *phyKey;
  UInt    elmSize;
  Boolean equals;

  value   = NULL;
  created = false;  
  status = _GetKeySpan(key, phySpan);
  if (status) goto SOMEERROR;

  remLen = phySpan.Length();
  phyIdx = phySpan.idx1;
  
  while (remLen) { // Some Keys with this Hash...

    elmSize = sparse._GetPhyElementSize();
    phyIdx = phySpan.idx1;
    phyKey  = (Byte *)sparse._GetPhyAddress(phyIdx);
    if (BEFE::IsNull(phyKey) || elmSize == 0) goto NULLPOINTER;

    equals = false;
    while (remLen) { // For each sub-key...
      if (sparse.isInsensitive)
        equals = key.CompareEqualsInsensitive(*(String *)phyKey);
      else
        equals = key.CompareEqualsSensitive(*(String *)phyKey);
      if (equals) break;
      phyKey += elmSize;
      phyIdx++;
      remLen--;
    } // ...For each sub-key
            
    if (equals) break;
    
  } // ...Some Keys with this Hash

  // Create the space if needed...
  if (remLen == 0) {
    if (sparse.isInsensitive)
      hash = key.HashInsensitive();
    else
      hash = key.HashSensitive();
    status = sparse._PhyInsert(phyIdx, hash, phyKey);
    if (status) goto SOMEERROR;
    status = ((String *)phyKey)->StartUp();
    if (status) goto SOMEERROR;
    *(String *)phyKey = key;
    created = true;
  }

  // Return the value address to the caller
  value = phyKey + sizeof(String);
  
  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
      
}

Status _MapBase::_Remove(String const &key) {

  Status  status;
  USpan   phySpan;
  UInt    phyIdx;
  UInt    remLen;
  Byte   *phyKey;
  UInt    elmSize;
  Boolean equals;

  status = _GetKeySpan(key, phySpan);
  if (status) goto SOMEERROR;

  remLen = phySpan.Length();
  phyIdx = phySpan.idx1;
  
  while (remLen) { // Some Keys with this Hash...

    elmSize = sparse._GetPhyElementSize();
    phyIdx = phySpan.idx1;
    phyKey  = (Byte *)sparse._GetPhyAddress(phyIdx);
    if ( BEFE::IsNull(phyKey) || elmSize == 0) goto NULLPOINTER;

    equals = false;
    while (remLen) { // For each sub-key...
      if (sparse.isInsensitive)
        equals = key.CompareEqualsInsensitive(*(String *)phyKey);
      else
        equals = key.CompareEqualsSensitive(*(String *)phyKey);
      if (equals) break;
      phyKey += elmSize;
      phyIdx++;
      remLen--;
    } // ...For each sub-key
    
    if (equals) break;
    
  } // ...Some Keys with this Hash

  // Make sure it's there...
  if (remLen == 0) goto NOTEXIST;
  
  // Remove it...
  status = ((String *)phyKey)->ShutDown();
  if (status) goto SOMEERROR;
  status = sparse._PhyRemove(phyIdx);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    NOTEXIST:    status = Error::IndexNotFound;       break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
      
}
  
Status _MapBase::_RemovePhyIndex(UInt phyIdx) {

  Status  status;
  Byte   *phyKey;

  // Make sure it's there...
  if (phyIdx >= sparse.Length()) goto NOTEXIST;
  phyKey = (Byte *)sparse._GetPhyAddress(phyIdx);
  if (BEFE::IsNull(phyKey)) goto NULLPOINTER;
  phyKey += sparse._GetPhyHeaderSize();
  
  // Remove it...
  status = ((String *)phyKey)->ShutDown();
  if (status) goto SOMEERROR;
  status = sparse._PhyRemove(phyIdx);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    NOTEXIST:    status = Error::IndexNotFound;       break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
      
}

Status _MapBase::_ResetKeys() {

  Status  retStatus;
  Status  status;
  UInt    numElms;
  Byte   *dstKey;
  UInt    phyElmSize;
  UInt    phyElm;

  retStatus = Error::None;
  numElms   = sparse.Length();
  
  if (sparse.mapKeyType && numElms != 0) { // StringMap and needs resetting......
  
    // Reset the Keys if required...
    if (sparse.mapKeyType && numElms != 0) { // Reset Keys...

      phyElmSize = sparse._GetPhyElementSize();
      dstKey = (Byte *)sparse._GetPhyAddress(0);
      if (BEFE::IsNull(dstKey) || phyElmSize == 0) goto NULLPOINTER;

      // Call key.StartUp on each key first
      for (phyElm=0; phyElm < numElms; phyElm++) {
        status = ((String *)dstKey)->Reset();
        if (status && !retStatus) retStatus = status;
        dstKey += phyElmSize;
      }
      if (retStatus) goto SOMEERROR;
          
    } // ...Reset Keys
    
  } // ...It's a StringMap
  
  // Handle Errors
  while (false) {
    NULLPOINTER: retStatus = Error::InternalNullPointer; break;
    SOMEERROR:                                           break;
  }
    
  return retStatus;
  
}
  
} // ...BEFE
