//!befe-publish src
//----------------------------------------------------------------------
// File: TestArray.cpp - TestArray Class Implementation
//----------------------------------------------------------------------
//
// The TestArray Class is used to test all the functionality of Arrays,
// Sparses, and Maps.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "_GlobalMacros.h"
#include "ToStrings.h"
#include "Map.h"
#include "Test.h"
#include "TestArray.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// TestArray.Run - Main Array Test method
//

Status TestArray::Run() {

  if (!gQuiet)
    out << "***Testing Array" << "...\n";
  //testBytes();
  //testArrayShort();
  //testArrayInt();
  testArrayLong();
  testStrings();
  testNumbers();
  testSparse1();
  testSparse2();
  testRemove();
  testCopy();
  testReverse();
  testUIntMap();
  testStringMap();
  testSort();
  testNamedStrings();
  
  return Error::None;

}

//----------------------------------------------------------------------
//
// TestArray.test - Utility method for testing Arrays in general
//

Status TestArray::test(UInt which, void *theArray) {

  Status status;
  struct {
	Bytes  b;
	Shorts s;
	Ints   i;
	Longs  l;
  }  before;
  struct {
	Bytes  b;
	Shorts s;
	Ints   i;
	Longs  l;
  } after;
  UInt   numElms;
  UInt   i;

  const char *prefix[] = {
     "Byte"
    ,"Short"
    ,"Int"
    ,"Long"
  };

  union {
    Byte    theBytes[8];
    Byte    b;
    Short   s;
    Int     i;
    Long    l;
  } elm;

  union {
    Bytes  *b;
    Shorts *s;
    Ints   *i;
    Longs  *l;
  } array;

  String theValue;

  if (which < 0 || which >= NUMTESTS) {
    out << "!!!INTERNAL ERROR: 'which' (" << which << ") is invalid" << Endl;
    goto DONE;
  }

  if (gVerbose)
    out << "  Testing " << prefix[which] << "s..." << Endl;

  elm.l = 0;
  array.b = (Bytes *)theArray;
  
  // Append some elements
  for (i=0;i<26;i++) { // For each value...

    // Set the one byte value
    elm.l = 0;
    if (TheBefe->TheProcessor->GetByteOrder() == Processor::LittleEndian)
      elm.theBytes[0] = i;
    else
      elm.theBytes[which] = i;

    // Based on the type...
    status = Error::Internal;
    switch (which) {
      case TestByte:  status = array.b->Append(&elm.b,1); break;
      case TestShort: status = array.s->Append(elm.s);    break;
      case TestInt:   status = array.i->Append(elm.i);    break;
      case TestLong:  status = array.l->Append(elm.l);    break;
    }
    if (status) goto ERROR;

  } // ...For each value

  // Take a snapshot
  switch (which) {
    case TestByte:
  	  numElms = array.b->Length();
   	  before.b = *array.b;
      break;
    case TestShort:
  	  numElms = array.s->Length();
   	  before.s = *array.s;
      break;
    case TestInt:
  	  numElms = array.i->Length();
   	  before.i = *array.i;
      break;
    case TestLong:
  	  numElms = array.l->Length();
   	  before.l = *array.l;
      break;
  }
  
  // Remove elements
  for (i=0;i<20;i++) {
    // Based on the type...
    status = Error::Internal;
    switch (which) {
      case TestByte:  status = array.b->Remove(3); break;
      case TestShort: status = array.s->Remove(3); break;
      case TestInt:   status = array.i->Remove(3); break;
      case TestLong:  status = array.l->Remove(3); break;
    }
    if (status) goto ERROR;
  }

  // Take another snapshot
  switch (which) {
    case TestByte:
   	  after.b = *array.b;
      break;
    case TestShort:
   	  after.s = *array.s;
      break;
    case TestInt:
   	  after.i = *array.i;
      break;
    case TestLong:
   	  after.l = *array.l;
      break;
  }

  // Insert an element
  elm.l = -1;
  if (TheBefe->TheProcessor->GetByteOrder() == Processor::LittleEndian)
    elm.theBytes[0] = -2;
  else
    elm.theBytes[1>>which] = -2;
  switch (which) {
    case TestByte:  status = array.b->Insert(5,&elm.b,1); break;
    case TestShort: status = array.s->Insert(5,elm.s);    break;
    case TestInt:   status = array.i->Insert(5,elm.i);    break;
    case TestLong:  status = array.l->Insert(3,elm.l);    break;
  }
  if (status) goto ERROR;

  // Display status before remove
  if (gVerbose)
    out << "    Before Remove and Insert..." << Endl;
  switch (which) {
    case TestByte:  numElms = before.b.Length(); break;
    case TestShort: numElms = before.s.Length(); break;
    case TestInt:   numElms = before.i.Length(); break;
    case TestLong:  numElms = before.l.Length(); break;
  }
  for (i=0; i < numElms; i++) {
    elm.l = -1;

    switch (which) {
      case TestByte:  status = before.b.Get(i,elm.b); break;
      case TestShort: status = before.s.Get(i,elm.s); break;
      case TestInt:   status = before.i.Get(i,elm.i); break;
      case TestLong:  status = before.l.Get(i,elm.l); break;
    }
    if (status) goto ERROR;
    switch (which) {
      case TestByte:  theValue = ToHexString(elm.b); break;
      case TestShort: theValue = ToHexString(elm.s); break;
      case TestInt:   theValue = ToHexString(elm.i); break;
      case TestLong:  theValue = ToHexString(elm.l); break;
    }
    if (gVeryVerbose)
      out << "      " << prefix[which] << "[" << i << "] = 0x" << theValue << Endl;
  }

  // Display status from copy constructor after remove
  {
    if (gVerbose)
      out << "    Copy Constructor (after Remove, before Insert)..." << Endl;
    switch (which) {
      case TestByte:  numElms = after.b.Length(); break;
      case TestShort: numElms = after.s.Length(); break;
      case TestInt:   numElms = after.i.Length(); break;
      case TestLong:  numElms = after.l.Length(); break;
    }
    for (i=0; i < numElms; i++) {
      elm.i = -1;
      switch (which) {
        case TestByte:  status = after.b.Get(i,elm.b); break;
        case TestShort: status = after.s.Get(i,elm.s); break;
        case TestInt:   status = after.i.Get(i,elm.i); break;
        case TestLong:  status = after.l.Get(i,elm.l); break;
      }
      if (status) goto ERROR;
      switch (which) {
        case TestByte:  theValue = ToHexString(elm.b); break;
        case TestShort: theValue = ToHexString(elm.s); break;
        case TestInt:   theValue = ToHexString(elm.i); break;
        case TestLong:  theValue = ToHexString(elm.l); break;
      }
      if (gVeryVerbose)
        out << "      " << prefix[which] << "[" << i << "] = 0x" << theValue << Endl;
    }
  }
  
  // Display status after remove
  if (gVerbose)
    out << "    After Remove and Insert..." << Endl;
  numElms = array.b->Length();
  for (i=0; i < numElms; i++) {
    elm.l = -1;
    switch (which) {
      case TestByte:  status = array.b->Get(i, elm.b); break;
      case TestShort: status = array.s->Get(i, elm.s); break;
      case TestInt:   status = array.i->Get(i, elm.i); break;
      case TestLong:  status = array.l->Get(i, elm.l); break;
    }
    if (status) goto ERROR;
    switch (which) {
      case TestByte:  theValue = ToHexString(elm.b); break;
      case TestShort: theValue = ToHexString(elm.s); break;
      case TestInt:   theValue = ToHexString(elm.i); break;
      case TestLong:  theValue = ToHexString(elm.l); break;
    }    
    if (gVeryVerbose)
      out << "      " << prefix[which] << "[" << i << "] = 0x" << theValue << Endl;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    ERROR: BEFE_ERROR("TestArray.test",status);  break;
    DONE:  status = Error::None;                 break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// TestArray.testArray<T> - Test Array of <T>
//

Status TestArray::testArrayShort() {
  Shorts array;
  test(TestShort,&array);
  return Error::None;
}

Status TestArray::testArrayInt() {
  Ints array;
  test(TestInt,&array);
  return Error::None;
}

Status TestArray::testArrayLong() {
  Longs array;
  test(TestLong,&array);
  return Error::None;
}


//----------------------------------------------------------------------
//
// TestArray.testBytes - Test Array of Bytes
//

Status TestArray::testBytes() {

  Status      status;
  const char *str1 = "Hi there!";
  Int         str1len;
  const char *str2 = " ABC!";
  Int         str2len;
  Bytes       bytes;
  Byte        buf[128];
  UInt        size;

  str1len = Strlen(str1);
  str2len = Strlen(str2);

  if (gVerbose)
    out << "  Class Bytes...\n";

  if (gVeryVerbose)
    out << "    sizeof(Bytes) = " << sizeof(Bytes) << Endl;

  // Test Append...
  if (gVeryVerbose)
    out << "    Before Append... bytes.Length() = " << bytes.Length() << Endl;
  status = bytes.Append((Byte *)str1,str1len);
  if (status) {
    out << "!!! Bytes.Get: " << ErrorString(status) << Endl;
    goto SOMEERROR;
  }
  if (gVeryVerbose) {
    out << "    After  Append... bytes.Length() = " << bytes.Length() << Endl
        << "                     Should be        " << str1len        << Endl;
  }

  // Test Get...
  status = bytes.Get(0,buf,sizeof(buf),size);
  if (status) {
    out << "!!! Bytes.Get: " << ErrorString(status) << Endl;
    goto SOMEERROR;
  }
  buf[size] = 0x00;
  if (gVeryVerbose) {
    out << "    After  Get...    buf            = '" << (char *)buf  << "'\n"
        << "                     Should be        '" <<         str1 << Endl;
  }

  // Test Set...
  status = bytes.Set(str1len-1,(Byte *)str2,str2len);
  if (status) {
    out << "!!! Bytes.Set: " << ErrorString(status) << Endl;
    goto SOMEERROR;
  }
  status = bytes.Get(0,buf,sizeof(buf),size);
  if (status) {
    out << "!!! Bytes.Set: " << ErrorString(status) << Endl;
    goto SOMEERROR;
  }
  buf[size] = 0x00;
  if (gVeryVerbose)
    out << "    After  Set...    buf            = '" << (char *)buf  << "'\n";
  Strcpy(buf,str1);
  buf[str1len-1] = 0x00;
  Strcat(buf,str2);
  if (gVeryVerbose)
    out << "                     Should be        '" << (char *)buf  << Endl;

  goto OK;

  // Handle errors
  while (false) {
	  OK:        status = Error::None; break;
	  SOMEERROR:                       break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// TestArray.testStrings - Test Array of Strings
//

Status TestArray::testStrings() {

  String  astring;
  Strings strings;
  Byte    buf[21];
  UInt    i;
  Status  status;
  UInt    usedlen;

  const char *strs[] = {
    "String 1"
    ,"2nd string"
    ,"..."
    ,"The last string"
    ,NULL
  };

  if (gVerbose)
    out << "  Class Strings..." << Endl;

  // Append the strings
  for (i=0; !IsNull(strs[i]); i++) {
    if (gVeryVerbose)
      out << "    Appending " << i << ": '" << strs[i] << "'..." << Endl;
    astring = String(strs[i]);
    status = strings.Append(astring);
    if (status) goto ERROR;
  }

  // Get each of them and make sure they're in there
  buf[sizeof(buf)-1] = 0;
  for (i=0; !IsNull(strs[i]); i++) {
    if (gVeryVerbose)
      out << "    Getting " << i << "..." << Endl;
    status = strings.Get(i,astring);
    if (status) goto ERROR;
    buf[0] = 0;
    status = astring.ToEllipsis((Byte *)buf,sizeof(buf)-1,usedlen);
    if (status) goto ERROR;
    if (gVeryVerbose)
      out << "      strings[" << i << "] = '" << (char *)buf << "'" << Endl;
    if (Strcmp(buf,strs[i]) != 0) {
      if (gVeryVerbose) {
        out << "      strings[" << i << "] is '" << (char *)buf << "'" << Endl;
        out << "        (should be '" << strs[i] << "'" << Endl;
      }
      goto INTERNALERROR;
    }
    if (gVeryVerbose)
      out << "      (they compare!)" << Endl;
  }

  // Remove index 2 ('...')
  if (gVeryVerbose)
    out << "    Removing strings[2]..." << Endl;
  status = strings.Remove(2);
  if (status) goto ERROR;

  // Insert "Hi there, guy!" in index 2...
  if (gVeryVerbose)
    out << "    Inserting \"Hi there, guy!\" at index 2..." << Endl;
  astring = "Hi there, guy!";
  status = strings.Insert(2,astring);
  if (status) goto ERROR;

  // List the current contents...
  if (gVeryVerbose)
    out << "    Final contents are...\n";

  for (i=0;i<strings.Length();i++) {

    status = strings.Get(i,astring);
    if (status) goto ERROR;

    buf[sizeof(buf)-1] = 0;
    status = astring.ToEllipsis((Byte *)buf,sizeof(buf)-1,usedlen);
    if (status) goto ERROR;
    if (gVeryVerbose)
      out << "      strings[" << i << "] = \"" << (const char *)buf << "\"" << Endl;
  }
  
  // Fill up a large string array with strings and clear it
  strings.SetEmpty();
  for (i=0;i<1;i++) {
    String s;
    s << "Element " << i;
    status = strings.Append(s);
    if (status) goto ERROR;
  }
  strings.SetEmpty();
  
  // Handle errors
  status = Error::None;
  while (false) {
    ERROR: out << "!!! Error: Index " << i << ": Error " << status << Endl; break;
    INTERNALERROR: status = Error::Internal;                                break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// TestArray.testStrings - Test Array of Numbers
//

Status TestArray::testNumbers() {

  Number  num;
  Numbers numbers;
  UInt    i;
  Status  status;

  const char *strs[] = {
     "123.456"
    ,"1234567"
    ,"-123e-234"
    ,"3"
    ,"4"
    ,"5"
    ,NULL
  };

  if (gVerbose)
    out << "  Class Numbers..." << Endl;

  // Append the Numbers
  for (i=0; !IsNull(strs[i]); i++) {
    if (gVeryVerbose)
      out << "    Appending " << i << ": " << strs[i] << "..." << Endl;
    num = Number(strs[i]);
    status = numbers.Append(num);
    if (status) goto ERROR;
  }

  // Get each of them and make sure they're in there
  for (i=0; !IsNull(strs[i]); i++) {
    if (gVeryVerbose)
      out << "    Getting " << i << "..." << Endl;
    status = numbers.Get(i,num);
    if (status) goto ERROR;
    if (gVeryVerbose)
      out << "      numbers[" << i << "] = " << num.ToString().MidEllipsis(30) <<  Endl;
  }

  // Remove index 2 ('...')
  if (gVeryVerbose)
    out << "    Removing numbers[2]..." << Endl;
  status = numbers.Remove(2);
  if (status) goto ERROR;

  // Insert "9999" in index 2...
  if (gVeryVerbose)
    out << "    Inserting 9999 at index 2..." << Endl;
  status = numbers.Insert(2,Number("9999"));
  if (status) goto ERROR;

  // List the current contents...
  if (gVeryVerbose)
    out << "    Final contents are...\n";

  for (i=0;i<numbers.Length();i++) {

    status = numbers.Get(i,num);
    if (status) goto ERROR;

    if (gVeryVerbose)
      out << "      numbers[" << i << "] = " << num.ToString() << Endl;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    ERROR: out << "!!! Error: Index " << i << ": Error " << status << Endl; break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// TestArray.testSparse1 - Test Sparse Class one way
// TestArray.testSparse1 - Test Sparse Class another way
//

Status TestArray::testSparse1() {


  Status     status;
  String     msg;

  SparseInts sarray;

  USpan      uSpan;
  USpan      span;
  Int        spanLen;
  Int        index;
  Int        val;

  Int        alen;
  Int        asiz;

  Int        ints[2];
  
  if (gVerbose)
    out << "  Class Sparse 1..." << Endl;

  //
  // Populate Sparse with...
  //
  //   a) For even ones < 5, 1 value
  //   b) For even ones > 5, 2 values
  //   c) For odd ones != 5, 1 value
  //   d) For odd ones == 5, 0 values
  //
  // After even pass...
  //
  //   [0] = 0:0
  //   [1] = 2:20
  //   [2] = 4:40
  //   [3] = 6:60
  //   [4] = 6:60
  //   [5] = 8:80
  //   [6] = 8:80
  //
  // After odd pass...
  //
  //   [ 0] = 0:0
  //   [ 1] = 1:10
  //   [ 2] = 2:20
  //   [ 3] = 3:30
  //   [ 4] = 4:40
  //   [ 5] = 6:60
  //   [ 6] = 6:61
  //   [ 7] = 7:70
  //   [ 8] = 8:80
  //   [ 9] = 8:81
  //   [10] = 9:90
  //

  // Append the even ones...
  for (index=0; index < 10; index+=2) {
    val = index*10;
    //out << "Appending " << val << ": " << tval << Endl;
    sarray.Append(index,val);
    if (index < 5) continue;
    //out << "Appending " << val << ": " << tval << Endl;
    val++;
    sarray.Append(index,val);

  }

  // Append the odd ones (except 5)...
  for (index=1; index < 10; index+=2) {
    if (index == 5) continue;
    val = index*10;
    //out << "Appending " << val << ": " << tval << Endl;
    sarray.Append(index,val);
  }

  // Display the Array's contents...
  if (gVeryVerbose)
    out << "    Array contents...\n";
  for (index=0; (UInt)index < 10; index++) {
    status = sarray._Get(index,(Byte *)ints);
    if (status == Error::IndexNotDistinct && index > 5 && index%2 == 0) continue;
    if (status && index != 5)
      BEFE_ERROR(String("Error getting index")+index,status);
    else if (gVeryVerbose)
      out << "      Array[" << index << "] = " << ints[0] << Endl;
  }

  //
  // Display everything we can
  //

  if (gVeryVerbose)
    out << "    Array Info...\n";
    
  // Display overall info
  alen = sarray.Length();
  if (gVeryVerbose)
    out << "      sarray.Length()         = " << alen << Endl;
  asiz = sarray.Size();
  if (gVeryVerbose)
    out << "      sarray.Size()           = " << asiz << Endl;
  uSpan = sarray.IndexSpan();
  if (gVeryVerbose)
    out << "      sarray.IndexSpan()      = " << uSpan.ToString() << Endl;

  // Display info for each index...
  for (index=0; index < 10; index++) {
    span = sarray.SubIndexSpan(index);
    spanLen = Max(0,span.Length());
    if (gVeryVerbose)
      out << "      sarray.SubIndexSpan(" << index << ") = " << span.ToString() << " (len=" << spanLen << ")\n";
  }

  //
  // Now, manipulate the contents a bit.
  //
  // Given the array, let's do the following...
  //  a) Get rid of [8][1] and [7]
  //  b) Set [8] to 79
  //  c) Set [6][1] to 62
  //
  // This should wind up with this...
  //
  //   [0] = 0:0
  //   [1] = 1:10
  //   [2] = 2:20
  //   [3] = 3:30
  //   [4] = 4:40
  //   [5] = 6:60
  //   [6] = 6:62
  //   [7] = 8:79
  //   [8] = 9:90

  status = sarray.Remove(8,1);
  if (status)
    BEFE_ERROR("Removing [8][1]",status);
  status = sarray.Remove(7,NaN);
  if (status)
    BEFE_ERROR("Removing [7]   ",status);

  val = 79;
  status = sarray.Set(8,val);
  if (status && status != Error::IndexNotDistinct)
    BEFE_ERROR("Setting  [8]   ",status);
  val = 62;
  status = sarray.Set(6,1,val);
  if (status)
    BEFE_ERROR("Setting  [6][1]",status);

  // Display the Array's final contents...
  if (gVeryVerbose)
    out << "    Array contents...\n";
  for (index=0; index < 10; index++) {
    status = sarray.Get(index,ints[0]);
    if (status && index != 5 && index != 7) {
      if (status == Error::IndexNotDistinct && index > 5 && index%2 == 0) continue;
      BEFE_ERROR(String("Getting index ")+index,status);
    }
    else if (!status && gVeryVerbose)
      out << "      Array[" << index << "] = " << ints[0] << Endl;
  }

  status = Error::None;

  return status;

}

Status TestArray::testSparse2() {

  Status      status;
  SparseUInts sparse;
  String      string1;
  String      string2;
  UInt        shash1;
  UInt        ihash1;
  UInt        shash2;
  UInt        ihash2;
  URange      urange;

  if (gVerbose)
    out << "  Class Sparse 2..." << Endl;

  string1 = "String 1";
  shash1   = string1.HashSensitive();
  ihash1   = string1.HashInsensitive();
  string2 = "String 2";
  shash2   = string2.HashSensitive();
  ihash2   = string2.HashInsensitive();
  
  if (gVeryVerbose) {
    out << "    sizeof(String)      = " << sizeof(String)      << '\n';
    out << "    sizeof(Strings)     = " << sizeof(Strings)     << '\n';
    out << "    sizeof(_SparseBase) = " << sizeof(_SparseBase) << '\n';
    out << "    sizeof(_ArrayBase)  = " << sizeof(_ArrayBase)  << '\n';
    out << "    String 1 = '" << string1 << "' SHash = 0x"  << ToHexString(shash1,8) << " IHash = 0x" << ToHexString(ihash1,8) << '\n';
    out << "    String 2 = '" << string2 << "' SHash = 0x"  << ToHexString(shash2,8) << " IHash = 0x" << ToHexString(ihash2,8) << '\n';
  }
  
  // Insert the string hashes...
  sparse.SetEmpty();
  status = sparse.Append(shash1,shash1);
  if (status) goto SOMEERROR;
  status = sparse.Append(ihash1,ihash1);
  if (status) goto SOMEERROR;
  status = sparse.Append(shash2,shash2);
  if (status) goto SOMEERROR;
  status = sparse.Append(ihash2,ihash2);
  if (status) goto SOMEERROR;

  //  
  // Test for existence...
  //
  
  if (!sparse.Exists(shash1))
    BEFE_WARN("String 1 Sensitive DOES NOT EXIST in Sparse");
  else if (gVeryVerbose)
    out << "    String 1 Sensitive   EXISTS in Sparse\n";

  if (!sparse.Exists(ihash1))
    BEFE_WARN("String 1 Insensitive DOES NOT EXIST in Sparse");
  else if (gVeryVerbose)
    out << "    String 1 Insensitive EXISTS in Sparse\n";

  if (!sparse.Exists(shash2))
    BEFE_WARN("String 2 Sensitive DOES NOT EXIST in Sparse");
  else if (gVeryVerbose)
    out << "    String 2 Sensitive   EXISTS in Sparse\n";

  if (!sparse.Exists(ihash2))
    BEFE_WARN("String 2 Insensitive DOES NOT EXIST in Sparse");
  else if (gVeryVerbose)
    out << "    String 2 Insensitive EXISTS in Sparse\n";

  if (sparse.Exists(0))
    BEFE_WARN("Bogus hash '0' exists in Sparse");
  else if (gVeryVerbose)
    out << "    Bogus hash '0' DOES NOT EXIST in Sparse (and rightly so!!!)\n";
  
  //    
  // Handle errors
  //
  
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  // Display error (if any)...
  if (status)
    BEFE_ERROR("TestArray.testSparse2",status);
    
  // Display final statistics
  if (gVeryVerbose)
    out << "    Number of elements: " << sparse.Length() << '\n';
  
  return status;
  
}

//----------------------------------------------------------------------
//
// TestArray.testRemove - Test various Array.Remove flavours
//

Status TestArray::testRemove() {


  Status   status;
  Bytes    bytes;
  Byte    *srcBuf;
  UInt     srcLen;
  Strings  output;
  UInt     i;
  
  char const *str;
  
  if (gVerbose)
    out << "  Array Remove Range/Span/Slice...\n";

  //
  // Remove Range...
  //

  if (gVeryVerbose)
    out << "    Remove Range '9:10'...\n";

  // Populate initial bytes
  str = "0123456789abcdef";
  bytes.Reset();
  status = bytes.Append((Byte *)str, 17);
  if (status) goto SOMEERROR;

  // Display buffer before...
  if (gVeryVerbose) {
    bytes.BufAndSize(srcBuf, srcLen);
    output.SetEmpty();
    output = ToStrings(srcBuf, srcLen, 0);
    out << "      Before...\n";
    for (i=0; i<output.Length(); i++) out << "        " << output.Get(i) << '\n';
  }
    
  // Remove some bytes
  status = bytes.Remove(Range("9..10"));
  if (status) goto SOMEERROR;
  
  // Display buffer after...
  if (gVeryVerbose) {
    bytes.BufAndSize(srcBuf, srcLen);
    output.SetEmpty();
    output = ToStrings(srcBuf, srcLen, 0);
    out << "      After...\n";
    for (i=0; i<output.Length(); i++) out << "        " << output.Get(i) << '\n';
  }
    
  //
  // Remove Span...
  //

  if (gVeryVerbose)
    out << "    Remove Span '10::8'...\n";

  // Populate initial bytes
  str = "0123456789abcdef";
  bytes.Reset();
  status = bytes.Append((Byte *)str, 17);
  if (status) goto SOMEERROR;

  // Display buffer before...
  if (gVeryVerbose) {
    bytes.BufAndSize(srcBuf, srcLen);
    output.SetEmpty();
    output = ToStrings(srcBuf, srcLen, 0);
    out << "      Before...\n";
    for (i=0; i<output.Length(); i++) out << "        " << output.Get(i) << '\n';
  }
    
  // Remove some bytes
  status = bytes.Remove(Span("10:8"));
  if (status) goto SOMEERROR;
  
  // Display buffer after...
  if (gVeryVerbose) {
    bytes.BufAndSize(srcBuf, srcLen);
    output.SetEmpty();
    output = ToStrings(srcBuf, srcLen, 0);
    Cout << "      After...\n";
    for (i=0; i<output.Length(); i++) out << "        " << output.Get(i) << '\n';
  }
   
  //
  // Remove Slice...
  //

  if (gVeryVerbose)
    out << "    Remove Slice '-1:-17:2'...\n"  ;
  // Populate initial bytes
  str = "0123456789abcdef";
  bytes.Reset();
  status = bytes.Append((Byte *)str, 17);
  if (status) goto SOMEERROR;

  // Display buffer before...
  if (gVeryVerbose) {
    bytes.BufAndSize(srcBuf, srcLen);
    output.SetEmpty();
    output = ToStrings(srcBuf, srcLen, 0);
    out << "       Before...\n";
    for (i=0; i<output.Length(); i++) out << "        " << output.Get(i) << '\n';
  }
    
  // Remove some bytes
  status = bytes.Remove(Slice("-1:-17:2"));
  if (status) goto SOMEERROR;
  
  // Display buffer after...
  if (gVeryVerbose) {
    bytes.BufAndSize(srcBuf, srcLen);
    output.SetEmpty();
    output = ToStrings(srcBuf, srcLen, 0);
    out << "      After...\n";
    for (i=0; i<output.Length(); i++) out << "        " << output.Get(i) << '\n';
  }
    
  //
  // Handle errros
  //
  
  while (false) {
    SOMEERROR: break;
  }

  if (status)
    BEFE_ERROR("TestArray.testRemove",status);
 
  return status;  

}

//----------------------------------------------------------------------
//
// TestArray.testCopy - Test Array Copy method
//

Status TestArray::testCopy() {

  Status        status;
  Strings       strings;
  Strings       stringsCopy;
  SparseStrings sstrings;
  SparseStrings sstringsCopy;
  String        str;
  
  if (gVerbose)
    out << "  Strings/SparseStrings Copy..." << Endl;

  // Populate the arrays...
  if (gVeryVerbose)
    out << "    Populating Strings...\n";
  status = strings.Append(String("String 1"));
  if (status) goto SOMEERROR;
  status = strings.Append(String("String 2"));
  if (status) goto SOMEERROR;
  status = strings.Append(String("String 3"));
  if (status) goto SOMEERROR;

  if (gVeryVerbose)
    out << "    Populating SparseStrings...\n";
  status = sstrings.Set(1,String("String 1"));
  if (status) goto SOMEERROR;
  status = sstrings.Set(3,String("String 3"));
  if (status) goto SOMEERROR;
  status = sstrings.Set(5,String("String 5"));
  if (status) goto SOMEERROR;

  // Copy them
  if (gVeryVerbose)
    out << "    Copying Strings...\n";
  status = stringsCopy.CopyFrom(strings);
  if (status) goto SOMEERROR;
  if (gVeryVerbose)
    out << "    Copying Sparse Strings...\n";
  status = sstringsCopy.CopyFrom(sstrings);
  if (status) goto SOMEERROR;

  // Just to make sure, remove something from original ones
  if (gVeryVerbose)
    out << "    Removing from Strings...\n";
  status = strings.Remove(0);
  if (status) goto SOMEERROR;
  if (gVeryVerbose)
    out << "    Removing from SparseStrings...\n";
  status = sstrings.Remove(1);
  if (status) goto SOMEERROR;
  if (stringsCopy.Length() != 3)  BEFE_WARN("Strings copy not correct length");
  if (sstringsCopy.Length() != 3) BEFE_WARN("SparseStrings copy not correct length");

  // Make sure the copys are intact
  if (gVeryVerbose)
    out << "    Checking integrity of copies...\n";
  str = stringsCopy.Get(0);
  if (str != "String 1") BEFE_WARN("Copy of Strings(0) not intact");
  str = sstringsCopy.Get(1);
  if (str != "String 1") BEFE_WARN("Copy of SparseStrings(1) not intact");

  // Make sure the originals are intact
  if (gVeryVerbose)
    out << "    Checking integrity of originals...\n";
  status = strings.Get(0, str);
  if (status) goto SOMEERROR;
  status = strings.Get(3, str);
  if (!status) BEFE_WARN("Expected error from Strings.Get(3), got None");
  
  status = sstrings.Get(3, str);
  if (status) goto SOMEERROR;
  status = sstrings.Get(1, str);
  if (!status) BEFE_WARN("Expected error from SparseStrings.Get(1), got None");
  
  //
  // Handle errros
  //
  
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }

  if (status)
    BEFE_ERROR("TestArray.testCopy",status);
 
  return status;  

}

//----------------------------------------------------------------------
//
// TestArray.testReverse - Test Array Reverse method
//

Status TestArray::testReverse() {

  Status  status;
  Strings strings;
  UInt    numStrings;
  UInt    curIdx;
  String  str;
  
  if (gVerbose)
    out << "  Strings Reverse..." << Endl;

  // Populate the array...
  if (gVeryVerbose)
    out << "    Populating Strings...\n";
  status = strings.Append(String("String 1"));
  if (status) goto SOMEERROR;
  status = strings.Append(String("String 2"));
  if (status) goto SOMEERROR;
  status = strings.Append(String("String 3"));
  if (status) goto SOMEERROR;

  // Print "Before"...
  if (gVeryVerbose) {
    numStrings = strings.Length();
    for (curIdx=0; curIdx < numStrings; curIdx++) {
      str = strings.Get(curIdx);
      if (curIdx == 0)
        out << "    Before: strings[" << curIdx << "] = '" << str << "'\n";
      else
        out << "            strings[" << curIdx << "] = '" << str << "'\n";
    }
    
  }
  
  // Reverse them
  if (gVeryVerbose)
    out << "    Reversing Strings...\n";
  status = strings.Reverse();
  if (status) goto SOMEERROR;

  // Print "After"...
  if (gVeryVerbose) {
    numStrings = strings.Length();
    for (curIdx=0; curIdx < numStrings; curIdx++) {
      str = strings.Get(curIdx);
      if (curIdx == 0)
        out << "    After:  strings[" << curIdx << "] = '" << str << "'\n";
      else
        out << "            strings[" << curIdx << "] = '" << str << "'\n";
    }    
  }
  
  
  //
  // Handle errros
  //
  
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }

  if (status)
    BEFE_ERROR("TestArray.testReverse",status);
 
  return status;  

}

//----------------------------------------------------------------------
//
// TestArray.testIntMap - Test IntMapPrimitive<T> and IntMapValue<V> templates
//

Status TestArray::testUIntMap() {

  Status                status;
  UIntMapPrimitive<Int> map;
  UIntMapValue<String>  smap;
  UIntMapValue<String>  smapCopy;
  Boolean               exists;
  Int                   val;
  String                str;
  UInt                  numErrors;

  UInts                 intKeys;
  UInt                  numKeys;
  UInt                  curIdx;
  
  if (gVerbose)
    out << "  IntMapPrimitive<T> and IntMapValue<V>..." << Endl;
  numErrors = 0;
  
  //
  // Test IntMapPrimitive<Int>...
  //
  
  // Display "Before" state
  if (gVeryVerbose)
    out << "    IntMapPrimitive Before: map.Length = " << map.Length() << '\n';
  exists = map.Exists(2);
  if (gVeryVerbose)
    out << "    IntMapPrimitive Before: Element 2 Exists() = " << (exists?"true":"false") << '\n';
  
  // Set the value
  status = map.Set(1,10);
  if (status) { BEFE_ERROR("map.Set",status); numErrors++; }
  status = map.Set(3,30);
  if (status) { BEFE_ERROR("map.Set",status); numErrors++; }
  status = map.Set(2,20);
  if (status) { BEFE_ERROR("map.Set",status); numErrors++; }
  if (gVeryVerbose)
    out << "    IntMapPrimitive Middle: map.Length = " << map.Length() << '\n';
  exists = map.Exists(2);
  if (gVeryVerbose)  
    out << "    IntMapPrimitive Middle: Element 2 Exists() = " << (exists?"true":"false") << '\n';
  
  // Get the value
  val = NaN;
  status = map.Get(2,val);
  if (status) { BEFE_ERROR("map.Get",status); numErrors++; }
  else if (val != 20) {
    BEFE_WARN(String("IntMapPrimitive Middle: Expected map[2] to be 20, is ") + val + '\n');
    numErrors++;
  }

  // Remove a value
  if (gVeryVerbose)  
    out << "    IntMapPrimitive Middle: Removing map[2]...\n";
  status = map.Remove(2);
  if (status)
    BEFE_ERROR("map.Remove",status);

  // Display "After" state
  if (gVeryVerbose)
    out << "    IntMapPrimitive After:  map.Length = " << map.Length() << '\n';
  exists = map.Exists(2);
  if (gVeryVerbose)
    out << "    IntMapPrimitive After:  Element 2 Exists() = " << (exists?"true":"false") << '\n';
  exists = map.Exists(3);
  if (gVeryVerbose)
    out << "    IntMapPrimitive After:  Element 3 Exists() = " << (exists?"true":"false") << '\n';
  
  // Display the keys
  intKeys = map.Keys();
  numKeys = intKeys.Length();
  for (curIdx=0; curIdx < numKeys; curIdx++) {
    val = intKeys.Get(curIdx);
    if (gVeryVerbose) {
      if (curIdx == 0)
        out << "    IntMapPrimitive Keys:   intKeys[0] = " << val << '\n';
      else
        out << "                            intKeys[" << curIdx << "] = " << val << '\n';
    }
  }
  
  //
  // Test IntMapValue<String>...
  //
  
  // Display "Before" state
  if (gVeryVerbose)
    out << "    IntMapValue     Before: map.Length = " << smap.Length() << '\n';
  exists = smap.Exists(2);
  if (gVeryVerbose)
    out << "    IntMapValue     Before: Element 2 Exists() = " << (exists?"true":"false") << '\n';
  
  // Set the value
  status = smap.Set(2,String("20"));
  if (status) BEFE_ERROR("map.Set",status);
  status = smap.Set(1,String("10"));
  if (status) BEFE_ERROR("map.Set",status);
  status = smap.Set(3,String("0123456789abcdef"));
  if (status) BEFE_ERROR("map.Set",status);
  if (gVeryVerbose)
    out << "    IntMapValue     Middle: map.Length = " << smap.Length() << '\n';
  exists = smap.Exists(2);
  if (gVeryVerbose)
    out << "    IntMapValue     Middle: Element 2 Exists() = " << (exists?"true":"false") << '\n';
  
  // Get the value
  str.Reset();
  status = smap.Get(2,str);
  if (status)
    BEFE_ERROR("smap.Get",status);
  else if (str != "20")
    BEFE_WARN(String("IntMapValue     Middle: Expected map[2] to be \"20\", is \"") + str + "\"\n");

  // Take a copy first
  smapCopy = smap;
  
  // Remove the value
  status = smap.Remove(2);
  if (status)
    BEFE_ERROR("smap.Remove",status);

  str.Reset();
  status = smap.Get(3,str);
  if (status)
    BEFE_ERROR("smap.Get",status);
  else if (str != "0123456789abcdef")
    BEFE_WARN(String("IntMapValue     Middle: Expected map[3] to be \"0123456789abcdef\", is \"") + str + "\"\n");
    
  // Display "After" state
  if (gVeryVerbose)
    out << "    IntMapValue     After:  map.Length = " << map.Length() << '\n';
  exists = smap.Exists(2);
  if (gVeryVerbose)
    out << "    IntMapValue     After:  Element 2 Exists() = " << (exists?"true":"false") << '\n';
  exists = smap.Exists(3);
  if (gVeryVerbose)
    out << "    IntMapValue     After:  Element 3 Exists() = " << (exists?"true":"false") << '\n';
    
  // Display the keys
  intKeys = smap.Keys();
  numKeys = intKeys.Length();
  for (curIdx=0; curIdx < numKeys; curIdx++) {
    val = intKeys.Get(curIdx);
    if (gVeryVerbose) {
      if (curIdx == 0)
        out << "    IntMapValue     Keys:   intKeys[0] = " << val << '\n';
      else
        out << "                            intKeys[" << curIdx << "] = " << val << '\n';
    }
  }
  
  return (numErrors)? Error::Failed : Error::None;

}

//----------------------------------------------------------------------
//
// TestArray.testStringMap - Test StringMapPrimitive<T> and StringMapValue<V> templates
//

Status TestArray::testStringMap() {
  
  Status                  status;
  StringMapPrimitive<Int> map;
  StringMapPrimitive<Int> mapCopy;
  StringMapValue<String>  smap;
  StringMapValue<String>  smapCopy;
  Boolean                 exists;
  Int                     val;
  String                  sVal;
  UInt                    numErrors;
  
  Strings                 strKeys;
  UInt                    numKeys;
  UInt                    curIdx;
  
  if (gVerbose)
    out << "  StringMapPrimitive<T> and StringMapValue<V>..." << Endl;
  numErrors = 0;
  
  //
  // Test StringMapPrimitive<Int>...
  //
  
  // Display "Before" state
  if (gVeryVerbose)
    out << "    StringMapPrimitive Before: map.Length = " << map.Length() << '\n';
  exists = map.Exists("two");
  if (gVeryVerbose)
    out << "    StringMapPrimitive Before: Element 'two' Exists()   = " << (exists?"true":"false") << '\n';
  
  // Set the value
  status = map.Set("one",10);
  if (status) { BEFE_ERROR("map.Set",status); numErrors++; }
  status = map.Set("three",30);
  if (status) { BEFE_ERROR("map.Set",status); numErrors++; }
  status = map.Set("two"  ,20);
  if (status) { BEFE_ERROR("map.Set",status); numErrors++; }

  if (gVeryVerbose)
    out << "    StringMapPrimitive Middle: map.Length = " << map.Length() << '\n';
  exists = map.Exists("two");
  if (gVeryVerbose)  
    out << "    StringMapPrimitive Middle: Element 'two' Exists()   = " << (exists?"true":"false") << '\n';
  
  // Get the value
  val = NaN;
  status = map.Get("two",val);
  if (status) { BEFE_ERROR("map.Get",status); numErrors++; }
  else if (val != 20) {
    BEFE_WARN(String("StringMapPrimitive Middle: Expected smap['two'] to be 20, is '") + val + "'\n");
    numErrors++;
  }

  // Take a copy first
  mapCopy = map;
  
  // Remove a value
  if (gVeryVerbose)  
    out << "    StringMapPrimitive Middle: Removing smap['two']...\n";
  status = map.Remove("two");
  if (status)
    BEFE_ERROR("smap.Remove",status);

  // Display "After" state
  if (gVeryVerbose)
    out << "    StringMapPrimitive After:  map.Length = " << map.Length() << '\n';
  exists = map.Exists("two");
  if (gVeryVerbose)
    out << "    StringMapPrimitive After:  Element 'two' Exists()   = " << (exists?"true":"false") << '\n';
  exists = map.Exists("three");
  if (gVeryVerbose)
    out << "    StringMapPrimitive After:  Element 'three' Exists() = " << (exists?"true":"false") << '\n';
        
  // Make sure copy still has Element 'two'...
  val = NaN;
  status = mapCopy.Exists("two");
  if (gVeryVerbose)
    out << "    StringMapPrimitive After:  Copy    'two' Exists()   = " << (exists?"true":"false") << '\n';
  status = mapCopy.Get("two",val);
  if (status) { BEFE_ERROR("map.Get",status); numErrors++; }
  else if (val != 20) {
    BEFE_WARN(String("StringMapPrimitive Middle: Expected mapCopy['two'] to be 20, is '") + val + "'\n");
    numErrors++;
  }
  
  // Display the keys
  strKeys = map.Keys();
  numKeys = strKeys.Length();
  for (curIdx=0; curIdx < numKeys; curIdx++) {
    sVal = strKeys.Get(curIdx);
    if (gVeryVerbose) {
      if (curIdx == 0)
        out << "    StringMapPrimitive Keys:   strKeys[0] = '" << sVal << "'\n";
      else
        out << "                               strKeys[" << curIdx << "] = '" << sVal << "'\n";
    }
  }
  
  //
  // Test StringMapValue<String>...
  //
  
  // Display "Before" state
  if (gVeryVerbose)
    out << "    StringMapValue     Before: smap.Length = " << map.Length() << '\n';
  exists = smap.Exists("two");
  if (gVeryVerbose)
    out << "    StringMapValue     Before: Element 'two' Exists()   = " << (exists?"true":"false") << '\n';
  
  // Set the value
  status = smap.Set("one","ten");
  if (status) { BEFE_ERROR("smap.Set",status); numErrors++; }
  status = smap.Set("three","thirty");
  if (status) { BEFE_ERROR("smap.Set",status); numErrors++; }
  status = smap.Set("two"  ,"twenty");
  if (status) { BEFE_ERROR("smap.Set",status); numErrors++; }

  if (gVeryVerbose)
    out << "    StringMapValue     Middle: smap.Length = " << map.Length() << '\n';
  exists = smap.Exists("two");
  if (gVeryVerbose)  
    out << "    StringMapValue     Middle: Element 'two' Exists()   = " << (exists?"true":"false") << '\n';
  
  // Get the value
  sVal.Reset();
  status = smap.Get("two",sVal);
  if (status) { BEFE_ERROR("smap.Get",status); numErrors++; }
  else if (val != 20) {
    BEFE_WARN(String("StringMapPrimitive Middle: Expected map['two'] to be 'twenty', is '") + sVal + "'\n");
    numErrors++;
  }

  // Take a copy first
  smapCopy = smap;
  
  // Remove a value
  if (gVeryVerbose)  
    out << "    StringMapValue     Middle: Removing map['two']...\n";
  status = smap.Remove("two");
  if (status)
    BEFE_ERROR("smap.Remove",status);

  // Display "After" state
  if (gVeryVerbose)
    out << "    StringMapValue     After:  smap.Length = " << map.Length() << '\n';
  exists = smap.Exists("two");
  if (gVeryVerbose)
    out << "    StringMapValue     After:  Element 'two' Exists()   = " << (exists?"true":"false") << '\n';
  exists = smap.Exists("three");
  if (gVeryVerbose)
    out << "    StringMapValue     After:  Element 'three' Exists() = " << (exists?"true":"false") << '\n';

  // Make sure copy still has Element 'two'...
  sVal.Reset();
  status = mapCopy.Exists("two");
  if (gVeryVerbose)
    out << "    StringMapPrimitive After:  Copy    'two' Exists()   = " << (exists?"true":"false") << '\n';
  status = smapCopy.Get("two",sVal);
  if (status) { BEFE_ERROR("map.Get",status); numErrors++; }
  else if (sVal != "twenty") {
    BEFE_WARN(String("StringMapPrimitive Middle: Expected mapCopy['two'] to be 'twenty', is '") + val + "'\n");
    numErrors++;
  }
  
  // Display the keys
  strKeys = smap.Keys();
  numKeys = strKeys.Length();
  for (curIdx=0; curIdx < numKeys; curIdx++) {
    sVal = strKeys.Get(curIdx);
    if (gVeryVerbose) {
      if (curIdx == 0)
        out << "    StringMapValue     Keys:   strKeys[0] = '" << sVal << "'\n";
      else
        out << "                               strKeys[" << curIdx << "] = '" << sVal << "'\n";
    }
  }
  
  return (numErrors)? Error::Failed : Error::None;

}

//----------------------------------------------------------------------
//
// TestArray.testSort - Test Array Sorting
//

Status TestArray::testSort() {

  Status status;
  UInt    numErrors;  
  Strings strs;
  UInt    curIdx;
  UInt    numStr;
  
  char const *constStrings[] = {
    "xyz",
    "Hi there",
    "aaa",
    "Aaaa",
    0 };

  if (gVerbose)
    out << "  Array.Sort..." << Endl;

  numErrors = 0;
  numStr    = 0;
  
  // Populate the strings
  for (curIdx=0; constStrings[curIdx]; curIdx++) {
    status = strs.Append(constStrings[curIdx]);
    if (status) goto SOMEERROR;
    numStr++;
  }
  
  if (strs.Length() != numStr)
    BEFE_WARN("TestArray.testSort: strs is wrong length");
    
  // Display
  if (gVerbose)
    out << "    Before Default Sort...\n";
  if (gVeryVerbose) {  
    for (curIdx=0; curIdx < strs.Length(); curIdx++)
      out << "      strs[" << curIdx << "] = '" << strs.Get(curIdx) << "'\n";  
  }
  
  // Sort it
  status = strs.Sort();
  if (status) goto SOMEERROR;
  
  // Display
  if (gVerbose)
    out << "    After Default Sort...\n";
  if (gVeryVerbose) {  
    for (curIdx=0; curIdx < strs.Length(); curIdx++)
      out << "      strs[" << curIdx << "] = '" << strs.Get(curIdx) << "'\n";  
  }

  // Make Insensitive and sort it again
  status = strs.Insensitive();
  if (status) goto SOMEERROR;
  status = strs.Sort();
  if (status) goto SOMEERROR;
  
  // Display
  if (gVerbose)
    out << "    After Insensitive Sort...\n";
  if (gVeryVerbose) {  
    for (curIdx=0; curIdx < strs.Length(); curIdx++)
      out << "      strs[" << curIdx << "] = '" << strs.Get(curIdx) << "'\n";  
  }

  // Make Sensitive and sort it again
  status = strs.Sensitive();
  if (status) goto SOMEERROR;
  status = strs.Sort();
  
  // Display
  if (gVerbose)
    out << "    After Sensitive Sort...\n";
  if (gVeryVerbose) {  
    for (curIdx=0; curIdx < strs.Length(); curIdx++)
      out << "      strs[" << curIdx << "] = '" << strs.Get(curIdx) << "'\n";  
  }

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  if (status)
    BEFE_ERROR("TestArray.testSort",status);
    
  return (numErrors)? Error::Failed : Error::None;
  
}

//----------------------------------------------------------------------
//
// TestArray.testNamedStrings - Test NamedStrings
//

Status TestArray::testNamedStrings() {

  NamedStrings vars;
  Strings      names;
  String       name;
  String       value;
  UInt         maxNameLen;
  UInt         curIdx;
  
  if (gVerbose)
    out << "  Array.NamedStrings..." << Endl;

  vars = TheBefe->TheProcess->GetEnvironmentValues();
  names = vars.Keys();
  
  // Figure out maximum Key length...
  maxNameLen=0;
  for (curIdx=0; curIdx < names.Length(); curIdx++)
    maxNameLen = Max(maxNameLen, names.Get(curIdx).Length());
  
  // Display them if requested
  if (gVeryVerbose) {
    out << "    There are " << names.Length() << " Environment Variables...\n";  
    for (curIdx=0; curIdx < names.Length(); curIdx++)
      out << "    " << names.Get(curIdx).Pad(maxNameLen) << " = " << vars.Get(names.Get(curIdx)).Ellipsis(40) << '\n';
    out << '\n';
  }
  
  // Try Key Insensitivity check...
  name = "pAtH";
  value = vars.Get(name);
  if (gVeryVerbose)
    out << "    Key \'" << name << "' = '" << vars.Get(name).Ellipsis(40) << "'\n";
    
  return Error::None;
  
}

} // ...Namespace BEFE
