//!befe-publish src
//----------------------------------------------------------------------
// File: TestUTF8.cpp - Implementation of the TestUTF8 Class
//----------------------------------------------------------------------
//
// This Class implements the BEFE UTF8 Class Unit Tests.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "UTF8.h"
#include "Test.h"
#include "TestUTF8.h"

namespace BEFE { // Namespace BEFE...

Status TestUTF8::Run() {

  if (!gQuiet)
    out << "***Testing UTF8...\n";

  testCount();
  testGetChar();
  testFolding();
  
  return Error::None;
}

Status TestUTF8::testCount() {

  const char *strings[] = {
    // Pairs of two strings... a) Test string, b) what the answer should be
     ""           ,"0"
    ,"0123456789" ,"10"
    ,"└┘┌┐"       ,"4"
    ,"This is a mixture of └┘ and ┌┐ characters","41"
    ,NULL
  };

  Int    i;
  Status status;
  UInt   numChars;
  UInt   numBytes;
  UInt   strSize;

  if (gVerbose)
    out << "  UTF8.Count..." << Endl;

  i = 0;
  while (strings[i] != 0) {
    strSize = Strlen(strings[i]);
    status = UTF8::Count((Byte *)strings[i],(UInt)strSize,numBytes,numChars);
    if (gVeryVerbose)
      out << "    numChars = " << numChars << ", should be " << strings[i+1] << Endl;
    i += 2;
  }

  status = Error::None;

  return status;

}

Status TestUTF8::testGetChar() {

  // This string has 21 characters (0..20) or (-1..-21)
  // The character in the middle is UTF-8 U+220F and it's
  // sitting at character position 10 or -11
  //
  // The testdata is a set of two integer pairs.  The first
  // one of the pair is the index, the 2nd one is the expected
  // character to get.
  //
  // NOTE: The Unicode character we use (U+220F) is encoded in
  //       UTF-8 as the three bytes 0xE2 0x88 0x8F

  const char *string = "0123456789∏1234567890";
  UInt        strl;

  Int testdata[] = {
       0, '0'
    , -1, '0'
    ,  5, '5'
    , -5, '6'
    , 10, 0x220f
    ,-11, 0x220f
    , 50, (Int)UTF8::BadChar
    ,-51, (Int)UTF8::BadChar
  };

  UInt   i;
  Status status;
  Int    thechar;
  Int    numbad;
  Int    numgood;

  if (gVerbose)
    out << "  UTF8.GetChar..." << Endl;

  numbad = numgood = 0;
  strl = Strlen(string);
  for (i=0;i<sizeof(testdata)/sizeof(Int)/2;i++) {
    thechar = UTF8::GetChar((Byte *)string,strl,testdata[i*2]);
    if (thechar != testdata[i*2+1]) {
      numbad++;
      if (!gQuiet)
        out << "    Test " << i << ": Expected 0x" << ToHexString(testdata[i*2+1],6)
                                 << " got 0x"       << ToHexString(thechar,6) << '\n';
    }
    else {
      numgood++;
      if (gVeryVerbose)
        out << "    Test " << i << ": Expected 0x" << ToHexString(testdata[i*2+1],6)
                                << " got 0x"       << ToHexString(thechar,6) << '\n';
    }
  }

  if (gVeryVerbose) {
    out << "    Tested " << numgood+numbad << " ..." << Endl;
    out << "      Succeeded: " << numgood << Endl;
    out << "      Failed:    " << numbad << Endl;
  }

  status = Error::None;

  return status;

}

Status TestUTF8::testFolding() {

  Status  status;
  Char    theChar;
  String  before;
  String  after;
  Boolean oldUnicodeFolding;
  Int     numFails;
  Globals oldGlobals;
  
  if (gVerbose)
    out << "  UTF8.Folding..." << Endl;
  numFails   = 0;
  
  // Load the folding declarations
  oldGlobals        = TheBefe->TheGlobals;
  oldUnicodeFolding = UTF8::IsUnicodeFolding();
  TheBefe->TheGlobals.bools.UnicodeTurkish = true;
  if (!oldUnicodeFolding) {
    status = UTF8::UnicodeFoldingOn();
    if (status) goto LOADFAILED;
  }
  
  //
  // Simple Folding...
  //
  
  if (gVerbose)
    out << "    Simple folding...\n";
  before = "French - GARČON";
  status = UTF8::FoldSimple(before, after);
  if (status) goto SFOLDFAILED;
  if (gVeryVerbose) {
    out << "      Before Fold = '" << before << "'\n";
    out << "      After  Fold = '" << after  << "'\n";
  }
  
  before = String("German - MAẞE");
  status = UTF8::FoldSimple(before, after);
  if (status) goto SFOLDFAILED;
  if (gVeryVerbose) {
    out << "      Before Fold   = '" << before << "'\n";
    out << "      After  Fold   = '" << after  << "'\n";
  }
  
  before = String("Turkish - Lowercase 'i', without the dot: '")+Char(0x0131u)+String("'");
  status = UTF8::UnfoldSimple(before, after);
  if (status) goto SFOLDFAILED;
  if (gVeryVerbose) {
    out << "      Before Unfold = \"" << before << "\"'\n";
    out << "      After  Unfold = \"" << after  << "\"'\n";
  }
  
  before = String("Turkish - Uppercase 'I', WITH    the dot: '")+Char(0x0130u)+String("'");
  status = UTF8::FoldSimple(before, after);
  if (status) goto SFOLDFAILED;
  if (gVeryVerbose) {
    out << "      Before Fold   = \"" << before << "\"\n";
    out << "      After  Fold   = \"" << after  << "\"\n";
  }
  
  before = String("Turkish - Uppercase Latin 'I', WITHOUT the dot: '")+Char(0x0130u)+String("'");
  status = UTF8::UnfoldSimple(before, after);
  if (status) goto SFOLDFAILED;
  if (gVeryVerbose) {
    out << "      Before Unfold = \"" << before << "\"\n";
    out << "      After  Unfold = \"" << after  << "\"\n";
  }
  
  before = String("Turkish - Lowercase Latin 'i', WITHOUT the dot: '")+Char(0x0131u)+String("'");
  status = UTF8::UnfoldSimple(before, after);
  if (status) goto SFOLDFAILED;
  if (gVeryVerbose) {
    out << "      Before Unfold = \"" << before << "\"\n";
    out << "      After  Unfold = \"" << after  << "\"\n";
  }
  
  //
  // Case Insensitivity
  //
  
  // Lowercase...
  before = "French - GARČON";
  after  = before.Lowercase();
  if (gVeryVerbose) {
    out << "      Lowercase before ='" << before << "'\n";
    out << "      Lowercase after  ='" << after  << "'\n";
  }
  if (after == before) {
    numFails++;
    BEFE_WARN(String("TestUTF8.testFolding: After '")+after+"' == '"+before+"' (shouldn't be)");
  }
  after.Insensitive();
  if (after != before) {
    numFails++;
    BEFE_WARN(String("TestUTF8.testFolding: Insensitive After '")+after+"' != '"+before+"' (shouldn't be)");
  }
  
  // Uppercase...
  before = "French - Garčon";
  after  = before.Uppercase();
  if (gVeryVerbose) {
    out << "      Uppercase before ='" << before << "'\n";
    out << "      Uppercase after  ='" << after  << "'\n";
  }
  if (after == before) {
    numFails++;
    BEFE_WARN(String("TestUTF8.testFolding: After '")+after+"' == '"+before+"' (shouldn't be)");
  }
  after.Insensitive();
  if (after != before) {
    numFails++;
    BEFE_WARN(String("TestUTF8.testFolding: Insensitive After '")+after+"' != '"+before+"' (shouldn't be)");
  }
  
  before = String("Turkish - Uppercase Latin 'I', WITHOUT the dot: '")+Char(0x0130u)+String("'");
  after  = before.Uppercase();
  if (gVeryVerbose) {
    out << "      Uppercase before ='" << before << "'\n";
    out << "      Uppercase after  ='" << after  << "'\n";
  }
  if (after == before) {
    numFails++;
    BEFE_WARN(String("TestUTF8.testFolding: After '")+after+"' == '"+before+"' (shouldn't be)");
  }
  after.Insensitive();
  if (after != before) {
    numFails++;
    BEFE_WARN(String("TestUTF8.testFolding: Insensitive After '")+after+"' != '"+before+"' (shouldn't be)");
  }
  
  before = String("Turkish - Lowercase Latin 'i', WITHOUT the dot: '")+Char(0x0131u)+String("'");
  after  = before.Uppercase();
  if (gVeryVerbose) {
    out << "      Uppercase before ='" << before << "'\n";
    out << "      Uppercase after  ='" << after  << "'\n";
  }
  if (after == before) {
    numFails++;
    BEFE_WARN(String("TestUTF8.testFolding: After '")+after+"' == '"+before+"' (shouldn't be)");
  }
  after.Insensitive();
  if (after != before) {
    numFails++;
    BEFE_WARN(String("TestUTF8.testFolding: Insensitive After '")+after+"' != '"+before+"' (shouldn't be)");
  }
  
  //
  // Finish up
  //
  
  status = UTF8::UnloadFolding();
  if (status) goto UNLOADFAILED;

  // Handle errors
  while (false) {
    LOADFAILED:   BEFE_ERROR("TestUTF8.TestFolding.LoadFolding",status);   break;
    SFOLDFAILED:  BEFE_ERROR("TestUTF8.TestFolding.FoldSimple",status);    break;
    UNLOADFAILED: BEFE_ERROR("TestUTF8.TestFolding.UnloadFolding",status); break;
  }
  
  // Clean up globals
  if (!oldUnicodeFolding)
    UTF8::UnicodeFoldingOff();
  TheBefe->TheGlobals = oldGlobals;
  
  // If no error and we failed, set status
  if (!status && numFails)
    status = Error::Failed;
    
  return status;

}

} // ...Namespace BEFE
