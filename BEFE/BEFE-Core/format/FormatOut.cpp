//----------------------------------------------------------------------
// File: FormatOut.cpp - Implementaion of the FormatOut Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "FormatOut.h"
#include "ToString.h"

namespace BEFE { // Namespace BEFE...

#define OUTSIZE(n)                \
  if ( !IsNull(n) )                   \
   {if (n%8==0) {                 \
      s << n/8;                   \
      if (n/8 == 1)               \
        s << " Byte";             \
      else                        \
        s << " Bytes";            \
    } else {                      \
      s << n << " Bits (";        \
      if ((n+7)/8 == 1)           \
        s << (n+7)/8 << " Byte)"; \
      else                        \
        s << (n+7)/8 << " Bytes)";\
    }                             \
  } else {s << "NaN";}

//----------------------------------------------------------------------
//
// Class FormatOut constructors and destructors
//
//----------------------------------------------------------------------

FormatOut::FormatOut() {}
FormatOut::~FormatOut() {}

//----------------------------------------------------------------------
//
// Miscellaneous non-class related output formatting
//
//-------------------------------------------------------------------

Status FormatOut::FormatPageSeparator(Strings &formatted) {

  String header;

  if (gConsoleUTF8)
    header = "─";
  else
    header = '-';

  header *= 60;
  return formatted.Append(header);
}

Status FormatOut::FormatCopyrights(Strings &formatted) {

  Status retStatus;
  Status status;
  String header;

  if (gConsoleUTF8) 
    header = "Copyright © 2011-2017 Bruce Ferris (UK), all rights reserved.";
  else
    header = "Copyright (C) 2011-2017 Bruce Ferris (UK), all rights reserved.";
  
  retStatus = formatted.Append(header);
  if (gVeryVerbose) {
    status = formatted.Append(String());
    if (status && !retStatus) retStatus = status;
    status = formatted.Append("    Email: befe@bferris.co.uk");
    if (status && !retStatus) retStatus = status;
    status = formatted.Append("  Twitter: @BEFESoftware");
    if (status && !retStatus) retStatus = status;
    status = formatted.Append("     Blog: http://befe-software-library.blogspot.com");
    if (status && !retStatus) retStatus = status;
  }
  
  return retStatus;
  
}

Status FormatOut::FormatHeader(Strings &array) {

  String      line;
 
  FormatPageSeparator(array);

  if (TheBefe && !TheBefe->GetApplicationName().IsNull()) {
		
		String name;
		String desc;
		UInt   version;
		UInt   subVersion;
		UInt   revision;
		
		name       = TheBefe->GetApplicationName();
		desc       = TheBefe->GetApplicationDescription();
		version    = TheBefe->GetApplicationVersion();
		subVersion = TheBefe->GetApplicationSubVersion();
		revision   = TheBefe->GetApplicationRevision();
		
		line = name;
		if (!IsNull(version)) {
			line << " (";
			line << version;
			if (!IsNull(subVersion))
				line << '.' << subVersion;
			if (!IsNull(revision))
				line << '.' << revision;
			line << ')';
		}
		if (!IsNull(desc))
			line << " - " << desc;
	}

  else {
    line = "BEFE - Better Software";
    if (gShowTimeStamps) {
      line += " @ ";
      if (gVeryVerbose)
        line += ToString(Time::Now(),Time::Milliseconds);
      else if (gVerbose)
        line += ToString(Time::Now(),Time::Seconds);
      else
        line += ToString(Time::Now());
    }
  }

  array.Append(line);
	
  line.SetEmpty();
  array.Append(line);
  FormatCopyrights(array);
  FormatPageSeparator(array);

  return Error::None;

}

Status FormatOut::FormatFooter(Strings &array) {

  String      line;
  Time        st;
  Time        et;

  FormatPageSeparator(array);
  line << "Finished";
  if (gShowTimeStamps) {
    line += " @ ";
    line += ToString(Time::Now());
  }
  if (gShowElapsed) {
    st.Set(gStartTime);
    et.Set();
    line << "     (Elapsed time: ";
    line += ToString(st,et);
    line << ")";
  }
  array.Append(line);
  FormatPageSeparator(array);

  return Error::None;

}

//----------------------------------------------------------------------
//
// Stream formatting
//
//----------------------------------------------------------------------

Status FormatOut::FormatStreamBasics(Stream &stream, Strings &strcaps) {

  String s;
  int    caps;
  int    elmsiz;
  Range  range;

  // Get all capabilities
  caps   = stream.GetCapability();
  elmsiz = stream.GetElementSize();
  range  = stream.PosRange();

  // Format capabilities
  #define FORMAT(thecap) ((caps&Stream::thecap)?"Yes":"No")
  s << "Can get elements:   " << FORMAT(CanGet) << " (read)";
  strcaps.Append(s);
  s.SetEmpty();
  s << "Can put elements:   " << FORMAT(CanPut) << " (write)";
  strcaps.Append(s);

  // Format Sizes
  s.SetEmpty();
  s  = "Element size:       ";
  OUTSIZE(elmsiz);
  strcaps.Append(s);
  s  = "Position Range:     ";
  s += ToString(range);
  strcaps.Append(s);

  return Error::None;

}

Status FormatOut::FormatStreamCapability(Stream &stream, Strings &strcaps) {

  String s;
  int    caps;

  // Get all capabilities
  caps      = stream.GetCapability();

  // Format capabilities
  #define FORMAT(thecap) ((caps&Stream::thecap)?"Yes":"No")
  s.SetEmpty();
  s << "Fixed Element Size: " << FORMAT(FixedElementSize);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Fixed Total Size:   " << FORMAT(FixedTotalSize);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Known Size:         " << FORMAT(KnownSize);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Has Buffer:         " << FORMAT(HasBuffer);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Can Buffer:         " << FORMAT(CanBuffer);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Can Position:       " << FORMAT(CanPosition);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Can Put elements:   " << FORMAT(CanPut);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Can Get elements:   " << FORMAT(CanGet);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Can Flush elements: " << FORMAT(CanFlush);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Can Truncate:       " << FORMAT(CanTruncate);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Can Look Ahead:     " << FORMAT(CanLookAhead);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Can Look Behind:    " << FORMAT(CanLookBehind);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Is Fixed Length:    " << FORMAT(IsFixedLength);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Is Raw:             " << FORMAT(IsRaw);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Is UTF-8:           " << FORMAT(IsUTF8);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Is BigEndian:       " << FORMAT(IsBigEndian);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Is Structured:      " << FORMAT(IsStructured);
  strcaps.Append(s);
  s.SetEmpty();
  s << "Uses Other Stream:  " << FORMAT(CanTruncate);
  strcaps.Append(s);
  #undef FORMAT

  return Error::None;

}

Status FormatOut::FormatStreamSizes(Stream &stream, Strings &strcaps) {

  String s;
  int    elmsiz;
  Range  range;

  // Get all capabilities
  elmsiz  = stream.GetElementSize();
  range   = stream.PosRange();

  // Format Sizes
  s  = "Element size:       ";
  OUTSIZE(elmsiz);
  strcaps.Append(s);
  s.SetEmpty();
  s  = "Positional Range:   ";
  s += ToString(range);
  strcaps.Append(s);
  #undef OUTSIZE

  return Error::None;

}

//----------------------------------------------------------------------
//
// String formatting
//
//----------------------------------------------------------------------

Status FormatOut::DumpStringInfo(String *string) {

  Strings info;
  String  line;
  UInt    i;

  StringInfo(string,info);
  for (i=0;i<info.Length();i++) {
    info.Get(i,line);
    Cout << line << Endl;
  }

  return Error::None;

}

Status FormatOut::StringInfo(String *string,Strings &strinfo) {

  Status status;
  String s;

  // If NULL pointer, it's easy
  s.SetEmpty();
  if (IsNull(string)) {
    s << "String @ NULL";
    strinfo.Append(s);
    goto OK;
  }
  s.SetEmpty();
  s << "String @ 0x" << ToHexString((Int)string,8);
  strinfo.Append(s);

  // Format flags
  s.SetEmpty();
  s << "  Mutable:  ";
  s << ((string->IsMutable())?"Yes":"No");
  strinfo.Append(s);
  if (!(string->IsASCII() | string->IsUTF8())) {
    s = "  Is ASCII: NaN <-- Unknown";
    strinfo.Append(s);
    s = "  Is UTF-8: NaN <-- Unknown";
    strinfo.Append(s);
  }
  else {
    s.SetEmpty();
    s << "  Is ASCII: ";
    s << ((string->IsASCII())?"Yes":"No");
    strinfo.Append(s);
    s.SetEmpty();
    s << "  Is UTF-8: ";
    s << ((string->IsUTF8())?"Yes":"No");
    strinfo.Append(s);
  }
  s.SetEmpty();
  s << "  Type:     ";
  switch (string->GetType()) {
    case String::Null:      s << "Null";            break;
    case String::Const:     s << "Const";           break;
    case String::ConstLen:  s << "Constlen";        break;
    case String::ASCII0:    s << "Empty (0 bytes)"; break;
    case String::ASCII1:    s << "ASCII 1 byte";    break;
    case String::ASCII2:    s << "ASCII 2 bytes";   break;
    case String::ASCII3:    s << "ASCII 3 bytes";   break;
    case String::ASCII4:    s << "ASCII 4 bytes";   break;
    case String::UtfInline: s << "Unicode (Char)";  break;
    case String::Buffer:    s << "Buffer";          break;
    default:
      s << "**Unknown**";
      break;
  }
  strinfo.Append(s);

  // Length
  s.SetEmpty();
  s << "  Length:   " << string->Length() << " (Chars)";
  strinfo.Append(s);
  s.SetEmpty();
  s << "  Size:     " << string->Size() << " (Bytes)";
  strinfo.Append(s);

  // Multiplier
  if (string->Multiplier() != 1) {
    s.SetEmpty();
    s << "  Multiple: " << string->Multiplier();
    strinfo.Append(s);
  }

  // Handle errors
  status = Error::None;
  while (false) {
    OK: status = Error::None; break;
  }

  return status;

}

} // ...Namespace BEFE
