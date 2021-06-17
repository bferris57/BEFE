//!befe-publish src
//----------------------------------------------------------------------
// File: testNull.cpp - TestNull Class Implementation
//----------------------------------------------------------------------
//
// The TestNull Class is used to test various Null related functionality.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "ToStrings.h"
#include "Map.h"
#include "Test.h"
#include "TestNull.h"
#include "LifeCycle.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// TestNull.Run - Main Null/Empty Test method
//

Status TestNull::Run() {

  if (!gQuiet)
    out << "***Testing Null" << "...\n";
  testIsNull();
  
  return Error::None;

}

//----------------------------------------------------------------------
//
// TestNull.testIsNull - Test the global IsNull function
//

Status TestNull::testIsNull() {

  Status status;
  UInt   numWarns;
  
  // Nullable things
  Byte    theByte;
  SByte   theSByte;
  Short   theShort;
  UShort  theUShort;
  Int     theInt;
  UInt    theUInt;
  Long    theLong;
  ULong   theULong;
  Char    theChar;
  String  theString;
  Number  theNumber;
  Bytes   theBytes;
  Strings theStrings;
  Numbers theNumbers;
  
  if (gVerbose)
    out << "  Testing IsNull()..." << Endl;

  // Initialise the primitives...
  SetNull(theByte  );
  SetNull(theSByte );
  SetNull(theShort );
  SetNull(theUShort);
  SetNull(theInt   );
  SetNull(theUInt  );
  SetNull(theLong  );
  SetNull(theULong );
  
  // Make sure they all say they're null...
  numWarns = 0;
  if (!IsNull(theByte))    {BEFE_WARN("Byte    not initialised to Null"); numWarns++;}
  if (!IsNull(theSByte))   {BEFE_WARN("SByte   not initialised to Null"); numWarns++;}
  if (!IsNull(theShort))   {BEFE_WARN("Short   not initialised to Null"); numWarns++;}
  if (!IsNull(theUShort))  {BEFE_WARN("UShort  not initialised to Null"); numWarns++;}
  if (!IsNull(theInt))     {BEFE_WARN("Int     not initialised to Null"); numWarns++;}
  if (!IsNull(theUInt))    {BEFE_WARN("UInt    not initialised to Null"); numWarns++;}
  if (!IsNull(theLong))    {BEFE_WARN("Long    not initialised to Null"); numWarns++;}
  if (!IsNull(theULong))   {BEFE_WARN("ULong   not initialised to Null"); numWarns++;}
  if (!IsNull(theChar))    {BEFE_WARN("Char    not initialised to Null"); numWarns++;}
  if (!IsNull(theString))  {BEFE_WARN("String  not initialised to Null"); numWarns++;}
  if (!IsNull(theNumber))  {BEFE_WARN("Number  not initialised to Null"); numWarns++;}
  if (!IsNull(theBytes))   {BEFE_WARN("Bytes   not initialised to Null"); numWarns++;}
  if (!IsNull(theStrings)) {BEFE_WARN("Strings not initialised to Null"); numWarns++;}
  if (!IsNull(theNumbers)) {BEFE_WARN("Numbers not initialised to Null"); numWarns++;}
  if (numWarns) goto FAILED;
  
  // Handle errors
  status = Error::None;
  while (false) {
    FAILED: status = Error::Failed; break;
  }
  if (status) BEFE_ERROR("TestNull.testIsNull",status);
    
  return status;

}

} // ...Namespace BEFE
