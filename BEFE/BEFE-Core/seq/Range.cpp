//----------------------------------------------------------------------
// File: Range.cpp - Implementation of the Range class
//----------------------------------------------------------------------
//
// This is the implementation of the BEFE Range Class.
//
// In short, Ranges are signed integer Ranges that are used to indicate
// an Inclusive Range of Elements or "thingies".
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Range.h"
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Range::Range() {
  StartUp();
}

Range::Range(Int index1) {
  idx1 = index1;
  idx2 = NaN;
}

Range::Range(Int index1, Int index2) {
  idx1 = index1;
  idx2 = index2;
}

Range::Range(char const *str) {
  String sstr = str;
  StartUp();
  Set(sstr);
}

Range::Range(String const &str) {
  StartUp();
  ((Range *)this)->FromString(str);
}

Range::Range(Range const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
}

Range &Range::operator=(Range const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
  return *this;
}

//----------------------------------------------------------------------
//
// BEFE lifecycle
//

Status Range::StartUp() {

  idx1  = NaN;
  idx2  = NaN;

  return Error::None;
}

Boolean Range::IsNull() const {
  return (BEFE::IsNull(idx1) && BEFE::IsNull(idx2));
}

Boolean Range::IsEmpty() const {
  return (idx1 == idx2);
}

Status Range::SetEmpty() {
  if (BEFE::IsNull(idx1))
    idx2 = idx1;
  else
    idx2 = idx1;
  return Error::None;
}

Status Range::CopyFrom(Range const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
  return Error::None;
}

Status Range::MoveFrom(Range const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
  ((Range *)&that)->StartUp();
  return Error::None;
}

UInt Range::Length() const {
  if (!IsResolved())
    return 0;
  if ( BEFE::IsNull(idx1) || BEFE::IsNull(idx2) )
    return UNaN;
  if (idx2 == idx1-1)
    return 0;
  if (idx2 < idx1)
    return (idx1-idx2) + 1;
  else
    return (idx2-idx1) + 1;
}

UInt Range::Size() const {
  return 0;
}

//----------------------------------------------------------------------
//
// Range Setting
//

Status Range::Set() {
  Status status;
  status = StartUp();
  return status;
}

Status Range::Set(Int index1) {

  idx1 = index1;
  idx2 = NaN;

  return Error::None;

}

Status Range::Set(Int index1, Int index2) {

  idx1 = index1;
  idx2 = index2;

  return Error::None;

}

Status Range::Set(String const &str) {

  Status status;

  status = FromString(str);

  return status;

}

Status Range::Set(const char *str) {

  Status status;
  String string;

  string = str;
  status = FromString(string);

  return status;

}

//----------------------------------------------------------------------
//
// Range Resolving
//

Boolean Range::IsResolved() const {
  if ( BEFE::IsNull(idx1) || BEFE::IsNull(idx2) || idx1 < 0 || idx2 < 0)
    return false;
  return true;
}

Range Range::Resolve(UInt len) const {

  Range that;

  that = *this;

  if ( BEFE::IsNull(len) ) goto DONE;
  
  if ( BEFE::IsNull(that.idx1) )
    that.idx1 = 0;
  if ( BEFE::IsNull(that.idx2) )
    that.idx2 = len-1;
  if (that.idx1 < 0) that.idx1 = that.idx1 + len;
  if (that.idx2 < 0) that.idx2 = that.idx2 + len;
  if (that.idx1 < 0)
    that.idx1 = 0;
  if (that.idx2 < 0)
    that.idx2 = -1;
  if (that.idx2 >= (Int)len)
    that.idx2 = (Int)len-1;

  while (false) {
    DONE: break;
  }
  
  return that;

}

Boolean Range::InRange(Int index) const {
  
  UInt len;
  len = Length();
  if ( BEFE::IsNull(index) || BEFE::IsNull(len) || BEFE::IsNull(idx1) ) 
    return false;
  if ( BEFE::IsNull(idx2) )
    return true;
  if (index < 0)
    index = index + len;
  if (index < 0 || (UInt)index >= len)
    return false;
  if (idx1 > idx2) // Backward...
    return index >= idx2 && index <= idx1;
  return index >= idx1 && index <= idx2;
}

//----------------------------------------------------------------------
//
// Resolved Range Order
//
  
Boolean Range::IsAscending() const {
  if (!IsResolved()) 
    return false;
  return (idx1 <= idx2);
}

Boolean Range::IsDescending() const {
  if (!IsResolved()) 
    return false;
  return (idx2 < idx1);
}

Status  Range::Reverse() {

  if (!IsResolved())
    return Error::SeqRangeNotResolved;

  idx1 ^= idx2;
  idx2 ^= idx1;
  idx1 ^= idx2;

  return Error::None;

}
  
//----------------------------------------------------------------------
//
// String representation
//
//----------------------------------------------------------------------

String Range::ToString() {

  Byte   buf[128];
  char  *cp;
  String str;
  Range  saverange;

  saverange = *this;

  if (idx1 >= 0x20 && idx1 <= 0x7f && idx2 >= 0x20 && idx2 <= 0x7f) {
    cp = (char *)buf;
    *cp++ = '\'';
    *cp++ = idx1;
    *cp++ = '\'';
    *cp++ = '.';
    *cp++ = '.';
    *cp++ = '\'';
    *cp++ = idx2;
    *cp++ = '\'';
    *cp++ = 0x00;
    goto DONE;
  }
  else {
    buf[0] = 0;
    if ( !BEFE::IsNull(idx1) )
      Itoa(idx1,buf+Strlen(buf),10);
    Strcat(buf,"..");
    if (!BEFE::IsNull(idx2) )
      Itoa(idx2,buf+Strlen(buf),10);
  }

  while (false) {
    DONE: break;
  }

  str = (char *)buf;
  *this = saverange;

  return str;

}

String Range::ToHexString() {

  Byte   buf[128];
  String str;
  Range  saverange;

  saverange = *this;

  buf[0] = 0;
  if (!BEFE::IsNull(idx1) ) {
    Strcat(buf,"0x");
    Itoa(idx1,buf+Strlen(buf),16);
  }
  Strcat(buf,"..");
  if (!BEFE::IsNull(idx2) ) {
    Strcat(buf,"0x");
    Itoa(idx2,buf+Strlen(buf),16);
  }

  str = (char *)buf;
  *this = saverange;

  return str;

}

Status Range::FromString(String const &str) {

  Status status;

  status = Parse(str,*this,"..");

  return status;
}

Status Range::FromString(const char *str) {

  Status status;
  String string;

  string = str;
  status = Parse(string,*this,"..");

  return status;
}

//----------------------------------------------------------------------
//
// String representation
//
//----------------------------------------------------------------------

Status Range::Parse(String const &str,Range &outRange, char const *sep) {

  Status  status;
  Byte    buf[23];
  UInt    len;
  Byte   *bp;
  Int     sidx;
  Int     eidx;
  Int     termno;
  Int     numterm;
  Int     theint;
  Byte    esc;

  // Initialise Range members
  status = outRange.StartUp();
  if (status) goto SOMEERROR;

  // Get the string (it's easier than doing string manipulation)...
  status = str.ToBytes(buf,sizeof(buf)-1,len);
  if (status) goto SOMEERROR;
  buf[len] = 0x00;

  // Find the first step...
  bp = buf;
  numterm = 1;
  while ((UInt)(bp-buf) < len) {
    if (*bp == *sep && (!*(sep+1) || *(bp+1) == *(sep+1))) {
      numterm++;
      break;
    }
    bp++;
  }

  //
  // Get terms...
  //

  for (termno=0;termno<numterm;termno++) {

    // Get the byte range
    if (termno == 0) {
      sidx = 0;
      eidx = bp-buf;
    }
    else {
      sidx = (bp-buf)+Strlen(sep);
      eidx = len;
    }

    // Get the Term
    theint = NaN;
    if (eidx-sidx > 0) {

      //
      // Escape sequences...
      //
      // Note: Convenience code to make it easier for C/C++ coders
      //       and other people who didn't read my documentation.
      //       These guys can't get their head around the fact that
      //       A SINGLE CHARACTER IS A SINGLE CHARACTER, so a
      //       character literal indicates ONE SINGLE CHARACTER.
      //
      //       So, since it's a single character ''' means the
      //       apostrophe character and '"' means a double quote
      //       character and '\' means a backslash character.
      //
      //       So, since there's no ambiguity in semantics, you
      //       don't need escape sequences when specifying a value
      //       range.  ;-)
      //
      // Note: In fact, I just may comment this out soon, so beware!!!
      //
      // Note: Notice how much more code it takes to handle the C/C++
      //       legacy thinking than the 'else' part of this 'if' statement
      //       that just handles numbers?????!!!!
      //
      //       And all this just because of the user's state of mind.
      //

      if (buf[sidx] == '\'' && buf[eidx-1] == '\'') { // Single quoted terms...
        if (eidx-sidx < 3) goto BADSTRING;
        // Non escape sequences (you can tell by the length...
        if (buf[sidx+1] != '\\') {
          // Not escape sequence, must single UTF-8...
          if ( UTF8::GetCharSize(buf[sidx+1])+2 != (UInt)(eidx-sidx) ) goto BADSTRING;
          theint = UTF8::GetChar(buf+sidx+1,len-sidx,0);
        }
        else { // Single character escape sequences...          
          sidx += 2;
          esc = buf[sidx];
          // If single digit ones...
          if (eidx-sidx == 2) {
            switch(esc) {
              case 'b':  theint = 0x08; break;
              case 'f':  theint = 0x0C; break;
              case 'n':  theint = 0x0A; break;
              case 'r':  theint = 0x0D; break;
              case 't':  theint = 0x09; break;
              case '\\': theint = esc;  break;
              default:
                goto BADSTRING;
            }
          }
          else  // Nope, it's a "no digit" one
            theint = '\\';

        } // ... Single character escape sequences
        
      } // ... Single quoted terms

      //
      // Proper integers...
      //
      else {

        if (eidx-sidx <= 0) goto BADNUMBER;
        status = Int32FromString(buf+sidx, eidx-sidx, Slice(), theint);
        if (status) goto SOMEERROR;

      }

    } // Non-blank term

    // Set the term
    if (termno == 0)
      outRange.idx1 = theint;
    else
      outRange.idx2 = theint;

  } // Term loop

  // If only one term, make the 2nd one the same as the first
  if (numterm == 1)
    outRange.idx2 = outRange.idx1;

  // Handle errors
  while (false) {
    SOMEERROR:                                    break;
    BADSTRING: status = Error::SeqRangeBadString; break;
    BADNUMBER: status = Error::SeqRangeBadNumber; break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Commonly used Ranges
//
//----------------------------------------------------------------------

Range ControlCharRange    = Range(0x00,0x1f);
Range ASCIIRange          = Range(0x00,0xff);
Range PrintableASCIIRange = Range(0x20,0x7f);
Range SByteRange          = Range(0x81,0xFE);
Range ByteRange           = Range(0x81,0xFE);
Range UByteRange          = Range(0x00,0xFF);
Range ShortRange          = Range(0x8001,0x7fff);
Range CharRange           = Range(0x000000,0x10FFFF);
Range SIntRange           = Range(0x80000001,0x7fffffff);
Range IntRange            = Range(0x80000001,0x7fffffff);
Range UIntRange           = Range(0x00000000,0xffffffff);

Boolean IsControlCharRange(Int value)    {return ControlCharRange.InRange(value); }
Boolean IsASCIIRange(Int value)          {return ASCIIRange.InRange(value); }
Boolean IsPrintableASCIIRange(Int value) {return PrintableASCIIRange.InRange(value); }
Boolean IsSByteRange(Int value)          {return SByteRange.InRange(value); }
Boolean IsByteRange(Int value)           {return ByteRange.InRange(value); }
Boolean IsUByteRangeRange(Int value)     {return UByteRange.InRange(value); }
Boolean IsShortRange(Int value)          {return ShortRange.InRange(value); }
Boolean IsCharRange(Int value)           {return CharRange.InRange(value); }
Boolean IsIntRange(Int value)            {return UIntRange.InRange(value); }

} // ...Namespace BEFE
