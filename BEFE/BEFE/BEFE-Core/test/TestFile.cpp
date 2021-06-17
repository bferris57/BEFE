//!befe-publish src
//----------------------------------------------------------------------
// File: testFile.cpp - TestFile Class (tests File Class)
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "_GlobalMacros.h"
#include "Test.h"
#include "File.h"
#include "FileStream.h"
#include "Lines.h"

namespace BEFE { // Namespace BEFE...

#define MAXLINES 25

Status TestFile::Run() { // TestFile.Run...

  Status     status;
  String     name;
  String     value;
  File       file;
  FileStream stream;
  UInt       len;
  Lines      lines;
  UInt       numLines;
  Strings    strings;
  UInt       lineNo;
  String     line;
  UInt       towrite;
  UInt       numwrite;
  Byte      *bp;
  Time       times[5];
  Int        fileno;
  
  const char *names[] = {"BEFE_Dictionary.txt","c:/BEFE/t.txt","e:/t.txt",0};

  if (!gQuiet)
    out  << "***Testing File..." << Endl;

  //
  // Read each of the files
  //

  for (fileno=0; names[fileno] ; fileno++) {

    name   = names[fileno];
    file   = name;
    stream = name;

    if (gVerbose)
      out << "  Reading file '" << name << "'...\n";

    if (!TheBefe->TheOS->Exists(name) && TheBefe->TheOS->IsRelativePath(name)) {
      status = TheBefe->TheOS->ApplyRelativePath(TheBefe->GetEnvironmentValue("BEFE_TestData"), name, name);
      if (status) {
        BEFE_ERROR("TestFile.Run",status);
        continue;
      }
      if (gVerbose)
        out << "    Trying '" << name << "'...\n";
      file   = name;
      stream = name;
    }
    times[0].Set();
    status = file.ReadLines(lines);
    if (status)
      BEFE_ERROR("file.ReadLines",status);
    times[1].Set();

    if (gVerbose) {
      out << "    Elapsed Read Time = " << ToString(times[0],times[1]) << '\n';

      len = file.Length();
      if ( IsNull(len) )
        out << "    Lines = NaN\n";
      else
        out << "    Lines = " << len << ((len == 1)?" Byte":" Bytes") << Endl;
      out << "    File '" << String(file.GetName()).MidEllipsis(40) << "' has " << lines.Length() << " lines\n";

    }

    // Display the lines if we're supposed to...
    if (gVeryVerbose && fileno == 0) {
      numLines = lines.Length();
      if (numLines <= MAXLINES) {
        for (lineNo=0; lineNo < numLines; lineNo++)
          out << "    " << lineNo+1 << ":... " << lines.GetLine(lineNo).Ellipsis(60) << '\n';
      }
      else {
        for (lineNo=0; lineNo < MAXLINES; lineNo++)
          out << "    " << lineNo+1 << ":... " << lines.GetLine(lineNo).Ellipsis(60) << '\n';
        out << "\n  ...\n\n";
        for (lineNo=numLines-MAXLINES; lineNo < numLines; lineNo++)
          out << "    " << lineNo+1 << ":... " << lines.GetLine(lineNo).Ellipsis(60) << '\n';
      }
      out << Endl;
    }

  }

  //
  // Test temporary file names
  //

  // Get the temporary path name
  name = TheBefe->TheOS->GetTempPath();
  if (gVerbose)
    out << "  Temporary Path = '" << name << "'\n";

  // See if it exists
  if (TheBefe->TheOS->Exists(name)) {
    if (gVerbose) {
      out << "  File '" << name << "' DOES exist\n";
      if (TheBefe->TheOS->IsFile(name))
        out << "    ... and it's a File\n";
      if (TheBefe->TheOS->IsDirectory(name))
        out << "    ... and it's a Directory\n";
    }
  }
  else {
    if (gVerbose)
      out << "  File or Dir '" << name << "' DOES NOT exist\n";
  }

  // Get list of files on D:...
  strings = TheBefe->TheOS->GetDirectories(String("C:\\BEFE"));
  if (gVerbose) {
    out << Endl;
    out << "  C:\\BEFE has " << strings.Length() << " sub-directories";
    for (lineNo=0; lineNo < strings.Length(); lineNo++) {
      line = strings.Get(lineNo);
      out << "\n    '" << line << "'";
    }
    if (lineNo != 0)
      out << Endl;
  }

  //
  // Test Temporary files
  //

  if (gVerbose)
    out << "  Testing Temporary files...\n";

  // Clear the name so it will create it
  file.SetName();

  // Write stuff
  bp = (Byte *)"Hello world";
  towrite = Strlen(bp);

  // TEMP...
  status = file.Open();
  if (status) {
    BEFE_ERROR("Opening...", status);
  }
  // ...TEMP
  status = file.Append(bp,towrite,numwrite);
  
  if (status && gVerbose)
    BEFE_ERROR("Appending...", status);
  if (status) goto DONE;

  // Get the name and say what we did
  if (gVerbose) {
    name = file.GetName();
    out << "    Appended to temp file '" << name << "'\n";
  }

  // Now, close it
  status = file.Close();
  if (status && gVerbose)
    BEFE_ERROR("Closing...", status);
  if (status) goto DONE;
  
DONE:

  return status;

} // ...testFile.Run()

} // ...Namespace BEFE
