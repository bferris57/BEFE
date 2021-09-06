//----------------------------------------------------------------------
// File: Test.cpp - Implementation of the Test Class (BEFE Unit Tester)
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Test.h"
#include "TestNumber.h"
#include "TestUTF8.h"
#include "TestLexicon.h"
#include "TestSequence.h"
#include "TestArray.h"
#include "TestString.h"
#include "TestNull.h"
#include "TestContains.h"
#include "TestCache.h"
#include "TestBcmd.h"

namespace BEFE { // Namespace BEFE...

extern Status trythis();

Test::Test() : out(Cout),indent("") {
}

Test::Test(Int thetests) : out(Cout) {

  //int *d;
  //int *s;

  //d = (int *)&out;
  //s = (int *)&Cout;
  //*d = (int)s;

  indent = "";
  indent = "";
  Run(thetests);
}

Test::~Test() {
}

Status Test::Run() {
  Run(TestType::All);
  return Error::None;
}

Status Test::Run(Int tests) {

  Status  status;
  Test   *tester;
  Int     mask;

  mask = 0x00000001;
  for (mask=1; mask != 0; mask<<=1) {
    tester = NULL;
    if      (tests&mask&TestType::Time)     tester = new TestTime();
    else if (tests&mask&TestType::Number)   tester = new TestNumber();
    else if (tests&mask&TestType::UTF8)     tester = new TestUTF8();
    else if (tests&mask&TestType::Lexicon)  tester = new TestLexicon();
    else if (tests&mask&TestType::Array)    tester = new TestArray();
    else if (tests&mask&TestType::String)   tester = new TestString();
    else if (tests&mask&TestType::Null)     tester = new TestNull();
    else if (tests&mask&TestType::Contains) tester = new TestContains();
    else if (tests&mask&TestType::Sequence) tester = new TestSequence();
    else if (tests&mask&TestType::Stream)   tester = new TestStream();
    else if (tests&mask&TestType::File)     tester = new TestFile();
    else if (tests&mask&TestType::Console)  tester = new TestConsole();
    else if (tests&mask&TestType::Message)  tester = new TestMessage();
    else if (tests&mask&TestType::Cache)    tester = new TestCache();
    else if (tests&mask&TestType::UUID)     tester = new TestUUID();
    //else if (tests&mask&TestType::IdSpace)  tester = new TestIdSpace();
    else if (tests&mask&TestType::Morphing) tester = new TestMorphing();
    else if (tests&mask&TestType::MinExp)   tester = new TestMinExp();
    else if (tests&mask&TestType::MD5)      tester = new TestMD5();
    else if (tests&mask&TestType::Bcmd)     tester = new TestBcmd();
    
    //else if (tests&mask&TestType::WordList) tester = new TestWordList();
    //else if (tests&mask&TestType::WordGrid) tester = new TestWordGrid();
    if (!IsNull(tester)) {
      tester->Run();
      delete tester;
    }
  };

  status = Error::None;
  if (tests&TestType::TryThis)
    status = trythis();
    
  return status;

}

void Test::Warn(String const &msg) {

  UInt oldForeground;
  UInt oldBackground;

  oldForeground = Cout.GetForegroundColour();
  oldBackground = Cout.GetBackgroundColour();
  Cout.SetForegroundColour(Console::Red);

  Cout << msg << '\n';

  Cout.SetForegroundColour(oldForeground);
  Cout.SetBackgroundColour(oldBackground);

}

void Test::Warn(char const *msg) {

  UInt oldForeground;
  UInt oldBackground;

  oldForeground = Cout.GetForegroundColour();
  oldBackground = Cout.GetBackgroundColour();
  Cout.SetForegroundColour(Console::Red);

  Cout << msg << '\n';

  Cout.SetForegroundColour(oldForeground);
  Cout.SetBackgroundColour(oldBackground);

}

static struct TestNames {char const *name; UInt testNo; } testNames[] = {
  {"A",        TestType::All},
  {"All",      TestType::All},
  {"None",     TestType::None},
  {"Cur",      TestType::Current},
  {"Current",  TestType::Current},
  {"Core",     TestType::Core},
  {"Time",     TestType::Time},
  {"Num",      TestType::Number},
  {"Number",   TestType::Number},
  {"UTF",      TestType::UTF8},
  {"UTF8",     TestType::UTF8},
  {"UTF-8",    TestType::UTF8},
  {"Lex",      TestType::Lexicon},
  {"Lexicon",  TestType::Lexicon},
  {"Seq",      TestType::Sequence},
  {"Sequence", TestType::Sequence},
  {"Array",    TestType::Array},
  {"String",   TestType::String},
  {"Null",     TestType::Null},
  {"Contains", TestType::Contains},
  {"Stream",   TestType::Stream},
  {"File",     TestType::File},
  {"Con",      TestType::Console},
  {"Console",  TestType::Console},
  {"Msg",      TestType::Message},
  {"Message",  TestType::Message},
  {".bmsg",    TestType::Message},
  {"Cache",    TestType::Cache},
  {"Caching",  TestType::Cache},
  {"UUID",     TestType::UUID},
  {"MinExp",   TestType::MinExp},
  {"MD5",      TestType::MD5},
  {"Bcmd",     TestType::Bcmd},
  {".bcmd",    TestType::Bcmd},
  {"Id",       TestType::IdSpace},
  {"IdSpace",  TestType::IdSpace},
  {"Morph",    TestType::Morphing},
  {"Morphing", TestType::Morphing},
  {"t",        TestType::TryThis},
  {"TryThis",  TestType::TryThis},
  {0,0}
};

Status Test::GetTestByName(String const &name, Int &theTests) {

  UInt testNo;

  testNo =0;
  while (!BEFE::IsNull(testNames[testNo].name)) {
    if (String(testNames[testNo].name).Insensitive() == name) break;
    testNo++;
  }

  if (BEFE::IsNull(testNames[testNo].name)) {
    theTests = 0;
    return Error::UnknownTest;
  }

  theTests = testNames[testNo].testNo;
  
  return Error::None;

}

} // ...Namespace BEFE
