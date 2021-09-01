//----------------------------------------------------------------------
// File: Buffer.cpp - Implementation of the Buffer class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Buffer.h"
#include "BufferLow.h"

namespace BEFE { // Namespace BEFE...

//
// Handy defines
//
  
#define BUF ((BufferLow *)pointer)
#define pBUF *((BufferLow **)&pointer)

//
// Static globals
//

UInt32 Buffer::totalAllocations  = 0;  // Total allocation calls
UInt32 Buffer::activeAllocations = 0;  // Currently active allocations

//----------------------------------------------------------------------
//
// Class (static) Methods
//

UInt Buffer::GetTotalAllocations() {
  return totalAllocations;
}

UInt Buffer::GetActiveAllocations() {
  return activeAllocations;
}

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Buffer::Buffer() {
  StartUp();
}

Buffer::Buffer(Buffer const &that) {
  StartUp();
  CopyFrom(that);
}

Buffer::Buffer(UInt initBytes, UInt incBytes) {
  StartUp();
  Allocate(initBytes, incBytes);
}

Buffer::~Buffer() {
  ShutDown();
}

Buffer &Buffer::operator = (Buffer const &that) {

  if (this != (Buffer *)&that) {
    ShutDown();
    StartUp();
    CopyFrom(that);
    NotConsumable();
  }
  
  return *this;
  
}
  
//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Buffer::StartUp() {
  pointer = NULL;
  return Error::None;
}

Status Buffer::ShutDown() {

  if (pointer) {
    BufferLow::Delete(*(BufferLow **)&pointer);;
    pointer = NULL;
  }

  return Error::None;

}

Status Buffer::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Status Buffer::SetEmpty() {

  Status status;
  
  if (!BEFE::IsNull(BUF))
    status = BufferLow::Resize(pBUF,0);
  else
    status = Error::None;
  
  return status;

}

Status Buffer::MoveFrom(Buffer const &that) {
  
  if (pointer)
    ShutDown();
  pointer = that.pointer;
  ((Buffer *)&that)->pointer = NULL;
	
	// Note: We put this here to make sure it was okay to do thia.
	//       It had a very minimal impact on the Unit Tests (there
	//       were a few more allocations than before).
	//
	//       So, we'll leave it like this for now because it cleans
	//       up our declaration of what MoveFrom is supposed to do
	//       versus 'operator ='.
	//
  NotConsumable();
		
  return Error::None;
  
}

Status Buffer::CopyFrom(Buffer const &that) {
  
  Status     status;
  
  Reset();
  if (that.IsConsumable()) {
    ((Buffer *)this)->pointer = that.pointer;
    ((Buffer *)&that)->pointer = NULL;
  }
  else if (that.pointer) {
    status = BufferLow::CopyFrom(pBUF, *((BufferLow **)&that.pointer));
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                       break;
  }
  
  return status;
  
}

Buffer &Buffer::Consumable() const {

  if (pointer)
    BufferLow::Consumable(pBUF);
    
  return *(Buffer *)this;
  
}

Buffer &Buffer::NotConsumable() const {
  
  if (pointer)
    BufferLow::NotConsumable(pBUF);
    
  return *(Buffer *)this;
  
}

Status Buffer::SetSize(UInt newBytes) {
  
  Status status;
  
  if (pointer)
    status = BUF->SetSize(pBUF,newBytes);
  else { // Allocate Buffer...
    status = Reset();
    if (status) goto SOMEERROR;
    status = BufferLow::New(newBytes, UNaN, pBUF);
    if (status) goto SOMEERROR;
  } // ...Allocate Buffer
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
    
  return status;
  
}

Status Buffer::Resize(UInt newBytes) {

  Status status;
  
  if (pointer)
    status = BUF->Resize(pBUF,newBytes);
  else
    status = Error::None;
    
  return status;
  
}
  
Status Buffer::Allocate(UInt initBytes) {
  return Allocate(initBytes, UNaN);
}

Status Buffer::Allocate(UInt initBytes, UInt incBytes) {

  Status     status;
  
  if (!BEFE::IsNull(BUF))
    status = Error::BufferAlreadyAllocated;
  else
    status = BufferLow::New(initBytes, incBytes, pBUF);
  
  return status;

}

//----------------------------------------------------------------------
//
// Manipulating Byte contents
//

Status Buffer::GetBytes(UInt bIndex, Byte *thebytes, UInt bufMaxSize, UInt &usedSize) {
  
  Status status;
  
  if (pointer) {
    status = BUF->GetBytes(pBUF, bIndex, thebytes, bufMaxSize, usedSize);
  }
  else
    status = Error::BufferNotAllocated;
    
 return status;
    
}

Status Buffer::SetBytes(UInt bIndex, Byte *thebytes, UInt bufMaxSize, UInt &usedSize) {
  
  Status status;
  
  if (pointer) {
    status = BUF->SetBytes(pBUF, bIndex, thebytes, bufMaxSize, usedSize);
  }
  else
    status = Error::BufferNotAllocated;
    
 return status;
    
}

Status Buffer::AppendBytes(Byte *theBytes, UInt numBytes) {
  
  Status status;
  
  if (pointer) {
    status = BUF->AppendBytes(pBUF, theBytes, numBytes);
  }
  else
    status = Error::BufferNotAllocated;
    
 return status;
      
}

Status Buffer::InsertBytes(UInt bIndex, Byte *theBytes, UInt numBytes) {
  
  Status status;
  
  if (pointer) {
    status = BUF->InsertBytes(pBUF, bIndex, theBytes, numBytes);
  }
  else
    status = Error::BufferNotAllocated;
    
 return status;
      
}

Status Buffer::RemoveBytes(UInt bIndex, UInt numBytes) {
  
  Status status;
  
  if (pointer) {
    status = BUF->RemoveBytes(pBUF, bIndex, numBytes);
  }
  else
    status = Error::BufferNotAllocated;
    
 return status;
      
}

Status Buffer::ReplaceBytes(UInt bIndex, UInt numBytes, Byte *newBytes, UInt newSize) {
  
  Status status;
  
  if (pointer) {
    status = BUF->ReplaceBytes(pBUF, bIndex, numBytes, newBytes, newSize);
  }
  else
    status = Error::BufferNotAllocated;
    
 return status;
      
}

//----------------------------------------------------------------------
//
// Handy C++ Manipulation
//

Status Buffer::Append(char const *that) {
  return Append((char *)that);
}

Status Buffer::Append(char *that) {
  
  Status status;
  UInt   len;
  
  if (!that) goto OK;
  len = Strlen(that);
  if (GetAllocBytes() == 0) {
    status = Allocate(len);
    if (status) goto SOMEERROR;
  }
  status = AppendBytes((Byte *)that,len);
  
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Buffer info
//

UInt Buffer::Size() const {

  UInt answer;
  
  if (pointer)
    answer = BUF->Size();
  else
    answer = 0;
    
 return answer;
      
}

UInt Buffer::Length() const {

  UInt answer;
  
  if (pointer)
    answer = BUF->Length();
  else
    answer = UNaN;
    
 return answer;
      
}

Boolean Buffer::IsConsumable() const {

  Boolean answer;
  
  if (pointer)
    answer = BUF->IsConsumable();
  else
    answer = false;
    
 return answer;
      
}

UInt Buffer::GetAllocBytes() const {

  UInt answer;
  
  if (pointer)
    answer = BUF->GetAllocBytes();
  else
    answer = 0;
    
 return answer;
      
}

UInt Buffer::GetIncrementBytes() const {

  UInt answer;
  
  if (pointer)
    answer = BUF->GetIncrementBytes();
  else
    answer = UNaN;
    
 return answer;
      
}

UInt Buffer::GetUsedBytes() const {

  UInt answer;
  
  if (pointer)
    answer = BUF->GetUsedBytes();
  else
    answer = 0;
    
 return answer;
      
}

Status Buffer::SetUsedBytes(UInt newSize) {

  Status status;
  
  if (pointer)
    status = BUF->SetUsedBytes(newSize);
  else if (newSize != 0)
    status = Error::BufferNotAllocated;
  else
    status = Error::None;
    
 return status;
      
}

Byte *Buffer::GetBuf() const {

  Byte *answer;
  
  if (pointer)
    answer = BUF->GetBuf();
  else
    answer = NULL;
    
 return answer;
        
}

} // ...Namespace BEFE
