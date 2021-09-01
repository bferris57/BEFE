//----------------------------------------------------------------------
// File: URange.cpp - Implementaion of the URange class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "URange.h"
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

URange::URange() {
  StartUp();
}

URange::URange(UInt index1) {
  idx1 = index1;
  idx2 = UNaN;
}

URange::URange(UInt index1, UInt index2) {
  idx1 = index1;
  idx2 = index2;
}

URange::URange(char const *str) {
  String sstr = str;
  StartUp();
  Set(sstr);
}

URange::URange(String const &str) {
  StartUp();
  ((Range *)this)->FromString(str);
}

URange::URange(URange const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
}

URange &URange::operator=(URange const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
  return *this;
}

//----------------------------------------------------------------------
//
// BEFE lifecycle
//

Status URange::StartUp() {

  idx1  = UNaN;
  idx2  = UNaN;

  return Error::None;
}

Boolean URange::IsNull() const {
  return (BEFE::IsNull(idx1) && BEFE::IsNull(idx2));
}

Boolean URange::IsEmpty() const {
  return (idx1 == idx2);
}

Status URange::SetEmpty() {
  if (BEFE::IsNull(idx1))
    idx2 = idx1;
  else
    idx2 = idx1;
  return Error::None;
}

Status URange::CopyFrom(URange const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
  return Error::None;
}

Status URange::MoveFrom(URange const &that) {
  idx1 = that.idx1;
  idx2 = that.idx2;
  ((Range *)&that)->StartUp();
  return Error::None;
}

UInt URange::Length() const {
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

UInt URange::Size() const {
  return 0;
}

//----------------------------------------------------------------------
//
// URange Setting
//

Status URange::Set() {
  Status status;
  status = StartUp();
  return status;
}

Status URange::Set(UInt index1) {

  idx1 = index1;
  idx2 = UNaN;

  return Error::None;

}

Status URange::Set(UInt index1, UInt index2) {

  idx1 = index1;
  idx2 = index2;

  return Error::None;

}

Status URange::Set(String &str) {

  Status status;

  StartUp();
  status = FromString(str);

  return status;

}

Status URange::Set(const char *str) {

  Status status;
  String string;

  string = str;
  StartUp();
  status = FromString(string);

  return status;

}

//----------------------------------------------------------------------
//
// URange Resolving
//

Boolean URange::IsResolved() const {
  if ( BEFE::IsNull(idx1) || BEFE::IsNull(idx2))
    return false;
  return true;
}

URange URange::Resolve(UInt len) const {

  URange that;

  that = *this;
  
  if ( BEFE::IsNull(len) ) goto DONE;
  
  if ( BEFE::IsNull(that.idx1) )
    that.idx1 = 0;
  if ( BEFE::IsNull(that.idx2) &&  !BEFE::IsNull(len) )
    that.idx2 = len-1;

  while (false) {
    DONE: break;
  }
  
  return that;

}

Boolean URange::InRange(UInt index) const {
  if ( BEFE::IsNull(index) )
    return false;
  if ( BEFE::IsNull(idx1) )
    return false;
  if ( BEFE::IsNull(idx2) )
    return true;
  if (idx1 > idx2) // Backward...
    return index >= idx2 && index <= idx1;
  return index >= idx1 && index <= idx2;
}

//----------------------------------------------------------------------
//
// String representation
//

String URange::ToString() {

  Byte   buf[128];
  char  *cp;
  String str;
  URange  saverange;

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
    if ( !BEFE::IsNull(idx2) )
      Itoa(idx2,buf+Strlen(buf),10);
  }

  while (false) {
    DONE: break;
  }

  str = (char *)buf;
  *this = saverange;

  return str;

}

String URange::ToHexString() {

  Byte   buf[128];
  String str;
  URange saverange;

  saverange = *this;

  buf[0] = 0;
  if ( !BEFE::IsNull(idx1) ) {
    Strcat(buf,"0x");
    Itoa(idx1,buf+Strlen(buf),16);
  }
  Strcat(buf,"..");
  if ( !BEFE::IsNull(idx2) ) {
    Strcat(buf,"0x");
    Itoa(idx2,buf+Strlen(buf),16);
  }

  str = (char *)buf;
  *this = saverange;

  return str;

}

Status URange::FromString(String &str) {

  Status status;

  status = Parse(str,*this,"..");

  return status;

}

Status URange::FromString(const char *str) {

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

Status URange::Parse(String const &str, URange &range, char const *sep) {

  Status  status;
  Byte    buf[23];
  UInt    len;
  Byte   *bp;
  UInt    sidx;
  UInt    eidx;
  UInt    termno;
  UInt    numterm;
  Int32   theint;
  Byte    esc;

  // Initialise URange members
  status = range.StartUp();
  if (status) goto SOMEERROR;

  // Get the string (it's easier than doing string manipulation)...
  status = str.ToBytes(buf,sizeof(buf)-1,len);
  if (status) goto SOMEERROR;
  buf[len] = 0x00;

  // Find the first double dot...
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
    theint = UNaN;
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
          if ( UTF8::GetCharSize(buf[sidx+1])+2 != eidx-sidx ) goto BADSTRING;
          theint = UTF8::GetChar(buf+sidx+1,len-sidx,0);
        }
        else {
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
          else // Nope, it's a "no digit" one
            theint = '\\';
        } // ... single character escape sequences
        
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
      range.idx1 = theint;
    else
      range.idx2 = theint;

  } // Term loop

  // If only one term, make the 2nd one the same as the first
  if (numterm == 1)
    range.idx2 = range.idx1;

  // Handle errors
  status = Error::None;
  while (false) {
    BADSTRING: status = Error::SeqRangeBadString; break;
    BADNUMBER: status = Error::SeqRangeBadNumber; break;
    SOMEERROR:                                    break;
  }

  return status;

}

} // ...Namespace BEFE
