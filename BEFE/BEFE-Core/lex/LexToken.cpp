//----------------------------------------------------------------------
// File: LexToken.cpp - Implementation of the BEFE LexToken Struct
//----------------------------------------------------------------------
//
// This class contains the C++ implementation of the BEFE LexToken
// Struct.  
//
// The purpose of a LexToken is to take a file, in a specified
// language, as input and to provide a stream of LexTokens as output.
//
// There's a bit more to it than that but that's the basic idea here.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Lex.h"
#include "LexToken.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

/* What it really should be...
LexToken::LexToken() {
  StartUp();
}
*/

// Slight optimisation...
LexToken::LexToken() :
  type(LexTokenType::Unknown),
  lineCount(0),
  status(0),
  value(),
  lineNo(UIntNaN),
  colNo(UIntNaN),
  span()
{
}
  
LexToken::LexToken(LexToken const &that) {
  ShutDown();
}

LexToken::~LexToken() {
  ShutDown();
}

LexToken &LexToken::operator=(LexToken const &that) {
	
  //Status status;
	
  /*status =*/ CopyFrom(that);
	
  return *this;

}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status LexToken::StartUp() {

  Status status;
  
  type = LexTokenType::Unknown;
  lineCount = 0;
  status = Error::None;
  status = value.StartUp();
  BEFE::SetNull(lineNo);
  BEFE::SetNull(colNo);
  BEFE::SetNull(span);
  
  return status;
	
}

Status LexToken::ShutDown() {
			
  Status status;
  
  status = value.ShutDown();
  
  return status;
	
}

Status LexToken::Clear() {
	
  type = LexTokenType::Unknown;
  span.idx1 = 0;
  span.idx2 = 0;
  
  return Error::None;
	
}

Status LexToken::Reset() {
	
  Status retStatus;
  Status status;
	
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
	
  return retStatus;
	
}

Boolean LexToken::IsNull() const {
  if (type == LexTokenType::Unknown && lineCount == 0 && BEFE::IsNull(span) )
    return true;
  return false;
}

Status LexToken::SetNull() {
  
  Status status;
  
  status = Reset();
  
  return status;
  
}

Boolean LexToken::IsEmpty() const {

  if (span.idx2 == span.idx1)
    return true;  
  return false;
  
}

Status LexToken::SetEmpty() {
  
  lineCount = 0;
  span.idx2 = span.idx1;

  return Error::None;

}

Status LexToken::CopyFrom(LexToken const &that) {

  type      = that.type;
  lineCount = that.lineCount;
  status    = that.status;
  value.ShutDown();
  value     = that.value;
  lineNo    = that.lineNo;
  colNo     = that.colNo;
  span      = that.span;

  return Error::None;  

}

Boolean LexToken::IsConsumable() const {
  return false;
}

LexToken &LexToken::Consumable() const {
  return *((LexToken *)this);
}

LexToken &LexToken::NotConsumable() const {
  return *((LexToken *)this);
}
  
UInt LexToken::Length() const {
  return span.Length();
}

UInt LexToken::Size() const {
  return span.Length();
}

} // ...Namespace BEFE
