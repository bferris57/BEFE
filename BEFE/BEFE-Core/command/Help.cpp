//----------------------------------------------------------------------
// File: Help.cpp - Help Function
//----------------------------------------------------------------------
//
// This file contains the Help function implementation.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Help.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Help - Display Help on a given subject
//

Status Help(String const &pFileName, String const &subject) {
  
  Status  status;
  String  fileName;
  Lines   lines;
  UInt    numLines;
  UInt    lineNo;
  String  line;
  String  begin;
  String  end;
  UInt    firstNw;
  Boolean saveWarn;  
  
  saveWarn = gShowWarnings;
  gShowWarnings = true;

  if (BEFE::IsNull(TheBefe)) goto NOBEFE;
  
  // Find and load the help file...
  if (!pFileName.IsNull())
    fileName = pFileName;
  else
    fileName = TheBefe->GetEnvironmentValue("BEFE_HelpFile");
  if (fileName.IsEmpty())
    fileName = "BEFE_ShellHelp.bhlp";
    
  if (fileName.IsEmpty()) goto NOHELP;
  if (!TheBefe->TheOS->IsFullPath(fileName)) {
    status = TheBefe->FindConfigurationFile(fileName);
    if (status) goto SOMEERROR;
  }
  if (!TheBefe->TheOS->Exists(fileName) || !TheBefe->TheOS->IsFile(fileName)) goto NOTFOUND;
  status = lines.Load(fileName);
  if (status) goto SOMEERROR;
  
  // Search for subject'...'
  begin = subject + "...";
  begin.Insensitive();
  end   = String("...") + subject;
  end.Insensitive();
  numLines = lines.Length();
  for (lineNo=0; lineNo < numLines; lineNo++) {
    line = lines.GetLine(lineNo);
    if (line.Get(Span(line.FirstNonWhite(),begin.Length())) == begin) break;
  }
  if (lineNo >= numLines) goto NOTFOUND;
  
  // Dump until end of file or end of subject
  while (++lineNo < numLines) {
    line = lines.GetLine(lineNo);
    if (line.Get(Span(0,end.Length())) == end) break;
    firstNw = line.FirstNonWhite();
    if (BEFE::IsNull(firstNw)    ||
        line.Get(firstNw) == '#' ||
        line.Get(Span(firstNw,firstNw+2)) == "//") continue;
    status = Unescape(line, line);
    if (status) goto SOMEERROR;
    Cout << line << '\n';
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOBEFE:    status = Error::NoBefe;
               break;
    NOHELP:    BEFE_WARN(String("*** No help file configured ***"));
               status = Error::None;
               break;
    NOTFOUND:  BEFE_WARN(String("*** No help available for subject '")+subject+"'***");
               status = Error::None;
               break;
    SOMEERROR: BEFE_ERROR("Help",status);
               break;
  }

  gShowWarnings = saveWarn;
  
  return status;
  
}

Status Help(String const &subject) {
  String fileName;
  return Help(fileName, subject);
}
  
} // ...Namespace BEFE