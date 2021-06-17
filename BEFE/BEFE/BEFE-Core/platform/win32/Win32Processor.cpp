//----------------------------------------------------------------------
// File: Win32Processor.h - Implementation of the Win32Processor Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Win32.h"

namespace BEFE { // Namespace BEFE...

Win32Processor::Win32Processor() {
  StartUp();
}

Win32Processor::~Win32Processor() {
  ShutDown();
}

Status Win32Processor::StartUp() {
  return _StartUp();
}

Status Win32Processor::ShutDown() {
  return _ShutDown();
}

Status Win32Processor::_StartUp() {

  // If no vtable, make one...
  if (*(UInt *)this == 0) {
    Win32Processor x;
    *(Int32 **)this = *(Int32 **)&x;
  }
  
  Processor::StartUp();

  type = "x86";
  manufacturer.StartUp();
  model.StartUp();
  description.StartUp();

  byteorder    = LittleEndian;
  registersize = sizeof(int)*8;
  memorysize   = Win32GetPhysicalMemory();
  speed        = Win32RegGetValueInt("HKEY_LOCAL_MACHINE/HARDWARE/DESCRIPTION/System/CentralProcessor/0:~MHz");

  if (!TheBefe->TheProcessor)
    TheBefe->TheProcessor = this;
    
  return Error::None;

}

Status Win32Processor::_ShutDown() {

  if (TheBefe->TheProcessor) {
    if (TheBefe->TheProcessor == this)
      TheBefe->TheProcessor = NULL;
    else
      Processor::ShutDown();
  }

  return Error::None;

}

//----------------------------------------------------------------------
//
// Processor specific
//

String Win32Processor::GetManufacturer() {
  InitVars();
  return manufacturer;
}

String Win32Processor::GetModel() {
  InitVars();
  return model;
}

String Win32Processor::GetDescription() {
  InitVars();
  return description;
}

//----------------------------------------------------------------------
//
// Internal Methods
//

Status Win32Processor::InitVars() {

  if (manufacturer.IsNull()) {
    Win32RegGetValueString("HKEY_LOCAL_MACHINE/HARDWARE/DESCRIPTION/System/CentralProcessor/0:VendorIdentifier",manufacturer);
    Win32RegGetValueString("HKEY_LOCAL_MACHINE/HARDWARE/DESCRIPTION/System/CentralProcessor/0:Identifier",model);
    Win32RegGetValueString("HKEY_LOCAL_MACHINE/HARDWARE/DESCRIPTION/System/CentralProcessor/0:ProcessorNameString",description);
  }
  
  return Error::None;
  
}

} // ...Namespace BEFE
