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
// Copyright (©) 2004-2021, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "linux.h"
#include "ToAscii.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Internal (static) Functions...
//
//----------------------------------------------------------------------

// For some odd reason, this isn't in MinGW's winnt.h...
#define HEAP_CREATE_ENABLE_EXECUTE 0x00040000

HANDLE CurrentHeap() {

  //return GetProcessHeap();

  if (gProcessHeapHand == 0) {

    BOOL bResult;

    bResult = HeapSetInformation(NULL, (HEAP_INFORMATION_CLASS)1, NULL, 0);

    gProcessHeapHand = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE+HEAP_GENERATE_EXCEPTIONS, 1024*1024, 1024*1024*1024);
    //gProcessHeapHand = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 1024*1024, 30*1024*1024);
    if (gProcessHeapHand == NULL) {
      Cout << "CurrentHeap: Failed to create a new heap with, GetLastError() = " << (Int)GetLastError() << '\n';
      return NULL;
    }

    bResult = HeapSetInformation(gProcessHeapHand, (HEAP_INFORMATION_CLASS)1, NULL, 0);
    if (!bResult) {
      Cout << "CurrentHeap: Failed to enable heap terminate-on-corruption.\n"
           << "             LastError = " << (Int)GetLastError << '\n';
      return (HANDLE)0;
    }

  }

  return gProcessHeapHand;

}

Status DisplayHeapInfo() {

  BOOL  bResult;
  ULONG heapInformation;

  bResult = HeapQueryInformation(CurrentHeap(),
                                 HeapCompatibilityInformation,
                                 &heapInformation,
                                 sizeof(heapInformation),
                                 NULL);
  if (bResult == FALSE) {
    Cout << "DisplayHeapInfo: Failed to retrieve heap features, GetLastError() = "
         << (Int)GetLastError() << '\n';
      return Error::Internal;
  }

  Cout << "DisplayHeapInfo: HeapCompatibilityInformation is " << (Int)heapInformation << '\n';

  return Error::None;

}

//----------------------------------------------------------------------
//
// Various overall Process specific functions
//
//----------------------------------------------------------------------

Status StartUp() {

  LinuxOperatingSystem  *theOS;

  // Get current process Handle if we don't have it already
  if (!gProcessHand)
    gProcessHand = GetCurrentProcess();

  // Allocate Process heap if we don't have it already
  CurrentHeap();

  // Allocate everything
  theOS        = new LinuxOperatingSystem();
  /*theConsole   =*/ new Win32Console();
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
  ExitProcess(status);
}

//----------------------------------------------------------------------
//
// Various Memory management functions
//
//----------------------------------------------------------------------

static UInt mallocCounter = 0;
static UInt freeCounter   = 0;

void ValidateHeap() {

  Byte    ourBuf[9];
  UInt    fg;
  Boolean ok;

  ok = HeapValidate(CurrentHeap(), 0, NULL);
  if (!ok) {
    if (Cout.GetCursor().x != 0) Cout << '\n';
    fg = Cout.SetForegroundColour(Console::Red);      
    ToHexAscii(mallocCounter, ourBuf);
    Cout << "Malloc: mallocCounter = 0x" << (char const *)ourBuf;
    ToHexAscii(freeCounter, ourBuf);
    Cout << ", freeCounter = 0x" << (char const *)ourBuf;
    Cout << " -- Heap Invalid!!!\n";
    if (gMemoryValidateAbort) {
      Cout << "***ABORTING***\n";
      Cout.SetForegroundColour(fg);
      TerminateProcess(gProcessHand,1);
    }
    Cout.SetForegroundColour(fg);
  }

}

Byte *Malloc(UInt size) {

  Byte    *bytes;
  Byte    *tbytes;

  bytes     = NULL;

  // Validate Heap if we're supposed to...
  mallocCounter++;
  if (gMemoryValidate && CurrentHeap())
    ValidateHeap();

  // Allocate the memory
  bytes = (Byte *)HeapAlloc(CurrentHeap(), HEAP_GENERATE_EXCEPTIONS, size);
    
  // Fill it with UInt8NaN...
  if (bytes && size) {
    tbytes = bytes;
    while (size) {*tbytes++ = UInt8NaN; size--;}
  }

  return bytes;
  
}

void Free(Byte *theMem) {

  Boolean ok;

  if (gMemoryValidate && CurrentHeap())
    ValidateHeap();

  if (!IsNull(theMem) && CurrentHeap()) {
    
    // Validate Heap if we're supposed to...
    freeCounter++;
    if (gMemoryValidate) {

      Byte ourBuf[9];
      UInt fg;
    
      ok = HeapValidate(CurrentHeap(), 0, NULL);
      if (!ok) {
        if (Cout.GetCursor().x != 0) Cout << '\n';
        fg = Cout.SetForegroundColour(Console::Red);      
        ToHexAscii(mallocCounter, ourBuf);
        Cout << "Free:   mallocCounter = 0x" << (char const *)ourBuf;
        ToHexAscii(freeCounter, ourBuf);
        Cout << ", freeCounter = 0x" << (char const *)ourBuf;
        Cout << " -- Heap Invalid!!!\n";
        if (gMemoryValidateAbort) {
          Cout << "***ABORTING***\n";
          Cout.SetForegroundColour(fg);
          TerminateProcess(gProcessHand,1);
        }
        Cout.SetForegroundColour(fg);
      }
    }  
    
    // Free the memory...
    HeapFree(CurrentHeap(), 0, theMem);
    
  }
}

Byte *Memmove(Byte *dst, Byte *src, Int len) {

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
