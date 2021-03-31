//----------------------------------------------------------------------
// File: Persist.cpp - Implementation of the base Persist class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Persist.h"
#include "Integer.h" // For IntReverseBytes, etc.

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Persist::Persist() {
  StartUp();
}
 
Persist::~Persist() {
  ShutDown();
}
  
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Persist::StartUp() {
  
  Status retStatus;
  Status status;
  
  retStatus = stream.StartUp();
  status = stream.SetAccess(File::ModeReadWrite);
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Status Persist::ShutDown() {

  Status retStatus;
  
  retStatus = stream.ShutDown();
  
  return retStatus;

}

Status Persist::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status    = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Boolean Persist::IsNull() const {
  return (BEFE::IsNull(storeType) && stream.IsNull());
}

Status Persist::SetNull() {
  return Reset();
}
  
Boolean Persist::IsEmpty() const {
  // For NOW...
  return false;  
}

Status Persist::SetEmpty() {
  return Error::NotImplemented;
}

Status Persist::MoveFrom(Persist const &that) {
  return Error::NotImplemented;
}

Status Persist::CopyFrom(Persist const &that) {
  return Error::NotImplemented;
}

Persist &Persist::Consumable() const {
  return *(Persist *)this;
}

Persist &Persist::NotConsumable() const {
  return *(Persist *)this;
}

Boolean Persist::IsConsumable() const {
  return false;
}

//----------------------------------------------------------------------
//
// Persist Member manipulation
//

String Persist::GetFilename() const {
  return stream.GetName();
}
  
Status Persist::SetFileName(char const *fileName) {
  
  Status status;
  UInt   mode;
  
  mode = stream.GetAccess();
  status = stream.SetName(fileName);
  stream.SetAccess(mode);
  
  return status;
  
}

Status Persist::SetFilename(String const &fileName) {
  return stream.SetName(fileName);
}
  
//----------------------------------------------------------------------
//
// Write Transactions
//

Status Persist::StartWrite() {
  
  Status status;
  UInt   uint;
  
  Byte   signature[] = {0xBE, 0xFE, // [0..1] - Signature
                        0x00,       // [2]    - Main Version
                        0x00,       // [3]    - Sub Version
                        0xff, 0xff, // [4..5] - Main Revision
                        0xff, 0xff, // [6..7] - Sub Revision
                        0xff  };    // [8]    - Data Structure Type (*** see tech docs***)
                        
  status = stream.SetSize(0);
  if (status) goto SOMEERROR;
  status = stream.SetPos(0);
  if (status) goto SOMEERROR;
  
  // Populate Signature
  uint = TheBefe->ThePlatform->GetMainVersion();
  signature[2] = uint;
  uint = TheBefe->ThePlatform->GetSubVersion();
  signature[3] = uint;
  uint = TheBefe->ThePlatform->GetMainRevision();
  signature[4] = uint>>8;
  signature[5] = uint;
  uint = TheBefe->ThePlatform->GetSubRevision();
  signature[6] = uint>>8;
  signature[7] = uint;
  signature[8] = 0; // ◄── This should, actually, be 0xFF but we'll leave it like this for now
  
  // Write the Signature
  status = WriteAtomicRaw(signature,sizeof(signature));
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

Status Persist::AbortWrite() {
  return Error::Failed;
}

Status Persist::AbortWrite(Status error) {
  if (error)
    BEFE_ERROR("AbortWrite",error);
  return Error::None;
}

Status Persist::EndWrite() {

  Status status;
  
  Byte   endSignature[] = {0xBE, 0xFF}; // [0..1] - ...Signature
                        
  // Write the End Signature
  status = WriteAtomicRaw(endSignature,sizeof(endSignature));
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

Status Persist::StartSequenceAnon() {
  BEFE_WARN("Persist.StartSequenceAnon() not implemented yet");
  return Error::NotImplemented;
}

Status Persist::StartSequenceAnon(UInt32 length) {
  BEFE_WARN("Persist.StartSequenceAnon(UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::StartSequenceNamed(char const *name) {
  BEFE_WARN("Persist.StartSequenceNamed(char const *) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::StartSequenceNamed(char const *name, UInt32 length) {
  if (gShowDebug)
    BEFE_WARN("Persist.StartSequenceNamed(char const *, UInt32) not implemented yet");
  return Error::None;
}

Status Persist::StartSequenceNamed(String const &name) {
  BEFE_WARN("Persist.StartSequenceNamed(String const &) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::StartSequenceNamed(String const &name, UInt32 length) {
  BEFE_WARN("Persist.StartSequenceNamed(String const &, UInt32) not implemented yet");
  return Error::NotImplemented;
}

Status Persist::StartSequenceInst(char const *instanceOf) {
  BEFE_WARN("Persist.StartInstSequence(char const *) not implemented yet");
  return Error::None;
}

Status Persist::StartSequenceInst(String const &instanceOf) {
  BEFE_WARN("Persist.StartInstSequence(String const &) not implemented yet");
  return Error::None;
}

Status Persist::StartSequenceInst(Class const &instanceOf) {
  
  Status status;
                        
  // Write the Start Sequence
  status = WriteAtomic((UInt8)ValueType::SequenceInstance);
  if (status) goto SOMEERROR;
  
  // Write the Class name
  status = WriteAtomicString(instanceOf.GetName());
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

Status Persist::StartSequenceInst(char const *instanceOf, UInt32 length) {
  //WARN("Persist.StartInstSequence(char const *, UInt32) not implemented yet");
  return Error::None;
}

Status Persist::StartSequenceInst(String const &instanceOf, UInt32 length) {
  //WARN("Persist.StartInstSequence(String const &, UInt32) not implemented yet");
  return Error::None;
}

Status Persist::StartSequenceInst(Class const &instanceOf, UInt32 length) {
  
  //WARN("Persist.StartInstSequence(Class const &, UInt32) not implemented yet");
  if (gVeryVerbose)
    Cout << "Saving instance of Class '" << instanceOf.GetName().Consumable() << "'...\n";
    
  return Error::None;
  
}

Status Persist::AbortSequence() {
  if (gShowDebug)
    BEFE_WARN("Persist.AbortSequence not implemented yet");
  return Error::None;
}

Status Persist::AbortSequence(Status error) {
  if (gShowDebug)
    BEFE_WARN("Persist.AbortSequence not implemented yet");
  if (error)
    BEFE_ERROR("AbortSequence",error);
  return Error::None;
}

Status Persist::EndSequence() {

  Status status;
  
  // Write the End Sequence
  status = WriteAtomic((UInt8)ValueType::EndSequence);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

} // ...Namespace BEFE