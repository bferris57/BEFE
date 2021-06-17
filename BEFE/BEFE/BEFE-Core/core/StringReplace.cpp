//----------------------------------------------------------------------
// File: StringReplace.cpp - Implementation of the String.Replace Methods
//----------------------------------------------------------------------
//
// This is the implementation of the declaredBEFE String Remove Method.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Index Remove
//

Status String::Replace(Int index, String const &that) {

  Status status;
  
  if (index == (Int)Length())
    status = Append(that);
  else {
    status = Remove(index);
    if (!status)
      status = Insert(index, that);
  }
    
  return status;

}

Status String::Replace(Int index, Char that) {

  Status status;
  
  if (index == (Int)Length())
    status = Append(that);
  else {
    status = Remove(index);
    if (!status) {
      if (index == -1 || index == (Int)Length())
        status = Append(that);
      else
        status = Insert(index, that);
    }
  }
    
  return status;

}

Status String::Replace(Int index, char that) {
  
  Status status;
  
  if (index == (Int)Length())
    status = Append(that);
  else {
    status = Remove(index);
    if (!status) {
      if (index == -1 || index == (Int)Length())
        status = Append(that);
      else
        status = Insert(index, that);
    }
  }
    
  return status;
  
}

//----------------------------------------------------------------------
//
// Range Replace
//

Status String::Replace(Range const &range, String const &that) {

  Status status;
  Range  resRange;
  Int    index;

  resRange = range.Resolve(Length());  
  status = Remove(range);
  if (!status) {
    index = (resRange.IsAscending()) ? resRange.idx1 : resRange.idx2;
    if (index == (Int)Length())
      status = Append(that);
    else
      status = Insert(index, that);
  }
 
  return status;

}

Status String::Replace(Range const &range, Char that) {

  Status status;
  Range  resRange;
  Int    index;

  resRange = range.Resolve(Length());  
  status = Remove(range);
  if (!status) {
    index = (resRange.IsAscending()) ? resRange.idx1 : resRange.idx2;
    if (index == (Int)Length())
      status = Append(that);
    else
      status = Insert(index, that);
  }
 
  return status;

}

Status String::Replace(Range const &range, char that) {

  Status status;
  Range  resRange;
  Int    index;

  resRange = range.Resolve(Length());  
  status = Remove(range);
  if (!status) {
    index = (resRange.IsAscending()) ? resRange.idx1 : resRange.idx2;
    if (index == (Int)Length())
      status = Append(that);
    else
      status = Insert(index, that);
  }
 
  return status;

}

//----------------------------------------------------------------------
//
// Span Replace
//

Status String::Replace(Span const &span, String const &that) {

  Status status;
  Span   resSpan;
  Int    index;

  resSpan = span.Resolve(Length());  
  status = Remove(span);
  if (!status) {
    index = (resSpan.IsAscending()) ? resSpan.idx1 : resSpan.idx2;
    if (index == (Int)Length())
      status = Append(that);
    else
      status = Insert(index, that);
  }

  return status;

}

Status String::Replace(Span const &span, Char that) {

  Status status;
  Span   resSpan;
  Int    index;

  resSpan = span.Resolve(Length());  
  status = Remove(span);
  if (!status) {
    index = (resSpan.IsAscending()) ? resSpan.idx1 : resSpan.idx2;
    if (index == (Int)Length())
      status = Append(that);
    else
      status = Insert(index, that);
  }

  return status;

}

Status String::Replace(Span const &span, char that) {

  Status status;
  Span   resSpan;
  Int    index;

  resSpan = span.Resolve(Length());
  status = Remove(span);
  if (!status) {
    index = (resSpan.IsAscending()) ? resSpan.idx1 : resSpan.idx2+1;
    if (index == (Int)Length())
      status = Append(that);
    else
      status = Insert(index, that);
  }
  
  return status;

}
  
} // ...Namespace BEFE