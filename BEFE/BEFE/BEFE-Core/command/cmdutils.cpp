//----------------------------------------------------------------------
// File: cmdutils.cpp - Implementaion of various command utilities
//----------------------------------------------------------------------
//
// gCommandScriptExts - List of Command Script file extensions
// BuildScriptExts    - Build gCommandScriptExts if not alread built
//
// OutputLines         - Print a Strings array contents as output lines
// OutputHeader        - Print BEFE standard header
// OutputFooter        - Print BEFE standard footer
//
// OutputPlatformInfo  - Print current platform information
// OutputProcessInfo   - Print current process information
// OutputProcessorInfo - Print current processor (CPU) information
// OutputOSInfo        - Print current OS information
// OutputNetworkInfo   - Print current Network information
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEWrappers.h"
#include "FormatOut.h"
#include "ToStrings.h"
#include "cmdutils.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// OutputLines         - Display a Strings array contents as output lines
// OutputHeader        - Display BEFE standard header
// OutputFooter        - Display BEFE standard footer
// OutputSwitches      - Display BEFE Global settings
//

void OutputLines(Strings const &lines) {
  
  String line;
  UInt   i;
  
  for (i=0; i < lines.Length(); i++) {
    lines.Get(i,line);
    Cout << line << "\n";
  }
  
}

void OutputHeader() {

  FormatOut fmt;
  Strings   lines;
  
  fmt.FormatHeader(lines);
  OutputLines(lines);
  
}

void OutputFooter() {
  
  FormatOut fmt;
  Strings   lines;
  
  fmt.FormatFooter(lines);
  OutputLines(lines);  
  
}

void OutputSwitches() {
  
  Strings lines;
  lines = ToStrings(TheBefe->TheGlobals);
  OutputLines(lines);
  
}

//----------------------------------------------------------------------
//
// OutputPlatformInfo  - Print current platform information
// OutputProcessInfo   - Print current process information
// OutputProcessorInfo - Print current processor (CPU) information
// OutputOSInfo        - Print current OS information
// OutputNetworkInfo   - Print current Network information
//

void OutputPlatformInfo() {

  Strings array;
  String  line;
  Status  status;
  UInt    i;

  if (IsNull(TheBefe->ThePlatform)) {
    Cout << "*** Current Platform Info not available ***" << Endl;
    goto DONE;
  }

  array.Append(line);

  status = ToStrings(*TheBefe->ThePlatform,array);
  if (status) {
    Cout << "*** Error " << status << " getting Platform Info ***" << Endl;
    goto DONE;
  }

  Cout << "Current Platform info..." << Endl;
  for (i=0; i < array.Length(); i++) {
    line.SetEmpty();
    array.Get(i,line);
    Cout << "  " << line << "\n";
  }

  Cout << Endl;

  while (false) {
    DONE: break;
  }
  
  return;

}

void OutputProcessInfo() {

  Strings array;
  String  line;
  Status  status;
  UInt    i;

  if (IsNull(TheBefe->TheProcess)) {
    Cout << "*** Current Process Info not available ***" << Endl;
    goto DONE;
  }

  array.Append(line);

  status = ToStrings(*TheBefe->TheProcess,array);
  if (status) {
    Cout << "*** Error " << status << " getting Process Info ***" << Endl;
    goto DONE;
  }

  Cout << "Current Process info..." << Endl;
  for (i=0; i < array.Length(); i++) {
    line.SetEmpty();
    array.Get(i,line);
    Cout << "  " << line << "\n";
  }

  Cout << Endl;

  while (false) {
    DONE: break;
  }

  return;

}

void OutputProcessorInfo() {

  Strings array;
  String  line;
  Status  status;
  UInt    i;

  if (IsNull(TheBefe->TheProcessor)) {
    Cout << "*** Current Processor (CPU) Info not available ***" << Endl;
    goto DONE;
  }

  array.Append(line);

  status = ToStrings(*TheBefe->TheProcessor,array);
  if (status) {
    Cout << "*** Error " << status << " getting Processor (CPU) Info ***" << Endl;
    goto DONE;
  }

  Cout << "Current Processor (CPU) info..." << Endl;
  for (i=0; i < array.Length(); i++) {
    line.SetEmpty();
    array.Get(i,line);
    Cout << "  " << line << "\n";
  }

  Cout << Endl;

  while (false) {
    DONE: break;
  }

  return;

}

void OutputOSInfo() {

  Strings array;
  String  line;
  Status  status;
  UInt    i;

  if (IsNull(TheBefe->TheOS)) {
    Cout << "*** Current Operating System Info not available ***" << Endl;
    goto DONE;
  }

  array.Append(line);

  status = ToStrings(*TheBefe->TheOS,array);
  if (status) {
    Cout << "*** Error " << status << " getting Operating System Info ***" << Endl;
    goto DONE;
  }

  Cout << "Current Operating System info..." << Endl;
  for (i=0; i < array.Length(); i++) {
    line.SetEmpty();
    array.Get(i,line);
    Cout << "  " << line << "\n";
  }

  Cout << Endl;

  while (false) {
    DONE: break;
  }

  return;
}

void OutputNetworkInfo() {

  Strings array;
  String  line;
  Status  status;
  UInt    i;

  if (IsNull(TheBefe->TheNetwork)) {
    Cout << "*** Current Network Info not available ***" << Endl;
    goto DONE;
  }

  array.Append(line);

  status = ToStrings(*TheBefe->TheNetwork,array);
  if (status) {
    Cout << "*** Error " << status << " getting Network Info ***" << Endl;
    goto DONE;
  }

  Cout << "Current Network info..." << Endl;
  for (i=0; i < array.Length(); i++) {
    line.SetEmpty();
    array.Get(i,line);
    Cout << "  " << line << "\n";
  }

  Cout << Endl;

  while (false) {
    DONE: break;
  }

  return;
}

//----------------------------------------------------------------------
//
// Message/error formatting
//

String CmdStringMessage(Status msgNo, String const &string) {
  return CmdStringMessage(ErrorString(msgNo).Consumable(), string);
}

String CmdIntMessage(Status msgNo, Int theInt) {
  return CmdIntMessage(ErrorString(msgNo).Consumable(), theInt);
}

String CmdUIntMessage(Status msgNo, UInt theUInt) {
  return CmdUIntMessage(ErrorString(msgNo).Consumable(), theUInt);
}

String CmdUIntMessage(Status msgNo, UInt theUInt1, UInt theUInt2) {
  return CmdUIntMessage(ErrorString(msgNo).Consumable(), theUInt1, theUInt2);
}

String CmdCharMessage(Status msgNo, Char theChar) {
  return CmdCharMessage(ErrorString(msgNo).Consumable(), theChar);
}

String CmdStatusMessage(Status msgNo, Status theError) {
  return CmdStatusMessage(GetMessage(msgNo).Consumable(), theError);
}

//----------------------------------------------------------------------
//
// String formatting
//

String CmdStringMessage(String const &msg, String const &string) {

  String theMsg;
  Int    pos;

  theMsg = msg;
  pos = theMsg.Find("${String}");
  if (!BEFE::IsNull(pos))
    theMsg.Replace(Span(pos, pos+9), string);

  return theMsg.Consumable();

}

String CmdIntMessage(String const &msg, Int theInt) {

  String theMsg;
  Int    pos;

  theMsg = msg;
  pos = theMsg.Find("${Int}");
  if (!BEFE::IsNull(pos))
    theMsg.Replace(Span(pos, pos+9), ToString(theInt));

  return theMsg.Consumable();

}

String CmdUIntMessage(String const &msg, UInt theUInt) {

  String theMsg;
  Int    pos;

  theMsg = msg;
  pos = theMsg.Find("${UInt}");
  if (!BEFE::IsNull(pos))
    theMsg.Replace(Span(pos, pos+9), ToString(theUInt));

  return theMsg.Consumable();

}

String CmdUIntMessage(String const &msg, UInt theUInt1, UInt theUInt2) {

  String theMsg;
  Int    pos;

  theMsg = msg;
  pos = theMsg.Find("${UInt1}");
  if (!BEFE::IsNull(pos))
    theMsg.Replace(Span(pos, pos+8), ToString(theUInt1));
  pos = theMsg.Find("${UInt2}");
  if (!BEFE::IsNull(pos))
    theMsg.Replace(Span(pos, pos+8), ToString(theUInt2));

  return theMsg.Consumable();

}

String CmdCharMessage(String const &msg, Char theChar) {

  String theMsg;
  Int    pos;

  theMsg = msg;
  pos = theMsg.Find("${Char}");
  if (!BEFE::IsNull(pos))
    theMsg.Replace(Span(pos, pos+9), theChar);

  return theMsg.Consumable();

}

String CmdStatusMessage(String const &msg, Status theError) {

  String theMsg;
  Int    pos;

  theMsg = msg;
  pos = theMsg.Find("${Status}");
  if (!BEFE::IsNull(pos))
    theMsg.Replace(Span(pos, pos+9), GetMessage(theError));

  return theMsg.Consumable();

}

} // ...Namespace BEFE