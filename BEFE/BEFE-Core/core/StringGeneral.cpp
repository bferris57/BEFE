//----------------------------------------------------------------------
// File: StringGeneral.cpp - Implementaion of general String Functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// String Escape/Unescape methods
//

String String::Escape() const {
  Status status;
  String outStr;
  status = BEFE::Escape(*this, outStr, false);
  if (status)
    outStr = *this;
  return outStr.Consumable();
}

Status String::Escape(String &outStr) const {
  Status status;
  status = BEFE::Escape(*this, outStr, true);
  return status;
}

Status String::Escape(String &outStr, Boolean escSpace) const {
  Status status;
  status = BEFE::Escape(*this, outStr, escSpace);
  return status;
}

String String::Unescape() {
  Status status;
  String outStr;
  status = BEFE::Unescape(*this, outStr);
  if (status)
    outStr = *this;
  return outStr.Consumable();
}

Status String::Unescape(String &outStr) const {
  Status status;
  status = BEFE::Unescape(*this, outStr);
  return status;
}
  
//----------------------------------------------------------------------
//
// Escape character substitution
//

Status Escape(String const &inStr, String &outStr, Boolean escSpace) {
  
  Status  status;           // Return Status
  Byte   *srcBuf;           // Address of inStr's Bytes
  UInt    srcByteCount;     // Number of Bytes in srcBuf
  UInt    srcCharCount;     // Number of Chars in srcBuf
  Byte   *bp;               // Input String current Byte pointer
  Boolean inHex;            // "In hex output mode?"
  String  tmpOut;           // Output String we're building
  UInt    charLen;          // UTF-8 Char length in Bytes
  Byte    firstOut;         // First Byte to output (for Control characters)
  
  ((String *)&inStr)->_BufByteAndCharCount(srcBuf, srcByteCount, srcCharCount);
  
  // It's easy if there's nothing to do...
  if (IsNull(srcBuf) || srcByteCount == 0 || srcCharCount == 0) goto OK;  
  
  //
  // Until we're finished...
  //
  
  bp = srcBuf;
  inHex = false;
  
  while (bp < srcBuf+srcByteCount) {

    // See if it's valid UTF-8...
    charLen = UTF8::GetCharSize(*bp);
    
    // If not ASCII Control and Invalid Utf-8, just output it...
    if (charLen == 1 && *bp >= 0x20) { // ASCII Printable...
      if (inHex) {
        status = tmpOut.Append('\\');
        if (status) goto SOMEERROR;
        inHex = false;
      }
      else if ((escSpace && *bp == ' ') || *bp == '\''|| *bp == '"' || *bp == '`' || *bp == '\\') {
        status = tmpOut.Append('\\');
        if (status) goto SOMEERROR;
      }
      status = tmpOut.Append(bp,charLen);
      if (status) goto SOMEERROR;
      bp += charLen;
      continue;
    } // ...ASCII Printable
    else if (charLen > 1 && charLen <= srcByteCount-(bp-srcBuf)) { // UTF-8 Sequence...
      if (inHex) {
        status = tmpOut.Append('\\');
        if (status) goto SOMEERROR;
        inHex = false;
      }
      status = tmpOut.Append(UTF8::GetChar(bp, charLen, 0));
      if (status) goto SOMEERROR;
      bp += charLen;
      continue;
    } // ...UTF-8 Sequence
    
    // Handle single character special escapes...
    firstOut = 0xFF;
    if      (*bp == 0x00) firstOut = 'a';
    else if (*bp == 0x08) firstOut = 'b';
    else if (*bp == 0x09) firstOut = 't';
    else if (*bp == 0x0A) firstOut = 'n';
    else if (*bp == 0x0B) firstOut = 'v';
    else if (*bp == 0x0C) firstOut = 'f';
    else if (*bp == 0x0D) firstOut = 'r';
    else if (*bp == 0x1B) firstOut = 'e';
    else                 firstOut = 0xFF;
    if (firstOut != 0xFF) { // Known special escape...
      status = tmpOut.Append('\\');
      if (status) goto SOMEERROR;
      status = tmpOut.Append(firstOut);
      if (status) goto SOMEERROR;
      bp++;
      inHex = false;
      continue;
    } // ...Known special escape
    
    // Need to output '\x'...
    if (!inHex) {
      status = tmpOut.Append("\\x");
      if (status) goto SOMEERROR;
      inHex = true;
    }

    // Output first nibble
    firstOut = (*bp)>>4;
    if (firstOut < 10) firstOut += '0';
    else               firstOut += 'A';
    status = tmpOut.Append(firstOut);
    if (status) goto SOMEERROR;
    
    // Output 2nd nibble
    firstOut = (*bp) & 0x0F;
    if (firstOut < 10) firstOut += '0';
    else               firstOut += 'A';
    status = tmpOut.Append(firstOut);
    if (status) goto SOMEERROR;

    // Position to next character
    bp++;
    
  };
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:             status = Error::None; break;
    SOMEERROR:                            break;
  }

  // Populate output string if appicable
  if (!status)
    outStr = tmpOut.Consumable();
    
  return status;
  
}

Status Unescape(String const &inStr, String &outStr) {
  
  Status  status;           // Return status
  Byte   *srcBuf;           // Address of inStr's Bytes
  UInt    srcByteCount;     // Number of Bytes in srcBuf
  UInt    srcCharCount;     // Number of Chars in srcBuf
  Byte   *bp;               // Input String current Byte pointer
  Byte   *startRun;         // Start of this run of same type of Chars
  String  tmpOut;           // Output String we're building
  UInt    charLen;          // UTF-8 Char length in Bytes
  Byte    firstOut;         // First Byte to output (for Control characters)
  
  ((String *)&inStr)->_BufByteAndCharCount(srcBuf, srcByteCount, srcCharCount);
  
  // It's easy if there's nothing to do...
  // Note: We have to do the assignment in case inStr is Empty instead of Null.  ;-)
  if (IsNull(srcBuf) || srcByteCount == 0 || srcCharCount == 0) {
    tmpOut = inStr;
    goto OK;
  }
  
  //
  // Until we're finished...
  //
  
  bp = srcBuf;
  startRun = NULL;
  
  while (bp < srcBuf+srcByteCount) {

    // If not escape character OR dangling escape, just output it...
    if (*bp != '\\' || bp == srcBuf+srcByteCount-1) {

      // Start a run if not already...
      if (IsNull(startRun)) startRun = bp;
      
      // See if it's valid UTF-8...
      charLen = UTF8::GetCharSize(*bp);
    
      // If it's a bad character...
      if (charLen == 0) {
        goto BADCHAR;
      }
      
      // Else, continue on...
      bp += charLen;
      
      continue;
      
    }

    // It's escaped, flush run so far (if any)...
    if (startRun) {
      if (bp-startRun) {
        status = tmpOut.Append(startRun, bp-startRun);
        if (status) goto SOMEERROR;
      }
      startRun = NULL;
    }
    
    // Handle single character special escapes...
    bp++;
    if      (*bp == 'a') firstOut = 0x00;
    else if (*bp == 'b') firstOut = 0x08;
    else if (*bp == 't') firstOut = 0x09;
    else if (*bp == 'n') firstOut = 0x0A;
    else if (*bp == 'v') firstOut = 0x0B;
    else if (*bp == 'f') firstOut = 0x0C;
    else if (*bp == 'r') firstOut = 0x0D;
    else if (*bp == 'e') firstOut = 0x1B;
    else                 firstOut = 0xFF;
    if (firstOut != 0xFF) { // Got it...
      status = tmpOut.Append(firstOut);
      if (status) goto SOMEERROR;
      bp++;
      continue;
    }
    
    // If '\x'...
    if (*bp == 'x') { // '\x'...
      
      // Position to first digit
      bp++;
      if ( (UInt)(bp-srcBuf) >= srcByteCount) break;
  
      // Count number of contiguous hex digits    
      startRun = bp;
      while ( (UInt)(bp-srcBuf) < srcByteCount && (
              (*bp >= '0' && *bp <= '9') ||
              (*bp >= 'a' && *bp <= 'f') ||
              (*bp >= 'A' && *bp <= 'F')) )
        bp++;
        
      // If none, special case...
      if (bp-startRun == 0) {
        status = tmpOut.Append('x');
        if (status) goto SOMEERROR;
        startRun = NULL;
        continue;
      }
      
      // Got some, if odd number of digits, output one nibble first...
      
      if ((bp-startRun)&0x01) {
        if      (*startRun >= 'a' && *startRun <= 'f') firstOut = *startRun-'a'+10;
        else if (*startRun >= 'A' && *startRun <= 'F') firstOut = *startRun-'A'+10;
        else                                           firstOut = *startRun-'0';
        status = tmpOut.Append(firstOut);
        if (status) goto SOMEERROR;
        startRun++;
      }
      
      // Do each of the remaining double-digit hexidecimal characters..
      while (startRun < bp) {
        if      (*startRun >= 'a' && *startRun <= 'f') firstOut = *startRun-'a'+10;
        else if (*startRun >= 'A' && *startRun <= 'F') firstOut = *startRun-'A'+10;
        else                                           firstOut = *startRun-'0';
        startRun++;
        firstOut <<= 4;
        if      (*startRun >= 'a' && *startRun <= 'f') firstOut += *startRun-'a'+10;
        else if (*startRun >= 'A' && *startRun <= 'F') firstOut += *startRun-'A'+10;
        else                                           firstOut += *startRun-'0';
        startRun++;
        status = tmpOut.Append(firstOut);
        if (status) goto SOMEERROR;
      };
      
      continue;
      
    } // ...'\x'
    
    // Otherwise, just output the next character
    status = tmpOut.Append(*bp);
    if (status) goto SOMEERROR;
    
    bp++;
    
  };
  
  // Flush run so far (if any)...
  if (startRun && bp-startRun) {
    if (startRun == srcBuf && (UInt)(bp-startRun) == srcByteCount)
      tmpOut = inStr;
    else {
      status = tmpOut.Append(startRun, bp-startRun);
      if (status) goto SOMEERROR;
    }
  }
    
  // Handle errors
  status = Error::None;
  while (false) {
    OK:             status = Error::None;                break;
    BADCHAR:        status = Error::BadUnicodeCodePoint; break;
    SOMEERROR:                                           break;
  }

  // Populate output string if appicable
  if (!status)
    outStr = tmpOut.Consumable();
    
  return status;
  
}

//----------------------------------------------------------------------
//
// Substitute - NamedStrings variable substitution
//

Status Substitute(String       const &inStr,      // IN:  Input String
                  NamedStrings const &substVars,  // IN:  Named Substitution Variables
                  String       const &startTag,   // IN:  Start Tag (eg "%" "${", etc)
                  String       const &endTag,     // IN:  End Tag (eg "%", "}", etc)
                  String             &outStr      // OUT: Resulting String
                 ) {

  Status  status;           // Return Status
  String  tmpOutStr;        // Working copy of Output String 
  String  tmpInStr;         // Working copy Input String
  String  tmpStr;           // Temporary working string
  String *ptrValue;         // Pointer to a physical substVars Map entry
  Strings processed;        // Strings already processed
  Byte   *srcBuf;           // Source Buffer (in inStr or tmpInStr)
  UInt    srcByteCount;     // Source Buffer Byte count
  UInt    srcCharCount;     // Source Buffer Char count
  Byte   *startBuf;         // Unused startTag Buffer
  UInt    startTagLen;      // Number of Chars in startTag
  UInt    startTagSize;     // Number ot Bytes in startTag
  Byte   *endBuf;           // Unused endTag Buffer (for UTF8 stuff)
  UInt    endTagLen;        // Number of Chars in endTag
  UInt    endTagSize;       // Number of Bytes in endTag
  Char    theChar;          // Unused Char (for UTF8 stuff)
  UInt    numMapEntries;    // Number of entries in the substitution Map
  UInt    numIters;         // Number of iterations through the loop
  UInt    numRepl;          // Number of replacements made
  // Current iteration's list of <startTag>..<endTag> occurrences...
  Span    span;             // A temporary Span we're calculating
  Span    prvSpan;          // Previous span we replaced
  Spans   spans;            // Spans specifiying where to replace
  UInt    numSpans;         // Number of Spans this iteration
  UInt    spanIdx;          // Current Span index into spans
  UInt    curRepl;          // Current number of replacements
  
  // Initialise iteration info
  ((String *)&inStr)->_BufByteAndCharCount(srcBuf, srcByteCount, srcCharCount);
  ((String *)&startTag)->_BufByteAndCharCount(startBuf, startTagSize, startTagLen);
  ((String *)&endTag)->_BufByteAndCharCount(endBuf, endTagSize, endTagLen);
  numMapEntries = substVars.Length();
  numIters = 0;
  numRepl  = 0;
  
  // Make sure there's something to do
  if (startTagLen == 0 || endTagLen == 0 || srcCharCount == 0) goto OK;
  
  //
  // Until nothing left to substitute...
  //
    
  tmpInStr = String((char const *)srcBuf, srcByteCount);

  do { // Until finished...
    
    curRepl = 0;
    
    // Find all the occurrences of <startTag>...<endTag>
    status = spans.SetEmpty();
    if (status) goto SOMEERROR;
    span.idx1 = 0;
    do {
      // Find next occurrence of startTag...
      span.idx2 = srcCharCount;
      span.idx1 = tmpInStr.Find(startTag, span);
      if (span.idx1 < 0) break;
      // Find following occurrence of the endTag...
      // NOTE: After this point, span contains character indexes but the
      //       code later on uses physical byte indexes.  So, we've got
      //       to convert it to Byte offset later.  This is all because
      //       of UTF-8.  ;-)
      span.idx2 = tmpInStr.Find(endTag, Span(span.idx1+startTagLen, srcCharCount));
      if (span.idx2 < 0) break;
      // Account for <endTag> length in span
      span.idx2 += endTagLen;
      // Append the Span to list of Spans this iteration...
      status = spans.Append(span);
      if (status) goto SOMEERROR;
      // Set up for next span
      span.idx1 = span.idx2;
    } while ((UInt)span.idx1 < srcCharCount);
    
    // Finished if nothing to replace
    numSpans = spans.Length();
    if (numSpans == 0) break;
    
    // Try replacing each Span...
    prvSpan = Span(0,0);
    tmpOutStr.SetEmpty();
    for (spanIdx=0; spanIdx < numSpans; spanIdx++) {
      span = spans.Get(spanIdx);
      // Note: As mentioned above when calculating Spans, they contain
      //       Char pos Spans, we need Byte pos to make things require
      //       less memory allocations, so we'll do that NOW...
      if (srcByteCount != srcCharCount) {
        status = UTF8::GetCharPos(srcBuf, srcByteCount, span.idx1, 0, span.idx1, theChar);
        if (status) goto SOMEERROR;
        if (span.idx2 == (Int)srcCharCount)
          span.idx2 = srcByteCount;
        else {
          status = UTF8::GetCharPos(srcBuf, srcByteCount, span.idx2, 0, span.idx2, theChar);
          if (status) goto SOMEERROR;
        }
      }
      
      // If something in between, append it
      if (span.idx1 - prvSpan.idx2) {
        tmpStr = String((char const *)srcBuf+prvSpan.idx2, span.idx1 - prvSpan.idx2);
        tmpOutStr += tmpStr;
      }
      // Get the Substitution Variable Name
      if (!span.Length()) goto ZEROLENGTH;
      tmpStr = String((char const *)srcBuf+span.idx1+startTagSize, span.idx2-span.idx1-startTagSize-endTagSize);
      // Perform the substitution...
      ptrValue = (String *)substVars._GetPhyAddress(tmpStr);
      if (ptrValue) ptrValue++; // ◄── Point to the Value not the Key  ;-)
      if (!IsNull(ptrValue)) { // It does exist, append it...
        tmpOutStr += *ptrValue;
        curRepl++;
      }
      else { // Doesn't exist, replace it with the original span...
        if (span.Length()) { // ◄── Shouldn't be zero but we're not sure at the moment
          tmpStr = String((char const *)srcBuf+span.idx1, span.idx2-span.idx1);
          tmpOutStr += tmpStr;
        }
      }
      // Get ready for next iteration
      prvSpan = span;
    }
    // Append remaining after last span
    if ((UInt)prvSpan.idx2 < srcByteCount) {
      tmpStr = String((char const *)srcBuf+span.idx2, srcByteCount-prvSpan.idx2);
      tmpOutStr += tmpStr;
    }

    // Finished with this iteration
    numIters++;
    numRepl += curRepl;

    // Stop if it's time
    if (numIters > numMapEntries || curRepl == 0) break;
    
    // Set up for next iteration...
    tmpInStr = tmpOutStr.Consumable();
    tmpInStr._BufByteAndCharCount(srcBuf, srcByteCount, srcCharCount);
    
  } while (true); // ...Until finished (could be circular)
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:         status = Error::None;               break;
    ZEROLENGTH: status = Error::InternalZeroLength; break;
    SOMEERROR:                                      break;
  }

  // If everything's okay, set the output string
  if (status == Error::None) {
    if (numIters == 0 || numRepl == 0)
      outStr = inStr;
    else
      outStr = tmpInStr.Consumable();
  }
  
  return status;
  
}

} // ...Namespace BEFE
