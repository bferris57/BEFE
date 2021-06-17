//!befe-publish src
//----------------------------------------------------------------------
// File: testNumber.cpp - TestNumber Class Implementation
//----------------------------------------------------------------------
//
// The TestNumber Class is used to test the Number Class.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Test.h"
#include "TestNumber.h"
#include "OraNumber.h"
#include "Number.h"

namespace BEFE { // Namespace BEFE...

Status TestNumber::Run() {

  if (!gQuiet)
    out << indent << "***Testing Numbers..." << Endl;

  testBitRange();
  testRoundToPowerOfTwo();
  testUInt32FromString();
  testIntFromString();
  testIntBitScatter();
  testOraNumber();
  testNumber();
  testNumberCompare();
  testIntLog2();
 
  return Error::None;
  
}

Status TestNumber::testBitRange() {

  UInt bit0, bit1;

  if (gVerbose)
    out << indent << "  Function BitRange and IsPowerOfTwo..." << Endl;

  BitRange(0xffffffff,bit0,bit1);
  if (gVeryVerbose) {
    out << "    BitRange(0xffffffff)     = " << bit0 << ":" << bit1 << Endl;
    out << "    IsPowerOfTwo(0xffffffff) = " << IsPowerOfTwo(0xffffffff) << Endl;
  }
  BitRange(0x80000000,bit0,bit1);
  if (gVeryVerbose) {
    out << "    BitRange(0x80000000)     = " << bit0 << ":" << bit1 << Endl;
    out << "    IsPowerOfTwo(0x80000000) = " << IsPowerOfTwo(0x80000000) << Endl;
  }
  BitRange(0x40000000,bit0,bit1);
  if (gVeryVerbose) {
    out << "    BitRange(0x40000000)     = " << bit0 << ":" << bit1 << Endl;
    out << "    IsPowerOfTwo(0x40000000) = " << IsPowerOfTwo(0x40000000) << Endl;
  }
  BitRange(0x20000000,bit0,bit1);
  if (gVeryVerbose) {
    out << "    BitRange(0x20000000)     = " << bit0 << ":" << bit1 << Endl;
    out << "    IsPowerOfTwo(0x20000000) = " << IsPowerOfTwo(0x20000000) << Endl;
  }
  BitRange(0x10000000,bit0,bit1);
  if (gVeryVerbose) {
    out << "    BitRange(0x10000000)     = " << bit0 << ":" << bit1 << Endl;
    out << "    IsPowerOfTwo(0x10000000) = " << IsPowerOfTwo(0x10000000) << Endl;
  }
  BitRange(0x03000000,bit0,bit1);
  if (gVeryVerbose) {
    out << "    BitRange(0x03000000)     = " << bit0 << ":" << bit1 << Endl;
    out << "    IsPowerOfTwo(0x03000000) = " << IsPowerOfTwo(0x03000000) << Endl;
  }

  return Error::None;
}


Status TestNumber::testRoundToPowerOfTwo() {

  Int thenum;

  if (gVerbose)
    out << "  Function RoundToPowerOfTwo...\n";

  thenum = RoundToPowerOfTwo(0x80000000);
  if (gVeryVerbose)
    out << "    RoundToPowerOfTwo(0x80000000) = 0x" << ToHexString(thenum,8) << '\n';

  thenum = RoundToPowerOfTwo(0x20000000);
  if (gVeryVerbose)
    out << "    RoundToPowerOfTwo(0x20000000) = 0x" << ToHexString(thenum,8) << '\n';

  thenum = RoundToPowerOfTwo(0x03000000);
  if (gVeryVerbose)
    out << "    RoundToPowerOfTwo(0x03000000) = 0x" << ToHexString(thenum,8) << '\n';

  thenum = RoundToPowerOfTwo(0x00000001);
  if (gVeryVerbose)
    out << "    RoundToPowerOfTwo(0x00000001) = 0x" << ToHexString(thenum,8) << '\n';

  thenum = RoundToPowerOfTwo(1000);
  if (gVeryVerbose)
    out << "    RoundToPowerOfTwo(1000)       = " << thenum << '\n';

  return Error::None;
}

Status TestNumber::testUInt32FromString() {

  const char *strs[] = {
     "0101"
    ,"1234"
    ,"0111"
    ,"BEFE"
    ,NULL
  };

  Int    bases[] = {2,7,10,16,17};

  String str;
  Int    num;
  UInt   sno;
  UInt   bno;

  if (gVerbose)
    out << "  Function UInt32FromString...\n";

  for (bno=0; bno < sizeof(bases)/sizeof(Int); bno++) {

    if (gVeryVerbose) {
      out << Endl;
      out << "    Base " << bases[bno] << " UInt32FromString...\n";
    }

    for (sno=0; !IsNull(strs[sno]); sno++) {
      str = strs[sno];
      // Forward
      num = UInt32FromString(str,Slice(),bases[bno]);
      if (gVeryVerbose) {
        if ( IsNaN(num) )
          out << "      Forward: str = '" << str << "'  num = NaN\n";
        else
          out << "      Forward: str = '" << str << "'  num = " << num << Endl;
      }
      // Reverse
      num = UInt32FromString(str,Slice(NaN,NaN,-1),bases[bno]);
      if (gVeryVerbose) {
        if ( IsNaN(num) )
          out << "      Reverse: str = '" << str << "'  num = NaN\n";
        else
          out << "      Reverse: str = '" << str << "'  num = " << num << Endl;
      }
    }
  }

  return Error::None;
}

Status TestNumber::testIntFromString() {

  const char *strs[] = {
     ""
    ,"0123456789"
    ,"-123456789"
    ,"BEFE"
    ,"0xBEFE"
    ,"-0xBEFE"
    ,"12345678912"
    ,"2147483647"
    ,"2147483648"
    ,"2147483649"
    ,"0x80000001"
    ,"4294967293"
    ,"4294967294"
    ,"4294967295"
    ,"4294967296"
    ,"4294967297"
    ,"4294967298"
    ,"4294967299"
    ,"4294967300"
    ,NULL
  };

  String str;
  Int    num;
  Int    sno;

  if (gVerbose)
    out << "  Function IntFromString...\n";

  for (sno=0; !IsNull(strs[sno]); sno++) {

    str = strs[sno];

    // Do it
    num = Int32FromString(str);

    // Pad the string for nice output...
    str = '\'';
    str += strs[sno];
    str += '\'';
    str.Pad(14);

    // Display if we're supposed to
    if (gVeryVerbose) {
      if ( IsNaN(num) )
        out << "    str = " << str << "  num = NaN\n";
      else
        out << "    str = " << str << "  num = " << num << Endl;
    }

  }

  return Error::None;
}

//----------------------------------------------------------------------
//
// testIntBitScatter - Test IntBit scattering/collecting
//

Status TestNumber::testIntBitScatter() {

  String str;
  UInt   theMask;
  UInt   theAnswer;

  if (gVerbose)
    out << "  IntBit Scattering/Collecting functions...\n";

  theMask = 0x06018080;

  // Test IntBitCount
  if (gVerbose)
    out << "  Function IntBitCount...\n";
  theAnswer = IntBitCount(theMask);
  if (theAnswer != 5)
    out << "    Expected 0x05 from IntBitCount, got 0x" << ToHexString(theAnswer,8) << '\n';

  // Test IntBitMax
  if (gVerbose)
    out << "  Function IntBitMax...\n";
  theAnswer = IntBitMax(5);
  if (theAnswer != 31)
    out << "    Expected 0x1f from IntBitMax, got 0x" << ToHexString(theAnswer,8) << '\n';

  // Test simple IntBitScatter->IntBitCollect to make sure it keeps the value intact
  if (gVerbose)
    out << "  Function IntBitScatter->IntBitCollect...\n";
  theAnswer = IntBitScatter(0x1f,0x1f);
  if (theAnswer != 0x1f)
    out << "    Expected 0x1f from IntBitScatter(0x1f,0x1f), got 0x" << ToHexString(theAnswer,8) << '\n';
  theAnswer = IntBitCollect(0x1f,0x1f);

  if (theAnswer != 0x1f)
    out << "    Expected 0x1f from IntBitCollect(0x1f,0x1f), got 0x" << ToHexString(theAnswer,8) << '\n';

  // Test IntBitScatter
  theMask = 0x06018080;
  if (gVerbose)
    out << "  Function IntBitScatter...\n";
  theAnswer = IntBitScatter(0x1F,theMask);
  if (theAnswer != 0x06018080)
    out << "    Expected 0x06018080 from IntBitScatter, got 0x" << ToHexString(theAnswer,8) << '\n';
  theAnswer = IntBitScatter(0x0E,theMask);
  if (theAnswer != 0x02018000)
    out << "    Expected 0x02018000 from IntBitScatter, got 0x" << ToHexString(theAnswer,8) << '\n';

  // Test IntBitCollect
  if (gVerbose)
    out << "  Function IntBitCollect...\n";
  theAnswer = IntBitCollect(0x06018080,theMask);
  if (theAnswer != 0x1F)
    out << "    Expected 0x1F from IntBitCollect, got 0x" << ToHexString(theAnswer,8) << '\n';
  theAnswer = IntBitCollect(0x02018000,theMask);
  if (theAnswer != 0x0E)
    out << "    Expected 0x0E from IntBitCollect, got 0x" << ToHexString(theAnswer,8) << '\n';

  return Error::None;
}

//----------------------------------------------------------------------
//
// testOraNumber - Test the OraNumber class
//

Status TestNumber::testOraNumber() {

  Status    status;
  String    inStr;
  OraNumber num;
  String    string;
  UInt      strIdx;
  UInt      length;
  UInt      size;
  
  char const *strings[] = {
     "-123.456e-10"
    ,"12345"
    ,".0000012345"
    ,0
   };
  

  if (gVerbose)
    out << "  OraNumber Class...\n";

  for (strIdx=0; strings[strIdx]; strIdx++) {

    inStr = strings[strIdx];
    if (gVeryVerbose)
      out << "    String '" << inStr << "'...\n";
    status = num.FromString(inStr);
    if (status) BEFE_ERROR("OraNumber.FromString",status);
    if (gVeryVerbose)
      out << "      Debug    -- " << num.ToDebug() << '\n';
    status = num.ToString(string);
    if (status) BEFE_ERROR("OraNumber.ToString",status);
    length = num.Length();
    size   = num.Size();
    if (gVeryVerbose) {
      out << "      ToString -- " << string << '\n';
      out << "      Length   -- " << length << " (10's digits)\n";
      out << "      Size     -- " << size   << " (Bytes, including NUL)\n";
    }
  }
    
  return status;
    
}

//----------------------------------------------------------------------
//
// testNumber - Test the Number class
//

Status TestNumber::testNumber() {

  Status  status;
  Number  num;
  String  str;
  Strings strings;
  UInt    i;
  
  char **pTests;
  char const *tests[] = {
     ""
    ,"0"
    ,"+0"
    ,"-0"
    ,"00000"
    ,"-00000"
    ,"+00000"
    ,"1"
    ,"+1"
    ,"-1"
    ,"10"
    ,"+10"
    ,"-10"
    ,"9"
    ,"99"
    ,"999"
    ,"00001"
    ,"+00001"
    ,"-00001"
    ,".0123456"
    ,"-.0123456"
    ,".00123456"
    ,"-.00123456"
    ,"123456"
    ,"-123456"
    ,".123456"
    ,"-.123456"
    ,"1.23456"
    ,"-1.23456"
    ,"12.3456"
    ,"-12.3456"
    ,"1.23456e0"
    ,"1.23456e-0"
    ,"1.23456e1"
    ,"1.23456e-1"
    ,"1.23456e2"
    ,"1.23456e-2"
    ,"1.23456e6"
    ,"1.23456e-6"
    ,"123e3"
    ,"123e-3"
    ,NULL};
  
  if (gVerbose)
    out << "  Number Class...\n";

  if (gVeryVerbose) {
    out << "    String           Number.ToString  Exponent ExpSpan  Digits    Hash    ExpString\n";
    out << "    ---------------- ---------------- -------- -------- ------ ---------- ---------\n";
  }
  
  pTests = (char **)tests;
  while (*pTests) {
    str = *pTests++;
    //if (str.Length() == 0) continue;
    status = num.FromString(str);
    if (status)
      BEFE_ERROR("Number.FromString",status);
    if (gVeryVerbose)
      out << "    '" << (str+'\'').Pad(15);
    str = num.ToString();
    if (gVeryVerbose)
      out << " '"     << (str+'\'').Pad(15) << " " << ToString(num.Exponent(),4)
          << "     "  << num.ExponentSpan().ToString().Pad(8)
          << ' '      << ToString(num.ExponentSpan().Length()).LeftPad(4)
          << "   0x"  << ToHexString(num.Hash(), 8) 
          << " "      << num.ToExpString().Pad(9) << '\n';
    if (gShowDebug && gVeryVerbose) {
      strings = num.ToDebugStrings();
      for (i=0; i < strings.Length(); i++)
        out << "        " << strings.Get(i) << '\n';
    }
  }  
  
  return status;
    
}

//----------------------------------------------------------------------
//
// testNumberCompare - Test the Number.Compare Method
//

Status TestNumber::testNumberCompare() {

  Number  num1;
  Number  num2;
  Int     cmp;
  
  char **pTests;
  char const *tests[] = {
     "0"     ,"0"
    ,"0"     ,"1"
    ,"0"     ,"-1"
    ,"1"     ,"1"
    ,"1"     ,"-1"
    ,"1e-5"  ,"1"
    ,"1e5"   ,"1"
    ,"-1e-5" ,"1"
    ,"-1e5"  ,"1"
    ,"-1e-5" ,"-1"
    ,"-1e5"  ,"-1"
    ,"123"   ,"456"
    ,"456"   ,"123"
    ,"-123"  ,"-456"
    ,"-456"  ,"-123"
    ,"123"   ,"1234"
    ,"123"   ,"12345"
    ,"1230"  ,"123"
    ,"1234"  ,"123"
    ,"12345" ,"123"
    ,NULL};
  
  if (gVerbose)
    out << "  Number Class Compare...\n";

  if (gVeryVerbose) {
    out << "    String#1 String#2 Result\n";
    out << "    -------- -------- ------\n";
  }
  
  pTests = (char **)tests;
  while (*pTests) {
    num1.FromString(String(*pTests++));
    num2.FromString(String(*pTests++));
    cmp = num1.Compare(num2);
    if (gVeryVerbose) {
      out << "    " << num1.ToString().Pad(8) << ' ';
      out << num2.ToString().Pad(8);
      out << "   ";
      if (cmp == 0)
        out << '=';
      else if (cmp < 0)
        out << '<';
      else
        out << '>';
      out << '\n';
    }
  }  
  
  return Error::None;
    
}

//----------------------------------------------------------------------
//
// testIntLog2 - Test the IntLog2 function
//

Status TestNumber::testIntLog2() { // TestNumber.testIntLog2...

  Status status;
  
  if (gVerbose)
    out << "  IntLog2...\n";

  if (gVeryVerbose) {
    out << "    Log2(0)  = " << IntLog2(0)  << '\n';
    out << "    Log2(1)  = " << IntLog2(1)  << '\n';
    out << "    Log2(2)  = " << IntLog2(2)  << '\n';
    out << "    Log2(3)  = " << IntLog2(3)  << '\n';
    out << "    Log2(4)  = " << IntLog2(4)  << '\n';
    out << "    Log2(5)  = " << IntLog2(5)  << '\n';
    out << "    Log2(6)  = " << IntLog2(6)  << '\n';
    out << "    Log2(7)  = " << IntLog2(7)  << '\n';
    out << "    Log2(8)  = " << IntLog2(8)  << '\n';
    out << "    Log2(15) = " << IntLog2(15) << '\n';
    out << "    Log2(16) = " << IntLog2(16) << '\n';
    out << "    Log2(17) = " << IntLog2(17) << '\n';
    out << "    Log2(0x7FFFFFFFu) = " << IntLog2(0x7FFFFFFFu) << '\n';
    out << "    Log2(0x80000000u) = " << IntLog2(0x80000000u) << '\n';
    out << "    Log2(0xFFFFFFFEu) = " << IntLog2(0xFFFFFFFEu) << '\n';
    out << "    Log2(0xFFFFFFFFu) = " << IntLog2(0xFFFFFFFFu) << '\n';
  }
  
  status = Error::None;
  
  return status;
  
} // ...TestNumber.testIntLog2

} // ...Namespace BEFE