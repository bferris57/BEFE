//----------------------------------------------------------------------
// File: cmdutils.h - Declarations for various command utility functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CMDUTILS_H // CMDUTILS_H...
#define CMDUTILS_H

#include "Primitives.h"
#include "Strings.h"
#include "cmdutils.h"

namespace BEFE { // Namespace BEFE...

void OutputLines(Strings const &lines);
void OutputHeader();
void OutputFooter();
void OutputSwitches();

void OutputPlatformInfo();
void OutputProcessInfo();
void OutputProcessorInfo();
void OutputOSInfo();
void OutputNetworkInfo();

// Message/error formatting
String CmdStringMessage(Status msgNo, String const &string);
String CmdIntMessage(Status msgNo, Int theInt);
String CmdUIntMessage(Status msgNo, UInt theUInt);
String CmdUIntMessage(Status msgNo, UInt theUInt1, UInt theUInt2);
String CmdCharMessage(Status msgNo, Char theChar);
String CmdStatusMessage(Status msgNo, Status theError);

// String formatting
String CmdStringMessage(String const &message, String const &string);
String CmdIntMessage(String const &message, Int theInt);
String CmdUIntMessage(String const &message, UInt theUInt);
String CmdUIntMessage(String const &message, UInt theUInt1, UInt theUInt2);
String CmdCharMessage(String const &message, Char theChar);
String CmdStatusMessage(String const &message, Status theError);

} // ...Namespace BEFE

#endif // ...CMDUTILS_H