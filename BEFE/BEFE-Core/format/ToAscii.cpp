//----------------------------------------------------------------------
// File: ToAscii.cpp - Implementaion of the ToAscii functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "ToAscii.h"

namespace BEFE { // Namespace BEFE...

void ToAscii_MidEllipsis(Byte *buf, size_t len) {

  UInt bufLen;
  UInt firstLen;
  UInt remLen;
  
  bufLen = Strlen(buf);
  if (bufLen > len) {
    if (len < 3) {
    }
    else {
      firstLen = (len-3)/2;
      buf[firstLen++]   = '.';
      buf[firstLen++] = '.';
      buf[firstLen++] = '.';
      remLen = len - firstLen;
      Memmove(buf+firstLen, buf + bufLen - remLen, remLen+1);
    }
  }
  
}

void ToAscii_LeftPad(Byte *buf, size_t len, Byte *outBuf) {

  UInt bufLen;
  UInt diff;
  
  bufLen = Strlen(buf);
  if (bufLen <= len) {
    diff = len - bufLen;
    while (diff) {
      *outBuf++ = ' ';
      diff--;
    }
    Memmove(outBuf, buf, bufLen+1);
  }
  else
    while (len) {*outBuf++ = '#'; len--;}
}  
  
//----------------------------------------------------------------------
//
// Int -> String...
//

void ToAscii(Int16 num, Byte *buf) {
  return ToAscii((Int64)num, buf);
}

void ToAscii(Int16 num, size_t len, Byte *buf) {

  Byte ourBuf[ToAscii_MaxNumLen];
  
  if (!buf) goto IGNORE;
  
  ToAscii((Int64)num, ourBuf);
  if (Strlen(ourBuf) > len)
    ToAscii_MidEllipsis(ourBuf, len);
  else
    ToAscii_LeftPad(ourBuf, len, buf);
    
  while (false) {
    IGNORE: break;
  }
  
  return;

}

void ToAscii(UInt16 num, Byte *buf) {

  ToAscii((UInt64)num,buf);

  return;

}

void ToAscii(UInt16 num, size_t len, Byte *buf) {

  Byte ourBuf[ToAscii_MaxNumLen];

  if (!buf) goto IGNORE;
  
  ToAscii((Int64)num, ourBuf);
  if (Strlen(ourBuf) > len)
    ToAscii_MidEllipsis(ourBuf, len);
  else
    ToAscii_LeftPad(ourBuf, len, buf);
    
  while (false) {
    IGNORE: break;
  }
  
  return;

}

void ToAscii(Int32 num, Byte *buf) {
  
  ToAscii((Int64)num, buf);
  
  return;

}

void ToAscii(Int32 num, size_t len, Byte *buf) {

  Byte ourBuf[ToAscii_MaxNumLen];

  if (!buf) goto IGNORE;
  
  ToAscii((Int64)num, ourBuf);
  if (Strlen(ourBuf) > len)
    ToAscii_MidEllipsis(ourBuf, len);
  else
    ToAscii_LeftPad(ourBuf, len, buf);
    
  while (false) {
    IGNORE: break;
  }
  
  return;

}

void ToAscii(UInt32 num, Byte *buf) {

  ToAscii((UInt64)num, buf);
  
  return;

}

void ToAscii(UInt32 num, size_t len, Byte *buf) {

  Byte ourBuf[ToAscii_MaxNumLen];

  if (!buf) goto IGNORE;
  
  ToAscii((Int64)num, ourBuf);
  if (Strlen(ourBuf) > len)
    ToAscii_MidEllipsis(ourBuf, len);
  else
    ToAscii_LeftPad(ourBuf, len, buf);
    
  while (false) {
    IGNORE: break;
  }
  
  return;

}

void ToAscii(UInt64 num, size_t len, Byte *buf) {

  Byte ourBuf[ToAscii_MaxNumLen];

  if (!buf) goto IGNORE;
  
  ToAscii(num, ourBuf);
  if (Strlen(ourBuf) > len)
    ToAscii_MidEllipsis(ourBuf, len);
  else
    ToAscii_LeftPad(ourBuf, len, buf);
    
  while (false) {
    IGNORE: break;
  }
  
  return;

}

void ToAscii(Int64 num, Byte *buf) {

  Byte   *bp;
  Boolean hasSign;
  
  if (num == LongNaN) // NaN...

    Strcpy(buf, "NaN");

  else { // Zero or Natural Number...
    
    // Handle sign
    if (num < 0) {
      num = -num;
      hasSign = true;
    }
    else
      hasSign = false;

    // Do it...
    bp = buf;
    do {
      *bp++ = num%10 + '0';
      num /= 10;
    } while (num);
    
    // Output sign if negative
    if (hasSign)
      *bp++ = '-';
      
    // Reverse the string's contents
    MemoryReverseBytes(buf,bp-buf);
    *bp = 0x00;
    
  } // ...Zero or Natural Number

  return;
  
}

void ToAscii(UInt64 num, Byte *buf) {

  Byte   *bp;

  if (num == UInt64NaN) // NaN...
  
    Strcpy(buf, "UNaN");
  
  else { // Zero or Natural Number...

    // Do it...
    bp = buf;
    do {
      *bp++ = num%10 + '0';
      num /= 10;
    } while (num);
    *bp = 0x00;

    // Reverse the string's contents
    MemoryReverseBytes(buf,Strlen(buf));

  }  // ...Zero or Natural Number

  return;

}

void ToAscii(Int64 num, size_t len, Byte *buf) {

  Byte ourBuf[ToAscii_MaxNumLen];

  if (!buf) goto IGNORE;
  
  ToAscii(num, ourBuf);
  if (Strlen(ourBuf) > len)
    ToAscii_MidEllipsis(ourBuf, len);
  else
    ToAscii_LeftPad(ourBuf, len, buf);
    
  while (false) {
    IGNORE: break;
  }
  
  return;

}

//----------------------------------------------------------------------
//
// Int -> HexAscii...
//

void ToHexAscii(UInt32 num, Byte *buf) {

  Byte  ourBuf[8];
  Byte *bp;
  UInt  movLen;
  
  if (!buf) goto DONE;
  
  // If UNaN...
  if ( BEFE::IsNull(num) ) {
    Strcpy(buf,"UNaN");
    goto DONE;
  }
  
  // Convert to hex...
  bp   = ourBuf + sizeof(ourBuf) - 1;
  do {
    *bp = num & 0x0f;
    if (*bp < 10)
      *bp-- += '0';
    else
	  *bp-- += ('A'-10);
    num >>= 4;
  } while (num);

  // Give it to them...
  movLen = sizeof(ourBuf) - (bp-ourBuf) - 1;
  Memmove(buf, bp+1, movLen);
  buf += movLen;
  *buf = 0x00;

  while (false) {
    DONE: break;
  }
  
  return;
  
}

void ToHexAscii(UInt32 num, size_t len, Byte *buf) {

  Byte  ourBuf[8];
  Byte *bp;
  UInt  movLen;
  
  if (!buf) goto DONE;
  
  // If UNaN...
  if ( IsNull(num) ) {
    Memmove(ourBuf,(Byte *)"....UNaN",8);
    goto DONE;
  }
  
  // Else, convert to hex...
  else {
    bp   = ourBuf + sizeof(ourBuf) - 1;    
    do {
      *bp = num & 0x0f;
      if (*bp < 10)
        *bp-- += '0';
      else
        *bp-- += ('A'-10);
      num >>= 4;
    } while (num);
  }

  // If expected length is greater than 8, pad...
  while (len > 8) { *buf++ = '.'; len--; }
  
  // If truncated...
  movLen = sizeof(ourBuf) - (bp-ourBuf) - 1;
  if (len < movLen)
    while (len) { *buf++ = '#'; len--; }
    
  // Else, give it to them...
  else {
    while (len > movLen) { *buf++ = '0'; len--; }
    Memmove(buf, bp+1, movLen);
    buf += movLen;
  }
  
  *buf = 0x00;
  
  while (false) {
    DONE: break;
  }
  
  return;
  
}

} // ...Namespace BEFE
