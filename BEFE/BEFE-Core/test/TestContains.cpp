//!befe-publish src
//----------------------------------------------------------------------
// File: TestContains.cpp - TestContains Class Implementation
//----------------------------------------------------------------------
//
// The TestContains Class is used to test various Array/Map/Sparse
// Contains functionality.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Test.h"
#include "TestContains.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// TestContains.Run - Main Contains Test method
//

Status TestContains::Run() {

  Status status;
  
  if (!gQuiet)
    out << "***Testing Contains" << "...\n";
  status = testContains();
  
  return status;

}

//----------------------------------------------------------------------
//
// TestNull.testContains - Test various Contains methods
//

Status TestContains::testContains() {

  Status status;
  UInt   numWarns;
  
  // Containment things
  Ints                    theInts;
  Strings                 theStrings;
  Numbers                 theNumbers;
  SparsePrimitive<Int>    theSInts;
  SparseValue<String>     theSStrings;
  UIntMapPrimitive<Int>   theIMInts;
  UIntMapValue<String>    theIMStrings;
  StringMapPrimitive<Int> theSMInts;
  StringMapValue<String>  theSMStrings;
  
  if (gVerbose)
    out << "  Testing <Class>.Contains()..." << Endl;

  numWarns = 0;
  
  // Populate and test the Arrays...
  theInts.Append(5);
  theInts.Append(6);
  theInts.Append(7);
  theInts.Append(8);
  if (theInts.Length() != 4) { Warn("Expected theInts to be 4 long"); numWarns++; }
  if (!theInts.Contains(7))  { Warn("Expected theInts to contain 7"); numWarns++; }
  if (theInts.Contains(17))  { Warn("Didn't expect theInts to contain 17"); numWarns++; }
  theStrings.Append("Five");
  theStrings.Append("Six");
  theStrings.Append(String("Seven").Insensitive());
  theStrings.Append("Eight");
  if (theStrings.Length() != 4)         { Warn("Expected theStrings to be 4 long"); numWarns++; }
  if (!theStrings.Contains("seven"))    { Warn("Expected theInts to contain 'seven'"); numWarns++; }
  if (!theStrings.Contains("Seven"))    { Warn("Expected theInts to contain 'Seven'"); numWarns++; }
  if (theStrings.Contains("Seventeen")) { Warn("Didn't expect theInts to contain 'Seventeen'"); numWarns++; }
  theNumbers.Append(Number(5));
  theNumbers.Append(Number(6));
  theNumbers.Append(Number(7));
  theNumbers.Append(Number(8));
  if (theNumbers.Length() != 4)        { Warn("Expected theNumbers to be 4 long");  numWarns++; }
  if (!theNumbers.Contains(Number(7))) { Warn("Expected theInts to contain 7"); numWarns++; }
  if (theNumbers.Contains(Number(17))) { Warn("Didn't expect theInts to contain 17"); numWarns++; }
  
  // Populate and test the Sparses...
  theSInts.Set(5,15);
  theSInts.Set(6,16);
  theSInts.Set(7,17);
  theSInts.Set(8,18);
  if (theSInts.Length() != 4) { Warn("Expected theSparse<Int> to be 4 long"); numWarns++; }
  if (theSInts.Contains(7))   { Warn("Didn't expect theSparse<Int> to contain 7"); numWarns++; }
  if (!theSInts.Contains(17)) { Warn("Expected theSparse<Int> to contain 17"); numWarns++; }
  theSStrings.Set(5,"Five");
  theSStrings.Set(6,"Six");
  theSStrings.Set(7,String("Seven").Insensitive());
  theSStrings.Set(8,"Eight");
  if (theSStrings.Length() != 4)         { Warn("Expected theSparse<String> to be 4 long"); numWarns++; }
  if (!theSStrings.Contains("seven"))    { Warn("Expected theSparse<String> to contain 'seven'"); numWarns++; }
  if (!theSStrings.Contains("Seven"))    { Warn("Expected theSparse<String> to contain 'Seven'"); numWarns++; }
  if (theSStrings.Contains("Seventeen")) { Warn("Didn't expect  theSparse<String> to contain 'Seventeen'"); numWarns++; }
  
  // Populate and test the IntMaps...
  theIMInts.Set(5,15);
  theIMInts.Set(6,16);
  theIMInts.Set(7,17);
  theIMInts.Set(8,18);
  if (theIMInts.Length() != 4) { Warn("Expected theIntMap<Int> to be 4 long"); numWarns++; }
  if (theIMInts.Contains(7))   { Warn("Didn't expect theIntMap<Int> to contain 7"); numWarns++; }
  if (!theIMInts.Contains(17)) { Warn("Expected theIntMap<Int> to contain 17"); numWarns++; }
  theIMStrings.Set(5,"Five");
  theIMStrings.Set(6,"Six");
  theIMStrings.Set(7,String("Seven").Insensitive());
  theIMStrings.Set(8,"Eight");
  if (theIMStrings.Length() != 4)         { Warn("Expected theIntMap<String> to be 4 long"); numWarns++; }
  if (!theIMStrings.Contains("seven"))    { Warn("Expected theIntMap<String> to contain 'seven'"); numWarns++; }
  if (!theIMStrings.Contains("Seven"))    { Warn("Expected theIntMap<String> to contain 'Seven'"); numWarns++; }
  if (theIMStrings.Contains("Seventeen")) { Warn("Didn't expect  theIntMap<String> to contain 'Seventeen'"); numWarns++; }
  
  // Populate and test the StringMaps...
  theSMInts.Set("5",15);
  theSMInts.Set("6",16);
  theSMInts.Set("7",17);
  theSMInts.Set("8",18);
  if (theSMInts.Length() != 4) { Warn("Expected theStringMap<Int> to be 4 long"); numWarns++; }
  if (theSMInts.Contains(7))   { Warn("Didn't expect theStringMap<Int> to contain 7"); numWarns++; }
  if (!theSMInts.Contains(17)) { Warn("Expected theStringMap<Int> to contain 17"); numWarns++; }
  theSMStrings.Set("5","Five");
  theSMStrings.Set("6","Six");
  theSMStrings.Set("7",String("Seven").Insensitive());
  theSMStrings.Set("8","Eight");
  if (theSMStrings.Length() != 4)         { Warn("Expected theStringMap<String> to be 4 long"); numWarns++; }
  if (!theSMStrings.Contains("seven"))    { Warn("Expected theStringMap<String> to contain 'seven'"); numWarns++; }
  if (!theSMStrings.Contains("Seven"))    { Warn("Expected theStringMap<String> to contain 'Seven'"); numWarns++; }
  if (theSMStrings.Contains("Seventeen")) { Warn("Didn't expect  theStringMap<String> to contain 'Seventeen'"); numWarns++; }
    
  // Handle errors
  if (numWarns) goto FAILED;
  status = Error::None;
  while (false) {
    FAILED: status = Error::Failed; break;
  }
  if (status) BEFE_ERROR("TestContains.testContains",status);
    
  return status;

}

} // ...Namespace BEFE
