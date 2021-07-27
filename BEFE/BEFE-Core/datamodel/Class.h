//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: Class.h - Class Class declarations
//----------------------------------------------------------------------
//
// This header contains the Class Class and related declarations
//
// *** See technical documentation for a more detailed description ***
//
// Note: For now, the Class class is a stub for future use and provides
//       very limited functionality.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CLASS_H
#define CLASS_H

#include "Primitives.h"
#include "Storage.h"

namespace BEFE {

//----------------------------------------------------------------------
//
// Class: Class - Data Model "Class" Class
//

class Class {
  
  //
  // Public Methods
  //
  
  // C++ Lifecycle
  public:          Class();
  public:          Class(char const *className);
  public:          Class(String const &className);
  public: virtual ~Class();
  
  // BEFE Lifecycle
  public: virtual Status  StartUp();
  public: virtual Status  ShutDown();
  public: virtual Status  Reset();
  public: virtual Boolean IsNull()  const;
  public: virtual Status  SetNull();  
  public: virtual Boolean IsEmpty() const;
  public: virtual Status  SetEmpty();
  public: virtual Status  MoveFrom(Class const &that);
  public: virtual Status  CopyFrom(Class const &that);
  public: virtual Class  &Consumable() const;
  public: virtual Class  &NotConsumable() const;
  public: virtual Boolean IsConsumable() const;
  
  // Member Query and Manipulation
  public: virtual Status GetName(String &className) const;
  public: virtual String GetName() const;
  public: virtual Status SetName(char const *className);
  public: virtual Status SetName(String const &className);
  
  //
  // Protected members
  //
  
  protected: String name;
  
};

} // Namespace BEFE

#endif // PERSIST_H