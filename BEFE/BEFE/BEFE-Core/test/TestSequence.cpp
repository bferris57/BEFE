//!befe-publish src
//----------------------------------------------------------------------
// File: TestSequence.cpp - Implementation of the TestSequence Class
//----------------------------------------------------------------------
//
// This Class implements the BEFE Sequence Class Unit Tests.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Test.h"
#include "TestSequence.h"
#include "Range.h"
#include "Span.h"
#include "USpan.h"

using namespace std;

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Run - Run all Sequence tests
//

Status TestSequence::Run() {

  if (!gQuiet)
    out << "***Testing Sequences..." << Endl;

  testAlloc();
  testSlice();
  testRange();
  testURange();
  testSpan();
  testUSpan();
  testCycle();
 
  return Error::None;
 
}
 
//----------------------------------------------------------------------
//
// testSlice - Test Slice class
//

Status TestSequence::testSlice() {

  const char *strs[] = {
     "[]"            // [ 0:10: 1] len=10
    ,"[5]"           // [ 5: 6: 1] len=10
    ,"[-5]"          // [ 5: ???
    ,"[:]"           // [ 0:10: 1] len=10
    ,"[::]"          // [ 0:10: 1] len=10
    ,"[5:5]"         // [ 5: 5: 1] len=0
    ,"[1:]"          // [ 1:10: 1] len=9
    ,"[:1]"          // [ 0: 1: 1] len=1
    ,"[1:2]"         // [ 1: 2: 1] len=1
    ,"[1::2]"        // [ 1:10: 2] len=5
    ,"[2:]"
    ,"[-1:3:-1]"
    ,"[-1::-2]"
    ,NULL
  };

  Status     status;
  int        strno;
  UInt       strmax;
  Slice      slice;
  String     str;
  String     rstr;
  Int        rlen;

  Ints       tarray;
  Ints       tarray2;
  String     string;
  String     string2;
  Byte       buf[256];

  if (gVerbose)
    out << "  Class Slice..." << Endl;

  // Figure out maximum string length
  strmax = 0;
  for (strno=0; !IsNull(strs[strno]); strno++)
    strmax = Max(strmax,Strlen(strs[strno]));

  // Process the strings
  for (strno=0; !IsNull(strs[strno]); strno++) {

    str = String(strs[strno]);
    if (gVeryVerbose) {
      out << "    Parse = '" << str << "'";
      out << ( String(' ')*(strmax - Strlen(strs[strno])) );
      out << "      status = ";
    }

    status = Slice::Parse(str,slice,'[',']');
    if (status) {
      out << "  !!! Error: " << ErrorString(status) << Endl;
      continue;
    }

    str = slice.ToString();
    if (gVeryVerbose)
      out << "'" << str << "'\n";

    slice = slice.Resolve(10);
    rlen  = slice.Length();
    rstr  = slice.ToString();
    if (gVeryVerbose) {
      out << "               ";
      out << String(' ')*strmax;
      out << "Resolve(10) = '" << rstr << "'\n";
      out << "               ";
      out << String(' ')*strmax;
      out << "     Length = " << rlen << Endl;
      out << Endl;
    }

  }

  // Try getting a char slice
  string = "0123456789";
  status = string.Get(slice,string2);
  if (status) goto SOMEERROR;
  if (gVeryVerbose) {
    out << "    Length(string)  = " << string.Length() << Endl;
    out << "    Length(string2) = " << string2.Length() << Endl;
  }
  string2.ToEllipsis(buf,sizeof(buf));
  if (gVeryVerbose)
    out << "    string2         = '" << (char *)buf << "'" << Endl;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: out << "!!!Error " << status << " from Get()" << Endl; break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// testAlloc - Test Alloc class
//

Status TestSequence::testAlloc() {

  const char *strs[] = {
     ""
    ,"0..+16384"
    ,"1..+32766"
    ,"2..+32767"
    ,"3..+32768"
    ,"0..10+10"
    ,"  0..100 +50% -75%"
    ,"100..  0  +0% -10%"
    ,"..+0%-10%"
    ,"1..2"
    ,"123"
    ,"2..1"
    ,"5.."
    ,"..6"
    ,"10..0-10"
    ,"3+16-8"
    ,"[..7]"
    ,"[]"
    ,"[..]"
    ,"[xxxx"
    ,"1..2+x-y"
    ,"1..2+200%"
    ,NULL
  };

  UInt     strno;
  UInt     strmax;
  Alloc    alloc;
  String   str;

  if (gVerbose)
    out << "  Class Alloc ()...\n";

  // Figure out maximum string length
  strmax = 0;
  for (strno=0; !IsNull(strs[strno]); strno++)
    strmax = Max(strmax,Strlen(strs[strno]));

  // Process the strings
  for (strno=0; !IsNull(strs[strno]); strno++) {

    str = String(strs[strno]);

    if (gVeryVerbose) {
      out << "    String = '" << str << "'";
      out << ( String(' ')*(strmax - Strlen(strs[strno])) );
    }

    alloc = Alloc(str);

    if (gVeryVerbose) {
      if (alloc.IsFixed())
        out << "   Fixed";
      else
        out << "  !Fixed";
      if (alloc.IsLimited())
        out << "  Limited";
      else
        out << " !Limited";
      out << " alloc = '";
      out << ToString(alloc);
      out << "'\n";
    }

  }

  return Error::None;

}

//----------------------------------------------------------------------
//
// testRange - Test Range class
//

Status TestSequence::testRange() {

  const char *strs[] = {
     ""
    ,".."
    ,"0.."
    ,"0..10"
    ,"123"
    ,"'a'"
    ,"'\\'"
    ,"'1'..'0'"
    ,"-1..0"
    ,"0..-1"
    ,"5..4"
    ,"5..3"
    ,"'a'..'z'"
    ,"0x00..0xff"
    ,"'\\'..'\\'"
    ,"'\\n'..'\\r'"
    ,"'\\b'..'\\f'"
    ,"'\\t'..'\\'"
    ,"0x0000..0x10ffff"
    ,"'¶'..'¾'"
    ,NULL
  };

  UInt     strno;
  UInt     strmax;
  Range    range;
  Int      len;
  String   str;

  if (gVerbose)
    out << "  Class Range...\n";

  // Figure out maximum string length
  strmax = 0;
  for (strno=0; !IsNull(strs[strno]); strno++)
    strmax = Max(strmax,Strlen(strs[strno]));

  // Process the strings
  for (strno=0; !IsNull(strs[strno]); strno++) {

    str   = String(strs[strno]);
    range = Range(str);

    // Output in decimal
    if (gVeryVerbose) {
      out << Endl;
      out << "    String = " << str;
      out << ( String(' ')*(strmax - Strlen(strs[strno])) );
      out << "  Range = ";
      str = range.ToString();
      out << str << Endl;
    }

    // Output in hex
    if (gVeryVerbose) {
      out << "             ";
      out << String(' ')*strmax;
      out << "    Hex = ";
      str = range.ToHexString();
      out << str;
      out << Endl;
    }

    // Output Non-Normal Length()
    len = range.Length();
    if (gVeryVerbose) {
      out << "             ";
      out << String(' ')*strmax;
      out << "    Len = ";
      if ( IsNull(len) )
        out << "UNaN";
      else
        out << len;
      out << Endl;
    }

    // Output Normal Length()
    len = range.Length();
    if (gVeryVerbose) {
      out << "             ";
      // +--  !!! KILLER line of code
      // |
      // V
      out << String(' ')*strmax;
      //out << Endl << "!!! strmax = " << strmax << Endl;
      out << "NormLen = ";
      if ( IsNull(len) )
        out << "UNaN";
      else
        out << len;
      out << Endl;
    }

  }

  return Error::None;

}

//----------------------------------------------------------------------
//
// testURange - Test URange class
//

Status TestSequence::testURange() {

  const char *strs[] = {
     ""
    ,".."
    ,"0.."
    ,"0..10"
    ,"123"
    ,"'a'"
    ,"'\\'"
    ,"'1'..'0'"
    ,"-1..0"
    ,"0..-1"
    ,"5..4"
    ,"5..3"
    ,"'a'..'z'"
    ,"0x00..0xff"
    ,"'\\'..'\\'"
    ,"'\\n'..'\\r'"
    ,"'\\b'..'\\f'"
    ,"'\\t'..'\\'"
    ,"0x0000..0x10ffff"
    ,"'¶'..'¾'"
    ,NULL
  };

  UInt     strno;
  UInt     strmax;
  URange   range;
  UInt     len;
  String   str;

  if (gVerbose)
    out << "  Class URange...\n";

  // Figure out maximum string length
  strmax = 0;
  for (strno=0; !IsNull(strs[strno]); strno++)
    strmax = Max(strmax,Strlen(strs[strno]));

  // Process the strings
  for (strno=0; !IsNull(strs[strno]); strno++) {

    str   = String(strs[strno]);
    range = URange(str);

    // Output in decimal
    if (gVeryVerbose) {
      out << Endl;
      out << "    String = " << str;
      out << ( String(' ')*(strmax - Strlen(strs[strno])) );
      out << "  Range = ";
      str = range.ToString();
      out << str << Endl;
    }

    // Output in hex
    if (gVeryVerbose) {
      out << "             ";
      out << String(' ')*strmax;
      out << "    Hex = ";
      str = range.ToHexString();
      out << str;
      out << Endl;
    }

    // Output Non-Normal Length()
    len = range.Length();
    if (gVeryVerbose) {
      out << "             ";
      out << String(' ')*strmax;
      out << "    Len = ";
      if ( IsNull(len) )
        out << "UNaN";
      else
        out << len;
      out << Endl;
    }

    // Output Normal Length()
    len = range.Length();
    if (gVeryVerbose) {
      out << "             ";
      // +--  !!! KILLER line of code
      // |
      // V
      out << String(' ')*strmax;
      //out << Endl << "!!! strmax = " << strmax << Endl;
      out << "NormLen = ";
      if ( IsNull(len) )
        out << "UNaN";
      else
        out << len;
      out << Endl;
    }

  }

  return Error::None;

}

//----------------------------------------------------------------------
//
// testSpan - Test Span class
//

Status TestSequence::testSpan() {

  const char *strs[] = {
     ""
    ,":"
    ,"0:"
    ,"0:10"
    ,"123"
    ,"'a'"
    ,"'\\'"
    ,"'1':'0'"
    ,"-1:0"
    ,"0:-1"
    ,"5:4"
    ,"5:3"
    ,"'a':'z'"
    ,"0x00:0xff"
    ,"'\\':'\\'"
    ,"'\\n':'\\r'"
    ,"'\\b':'\\f'"
    ,"'\\t':'\\'"
    ,"0x0000:0x110000"
    ,"'¶':'¾'"
    ,NULL
  };

  UInt     strno;
  UInt     strmax;
  Span     span;
  Int      len;
  String   str;

  if (gVerbose)
    out << "  Class Span...\n";

  // Figure out maximum string length
  strmax = 0;
  for (strno=0; !IsNull(strs[strno]); strno++)
    strmax = Max(strmax,Strlen(strs[strno]));

  // Process the strings
  for (strno=0; !IsNull(strs[strno]); strno++) {

    str  = String(strs[strno]);
    span = Span(str);

    // Output in decimal
    if (gVeryVerbose) {
      out << Endl;
      out << "    String = " << str;
      out << ( String(' ')*(strmax - Strlen(strs[strno])) );
      out << "  Span = ";
      str = span.ToString();
      out << str << Endl;
    }

    // Output in hex
    if (gVeryVerbose) {
      out << "             ";
      out << String(' ')*strmax;
      out << "    Hex = ";
      str = span.ToHexString();
      out << str;
      out << Endl;
    }

    // Output Non-Normal Length()
    len = span.Length();
    if (gVeryVerbose) {
      out << "             ";
      out << String(' ')*strmax;
      out << "    Len = ";
      if ( IsNull(len) )
        out << "UNaN";
      else
        out << len;
      out << Endl;
    }

    // Output Normal Length()
    len = span.Length();
    if (gVeryVerbose) {
      out << "             ";
      // +--  !!! KILLER line of code
      // |
      // V
      out << String(' ')*strmax;
      //out << Endl << "!!! strmax = " << strmax << Endl;
      out << "NormLen = ";
      if ( IsNull(len) )
        out << "UNaN";
      else
        out << len;
      out << Endl;
    }

  }

  return Error::None;

}

//----------------------------------------------------------------------
//
// testUSpan - Test USpan class
//

Status TestSequence::testUSpan() {

  const char *strs[] = {
     ""
    ,":"
    ,"0:"
    ,"0:10"
    ,"123"
    ,"'a'"
    ,"'\\'"
    ,"'1':'0'"
    ,"-1:0"
    ,"0:-1"
    ,"5:4"
    ,"5:3"
    ,"'a':'z'"
    ,"0x00:0xff"
    ,"'\\':'\\'"
    ,"'\\n':'\\r'"
    ,"'\\b':'\\f'"
    ,"'\\t':'\\'"
    ,"0x0000:0x110000"
    ,"'¶':'¾'"
    ,NULL
  };

  UInt     strno;
  UInt     strmax;
  USpan    span;
  Int      len;
  String   str;

  if (gVerbose)
    out << "  Class USpan...\n";

  // Figure out maximum string length
  strmax = 0;
  for (strno=0; !IsNull(strs[strno]); strno++)
    strmax = Max(strmax,Strlen(strs[strno]));

  // Process the strings
  for (strno=0; !IsNull(strs[strno]); strno++) {

    str  = String(strs[strno]);
    span = USpan(str);

    // Output in decimal
    if (gVeryVerbose) {
      out << Endl;
      out << "    String = " << str;
      out << ( String(' ')*(strmax - Strlen(strs[strno])) );
      out << "  USpan = ";
      str = span.ToString();
      out << str << Endl;
    }

    // Output in hex
    if (gVeryVerbose) {
      out << "             ";
      out << String(' ')*strmax;
      out << "    Hex = ";
      str = span.ToHexString();
      out << str;
      out << Endl;
    }

    // Output Non-Normal Length()
    len = span.Length();
    if (gVeryVerbose) {
      out << "             ";
      out << String(' ')*strmax;
      out << "    Len = ";
      if ( IsNull(len) )
        out << "UNaN";
      else
        out << len;
      out << Endl;
    }

    // Output Normal Length()
    len = span.Length();
    if (gVeryVerbose) {
      out << "             ";
      // ┌──  !!! KILLER line of code caused BIG problems once!!!
      // │        (who'd a thunk it?
      // ▼
      out << String(' ')*strmax;
      //out << Endl << "!!! strmax = " << strmax << Endl;
      out << "NormLen = ";
      if ( IsNull(len) )
        out << "UNaN";
      else
        out << len;
      out << Endl;
    }

  }

  return Error::None;

}

//----------------------------------------------------------------------
//
// testCycle - Test Cycle class
//

Status TestSequence::testCycle() {

  Cycle  cycle;
  String string;
  UInt   i;
  String string2;
  
  if (gVerbose)
    out << "  Class Cycle...\n";

  string = "0123456789abcdef";
 
  //
  // Test operator =
  //
 
  if (gVeryVerbose)
    out << "    Cycle Operator =...\n";

  cycle = Cycle(string.Length());
  if (gVeryVerbose) {
    out << "      Position = " << cycle.Position() << '\n';
    out << "      Length   = " << cycle.Length() << '\n';
  }
  
  cycle = 20u;
  if (gVeryVerbose) {
    out << "      UInt 20...\n";
    out << "        Position = " << cycle.Position() << '\n';
  }
 
  cycle = -5;
  if (gVeryVerbose) {
    out << "      Int -5...\n";
    out << "        Position = " << cycle.Position() << '\n';
  }
 
  //
  // Test operator + and operator +=
  //
 
  if (gVeryVerbose) {
    out << "    Cycle Operator + and +=...\n";
    out << "      string           = '" << string << "'\n";
  }
  
  string2.Reset();
  cycle = Cycle(string.Length());
  for (i=0; i < string.Length(); i++) {
    string2 += string.Get(cycle);
    cycle += 2;
  }
  if (gVeryVerbose)
    out << "      '+= 2'   string2 = '" << string2 << "'\n";
 
  string2.Reset();
  for (i=0; i < string.Length(); i++) {
    string2 += string.Get(cycle);
    cycle += i+1;
  }
  if (gVeryVerbose) {
    out << "      '+= i+1' string2 = '" << string2 << "'  ";
    if (gConsoleUTF8)
      out << "◄──";
    else
      out << "<--";
    out << " Pretty cool, eh?\n";
  }
  
  //
  // Test operator - and operator -=
  //
 
  if (gVeryVerbose) {
    out << "    Cycle Operator - and -=...\n";
    out << "      string           = '" << string << "'\n";
  }
  
  string2.Reset();
  cycle = Cycle(string.Length());
  for (i=0; i < string.Length(); i++) {
    string2 += string.Get(cycle);
    cycle -= 2;
  }
  if (gVeryVerbose)
    out << "      '-= 2'   string2 = '" << string2 << "'\n";
 
  string2.Reset();
  for (i=0; i < string.Length(); i++) {
    string2 += string.Get(cycle);
    cycle -= i+1;
  }
  if (gVeryVerbose) {
    out << "      '-= i+1' string2 = '" << string2 << "'  ";
    if (gConsoleUTF8)
      out << "◄──";
    else
      out << "<--";
    out << " Pretty cool, eh?\n";
  }
  
  //
  // Test operator ++
  //
 
  if (gVeryVerbose) {
    out << "    Cycle Operator ++...\n";
    out << "      string                    = '" << string << "'\n";
  }
  
  string2.Reset();
  cycle = Cycle(string.Length());
  for (i=0; i < string.Length(); i++)
    string2 += string.Get(cycle++);
  if (gVeryVerbose)
    out << "      'string[cycle++]' string2 = '" << string2 << "'\n";
 
  string2.Reset();
  cycle = Cycle(string.Length());
  for (i=0; i < string.Length(); i++)
    string2 += string.Get(++cycle);
  if (gVeryVerbose)
    out << "      'string[++cycle]' string2 = '" << string2 << "'\n";
 
  //
  // Test operator ++
  //

  if (gVeryVerbose) { 
    out << "    Cycle Operator --...\n";
    out << "      string                    = '" << string << "'\n";
  }
  
  string2.Reset();
  cycle = Cycle(string.Length());
  for (i=0; i < string.Length(); i++)
    string2 += string.Get(cycle--);
  if (gVeryVerbose)
    out << "      'string[cycle--]' string2 = '" << string2 << "'\n";
 
  string2.Reset();
  cycle = Cycle(string.Length());
  for (i=0; i < string.Length(); i++)
    string2 += string.Get(--cycle);
  if (gVeryVerbose)
    out << "      'string[--cycle]' string2 = '" << string2 << "'\n";
 
  return Error::None;
  
}

} // ...Namespace BEFE
