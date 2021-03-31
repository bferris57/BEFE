//----------------------------------------------------------------------
// File: ToStrings.cpp - Implementation of various Multiple line
//                       ToStrings utility functions
//----------------------------------------------------------------------
//
// In general, most of the "hard work" is done in the "normal form"
// functions.
//
// The "handy form" functions are here for ease of String formatting in
// C++ code.  Since the target of this stuff is intended for human eyes
// and NOT for other software, "sloppy is as sloppy does" so we don't
// care too much about possible runtime errors that could occur.
//
// However, if the output is intended to go somewhere to be processed
// by other software (like Files, on the network, etc.) then the
// "normal form" functions should be used instead of the handy ones.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "ToStrings.h"
#include "ToString.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Displaying to Cout
//

void DisplayStrings(Strings &strings) {
  DisplayStrings(strings,0);
}

void DisplayStrings(Strings &strings, UInt indent) {
  
  String ind;
  UInt   len;
  UInt   i;
  
  ind = String(' ');
  ind *= indent;
  len = strings.Length();
  for (i=0; i < len; i++)
    Cout << ind << strings.Get(i).Consumable() << '\n';

  return;
  
}

void DisplayMemory(Byte *vbuf, UInt len, UInt startoffset, UInt indent) {

  Status  status;
  Strings strings;
  String  string;
  UInt    numStr;
  UInt    strIdx;
  String  indentStr;

  status = ToStrings(vbuf, len, startoffset, strings);
  if (status) {
    BEFE_ERROR("DisplayMemory(ToStrings)",status);
    return;
  }

  indentStr = ' ';
  indentStr *= indent;

  numStr = strings.Length();
  for (strIdx=0; strIdx < numStr; strIdx++) {
    string = strings.Get(strIdx);
    Cout << indentStr << string << '\n';
  }

  return;

}

void DisplayMemory(Byte *vbuf, UInt len, UInt startoffset) {
  DisplayMemory(vbuf,len,startoffset,0);
}

//----------------------------------------------------------------------
//
// Instance -> Strings handy form
//

Strings ToStrings(Process &info) {

  Status  status;
  Strings strings;
  String  err;

  status = ToStrings(info,strings);
  if (status) {
    err = ErrorString(status);
    strings.Append(err);
  }
  return strings.Consumable();
}

Strings ToStrings(Platform &info) {

  Status  status;
  Strings strings;
  String  err;

  status = ToStrings(info,strings);
  if (status) {
    err = ErrorString(status);
    strings.Append(err);
  }
  return strings.Consumable();
}

Strings ToStrings(Processor &info) {

  Status  status;
  Strings strings;
  String  err;

  status = ToStrings(info,strings);
  if (status) {
    err = ErrorString(status);
    strings.Append(err);
  }

  return strings.Consumable();

}

Strings ToStrings(OperatingSystem &info) {

  Status  status;
  Strings strings;
  String  err;

  status = ToStrings(info,strings);
  if (status) {
    err = ErrorString(status);
    strings.Append(err);
  }

  return strings.Consumable();

}

Strings ToStrings(Network &info) {

  Status  status;
  Strings strings;
  String  err;

  status = ToStrings(info,strings);
  if (status) {
    err = ErrorString(status);
    strings.Append(err);
  }

  return strings.Consumable();

}

Strings ToStrings(IdSpaceInfo &info) {

  Status  status;
  Strings strings;
  String  err;

  status = ToStrings(info,strings);
  if (status) {
    err = ErrorString(status);
    strings.Append(err);
  }

  return strings.Consumable();

}

Strings ToStrings(FileInfo const &info) {

  Status  status;
  Strings strings;
  String  err;

  status = ToStrings(info,strings);
  if (status) {
    err = ErrorString(status);
    strings.Append(err);
  }

  return strings.Consumable();

}

Strings ToStrings(StreamStats const &stats) {

  Status  status;
  Strings strings;
  String  err;

  status = ToStrings(stats,strings);
  if (status) {
    err = ErrorString(status);
    strings.Append(err);
  }

  return strings.Consumable();

}

//----------------------------------------------------------------------
//
// Instance -> Strings normal form
//

Status ToStrings(Process &proc, Strings &strings) {

  String value;
  String outstr;

  // Platform General
  outstr  = "Executable: ";
  outstr += proc.GetExecutableFileName();
  strings.Append(outstr);
  outstr  = "Process ID: ";
  outstr += proc.GetID();
  strings.Append(outstr);

  return Error::None;

}

Status ToStrings(Platform &plat, Strings &strings) {

  String value;
  String outstr;
  Int    comma;
  String version;
  
  // Before we start...
  version = plat.GetVersionString();
  comma = version.Find(',');
  if (IsNull(comma)) {
    version += '.';
    version += plat.GetRevisionString();
  }
  else {
    version.Insert(comma, '.');
    version.Insert(comma+1, plat.GetRevisionString())  ;
  }
  
  // Platform General
  outstr  = "Platform:         ";
  outstr += plat.GetShortName();
  if (plat.GetBaseLibraries().Length()) {
    outstr += '/';
    outstr += plat.GetBaseLibraries();
  }
  strings.Append(outstr);
  
  outstr  = "Version:          ";
  outstr.Append(version);
  strings.Append(outstr);  
  
  outstr  = "O/S Name:         ";
  outstr += plat.GetOSName();
  strings.Append(outstr);

  outstr  = "O/S Description:  ";
  outstr += plat.GetOSDescription();
  strings.Append(outstr);

  outstr  = "O/S Version:      ";
  outstr += plat.GetOSVersion();
  strings.Append(outstr);

  outstr  = "O/S Vendor:       ";
  outstr += plat.GetOSVendor();
  strings.Append(outstr);

  // Until we get this working on Linux (because it needs root access)
  // we'll ignore it...
  if (!Is_Linux()) {
    
    outstr  = "Hardware Name:    ";
    outstr += plat.GetHWName();
    strings.Append(outstr);

    outstr  = "Hardware Model:   ";
    outstr += plat.GetHWModel();
    strings.Append(outstr);

    outstr  = "Hardware Version: ";
    outstr += plat.GetHWVersion();
    strings.Append(outstr);

    outstr  = "Hardware Vendor:  ";
    outstr += plat.GetHWVendor();
    strings.Append(outstr);

  }
  
  return Error::None;

}

Status ToStrings(Processor &proc, Strings &strings) {

  Long   memsize;
  String outstr;
  Int    speed;

  // Processor General
  outstr.SetEmpty();
  outstr << "Type:          " << proc.GetType().Ellipsis(50);
  strings.Append(outstr);
  outstr.SetEmpty();
  outstr << "Manufacturer:  " << proc.GetManufacturer().Ellipsis(50);
  strings.Append(outstr);
  outstr.SetEmpty();
  outstr << "Model:         " << proc.GetModel().Ellipsis(50);
  strings.Append(outstr);
  outstr.SetEmpty();
  outstr << "Description:   " << proc.GetDescription().Ellipsis(50);
  strings.Append(outstr);

  outstr.SetEmpty();
  outstr << "Byte Order:    ";
  switch (proc.GetByteOrder()) {
    case Processor::None:         outstr << "Unknown";     break;
    case Processor::BigEndian:    outstr << "BigEndian";    break;
    case Processor::LittleEndian: outstr << "LittleEndian"; break;
    default:                      outstr << "Unknown - " << proc.GetByteOrder(); break;
  }
  strings.Append(outstr);

  outstr.SetEmpty();
  outstr << "Register Size: " << proc.GetRegisterSize() << " bits";
  strings.Append(outstr);

  outstr.SetEmpty();
  outstr << "Pointer Size:  " << proc.GetPointerSize() << " bits";
  strings.Append(outstr);

  memsize = proc.GetMemorySize();
  outstr.SetEmpty();
  outstr << "Memory Size:   ";
  if (memsize == 0)
    outstr << "Unknown";
  else
    ByteUnits(memsize,outstr);
  strings.Append(outstr);

  speed = proc.GetSpeed();
  outstr.SetEmpty();
  outstr << "Clock Speed:   ";
  if (speed == 0)
    outstr << "Unknown";
  else
    outstr << speed << " MHz (~" << (speed+512)/1024 << " GHz)";
  strings.Append(outstr);

  return Error::None;

}

Status ToStrings(OperatingSystem &os, Strings &strings) {

  String  value;
  String  outstr;
  Strings names;
  String  name;
  UInt    maxlen;
  String  desc;
  UInt    i;

  // Platform General
  outstr  = "OS:                ";
  outstr += os.GetShortName();
  strings.Append(outstr);
  outstr  = "OS Name:           ";
  outstr += os.GetName();
  strings.Append(outstr);
  outstr  = "OS Description:    ";
  outstr += os.GetDescription();
  strings.Append(outstr);
  outstr  = "OS Version:        ";
  outstr += os.GetVersion();
  strings.Append(outstr);
  outstr  = "OS Vendor:         ";
  outstr += os.GetVendor();
  strings.Append(outstr);
  outstr  = "OS 16-bit Apps:    ";
  outstr += ((os.GetApps16()) ? CONSTR_Yes : CONSTR_No);
  strings.Append(outstr);
  outstr  = "OS 32-bit Apps:    ";
  outstr += ((os.GetApps32()) ? CONSTR_Yes : CONSTR_No);
  strings.Append(outstr);
  outstr  = "OS 64-bit Apps:    ";
  outstr += ((os.GetApps64()) ? CONSTR_Yes : CONSTR_No);
  strings.Append(outstr);
  outstr  = "Memory Page Size:  ";
  if (os.GetMemoryPageSize() == UNaN)
    outstr += "NaN";
  else {
    outstr += ToString(os.GetMemoryPageSize());
    outstr += " Bytes";
  }
  strings.Append(outstr);

  outstr  = "OS File Separator: '";
  outstr += os.GetFileSeparator();
  outstr += "'";
  strings.Append(outstr);
  outstr  = "OS Path Separator: '";
  outstr += os.GetPathSeparator();
  outstr += "'";
  strings.Append(outstr);
  outstr  = "OS List Separator: '";
  outstr += os.GetListSeparator();
  outstr += "'";
  strings.Append(outstr);
  outstr  = "OS Temporary Path: '";
  outstr += os.GetTempPath();
  outstr += "'";
  strings.Append(outstr);
  outstr  = "Host Name:         ";
  outstr += os.GetHostName();
  strings.Append(outstr);

  //
  // Devices...
  //

  // Get the device names
  names = os.GetDeviceNames();

  // Get maximum length
  maxlen = 0;
  for (i=0; i < names.Length(); i++)
    maxlen = Max(maxlen,names.Get(i).Length());

  // Put some spacing in
  name.SetEmpty();
  strings.Append(name);

  // Format them
  if (maxlen == 0)
    strings.Append("Devices: **None**");
  else {

    // If not verbose, simply list them
    if (!gVerbose) {
      outstr = "Devices: ";
      for (i=0; i<names.Length(); i++) {
        name = names.Get(i);
        if (i != 0)
          outstr += ", ";
        outstr += name;
      }
      strings.Append(outstr);
    }

    // It is verbose, give more detail
    else {

      for (i=0;i<names.Length();i++) {
        name = names.Get(i);
        if (i==0)
          outstr = "Devices: ";
        else
          outstr = "         ";
        name = names.Get(i);
        desc = os.GetDeviceDescription(name);
        name.Pad(maxlen);
        outstr += name;
        outstr += " - ";
        outstr += desc;
        strings.Append(outstr);
      }

    }

  }

  return Error::None;

}

Status ToStrings(Network &net, Strings &strings) {

  String  value;
  String  outstr;
  Strings addresses;
  String  addr;
  String  str;
  UInt    i;

  // Network General
  outstr  = "Host Name:      ";
  outstr += net.GetHostName();
  strings.Append(outstr);

  // Domain Names
  net.GetDomainNames(addresses);
  outstr  = "Domain Name:    ";
  for (i=0; i < addresses.Length(); i++) {
    addresses.Get(i,addr);
    if (addr.Length() > 0) {
      if (i > 0)
        outstr += ", ";
      outstr += addr;
    }
  }
  if (addresses.Length() == 0) {
    outstr += "**None**";
  }
  strings.Append(outstr);

  // MAC Addresses
  net.GetMACAddresses(addresses);
  str.SetEmpty();
  outstr = "MAC  Addresses: ";
  for (i=0;i<addresses.Length();i++) {
    addresses.Get(i,addr);
    if (addr.Length() > 0) {
      outstr += addr;
      strings.Append(outstr);
      outstr = "                ";
    }
  }
  if (addresses.Length() == 0) {
    outstr += "**None**";
    strings.Append(outstr);
  }

  // IPv4 Addresses
  net.GetIPv4Addresses(addresses);
  str.SetEmpty();
  outstr = "IPv4 Addresses: ";
  for (i=0;i<addresses.Length();i++) {
    addresses.Get(i,addr);
    if (addr.Length() > 0) {
      outstr += addr;
      strings.Append(outstr);
      outstr = "                ";
    }
  }
  if (addresses.Length() == 0) {
    outstr += "**None**";
    strings.Append(outstr);
  }

  return Error::None;
}

Status ToStrings(IdSpaceInfo &info,Strings &strings) {

  Status      status;
  String      outstr;

  // IdSpace header
  status = ToString(info);
  if (status) goto SOMEERROR;
  status = strings.Append(outstr);
  if (status) goto SOMEERROR;

  // IdSpace details
  outstr  = "  IdSpace:       ";
  outstr += (Int)info.id;
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;
  outstr  = "  Short Name:    ";
  outstr += info.shortName;
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;
  outstr  = "  Description:   ";
  outstr += info.description;
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;
  outstr  = "  Created:       ";
  outstr += ToString(info.creationTime,Time::Second);
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;
  outstr  = "  Last Accessed: ";
  outstr += ToString(info.accessTime,Time::Second);
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;
  outstr  = "  Last Updated:  ";
  outstr += ToString(info.updateTime,Time::Second);
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;
  outstr  = "  Next Id:       ";
  outstr += (Int)info.nextId;
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;
  outstr  = "  Id Range:      ";
  outstr += info.idRange.ToString();
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;
  outstr  = "  Id Count:      ";
  outstr += info.idCount;
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  if (status) {
    outstr += ErrorString(status);
    strings.Append(outstr);
  }

  return status;

}

Status ToStrings(FileInfo const &info, Strings &strings) {

  Status status;
  String outstr;

  outstr  = "Name:          '";
  outstr += info.name;
  outstr += '\'';
  outstr.Consumable();
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;

  outstr  = "Size:          ";
  if (info.size > 1024*1024 && !gVeryVerbose)
    outstr += ByteUnits(info.size);
  else {
    outstr += (UInt)info.size;
    outstr += " Bytes";
  }
  outstr.Consumable();
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;

  outstr  = "Created:       ";
  outstr += ToString(info.creationTime, Time::Seconds);
  outstr.Consumable();
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;

  outstr  = "Last Accessed: ";
  outstr += ToString(info.accessTime, Time::Seconds);
  outstr.Consumable();
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;

  outstr  = "Last Updated:  ";
  outstr += ToString(info.updateTime, Time::Seconds);
  outstr.Consumable();
  status  = strings.Append(outstr);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  if (status) {
    outstr += ErrorString(status);
    outstr.Consumable();
    strings.Append(outstr);
  }

  return status;

}

Status ToStrings(StreamStats const &stats, Strings &info) {

  Status status;
  String outLine;

  outLine = "Logical  Reads: ";
  outLine += stats.numLogReads;
  status = info.Append(outLine);
  if (status) goto SOMEERROR;
  outLine = "        Writes: ";
  outLine += stats.numLogWrites;
  status = info.Append(outLine);
  if (status) goto SOMEERROR;
  outLine = "Physical Reads: ";
  outLine += stats.numPhyReads;
  status = info.Append(outLine);
  if (status) goto SOMEERROR;
  outLine = "        Writes: ";
  outLine += stats.numPhyWrites;
  status = info.Append(outLine);
  if (status) goto SOMEERROR;
  outLine = "    Bytes Read: ";
  outLine += stats.numBytesRead;
  status = info.Append(outLine);
  if (status) goto SOMEERROR;
  outLine = "       Written: ";
  outLine += stats.numBytesWritten;
  status = info.Append(outLine);
  if (status) goto SOMEERROR;
  outLine = "    Lines Read: ";
  outLine += stats.numLineReads;
  status = info.Append(outLine);
  if (status) goto SOMEERROR;
  outLine = "       Written: ";
  outLine += stats.numLineWrites;
  status = info.Append(outLine);
  if (status) goto SOMEERROR;

  status = Error::None;

  // Handle Errors...
  while (false) {
    SOMEERROR: BEFE_ERROR("ToStrings(StreamStats &, Strings &)",status); break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Memory -> Strings
//

Strings ToStrings(Byte *vbuf, UInt len, UInt startoffset) {
  
  Strings strings;
  
  ToStrings(vbuf, len, startoffset, strings);
  
  return strings.Consumable();
  
}

Strings ToStrings(Buffer const &buf) {
  
  Strings strings;
  
  ToStrings(buf, strings);
  
  return strings.Consumable();
  
}

Strings ToStrings(Buffer const &buf, UInt startoffset) {
  
  Strings strings;
  
  ToStrings(buf, startoffset, strings);
  
  return strings.Consumable();

}

Strings ToStrings(BufferStream const &stream) {
  
  Strings strings;
  
  ToStrings(stream, strings);
  
  return strings.Consumable();
  
}

Strings ToStrings(BufferStream const &stream, UInt startoffset) {
  
  Strings strings;
  
  ToStrings(stream, startoffset, strings);
  
  return strings.Consumable();

}

//----------------------------------------------------------------------
//
// Memory -> Strings normal form
//

Status ToStrings(Byte *buf, UInt len, UInt startoffset, Strings &strings) {

  Byte    hexbuf[48];
  String  chrStr;
  Byte   *hcp;
  Int     i;
  Int     nib;
  Int     offset;
  String  string;
  Char    dot;

  if (gConsoleUTF8)
    //dot = 0x2593u; // U+2593 Dark Shade
    //dot = 0x25D8u; // U+25D8 Inverse Bullet
    //dot = 0x25A0u; // U+25A0 Black Square
    dot = 0x25CAu; // U+25CA Lozenge
  else
    dot = '.';

  offset = (int)startoffset;
    
  while (buf && len>0) {

    // Clear buffers
    MemoryFillRaw((Byte *)hexbuf,' ',sizeof(hexbuf)-1);
    hexbuf[sizeof(hexbuf)-1] = 0;
    hcp = hexbuf;
    chrStr.Reset();

    // Form buffers
    i  = 0;
    while (i < 16 && len != 0) {

      // Translate character
      if (*buf < 0x20 || *buf >= 0x7f)
        chrStr += dot;
      else
        chrStr += Char(*buf);

      // Output hex space if not first one
      if (i != 0)
        *hcp++ = ' ';

      // Process nib1
      nib = (*buf >> 4)&0x0f;
      if (nib < 10)
        *hcp++ = '0'+nib;
      else
       *hcp++ = 'A'+nib-10;

      // Process nib2
      nib = *buf & 0x0f;
      if (nib < 10)
        *hcp++ = '0'+nib;
      else
        *hcp++ = 'A'+nib-10;

      // Position to next character
      buf++;
      i++;
      len--;
    }

    // Append to strings...
    string = ToHexString(offset,8);
    string += ": ";
    string += (char *)hexbuf;
    string += "   ";
    string += chrStr;
    strings.Append(string);

    offset += 16;
  }

  return Error::None;

};

Status ToStrings(Buffer const &buf, Strings &strings) {
  return ToStrings(buf.GetBuf(), buf.Length(), (UInt)buf.GetBuf(), strings);
}

Status ToStrings(Buffer const &buf, UInt startoffset, Strings &strings) {
  return ToStrings(buf.GetBuf(), buf.Length(), startoffset, strings);
}

Status ToStrings(BufferStream const &stream, Strings &strings) {
  return ToStrings(stream.GetBuf(), stream.Length(), (UInt)stream.GetBuf(), strings);
}

Status ToStrings(BufferStream const &stream, UInt startoffset, Strings &strings) {
  return ToStrings(stream.GetBuf(), stream.Length(), startoffset, strings);
}

Strings ToStrings(Globals const &globs) {
  
  Strings ourStrings;
    
  ToStrings(globs, ourStrings);
  
  return ourStrings.Consumable();
  
}

Status ToStrings(Globals const &globs, Strings &strings) {

  Status status;

  status = globs.ToStrings(String(), strings);
  
  return status;
  
}

} // ...Namespace BEFE