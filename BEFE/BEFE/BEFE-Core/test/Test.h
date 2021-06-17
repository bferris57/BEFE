//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: Test.h - Declarations for the main unit tester Test Class
//----------------------------------------------------------------------
//
// The Test Class is responsibility for providing a Test environment
// for all currently testable classes.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TEST_H // TEST_H...
#define TEST_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Namespace: TestType - Named types of Tests
//

namespace TestType {

  enum {

     Core     = 0x0000ffff
    ,Time     = 0x00000001
    ,Number   = 0x00000002
    ,UTF8     = 0x00000004
    ,Lexicon  = 0x00000008
    ,Sequence = 0x00000010
    ,Array    = 0x00000020
    ,String   = 0x00000040
    ,Null     = 0x00000080
    ,Contains = 0x00000100
    ,Stream   = 0x00000200
    ,File     = 0x00000400
    ,Console  = 0x00000800
    ,IdSpace  = 0x00001000
    ,Morphing = 0x00002000
    ,Message  = 0x00004000
    ,Cache    = 0x00008000
    ,UUID     = 0x00010000
    ,MinExp   = 0x00020000
    ,MD5      = 0x00040000
    ,Bcmd     = 0x00080000

    ,TryThis  = 0x80000000
    ,Current  = Lexicon

    //,Current  = 0x000000B8 // Stream | Slice | String | UTF8
    ,None     = 0x40000000 // Empty "None" test (don't do anything)
    ,All      = 0x7fffffff

  };

} // ...Namespace TestType

//----------------------------------------------------------------------
//
// Class: Test - Test context class
//

class Test { // Class Test...

  // Members
  public: Console    &out;
  public: const char *indent;

  // Methods
  public:         Test();
  public:         Test(Int thetests);
  public: virtual ~Test();

  public:         Status Run(Int thetests);
  public: virtual Status Run();
  public: virtual void   Warn(String const &msg);
  public: virtual void   Warn(char const *msg);

  // Test Names
  public:         Status GetTestByName(String const &name, Int &theTests);

}; // ...Class Test

// Core class/utils tests
class TestTime     : public Test {virtual Status Run();};
class TestNumber;
class TestSequence;
class TestUTF8;
class TestLexicon;
class TestArray;
class TestString;
class TestNull;
class TestContains;
class TestStream   : public Test {virtual Status Run();};
class TestFile     : public Test {virtual Status Run();};
class TestConsole  : public Test {virtual Status Run();};
class TestIdSpace  : public Test {virtual Status Run();};
class TestMessage  : public Test {virtual Status Run();};
class TestCache;
class TestUUID     : public Test {virtual Status Run();};
class TestMinExp   : public Test {virtual Status Run();};
class TestMD5      : public Test {virtual Status Run();};

// Cross Class Functionality
class TestMorphing : public Test {virtual Status Run();};

// Word/dictionary class tests
class TestWordList : public Test {virtual Status Run();};
class TestWordGrid : public Test {virtual Status Run();};

} // ...Namespace BEFE

#endif // ...TEST_H
