//----------------------------------------------------------------------
// File: Class.cpp - Implementation of the Class class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Class.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Class::Class() {
  StartUp();
}
 
Class::Class(char const *className) {
  StartUp();
  name = className;
}

Class::Class(String const &className) {
  StartUp();
  name = className;
}

Class::~Class() {
  ShutDown();
}
  
// BEFE Lifecycle
Status Class::StartUp() {
  
  Status retStatus;
  
  retStatus = name.StartUp();
  
  return retStatus;
  
}

Status Class::ShutDown() {
  
  Status retStatus;
  
  retStatus = name.ShutDown();
  
  return retStatus;
  
}

Status Class::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status    = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}
  
Boolean Class::IsNull() const {
  return name.IsNull();
}

Status Class::SetNull() {
  name.SetNull();
  return Error::None;
}

Boolean Class::IsEmpty() const {
  return name.IsEmpty();
}

Status Class::SetEmpty() {
  name.SetEmpty();
  return Error::None;
}

Status Class::MoveFrom(Class const &that) {
  return Error::NotImplemented;
}

Status Class::CopyFrom(Class const &that) {
  return Error::NotImplemented;
}

Class &Class::Consumable() const {
  name.Consumable();
  return *(Class *)this;
}

Class &Class::NotConsumable() const {
  name.NotConsumable();
  return *(Class *)this;
}

Boolean Class::IsConsumable() const {
  return name.IsConsumable();
}

// Member Query and Manipulation
Status Class::GetName(String &className) const {
  className = name;
  return Error::None;
}

String Class::GetName() const {
  return name;
}

Status Class::SetName(char const *className) {
  name = className;
  return Error::None;
}

Status Class::SetName(String const &className) {
  name = className;
  return Error::None;
}

} // ...Namespace BEFE