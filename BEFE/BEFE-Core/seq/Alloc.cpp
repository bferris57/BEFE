//----------------------------------------------------------------------
// File: Alloc.cpp - Implementaion of the Alloc Class
//----------------------------------------------------------------------
//
// The Alloc Class us used to indicate fairly common Storage Allocation
// behaviour in terms of an InitElms, MaxElms, IncElms, and DecElms.
//
// Note: An Alloc intended to identify Logical Elements NOT Physical
//       Storage Elements.  Having said that, an Alloc can be used to
//       maintain Physical Storage Element allocation but should only
//       used that way carefully, and if this usage is clearly expressed
//       in the related usage context.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Alloc.h"

namespace BEFE { // Namespace BEFE...

// Handy #defines
#define DEFAULT_MIN 0
#define DEFAULT_INC 16
#define DEFAULT_DEC 16

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Alloc::Alloc() {
  StartUp();
}

Alloc::Alloc(const char *str) {
  String sstr = str;
  StartUp();
  Parse(sstr);
}

Alloc::Alloc(String const &str) {
  StartUp();
  Parse(str);
}

Alloc::Alloc(UInt fixedElms) {
  StartUp();
  minElms = fixedElms;
  maxElms = fixedElms;
  incElms = 0;
  decElms = 0;
}

Alloc::Alloc(UInt initElms, UInt incElms) {
  StartUp();
  this->minElms = initElms;
  BEFE::SetNull(this->maxElms);
  this->incElms = incElms;
}

Alloc::~Alloc() {
}

//----------------------------------------------------------------------
//
// BEFE lifecycle
//

Status Alloc::StartUp() {
  return SetClosed();
}

Boolean Alloc::IsNull() const {
  return (minElms    == 0 &&
          maxElms    == 0 &&
          incElms    == 0 &&
          incElmsPct == 0 &&
          decElms    == 0 &&
          decElmsPct == 0);
}

Status Alloc::SetNull() {

  minElms    = 0;
  maxElms    = 0;
  incElms    = 0;
  incElmsPct = 0;
  decElms    = 0;
  decElmsPct = 0;
  
  return Error::None;

}

Boolean Alloc::IsEmpty() const {
  return IsNull();
}

Status Alloc::SetEmpty() {
  return SetNull();
}

Status Alloc::CopyFrom(Alloc const &that) {
  *this = that;
  return Error::None;
}

Status Alloc::MoveFrom(Alloc const &that) {
  *this = that;
  ((Alloc *)&that)->StartUp();
  return Error::None;
}

//----------------------------------------------------------------------
//
// Querying
//
  
  UInt    Alloc::MinElms()    {return minElms;}
  UInt    Alloc::MaxElms()    {return maxElms;}
  UInt    Alloc::IncElms()    {return (incElms==0x7fff)?UNaN:incElms;}
  Boolean Alloc::IncElmsPct() {return incElmsPct==0;}
  UInt    Alloc::DecElms()    {return (decElms==0x7fff)?UNaN:decElms;}
  Boolean Alloc::DecElmsPct() {return decElmsPct==0;}
  
//----------------------------------------------------------------------
//
// Normalising
//

Alloc Alloc::Normalise() const {
  
  Alloc that;
  
  that = *this;
  if ( BEFE::IsNull(that.minElms) )   that.minElms = DEFAULT_MIN;
  //if ( IsNull(that.maxElms)     that.maxElms = that.minElms;
  if (that.incElms == 0x7fff) that.incElms = DEFAULT_INC;
  if (that.decElms == 0x7fff) that.decElms = DEFAULT_DEC;

  return that;

}

Alloc Alloc::Normalise(UInt elmSize) const {
  
  Alloc that;
  
  that = *this;
  if ( BEFE::IsNull(that.minElms) ) that.minElms = DEFAULT_MIN;
  //if ( IsNull(that.maxElms) )   that.maxElms = that.minElms;
  if (that.incElms == 0x7fff) that.incElms = DEFAULT_INC;
  if (that.decElms == 0x7fff) that.decElms = DEFAULT_DEC;
  that.minElms *= elmSize;
  if ( !BEFE::IsNull(that.maxElms) )
    that.maxElms *= elmSize;
  if (!that.incElmsPct) that.incElms *= elmSize;
  if (!that.decElmsPct) that.decElms *= elmSize;

  return that;

}

Alloc Alloc::Normalise(UInt elmSize, UInt initElms) const {

  Alloc that;

  that = *this;
  if ( BEFE::IsNull(that.minElms) ) that.minElms = initElms;
  //if ( IsNull(that.maxElms) )   that.maxElms = that.minElms;
  if (that.incElms == 0x7fff) that.incElms = DEFAULT_INC;
  if (that.decElms == 0x7fff) that.decElms = DEFAULT_DEC;
  if ( !BEFE::IsNull(that.maxElms) )
    that.maxElms *= elmSize;
  that.maxElms *= elmSize;
  if (!that.incElmsPct) that.incElms *= elmSize;
  if (!that.decElmsPct) that.decElms *= elmSize;

  return that;

}

//----------------------------------------------------------------------
//
// Alloc Setting
//
//----------------------------------------------------------------------

Status Alloc::Set() {
  Status status;
  status = StartUp();
  return status;
}

Status Alloc::Set(String &str) {
  Status retStatus;
  Status status;
  retStatus = StartUp();
  status = Parse(str);
  if (status && !retStatus) retStatus = status;
  return status;
}

Status Alloc::SetOpen() {

  minElms    = UNaN;
  maxElms    = UNaN;
  incElms    = 0x7fff;
  incElmsPct = false;
  decElms    = 0x7fff;
  decElmsPct = false;

  return Error::None;
  
}

Status Alloc::SetClosed() {
  
  minElms    = 0;
  maxElms    = 0;
  incElms    = 0;
  incElmsPct = false;
  decElms    = 0;
  decElmsPct = false;
  
  return Error::None;
  
}

//----------------------------------------------------------------------
//
// Alloc Query
//
//----------------------------------------------------------------------

Boolean Alloc::IsFixed() {
  return (incElms == 0 && decElms == 0) ||
         ( minElms == maxElms && !BEFE::IsNull(minElms) && !BEFE::IsNull(maxElms) );
}

Boolean Alloc::IsLimited() {
  return !BEFE::IsNull(maxElms) || incElms == 0;
}


UInt Alloc::GetMin() {
  return minElms;
}

UInt Alloc::GetMax() {
  return maxElms;
}

UInt Alloc::GetInc() {
  if (incElms == 0x7fff)
    return UNaN;
  return incElms;
}

UInt Alloc::GetDec() {
  if (decElms == 0x7fff)
    return GetMin();
  return decElms;
}

UInt Alloc::GetIncElements(UInt len) {
  if ( BEFE::IsNull(len) || incElms <= 0)
    return DEFAULT_INC;
  if (incElms == 0)
    return 0;
  return (len+incElms-1)/incElms*incElms;
}

UInt Alloc::GetDecElements(UInt len) {
  if (len <= 0 || decElms <= 0)
    return DEFAULT_MIN;
  return (len+decElms-1)/decElms*decElms;
}

//----------------------------------------------------------------------
//
// String methods...
//
// The general form of an Alloc string is...
//
//   <min>[..<max>][+<inc>['%']][-<dec>['%']]
//
//     and...
//
//   '[' <min>[..<max>][+<inc>['%']][-<dec>['%']] ']'
//
// Where...
//
//   <min> => If not empty string, Minimum elements
//   <max> => If not empty string, Maximum elements
//   <inc> => If not empty string, Increment elements
//   <dec> => If not empty string, Decrement elements
//
// The [] version is equivalent to [0..UNaN+16-16]
// The [<min>] version is equivalent to [<min>..<min>+0-0] (Fixed Size)
// The [<min>..] version is equivalent to [<min>..UNaN]
// The [<min>..<max>] version is equivalent to  [<min>..<max>+16-16]
// The [..<max>] version is equivalent to [0..<max>+16-16]
//

String Alloc::ToString() const {

  String string;

  string += '[';

  if ( !BEFE::IsNull(minElms) )
    string += minElms;
  if (maxElms != minElms)
    string += "..";
  if ( !BEFE::IsNull(maxElms) && maxElms != minElms)
    string += maxElms;

  if (((Alloc *)this)->IsFixed()) goto DONE;

  if (incElms != 0x7fff) {
    string += '+';
    string += incElms;
    if (incElmsPct)
      string += '%';
  }
  if (decElms != 0x7fff) {
    string += '-';
    string += decElms;
    if (decElmsPct)
      string += '%';
  }

  while (false) {
    DONE: break;
  }

  string += ']';

  return string.Consumable();

}

Status Alloc::Parse(String const &string) {

  Status  status;           // Return status
  UInt    uints[4];         // The four UInts we're building up
  UInt    uintlens[4];      // The number of characters in each UInt we're building up
  Boolean uintpcts[2];      // "Was '%' specified for UInt 2 or 3?"
  Boolean havedots;         // "Was '..' specified?"
  Int     uintno;           // Current UInt we're working on
  UInt    prevuint;         // Previuos UInt value (before multiply and add)
  Byte    buf[256];         // Bytes to parse
  UInt    bufl;             // Bytes left to parse
  Byte   *bp;               // Current byte
  Byte   *tbp;              // Another current byte

  // Get a local copy of the string bytes...
  status = ((String)string).ToBytes(buf,sizeof(buf),bufl);
  if (status) goto SOMEERROR;

  // Trim leading and trailing whitespace
  bp = buf;
  while (bufl && (*bp == ' ' || *bp == '\t')) {bp++;bufl--;}
  tbp = buf+bufl-1;
  while (bufl && (*tbp == ' ' || *tbp == '\t')) {*tbp-- = 0; bufl--;}

  // Error if empty
  if (!bufl) goto PARSEERROR;

  // Skip leading leading '[', and if so, tailing ']'
  if (*bp == '[') {
    bp++; bufl--;
    if (!bufl || bp[bufl-1] != ']') goto PARSEERROR;
    bufl--;
    bp[bufl] = 0;
  }

  // Initialise the numbers and their lengths
  for (uintno=0; uintno<4; uintno++) {uints[uintno] = 0; uintlens[uintno] = 0;}
  uintpcts[0] = uintpcts[1] = false;

  // For each of the numbers...
  havedots = false;
  for (uintno=0; uintno<4 && bufl; uintno++) {

    // Skip leading whitespace
    while (bufl && (*bp == ' ' || *bp == '\t')) {bufl--; bp++; continue;}
    if (!bufl) break;

    // If we're at UInt[0] (Min) and at a '..' ...
    if (uintno == 1 && bufl >= 2 && *bp == '.' && *(bp+1) == '.') {
      bufl--; bufl--;
      bp++;   bp++;
      havedots = true;
    }

    // If we're at UInt[1] (Max) and at a '+' or '-'...
    if (uintno == 2 && (*bp == '+' || *bp == '-')) {
      bufl--;
      bp++;
    }

    // If we're at UInt[2] (Inc) and at a '-'...
    if (uintno == 3 && *bp == '-') {
      bufl--;
      bp++;
    }

    // Now, process the digits
    while (bufl && *bp >= '0' && *bp <= '9') {
      prevuint = uints[uintno];
      uints[uintno] = uints[uintno] * 10 + (*bp - '0');
      if (uints[uintno] < prevuint) goto PARSEERROR;
      uintlens[uintno]++;
      bp++;
      bufl--;
    }

    // Handle '%' if specified
    if (uintno >= 2 && bufl && uintlens[uintno] != 0 && *bp == '%') {
      if (uints[uintno] > 100) goto PARSEERROR;
      uintpcts[uintno-2] = true;
      bp++;
      bufl--;
    }

  }

  // Skip trailing whitespace...
  while (bufl && (*bp == ' ' || *bp == '\t')) {bp++; bufl--;}

  // Error if we still have some left
  if (bufl) goto PARSEERROR;

  // Put the numbers in their places...
  minElms     = maxElms    = UNaN;
  incElms     = decElms    = 0x7fff;
  incElmsPct  = decElmsPct = false;
  if (uintlens[0]) minElms = uints[0];
  if (uintlens[1]) maxElms = uints[1];
  if ( !havedots && !BEFE::IsNull(minElms) )
    maxElms = uints[0];
  if (uintlens[2]) {
    if (uints[2] >= 0x7fff) goto PARSEERROR;
    incElms    = uints[2];
    incElmsPct = uintpcts[0];
  }
  if (uintlens[3]) {
    if (uints[3] >= 0x7fff) goto PARSEERROR;
    decElms    = uints[3];
    decElmsPct = uintpcts[1];
  }

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                               break;
    PARSEERROR: status = Error::SeqBadAlloc; break;
  }

  if (status) {
    minElms = 0;
    maxElms = 0;
    incElms = 0x7fff;
    decElms = 0x7fff;
  }

  return status;

}

} // ...Namespace BEFE
