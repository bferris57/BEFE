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
#include "Slice.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Slice::Slice() {
  Set();
}

Slice::Slice(Int index1) {
  Set(index1);
}

Slice::Slice(Int index1,Int index2) {
  Set(index1,index2);
}

Slice::Slice(Int index1,Int index2,Int step) {
  Set(index1,index2,step);
}

Slice::Slice(const char *str) {
  Set();
  FromString(str);
}

Slice::Slice(Range const &range) {
  Set(range);
}

Slice::Slice(URange const &range) {
  Set(range);
}

Slice::Slice(Span const &span) {
  Set(span);
}

Slice::Slice(USpan const &span) {
  Set(span);
}

Slice::Slice(String const &str) {
  Set();
  FromString(str);
}

Slice::~Slice() {}

//----------------------------------------------------------------------
//
// Setting
//
//----------------------------------------------------------------------

void Slice::Set() {
  idx1 = idx2 = step = NaN;
}

void Slice::Set(Int index1) {
  idx1 = index1;
  idx2 = step = NaN;
}

void Slice::Set(Int index1,Int index2) {
  idx1 = index1;
  idx2 = index2;
  step = NaN;
}

void Slice::Set(Int index1,Int index2,Int sstep) {
  idx1 = index1;
  idx2 = index2;
  step = sstep;
}

void Slice::Set(Range range) {
  idx1 = range.idx1;
  idx2 = range.idx2+1;
  step = (range.idx2 < range.idx1) ? -1 : 1;
}

void Slice::Set(URange range) {
  idx1 = range.idx1;
  idx2 = (range.idx2 < range.idx1) ? range.idx2-1 : range.idx2+1;
  step = (range.idx2 < range.idx1) ? -1 : 1;
}

void Slice::Set(Span span) {
  idx1 = span.idx1;
  idx2 = span.idx2;
  step = (span.idx2 < span.idx1) ? -1 : 1;
}

void Slice::Set(USpan span) {
  idx1 = span.idx1;
  idx2 = span.idx2;
  step = (span.idx2 < span.idx1) ? -1 : 1;
}

//----------------------------------------------------------------------
//
// Resolving
//
//----------------------------------------------------------------------

Slice Slice::Resolve(UInt len) const {

  Slice  resolved;

  // Normalise step
  if ( IsNaN(step) )
    resolved.step = 1;
  else
    resolved.step = step;

  // Normalise Start
  if ( IsNaN(idx1) ) {
    if (resolved.step < 0)
      resolved.idx1 = len-1;
    else
      resolved.idx1 = 0;
  }
  else {
    resolved.idx1 = idx1;
    if (resolved.idx1 < 0)
      resolved.idx1 = len+resolved.idx1;
    if (resolved.idx1 > (Int)len)
      resolved.idx1 = len;
  }
  if (resolved.idx1 < 0)
    resolved.idx1 = NaN;

  // Normalise Finish
  if ( IsNaN(idx2) ) {
    if (resolved.step < 0)
      resolved.idx2 = -1;
    else
      resolved.idx2 = len;
  }
  else {
    resolved.idx2 = idx2;
    if (resolved.idx2 < 0)
      resolved.idx2 = len+resolved.idx2;
  }
  if (resolved.idx2 < 0 || (UInt)resolved.idx2 > len) {
    if (resolved.step < 0)
      resolved.idx2 = -1;
    else
      resolved.idx2 = len;
  }
  // If Start is NaN, clear the rest
  if ( IsNaN(resolved.idx1) )
    resolved.idx2 = resolved.step = NaN;

  return resolved;

}

UInt Slice::Length() const {

  UInt len;

  if ( IsNaN(idx1) || IsNaN(idx1) || IsNaN(step) )
    return UNaN;
  if (idx1-idx2 == 0 || step == 0)
    return 0;

  if (idx2 < idx1)
    len = idx1 - idx2;
  else
    len = idx2 - idx1;

  len = len / step;

  return len;
}

//----------------------------------------------------------------------
//
// String representation
//
//----------------------------------------------------------------------

String Slice::ToString() const {

  Byte    buf[256];
  Byte   *cp;
  String  status;

  Strcpy(buf,"[");
  cp = buf+Strlen(buf);
  if ( !IsNull(idx1) ) {
    Itoa(idx1,cp,10);
    cp += Strlen(cp);
  }
  if ( IsNaN(idx1) && IsNaN(idx2) && IsNaN(step) ) goto ENDBRACKET;
  *cp++ = ':';
  if ( IsNaN(idx2) || IsNaN(step) ) {
    if ( !IsNaN(idx2) ) {
      Itoa(idx2,cp,10);
      cp += Strlen(cp);
    }
    if (!IsNull(step) ) {
      *cp++ = ':';
      Itoa(step,cp,10);
      cp += Strlen(cp);
    }
  }
  *cp = 0;

ENDBRACKET:

  Strcat(buf,"]");

  status.Append((Byte *)buf,Strlen(buf));

  return status;

}

Status Slice::FromString(String const &str) {

  Status status;
  Slice  that;

  status = Parse(str,that,'[',']');
  if (status == Error::None)
    *this = that;

  return status;

}

Status Slice::FromString(const char *str) {

  Status status;
  String string;
  Slice  that;

  string = str;
  status = Parse(string,that,'[',']');
  if (status == Error::None)
    *this = that;

  return status;

}

//----------------------------------------------------------------------
//
// String parsing
//

Status Slice::Parse(String const &str, Slice &outslice, Char sdelim, Char edelim) { // Slice.Parse...

  Status status;
  Int    ints[3];
  Int    intno;

  Int    start;
  Int    end;
  Int    istart;
  Int    iend;
  Slice  islice;
  Char   c;

  // Initialise
  end = str.Length();
  ints[0] = NaN;
  ints[1] = NaN;
  ints[2] = NaN;

  // Skip over leading/trailing whitespace
  for (start=0; start < end; start++) {
    c = str.Get(start);
    if (c > ' ') break;
  }
  while (end > 1) {
    c = str.Get(end-1);
    if (c > ' ') break;
    end--;
  }

  // Nothing to do if no string
  if (end <= start) goto OK;

  // Skip over leading/trailing '(' and ')'...
  if (start < end && end >= 2) {
    c = str.Get(start);
    if (c == sdelim) {
      start++;
      c = str.Get(end-1);
      if (c != edelim) goto BADSTRING;
      end--;
    }
  }

  // Nothing to do if no string
  if (end <= 0) goto OK;

  //
  // For each number separated by ':'...
  //

  for (intno=0; intno < 3; intno++) {

    // Initialise number range
    istart = start;
    iend   = end;

    // Skip over leading whitespace
    while (istart < iend) {
      c = str.Get(istart);
      if (c > ' ') break;
      istart++;
    }

    // Skip until ':' or end
    iend = istart;
    while (iend < end) {
      c = str.Get(iend);
      if (c == ':')
        break;
      iend++;
    }

    // Adjust for next time around...
    if (istart >= end) break;
    start = iend+1;

    // Skip over trailing whitespace
    while ((iend-istart) > 1) {
      c = str.Get(iend-1);
      if (c > ' ') break;
      iend--;
    }

    // If no number, continue to next one
    if (istart >= iend) continue;

    // Process the number
    islice = Slice(istart,iend,1);
    status = Int32FromString(str, islice, ints[intno]);
    if (status) goto SOMEERROR;

    // If first number specified and no separators, let's treat it as an index
    if (intno == 0 && iend >= end) {
      if (ints[0] >= 0) {
        ints[1] = ints[intno]+1;
        ints[2] = 1;
      }
      else {
        ints[1] = ints[intno]-1;
        ints[2] = -1;
      }
      break;
    }

  }

  // Make sure we're finished with all of string...
  if (start < end) goto BADSTRING;
  
  // Handle errors
  status = Error::None;
  while (false) {
    BADSTRING: status = Error::SeqSliceBadString; break;
    OK:        status = Error::None;              break;
    SOMEERROR:                                    break;
  }

  if (!status) {
    outslice.idx1 = ints[0];
    outslice.idx2 = ints[1];
    outslice.step = ints[2];
  }

  return status;

} // ...Slice.Parse

} // ...Namespace BEFE
