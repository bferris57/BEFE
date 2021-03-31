//!befe-publish src
//----------------------------------------------------------------------
// File: TestMD5.cpp - Implementation of the TestMD5 Class
//----------------------------------------------------------------------
//
// This Class implements the BEFE UUID Class Unit Tests.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Test.h"
#include "MD5.h"

namespace BEFE { // Namespace BEFE...

Status TestMD5::Run() {

  Status  status;
  String  str;
  MD5Calc md5Calc;
  MD5     md5;
  UInt    i;
  const struct {char const *val; char const *md5;} strings[] = {
    {"The quick brown fox jumps over the lazy dog",  "9e107d9d372bb6826bd81d3542a419d6"},
    {"The quick brown fox jumps over the lazy dog.", "e4d909c290d0fb1ca068ffaddf22cbd0"},
    {"",                                             "d41d8cd98f00b204e9800998ecf8427e"},
    {0,0}
  };
  
  if (!gQuiet)
    out << "***Testing MD5...\n";

  for (i=0; strings[i].val; i++) {
    status = md5Calc.Set(strings[i].val);
    if (status) {
      BEFE_ERROR("md5.Set",status);
      break;
    }
    md5 = md5Calc.Get();
    if (gVerbose) {
      out << "  strings[i] = '" << strings[i].val << "'\n";
      out << "      Expect = 0x" << strings[i].md5 << '\n';
      out << "         Got = 0x" << ToHexString(md5).ToLowercase() << '\n';
    }
  }
  
  return status;
    
}

} // ...Namespace BEFE
