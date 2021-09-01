//!befe-publish inc
//----------------------------------------------------------------------
// File: Processor.h - Generic processor class declarations
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// NOTE: The "real" implementation of these classes is up to the
//       specific port that the product was built for.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef PROCESSOR_H // PROCESSOR_H...
#define PROCESSOR_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

class Processor { // Class Processor...

//
// Public enum declarations
//

public:

  enum ByteOrder {
     None         = 0x00000000  // Unknown
    ,BigEndian    = 0x00000001  // Non Intel (cool!)
    ,LittleEndian = 0x00000002  // Bloody Intel
  };

//
// Protected instance members
//

protected:

  String  type;          // Short distinct
  String  manufacturer;  // Manufacturer
  String  model;         // Model
  String  description;   // Verbose description
  UInt    byteorder;     // See enum above
  UInt    registersize;  // In bits
  UInt    pointersize;   // In bits
  Long    memorysize;    // In bytes (0="Unknown")
  UInt    speed;         // Processor speed (in MHz)

//
// Public instance methods
//

public:

  // C++ Lifecycle
  public: Processor();
  public: virtual ~Processor();

  // BEFE Lifecycle
  public: virtual Status StartUp();
  public: virtual Status ShutDown();
  public: virtual Status Reset();
  
  // Processor specific
  virtual String   GetType();
  virtual String   GetManufacturer();
  virtual String   GetModel();
  virtual String   GetDescription();
  virtual UInt     GetByteOrder();
  virtual UInt     GetRegisterSize();
  virtual UInt     GetPointerSize();
  virtual Long     GetMemorySize();
  virtual UInt     GetSpeed(); // Return registered processor speed (MHz)

}; // ...Class Processor

} // ...Namespace BEFE

#endif // ...PROCESSOR_H