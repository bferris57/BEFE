//----------------------------------------------------------------------
// File: StringSplit.cpp - String Class Counting and Splitting methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "UTF8.h"
#include "SBuffer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Counting
//

UInt String::Count(Char c) const {
  
  Status status;
  UInt   count;
  
  status = ((String *)this)->_LowCountAndSplit(String(c), isSensitive, false, false, (Strings *)NULL, (Spans *)NULL , &count);
  if (status)
    count = 0;
  return count;

}

UInt String::Count(char c) const {

  Status status;
  UInt   count;

  status = ((String *)this)->_LowCountAndSplit(String(c), isSensitive, false, false, NULL, NULL , &count);
  if (status)
    count = 0;
  else if (count) // ◄── Because "Split" splits into Occurrences+1 "parts"
    count--;
    
  return count;

}

UInt String::Count(String const &s) const {

  Status status;
  UInt   count;

  status = ((String *)this)->_LowCountAndSplit(s, isSensitive && s.isSensitive, false, false, NULL, NULL , &count);
  if (status)
    count = 0;
  else if (count) // ◄── Because "Split" splits into Occurrences+1 "parts"
    count--;

  return count;

}

//----------------------------------------------------------------------
//
// Splitting and Returning Strings
//

Strings String::SplitToStrings(Char searchChar) const {

  Strings sArray;

  ((String *)this)->_LowCountAndSplit(String(searchChar), isSensitive, false, false, &sArray, NULL, NULL);

  return sArray.Consumable();
  
}

Strings String::SplitToStrings(char searchChar) const {

  Strings sArray;

  ((String *)this)->_LowCountAndSplit(String(searchChar), isSensitive, false, false, &sArray, NULL, NULL);

  return sArray.Consumable();
  
}

Strings String::SplitToStrings(String const &searchString) const {

  Strings sArray;

  ((String *)this)->_LowCountAndSplit(searchString, isSensitive && searchString.isSensitive, false, false, &sArray, NULL, NULL);

  return sArray.Consumable();
  
}

//----------------------------------------------------------------------
//
// Splitting and Populating Strings
//

Status String::SplitToStrings(Char searchChar,Strings &theArray) const {

  Status status;

  status = ((String *)this)->_LowCountAndSplit(String(searchChar), isSensitive, false, false, &theArray, NULL, NULL);
  if (status)
    theArray.Reset();
    
  return status;

}

Status String::SplitToStrings(char searchChar, Strings &theArray) const {

  Status status;

  status = ((String *)this)->_LowCountAndSplit(String(searchChar), isSensitive, false, false, &theArray, NULL, NULL);
  if (status)
    theArray.Reset();
    
  return status;
  
}

Status String::SplitToStrings(String const &searchString,Strings &theArray) const {

  Status status;

  status = ((String *)this)->_LowCountAndSplit(searchString, isSensitive && searchString.isSensitive, false, false, &theArray, NULL, NULL);
  if (status)
    theArray.Reset();
    
  return status;
  
}

//----------------------------------------------------------------------
//
// Splitting and Returning ConstLen Strings
//

Status String::_SplitToConstStrings(Char searchChar, Strings &splitArray) const {

  Status status;

  if (isMutable) goto MUTABLE;
  status = ((String *)this)->_LowCountAndSplit(String(searchChar), isSensitive, true, false, &splitArray, NULL, NULL);

  while (false) {
    MUTABLE: status = Error::StringIsMutable; break;
  }
  
  if (status)
    splitArray.Reset();
    
  return status;
  
}

Status String::_SplitToConstStrings(char searchChar, Strings &splitArray) const {

  Status status;

  if (isMutable) goto MUTABLE;
  status = ((String *)this)->_LowCountAndSplit(String(searchChar), isSensitive, true, false, &splitArray, NULL, NULL);

  while (false) {
    MUTABLE: status = Error::StringIsMutable; break;
  }
  
  if (status)
    splitArray.Reset();
    
  return status;
    
}

Status String::_SplitToConstStrings(String const &searchString, Strings &splitArray) const {

  Status status;

  if (isMutable) goto MUTABLE;
  status = ((String *)this)->_LowCountAndSplit(searchString,
                                               isSensitive && searchString.isSensitive, true, false,
                                               &splitArray, NULL, NULL);
    
  while (false) {
    MUTABLE: status = Error::StringIsMutable; break;
  }
  
  if (status)
    splitArray.Reset();
  
  return status;
  
  
}

//----------------------------------------------------------------------
//
// Splitting and Returning Spans
//

Spans String::SplitToSpans(Char searchChar) const {

  Status status;
  Spans  sArray;

  status = ((String *)this)->_LowCountAndSplit(String(searchChar), isSensitive, false, true, NULL, &sArray, NULL);
  if (status) BEFE_ERROR("String.SplitToSpans",status);
  
  return sArray.Consumable();
  
}

Spans String::SplitToSpans(char searchChar) const {

  Status status;
  Spans  sArray;

  status = ((String *)this)->_LowCountAndSplit(String(searchChar), isSensitive, false, true, NULL, &sArray, NULL);
  if (status) BEFE_ERROR("String.SplitToSpans",status);
  
  return sArray.Consumable();
  
}

Spans String::SplitToSpans(String const &searchString) const {

  Status status;
  Spans  sArray;

  status = ((String *)this)->_LowCountAndSplit(searchString,
                                               isSensitive && searchString.isSensitive, false, true,
                                               NULL, &sArray, NULL);
  if (status) BEFE_ERROR("String.SplitToSpans",status);
  
  return sArray.Consumable();
  
}

//----------------------------------------------------------------------
//
// Splitting and Populating Logical Spans
//

Status String::SplitToSpans(Char searchChar, Spans &theArray) const {

  Status status;

  status = ((String *)this)->_LowCountAndSplit(String(searchChar), isSensitive, false, true, NULL, &theArray, NULL);
  if (status) BEFE_ERROR("String.SplitToSpans",status);
  
  return status;

}

Status String::SplitToSpans(char searchChar, Spans &theArray) const {

  Status status;

  status = ((String *)this)->_LowCountAndSplit(String(searchChar), isSensitive, false, true, NULL, &theArray, NULL);
  if (status) BEFE_ERROR("String.SplitToSpans",status);
  
  return status;

}
  
Status String::SplitToSpans(String const &searchString, Spans &theArray) const {

  Status status;

  status = ((String *)this)->_LowCountAndSplit(searchString,
                                               isSensitive && searchString.isSensitive, false, true,
                                               NULL, &theArray, NULL);
  if (status) BEFE_ERROR("String.SplitToSpans",status);
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Method: _LowCountAndSplit - The Method that DOES IT ALL!!!
//
// This method does ALL the splitting into non-Const Strings,
// Const Strings, and Spans, and counting.
//
// Note: This may seem like a cumbersome and overcomplicated bit of
//       over-kill code but, if you look closely, you'll see that it
//       pretty much does everything optimally in all cases whether
//       what's being searched for is a single Character, multiple
//       Characters, ASCII, UTF-8, and case sensitive/case-insensitive.
//
//       And, to be honest, it was a real bug-bear to get it all down
//       to a single bit of code but well worth it from our point of
//       view as it's all in one place and any un-caught bugs are most
//       likely to be found here other than spread around the calling
//       methods.
//

Status String::_LowCountAndSplit(String const &searchString,  // Search String
                                 Boolean  sensitive,          // true  = "Case sensitive search"
                                 Boolean  constStrings,       // true  = "Make Const Strings"
                                 Boolean  logical,            // true  = "Logical Spans", false="Physical Spans"
                                 Strings *strings,            // !NULL = Strings to populate
                                 Spans   *spans,              // !NULL = Spans to populate
                                 UInt    *count) {            // Count of occurrences

  Status  status;                     // Return Status
  Byte   *buf;                        // this Buffer
  UInt    bufSize;                    // this Buffer Size (in Bytes)
  UInt    bufLen;                     // this Buffer Size (in Chars, if logical==true)
  Byte   *curByte;                    // Where first Byte of search Char was found
  Byte   *curByte1;                   // Where first Byte of first search UTF-8 Char was found
  Byte   *curByte2;                   // Where first Byte of alternate search UTF-8 Char was found
  Byte   *thatBuf;                    // Search String Buffer
  UInt    thatSize;                   // Search String Buffer Size (in Bytes)
  UInt    thatLen;                    // Search String Buffer Length (in Chars)
  String  tString;                    // ConstLen String representing found position in this Buffer
  String  ncString;                   // Non-Const copy of ConstLen tString
  Char    firstChar;                  // First Char in search String
  Byte    utfBuf1[UTF8::MaxLength];   // Normal case first Char
  UInt    utfLen1;                    // Length (in Bytes) of first Char
  Byte    utfBuf2[UTF8::MaxLength];   // Alternate case first Char
  UInt    utfLen2;                    // Length (in Bytes) of Alternate Char
  Span    phySpan;                    // Physical Span of previous/next portion
  Span    logSpan;                    // Logical Span of previous/next portion
  Strings lStrings;                   // Local Strings we're building
  Spans   lSpans;                     // Local Spans we're building
  
  // Figure out where to get the data from...
  if (type == Null) {
    buf = NULL;
    bufSize = 0;
  }
  else if (type == Const) {
    buf = data;
    if (buf)
      bufSize = Strlen(data);
    else
      bufSize = 0;
  }
  else if (type == ConstLen) {
    buf     = data;
    bufSize = constLen;
  }
  else if (type >= ASCII0 && type <= UtfInline) {
    buf     = (Byte *)&data;
    bufSize = constLen;
  }
  else if (type == Buffer) {
    buf     = ((SBuffer *)&data)->GetBuf();
    bufSize = ((SBuffer *)&data)->Size();
  }
  else
    goto NOTCAPABLE;
  if (logical)
    bufLen = Length();
    
  // Clear the Output...
  if (strings) {
    status = strings->SetLength(0);
    if (status) goto SOMEERROR;
    lStrings = *strings;    // ◄── So caller can pre-allocate Increment if they wish
  }
  if (spans) {
    status = spans->SetLength(0);
    if (status) goto SOMEERROR;
    lSpans = *spans;       // ◄── So caller can pre-allocate Increment if they wish
  }
  if (count)
    *count = 0;
    
  // Get search string info...
  searchString._BufAndSize(thatBuf, thatSize);
  if (!thatBuf) {
    if (strings) {
      status = strings->Append(*this);
      if (status) goto SOMEERROR;
    }
    goto OK;
  }
  if (logical)
    thatLen = searchString.Length();
    
  // Set up search first Char(s)
  if (!sensitive) { // Need to do insensitive search...
    firstChar = UTF8::Fold(searchString.Get(0));
    status = UTF8::EncodeChar(firstChar, utfBuf1, utfLen1);
    if (status) goto SOMEERROR;
    status = UTF8::EncodeChar(UTF8::Unfold(firstChar),utfBuf2, utfLen2);
    if (status) goto SOMEERROR;
  }
  else { // Need to do sensitive search...
    firstChar = searchString.Get(0);
    status = UTF8::EncodeChar(firstChar, utfBuf1, utfLen1);  
    if (status) goto SOMEERROR;
    sensitive = true;
  }
  
  // Don't do anything if we're empty
  if (bufSize == 0 || BEFE::IsNull(buf)) goto OK;
  
  // Initialise local Spans and String
  tString.isMutable = false;
  tString.isByte    = false;
  tString.isChar    = false;
  tString.type      = ConstLen;
  tString.data      = buf;  
  phySpan = Span(0,NaN);
  logSpan = Span(0,NaN);
  
  // For each character...  
  do {

    // Find the next occurrence of the first byte
    curByte1 = Memchr(tString.data, utfBuf1[0], bufSize - (tString.data - buf));
    if (sensitive)
      curByte2 = curByte1;
    else
      curByte2 = Memchr(tString.data, utfBuf2[0], bufSize - (tString.data - buf));
    // Figure out which one is first (either or both may be NULL)...
    curByte = NULL;
    if (!BEFE::IsNull(curByte1)) {
      curByte = curByte1;
      if (!BEFE::IsNull(curByte2) && curByte2 < curByte1)
        curByte = curByte2;
    }
    else if (!BEFE::IsNull(curByte2))
      curByte = curByte2;
      
    // Handle this one...
    if (!curByte)
      tString.constLen = buf + bufSize - tString.data;
    else
      tString.constLen = curByte - tString.data;
    phySpan.idx2 = phySpan.idx1+tString.constLen;
    if (logical)
      logSpan.idx2 = logSpan.idx1 + tString.Length();
    
    // Append to lStrings...
    if (strings) {
      if (constStrings)
        status = lStrings.Append(tString);
      else {
        ncString = tString;
        status = ncString.Mutable();
        if (status) goto SOMEERROR;
        status = lStrings.Append(ncString.Consumable());
      }
      if (status) goto SOMEERROR;
    }
    
    // Append to lSpans...
    if (spans) {
      if (logical)
        status = lSpans.Append(logSpan);
      else
        status = lSpans.Append(phySpan);
      if (status) goto SOMEERROR;
    }
    
    // Update count...
    if (count)
      (*count)++;
      
    // Position to next character
    tString.data += tString.constLen + thatSize;
    phySpan.idx1 += tString.constLen + thatSize;
    if (logical)
      logSpan.idx1 += tString.Length() + thatLen;
      
  } while ( (UInt)(tString.data-buf) < bufSize );
  
  // If last occurrence was searchString, append an empty string
  if ( (UInt)(tString.data-buf) == bufSize ) {
    if (strings) {
      status = lStrings.Append(String());
      if (status) goto SOMEERROR;
    }
    if (spans) {
      if (logical)
        status = lSpans.Append(Span(bufLen,bufLen));
      else
        status = lSpans.Append(Span(bufSize,bufSize));
      if (status) goto SOMEERROR;
    }
    if (count)
      (*count)++;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTCAPABLE: status = Error::NotCapable;      break;
    OK:         status = Error::None;            break;
    SOMEERROR:                                   break; 
  }
  
  // If no errors, return info to caller...
  if (!status) {
    if (strings)
      (*strings) = lStrings.Consumable();
    if (spans)
      (*spans) = lSpans.Consumable();
  }
  
  return status;
  
}

} // ...Namespace BEFE
