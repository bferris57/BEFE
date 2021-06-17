//!befe-publish src
//----------------------------------------------------------------------
// File: Globals.cpp - Implementation of BEFE Globals
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"

namespace BEFE { // Namespace BEFE...

// Forward Function Prototypes
static String GlobalMessage(Status msgNo);
static String GlobalStringMessage(Status msgNo, String const &substStr);
static String GlobalBoolMessage(Status msgNo, Boolean onOff);

//----------------------------------------------------------------------
//
// Global String Constants
//

const char *CONSTR_True         = "True";
const char *CONSTR_False        = "False";
const char *CONSTR_Yes          = "Yes";
const char *CONSTR_No           = "No";
const char *CONSTR_On           = "On";
const char *CONSTR_Off          = "Off";

const char *CONSTR_NaN          = "NaN";
const char *CONSTR_Null         = "Null";
const char *CONSTR_null         = "null";
const char *CONSTR_StarNull     = "**Null**";
const char *CONSTR_None         = "None";
const char *CONSTR_QQQ          = "???";
const char *CONSTR_StarNone     = "**None**";
const char *CONSTR_StarEmpty    = "**Empty**";
const char *CONSTR_NotAvailable = "Not Available";
const char *CONSTR_Unavailable  = "**Unavailable**";
const char *CONSTR_Deprecated   = "**Deprecated, do not use**";

const char *CONSTR_Generic      = "Generic";
const char *CONSTR_Unknown      = "Unknown";
const char *CONSTR_StarUnknown  = "**Unknown**";
const char *CONSTR_NotKnown     = "Not Known";

const char *CONSTR_Error        = "Error: ";
const char *CONSTR_Warning      = "Warning: ";
const char *CONSTR_Syntax       = "Syntax Error: ";

const char *CONSTR_Closed       = "Closed";
const char *CONSTR_Open         = "Open";

const char *CONSTR_Bytes        = "Bytes";
const char *CONSTR_KiloBytes    = "KB";
const char *CONSTR_MegaBytes    = "MB";
const char *CONSTR_GigaBytes    = "GB";
const char *CONSTR_TeraBytes    = "TB";

const char *CONSTR_SIGINT        = "!!! User Requested Termination !!!";

const char NUL  = 0x00;
const char BEL  = 0x07;
const char BS   = 0x08;
const char HT   = 0x09;
const char LF   = 0x0A;
const char Endl = 0x0A;
const char CR   = 0x0D;
const char ESC  = 0x1B;
const char SP   = 0x20;

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Globals::Globals() {
  StartUp();
}

Globals::Globals(Globals const &that) {
  CopyFrom(that);
}

Globals::~Globals() {
  ShutDown();
}

Globals &Globals::operator=(Globals const &that) {
  CopyFrom(that);
  return *this;
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Globals::StartUp() {

  // Non-Configurable Settings
  //longs.StartTime                  = Time::Now();
  //uints.DefaultBufSize          = 256;      // Default Buffer size (in Bytes)
  uints.DefaultBufSize          = 4096;     // Default Buffer size (in Bytes)
  uints.UnicodeMaxFullFoldChars = 32;       // Default String comparison Max Unfold Characters
  uints.UnicodeCurFullFoldChars = UNaN;     // Loaded  String comparison Max Unfold Characters
  uints.ConsoleHistory          = 10;       // Number of Console history lines (Null or > 1)

  // Flow control
  bools.FlowQuitNow           = false;        // true = "Exit Process/Thread NOW!!!"

  // Verbosity
  bools.Quiet                 = false;
  bools.Verbose               = false;
  bools.VeryVerbose           = false;
  bools.VeryVeryVerbose       = false;
  bools.ShowElapsed           = true;         // true = "Show process elapsed time at various applicable times"
  bools.ShowTimeStamps        = false;        // true = "Show time stamps at various places in output"
  bools.ShowLocalTime         = false;        // true = "Show local time not UTC time"
  bools.ShowDeprecated        = false;        // true = "Show use of deprecated functionality"
  bools.ShowWarnings          = false;        // true = "Show warnings"
  bools.ShowInfo              = false;        // true = "Show information while processing"
  bools.ShowDebug             = false;        // true = "Show debug output"
  //bools.ShowDebug             = true;         // true = "Show debug output"
  bools.ShowHeaders           = true;         // true = "Show BEFE Header/Trailer"

  // Files
  bools.KeepTemporaryFiles    = false;        // true = "Don't delete temporary files when closing them"
  bools.ShowHiddenFiles       = false;        // true = "Show hidden files"
  bools.ShowHiddenDirectories = false;        // true = "Show hidden directories"
  
  // Unicode Behaviour
  bools.UnicodeFolding        = false;        // true  = "Use Unicode Folding"
  bools.UnicodeTurkish        = false;        // false = "Use Turkish Unicoe Folding"
  
  // Memory usage
#if 0  // For MemoryStats debugging purposes...
  bools.LogMemory             = true;         // true = "Logging", false="No stats at all"
  bools.LogMemoryAlloc        = true;         // true = "Show each memory allocation"
  bools.LogMemoryAllocSizes   = true;         // true = "Keep size counts"
  bools.LogMemoryFree         = true;         // true = "Show each memory free"
  bools.ShowMemoryStats       = true;         // true = "Show MemoryStats on exit"
#else // Normal settings...
  bools.LogMemory             = true;         // true = "Logging", false="No stats at all"
  bools.LogMemoryAlloc        = false;        // true = "Show each memory allocation"
  bools.LogMemoryAllocSizes   = false;        // true = "Keep size counts"
  bools.LogMemoryFree         = false;        // true = "Show each memory free"
  bools.ShowMemoryStats       = false;        // true = "Show MemoryStats on exit"
#endif
  bools.LogMemoryFill         = false;        // true = "Show each memory fill"
  bools.LogMemoryCopy         = false;        // true = "Show each memory copy"
  bools.MemoryValidate        = false;        // true = "Validate Native Heap" (slows down everything)
  bools.MemoryValidateAbort   = false;        // true = "Abort if Native Heap Invalid"
  
  // Command processors
  bools.CommandIndent         = false;        // true = "Indent Shell Commands"
  bools.CommandEnvSubst       = true;         // true = "Perform Command Line Environment Variable Substitution"

  // Console extensions
  bools.ConsoleColour         = true;         // true = "Enable Console Colour"
  bools.ConsoleIntensity      = true;         // true = "Enable Console Intensity"
  bools.ConsoleUTF8           = true;         // true = "Enable Console UTF-8"
  bools.ConsoleBeep           = false;        // true = "Enable Console Beep (BEL)"
  bools.ConsoleInteractive    = false;        // true = "Force Console display prompt"
  
  // Others
  bools.PythonOutput          = false;        // true = "Form Python Friendly Output when possible"

  return Error::None;

};

Status Globals::CopyFrom(Globals const &that) {
  if (this != &that)
    MemoryCopyRaw((Byte *)&that,(Byte *)this,sizeof(Globals));
  return Error::None;
}

Status Globals::ShutDown() {
  return Error::None;
}

//----------------------------------------------------------------------
//
// Formatting
//

Strings Globals::ToStrings() const {
  Strings strings;
  ToStrings(String(), strings);
  return strings.Consumable();
}

Status Globals::ToStrings(String const &option, Strings &strings) const {

  Status  status;  
  String  s;
  Globals saveGlobs;
  Time    now;
  String  parts;

  //
  // Startup/Elapsed...
  //
  
  if (BEFE::IsNull(TheBefe)) goto NOBEFE;
  
  saveGlobs = TheBefe->TheGlobals;
  
  //
  // Configurable settings...
  //

  strings.Append(String());
        
  s = GlobalMessage(Error::GlobStrConfig);
  strings.Append(s.Consumable());
  strings.Append(String());

	s = GlobalBoolMessage(Error::GlobOpt_q, bools.Quiet);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_v, bools.Verbose);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_vv, bools.VeryVerbose);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_vvv, bools.VeryVeryVerbose);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_et, bools.ShowElapsed);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_ts, bools.ShowTimeStamps);
	strings.Append(s.Consumable());

	strings.Append(String());

	s = GlobalBoolMessage(Error::GlobOpt_localtime, bools.ShowLocalTime);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_dep, bools.ShowDeprecated);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_w, bools.ShowWarnings);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_info, bools.ShowInfo);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_debug, bools.ShowDebug);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_head, bools.ShowHeaders);
	strings.Append(s.Consumable());

	strings.Append(String());

	s = GlobalBoolMessage(Error::GlobOpt_keep, bools.KeepTemporaryFiles);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_allfiles, bools.ShowHiddenFiles);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_alldirs, bools.ShowHiddenDirectories);
	strings.Append(s.Consumable());

	strings.Append(String());

	s = GlobalBoolMessage(Error::GlobOpt_ufolding, bools.UnicodeFolding);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_uturkish, bools.UnicodeTurkish);
	strings.Append(s.Consumable());

	strings.Append(String());

	s = GlobalBoolMessage(Error::GlobOpt_m, bools.LogMemory);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_ma, bools.LogMemoryAlloc);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_mf, bools.LogMemoryFree);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_ms, bools.LogMemoryAllocSizes || bools.ShowMemoryStats);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_mv, bools.MemoryValidate);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_mva, bools.MemoryValidateAbort);
	strings.Append(s.Consumable());

	strings.Append("");

	s = GlobalBoolMessage(Error::GlobOpt_indent, bools.CommandIndent);
	strings.Append(s.Consumable());

	strings.Append(String());

	s = GlobalBoolMessage(Error::GlobOpt_cc, bools.ConsoleColour);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_ci, bools.ConsoleIntensity);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_cutf, bools.ConsoleUTF8);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GlobOpt_cutf, bools.ConsoleBeep);
	strings.Append(s.Consumable());
	s = GlobalBoolMessage(Error::GLobOpt_cint, bools.ConsoleInteractive);
	strings.Append(s.Consumable());

	s = GlobalBoolMessage(Error::GlobOpt_pyout, bools.PythonOutput);
	strings.Append(s.Consumable());

	strings.Append("");

	s = GlobalBoolMessage(Error::GlobOpt_envsubst, bools.CommandEnvSubst);
	strings.Append(s.Consumable());

  //
  // Non-configurable Settings...
  //
  
  strings.Append(String());

  s = GlobalMessage(Error::GlobStrNonConfig);
  strings.Append(s.Consumable());

  strings.Append(String().Consumable());
  
  s = GlobalStringMessage(Error::GlobStrBufferSize, ToString(uints.DefaultBufSize));
  strings.Append(s.Consumable());

  s = GlobalStringMessage(Error::GlobStrDefaultMaxUnfold, ToString(uints.UnicodeMaxFullFoldChars));
  strings.Append(s.Consumable());
  
  s = GlobalStringMessage(Error::GlobStrCurrentMaxUnfold, ToString(uints.UnicodeCurFullFoldChars));
  strings.Append(s.Consumable());

  // Finally, we now have all our messages in the strings parameter. Some of these contain the
  // '¶' character indicating they're column alligned. So, we have to figure out the maximum
  // size of each column and extend them with spaces appropriately...
  {
  	Ints colMax;
  	UInt numStrings = strings.Length();

  	for (UInt idx=0; idx < numStrings; idx++) {

  		String  str;
  		UInt    col = 0;
  		Span    span;

  		str = strings[idx];
  		span = Span(0,str.Length());

  		while (true) {

  			Int pos;

  			pos = str.Find("¶",span);
  			if (pos < 0) break;


  			while (col >= colMax.Length())
  				colMax.Append(0);

  			if (pos-span.idx1 > colMax[col])
  				colMax[col] = pos-span.idx1;

  			span.idx1 = pos+1;

  			++col;

  		}


  	}

  	// Now, we've got pad info, adjust each string...
  	for (UInt idx=0; idx < numStrings; idx++) {

  		String  str;
  		String  out;
  		Int     col = 0;
  		Span    inSpan;
  		Span    outSpan;
  		String  outFrag;

  		str = strings[idx];
  		inSpan = Span(0,str.Length());

  		while (true) {

  			Int    pos;

  			pos = str.Find("¶",inSpan);
  			if (pos < 0) break;

  			outSpan.idx1 = inSpan.idx1;
  			outSpan.idx2 = pos;
  			outFrag = str.Get(outSpan);
  			outFrag = outFrag.Pad(colMax[col]);
  			out.Append(outFrag);

  			inSpan.idx1 = pos+1;

  			++col;

  		}

  		out.Append(str.Get(inSpan));
  		strings[idx] = out;

  	}

  }
  //
  // Handle errors
  //

  status = Error::None;
  while (false) {
    NOBEFE: status = Error::NoBefe; break;
  }
  
  TheBefe->TheGlobals = saveGlobs;
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Local Functions
//

String GlobalMessage(Status msgNo) { // GlobalMessage...

  String str;

  if(!BEFE::IsNull(TheBefe))
    str = TheBefe->GetMessage(msgNo);

  return str.Consumable();

} // ...GlobalMessage

String GlobalStringMessage(Status msgNo, String const &substStr) { // GlobalStringMessage...

  String str;
  Int    pos;

  if (!BEFE::IsNull(TheBefe)) {
    str = TheBefe->GetMessage(msgNo);
    pos = str.Find("${String}");
    if (!BEFE::IsNull(pos))
      str.Replace(Span(pos, pos+9), substStr);
  }

  return str.Consumable();

} // ...GlobalStringMessage

String GlobalBoolMessage(Status msgNo, Boolean onOff) { // GlobalBoolMessage...

  String  str;
  UInt    pos;
  Char    paragraph;
  Strings parts;

  if (!BEFE::IsNull(TheBefe)) {
    paragraph = Char("¶");
    str = "  ";
    str += TheBefe->GetMessage(msgNo);
    pos = str.Find("${flag}");
    if (!BEFE::IsNull(pos))
      str.Replace(Span(pos, pos+7), (onOff)?"+":"-");
    pos = str.Find("${OnOff}");
    if (!BEFE::IsNull(pos))
      str.Replace(Span(pos, pos+8), (onOff)?CONSTR_On:CONSTR_Off);
  }

  return str.Consumable();

} // ...GlobalBoolMessage

} // ...Namespace BEFE
