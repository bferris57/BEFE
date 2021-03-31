//----------------------------------------------------------------------
// File: Ids.cpp - Implementation of the Ids class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Ids.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Ids::Ids() {
  StartUp();
}

Ids::Ids(Ids const &that) {
  StartUp();
  CopyFrom(that);
}

Ids::~Ids() {
  ShutDown();
}

Ids &Ids::operator=(Ids const &that) {
  _ids.CopyFrom(that._ids);
  return *this;
}

  
// BEFE Lifecycle
Status Ids::StartUp() {
  Status status;
  status = _ids.StartUp();
  return status;
}

Status Ids::ShutDown() {
  Status status;
  status = _ids.ShutDown();
  return status;
}

Status Ids::Reset() {
  Status status;
  status = _ids.Reset();
  return status;
}

Boolean Ids::IsNull()  const {
  return _ids.IsNull();  
}

Status Ids::SetNull() {
  return _ids.SetNull();
}

Boolean Ids::IsEmpty() const {
  return _ids.IsEmpty();
}

Status Ids::SetEmpty() {
  return _ids.SetEmpty();
}

Status Ids::MoveFrom(Ids const &that) {
  Status status;
  status = _ids.MoveFrom(that._ids);
  return status;
}

Status Ids::CopyFrom(Ids const &that) {
  Status status;
  status = _ids.MoveFrom(that._ids);
  return status;
}

Ids &Ids::Consumable() const {
  _ids.Consumable();
  return *((Ids *)this);
}

Ids &Ids::NotConsumable() const {
  _ids.NotConsumable();
  return *((Ids *)this);
}

Boolean Ids::IsConsumable() const {
  return _ids.IsConsumable();
}

Status Ids::SetLength(UInt newElms) const {
  Status status;
  status = ((_ArrayBase *)&_ids)->SetLength(newElms);
  return status;
}

Boolean Ids::Contains(Id const &id) const {
  
  Status  status;
  Boolean answer;
  UInt    index;
  
  status = Find(id, index);
  answer = (status == Error::None);
  
  return answer;
  
}

Status Ids::Contains(Id const &id, Boolean &answer) const {

  Status  status;
  UInt    index;
  
  answer = false;
  status = Find(id, index);
  if (!status)
    answer = true;
  
  return status;

}
  
UInt Ids::Length() const {
  return _ids.Length();
}

UInt Ids::Size() const {
  return _ids.Size();
}
  
// Contents Access Methods
Id Ids::Get(Int index) {
  Status status;
  Id     id;
  status = _ids.Get(index, id);
  if (status)
    id = UNaN;
  return id;    
}

Status Ids::Get(Int index, Id &that) {
  return _ids.Get(index, that);
}

Status Ids::Insert(Id id) {
  
  Status status;
  UInt   index;
  
  status = Find(id, index);
  if (status && status != Error::NotFound) goto SOMEERROR;
  if (!status) goto EXISTS;
  if (index == _ids.Length())
    status = _ids.Append(id);
  else
    status = _ids.Insert(index, id);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    EXISTS:    status = Error::AlreadyExists; break;
    SOMEERROR:                                break;
  }

  return status;
    
}

Status Ids::Remove(Id that) {
  
  Status status;
  UInt   index;
  
  status = Find(that, index);
  if (status) goto SOMEERROR;
  status = _ids._Remove(index);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Protected Methods
//

Status Ids::Find(Id that, UInt &idx) const {
  
  Status  status;
  Id     *buf;
  UInt    numIds;
  Int     ll;
  Int     ul;

  ((_ArrayBase *)&_ids)->_BufAndLength(*(Byte **)&buf, numIds);
  if (BEFE::IsNull(numIds) || numIds == 0) {
    idx = 0;
    goto NOTFOUND;
  }
  
  ll = 0;
  ul = numIds-1;
  while (ll <= ul) {
    idx = (ll+ul)>>1;
    if ((UInt)buf[idx] == (UInt)that) break;
    if ((UInt)buf[idx] > (UInt)that)
      ul = idx-1;
    else
      ll = idx+1;
  }
  if (buf[idx] != (UInt)that) {
    idx = ll;
    goto NOTFOUND;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTFOUND: status = Error::NotFound; break;
  }
    
  return status;
  
}

} // ...Namespace BEFE