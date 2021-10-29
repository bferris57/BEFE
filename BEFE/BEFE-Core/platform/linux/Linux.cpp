//----------------------------------------------------------------------
// File: linux.cpp - Base functions for the Linux port
//----------------------------------------------------------------------
//
// This file provides the port specific implementations of functions
// and globals used throughout the BEFE code base.
//
// The functions and globals here are referenced in the header
// file "BEFE.h" and "globals.h" and throughout the code.
//
// The reason these functions are here is because they are, or we
// expect them to be, very port specific and may need to be written in
// different forms on each Platform, Processor, Operating System,
// and so on.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Linux.h"
#include "ToAscii.h"

#include <cstdlib>

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Various Functions...
//
//----------------------------------------------------------------------

Time LinuxTimespecToTIme(struct timespec &spec) {

  Long val;

  val = (spec.tv_sec - Time::EpochDaysSince1970)*100000L + ((spec.tv_nsec+500)/1000);

  return Time(val);

}
  
//----------------------------------------------------------------------
//
// Various file/path functions...
//
//----------------------------------------------------------------------

const Char  pathseparator = '/';
const char *LinuxReservedFileChars = "<>:\"'/\\|?*";

Status LinuxStat(String const &dirName, struct stat *retInfo) {

  Status       status;
  Byte         buf[PATH_MAX];
  UInt         bufl;
  UInt         winCount;
  UInt         natCount;
  Byte        *tBuf;
  struct stat  info;
  int          rc;
  
  // Doesn't exist if it doesn't have a name
  if (dirName.Length() == 0) goto NONAME;

  // Doesn't exist if it's not a full path...
	if (!LinuxIsFullPath(dirName)) goto BADNAME;
	
  // Get the full name bytes
  dirName._BufAndSize(tBuf,bufl);
  if (!tBuf || !bufl) goto BADNAME;
  if (bufl >= PATH_MAX) goto TOOLONG;
  memcpy(buf, tBuf, bufl);
  buf[bufl] = 0x00;
    
  // Convert to Native format if needed
  natCount = dirName.Count('/');
  winCount = dirName.Count('\\');
  if (winCount && !natCount)
    for (tBuf=buf; *tBuf; tBuf++) if (*tBuf == '\\') *tBuf = '/';
  
  // Strip off trailing '/'
  if (bufl && buf[bufl-1] == '/')
    buf[bufl-1] = 0x00;

  // stat it...
  rc = stat((const char *)buf, &info);
  if (rc) goto NOTEXIST;
  if (retInfo)
    *retInfo = info;

  // Handle errors...
  status = Error::None;
  while (true) {
    BADNAME:  status = Error::FileInvalidPathName; break;
    NONAME:   status = Error::FileNoName;          break;
    TOOLONG:  status = Error::FileNameTooLong;     break;
    NOTEXIST: status = Error::FileDoesNotExist;    break;
  }
 
  return status;

}

Status LinuxPathSplit(String const &path, Strings &parts) {

  Status status;
  Char   splitchar;

  if (path.Count('\\') != 0)
    splitchar = '\\';
  else if (path.Count('/') != 0)
    splitchar = '/';
  else
    splitchar = (char)0;

  status = Error::None;
  status = path.SplitToStrings(splitchar,parts);

  return status;

}

Status LinuxPathJoin(Strings const &parts, String &full) {

  Status status;

  full.SetEmpty();
  full = parts.Join(pathseparator);

  status = Error::None;

  // Sanity check...
  if (parts.Length() != 0 && full.Length() == 0)
    status = Error::Internal;

  return status;

}

Boolean LinuxIsFullPath(String const &path) {

  UInt    pathLen;
  Char    chars[3];
  Boolean answer;

  pathLen = path.Length();

  // Must be at least three long
  if (pathLen < 3) goto NOPE;

  // Get the first three characters
  chars[0] = path.Get(0);
  chars[1] = path.Get(1);
  chars[2] = path.Get(2);

  // Special case for Network (SMB) file names
  if ((chars[0] == '\\' && chars[1] == '\\') ||
      (chars[0] == '/'  && chars[1] == '/' ) ) goto YEP;

  // Make sure it's <Drive Letter>:
  if (  chars[1] == ':' &&
       (chars[2] == '\\' || chars[2] == '/') &&
       ((chars[0] >= 'a' && chars[1] <= 'z') || (chars[0] >= 'A' && chars[1] <= 'Z') )
     ) goto YEP;

  // Handle errors
  answer = false;
  while (false) {
    NOPE: answer = false; break;
    YEP:  answer = true;  break;
  }

  return answer;

}

Boolean LinuxIsRelativePath(String const &path) {

  Status  status;
  Strings parts;
  UInt    numParts;
  UInt    partNo;
  String  part;
  Boolean answer;
  Char    firstChar;

  // Special case for empty, local, and remote paths
  if (path.Length() == 0) goto NOPE;
  if (LinuxIsRemotePath(path)) goto NOPE;
  if (LinuxIsLocalPath(path)) goto NOPE;

  // Special case if starts with '~'...
  firstChar = path.Get(0);
  if (firstChar == '~' || firstChar == '!' || firstChar == '@') goto YEP;
  
  // Special case for  '\'...
  if (firstChar == '\\' || firstChar == '/') goto YEP;

  // Special case for no '\' or '/'...
  if (path.Count('\\') == 0 && path.Count('/') == 0) goto YEP;
  
  // Pull apart by the Path Separator
  status = LinuxPathSplit(path,parts);
  if (status) goto SOMEERROR;
  numParts = parts.Length();

  // For each Part...
  for (partNo=0; partNo < numParts; partNo++) {

    part = parts.Get(partNo);
    if (part.Length() == 0) continue;
    if (part == ".." || part == ".") break;
    if (!LinuxIsValidFileName(part)) goto NOPE;

  }

  answer = (partNo < numParts);
  
  // Handle errors...
  status = Error::None;
  while (false) {
    SOMEERROR:
    NOPE:      answer = false; break;
    YEP:       answer = true;  break;
  }

  return answer;

}

Boolean LinuxIsLocalPath(String const &fullPath) {
  
  Int foundPosColon;
  Int foundPos;
  
  if (!LinuxIsFullPath(fullPath))
    return false;
  foundPosColon = fullPath.Find(':');
  if (IsNull(foundPosColon)) 
    return false;
  foundPos = fullPath.Find('/');
  if (foundPos >= 0)
    return foundPosColon < foundPos;
  foundPos = fullPath.Find('\\');
  if (foundPos >= 0)
    return foundPosColon < foundPos;
  
  return false;
  
}

Boolean LinuxIsRemotePath(String const &fullPath) {
  String firstTwo;
  firstTwo = fullPath.Get(Span(0,2));
  return firstTwo == "//" || firstTwo == "\\\\";
}

Boolean LinuxIsValidFileName(String const &fileName) {

  Boolean  answer;
  Byte    *buf;
  UInt     size;
  Byte    *curChar;
  Byte    *firstDot;
  Byte    theChar;
  UInt    reml;

  char    *cp;

  fileName._BufAndSize(buf, size);

  // Invalid if empty or too long...
  if (size <= 0 || size >= PATH_MAX) goto NOPE;

  // Validate the characters used in the name...
  //   (and find the first '.', or end of file)
  firstDot = NULL;
  curChar  = buf;
  reml = size;
  while (reml > 0) {
    // Get the character
    theChar = *curChar++;
    reml--;
    // If it's less than 0x20, it's bad
    if (theChar < 0x20) goto NOPE;
    // If it's in the list of bad chars, it's bad
    for (cp=(char *)LinuxReservedFileChars;*cp;cp++)
      if (*cp == theChar) goto NOPE;
    // If it's a '.' and we don't have the first one yet, save where it is
    if (theChar == '.' && IsNull(firstDot))
      firstDot = curChar-1;
  }
  if (IsNull(firstDot))
    firstDot = buf + size - 1;

  // Validate last character
  theChar = buf[size-1];
  if (theChar == ' ' || theChar == '.') goto NOPE;

  // If everything before the first '.' or end of string is a reserved
  // win32 device name, it's not valid
  if ( !IsNull(LinuxGetDeviceByName(buf, firstDot-buf)) ) goto NOPE;

  // *** It seems to qualify as a "valid file name" according to Microsoft
  answer = true;
  
  // Handle errors
  while (false) {
    NOPE: answer = false; break;
  }

  return answer;

}

Boolean LinuxIsValidPathName(String const &fullpath) {

  Boolean answer;
  Status  status;
  Strings parts;
  Int     numparts;
  Int     devno;
  Char    achar;
  Int     partno;
  String  part;
  UInt    valStart;         // Part validation starting index

  // If empty, it's not valid
  if (fullpath.Length() <= 0) goto NOPE;

  // Split it into consitituent parts
  status = LinuxPathSplit(fullpath,parts);
  if (status) goto NOPE;
  numparts = parts.Length();
  if (numparts <= 0) goto NOPE;

  // If it's a network path...
  if (numparts > 2 && parts.Get(0).Length() == 0 && parts.Get(1).Length() == 0) {

    valStart  = 2;

  }

  // It's not a network path...
  else {  // ...It's a "local" path

    valStart  = 1;

    // Validate the first part.  This must be either a drive letter followed by
    // a ':', or a reserved drive name.  In the first case, there has to be
    // more parts.  In the second case there can't be any more parts

    status = parts.Get(0,part);
    if (status) goto NOPE;

    // If two letters, it HAS TO be a drive number followed by a ':'...
    if (part.Length() == 2) {
      achar = part.Get(0);
      // Turn into uppercase
      if (achar >= 'a' && achar <= 'z')
        achar = achar - 'a' + 'A';
      // Must be a drive letter
      if (achar < 'A' || achar > 'Z') goto NOPE;
      // Must be followed by ':'
      achar = part.Get(1);
      if (achar != ':') goto NOPE;
      // It's a drive letter, must be followed by more parts
      if (numparts == 1) goto NOPE;
    }

    // Not two letters, make sure it's a reserved device name
    else {
      // Not two letters, see if it's a reserved device name
      devno = LinuxGetDeviceByName(part);
      if ( IsNull(devno) ) goto NOPE;
      // If there's other parts, it's not valid
      if (numparts > 1) goto NOPE;
      goto YEP;
    }

  } // ...It's a "local" path

  // First part's okay and it's a drive letter and we have more parts...
  // Make sure the remaining parts are valid file/directory names...
  //
  // Note: Ignore empty last part
  //
  for (partno=valStart; partno < numparts; partno++) {
    status = parts.Get(partno,part);
    if (status) goto NOPE;
    if (partno == numparts-1 && part.Length() == 0) break;
    if (!LinuxIsValidFileName(part)) goto NOPE;
  }

  // Handle errors...
  answer = true;
  while (false) {
    NOPE: answer = false; break;
    YEP:  answer = true;  break;
  }

  return answer;
  
}

Status LinuxCreateDirectory(String const &dir) {

  return Error::NotImplemented;

}

//----------------------------------------------------------------------
//
// Various overall Process specific functions
//
//----------------------------------------------------------------------

Status StartUp() {

  LinuxOperatingSystem  *theOS;

  // Allocate everything
  theOS        = new LinuxOperatingSystem();
  /*theConsole   =  new LinuxConsole(); */
  /*thePlatform  =*/ new LinuxPlatform();
  /*theProcess   =*/ new LinuxProcess();
  /*theProcessor =*/ new LinuxProcessor();
  /*theNetwork   =*/ new LinuxNetwork();
  
  TheBefe->TheGlobals.longs.StartTime = theOS->GetCurrentTime();  

  return Error::None;

}

void ShutDown() {

  if (TheBefe) { // If TheBefe...
    
    if (TheBefe->TheNetwork)  {
      delete TheBefe->TheNetwork;
      TheBefe->TheNetwork = NULL;
    }
    if (TheBefe->TheProcessor)  {
      delete TheBefe->TheProcessor;
      TheBefe->TheProcessor = NULL;
    }
    if (TheBefe->TheProcess)  {
      delete TheBefe->TheProcess;
      TheBefe->TheProcess = NULL;
    }
    if (TheBefe->TheOS) {
      delete TheBefe->TheOS;
      TheBefe->TheOS = NULL;
    }
    if (TheBefe->ThePlatform) {
      delete TheBefe->ThePlatform;
      TheBefe->ThePlatform = NULL;
    }
  
  } // ...If TheBefe  

}

void Exit(Status status) {
  ShutDown();
  exit(status);
}

//----------------------------------------------------------------------
//
// Various Memory management functions
//
//----------------------------------------------------------------------

static UInt mallocCounter = 0;
static UInt freeCounter   = 0;

void ValidateHeap() {
}

Byte *Malloc(UInt size) {

  Byte *bytes;
  Byte *tbytes;

  bytes = (Byte *)malloc(size);

  // Fill it with UInt8NaN...
  if (bytes && size) {
    tbytes = bytes;
    while (size) {*tbytes++ = UInt8NaN; size--;}
  }

  // For debugging...
  mallocCounter++;

  return bytes;
  
}

void Free(Byte *theMem) {

  // Free the memory...
  if (theMem) {
   free(theMem);
  }

  // For debugging...
  freeCounter++;

}

Byte *Memcpy(Byte *dst, Byte *src, Int len) {

  Byte *result;

  result = dst;

  if (!IsNull(dst) && !IsNull(src) && len > 0 && dst != src) {

    // If overlap, copy in reverse...
    if (dst >= src && dst < (src+len)) {
      dst += len-1;
      src += len-1;
      while (len > 0) {*dst-- = *src--; len--;};
    }
    else
      while (len > 0) {*dst++ = *src++; len--;};

  }

  return result;

}

Byte *Memchr(Byte *buf, Byte byte, Int len) {

  Byte *result;

  if (!IsNull(buf) && len > 0) {
    while (*buf++ != byte && --len > 0) {};
    if (len)
      result = buf-1;
    else
      result = NULL;
  }
  else
    result = NULL;

  return result;

}

//----------------------------------------------------------------------
//
// Various overall string functions (<cstring> or <string.h>)
//
// NOTE: These need to be rewitten in assembler
//
//----------------------------------------------------------------------

UInt Strlen(const void *str) {

  UInt  len;
  Byte *bstr;

  bstr = (Byte *)str;

  len = 0;
  if (bstr) {
    while (*bstr++)
      len++;
  }

  return len;
}

Int Strcmp(const void *s1, const void *s2) {

  Int   result;
  Byte *pb1;
  Byte *pb2;

  pb1 = (Byte *)s1;
  pb2 = (Byte *)s2;
  if (!pb1) goto LESS;
  if (!pb2) goto GREATER;
  do {
    if (*pb1 < *pb2) goto LESS;
    if (*pb1 > *pb2) goto GREATER;
    if (*pb1 == 0x00) break;
    ++pb1;
    ++pb2;
  } while (true);
  result = 0;
  goto DONE;

LESS:    result = -1; goto DONE;
GREATER: result = 1; goto DONE;

DONE:

  return result;

}

Int Strcmpi(const void *s1, const void *s2) {

  Int   result;
  Byte  b1;
  Byte  b2;
  Byte *pb1;
  Byte *pb2;

  pb1 = (Byte *)s1;
  pb2 = (Byte *)s2;
  if (!pb1) goto LESS;
  if (!pb2) goto GREATER;
  do {
    b1 = *pb1;
    b2 = *pb2;
    if (b1 >= 'A' && b1 <= 'Z') b1 = b1 - 'A' + 'a';
    if (b2 >= 'A' && b2 <= 'Z') b2 = b2 - 'A' + 'a';
    if (b1 < b2) goto LESS;
    if (b1 > b2) goto GREATER;
    if (b1 == 0x00) break;
    ++pb1;
    ++pb2;
  } while (true);
  result = 0;
  goto DONE;

LESS:    result = -1; goto DONE;
GREATER: result = 1; goto DONE;

DONE:

  return result;

}

Int Strncmp(const void *s1, const void *s2, UInt len) {

  Int   result;
  Byte *pb1;
  Byte *pb2;

  pb1 = (Byte *)s1;
  pb2 = (Byte *)s2;
  if (!pb1) goto LESS;
  if (!pb2) goto GREATER;
  while (len) {
    if (*pb1 < *pb2) goto LESS;
    if (*pb1 > *pb2) goto GREATER;
    ++pb1;
    ++pb2;
    --len;
  };
  result = 0;
  goto DONE;

LESS:    result = -1; goto DONE;
GREATER: result = 1; goto DONE;

DONE:

  return result;

}

void Strcpy(void *dst, const void *src) {

  Byte *bdst;
  Byte *bsrc;

  bdst = (Byte *)dst;
  bsrc = (Byte *)src;

  if (bdst) {
    while (*bsrc) { *bdst++ = *bsrc++; }
    *bdst = 0;
  }

}

void Strcat(void *dst, const void* src) {
  
  Byte *bdst;
  Byte *bsrc;
  
  bdst = (Byte *)dst;
  bsrc = (Byte *)src;
  
  if (bdst) {
    bdst += Strlen(bdst);
    Strcpy(bdst,bsrc);
  }

}

void *Strchr(const void *s1,UInt chr) {

  UInt8 *bs1;

  bs1 = (UInt8 *)s1;
  if (bs1 && chr > 0 && chr < 0xff) {
    while (*bs1 != 0x00 && *bs1 != chr) bs1++;
    if (*bs1 == chr)
      return bs1;
  }

  return NULL;

}

void *Strstr(void *s1, const void *s2) {

  Int len1;
  Int len2;
  Byte *bs1;
  Byte *bs2;

  bs1 = (Byte *)s1;
  bs2 = (Byte *)s2;
  len2 = Strlen(bs2);
  len1 = Strlen(bs1) - len2;

  while (len1) {
    if (Strncmp(bs1,bs2,len2) == 0) return bs1;
    bs1++;
  }

  return NULL;
}

//----------------------------------------------------------------------
//
// Various overall integer functions (<cstdlib> or <stdlib.h>)
//
//----------------------------------------------------------------------

void Itoa(Int num, Byte *buf, Int base) {

  String tString;
  UInt   usedLen;

  if (base == 10)
    tString = ToString(num);
  else if (base == 16)
    tString = ToHexString(num);
  else
    tString = "???";
  tString.ToBytes(buf, 10, usedLen);
  buf[usedLen] = 0x00;
  
}

Int Atoi(Byte *buf, UInt bufLen) {
  
  Status status;
  UInt   uInt;
  UInt   numDigits;
  UInt   prevUInt;  
  Int    sign;
  char  *cp;
  char   c;
  
  sign      = 0;
  uInt      = 0;
  numDigits = 0;
  
  cp = (char *)buf;
  if (!cp) goto INVALIDPARAMETER;

  // Skip leading whitespace
  while (bufLen && (*cp == ' ' || *cp == '\t')) {cp++; bufLen--;}

  // Turn into number  
  while (bufLen) {
    c = *cp++;
    bufLen--;
    // Handle sign...
    if (c == '+' || c == '-') {
      if (numDigits || sign) goto BADDIGIT;
      if (c == '+')
        sign = 1;
      else
        sign = 0;
    }
    // Handle digit...
    else if (c >= '0' && c <= '9') {
      prevUInt = uInt;
      uInt = uInt * 10 + (c-'0');
      if (uInt < prevUInt) goto OVERFLOW;
      numDigits++;
    }
    // Bad digit
    else
      goto BADDIGIT;
  }
  
  if (sign == 0)
    sign = 1;
  if (uInt >= (UInt)NaN) goto OVERFLOW;
  sign = sign * uInt;
  
  // Handle errors
  status = Error::None;
  while (false) {
    BADDIGIT:         status = Error::InvalidNumberDigit; break;
    OVERFLOW:         status = Error::IntegerOverflow;    break;
    INVALIDPARAMETER: status = Error::InvalidParameter; break;
  }
  
  if (status)
    sign = 0;
    
  return sign;
  
}

Int Atoi(Byte *buf) {
  return Atoi(buf,(buf)?Strlen(buf):0);
}

void Srand(Int seed) {
  srand(seed);
}

Int Rand() {
  return rand();
}

} // ...Namespace BEFE
