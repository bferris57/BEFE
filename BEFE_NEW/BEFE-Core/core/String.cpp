//----------------------------------------------------------------------
// File: String.cpp - String Class base methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "UTF8.h"
#include "Buffer.h"
#include "SBuffer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class methods
//
// Method: CountChars - Count Chars in string, counting UTF-8 as one
//
// Usage:  count = CountChars(buf, len);
//
// This function DOES NOT validate the UTF-8, it only counts the
// characters.
//

UInt String::CountChars(Byte *buf,UInt size) {

  UInt charCount;
  UInt byteCount;

  UTF8::Count(buf,size,byteCount,charCount);

  return charCount;

}

//----------------------------------------------------------------------
//
// BEFE Lifecycle Methods...
//
// Methods: StartUp       - Initialise uninitialised instance
//          ShutDown      - Clear initialised instance, freeing allocated data
//          Reset         - Reset the String
//          SetEmpty      - Empty the String's contents
//          MoveFrom      - Move from another, possibly consumable, String
//          CopyFrom      - Copy from another, possibly consumable, String
//          Consumable    - "This String is Consumable"
//          NotConsumable - "This String is NOT Consumable"
//          Length        - Return number of Characters in String
//          Size          - Return number of Bytes in String
//

Status String::StartUp() {

  isMutable    = true;
  isByte       = false;
  isChar       = false;
  isSensitive  = true;
  type         = Null;
  mult         = 1;
  constLen     = 0;
  data         = NULL;

  return Error::None;
}

Status String::ShutDown() {

  Status status;

  if (type == Buffer && !BEFE::IsNull(data))
    status = ((SBuffer *)&data)->ShutDown();
  else
    status = Error::None;
    
  return status;

}

Status String::Reset() {
  ShutDown();
  StartUp();
  return Error::None;
}

Status String::SetEmpty() {
  Status status;
  
  status = Reset();
  if (!status) {
    isMutable = true;
    isByte    = true;
    isChar    = false;
    type      = ASCII0;
    mult      = 1;
    constLen  = 0;
    data      = (Byte *)0;
  }

  return status;

}

Status String::MoveFrom(String const &that) {

  UInt *iThis;
  UInt *iThat;
  UInt  i;
  
  this->ShutDown();
  iThis = (UInt *)this;
  iThat = (UInt *)&that;
  for (i=sizeof(String)/sizeof(UInt); i; i--) *iThis++ = *iThat++;
  ((String *)&that)->StartUp();

  // Too see what REAL impact this has on memory allocations...
  //
  // Actually, had no impact in Unit Tests so we'll leave it
  // here for now since it's not hurting and could help
  NotConsumable();
  
  return Error::None;

}

Status String::CopyFrom(String const &that) {

  Status status;
  
  if (this == &that) goto FINISHED;
  
  if (that.IsConsumable()) {
    status = MoveFrom((String &)that);
    if (status) goto SOMEERROR;
  }
  else {
    status = Reset();
    if (status) goto SOMEERROR;
    status = _BuildFrom(that);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    FINISHED:  status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;

}

String &String::Consumable() const {
  if (type == Buffer && !BEFE::IsNull(data))
    ((SBuffer *)&data)->Consumable();
  return *((String *)this);
}

String &String::NotConsumable() const {
  if (type == Buffer && !BEFE::IsNull(data))
    ((SBuffer *)&data)->NotConsumable();
  return *((String *)this);
}

Boolean String::IsConsumable() const {
  if (type != Buffer || BEFE::IsNull(data))
    return false;
  return ((SBuffer *)&data)->IsConsumable();
}

UInt String::Length() const {

  UInt     charLength;
  UInt     byteLength;
  SBuffer *sBuf;
  Byte    *srcBuf;
  UInt     srcBufSize;

  charLength  = 0;
  srcBuf      = NULL;
  srcBufSize  = 0;
  switch (type) {
    case Null:
      break;
    case Const:
      srcBuf = (Byte *)data;
      if (BEFE::IsNull(srcBuf)) break;
      srcBufSize = Strlen((char *)data);
      break;
    case ConstLen:
      srcBuf     = (Byte *)data;
      srcBufSize = constLen;
      break;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      //srcBuf     = (Byte *)&data;
      //srcBufSize = type - ASCII0;
      charLength = constLen;
      break;
    case UtfInline:
      srcBuf     = (Byte *)&data;
      srcBufSize = constLen;
      break;
    case Buffer:
      sBuf = (SBuffer *)&data;
      if (BEFE::IsNull(sBuf)) break;
      srcBuf     = sBuf->GetBuf();
      srcBufSize = sBuf->GetUsedBytes();
      break;
    default:
      break;
  }

  // If we have a buffer and length, ask UTF8 to count it
  if (srcBufSize != 0 && !BEFE::IsNull(srcBuf))
    UTF8::Count(srcBuf,srcBufSize,byteLength,charLength);

  return charLength;

}

UInt String::Size() const {

  Int      size;
  SBuffer *sbuf;

  size = 0;
  switch (type) {
    case Null:
      break;
    case Const:
      if (BEFE::IsNull(data)) break;
      size = Strlen((char *)data);
      break;
    case ConstLen:
      if (BEFE::IsNull(data)) break;
      size = constLen;
      break;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      size = type - ASCII0;
      break;
    case UtfInline:
      size = constLen;
      break;
    case Buffer:
      sbuf = (SBuffer *)&data;
      if (BEFE::IsNull(sbuf)) break;
      size = sbuf->GetUsedBytes();
      break;
    default:
      break;
  }

  return size;

}

Boolean String::IsMutable() const {
  return isMutable;
}

Status String::Mutable() {
  
  Status status;
  Byte  *buf;
  UInt   byteCount;
  UInt   charCount;
  
  // If not mutable, make it mutable
  if (type < ASCII0 || type > UtfInline) {
    _BufByteAndCharCount(buf, byteCount, charCount);
    if ((type == Const || type == ConstLen) && byteCount <= 4 && isByte) { // Turn into ASCII0..ASCII4...
      MemoryCopyRaw((Byte *)data, (Byte *)&data, constLen);
      type = ASCII0+constLen;
    }
    else if ((type == Const || type == ConstLen) && charCount == 1 && isChar) { // Turn into UtfInline...
      MemoryCopyRaw((Byte *)data, (Byte *)&data, constLen);
      type = UtfInline;
    }
    else { // Need to turn it into a buffer...
      status = _MorphTo(Buffer);
      if (status) goto SOMEERROR;
    }
  }
  isMutable = true;
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }

  return status;

}

Status String::Immutable() {
  isMutable = false;
  return Error::None;
}

Status String::CompareEquals(String const &that, Boolean &answer) {
  answer = CompareEquals(that);
  return Error::None;
}

Status String::CompareRelative(String const &that, Int result) {
  result = Compare(that);
  return Error::None;
}

//----------------------------------------------------------------------
//
// Morphing Methods...
//

Bytes String::MorphToBytes() {

  Status  status;
  SBuffer *sbuf;
  Bytes   that;

  status = _MorphTo(Buffer,0);
  if (status)
    BEFE_WARN("String.MorphToBytes call to MorphTo(Buffer,0) failed");
  else {
    sbuf = (SBuffer *)&data;
    sbuf->Consumable();
    that.buffer = *sbuf;
    that.Consumable();
    StartUp();
  }

  return that;
}

//----------------------------------------------------------------------
//
// Mutability Methods...
//
// Methods: MakeBuffer   - Turn String into a Mutable Buffer type
//

Status String::MakeBuffer() {

  Status status;

  if (type != Buffer) {
    status = _MorphTo(Buffer,Size());
    if (status) goto SOMEERROR;
  }
  isMutable = true;
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Method: _Allocate - Compute string buffer and size in bytes
//
// Usage:  status = _Allocate(UInt numBytes);
//
// Where:  numBytes - IN:  Number of Bytes in Buffer
//
//         status   - OUT: Return Status
//
// This allocates a String of type Buffer and allocates numBytes for the
// buffer and sets UsedBytes to zero.
//
// NOTE: This method will fail if the String already has characters
//       in it.
//

Status String::_Allocate(UInt numBytes) {
  
  Status status;
  
  if (Size()) goto NOTEMPTY;
  if (numBytes) {
    status = Reset();
    if (status) goto SOMEERROR;
    status = ((SBuffer *)&data)->Allocate(numBytes);
    if (status) goto SOMEERROR;
    isMutable   = true;
    isByte      = false;
    isChar      = false;
    type        = Buffer;
    mult        = 1;
    constLen    = 0;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    NOTEMPTY:  status = Error::StringNotEmpty; break;
    SOMEERROR:                                 break;
  }
  
  return status;
  
}
  
//----------------------------------------------------------------------
//
// Method: _BufAndSize - Compute string buffer and size in bytes
//
// Usage:  _BufAndSize(buf,size);
//
// Where:  buf     - OUT:    Address of first byte
//         size    - OUT:    Size (in bytes, see below)
//
// If the type is Null, buf will be set to NULL and len will be set to zero.
//
// If the type is ASCII0..UtfInline, buf will be set to the data member
// of the String instance.
//
// NOTE: There shouldn't EVER be a case where type == UtfInline and it's
//       and ASCII character.  In fact, we check this and WARN if so!
//
// Otherwise, buf is updated to point to the first byte of the string
// and len is updated to contain the number of bytes in the string.
//

void String::_BufAndSize(Byte *&buf, UInt &size) const {

  SBuffer *sbuf;

  switch (type) {
    case Null:
      buf = NULL;
      size = 0;
      break;
    case Const:
      buf = data;
      if (!BEFE::IsNull(buf))
        size = Strlen((const char *)buf);
      else
        size = 0;
      break;
    case ConstLen:
      buf  = data;
      size = (UInt)constLen;
      break;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      buf  = (Byte *)&data;
      size = type-ASCII0;
      break;
    case UtfInline:
      buf  = (Byte *)&data;
      size = constLen;
      if (size == 1)
        BEFE_WARN("String.GetBufAndData: Type=UtfInline but character is ASCII");
      break;
    case Buffer:
      sbuf = (SBuffer *)&data;
      if (BEFE::IsNull(sbuf->pointer)) {
        buf  = NULL;
        size = 0;
        break;
      }
      buf  = sbuf->GetBuf();
      size = sbuf->GetUsedBytes();
      break;
    default:
      BEFE_WARN("String.GetBufAndSize: Unknown type");
      break;
  }

  return;

}

//----------------------------------------------------------------------
//
// Method: _BufByteAndCharCount - Compute string buffer and Byte/Char counts
//
// Usage:  _BufByteAndCharCount(buf,byteCount,charCount;
//
// Where:  buf       - OUT: Address of first byte in String Buffer
//         bytecount - Out: Number of Bytes in String Buffer
//         charCount - Out: Number of Chars (logical) in String Buffer
//
// If the type is Null, buf will be set to NULL and both counts will be
// set to zero.
//
// NOTE: This returns non-multiple sizes.  It is up to the
//       caller to do whatever with mult.  ;-)

Status String::_BufByteAndCharCount(Byte *&buf,UInt &byteCount, UInt &charCount) {
  Status status;
  
  // Initialise output
  byteCount = 0;
  charCount = 0;

  // First, it depends on type...
  switch (type) {
    case Null:
      isByte = false;
      isChar = false;
      goto OK;
    case Const:
      if (BEFE::IsNull(data)) goto OK;
      buf       = (Byte *)data;
      byteCount = Strlen(buf);
      break;
    case ConstLen:
      if (BEFE::IsNull(data)) goto OK;
      buf       = (Byte *)data;
      byteCount = constLen;
      break;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      isByte = true;
      isChar = false;
      buf       = (Byte *)&data;
      byteCount = type-ASCII0;
      charCount = byteCount;
      goto OK;
    case UtfInline:
      isByte = false;
      isChar = true;
      buf       = (Byte *)&data;
      byteCount = constLen;
      charCount = 1;
      goto OK;
    case Buffer:
      _BufAndSize(buf,byteCount);
      // If we already know, it's easy
      if (isByte && !isChar) {
        isByte = false;
        isChar = true;
        charCount = byteCount;
        goto OK;
      }
      // Nope, need to figure it out...
      break;
    default:
      goto INTERNALERROR;
  }

  // If no buffer, let's just say isByte=true...
  if (BEFE::IsNull(buf)) {
    isByte = true;
    isChar = false;
    goto OK;
  }

  UTF8::Count(buf,byteCount,byteCount,charCount);
  isByte = (byteCount == charCount);
  isChar = !isByte;

  // Handle errors
  status = Error::None;
  while (false) {
    INTERNALERROR: status = Error::Internal; break;
    OK:            status = Error::None;     break;
  }

  return status;
}

//----------------------------------------------------------------------
//
// Method: _LogToPhySpan - Normalise a Logical Char Span to a physical Byte Span
//
// Usage:  phySpan = _LogToPhySpan(logSpan)
//
//         if (phySpan.Length()) { // Do Something...
//           ...
//         } // ...Do Something
//
// Where:  logSpan   - Out: Logical Char Span
//         physpan   - Ret: Physical Byte Span
//
// Notes:  This method returns phySpan.idx1 == UNaN if the Span starts 
//         outside the valid String index space.
//

Span String::_LogToPhySpan(Span const &span) const {
  
  Status  status;
  Byte   *buf;
  UInt    byteCount;
  UInt    charCount;  
  Span    ourSpan;
  Span    retSpan;
  Char    theChar1;
  UInt    theChar1Len;
  Char    theChar2;
  UInt    theChar2Len;
  Boolean reverse;
  Boolean bothNegative;
  
  ((String *)this)->_BufByteAndCharCount(buf, byteCount, charCount);
  
  status = Error::None;
  
  while ( ( !BEFE::IsNull(span.idx1) ||  !BEFE::IsNull(span.idx2) ) && byteCount != 0 && charCount != 0) {
    
    // Get a copy of the span
    ourSpan = span;
    
    // Figure out if it's reverse or not...
    if ( !BEFE::IsNull(span.idx1) &&  !BEFE::IsNull(span.idx2) ) { // Close Ended...
      if ( (span.idx1 < 0 && span.idx2 < 0 && span.idx2 < span.idx1-1) ||
           (span.idx1 >= 0 && span.idx2 >= 0 && span.idx2 < span.idx1-1)
         )
        reverse = true;
      else
        reverse = false;
    } // ...Close Ended
    else if ( IsNaN(span.idx1) &&  IsNaN(span.idx2) ) // Both Open Ended...
      reverse = false;
    else { // One Open Ended...
      if ( !BEFE::IsNull(span.idx1) && !BEFE::IsNull(span.idx2) )
        reverse = true;
      else
        reverse = false;
    } // ...One Open Ended
    
    bothNegative = span.idx1 < 0 && !BEFE::IsNull(span.idx1) && span.idx2 < 0 && !BEFE::IsNull(span.idx2);
    
    // Normalise Span...
    if ( IsNaN(ourSpan.idx1) )
      ourSpan.idx1 = 0;
    if (ourSpan.idx1 < 0)
      ourSpan.idx1 += charCount;
    if ( IsNaN(ourSpan.idx2) )
      ourSpan.idx2 = (reverse)?-charCount-1:charCount;
    if (ourSpan.idx2 < 0) 
      ourSpan.idx2 += charCount;
    if (ourSpan.idx1 < 0  || ourSpan.idx1 >= (Int)charCount) break;
    if (ourSpan.idx2 < -1 || ourSpan.idx2 >  (Int)charCount) break;
    
    // Find position of each logical index
    if (byteCount == charCount) { // ASCII...
      retSpan = ourSpan;
    } // ...ASCII

    else { // UTF-8...

      status = UTF8::GetCharPos(buf, byteCount, ourSpan.idx1, 0, retSpan.idx1, theChar1);
      if (status) break;
      if ((UInt)ourSpan.idx2 == charCount)
        retSpan.idx2 = byteCount;
      else if (ourSpan.idx2 == -1)
        retSpan.idx2 = -1;
      else{
        status = UTF8::GetCharPos(buf, byteCount, ourSpan.idx2, 0, retSpan.idx2, theChar2);
        if (status) break;
      }
      
      // Adjust if reverse and both negative...
      if (reverse && !bothNegative) {

        theChar1Len = UTF8::GetByteSize(theChar1) - 1;
        theChar2Len = UTF8::GetByteSize(theChar2) - 1;
        retSpan.idx1 += theChar1Len;
        retSpan.idx2 += theChar2Len;

      } // ...Reverse
      
    } // ...UTF-8
    
    break;
 
 }
 
  if (status)
    retSpan.StartUp();
 
  return retSpan;
  
}

//----------------------------------------------------------------------
//
// Method: _LogToPhyRange - Normalise a Logical Char Range to a physical Byte Range
//
// Usage:  phyRange = _LogToPhyRange(logRange)
//
//         if (phyRange.Length()) { // Do Something...
//           ...
//         } // ...Do Something
//
// Where:  logRange - Out: Logical Char Range
//         phyRange - Ret: Physical Byte Range
//
// Notes:  This method returns phyRange.idx1 == UNaN if the Range starts 
//         outside the valid String index space.
//

Range String::_LogToPhyRange(Range const &range) const {

    Span   ourSpan;
    Range  ourRange;
    
    if ( !BEFE::IsNull(range.idx1) || !BEFE::IsNull(range.idx2) ) { // Something Specified...
    
      ourSpan.idx1 = range.idx1;
      ourSpan.idx2 = range.idx2;
      if ( !BEFE::IsNull(ourSpan.idx2) ) {
        if (ourSpan.idx2 == -1)
          ourSpan.idx2 = NaN;
        else
          ourSpan.idx2++;
      }
      ourSpan = _LogToPhySpan(ourSpan);
      ourRange.idx1 = ourSpan.idx1;
      ourRange.idx2 = ourSpan.idx2;
      if (ourRange.idx2 < 0) {
        if (ourRange.idx2 != -1)
          ourRange.idx2++;
      }
      else
        ourRange.idx2--;
      
      if ( ourRange.idx1 < 0 || ourRange.idx2 < 0) {
        ourRange.idx1 = NaN;
        ourRange.idx2 = NaN;
      }

    } // ...Something Specified
    
    return ourRange;
    
}

//----------------------------------------------------------------------
//
// Underlying SBuffer implementation...
//

UInt String::_GetAllocBytes() const {
  
  if (type == Buffer && !BEFE::IsNull(data))
    return ((SBuffer *)&data)->GetAllocBytes();
  return 0;
  
}

UInt String::_GetIncrementBytes() const {
  
  if (type == Buffer && !BEFE::IsNull(data))
    return ((SBuffer *)&data)->GetIncrementBytes();
  return 0;
  
}

UInt String::_GetUsedBytes() const {
  
  if (type == Buffer && !BEFE::IsNull(data))
    return ((SBuffer *)&data)->GetUsedBytes();
  return 0;
  
}

Status String::_SetUsedBytes(UInt newSize) {
  
  Status status;
  
  if (type != Buffer || BEFE::IsNull(data)) {
    status = MakeBuffer();
    if (status) goto SOMEERROR;
  }

  status = ((SBuffer *)&data)->SetUsedBytes(newSize);

  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

Byte *String::_GetBuf() const {
  
  Byte *theBuf;
  
  if (type == Buffer && !BEFE::IsNull(data))
    theBuf = ((SBuffer *)&data)->GetBuf();
  else
    BEFE::SetNull(theBuf);
    
  return theBuf;
  
}

//----------------------------------------------------------------------
//
// Low Level Morphing Methods...
//

Status String::_MorphTo(StringType newtype) {
  return _MorphTo(newtype,UNaN);
}

Status String::_MorphTo(StringType newType,UInt minSize) {

  Status   status;
  SBuffer *sBuf;
  Byte    *buf;
  UInt     bufLen;
  UInt     charCount;
  UInt     byteCount;
  Char     theChar;
  Byte     utfBuf[UTF8::MaxLength];
  Byte    *bp1;
  Byte    *bp2;
  
  switch (newType) {
    case Null:
      ShutDown();
      break;
    case Const:
      break;
    case ConstLen:
      break;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
    case UtfInline:
      if (type <= Null || type > Buffer)
        goto INTERNALERROR;
      sBuf = (SBuffer *)&data;
      if (type != Buffer || BEFE::IsNull(sBuf)) break;
      buf = sBuf->GetBuf();
      charCount = sBuf->Length();
      byteCount = sBuf->Size();
      if (charCount == byteCount) { // No UTF-8...
        if (byteCount > 4) break;
        bp1 = buf;
        bp2 = utfBuf;
        *bp2++ = *bp1++;
        *bp2++ = *bp1++;
        *bp2++ = *bp1++;
        *bp2++ = *bp1++;
        sBuf->SetEmpty();
        bp1 = utfBuf;
        bp2 = data;
        *bp2++ = *bp1++;
        *bp2++ = *bp1++;
        *bp2++ = *bp1++;
        *bp2++ = *bp1++;
        type     = ASCII0 + byteCount;
        constLen = byteCount;
      }
      else {                        // Has UTF-8...
        if (charCount != 1) break;
        theChar = sBuf->GetChar(0);
        sBuf->SetEmpty();
        status = UTF8::EncodeChar(theChar,utfBuf,charCount);
        if (status) goto SOMEERROR;
        *(Int *)&data = *(Int *)utfBuf;
        type     = UtfInline;
        constLen = charCount;
      }
      isMutable = true;
      break;
    case Buffer:
      if (type < Null || type > Buffer)
        goto INTERNALERROR;
      if (type == Buffer)
        break;
      sBuf = (SBuffer *)&data;
      if (type == Null) {
        sBuf->StartUp();
        status = sBuf->Allocate(minSize,UNaN);
        if (status) goto SOMEERROR;
        type     = Buffer;
        constLen = 0;
      }
      else if (type == Const || type == ConstLen) { // Convert from (const char *) to Buffer...
        if (BEFE::IsNull(data)) {
          ShutDown();
          break;
        }
        buf = (Byte *)data;
        if (type == Const)
          bufLen = Strlen((const char *)buf);
        else
          bufLen = constLen;
        sBuf->StartUp();
        status = sBuf->Allocate((minSize!=UNaN)?Max(bufLen,minSize):bufLen,UNaN);
        if (status) goto SOMEERROR;
        status = sBuf->AppendBytes(buf,bufLen);
        if (status) goto SOMEERROR;
        type     = Buffer;
        constLen = 0;
        NotConsumable();
      }
      else if (type >= ASCII0 && type <= ASCII4) {
        *(Int *)utfBuf = *(Int *)&data;
        sBuf = (SBuffer *)&data;
        sBuf->StartUp();
        status = sBuf->Allocate(UNaN,UNaN);
        if (status) goto SOMEERROR;
        status = sBuf->AppendBytes(utfBuf,constLen);
        if (status) goto SOMEERROR;
        type     = Buffer;
        constLen = 0;
      }
      else if (type >= UtfInline && type <= UtfInline) {
        *(Int *)utfBuf = *(Int *)&data;
        sBuf = (SBuffer *)&data;
        sBuf->StartUp();
        status = sBuf->Allocate(UNaN,UNaN);
        if (status) goto SOMEERROR;
        status = sBuf->AppendBytes(utfBuf,constLen);
        if (status) goto SOMEERROR;
        type     = Buffer;
        constLen = 0;
      }
      isMutable = true;
      break;
    default:
      goto INTERNALERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    INTERNALERROR: status = Error::Internal;
                   BEFE_WARN("String.MorphTo: INTERNAL ERROR");
                   break;
    SOMEERROR:     break;
  }

  return status;

}

} // ...Namespace BEFE
