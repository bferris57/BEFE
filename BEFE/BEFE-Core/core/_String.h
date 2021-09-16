//!befe-publish inc
//----------------------------------------------------------------------
// File: String.h - Declarations for the String class
//----------------------------------------------------------------------
//
// This class is a BEFE foundation class.  As such, it appears
// throughout the product (almost !!!EVERYWHERE!!!).
//
// *** See technical documentation for a more detailed description ***
//
// Note: I know you're familiar with C++ and its strangenesses BUT just
//       to reiterate an obvious point even if you're a "guru", and
//       we're only mentioning it here because we keep forgetting this
//       subtle difference ourselves...
//
//             IN PLACE Methods: Return String &
//         OUT OF PLACE Methods: Return String
//
//
//       Further note, we've intentionally bent the "C++ rules" having
//       to do with the meaning of 'const'.  In String, and throughout
//       BEFE, when we use 'const' it is to get the compiler to 
//       generate more efficient code NOT because we promise to honour
//       some weird sanctity of the instance's data.
//
//       Granted, this means we have to do some in-place type casting
//       at various places in the implementation but, as far as we're
//       concerned, it's worth it to get the speed improvement as the
//       end result for all that bother.
//
//       So, ***BEWARE***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef _BEFE_STRING_H // _BEFE_STRING_H...
#define _BEFE_STRING_H

#include "Primitives.h"
#include "Array.h"
#include "Bytes.h"
#include "Strings.h"
#include "Slice.h"

namespace BEFE { // Namespace BEFE...

// Forward/External class declarations
class testString;
class Persist;
class Strings;

class String { // String Class...

  // Friend classes
  friend class Strings;
  friend class testString;
  friend class Stream;
  friend class Bytes;
  friend class Persist;
  friend class Dictionary;
  
//
// Public type declarations
//

  // Internal string implementation type
  public: enum StringType {
     Null         // 0: Null
    ,Const        // 1: C++ (const char *, NUL terminated)
    ,ConstLen     // 2: C++ (const char *, Length terminated)
    ,ASCII0       // 3: ASCII 0 characters (Empty)
    ,ASCII1       // 4: ASCII 1 character
    ,ASCII2       // 5: ASCII 2 characters
    ,ASCII3       // 6: ASCII 3 characters
    ,ASCII4       // 7: ASCII 4 characters
    ,UtfInline    // 8: 1 Unicode character
    ,Buffer       // 9: SBuffer
  };

  //
  // Public class methods
  //

  // Implemented in String.cpp
  public: static UInt CountChars(Byte *buf,UInt size);

  //
  // Public instance methods
  //

  // C++ Lifecycle
  //   Implemented in StringConstruct.cpp
  public: String();
  public: ~String();
  public: String(Byte *buf, UInt size);         // Uses caller's buffer
  public: String(String const &that);
  public: String(Char that);
  public: String(char   const *that);
  public: String(char   const *that, UInt len);
  public: String(char         *that);
  public: String(char val);
  public: String(int val);

  // BEFE Lifecycle
  //   Implemented in String.cpp
  public: Status  StartUp();                    // Initialise uninitialised instance
  public: Status  ShutDown();                   // Finish initialised instance
  public: Status  Reset();                      // Reset (Nullify) the String
  public: Boolean IsNull() const { return type==Null/* || (type==Buffer && data==NULL) || type==ASCII0*/; };
  public: Status  SetNull()      { return Reset();         };
  public: Boolean IsEmpty() const { return Length() == 0; };
  public: Status  SetEmpty();
  public: Status  MoveFrom(String const &that); // ◄── Move from another, possibly consumable, String
  public: Status  CopyFrom(String const &that); // ◄── Copy from another, possibly consumable, String
  //public: Status     &NewIterator(UInt iterType, Iterator *&newIter); // ◄── Not implemented yet
  public: String &Consumable() const;           // ◄── Called mainly before returning local String
  public: String &NotConsumable() const;        // ◄── Called mainly from MoveFrom
  public: Boolean IsConsumable() const;         // ◄── "Is this String Consumable?"
  public: Boolean IsMutable() const;            // "Is this String mutable?"
  public: Status  Mutable();                    // Make this string Mutable (possible copy)
  public: Status  Immutable();                  // Make this string Immutable
  public: UInt    Length() const;               // Return number of Chars in the String
  public: UInt    Size() const;                 // Return number of Bytes in the String
  public: Status  CompareEquals(String const &that, Boolean &answer);
  public: Status  CompareRelative(String const &that, Int result);

  // Type Morphing
  //   Implemented in String.cpp
  public: Bytes   MorphToBytes();
  public: Status  MakeBuffer();                 // Turn string into Mutable Buffer type

  // Query
  //   Implemented in StringQuery.cpp
  public: Boolean IsASCII() const;              // "Is this string ASCII only?"
  public: Boolean IsUTF8() const;               // "Does this String have non-ASCII UTF-8 in it?"
  public: Boolean IsUnicode() const;            // ***Same as above***
  public: Boolean IsSensitive() const;          // "Is this string Case Sensitive?"
  public: Boolean IsInsensitive() const;        // "Is this string Case Insensitive?"
  public: String &Sensitivity(Boolean sense) const;// Set Sensitivity (true="Is Case Sensitive")
  public: String &Sensitive() const;            // Set "Is Case Sensitive"
  public: String &Insensitive() const;          // Set "Is NOT Case Sensitive"
  public: UInt    GetType() const;              // Get underlying implementation type of String
  public: UInt    Multiplier() const;           // Get the string multiplier
  public: UInt32  Hash() const;                 // Get the Hash value of this string
  public: UInt32  HashSensitive() const;        // Get the case sensitive Hash value of this string
  public: UInt32  HashInsensitive() const;      // Get the case insensitive Hash value of this string

  // Comparison
  //   Implemented in StringCompare.cpp
  public: Int     Compare(String const &that) const;
  public: Int     Compare(String const &that, Boolean sensitive) const;
  public: Int     CompareSensitive(String const &that) const;
  public: Int     CompareInsensitive(String const &that) const;
  public: Boolean IsEqual(String const &that) const;
  public: Boolean CompareEquals(String const &that) const;
  public: Boolean CompareEquals(String const &that, Boolean sensitive) const;
  public: Boolean CompareEqualsSensitive(String const &that) const;
  public: Boolean CompareEqualsInsensitive(String const &that) const;
  
  // C++ Operators: =, +=, +, *=, *, <<=, <<
  //   Implemented in StringOperator.cpp
  public: String &operator =  (String const &that);
  public: String &operator =  (char const *that);
  public: String &operator =  (char *that);
  public: String &operator =  (char that);
  public: String &operator =  (Char that);
  public: String &operator =  (Int  that);
  public: String &operator =  (UInt that);
  
  public: String &operator += (String const &that);
  public: String &operator += (Char that);
  public: String &operator += (char const *that);
  public: String &operator += (char *that);
  public: String &operator += (char that);
  public: String &operator += (Int  that);
  public: String &operator += (UInt that);
  
  public: String  operator +  (String const &that) const;
  public: String  operator +  (Char that)          const;
  public: String  operator +  (char const *that)   const;
  public: String  operator +  (char *that)         const;
  public: String  operator +  (char that)          const;
  public: String  operator +  (Int  that)          const;
  public: String  operator +  (UInt that)          const;
  
  public: String &operator *= (UInt mult);
  public: String &operator *= (Int mult);
  public: String &operator *= (size_t mult);
  
  public: String  operator *  (UInt mult) const;
  public: String  operator *  (Int mult) const;
  
  public: String &operator <<= (char  const *str);
  public: String &operator <<= (char *str);
  public: String &operator <<= (void *ptr);
  public: String &operator <<= (bool  b);
  public: String &operator <<= (char  c);
  public: String &operator <<= (Int   i);
  public: String &operator <<= (UInt  i);

  public: String &operator <<  (char  const *str);
  public: String &operator <<  (char *str);
  public: String &operator <<  (void *ptr);
  public: String &operator <<  (bool  b);
  public: String &operator <<  (char  c);
  public: String &operator <<  (Int   i);
  public: String &operator <<  (UInt  i);
  public: String &operator <<  (Char  c);
  public: String &operator <<  (String const &str);
  
  // Operator == and related compares
  //   Implemented in StringCompare.cpp
  public: Boolean operator == (char const *that)   const;
  public: Boolean operator == (char *that)         const;
  public: Boolean operator == (String const &that) const;
  public: Boolean operator != (char const *that)   const;
  public: Boolean operator != (char *that)         const;
  public: Boolean operator != (String const &that) const;
  public: Boolean operator <  (char const *that)   const;
  public: Boolean operator <  (char *that)         const;
  public: Boolean operator <  (String const &that) const;
  public: Boolean operator <= (char const *that)   const;
  public: Boolean operator <= (char *that)         const;
  public: Boolean operator <= (String const &that) const;
  public: Boolean operator >  (char const *that)   const;
  public: Boolean operator >  (char *that)         const;
  public: Boolean operator >  (String const &that) const;
  public: Boolean operator >= (char const *that)   const;
  public: Boolean operator >= (char *that)         const;
  public: Boolean operator >= (String const &that) const;

  // C++ Operators: []
  //   Implemented in StringSubscript.cpp
  public: Char   operator[] (Int index) const;
  public: String operator[] (Range const &range) const;
  public: String operator[] (Span const &span) const;
  public: String operator[] (Slice const &slice) const;

  // Operator <type>
  //   Implemented in StringOperator.cpp
  public: operator bool();
  public: operator int();

  // Byte Formatting
  //   Implemented in StringFormat.cpp
  public: Status  ToBytes(Byte *buf, UInt maxlen, UInt &usedlen) const;
  public: Status  ToEllipsis(Byte *buf, UInt maxlen) const;
  public: Status  ToEllipsis(Byte *buf, UInt maxlen, UInt &usedlen) const;

  // Content Query
  //  Implemented in StringGet.cpp
  public: Char    Get() const;                  // "Get first char"
  public: Char    Get(Int index) const;         // "Get Nth char"
  public: Status  Get(Int index, Char &c) const;
  public: String  Get(Span  const &span) const;
  public: Status  Get(Span  const &span, String &outSpan) const;  
  public: String  Get(Range const &range) const;
  public: Status  Get(Range const &range, String &outRange) const;  
  public: String  Get(Slice const &slice) const;
  public: Status  Get(Slice const &slice, String &outSlice) const;

  // Append
  //   Implemented in StringAppend.cpp
  public: Status  Append(Char thechar);
  public: Status  Append(char thechar);
  public: Status  Append(Byte *buf, UInt size);
  public: Status  Append(String const& that);
  public: Status  Append(void   const* that, UInt thatSize);
  public: Status  Append(char   const* that);
  public: Status  Append(char        * that);

  // Insert
  //   Implemented in StringInsert.cpp
  public: Status  Insert(Int index, Char          that);
  public: Status  Insert(Int index, char          that);
  public: Status  Insert(Int index, String const &that);
  
  // Remove
  //   Implemented in StringRemove.cpp
  public: Status  Remove(Int          index);
  public: Status  Remove(Range const &range);
  public: Status  Remove(Span  const &span);
  
  // Replace - Single Occurrence
  //   Implemented in StringReplace.cpp
  public: Status  Replace(Int          index, String const &that);
  public: Status  Replace(Int          index, Char          that);
  public: Status  Replace(Int          index, char          that);
  public: Status  Replace(Range const &range, String const &that);
  public: Status  Replace(Range const &range, Char          that);
  public: Status  Replace(Range const &range, char          that);
  public: Status  Replace(Span  const &span,  String const &that);
  public: Status  Replace(Span  const &span,  Char          that);
  public: Status  Replace(Span  const &span,  char          that);
  
  // Find
  //  Implemented in StringFind.cpp
  //
  // Note: We had to add the Find(char...) methods because even though
  //       Char(char) is naturally called by C++, there is an ambiguity
  //       with String.Find(String const &) since String has a (char)
  //       constructor as well.
  public: Status  Find(Char c, Range const &range, Int &foundPos) const;
  public: Status  Find(Char c, Span const &span, Int &foundPos) const;
  public: Status  Find(Char c, Int &foundPos) const;

  public: Int     Find(Char c, Range const &range) const;  
  public: Int     Find(Char c, Span const &span) const;
  public: Int     Find(Char c) const;

  public: Status  Find(char c, Range const &range, Int &foundPos) const;
  public: Status  Find(char c, Span const &span, Int &foundPos) const;
  public: Status  Find(char c, Int &foundpos) const;

  public: Int     Find(char c, Range const &range) const;
  public: Int     Find(char c, Span const &span) const;
  public: Int     Find(char c) const;

  public: Status  Find(String const &string, Range const &range, Int &foundPos) const;
  public: Status  Find(String const &string, Span const &span, Int &foundPos) const;
  public: Status  Find(String const &string, Int &foundPos) const;

  public: Int     Find(String const &string, Range const &range) const;
  public: Int     Find(String const &string, Span const &span) const;
  public: Int     Find(String const &string) const;
  public: Int     Find(char const *string) const;

  public: Int     FirstBlank() const;
  public: Int     LastBlank() const;
  public: Int     FirstWhite() const;           // ◄── "WhiteSpace" is anything <= 0x20
  public: Int     LastWhite() const;            // ◄── "WhiteSpace" is anything <= 0x20
  public: Int     FirstNonBlank() const;
  public: Int     LastNonBlank() const;
  public: Int     FirstNonWhite() const;        // ◄── "WhiteSpace" is anything <= 0x20
  public: Int     LastNonWhite() const;         // ◄── "WhiteSpace" is anything <= 0x20
  
  public: Int     FindChars(String const &string, Int startPos) const;

  // Unicode Case Folding
  //   Implemented in StringFolding.cpp
  public: String &ToLowercase() const;          // In-place     Lowercase (becomes mutable))
  public: String &ToUppercase() const;          // In-place     Uppercase (becomes mutable)
  public: String  Lowercase();                  // Out-of-place Lowercase
  public: String  Uppercase();                  // Out-of-place Uppercase
  public: Status  Fold(Boolean doFold);         //  In-place ASCII/Unicode Fold/Unfold (becomes mutable)

  // Content manipulation
  //  Implemented in StringManip.cpp
  public: String &Trim();
  public: String &LeftTrim();
  public: String &RightTrim();

  public: String &Truncate();
  public: String &Truncate(UInt len);

  public: String &Pad(UInt len);
  public: String &Pad(UInt len, Char padChar);
  public: String &RightPad(UInt len);
  public: String &RightPad(UInt len, Char padChar);
  public: String &LeftPad(UInt len);
  public: String &LeftPad(UInt len, Char padChar);

  public: String  Ellipsis(UInt len);           // Note: Out-of-place because these 'might'
  public: String  PreEllipsis(UInt len);        //       need to modity the String's
  public: String  MidEllipsis(UInt len);        //       contents for display purposes...
  public: String  PostEllipsis(UInt len);       //       So you'll want the original untouched

  public: String  PadOrEllipsis(UInt len);                // Out-of-place because of Ellipsis()
  public: String  PadOrEllipsis(UInt len, Char padChar);  // Out-of-place because of Ellipsis()
  public: String &TruncateOrPad(UInt len);
  public: String &TruncateOrPad(UInt len, Char padChar);

  // Joining
  //   Implemented in StringJoin.cpp
  public: Status  Join(Strings const &strings, String &outString) const;
  public: String  Join(Strings const &strings) const;
  
  // Counting
  //  Implemented in StringSplit.cpp
  public: UInt    Count(Char          c) const; // Count Occurrences of Char in String
  public: UInt    Count(char          c) const; // Count Occurrences of Char in String
  public: UInt    Count(String const &s) const; // Count Occurrences of String in String

  // Splitting
  //  Implemented in StringSplit.cpp
  //
  // Note: SplitToSpans() is preferrable to SplitToStrings() or
  //       SplitToConstStrings() to avoid future bad pointers in
  //       ConstLen Strings or unnecessary memory allocation
  //
  // Note: The Split() methods are equivalent to calling SplitToStrings
  //       and only exist for coding ease.  Still, it's "ease" not
  //       clarity, beause they really mean SplitToStrings!!!
  public: Strings Split(Char searchChar) const                                 {return SplitToStrings(searchChar);}
  public: Strings Split(char searchChar) const                                 {return SplitToStrings(searchChar);}
  public: Strings Split(String const &searchString) const                      {return SplitToStrings(searchString);}
  public: Status  Split(Char searchChar,   Strings &splitArray) const          {return SplitToStrings(searchChar, splitArray);}
  public: Status  Split(char searchChar,   Strings &splitArray) const          {return SplitToStrings(searchChar, splitArray);}
  public: Status  Split(String const &searchString, Strings &splitArray) const {return SplitToStrings(searchString, splitArray);}

  public: Strings SplitToStrings(Char          searchChar  ) const;
  public: Strings SplitToStrings(char          searchChar  ) const;
  public: Strings SplitToStrings(String const &searchString) const;

  public: Status  SplitToStrings(Char          searchChar,   Strings &splitArray) const;
  public: Status  SplitToStrings(char          searchChar,   Strings &splitArray) const;
  public: Status  SplitToStrings(String const &searchString, Strings &splitArray) const;

  public: Spans   SplitToSpans(Char          searchChar  ) const;
  public: Spans   SplitToSpans(char          searchChar  ) const;
  public: Spans   SplitToSpans(String const &searchString) const;

  public: Status  SplitToSpans(Char          searchChar,   Spans &splitArray) const;
  public: Status  SplitToSpans(char          searchChar,   Spans &splitArray) const;
  public: Status  SplitToSpans(String const &searchString, Spans &splitArray) const;

  // Reversing characters
  //   Implemented in StringReverse.cpp
  public: Status Reverse();
  public: Status Reverse(Range const &range);
  public: Status Reverse(Span const &span);
  
  // Escaping and Unescaping
  //   Implemented in StringGeneral.cpp (see StringGeneral.h)
  public: String Escape() const;                // ◄── eg, turn '\' into '\\'
  public: Status Escape(String &outStr) const;  // ◄── eg, turn '\' into '\\'
  public: Status Escape(String &outStr, Boolean escSpace) const;  // ◄── eg, turn '\' into '\\'
  public: String Unescape();                    // ◄── eg, turn '\\' into '\'
  public: Status Unescape(String &outStr) const;// ◄── eg, turn '\\' into '\'
  
  // String variable substitution (eg '%..%'─►'xxxx' and '$(path)'─►'/abc/def;xxx')
  //
  // Note: We'd love to put these in here, but NamedStrings is a template so it
  //       screws up our header dependencies...  Map.h uses "sizeof(String)" and
  //       hence...Kablooi...  Yuck!
  //
  //       Once we manually declare and write Map-like stubs for NamedStrings 
  //       then we'll put these back here.
  //
  //       In the meantime, for String Substitution you'll have to use the
  //       Substitute Function in StringGeneral.h instead of using a String method.
  //
  //public: String Substitute(NamedStrings const &substVars,     // Named Substitution Variables
  //                          String       const &startTag,     // in:  Start Tag (eg '%' and '$('
  //                          String       const &endTag) const;// IN:  End Tag (eg '%' AND ')')
  //public: Status Substitute(NamedStrings const &substVars,  // Named Substitution Variables
  //                          String       const &startTag,   // in:  Start Tag (eg '%' and '$('
  //                          String       const &endTag,     // IN:  End Tag (eg '%' AND ')')
  //                          String       outStr) const;     // OUT: Resulting String
  
  //
  // Protected and non-formal Methods
  //

  // Implemented in String.cpp
  public:    Status _Allocate(UInt numBytes);
  public:    void   _BufAndSize(Byte *&buf, UInt &size) const;
  public:    Status _BufByteAndCharCount(Byte *&buf, UInt &byteCount, UInt &charCount);
  public:    Span   _LogToPhySpan(Span const &that) const;
  public:    Span   _PhyToLogSpan(Span const &that) const;
  public:    Range  _LogToPhyRange(Range const &that) const;
  
  // Underlying SBuffer implementation...
  //   Implemented in String.cpp
  public:    UInt   _GetAllocBytes() const;       // Get allocated bytes
  public:    UInt   _GetIncrementBytes() const;   // Get byte increment count
  public:    UInt   _GetUsedBytes() const;        // Get number of used bytes
  public:    Status _SetUsedBytes(UInt newSize);  // Set number of used bytes
  public:    Byte  *_GetBuf() const;              // Get pointer to the bytes (transient)

  // Implemented in StringBuild.cpp
  protected: Status _BuildFrom(Byte *buf, UInt size);
  protected: Status _BuildFrom(String const &that);
  protected: Status _BuildFrom(char   const *that);
  protected: Status _BuildFrom(char   const *that, UInt thatLen);
  protected: Status _BuildFrom(Char thechar);

  // Persistence
  //   Implemented in StringPersist.cpp
  protected: Status _PersistWriteAtomic(Persist &store);
  protected: Status _PersistReadAtomic(Persist &store);
  
  // Low level Comparison
  //    Implemented in StringCompare.cpp
  protected: Status _CompareEquals             (String const &that, Boolean &result);
  protected: Status _CompareRelative           (String const &that, Int     &result);
  
  protected: Status _CompareEqualsSensitive    (String const &that, Boolean &result);
  protected: Status _CompareEqualsInsensitive  (String const &that, Boolean &result);
  
  protected: Status _CompareRelativeSensitive  (String const &that, Int     &result);
  protected: Status _CompareRelativeInsensitive(String const &that, Int     &result);
  
  // Low level Find String
  //   Implemented in StringFind.cpp
  protected: Status _FindCharLow  (Char          that, Span &span, Int &result);
  protected: Status _FindStringLow(String const &that, Span &Span, Int &result);

  // Splitting (but "not published")
  //  Implemented in StringSplit.cpp
  public: Status  _SplitToConstStrings(Char          searchChar,   Strings &splitArray) const;
  public: Status  _SplitToConstStrings(char          searchChar,   Strings &splitArray) const;
  public: Status  _SplitToConstStrings(String const &searchString, Strings &splitArray) const;

  // Low level Count and Split
  //   Implemented in StringSplit.cpp
  protected: Status _LowCountAndSplit(String const &searchString,   // Search String
                                      Boolean  sensitive,           // true  = "Case sensitive search"
                                      Boolean  constStrings,        // true  = "Make Const Strings"
                                      Boolean  logical,             // true  = "Logical Spans", false="Physical Spans"
                                      Strings *strings,             // !NULL = Strings to populate
                                      Spans   *spans,               // !NULL = Spans to populate
                                      UInt    *count);              // Count of occurrences
  
  // Low level Morphing
  //   Implemented in String.cpp
  protected: Status  _MorphTo(StringType t);
  protected: Status  _MorphTo(StringType t, UInt len);

  //
  // Protected members
  //

  // NOTE: Should be protected but left public for now!!!!!
  public: Boolean  isMutable:1;     // "Is this String mutable?"
  public: Boolean  isByte:1;        // ◄──┬── If both are same it means "unknown"
  public: Boolean  isChar:1;        // ◄──┘
  public: Boolean  isSensitive:1;   // ◄── true = Case sensitive compares
  public: UInt     type:4;          // See enum StringType
  public: UInt8    mult;            // Multiple occurrences
  public: UInt16   constLen;        // ◄── Applicable for types Const..UtfInline
  public: Byte    *data;            // Different usage based on StringType

}; // ...String Class

//----------------------------------------------------------------------
//
// String BEFE Lifecycle...
//

            Status   StartUp       (String &rThis)                     ;
            Status   ShutDown      (String &rThis)                     ;
            Status   Reset         (String &rThis)                     ;
            Status   Clear         (String &rThis)                     ;
BEFE_INLINE Boolean  IsNull        (String const &rThis)               {return rThis.IsNull();}
BEFE_INLINE Status   SetNull       (String &rThis)                     {return rThis.SetNull();}
BEFE_INLINE Boolean  IsEmpty       (String const &rThis)               {return rThis.IsEmpty();}
BEFE_INLINE Status   SetEmpty      (String &rThis)                     {return rThis.SetEmpty();}
            Status   MoveFrom      (String &rThis, UInt8 const &rThat) ;
            Status   CopyFrom      (String &rThis, UInt8 const &rThat) ;
            Boolean  IsConsumable  (String const &rThis)               ;
            String  &Consumable    (String &rThis)                     ;
            String  &NotConsumable (String &rThis)                     ;
            Boolean  IsMutable     (String const &rThis)               ;
            String  &Mutable       (String &rThis)                     ;
            String  &Immutable     (String &rThis)                     ;
            UInt     Length        (String const &rThis)               ;
            UInt     Size          (String const &rThis)               ;
            
} // ...Namespace BEFE

#endif // ..._BEFE_STRING_H
