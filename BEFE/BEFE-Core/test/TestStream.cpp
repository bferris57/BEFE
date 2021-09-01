//!befe-publish src
//----------------------------------------------------------------------
// File: testStream.cpp - TestStream Class (tests Stream Class)
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
#include "Stream.h"
#include "FormatOut.h"

namespace BEFE { // Namespace BEFE...

Status TestStream::Run() {

  Stream      sbase;
  //StreamBit   sbit;
  //StreamASCII sascii;
  //StreamByte  sbyte;
  //StreamChar  schar;

  Stream     *streams[]   = {&sbase,/*&sbit,&sascii,&sbyte,&schar,*/NULL};
  const char *classname[] = {"Stream"
                            //,"StreamBit"
                            //,"StreamASCII"
                            //,"StreamByte"
                            ,"StreamChar"
                            ,0
                            };
  String      str;
  Byte        buf[256];
  Strings     fmt;
  UInt        sidx;
  UInt        i;
  FormatOut   fmtout;

  if (!gQuiet)
    out  << "***Testing Streams..." << Endl;

  // Print class capabilities
  for (sidx=0; !IsNull(streams[sidx]); sidx++) {

    if (gVerbose)
      Cout << "  Class " << classname[sidx] << " capability and sizes..." << Endl;

    fmt.SetEmpty();
    if (gVeryVerbose) {
      fmtout.FormatStreamBasics(*streams[sidx],fmt);
      fmtout.FormatStreamCapability(*streams[sidx],fmt);
      fmtout.FormatStreamSizes(*streams[sidx],fmt);
      for (i=0;i<fmt.Length();i++) {
        str.SetEmpty();
        fmt.Get(i,str);
        str.ToEllipsis(buf,sizeof(buf));
        Cout << "    " << (char *)buf << Endl;
      }
    }
  }

  return Error::None;
  
} // testStream::Run()

} // ...Namespace BEFE
