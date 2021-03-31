//!befe-publish src
//----------------------------------------------------------------------
// File: TestString.cpp - Implementation of the TestString Class
//----------------------------------------------------------------------
//
// This Class implements the BEFE String Class Unit Tests.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "SBuffer.h"
#include "Process.h"

#include "Test.h"
#include "TestString.h"


namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Method: Run - Main Test 
//

Status TestString::Run() { // Run...

  if (!gQuiet)
    out << "***Testing String...\n";

  testSBuffer();
  testConstruct();
  testOpEquals();
  testOpPlusEquals();
  testOpPlus();
  testOpMultiply();
  testOpLeftShift();
  testManip();
  testOpCompares();
  testFindChar(true);
  testFindChar(false);
  testFindString(true);
  testFindString(false);
  testLogToPhySpan();
  testLogToPhyRange();
  testRemoveRange();
  testRemoveSpan();
  testInsertChar();
  testInsertString();
  testReplaceInt();
  testReplaceRange();
  testReplaceSpan();
  testSplit();
  testGeneral();
  testReverse();
  testOpSubscript();

  return Error::None;
  
} // ...Run

//----------------------------------------------------------------------
//
// Method: testConstruct - Test String Constructors
//

void TestString::testConstruct() { // testConstruct...

  String a;
  String b = "abc";
  String c(b);
  String d((Byte *)"def",3);
  String e((char *)"!!!Hello ∏ World!!!");

  String *string[] = {&a,&b,&c,&d,&e,NULL};
  UInt    sIdx;
  char    name[2];

  Byte     buf[11];
  UInt     usedlen;
  Status   status;

  sIdx = 0;
  name[0] = 'a';
  name[1] = 0;

  if (gVerbose)
    out  << "  String Constructors..." << Endl;
  if (gVeryVerbose)
    out  << "    sizeof(String) = " << sizeof(String) << Endl;
  
  while (!IsNull(string[sIdx])) {

    if (gVeryVerbose)
      out  << "    Construct " << name << " ..." << Endl;

    if (gVeryVerbose) {
      out << "      " << indent << "  " << name << ".IsMutable()  = "
                                << ((string[sIdx]->IsMutable())?"true":"false") << '\n';
      out << "      " << indent << "  " << name << ".Size()       = " << string[sIdx]->Size()  << '\n';
      out << "      " << indent << "  " << name << ".Length()     = " << string[sIdx]->Length() << '\n';
      out << "      " << indent << "  " << name << ".Hash()       = " << string[sIdx]->Hash()   << '\n';
    }
      
    status = string[sIdx]->ToEllipsis(buf,sizeof(buf)-1,usedlen);
    if (status) {
      out << "!!! Status " << status << " from String.ToBytes()" << Endl;
      goto DONE;
    }

    buf[sizeof(buf)-1] = 0;
    if (gVeryVerbose)
      out  << "        " << name << " = \"" << (char *)buf << "\" usedlen=" << usedlen << Endl;

    sIdx++;
    name[0]++;
    
  }

  while (false) {
    DONE: break;
  }

  return;

} // ...testConstruct

//----------------------------------------------------------------------
//
// Method: testOpEquals - Test String operator=
//

void TestString::testOpEquals() { // testOpEquals...

  Byte     buf[21];
  UInt     usedlen;
  Status   status;

  String s1;
  String s2;

  if (gVerbose)
    out  << "  String operator= ..." << Endl;

  s1 = "First string";
  status = s1.ToEllipsis(buf,sizeof(buf),usedlen);
  if (status) {
    out << "!!! Status " << status << " from String.ToEllipsis()" << Endl;
    goto DONE;
  }
  if (gVeryVerbose)
    out  << "    s1 = \"" << (char *)buf << "\" usedlen=" << usedlen << Endl;

  s2 = (char *)"Second string";
  status = s2.ToEllipsis(buf,sizeof(buf),usedlen);
  if (status) {
    out << "!!! Status " << status << " from String.ToEllipsis()" << Endl;
    goto DONE;
  }
  if (gVeryVerbose)
    out  << "    s2 = \"" << (char *)buf << "\" usedlen=" << usedlen << Endl;

  s2 = s1;
  s1 = "xxx";
  status = s2.ToEllipsis(buf,sizeof(buf),usedlen);
  if (status) {
    out << "!!! Status " << status << " from String.ToEllipsis()" << Endl;
    goto DONE;
  }
  if (gVeryVerbose)
    out  << "    s2 = \"" << (char *)buf << "\" usedlen=" << usedlen << Endl;
  status = s1.ToEllipsis(buf,sizeof(buf),usedlen);
  if (status) {
    out << "!!! Status " << status << " from String.ToEllipsis()" << Endl;
    goto DONE;
  }
  if (gVeryVerbose)
    out  << "      (was created from s1, which was changed to \"" << (char *)buf << "\" afterwards)" << Endl;

  while (false) {
    DONE: break;
  }

  return;

} // ...testOpEquals

//----------------------------------------------------------------------
//
// Method: testOpPlusEquals - Test String operator+=
//

void TestString::testOpPlusEquals() { // testOpPlusEquals...

  String s1;
  String s2;
  String s3;
  String *strings[] = {&s1, &s2, &s3 ,NULL};
  Int    idx;
  Status status;

  Byte   buf[256];
  UInt   usedlen;

  if (gVerbose)
    out  << "  String operator+= ..." << Endl;

  s1 = "String 1";
  s2 = "String 2";
  s3 += s1;
  s3 += s2;

  for (idx=0; !IsNull(strings[idx]); idx++) {
    status = strings[idx]->ToEllipsis(buf,sizeof(buf),usedlen);
    if (status) goto ERROR;
    if (gVeryVerbose)
      out  << "    s" << idx+1 << " = \"" << (char *)buf << "\"" << Endl;
  }

  while (false) {
    ERROR: out << "!!! Status " << status << " from String.ToEllipsis()" << Endl;
  }

  return;

} // ...testOpPlusEquals

//----------------------------------------------------------------------
//
// Method: testOpPlus - Test String operator+
//

void TestString::testOpPlus() { // testOpPlus...

  String s1;
  String s2;
  String s3;
  String *strings[] = {&s1, &s2, &s3 ,NULL};
  Int    idx;
  Status status;

  Byte   buf[256];
  UInt   usedlen;

  if (gVerbose)
    out  << "  String operator+ ..." << Endl;

  s1 = "1x";
  s2 = "2";
  s3 = s1 + s2;

  for (idx=0; !IsNull(strings[idx]); idx++) {
    status = strings[idx]->ToEllipsis(buf,sizeof(buf),usedlen);
    if (status) goto ERROR;
    if (gVeryVerbose)
      out  << "    s" << idx+1 << " = \"" << (char *)buf << "\"" << Endl;
  }

  while (false) {
    ERROR: out << "!!! Status " << status << " from String.ToEllipsis()" << Endl;
  }

  return;

} // ...testOpPlus

//----------------------------------------------------------------------
//
// Method: testOpMultiply - Test String operator*
//

void TestString::testOpMultiply() { // testOpMultiply...

  String s;
  Byte   buf[256];
  Status status;
  UInt   usedlen;

  if (gVerbose)
    out  << "  String operator* ..." << Endl;

  s = String("Hi!")*3;
  status = s.ToEllipsis(buf,sizeof(buf),usedlen);
  if (status) goto ERROR;
  if (gVeryVerbose)
    out  << "    String(\"Hi!\")*3 = \"" << (char *)buf << "\"" << Endl;

  s = String("-")*50;
  status = s.ToEllipsis(buf,sizeof(buf),usedlen);
  if (status) goto ERROR;
  if (gVeryVerbose)
    out  << "    String(\"-\")*50  = \"" << (char *)buf << "\"" << Endl;

  while (false) {
    ERROR: out << "!!! Status " << status << " from String.ToEllipsis()" << Endl;
  }

  return;

} // ...testOpMultiply

//----------------------------------------------------------------------
//
// Method: testOpLeftshirt - Test String operator<<
//

void TestString::testOpLeftShift() { // testOpLeftShift...

  String s;
  String s2;
  Status status;

  Byte   buf[256];
  UInt   usedlen;

  if (gVerbose)
    out  << "  String operator<< ..." << Endl;

  s << "abc<<";
  s << ',' << 123 << ",0x" << (void *)0xcafebabe << "," << 'x' << ',' << false;

  status = s.ToEllipsis(buf,sizeof(buf),usedlen);
  if (status) goto ERROR;
  if (gVeryVerbose)
    out  << "    operator<<  status = \"" << (char *)buf << "\"" << Endl;

  s2 <<= "abc<<=";
  s2 <<= ',';
  s2 <<= 123;
  s2 <<= ",0x";
  s2 <<= (void *)0xbefef00d;
  s2 <<= ",";
  s2 <<= 'x';
  s2 <<= ',';
  s2 <<= false;

  status = s2.ToEllipsis(buf,sizeof(buf),usedlen);
  if (status) goto ERROR;
  if (gVeryVerbose)
    out  << "    operator<<= status = \"" << (char *)buf << "\"" << Endl;

  s.SetEmpty();
  // s = "";
  s <<= (Int)NaN;
  status = s.ToEllipsis(buf,sizeof(buf),usedlen);
  if (status) goto ERROR;
  if (gVeryVerbose)
    out  << "    Nan = " << (char *)buf << Endl;

  while (false) {
    ERROR: out << "!!! Status " << status << " from String.ToEllipsis()" << Endl;
  }

  return;

} // ...testOpLeftShift

//----------------------------------------------------------------------
//
// Method: testOpCompares - Test String operator== and operator!=
//
// NOTE: This is a noddy compare at the moment...  It only compares with
//       const char *.  We'll need to beef this up once we get the less
//       then and greater than comparisons implemented.

void TestString::testOpCompares() { // testOpCompares...

  String string;

  if (gVerbose)
    out  << "  String Comparison ..." << Endl;

  string = String("abc");

  if (string != "abc")
    out  << "  !!! \"abc\" != \"abc\"!!!" << Endl;
  else if (gVeryVerbose)
    out  << "    \"abc\" == \"abc\"" << Endl;

  if (string == "def")
    out  << "  !!! \"abc\" == \"def\"!!!" << Endl;
  else if (gVeryVerbose)
    out  << "    \"abc\" != \"def\"" << Endl;

  return;

} // ...testOpCompares

//----------------------------------------------------------------------
//
// Method: testSBuffer - Test SBuffer Class
//

void TestString::testSBuffer() { // testSBuffer...

  Status        status;
  SBuffer       thebuf;
  const char   *teststring = "0123456789ABCDEF";
  const char   *testutf8   = "∏";
  Int           usedbytes;
  Int           charcount;
  Int           bytecount;

  if (gVerbose)
    out  << "  SBuffer class ..." << Endl;

  // Allocate a buffer
  status = thebuf.Allocate(UNaN,UNaN);
  if (status) goto ERROR;
  if (gVeryVerbose)
    out  << "    Allocated buffer at 0x" << (void *)thebuf.pointer
        << " alloc=" << thebuf.GetAllocBytes()
        << " inc="   << thebuf.GetIncrementBytes()
        << " used="  << thebuf.GetUsedBytes()
        << Endl;

  // Append to the buffer
  status = thebuf.AppendBytes((Byte *)teststring,Strlen(teststring));
  if (status) goto ERROR;
  if (gVeryVerbose) {
    out  << "    Appended buffer at 0x" << (void *)thebuf.pointer
        << " alloc=" << thebuf.GetAllocBytes()
        << " inc="   << thebuf.GetIncrementBytes()
        << " used="  << thebuf.GetUsedBytes()
        << Endl;
  }

  // Resize the buffer
  status = thebuf.Resize(50);
  if (status) goto ERROR;
  if (gVeryVerbose)
    out  << "    Resized buffer at 0x" << (void *)thebuf.pointer
        << " alloc=" << thebuf.GetAllocBytes()
        << " inc="   << thebuf.GetIncrementBytes()
        << " used="  << thebuf.GetUsedBytes()
        << Endl;

  // Append more to the buffer
  status = thebuf.AppendBytes((Byte *)teststring,Strlen(teststring));
  if (status) goto ERROR;
  status = thebuf.AppendBytes((Byte *)teststring,Strlen(teststring)+1);
  if (status) goto ERROR;
  if (gVeryVerbose) {
    out  << "    Appended buffer at 0x" << (void *)thebuf.pointer
        << " alloc=" << thebuf.GetAllocBytes()
        << " inc="   << thebuf.GetIncrementBytes()
        << " used="  << thebuf.GetUsedBytes()
        << Endl;
    out  << "    Appended buffer Contains: '" << (char *)thebuf.GetBuf() << "'" << Endl;
  }

  // Remove bytes from the buffer
  status = thebuf.RemoveBytes(16,16);
  if (status) goto ERROR;
  if (gVeryVerbose) {
    out  << "    Removed buffer at 0x" << (void *)thebuf.pointer
        << " alloc=" << thebuf.GetAllocBytes()
        << " inc="   << thebuf.GetIncrementBytes()
        << " used="  << thebuf.GetUsedBytes()
        << Endl;
    out  << "    Removed buffer Contains: '" << (char *)thebuf.GetBuf() << "'" << Endl;
  }

  // Insert non-hex digit bytes back into the middle
  status = thebuf.InsertBytes(16,(Byte *)"abcdefghijklmnop",16);
  if (status) goto ERROR;
  if (gVeryVerbose) {
    out  << "    Inserted buffer at 0x" << (void *)thebuf.pointer
        << " alloc=" << thebuf.GetAllocBytes()
        << " inc="   << thebuf.GetIncrementBytes()
        << " used="  << thebuf.GetUsedBytes()
        << Endl;
    out  << "    Inserted buffer Contains: '" << (char *)thebuf.GetBuf() << "'" << Endl;
  }

  // Replace those with "xxxx"...
  status = thebuf.ReplaceBytes(16,16,(Byte *)"xxxx",4);
  if (status) goto ERROR;
  if (gVeryVerbose) {
    out  << "    Replaced buffer at 0x" << (void *)thebuf.pointer
        << " alloc=" << thebuf.GetAllocBytes()
        << " inc="   << thebuf.GetIncrementBytes()
        << " used="  << thebuf.GetUsedBytes()
        << Endl;
    out  << "    Replaced buffer Contains: '" << (char *)thebuf.GetBuf() << "'" << Endl;
  }

  // Insert a UTF-8 character in the string
  status = thebuf.InsertBytes(0,(Byte *)testutf8,Strlen(testutf8));
  if (status) goto ERROR;
  if (gVeryVerbose) {
    out  << "      UTF-8 buffer at 0x" << (void *)thebuf.pointer
        << " alloc=" << thebuf.GetAllocBytes()
        << " inc="   << thebuf.GetIncrementBytes()
        << " used="  << thebuf.GetUsedBytes()
        << Endl;
    out  << "      UTF-8 buffer Contains: '" << (char *)thebuf.GetBuf() << "'" << Endl;
  }

  // Get the used bytes, char count, and byte count
  usedbytes = thebuf.GetUsedBytes();
  charcount = thebuf.Length();
  bytecount = thebuf.Size();
  if (gVeryVerbose) {
    String note;
    if (gConsoleUTF8)
      note = " ◄── Includes NUL";
    else
      note = " <-- Includes NUL";
    out  << "      UTF-8 used bytes: " << usedbytes << note << Endl;
    out  << "      UTF-8 Char count: " << charcount << note << Endl;
    out  << "      UTF-8 Byte count: " << bytecount << note << Endl;
  }

  // Clear the buffer
  thebuf.SetEmpty();

  // Print final allocation stats
  if (gVeryVerbose) {
    out  << "    Total Allocations:  " << Buffer::GetTotalAllocations() << Endl;
    out  << "    Active Allocations: " << Buffer::GetActiveAllocations() << Endl;
  }

  while (false) {
    ERROR: out  << "  !!! Error " << status << "!!!" << Endl;
  }

  return;
  
} // ...testSBuffer

//----------------------------------------------------------------------
//
// Method: testManip - Test various String Manipulation methods
//

void TestString::testManip() { // testManip...

  Status  status;
  String  exename;
  Strings parts;
  Char    c;
  UInt    index;

  Byte    buf[256];
  Span    span;
  String  tstring;

  if (gVerbose)
    out  << "  String Manipulation ..." << Endl;

  // Get the string to split, we'll use the executable name for now
  exename = TheBefe->TheProcess->GetExecutableFileName();
  exename.ToEllipsis((Byte *)buf,sizeof(buf));
  if (gVeryVerbose)
    out  << "    Process executable: \"" << (char *)buf<< "\" (len=" << exename.Length() << ")\n";

  // Get the Nth character
  c = exename.Get(3);
  if (gVeryVerbose) {
    if (c == (UInt)0)
      out  << "    Executable[3]                   = NUL" << Endl;
    else
      out  << "    Executable[3]                   = '" << (char)c << "'" << Endl;
  }
  c = exename.Get(-4);
  if (gVeryVerbose) {
    if (c == (UInt)0)
      out  << "    Executable[-4]                  = NUL" << Endl;
    else
      out  << "    Executable[-4]                  = '" << (char)c << "'" << Endl;
  }

  // Find a character
  index = exename.Find('c');
  if (gVeryVerbose)
    out  << "    Executable.Find('c')        = " << index << Endl;
  span = Span("-1:-14");
  index = exename.Find('\\',span);
  if (gVeryVerbose)
    out  << "    Executable.Find('\\'[-1:-14]) = " << index << Endl;
  span = Span("-1:0");
  index = exename.Find('\\',span);
  if (gVeryVerbose)
    out  << "    Executable._Find('\\',[-1:0]) = " << index << Endl;
  span = Span("-1:0");
  index = exename.Find(':',span);
  if (gVeryVerbose)
    out  << "    Executable._Find(':',[::-1]) = " << index << Endl;

  // Splice executable name into parts
  status = exename.SplitToStrings('\\',parts);
  if (status)
    out << "  !!! Error '" << ErrorString(status) << " from exename.Split()\n";
  else if (gVeryVerbose) {
    out  <<"    Executable has " << parts.Length() << " parts" << Endl;
    for (index=0; index < parts.Length(); index++) {
      status = parts.Get(index,tstring);
      if (status) {
        out << "  !!! Error '" << ErrorString(status) << " from parts.Get()\n";
        break;
      }
      tstring.ToEllipsis(buf,sizeof(buf));
      out  << "    Part " << index << " = '" << (char *)buf << "'" << Endl;
    }
  }

  //
  // Truncate
  //

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.Truncate()        = '";
  tstring.Truncate();
  if (gVeryVerbose)
    out << tstring << "'\n";

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.Truncate(5)       = '";
  tstring.Truncate(5);
  if (gVeryVerbose)
    out << tstring << "'\n";

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.Truncate(20)      = '";
  tstring.Truncate(20);
  if (gVeryVerbose)
    out << tstring << "'\n";

  //
  // Pad
  //

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.Pad(0)            = '";
  tstring.Pad(0);
  if (gVeryVerbose)
    out << tstring << "'\n";

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.Pad(5)            = '";
  tstring.Pad(5);
  if (gVeryVerbose)
    out << tstring << "'\n";

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.Pad(20)           = '";
  tstring.Pad(20);
  if (gVeryVerbose)
    out << tstring << "'\n";

  //
  // TruncateOrPad
  //

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.TruncateOrPad(0)  = '";
  tstring.TruncateOrPad(0);
  if (gVeryVerbose)
    out << tstring << "'\n";

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.TruncateOrPad(5)  = '";
  tstring.TruncateOrPad(5);
  if (gVeryVerbose)
    out << tstring << "'\n";

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.TruncateOrPad(20) = '";
  tstring.TruncateOrPad(20);
  if (gVeryVerbose)
    out << tstring << "'\n";

  /*
  Status       LeftTrim();
  Status       RightTrim();
  Status       Ellipsis(Int len);
  */

  //
  // Ellipsis
  //

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.Ellipsis(0)       = '";
  tstring.Ellipsis(0);
  if (gVeryVerbose)
    out << tstring << "'\n";

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.Ellipsis(5)       = '";
  tstring.Ellipsis(5);
  if (gVeryVerbose)
    out << tstring << "'\n";

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.Ellipsis(7)       = '";
  tstring.Ellipsis(7);
  if (gVeryVerbose)
    out << tstring << "'\n";

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.Ellipsis(20)      = '";
  tstring.Ellipsis(20);
  if (gVeryVerbose)
    out << tstring << "'\n";

  //
  // PadOrEllipsis
  //

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.PadOrEllipsis(0)  = '";
  tstring.PadOrEllipsis(0);
  if (gVeryVerbose)
    out << tstring << "'\n";

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out  << "    '" << tstring << "'.PadOrEllipsis(5)  = '";
  tstring.PadOrEllipsis(5);
  if (gVeryVerbose)
    out << tstring << "'\n";

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out << "    '" << tstring << "'.PadOrEllipsis(7)  = '";
  tstring.PadOrEllipsis(7);
  if (gVeryVerbose)
    out << tstring << "'\n";

  tstring = (char *)"abcdedfghi";
  if (gVeryVerbose)
    out << "    '" << tstring << "'.PadOrEllipsis(20) = '";
  tstring.PadOrEllipsis(20);
  if (gVeryVerbose)
    out << tstring << "'\n";

  return;

} // ...testManip

//----------------------------------------------------------------------
//
// Method: testFindChar - Test String.Find(Char) methods
//

void TestString::testFindChar(Boolean sensitive) { // testFindChar...

  String  sString1;
  String  sString2;
  Char    sChar1;
  Char    sChar2;
  Range   range;
  Span    span;
  Int     found;
  
  if (gVerbose)
    out  << "  String Find(Char) " << ((sensitive)?"Sensitive":"Insensitive") << "..." << Endl;

  if (sensitive) {
    sString1 = "0123456789abcdef";
    sString2 = "c:/*/c++/*/*/*.h";
    sChar1   = 'a';
    sChar2   = '*';
  }
  else {
    sString1 = String("ThisIsSomeString").Insensitive();
    sString2 = String("c:/*/c++/*/*/*.h").Insensitive();
    sChar1   = 's';
    sChar2   = 'C';
  }
  
  // Try unlimited Find...
  if (gVeryVerbose)
    out << "    Char find '" << sChar1 << "' in '" << sString1 << "'...\n";
  found = sString1.Find(sChar1);
  if (gVeryVerbose)
    out << "      Found at: " << found << '\n';
  if (gVeryVerbose)
    out << "    Char find '" << sChar2 << "' in '" << sString2 << "'...\n";
  found = sString2.Find(sChar2);
  if (gVeryVerbose)
    out << "      Found at: " << found << '\n';
  
  // Try Range limited Find forward...
  if (gVeryVerbose)
    out << "    Range find '" << sChar2 << "' in '" << sString2 << "'[0..] forward...\n";
  range = Range("0..");
  while (true) {
    found = sString2.Find(sChar2,range);
    if (found < 0) break;
    if (gVeryVerbose)
      out << "      Found '*' at: " << found << '\n';
    range.idx1 = found+1;
  }

  // Try Range limited Find reverse...
  if (gVeryVerbose)
    out << "    Range find '" << sChar2 << "' in '" << sString2 << "'[-1..0] reverse...\n";
  range = Range("-1..0");
  while (true) {
    found = sString2.Find(sChar2,range);
    if (found < 0) break;
    if (gVeryVerbose)
      out << "      Found '" << sChar2 << "' at: " << found << '\n';
    range.idx1 = found-1;
  }

  // Try Span limited Find forward...
  if (gVeryVerbose)
    out << "    Span  find '" << sChar2 << "' in '" << sString2 << "'[0:] reverse...\n";
  span = Span("0:");
  while (true) {
    found = sString2.Find(sChar2,span);
    if (found < 0) break;
    if (gVeryVerbose)
      out << "      Found '" << sChar2 <<"' at: " << found << '\n';
    span.idx1 = found+1;
  }

  // Try Span limited Find reverse...
  if (gVeryVerbose)
    out << "    Span  find '" << sChar2 << "' in '" << sString2 << "'[-1:0] reverse...\n";
  span = Span("-1:0");
  while (true) {
    found = sString2.Find(sChar2,span);
    if (found < 0) break;
    if (gVeryVerbose)
      out << "      Found '" << sChar2 << "' at: " << found << '\n';
    span.idx1 = found-1;
  }

  return;

} // ...testFindChar

//----------------------------------------------------------------------
//
// Method: testFindString - Test String.Find(String) methods
//

void TestString::testFindString(Boolean sensitive) { // testFindString...

  String  string;
  String  findString;
  Range   range;
  Span    span;
  Slice   slice;
  Int     found;
  
  if (gVerbose)
    out  << "  String Find(String) " << ((sensitive)?"Sensitive":"Insensitive") << "..." << Endl;
  string = "theStringAndAnotherString";
  if (sensitive)
    findString = "String";
  else {
    findString = "string";
    findString.Insensitive();
  }
  
  // Try unlimited Find...
  if (gVeryVerbose)
    out << "    Char  find '" << findString << "' in '" << string << "'...\n";
  found = string.Find(findString);
  if (gVeryVerbose)
    out << "      Found at: " << found << '\n';
  
  // Try Range limited Find forward...
  if (gVeryVerbose)
    out << "    Range find '" << findString << "' in '" << string << "' forward...\n";
  range = Range("0..");
  while (true) {
    found = string.Find(findString,range);
    if (found < 0) break;
    if (gVeryVerbose)
      out << "      Found at: " << found << '\n';
    range.idx1 = found+1;
  }

  // Try Range limited Find reverse...
  if (gVeryVerbose)
    out << "    Range find '" << findString << "' in '" << string << "' reverse...\n";
  range = Range("-1..0");
  while (true) {
    found = string.Find(findString,range);
    if (found < 0) break;
    if (gVeryVerbose)
      out << "      Found at: " << found << '\n';
    range.idx1 = found-1;
  }

  // Try Span limited Find forward...
  if (gVeryVerbose)
    out << "    Span  find '" << findString << "' in '" << string << "' forward...\n";
  span = Span("0:");
  while (true) {
    found = string.Find(findString,span);
    if (found < 0) break;
    if (gVeryVerbose)
      out << "      Found at: " << found << '\n';
    span.idx1 = found+1;
  }

  // Try Span limited Find reverse...
  if (gVeryVerbose)
    out << "    Span  find '" << findString << "' in '" << string << "' reverse...\n";
  span = Span("-1:0");
  while (true) {
    found = string.Find(findString,span);
    if (found < 0) break;
    if (gVeryVerbose)
      out << "      Found at: " << found << '\n';
    span.idx1 = found-1;
  }

  return;

} // ...testFindString

//----------------------------------------------------------------------
//
// Method: testLogToPhySpan - Test String._LogToPhySpan method
//

void TestString::testLogToPhySpan() { // testLogToPhySpan...

  String theStr;
  UInt   len;
  UInt   siz;
  Span   span;
  Span   phySpan;
  UInt   phyLen;
  UInt   testIdx;
  UInt   i;

  char const *testStrings[] = { "0123456789abcdef", "012◄─►cdef", 0};
  
  char const *spanStrings[] = {
     ":"
    ,"-1:"
    ,":-2"
    ,"-2:"
    ,":-1"
    ,"0:"
    ,"0:-1"
    ,"-1:0"
    ,"0:3"
    ,"2:6"
    ,"3:3"
    ,"3:2"
    ,"3:4"
    ,"4:3"
    ,"-1:-9"
    ,"-1:-10"
    ,"-1:-11"
    ,"-1..-17"
    ,"-1:2"
    ,"2:-1"
    ,0
  };

  if (gVerbose)
    out  << "  String._LogToPhySpan..." << Endl;

  for (testIdx=0; testStrings[testIdx]; testIdx++) { // For each test string...
    
    theStr = testStrings[testIdx];
    
    len = theStr.Length();
    siz = theStr.Size();
  
    if (gVeryVerbose) {
      out << "    String = '" << theStr << "'\n";
      out << "      Length = "  << len    << " Chars\n";
      out << "      Size   = "  << siz    << " Bytes\n";
    }
  
    for (i=0; spanStrings[i]; i++) { // For each Span...
      
      span = Span(spanStrings[i]);

      if (gVeryVerbose)
        out << "      String " << ToString(i).LeftPad(2) << ": Span " << span.ToString().Pad(6);

      phySpan = theStr._LogToPhySpan(span);
      phyLen  = phySpan.Length();

      if (gVeryVerbose)
        out << " -> "             << phySpan.ToString().Pad(6)
            << " Length = " << ToString(phyLen).Pad(2) << " Bytes"
            << "   Result = '" << theStr.Get(span) << "'\n";

    } // ...For each Span

  } // ...For each test string
  
  return;

} // ...testLogToPhySpan

//----------------------------------------------------------------------
//
// Method: testLogToPhyRange - Test String._LogToPhyRange method
//

void TestString::testLogToPhyRange() { // testLogToPhyRange...

  String theStr;
  String resultStr;
  UInt   len;
  UInt   siz;
  Range  range;
  Range  phyRange;
  UInt   phyLen;
  UInt   testIdx;
  UInt   i;

  char const *testStrings[] = { "0123456789abcdef", "012◄─►cdef", 0};
  
  char const *rangeStrings[] = {
     ".."
    ,"0.."
    ,"..-2"
    ,"-2.."
    ,"..-1"
    ,"0.."
    ,"0..0"
    ,"0..2"
    ,"2..5"
    ,"3..3"
    ,"3..2"
    ,"3..4"
    ,"4..3"
    ,"-1..-9"
    ,"-1..-10"
    ,"-1..-11"
    ,"-1..-17"
    ,"-1..2"
    ,"2..-1"
    ,0
  };

  if (gVerbose)
    out  << "  String._LogToPhySRange..." << Endl;

  for (testIdx=0; testStrings[testIdx]; testIdx++) { // For each test string...
    
    theStr = testStrings[testIdx];
    
    len = theStr.Length();
    siz = theStr.Size();
  
    if (gVeryVerbose) {
      out << "    String = '" << theStr << "'\n";
      out << "      Length = "  << len    << " Chars\n";
      out << "      Size   = "  << siz    << " Bytes\n";
    }
  
    for (i=0; rangeStrings[i]; i++) { // For each Range...
      
      range = Range(rangeStrings[i]);

      if (gVeryVerbose)
        out << "      String " << ToString(i).LeftPad(2) << ": Range " << range.ToString().Pad(7);

      phyRange  = theStr._LogToPhyRange(range);
      phyLen    = phyRange.Length();
      resultStr = theStr.Get(range);
      
      if (gVeryVerbose)
        out << " -> "          << phyRange.ToString().Pad(6)
            << " Length = "    << ToString(phyLen).Pad(4) << " Bytes"
            << "   Result = '" << resultStr << "'\n";

    } // ...For each Range

  } // ...For each test string
  
  return;

} // ...testLogToPhyRange

//----------------------------------------------------------------------
//
// Method: testRemoveRange - Test String.Remove(Range)
//

void TestString::testRemoveRange() { // testRemoveRange...

  Status status;
  String theStr;
  String remStr;
  Range  range;
  UInt   len;
  UInt   siz;
  UInt   testIdx;
  UInt   i;

  char const *testStrings[] = { "0123456789abcdef", "012◄─►cdef", 0};
  
  char const *rangeStrings[] = {
     ".."
    ,"0.."
    ,"..-2"
    ,"-2.."
    ,"3..5"
    ,"0..2"
    ,"4..5"
    ,0
  };

  if (gVerbose)
    out  << "  String.RemoveRange..." << Endl;

  for (testIdx=0; testStrings[testIdx]; testIdx++) { // For each test string...
    
    theStr = testStrings[testIdx];
    
    len = theStr.Length();
    siz = theStr.Size();
  
    if (gVeryVerbose) {
      out << "    String = '" << theStr << "'\n";
      out << "      Length = "  << len    << " Chars\n";
      out << "      Size   = "  << siz    << " Bytes\n";
    }
  
    for (i=0; rangeStrings[i]; i++) { // For each Range...
      
      range = Range(rangeStrings[i]);
      remStr = theStr;
      
      if (gVeryVerbose)
        out << "      Range " << ToString(i).Pad(2) << ": " << range.ToString().Pad(7);

      status = remStr.Mutable();
      status = remStr.Remove(range);
      if (status) {
        if (gVeryVerbose)
          out << " Error...\n";
        BEFE_ERROR("String.Remove",status);
      }
      else if (gVeryVerbose)
        out << " Result = '" << remStr << "'\n";

    } // ...For each Range

  } // ...For each test string
  
  return;

} // ...testRemoveRange

//----------------------------------------------------------------------
//
// Method: testRemoveSpan - Test String.Remove(Span) method
//

void TestString::testRemoveSpan() { // testRemoveSpan...

  Status status;
  String theStr;
  String remStr;
  Span   span;
  UInt   len;
  UInt   siz;
  UInt   testIdx;
  UInt   i;

  char const *testStrings[] = { "0123456789abcdef", "012◄─►cdef", 0};
  
  char const *spanStrings[] = {
     ":"
    ,"0:"
    ,":-1"
    ,"-2:"
    ,"3:6"
    ,"0:3"
    ,"4:6"
    ,0
  };

  if (gVerbose)
    out  << "  String.RemoveSpan..." << Endl;

  for (testIdx=0; testStrings[testIdx]; testIdx++) { // For each test string...
    
    theStr = testStrings[testIdx];
    
    len = theStr.Length();
    siz = theStr.Size();
  
    if (gVeryVerbose) {
      out << "    String = '" << theStr << "'\n";
      out << "      Length = "  << len    << " Chars\n";
      out << "      Size   = "  << siz    << " Bytes\n";
    }
  
    for (i=0; spanStrings[i]; i++) { // For each Span...
      
      span = Span(spanStrings[i]);
      remStr = theStr;
      
      if (gVeryVerbose)
        out << "      Span " << ToString(i).Pad(2) << ": " << span.ToString().Pad(7);

      status = remStr.Mutable();
      status = remStr.Remove(span);
      if (status) {
        if (gVeryVerbose)
          out << " Error...\n";
        BEFE_ERROR("String.Remove",status);
      }
      else if (gVeryVerbose)
        out << " Result = '" << remStr << "'\n";

    } // ...For each Span

  } // ...For each test string
  
  return;

} // ...testRemoveSpan

//----------------------------------------------------------------------
//
// Method: testInsertChar - Test String.Insert(Int,Char) method
//

void TestString::testInsertChar() { // testInsertChar...

  Status status;
  String string;

  if (gVerbose)
    out  << "  String.InsertChar..." << Endl;

  //
  // Try inserting into empty String...
  //
  
  string.Reset();
  status = string.Insert(0,Char('x'));
  if (status)
    BEFE_ERROR("String.Insert(0,'x')",status);
  else if (gVeryVerbose)
    out << "    " << String("''.Insert(0,'x')").Pad(30) << " = '" << string << "'\n"; 
  
  string.Reset();
  status = string.Insert(0,Char(0x2500u));
  if (status)
    BEFE_ERROR("String.Insert(0,'─')",status);
  else if (gVeryVerbose)
    out << "    " << String("''.Insert(0,'─')").Pad(30) << " = '" << string << "'\n"; 
    
  //
  // Try inserting into short ASCII strings...
  //
  
  string = "abc";
  if (string.GetType() != String::ASCII3)
    BEFE_WARN("Expected 'abc' to be type ASCII4 but it wasn't");    
  string.Mutable();
  status = string.Insert(0,'x');
  if (status)
    BEFE_ERROR("String.Insert(0,'x')",status);
  else if (gVeryVerbose)
    out << "    " << String("'abc'.Insert(0,'x')").Pad(30) << " = '" << string << "'\n"; 
  if (string.GetType() != String::ASCII4)
    BEFE_WARN("Expected 'xabc' to be type ASCII4 but it wasn't");

  status = string.Insert(0,'y');
  if (status)
    BEFE_ERROR("String.Insert(0,'y')",status);
  else if (gVeryVerbose)
    out << "    " << String("'xabc'.Insert(0,'y')").Pad(30) << " = '" << string << "'\n"; 
  if (string.GetType() != String::Buffer)
    BEFE_WARN("Expected 'xabc' to be type Buffer but it wasn't");
    
  //
  // Try inserting into UTF-8 strings...
  //

  string = "123◄►cdef";
  string.Mutable();
  if (string.GetType() != String::Buffer)
    BEFE_WARN("Expected '123cdef' to be type Buffer but it wasn't");    
  status = string.Insert(4,Char(0x2500u));
  if (status)
    BEFE_ERROR("String.Insert(4,'─')",status);
  else if (gVeryVerbose)
    out << "    " << String("'123◄►cdef'.Insert(0,'─')").Pad(30) << " = '" << string << "'\n"; 
  return;

} // ...testInsertChar

//----------------------------------------------------------------------
//
// Method: testInsertString - Test String.Insert(Int,String) method
//

void TestString::testInsertString() { // testInsertString...

  Status status;
  String string;

  if (gVerbose)
    out  << "  String.InsertString..." << Endl;

  //
  // Try inserting into empty String...
  //
  
  string.Reset();
  status = string.Insert(0,"x");
  if (status)
    BEFE_ERROR("String.Insert(0,'x')",status);
  else if (gVeryVerbose)
    out << "    " << String("''.Insert(0,'x')").Pad(30) << " = '" << string << "'\n"; 
  
  string.Reset();
  status = string.Insert(0,String("─"));
  if (status)
    BEFE_ERROR("String.Insert(0,'─')",status);
  else if (gVeryVerbose)
    out << "    " << String("''.Insert(0,'─')").Pad(30) << " = '" << string << "'\n"; 
    
  //
  // Try inserting into short ASCII strings...
  //
  
  string = "abc";
  if (string.GetType() != String::ASCII3)
    BEFE_WARN("Expected 'abc' to be type ASCII4 but it wasn't");    
  string.Mutable();
  status = string.Insert(0,"x");
  if (status)
    BEFE_ERROR("String.Insert(0,'x')",status);
  else if (gVeryVerbose)
    out << "    " << String("'abc'.Insert(0,'x')").Pad(30) << " = '" << string << "'\n"; 
  if (string.GetType() != String::ASCII4)
    BEFE_WARN("Expected 'xabc' to be type ASCII4 but it wasn't");

  status = string.Insert(0,"y");
  if (status)
    BEFE_ERROR("String.Insert(0,'y')",status);
  else if (gVeryVerbose)
    out << "    " << String("'xabc'.Insert(0,'y')").Pad(30) << " = '" << string << "'\n"; 
  if (string.GetType() != String::Buffer)
    BEFE_WARN("Expected 'xabc' to be type Buffer but it wasn't");
    
  //
  // Try inserting into ASCII strings...
  //

  string = "012cdef";
  string.Mutable();
  if (string.GetType() != String::Buffer)
    BEFE_WARN("Expected '123cdef' to be type Buffer but it wasn't");    
  status = string.Insert(3,"◄─►");
  if (status)
    BEFE_ERROR("String.Insert(3,'◄─►')",status);
  else if (gVeryVerbose)
    out << "    " << String("'123cdef'.Insert(3,'◄─►')").Pad(30) << " = '" << string << "'\n"; 
  if (string.GetType() != String::Buffer)
    BEFE_WARN("Expected '123◄─►cdef' to be type Buffer but it wasn't");

  //
  // Try inserting into UTF-8 strings...
  //

  string = "012◄─►cdef";
  string.Mutable();
  if (string.GetType() != String::Buffer)
    BEFE_WARN("Expected '123◄─►cdef' to be type Buffer but it wasn't");    
  status = string.Insert(5,"◄─►─");
  if (status)
    BEFE_ERROR("String.Insert(5,'◄─►─')",status);
  else if (gVeryVerbose)
    out << "    " << String("'123◄─►cdef'.Insert(3,'◄─►─')").Pad(30) << " = '" << string << "'\n"; 
  if (string.GetType() != String::Buffer)
    BEFE_WARN("Expected '123◄─►cdef' to be type Buffer but it wasn't");

  return;

} // ...testInsertString

//----------------------------------------------------------------------
//
// Method: testReplaceInt - Test String.Replace(Int,...) Methods
//

void TestString::testReplaceInt() { // testReplaceInt...

  Status status;
  String before;
  String after;

  if (gVerbose)
    out  << "  String.Replace(Int)..." << Endl;

  //
  // Try Replacing char at various locations
  //
  
  before = "0123456789abcdef";
  before.Mutable();
  
  after  = before;
  status = after.Replace(0,'x');
  if (status)
    BEFE_ERROR("String.Replace(0,'x')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(0,'x')").Pad(45) << " = '" << after << "'\n"; 

  after = before;
  status = after.Replace(10,'X');
  if (status)
    BEFE_ERROR("String.Replace(10,'x')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(10,'x')").Pad(45) << " = '" << after << "'\n"; 
  
  after = before;
  status = after.Replace(before.Length(),'X');
  if (status)
    BEFE_ERROR(String("String.Replace(")+before.Length()+",'x')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace("+before.Length()+",'x')").Pad(45) << " = '" << after << "'\n"; 
  
  after = before;
  status = after.Replace(17,'X');
  if (status != Error::IndexOutOfRange)
    BEFE_ERROR("String.Replace(17,'x')",status);

  after = before;
  status = after.Replace(-1,'X');
  if (status)
    BEFE_ERROR("String.Replace(-1,'x')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(-2,'x')").Pad(45) << " = '" << after << "'\n"; 

  //
  // Try Replacing Char at various locations
  //
  
  before = "0123456789abcdef";
  before.Mutable();
  
  after  = before;
  status = after.Replace(0,Char(0x25B2u));
  if (status)
    BEFE_ERROR("String.Replace(0,'▲')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(0,'▲')").Pad(45) << " = '" << after << "'\n"; 

  after = before;
  status = after.Replace(10,Char(0x25B2u));
  if (status)
    BEFE_ERROR("String.Replace(10,'▲')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(10,'▲')").Pad(45) << " = '" << after << "'\n"; 
  
  after = before;
  status = after.Replace(before.Length(),Char(0x25B2u));
  if (status)
    BEFE_ERROR(String("String.Replace(")+before.Length()+",'▲')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace("+before.Length()+",'x')").Pad(45) << " = '" << after << "'\n"; 
  
  after = before;
  status = after.Replace(17,Char(0x25B2u));
  if (status != Error::IndexOutOfRange)
    BEFE_ERROR("String.Replace(17,'▲')",status);

  after = before;
  status = after.Replace(-1,Char(0x25B2u));
  if (status)
    BEFE_ERROR("String.Replace(-1,'▲')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(-2,'▲')").Pad(45) << " = '" << after << "'\n"; 
  
  //
  // Try Replacing String at various locations
  //
  
  before = "0123456789abcdef";
  before.Mutable();
  
  after  = before;
  status = after.Replace(0,"◄─►");
  if (status)
    BEFE_ERROR("String.Replace(0,'◄─►')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(0,'◄─►')").Pad(45) << " = '" << after << "'\n"; 

  after = before;
  status = after.Replace(10,"◄─►");
  if (status)
    BEFE_ERROR("String.Replace(10,'◄─►')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(10,'◄─►')").Pad(45) << " = '" << after << "'\n"; 
  
  after = before;
  status = after.Replace(before.Length(),"◄─►");
  if (status)
    BEFE_ERROR(String("String.Replace(")+before.Length()+",'◄─►')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace("+before.Length()+",'x')").Pad(45) << " = '" << after << "'\n"; 
  
  after = before;
  status = after.Replace(17,"◄─►");
  if (status != Error::IndexOutOfRange)
    BEFE_ERROR("String.Replace(17,'◄─►')",status);

  after = before;
  status = after.Replace(-1,"◄─►");
  if (status)
    BEFE_ERROR("String.Replace(-1,'◄─►')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(-2,'◄─►')").Pad(45) << " = '" << after << "'\n"; 
  
  return;

} // ...testReplaceInt

//----------------------------------------------------------------------
//
// Method: testReplaceRange - Test String.Replace(Range const &,...) Methods
//

void TestString::testReplaceRange() {  // testReplaceRange...

  Status status;
  String before;
  String after;

  if (gVerbose)
    out  << "  String.Replace(Range)..." << Endl;

  //
  // Try Replacing char at various locations
  //
  
  before = "0123456789abcdef";
  before.Mutable();
  
  after  = before;
  status = after.Replace(Range("0..7"),'X');
  if (status)
    BEFE_ERROR("String.Replace(Range(0..7),'X')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(0..7),'X')").Pad(45) << " = '" << after << "'\n"; 

  after = before;
  status = after.Replace(Range("10..15"),'X');
  if (status)
    BEFE_ERROR("String.Replace(Range(10..15),'x')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(10..15),'x')").Pad(45) << " = '" << after << "'\n"; 
  
  after = before;
  status = after.Replace(Range("-1..-6"),'X');
  if (status)
    BEFE_ERROR("String.Replace(Range(-1..-6),'X')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(-1..-6),'X')").Pad(45) << " = '" << after << "'\n"; 
    
  after = before;
  status = after.Replace(Range("-6..-6"),'X');
  if (status)
    BEFE_ERROR("String.Replace(Range(-6..-6),'X')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(-6..-6),'X')").Pad(45) << " = '" << after << "'\n"; 
    
  after = before;
  status = after.Replace(Range("16..20"),'X');
  if (status != Error::StringRemoveInvalid)
    BEFE_ERROR("String.Replace(Range(16..20),'X')",status);
      
  //
  // Try Replacing Char at various locations
  //
  
  before = "0123456789abcdef";
  before.Mutable();
  
  after  = before;
  status = after.Replace(Range("0..7"),Char(0x25B2u));
  if (status)
    BEFE_ERROR("String.Replace(Range(0..7),'▲')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(0..7),'X')").Pad(45) << " = '" << after << "'\n"; 

  after = before;
  status = after.Replace(Range("10..15"),Char(0x25B2u));
  if (status)
    BEFE_ERROR("String.Replace(Range(10..15),'▲')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(10..15),'x')").Pad(45) << " = '" << after << "'\n"; 
  
  after = before;
  status = after.Replace(Range("-1..-6"),Char(0x25B2u));
  if (status)
    BEFE_ERROR("String.Replace(Range(-1..-6),'▲')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(-1..-6),'X')").Pad(45) << " = '" << after << "'\n"; 
    
  after = before;
  status = after.Replace(Range("-6..-6"),Char(0x25B2u));
  if (status)
    BEFE_ERROR("String.Replace(Range(-6..-6),'▲')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(-6..-6),'X')").Pad(45) << " = '" << after << "'\n"; 
    
  after = before;
  status = after.Replace(Range("16..20"),Char(0x25B2u));
  if (status != Error::StringRemoveInvalid)
    BEFE_ERROR("String.Replace(Range(16..20),'▲')",status);
      
  //
  // Try Replacing String at various locations
  //
  
  before = "0123456789abcdef";
  before.Mutable();
  
  after  = before;
  status = after.Replace(Range("0..7"),"◄─►");
  if (status)
    BEFE_ERROR("String.Replace(Range(0..7),'◄─►')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(0..7),'◄─►')").Pad(45) << " = '" << after << "'\n"; 

  after = before;
  status = after.Replace(Range("10..15"),"◄─►");
  if (status)
    BEFE_ERROR("String.Replace(Range(10..15),'◄─►')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(10..15),'◄─►')").Pad(45) << " = '" << after << "'\n"; 
  
  after = before;
  status = after.Replace(Range("-1..-6"),"◄─►");
  if (status)
    BEFE_ERROR("String.Replace(Range(-1..-6),'◄─►')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(-1..-6),'◄─►')").Pad(45) << " = '" << after << "'\n"; 
    
  after = before;
  status = after.Replace(Range("-6..-6"),"◄─►");
  if (status)
    BEFE_ERROR("String.Replace(Range(-6..-6),'◄─►')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(-6..-6),'◄─►')").Pad(45) << " = '" << after << "'\n"; 
    
  after = before;
  status = after.Replace(Range("16..20"),"◄─►");
  if (status != Error::StringRemoveInvalid)
    BEFE_ERROR("String.Replace(Range(16..20),'◄─►')",status);
      
  return;

}  // ...testReplaceRange

//----------------------------------------------------------------------
//
// Method: testReplaceSpan - Test String.Replace(Span const &,...) Methods
//

void TestString::testReplaceSpan() { // testReplaceSpan...

  Status status;
  String before;
  String after;

  if (gVerbose)
    out  << "  String.Replace(Span)..." << Endl;

  //
  // Try Replacing char at various locations
  //
  
  before = "0123456789abcdef";
  before.Mutable();
  
  after  = before;
  status = after.Replace(Span("0:8"),'X');
  if (status)
    BEFE_ERROR("String.Replace(Span(0:8),'X')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Span(0:8),'X')").Pad(45) << " = '" << after << "'\n"; 

  after = before;
  status = after.Replace(Span("10:16"),'X');
  if (status)
    BEFE_ERROR("String.Replace(Span(10:16),'X')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Range(10:16),'X')").Pad(45) << " = '" << after << "'\n"; 
  
  after = before;
  status = after.Replace(Span("-1:-7"),'X');
  if (status)
    BEFE_ERROR("String.Replace(Span(-1:-7),'X')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Span(-1..-7),'X')").Pad(45) << " = '" << after << "'\n"; 

  after = before;
  status = after.Replace(Span("-6:-7"),'X');
  if (status)
    BEFE_ERROR("String.Replace(Span(-6:-7),'X')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Span(-6:-7),'X')").Pad(45) << " = '" << after << "'\n"; 
    
  after = before;
  status = after.Replace(Span("16:21"),'X');
  if (status != Error::StringRemoveInvalid)
    BEFE_ERROR("String.Replace(Range(16:21),'X')",status);
    
  //
  // Try Replacing Char at various locations
  //
  
  before = "0123456789abcdef";
  before.Mutable();
  
  after  = before;
  status = after.Replace(Span("0:8"),Char(0x25B2u));
  if (status)
    BEFE_ERROR("String.Replace(Span(0:8),'▲')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Span(0:8),'X')").Pad(45) << " = '" << after << "'\n"; 

  after = before;
  status = after.Replace(Span("10:16"),Char(0x25B2u));
  if (status)
    BEFE_ERROR("String.Replace(Span(10:16),'▲')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Span(10:16),'x')").Pad(45) << " = '" << after << "'\n"; 
  
  after = before;
  status = after.Replace(Span("-1:-7"),Char(0x25B2u));
  if (status)
    BEFE_ERROR("String.Replace(Span(-1:-7),'▲')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Span(-1:-7),'X')").Pad(45) << " = '" << after << "'\n"; 
    
  after = before;
  status = after.Replace(Span("-6:-7"),Char(0x25B2u));
  if (status)
    BEFE_ERROR("String.Replace(Span(-6:-7),'▲')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Span(-6:-7),'X')").Pad(45) << " = '" << after << "'\n"; 
    
  after = before;
  status = after.Replace(Span("16:21"),Char(0x25B2u));
  if (status != Error::StringRemoveInvalid)
    BEFE_ERROR("String.Replace(Span(16:21),'▲')",status);
      
  //
  // Try Replacing String at various locations
  //
  
  before = "0123456789abcdef";
  before.Mutable();
  
  after  = before;
  status = after.Replace(Span("0:8"),"◄─►");
  if (status)
    BEFE_ERROR("String.Replace(Span(0:8),'◄─►')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Span(0:8),'◄─►')").Pad(45) << " = '" << after << "'\n"; 

  after = before;
  status = after.Replace(Span("10:16"),"◄─►");
  if (status)
    BEFE_ERROR("String.Replace(Span(10:16),'◄─►')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Span(10:16),'◄─►')").Pad(45) << " = '" << after << "'\n"; 
  
  after = before;
  status = after.Replace(Span("-1:-7"),"◄─►");
  if (status)
    BEFE_ERROR("String.Replace(Span(-1:-7),'◄─►')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Span(-1:-7),'◄─►')").Pad(45) << " = '" << after << "'\n"; 
    
  after = before;
  status = after.Replace(Span("-6:-7"),"◄─►");
  if (status)
    BEFE_ERROR("String.Replace(Span(-6:-7),'◄─►')",status);
  else if (gVeryVerbose)
    out << "    '" << String(before+"'.Replace(Span(-6:-7),'◄─►')").Pad(45) << " = '" << after << "'\n"; 
    
  after = before;
  status = after.Replace(Span("16:21"),"◄─►");
  if (status != Error::StringRemoveInvalid)
    BEFE_ERROR("String.Replace(Span(16:21),'◄─►')",status);
        
  return;

} // ...testReplaceSpan

//----------------------------------------------------------------------
//
// Method: testSplit - Test String.Split Methods
//
// There's several cases we'll be wanting to test here, but we're
// not exactly doing them all, for now...
//
//   For Input...
//
//     o char (ASCII only)
//     o Char (ASCII and Unicode)
//     o String (ASCII and UTF-8)
//
//   For processing...
//
//     o Case sensitive
//     o Case insensitive
//
//   For output...
//
//     o Returning Mutable Strings
//     o Returning Const Strings
//     o Returning Spans
//     o Returning Count
//

void TestString::testSplit() { // TestString.testSplit...

  Status  status;
  Strings strings;
  String  sString;
  String  tString;
  String  aString;
  UInt    testIdx;
  UInt    curIdx;
  
  char const *testStrings[] = {
    "XXX", "XXX",
    "XXX", "XXXdef",
    "XXX", "abcdef",
    "XXX", "abcXXXdef",
    "XXX", "abcdefXXX",
    "XXX", "xxx",
    "XXX", "xxxdef",
    "XXX", "abcdef",
    "XXX", "abcxxxdef",
    "XXX", "abcdefxxx",
    "xxx", "XXX",
    "xxx", "XXXdef",
    "xxx", "abcdef",
    "xxx", "abcXXXdef",
    "xxx", "abcdefXXX",
    "*",   "c:/cloud/*/*/*.cpp",
    NULL
  };

  if (gVerbose)
    out  << "  String.Split..." << Endl;

  //
  // Test non-const Split on string...
  //
  
  if (gVerbose)
    out << "  Const Split on ConstStrings...\n";
  
  for (testIdx=0; testStrings[testIdx] && testStrings[testIdx+1]; testIdx += 2) { // For each input string...

    sString = testStrings[testIdx];
    tString = testStrings[testIdx+1];
    if (testStrings[testIdx][0] == 'x')
      sString.Insensitive();
      
    if (gVeryVerbose) {
      out << "    Search String: '" << sString << "'\n";
      out << "    Test   String: '" << tString << "'\n";
    }

    status = tString._SplitToConstStrings(sString, strings);
    if (status) {
      BEFE_ERROR("String._SplitToConstStrings",status);
      continue;
    }

    if (gVeryVerbose) {
      if (strings.Length() == 0)
        out << "      *** Result empty***\n";
      else
        out << "      " << strings.Length() << " String" << ((strings.Length() != 1)?"s":"") << " found...\n";
      for (curIdx=0; curIdx < strings.Length(); curIdx++) {
        aString = strings.Get(curIdx);
        out << "        [" << curIdx << "] = '" << aString << "'\n"; 
      }
    }
    
  } // ...For each input string
  
  //
  // Test non-const Split on string...
  //
  
  if (gVerbose)
    out << "  Non-const Split on Const Strings...\n";
  
  for (testIdx=0; testStrings[testIdx] && testStrings[testIdx+1]; testIdx += 2) { // For each input string...

    sString = testStrings[testIdx];
    tString = testStrings[testIdx+1];

    if (gVeryVerbose) {
      out << "  Search String: '" << sString << "'\n";
      out << "  Test   String: '" << tString << "'\n";
    }

    status = tString.SplitToStrings(sString, strings);
    if (status) {
      BEFE_ERROR("String.SplitToStrings",status);
      continue;
    }

    if (gVeryVerbose) {
      if (strings.Length() == 0)
        out << "    *** Result empty***\n";
      else
        out << "    " << strings.Length() << " String" << ((strings.Length() != 1)?"s":"") << " found...\n";
      for (curIdx=0; curIdx < strings.Length(); curIdx++) {
        aString = strings.Get(curIdx);
        out << "      [" << curIdx << "] = '" << aString << "'\n"; 
      }
    }
  
  } // ...For each input string
  
  return;
  
} // ...TestString.testSplit

//----------------------------------------------------------------------
//
// Method: testGeneral - Test General String Functions
//

void TestString::testGeneral() { // TestString.testGeneral...

  Status              status;
  NamedStrings const *envValues;
  String              before;
  String              after;
  String              after2;
  Byte               *buf;
  UInt                byteCount;
  UInt                charCount;
  // For Unescape...
  const char *beforeStrings[] = {
    "0123456789abcdef",
    "...\\n""──►Line 1◄──\\x0A"
            "...\\x2a2a2a2121212a2a2a...\\xA" // '...***!!!***...'
            "──►Line 2◄──\\n"
            "...",
    NULL
    };
  UInt beforeIdx;

  if (gVerbose)
    out  << "  String.General..." << Endl;
  
  //
  // Test SubstituteStrings...
  //
  
  if (gVerbose)
    out << "    SubstituteStrings...\n";
  envValues = TheBefe->GetEnvironmentValues();
  if (gVeryVerbose)
    out << "      There are " << envValues->Length() << " environment name/values\n";
  
  before = "──►Befe_Home = '%Befe_Home%'\\n"
           "──►Befe_Root = '%Befe_Root%'\\n"
           "──►Befe_Temp = '%Befe_Temp%'\\n";
  status = Substitute(before, *envValues, "%", "%", after);
  if (gVeryVerbose) {
    out << "      before = '" << before << "'\n";
    out << "      after  = '" << after  << "'\n";
  }
  if (status) goto SOMEERROR;
  
  //
  // Do a "no conversion" call to Unescape...
  //
    
  if (gVerbose)
    out << "    Unescape/Escape ...\n";    

  for (beforeIdx=0; beforeStrings[beforeIdx]; beforeIdx++) {
    
    before = beforeStrings[beforeIdx];
    after  = before.Unescape();
    after2 = after.Escape();
    if (gVeryVerbose) {
      before._BufByteAndCharCount(buf, byteCount, charCount);
      out << "      Before.....|" << before << "| (Before = 0x" << (void *)buf << ")\n";
      after._BufByteAndCharCount(buf, byteCount, charCount);
      out << "        After....|" << after  << "| (After  = 0x" << (void *)buf << ")\n";
      out << "        After2...|" << after2 << "|\n";
      out << "        Before.IsMutable() = " << ((before.IsMutable())?"True":"False") << '\n';
      out << "        After.IsMutable()  = " << ((after.IsMutable())?"True":"False") << '\n';
      out << "        After2.IsMutable() = " << ((after2.IsMutable())?"True":"False") << '\n';
    }

  } // ...For each beforeString
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: BEFE_ERROR("TestString.testGeneral",status); break;
  }
  
  return;
  
} // ...TestString.testGeneral

//----------------------------------------------------------------------
//
// Method: testReverse - Test String.Reverse Methods
//

void TestString::testReverse() { // TestString.testReverse...

  Status              status;

  char const *strings[] = {
    "ab◀▶──fg",
    "ab╞╡══fg",
    "One byte each... ©®",
    "Two bytes each... ⁜⁑",
    0};
  Int    i;
  String str1;
  String str2;
  Range  r;
  Span   s;
  
  if (gVerbose)
    out  << "  String.Reverse..." << Endl;
  
  for (i=0; strings[i] != NULL; i++) {

    str1 = strings[i];

    str2 = str1;
    str2.Mutable();
    status = str2.Reverse();
    if (status) BEFE_ERROR("String.Reverse",status);
    if (gVerbose)
      out << "    strings[" << i << "]: Reverse()       - Before = '" << str1 << "', After = '" << str2 << "'\n";

    str2 = str1;
    str2.Mutable();
    r = Range("3..5");
    status = str2.Reverse(r);
    if (status) BEFE_ERROR("String.Reverse",status);
    if (gVerbose)
      out << "    strings[" << i << "]: Reverse('3..5') - Before = '" << str1 << "', After = '" << str2 << "'\n";

    str2 = str1;
    str2.Mutable();
    s = Span("3:6");
    status = str2.Reverse(s);
    if (status) BEFE_ERROR("String.Reverse",status);
    if (gVerbose)
      out << "    strings[" << i << "]: Reverse('3:6')  - Before = '" << str1 << "', After = '" << str2 << "'\n";
      
  }

  if (gVerbose)
    out  << "  String.General..." << Endl;
  
  // Handle errors
  status = Error::None;
  
  return;
  
} // ...TestString.testReverse

//----------------------------------------------------------------------
//
// Method: testOpSubscript - Test String.operator[]
//

void TestString::testOpSubscript() { // TestString.testOpSubscript...

  String str;
  Char   c;
  String s;
  String prefix;
  UInt   pass;
  UInt   expNo;
  
  static char const *exp[] = {
     "'8',        "
    ,"'E',        "
    ,"'456789ABC',"
    ,"'CBA987654',"
    ,"'456789AB', "
    ,"'CBA98765', "
    ,"'456789AB', "
    ,"'CBA98765', "
    ,"'♠',        "
    ,"'E',        "
    ,"'4567♠9ABC',"
    ,"'CBA9♠7654',"
    ,"'4567♠9AB', "
    ,"'CBA9♠765', "
    ,"'4567♠9AB', "
    ,"'CBA9♠765', "
  };

  if (gVerbose)
    out << "  String.operator[]..." << Endl;

  expNo = 0;
  
  for (pass=0; pass < 4; pass++) {

    if (pass%2 == 0)
      str = "0123456789ABCDEF";
    else
      str = "01234567♠9ABCDEF";

    if (pass < 2)
      prefix = "    Immutable ";
    else {
      prefix = "    Mutable   ";
      str.Mutable();
    }

    c =  str[8];
    if (gVerbose) out << prefix << "str[8]:              expect " << exp[(expNo++)%16] << " got '" << c << "'\n";
    c =  str[-2];
    if (gVerbose) out << prefix << "str[-2]:             expect " << exp[(expNo++)%16] << " got '" << c << "'\n";
    s =  str[Range(4,12)];
    if (gVerbose) out << prefix << "str[Range(4,12)]:    expect " << exp[(expNo++)%16] << " got '" << s << "'\n";
    s =  str[Range(12,4)];
    if (gVerbose) out << prefix << "str[Range(12,4)]:    expect " << exp[(expNo++)%16] << " got '" << s << "'\n";
    s =  str[Span(4,12)];
    if (gVerbose) out << prefix << "str[Span(4,12)]:     expect " << exp[(expNo++)%16] << " got '" << s << "'\n";
    s =  str[Span(12,4)];
    if (gVerbose) out << prefix << "str[Span(12,4)]:     expect " << exp[(expNo++)%16] << " got '" << s << "'\n";
    s =  str[Slice(4,12,1)];
    if (gVerbose) out << prefix << "str[Slice(4,12,1)];  expect " << exp[(expNo++)%16] << " got '" << s << "'\n";
    s =  str[Slice(12,4,-1)];
    if (gVerbose) out << prefix << "str[Slice(12,4,-1)]: expect " << exp[(expNo++)%16] << " got '" << s << "'\n";

  }

  return;

} // ...TestString.testOpSubscript

} // ...Namespace BEFE
