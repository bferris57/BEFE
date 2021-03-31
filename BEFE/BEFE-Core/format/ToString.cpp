//----------------------------------------------------------------------
// File: ToString.cpp - Implementaion of the ToString functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "ToString.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Byte Units -> String
//

String ByteUnits(Long num) {
  Status status;
  String string;
  status = ByteUnits(num,string);
  if (status)
    string = ErrorString(status);
  return string;
}

Status ByteUnits(Long num,String &string) {

  // Negate if negative
  if (num < 0)
    num = -num;

  // If zero or NaN, it's easy...
  if (num == 0) {
    string << "0 Bytes";
    goto DONE;
  }
  if (num == LongNaN) {
    string << "Unknown";
    goto DONE;
  }

  // If very verbose and fits in an int...
  if (gVeryVerbose && num < 0x7fffffffL) {
    string << (int)num << " Bytes";
    goto DONE;
  }

  // If it's less than 1KB...
  if (num < 1024) {
    string << (int)num << " B";
    goto DONE;
  }
  num = (num + 512)/1024;

  // If it's less than 1MB...
  if (num < 1024) {
    string << '~' << (int)num << " KB";
    goto DONE;
  }
  num = (num + 512)/1024;

  // If it's less than 1GB...
  if (num < 1024) {
    string << '~' << (int)num << " MB";
    goto DONE;
  }
  num = (num+512)/1024;

  // If it's less than 1TB...
  if (num < 1024) {
    string << '~' << (int)num << " GB";
    goto DONE;
  }
  num = (num+512)/1024;

  // Else, it's Terabytes....
  string << '~' << (int)num << " TB";

  while (false) {
    DONE: break;
  }

  return Error::None;
}

//----------------------------------------------------------------------
//
// Boolean -> String
//

String ToString(Boolean b) {
  return String((b)?CONSTR_True:CONSTR_False);
}

//----------------------------------------------------------------------
//
// Int -> String...
//

String ToString(Int16 num) {
  return ToString((Int64)num);
}

String ToString(Int16 num, UInt len) {

  String string;

  string = ToString((Int64)num);
  if (string.Length() > len) {
    if (len >= 6)
      string = string.MidEllipsis(len);
    else {
      string = '#';
      string *= len;
    }
  }

  return string.Consumable();

}

String ToString(UInt16 num) {
  return ToString((UInt64)num);
}

String ToString(UInt16 num, UInt len) {

  String string;

  string = ToString((UInt64)num);
  if (string.Length() > len) {
    if (len >= 6)
      string = string.MidEllipsis(len);
    else {
      string = '#';
      string *= len;
    }
  }

  return string.Consumable();

}

String ToString(Int32 num) {
  return (num==NaN) ? String("NaN") : ToString((Int64)num);
}

String ToString(Int32 num, UInt len) {

  String string;
  
  if (IsNull(num))
    string = CONSTR_NaN;
  else
    string = ToString((Int64)num);
  if (string.Length() > len) {
    if (len >= 6)
      string = string.MidEllipsis(len);
    else {
      string = '#';
      string *= len;
    }
  }
  else
    string.LeftPad(len);
  
  return string.Consumable();

}

String ToString(UInt32 num) {
  return (num==UNaN) ? String("UNaN") : ToString((UInt64)num);
}

String ToString(UInt32 num, UInt len) {

  String string;

  string = ToString((UInt64)num);
  if (string.Length() > len) {
    if (len >= 6)
      string = string.MidEllipsis(len);
    else {
      string = '#';
      string *= len;
    }
  }
  else
    string.LeftPad(len);

  return string.Consumable();

}

String ToString(Int64 num) {

  String  string;
  Byte   *bp;
  UInt    numBytes;
  Boolean hasSign;
  
  if (num == LongNaN) // NaN...

    string = "NaN";

  else { // Zero or Natural Integer...

    // Handle sign
    if (num < 0) {
      string += '-';
      num = -num;
      hasSign = true;
    }
    else
      hasSign = false;

    // Do it...
    do {
      string.Append((char)(num%10 + '0'));
      num /= 10;
    } while (num);

    // Reverse the string's contents
    string._BufAndSize(bp,numBytes);
    if (hasSign) {
      bp++;
      numBytes--;
    }
    MemoryReverseBytes(bp,numBytes);
    
  } // ...Zero or Natural Integer

  return string.Consumable();
}

String ToString(UInt64 num) {

  String  string;
  Byte   *bp;
  UInt    numBytes;

  if (num == UInt64NaN) // NaN...
  
    string = "NaN";
  
  else { // Zero or Natural Integer...

    // Do it...
    do {
      string += (char)(num%10 + '0');
      num /= 10;
    } while (num);

    // Reverse the string's contents
    string._BufAndSize(bp,numBytes);
    MemoryReverseBytes(bp,numBytes);

  }  // ...Zero or Natural Integer

  string.Consumable();
  return string;

}

String ToString(Int64 num, UInt len) {

  String string;

  string = ToString(num);
  if (string.Length() > len)
    string = string.MidEllipsis(len);
  else
    string.LeftPad(len);

  string.Consumable();
  return string;

}

String ToString(UInt64 num, UInt len) {

  String string;

  string = ToString(num);
  if (string.Length() > len)
    string = string.MidEllipsis(len);
  else
    string.LeftPad(len);

  string.Consumable();
  return string;

}

//----------------------------------------------------------------------
//
// Integer -> String...
//

String ToString(Integer const &num) {
  return num.ToString();
}

String ToString(Integer const &num, UInt len) {

  String string;

  string = num.ToString();
  if (string.Length() > len) {
    if (len > 6)
      string = string.MidEllipsis(len);
    else
      string = String("######").Get(Slice(0,len));
  }
  else
    string.LeftPad(len);

  string.Consumable();
  return string;

}

//----------------------------------------------------------------------
//
// Int -> HexString...
//

String ToHexString(Int32 num) {
  
  String answer;
  
  if (num == -1)
    answer = "FFFFFFFF";
  else if ( IsNaN(num) )
    answer = "NaN";
  else
    answer = ToHexString((UInt32)num);
  
  answer.Consumable();
  return answer;
  
}

String ToHexString(Int32 num, UInt len) {
  
  String answer;
  
  if (num == -1)
    answer = "FFFFFFFF";
  else if ( IsNaN(num) )
    answer = "NaN";
  else
    answer = ToHexString((UInt32)num, len);
  
  answer.Consumable();
  return answer;
  
}

String ToHexString(UInt32 num) {

  String  string;
  Byte    buf[8];
  Byte   *bp;

  // Initialise conversion loop
  bp   = buf + sizeof(buf)-1;

  // Special case for UNaN...
  //if ( IsNull(num) ) {
  //  string = "UNaN";
  //  goto DONE;
  //}

  // Convert to hex...
  do {
    *bp = num & 0x0f;
    if (*bp < 10)
      *bp-- += '0';
    else
      *bp-- += ('A'-10);
    num >>= 4;
  } while (num);
  bp++;

  // Add characters in buf to string...
  string.Append(bp,sizeof(buf)-(bp-buf));

  return string.Consumable();

}

String ToHexString(UInt32 num, UInt len) {

  String  string;
  Byte    buf[8];
  Byte   *bp;
  Int     reml;
  UInt32  tnum;
  Int     tlen;

  // Initialise conversion loop
  reml = sizeof(buf);
  bp   = buf + reml -1;
  tlen = len;
  tnum = num;
  
  // Special case for UNaN...
  /*
  if ( IsNull(num) ) {
    Memmove(buf,(Byte *)"....UNaN",8);
    if (tlen >= 8) {
      reml = 0;
      tlen -= 8;
      bp = buf;
    }
    else {
      tlen = 0;
      reml -= len;
      bp = buf+sizeof(buf) - len;
    }
    if (len >= 4) {
      string.Append(bp,sizeof(buf)-reml);
      goto DONE;
    }
    else {
      string  = '#';
      string *= len;
    }
    goto DONE;
  }
  */
  
  // Convert to hex...
  do {
    *bp = tnum & 0x0f;
    if (*bp < 10)
      *bp-- += '0';
    else
      *bp-- += ('A'-10);
    reml--;
    tlen--;
    tnum >>= 4;
  } while (reml && tlen);
  bp++;

  // If we've run out of given length and still have some number left...
  if (tlen == 0 && tnum) {
    if (len) {
      string = '#';
      string *= len;
    }
    goto DONE;
  }

  // If we've run out of buf, add extra pad at beginning
  if (reml == 0 && tlen) {
    string = '.';
    string *= tlen;
  }

  // Add characters in buf to string...
  string.Append(bp,sizeof(buf)-(bp-buf));
 
  while (false) {
    DONE: break;
  }

  string.Consumable();
  return string;

}

String ToHexString(Int64 num) {
  
  String answer;
  
  if (num == -1)
    answer = "FFFFFFFFFFFFFFFF";
  else if ( IsNaN(num) )
    answer = "NaN64";
  else
    answer = ToHexString((UInt64)num);
  
  answer.Consumable();
  return answer;
  
}

String ToHexString(Int64 num, UInt len) {
  
  String answer;
  
  if (num == -1)
    answer = "FFFFFFFFFFFFFFFF";
  else if ( IsNaN(num) )
    answer = "NaN64";
  else
    answer = ToHexString((UInt64)num, len);
  
  answer.Consumable();
  return answer;
  
}

String ToHexString(UInt64 num) {

  String  string;
  Byte    buf[16];
  Byte   *bp;

  // Initialise conversion loop
  bp   = buf + sizeof(buf)-1;

  // Special case for UNaN64...
  if ( IsNull(num) ) {
    Strcpy(buf,"UNaN64");
    bp = buf;
    goto DONE;
  }

  // Convert to hex...
  do {
    *bp = num & 0x0f;
    if (*bp < 10)
      *bp-- += '0';
    else
      *bp-- += ('A'-10);
    num >>= 4;
  } while (num);
  bp++;

  // Handle errors
  while (false) {
    DONE: break;
  }

  // Add characters in buf to string...
  string.Append(bp,sizeof(buf)-(bp-buf));

  string.Consumable();
  return string;

}

String ToHexString(UInt64 num, UInt len) {

  String  string;
  Byte    buf[16];
  Byte   *bp;
  Int     reml;
  UInt32  tnum;
  Int     tlen;

  // Initialise conversion loop
  reml = sizeof(buf);
  bp   = buf + reml -1;
  tlen = len;
  tnum = num;

  // Special case for UNaN...
  if ( IsNull(num) ) {
    Memmove(buf,(Byte *)"..........UNaN64",16);
    if (tlen >= 16) {
      reml = 0;
      tlen -= 16;
      bp = buf;
    }
    else {
      tlen = 0;
      reml -= len;
      bp = buf+sizeof(buf) - len;
    }
    if (len >= 4) {
      string.Append(bp,sizeof(buf)-reml);
      goto DONE;
    }
    else {
      string  = '#';
      string *= len;
    }
    goto DONE;
  }

  // Convert to hex...
  do {
    *bp = tnum & 0x0f;
    if (*bp < 10)
      *bp-- += '0';
    else
      *bp-- += ('A'-10);
    reml--;
    tlen--;
    tnum >>= 4;
  } while (reml && tlen);
  bp++;

  // If we've run out of given length and still have some number left...
  if (tlen == 0 && tnum) {
    if (len) {
      string = '#';
      string *= len;
    }
    goto DONE;
  }

  // If we've run out of buf, add extra pad at beginning
  if (reml == 0 && tlen) {
    string = '.';
    string *= tlen;
  }

  // Add characters in buf to string...
  string.Append(bp,sizeof(buf)-reml);

  while (false) {
    DONE: break;
  }

  string.Consumable();
  return string;

}

String ToHexString(Byte *bytes, UInt len) {
  
  String str;
  Byte   b;

  while (bytes && len--) {
    b = *bytes++;
    if ((b>>4) < 10)
      str += Char( (char)('0' + (b>>4)) );
    else
      str += Char( (char)('A' + ((b>>4)-10)) );
    if ((b&0x0f) < 10)
      str += Char( (char)('0' + (b&0x0f)) );
    else
      str += Char( (char)('A' + ((b&0x0f)-10)) );
  }

  return str.Consumable();
  
}

//----------------------------------------------------------------------
//
// Time -> String handy form
//

String ToString(Time t1, Time t2) {
  Status status;
  String string;
  status = ToString(t1,t2,string);
  if (status)
    string = ErrorString(status);
  return string;
}

String ToString(Time ts) {
  Status status;
  String string;
  status = ToString(ts,string);
  if (status)
    string = ErrorString(status);
  return string;
}

String ToString(Time ts, Time::Unit unit) {
  Status status;
  String string;
  status = ToString(ts,unit,string);
  if (status)
    string = ErrorString(status);
  return string;
}

//----------------------------------------------------------------------
//
// Time -> String normal form
//

Status ToString(Time t1, Time t2 ,String &formatted) {

  Time et;
  Int  weeks;
  Int  days;
  Int  hours;
  Int  minutes;
  Int  seconds;
  Int  milliseconds;

  et = t1;
  et = et.Delta(t2);

  if (et.Get() == LongNaN) {
    formatted += CONSTR_Null;
    goto DONE;
  }

  weeks        = et.GetAs(Time::Weeks);
  days         = et.GetAs(Time::Days);
  hours        = et.GetAs(Time::Hours);
  minutes      = et.GetAs(Time::Minutes);
  seconds      = et.GetAs(Time::Seconds);
  milliseconds = et.GetAs(Time::Milliseconds);

  if (weeks)
    formatted << weeks << "w";
  if (days)
    formatted << days << "d";
  if (hours)
    formatted << hours << "h";
  if (minutes)
    formatted << minutes << "m";
  if (seconds)
    formatted << seconds << "s";
  if (milliseconds || et.Get() == 0)
    formatted << milliseconds << "ms";

  while (false) {
    DONE: break;
  }

  return Error::None;
}

Status ToString(Time ts, String &formatted) {
  return ToString(ts,Time::Days,formatted);
}

Status ToString(Time ts, Time::Unit unit, String &formatted) {

  Long utc;
  Int  year;
  Int  month;
  Int  day;
  Int  hour;
  Int  minute;
  Int  second;
  Int  millisecond;

  utc = ts.Get();
  if (utc == LongNaN) {
    formatted += CONSTR_Null;
    goto DONE;
  }

  if (gShowLocalTime) {
    utc += TheBefe->TheOS->GetUTCTimeDiff();
    ts.Set(utc);
  }

  year        = ts.GetYear();
  month       = ts.GetMonthOfYear();
  day         = ts.GetDayOfMonth();
  hour        = ts.GetAs(Time::Hours);
  minute      = ts.GetAs(Time::Minutes);
  second      = ts.GetAs(Time::Seconds);
  millisecond = ts.GetAs(Time::Milliseconds);

  if (year <= 0) {
    formatted += (-year)+1;
    formatted += "BC";
  }
  else {
    if (year > 700)
      formatted += year;
    else {
      formatted += year;
      formatted += "AD";
    }
  }

  formatted += '-';
  if (month < 10)
    formatted += '0';
  formatted += month;

  formatted += '-';
  if (day < 10)
    formatted += '0';
  formatted += day;

  if (unit <= Time::Hour) {
  formatted += ' ';
  if (hour < 10)
    formatted += '0';
  formatted += hour;
  }

  if (unit <= Time::Minute) {
  formatted += ':';
  if (minute < 10)
    formatted += '0';
  formatted += minute;
  }

  if (unit <= Time::Second) {
  formatted += ':';
  if (second < 10)
    formatted += '0';
  formatted += second;
  }

  if (unit <= Time::Millisecond) {
  formatted += '.';
  if (millisecond < 100)
    formatted += '0';
  if (millisecond < 10)
    formatted += '0';
  formatted += millisecond;
  }

  if (unit <= Time::Hour)
    formatted += (gShowLocalTime)?" (Local Time)":" (UTC/GMT)";

  while (false) {
    DONE: break;
  }

  return Error::None;
}

//----------------------------------------------------------------------
//
// Sequence declarations -> String handy form
//

String ToString(Span span) {

  String string;

  string = span.ToString();
  
  string.Consumable();
  return string;

}

String ToString(USpan span) {

  String string;

  string = span.ToString();
  
  string.Consumable();
  return string;

}

String ToString(Range range) {

  String string;

  if (IsPrintableASCIIRange(range.idx1) && IsPrintableASCIIRange(range.idx2)) {
    string += '\'';
    string += (char)range.idx1;
    string += "'..'";
    string += (char)range.idx2;
    string += '\'';
  }
  else
    string += range.ToString();

  return string;

}

String ToString(URange range) {

  String string;

  if (IsPrintableASCIIRange(range.idx1) && IsPrintableASCIIRange(range.idx2)) {
    string += '\'';
    string += (char)range.idx1;
    string += "'..'";
    string += (char)range.idx2;
    string += '\'';
  }
  else {
    if ((Int)range.idx1 < 0 || (Int)range.idx2 < 0)
      string = range.ToHexString();
    else
      string = range.ToString();
  }

  return string;

}

String ToString(Slice slice) {

  String string;

  string = '[';
  if ( !IsNull(slice.idx1) )
    string += slice.idx1;
  string += ':';
  if ( !IsNull(slice.idx2) ||  !IsNull(slice.step) ) {
    if ( !IsNull(slice.idx2) )
      string += slice.idx2;
    string += ':';
    if ( !IsNull(slice.step) )
      string += slice.step;
  }
  string += ']';

  return string;

}

String ToString(Alloc alloc) {

  String string;

  string = alloc.ToString();

  return string.Consumable();

}

String ToString(IdSpaceInfo &info) {

  Status status;
  String string;
  String err;

  status = ToString(info,string);
  if (status)
    string = ErrorString(status);

  return string;

}

//----------------------------------------------------------------------
//
// Sequence declarations -> String normal form
//

Status ToString(Slice slice, String &string) {
  string = ToString(slice);
  return Error::None;
}

Status ToString(Alloc alloc, String &string) {
  string = ToString(alloc);
  return Error::None;
}

Status ToString(Range range, String &string) {
  string = range.ToString();
  return Error::None;
}

Status ToString(IdSpaceInfo &info, String &string) {

  Status      status;

  // IdSpace header
  string = "IdSpaces[";
  if ( !IsNull((Int)info.id) )
    string += (Int)info.id;
  else
    string += CONSTR_NaN;
  string += "] = '";
  if (!TheBefe->TheIdSpace) {
    string += CONSTR_StarUnknown;
    goto OK;
  }
  status = IdSpace::GetIdSpaceInfo(info.id,info);
  if (status) goto SOMEERROR;

  string += info.shortName;
  string += '\'';

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }
  if (status)
    string += ErrorString(status);

  return status;

}

//----------------------------------------------------------------------
//
// Handy SVN keyword string parsing functions
//
// Functions: SVNGetName  - Given SVN string, return Name in string
//            SVNGetValue - Given SVN string, return Value in string
//            SVNParse    - Parse an SVN string and return name and value
//

String SVNGetName(char const *rawsvn) {
  String name;
  String value;
  SVNParse(rawsvn,name,value);
  return name.Consumable();
}

String SVNGetValue(char const *rawsvn) {
  String name;
  String value;
  SVNParse(rawsvn,name,value);
  return value.Consumable();
}

void SVNParse(char const *rawsvn,String &name,String &value) {

  Strings parts;
  String  part;
  Slice   slice;
  UInt    partno;

  name.SetEmpty();
  value.SetEmpty();
  part += rawsvn;

  // Strip off leading and trailing '$'
  if (part.Get(0) == '$') {
    slice.Set(1,NaN);
    part.Get(slice,part);
  }
  if (part.Get(-1) == '$') {
    slice.Set(0,-1);
    part.Get(slice,part);
  }

  // Split it on the ':' (if it's there)
  parts = part.SplitToStrings(':');

  // Trim off leading and trailing spaces on each part...
  for (partno=0; partno < parts.Length(); partno++) {
    parts.Get(partno,part);
    slice.Set(1,NaN);
    while (part.Get(0) == ' ')
      part.Get(slice,part);
    slice.Set(0,-1);
    while (part.Get(-1) == ' ')
      part.Get(slice,part);
    parts.Set(partno,part);
  }

  // Return the parts
  parts.Get(0,name);
  parts.Get(1,value);

  return;
}

//----------------------------------------------------------------------
//
// MD5 functions
//

String ToHexString(MD5 const &that) {
  
  String str;
  
  str = ToHexString((Byte *)&that.value[0], sizeof(that.value));
  
  return str.Consumable();
  
}

} // ...Namespace BEFE