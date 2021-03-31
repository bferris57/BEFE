//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: Utf8.h - UTF-8 string management
//----------------------------------------------------------------------
//
// This class is responsible for performing operations on, and
// giving information about UTF-8 character strings.
//
// This header declares the UTF-8 encoding/decoding functions.  The
// intent here is to provide a means of managing in-memory strings
// with embedded UTF-8 in them.
//
// The implementation provides strict UTF-8 encoding/decoding.  It does
// NOT handle CESU-8 or UTF-16 encoding.  From our point of view, that
// is "old stuff" and is intentionally avoided.
//
// Although we DO decode Modified UTF-8 (without the shortest encodings)
// but when encoding, we'll always produce proper shortest encodings.
// This means that 0x00 is not encoded as 0xC0 0x80 by us.
//
// We also don't do anything special about those crazy BOM's Microsoft
// sometimes puts at the front of text file.  That's up to the caller
// to avoid and discard these characters.
//
// Throughout this code, we presume that an entirely encoded UTF-8
// fragment is in the memory buffer.  This shouldn't cause too many
// problems but just BE AWARE that, when doing character counting
// and character indexing into the strings, invalid partial UTF-8
// fragments are ignored so the counts or positioning may be a bit
// off.
///
// Note: About the string indexes... We index characters in the
//       string from the start with zero indexing.  But, like python,
//       we also accept negative indexing indicating
//       "start from the end".
//
//       This also lets us start at a given arbitrairy Byte/Char in the
//       string and walk forwards (zero or positive relative indexes)
//       or backwards (negative relative indexes).
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef UTF8_H // UTF8_H...
#define UTF8_H

#include "Primitives.h"
#include "Sparse.h"

namespace BEFE // Namespace BEFE...
{

class UTF8 { // Class UTF8...

  //
  // Public literal values
  //

  public: enum : UInt {
     MinChar   = 0
    ,MaxChar   = 0x10ffff
    ,BadChar   = UNaN
    ,MinLength = 1
    ,MaxLength = 4
  };

  //
  // UTF8 Class functions
  //

  public: static UInt    GetCharSize(Byte byte);
  public: static UInt    GetByteSize(Char thechar);
  public: static Char    GetChar(Byte *buf, UInt bufl, Int index);
  public: static Char    GetChar(Byte *buf, UInt bufl, Int index, Int curpos, Int &newpos);
  public: static Status  GetCharPos(Byte *buf, UInt bufLen, Int relIndex, Int curPos, Int &foundPos, Char &theChar);
  public: static Status  EncodeChar(Char theChar, Byte *buf, UInt &charLen);
  public: static Status  DecodeChar(Byte *buf, Char &theChar, UInt &charLen);
  public: static Status  Count(Byte *buf, UInt bufSize, UInt &byteCount, UInt &charCount);
  
  // Unicode Folding flags
  public: static Boolean IsUnicodeFolding();
  public: static Boolean IsTurkishFolding();
  public: static Status  UnicodeFoldingOn();
  public: static Status  UnicodeFoldingOff();
  public: static Status  TurkishFoldingOn();
  public: static Status  TurkishFoldingOff();
  
  // Externalising Unicode Case Folding (including Turkish)
  public: static Status  LoadFolding();
  public: static Status  LoadFolding(String const &fileName);
  public: static Status  UnloadFolding();
  
  // Handy Unicode Case Folding
  public: static Char    Fold(Char in);
  public: static Char    Unfold(Char in);
  
  // Base Unicode Case Folding
  public: static Status  Fold(String const &in, String &out);
  public: static Status  FoldSimple(Char in, Char &out);
  public: static Status  FoldSimple(String const &in, String &out);
  public: static Status  FoldFull(Char in, String &out);
  public: static Status  FoldFull(String const &in, String &out);
  
  // Base Unicode Case Folding
  public: static Status  Unfold(String const &in, String &out);
  public: static Status  UnfoldSimple(Char in, Char &out);
  public: static Status  UnfoldSimple(String const &in, String &out);
  public: static Status  UnfoldFull(Char in, String &out);
  public: static Status  UnfoldFull(String const &in, String &out);
  
  // Miscellaneous
  public: static Status  ReverseChars(Byte *buf, UInt size);
  
  //
  // Protected class members
  //
  
  protected: static String      foldFileName;
  protected: static SparseChars simpleFold;
  protected: static SparseChars simpleUnfold;
  protected: static SparseChars fullFold;
  
}; // ...Class UTF8

} // ...Namespace BEFE

#endif // ...UTF8_H
