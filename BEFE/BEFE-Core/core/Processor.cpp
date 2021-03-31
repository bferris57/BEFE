//----------------------------------------------------------------------
// File: Processor.cpp - Generic processor implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// NOTE: The "real" implementation of these classes is up to the
//       specific port that the product was built for.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------

#include "BEFE.h"
#include "Processor.h"

namespace BEFE {

Processor::Processor()  {StartUp();}
Processor::~Processor() {ShutDown();}

Status Processor::StartUp() {

  if (this != TheBefe->TheProcessor) {

    type         = String(CONSTR_QQQ);
    manufacturer.StartUp();
    model.StartUp();
    //manufacturer = String(CONSTR_Unknown) + " Manufacturer";
    //model        = String(CONSTR_Unknown) + " Model";
    description  = String(CONSTR_Unknown);
    byteorder    = None;
    registersize = sizeof(int)*8;
    pointersize  = sizeof(void *)*8;
    memorysize   = 0;
    speed        = 0;

  }

  return Error::None;
}

Status Processor::ShutDown() {

  type.ShutDown();
  manufacturer.ShutDown();
  model.ShutDown();
  description.ShutDown();

  return Error::None;
}

Status Processor::Reset() {

  type.Reset();
  manufacturer.Reset();
  model.Reset();
  description.Reset();

  return Error::None;
}

String Processor::GetType() {
  return type;
}

String Processor::GetManufacturer() {
  return manufacturer;
}

String Processor::GetModel() {
  return model;
}

String Processor::GetDescription() {
  return description;
}

UInt Processor::GetByteOrder() {
  return byteorder;
}

UInt Processor::GetRegisterSize() {
  return registersize;
}

UInt Processor::GetPointerSize() {
  return pointersize;
}

Long Processor::GetMemorySize() {
  return memorysize;
}

UInt Processor::GetSpeed() {
  return speed;
}

}; // Namespace BEFE
