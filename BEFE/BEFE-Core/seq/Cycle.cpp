//----------------------------------------------------------------------
// File: Slice.cpp - Implementaion of the Slice class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Cycle.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Cycle::Cycle() {
  curPos = 0;
  curLen = UNaN;
}

Cycle::Cycle(UInt len) {
  curPos = 0;
  curLen = len;
}

Cycle::Cycle(Cycle const &that) {
  curPos = that.curPos;
  curLen = that.curLen;
}

Cycle::~Cycle() {
}

//----------------------------------------------------------------------
//
// C++ Casting
//

Cycle::operator UInt() const {
  
  return curPos;
  
}

Cycle::operator Int() const {
  
  return curPos;
  
}

//----------------------------------------------------------------------
//
// C++ Operators
//

UInt Cycle::operator = (Cycle const &that) {
  curPos = that.curPos;
  curLen = that.curLen;
  return curPos;
}

UInt Cycle::operator = (UInt pos) {
  
  if ( !IsNull(curLen) && curLen != 0 && pos >= curLen)
    curPos = pos % curLen;
  else
    curPos = pos;
    
  return curPos;

}

UInt Cycle::operator = (Int pos) {

  if ( !IsNull(curLen) && curLen != 0 && (UInt)pos >= curLen) {
    if (pos < 0)
      curPos = curLen + pos%(Int)curLen;
    else
      curPos = pos % (Int)curLen;
  }
  else if (pos < 0)
    curPos = 0;
  else
    curPos = pos;

  return curPos;

}

UInt Cycle::operator += (UInt val) {

  curPos = *this + val;

  return curPos;

}
  
UInt Cycle::operator + (UInt val) const {
  
  UInt newPos;
  
  newPos = curPos;
  if ( !IsNull(curLen) && curLen != 0)
    newPos = (newPos + val) % curLen;
    
  return newPos;
  
}

UInt Cycle::operator += (Int val) {

  curPos = *this + val;

  return curPos;

}
  
UInt Cycle::operator + (Int  val) const {
  
  Int newPos;
  
  newPos = curPos;
  if ( !IsNull(curLen) && curLen != 0) {
    newPos = (newPos + val)%(Int)curLen;
    if (newPos < 0)
      newPos = ((Int)curLen+newPos)%(Int)curLen;
  }

  return newPos;
  
}

UInt Cycle::operator -= (UInt val) {

  curPos = *this - val;
  
  return curPos;

}
  
UInt Cycle::operator - (UInt val) const {
  
  UInt newPos;
  
  newPos = curPos;
  if ( !IsNull(curLen) && curLen != 0) {
    val %= curLen;
    if (val > newPos)
      newPos = (val-newPos) + curLen;
    else
      newPos -= val;
  }
  
  return newPos;
  
}

UInt Cycle::operator -= (Int val) {

  curPos = *this - val;
  
  return curPos;

}
  
UInt Cycle::operator - (Int  val) const {
  
  Int newPos;
  
  newPos = curPos;
  if ( !IsNull(curLen) && curLen != 0) {
    val %= (Int)curLen;
    if (val < 0) {
      newPos += -val;
      newPos %= (Int)curLen;
    }
    else {
      newPos = (newPos-val) % (Int)curLen;
      if (newPos < 0)
        newPos += (Int)curLen;
    }
  }
  
  return newPos;

}

//----------------------------------------------------------------------
//
// C++ Prefix/Posfix operators
//

UInt Cycle::operator ++ () { // ◄── Prefix ++
  
  if ( !IsNull(curLen) && curLen != 0) {
    curPos++;
    if (curPos >= curLen)
      curPos = 0;
  }
  
  return curPos;
}

UInt Cycle::operator ++ (int) { // ◄── Postfix  ++

  UInt oldPos;

  oldPos = curPos;  
  if ( !IsNull(curLen) && curLen != 0) {
    curPos++;
    if (curPos >= curLen)
      curPos = 0;
  }
  
  return oldPos;

}

UInt Cycle::operator -- () { // ◄── Prefix ++
  
  if ( !IsNull(curLen) && curLen != 0) {
    if (curPos == 0)
      curPos = curLen-1;
    else
      curPos--;
  }
  
  return curPos;
  
}

UInt Cycle::operator -- (int) { // ◄── Postfix ++
  
  UInt oldPos;
  
  oldPos = curPos;
  if ( !IsNull(curLen) && curLen != 0) {
    if (curPos == 0)
      curPos = curLen-1;
    else
      curPos--;
  }
  
  return oldPos;
  
}

//----------------------------------------------------------------------
//
// Query/Adjustment
//

UInt Cycle::Length() const {
  return curLen;
}

UInt Cycle::Position() const {
  return curPos;
}

Cycle &Cycle::SetLength(UInt len) {
  curLen = len;
  return *this;
}

Cycle &Cycle::SetPosition(UInt pos) {
  curPos = pos;
  return *this;
}  

} // ...Namespace BEFE