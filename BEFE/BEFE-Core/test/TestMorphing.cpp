//!befe-publish src
//----------------------------------------------------------------------
// File: testMorphing.cpp - TestMorphing Class (tests Cross Class Morphing)
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Test.h"
#include "ToStrings.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// TestMorphing methods
//

Status TestMorphing::Run() { // TestMorphing.Run...

  Int     status;
  String  string;
  Bytes   bytes;
  Strings strings;
  Byte   *buf;
  UInt    bufSize;
  Byte   *dstBuf;
  UInt    i;
  String  line;

  if (!gQuiet)
    out  << "***Testing Morphing..." << Endl;

  // Try displaying the String buffer in hex...
  string = (char *)"This String that contains two UTF-8 characters... ¶©";
  string._BufAndSize(buf,bufSize);
  strings.SetEmpty();
  status = ToStrings(buf,(UInt)bufSize,(UInt)0,strings);
  if (status) goto TOSTRINGSFAILED;

  if (gVerbose)
    Cout << "  Morph Source String = '" << string << "'\n";
  if (gVeryVerbose) {
    Cout << Endl;
    if (strings.Length() == 0)
      Cout << "  String Buffer = **Empty**\n";
    else {
      Cout << "  String Buffer before Morph to Bytes...\n\n";
      for (i=0;i<strings.Length();i++) {
        line = strings.Get(i);
        Cout << "    " << line << Endl;
      }
    }
    Cout << Endl;
  }

  // Now, morph it
  bytes = string.MorphToBytes();

  // Sanity checks
  if (bytes.Length() == 0)
    BEFE_WARN("Destination Morphed Bytes is Zero long");
  if (string.Length() != 0)
    BEFE_WARN("Source Morphed String is NOT Zero long");

  // Try displaying the Bytes buffer in hex...
  bytes.BufAndSize(buf,bufSize);
  strings.SetEmpty();
  status = ToStrings(buf,bufSize,0,strings);
  if (status) goto TOSTRINGSFAILED;

  if (gVeryVerbose) {
    Cout << Endl;
    if (strings.Length() == 0)
      Cout << "  Bytes Buffer = **Empty**\n";
    else {
      Cout << "  Bytes Buffer after Morph from String...\n\n";
      for (i=0;i<strings.Length();i++) {
        line = strings.Get(i);
        Cout << "    " << line << Endl;
      }
    }
    Cout << Endl;
  }

  // Now, morph it back
  string = bytes.MorphToString();

  // Sanity checks
  if (string.Length() == 0)
    BEFE_WARN("Destination Morphed String is Zero long");
  if (bytes.Length() != 0)
    BEFE_WARN("Source Morphed Bytes is NOT Zero long");

  // Try displaying the Bytes buffer in hex...
  dstBuf = buf;
  string._BufAndSize(buf,bufSize);
  strings.SetEmpty();
  status = ToStrings(buf,bufSize,0,strings);
  if (status) goto TOSTRINGSFAILED;

  if (gVeryVerbose) {
    Cout << Endl;
    if (strings.Length() == 0)
      Cout << "  Strings Buffer = **Empty**\n";
    else {
      Cout << "  Strings Buffer after Morph from Bytes...\n\n";
      for (i=0;i<strings.Length();i++) {
        line = strings.Get(i);
        Cout << "    " << line << Endl;
      }
    }
    Cout << Endl;
  }

  // More sanity checks
  if (dstBuf != buf)
    BEFE_WARN("String Morph Buffer not same address as Bytes Morph buffer");

  // Handle errors
  status = Error::None;
  while (false) {
    TOSTRINGSFAILED: BEFE_WARN("ToStrings failed"); break;
  }

  return status;

} // ...TestMorpning.Run

} // ...Namespace BEFE
