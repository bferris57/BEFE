//----------------------------------------------------------------------
// File: LexState.cpp - Implementation of the BEFE LexState Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "ToStrings.h"

#include "LexState.h"
#include "Lex.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

LexState::LexState() {
  StartUp();
}

LexState::LexState(LexState const &that) {
  StartUp();
  CopyFrom(that);
}

LexState::~LexState() {
  ShutDown();
}

LexState &LexState::operator=(LexState const &that) {
  StartUp();
  CopyFrom(that);
  return *this;
}

//----------------------------------------------------------------------
//	
// BEFE Lifecycle
//

Status LexState::StartUp() {

  Status retStatus;
  Status status;
  
  BEFE::SetNull(id);
  retStatus = name.StartUp();
  type = TypeUnknown;
  status = ranges.StartUp();
  if (status && !retStatus) retStatus = status;
  MemoryFillRaw(&byteActions[0], (Byte)Lex::ActionUnhandled, sizeof(byteActions));
  
  return retStatus;

}

Status LexState::ShutDown() {

  Status retStatus;
  Status status;

  retStatus = name.ShutDown();
  status = ranges.ShutDown();
  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status LexState::Clear() {
  MemoryFillRaw(&byteActions[0], Lex::ActionUnhandled, sizeof(byteActions));
  return Error::None;
}  

Status LexState::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Boolean LexState::IsNull() const {
  
  LexState nullState;
  Int      result;

  result = MemoryCompareRaw((Byte *)&nullState.byteActions[0], (Byte *)&byteActions[0], sizeof(byteActions));
  
  return result == 0 && name.IsNull() && BEFE::IsNull(id);
  
}

Status LexState::SetNull() {
  return Reset();
}

Boolean LexState::IsEmpty() const {
  
  Int      result;
  LexState nullState;
  
  result = MemoryCompareRaw((Byte *)&nullState.byteActions[0], (Byte *)&byteActions[0], sizeof(byteActions));
  
  return result == 0;

}

Status LexState::SetEmpty() {
  
  LexState nullState;
  
  MemoryCopyRaw((Byte *)&nullState.byteActions[0], (Byte *)&byteActions[0], sizeof(byteActions));
  
  return Error::None;
  
}

Boolean LexState::IsConsumable() const {
  return name.IsConsumable();
}

LexState &LexState::Consumable() {
  name.Consumable();
  return *this;
}

LexState &LexState::NotConsumable() {
  name.NotConsumable();
  return *this;
}

Status LexState::MoveFrom(LexState const &that) {

  Status status;

  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(LexState));
  status = ((LexState *)&that)->StartUp();
  NotConsumable();
  
  return status;
  
}

Status LexState::CopyFrom(LexState const &that) {

  Status retStatus;
  Status status;
  
  retStatus = Error::None;
  if (this != (LexState *)&that) {
    if (IsConsumable())
      retStatus = MoveFrom(that);
    else {
      id = that.id;
      retStatus = name.CopyFrom(that.name);
      type = that.type;
      MemoryCopyRaw((Byte *)&that.byteActions[0], (Byte *)&byteActions[0], sizeof(byteActions));
      status = ranges.CopyFrom(that.ranges);
      if (status && !retStatus) retStatus = status;
    }
  }

  return retStatus;
  
}

//----------------------------------------------------------------------
//
// Member Access
//

String LexState::GetName() const {
  
  String theName;
  
  theName = name;
  
  return theName.Consumable();
  
}
  
Status LexState::SetName(String const &newName) {
  
  Status status;
  
  status = name.CopyFrom(newName);
  
  return status;

}

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Status LexState::ToStrings(Strings &strings) { // LexState.ToStrings...

  Status  status;
  String  leadIn;
  String  leadOut;
  String  str;
  Strings memStrings;
  UInt    i;
  
  status = strings.SetEmpty();
  if (status) goto SOMEERROR;
  
  leadIn  = GetEnvironmentValue("BEFE_QuoteLeadIn");
  leadOut = GetEnvironmentValue("BEFE_QuoteLeadOut");

  status = strings.Append(String("LexState @ 0x") << ToHexString((PtrInt)&byteActions[0], 8) << "...");
  if (status) goto SOMEERROR;
  
  if (IsNull()) {
    status = strings.Append("  IsNull()  = True");
    if (status) goto SOMEERROR;
  }
  else {
    status = strings.Append(String("  IsNull()  = False\n"));
    if (status) goto SOMEERROR;
    status = strings.Append(String("  IsEmpty() = ") << ((IsEmpty())?CONSTR_True:CONSTR_False));
    if (status) goto SOMEERROR;
    status = strings.Append(String("  name      = '") << name << '\'');
    if (IsEmpty()) {
      status = strings.Append(String("  actions   = ***Empty***"));
      if (status) goto SOMEERROR;
    }
    else {
      status = strings.Append(String("  actions   = ..."));
      if (status) goto SOMEERROR;
      status = BEFE::ToStrings((Byte *)&byteActions[0], sizeof(byteActions), /*(UInt)&ctions[0]*/0, memStrings);
      if (status) goto SOMEERROR;
      for (i=0; i < memStrings.Length(); i++)
        strings.Append(( String(' ')*4 + memStrings.Get(i) ).Consumable());
    }
  }

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
} // ...LexState.ToStrings

//----------------------------------------------------------------------
//
// Range Management
//

Status LexState::PopulateRangesFromBytes() { // LexState.PopulateRangesFromBytes...
  
  Status   status;
  Byte     prevByte;
  Byte     curByte;
  LexRange range;
  UInt     i;
  
  // Clear the ranges first...
  status = ranges.Reset();
  if (status) goto SOMEERROR;
  
  // Initialise the range
  range.range.idx1 = 0;
  range.range.idx2 = UInt32NaN;
  range.actionNo   = byteActions[0];
  prevByte         = byteActions[0];
  
  // For each byte in byteActions...
  for (i=0; i < sizeof(byteActions); i++) {

    curByte = byteActions[i];

    if (curByte == prevByte)
      continue;

    if (range.actionNo != Lex::ActionUnhandled) {
      range.range.idx2 = i - 1;
      status = ranges.Append(range);
      if (status) goto SOMEERROR;
    }
    
    range.range.idx1 = i;
    BEFE::SetNull(range.range.idx2);
    range.actionNo   = curByte;
    prevByte = curByte;
    
  }

  // Append the last range  
  if (BEFE::IsNull(range.range.idx2) && range.actionNo != Lex::ActionUnhandled) {
    range.range.idx2 = i-1;
    status = ranges.Append(range);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
} // ...LexState.PopulateRangesFromBytes

Byte LexState::GetCharActionNo(Char c) { // LexState.GetCharActionNo...

  Byte theAction;
  UInt i;
  
  // Denormalise the pointers
  numRanges = ranges.Length();
  range0 = (LexRange *)ranges._GetAddress(0);
  
  // Only if valid...
  theAction = Lex::ActionUnhandled;
  if (!BEFE::IsNull(range0) && IsUnicode(c)) {
    for (i=0; i < numRanges; i++) {
      if (c >= range0[i].range.idx1 && c <= range0[i].range.idx2) {
        theAction = range0[i].actionNo;
        break;
      }
    }
  }
  
  return theAction;
  
} // ..LexState.GetCharActionNo

} // ...Namespace BEFE
