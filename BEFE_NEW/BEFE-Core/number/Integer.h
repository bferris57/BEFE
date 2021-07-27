//!befe-publish inc
//----------------------------------------------------------------------
// File: Integer.h - Declaration of the Integer Class
//----------------------------------------------------------------------
//
// This header declares the Integer Class.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef INTEGER_H // INTEGER_H...
#define INTEGER_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// BEFE Integer Class
//
// This class implements an "unlimited precision" integer number.
//
// Unlike processor registers and how processors typically treat numbers
// in memory, we don't have a "one's complement" or "two's complement"
// scenario. Instead, we keep all non-zero numbers to whatever 
// precision they need and, when you want it as a typical processor
// aware number (a byte, word, etc.) we convert it for you.
//
// This may seem like an odd approach, and it is.  What it means is
// the implementation focuses more to the "Distance" operation than
// it does the "Subtract" operation.
//
// We also take sign into account when performing various "compare"
// operations, meaning negative numbers are "less" than positive ones.
// This sort of rubs us the wrong way in a fundamental sense but
// that's how people these days are taught to think, so in order to
// be practical about it, we need to cater for the "negative" notion.
//
// Note: There are various flavours of "infinite precision" numbers
//       knocking about the internet (most notably the GNU MP Library)
//       but we haven't used those here mainly because of the licensing
//       restrictions placed on them and, more importantly, it's so
//       much more fun to write it yourself, and it really does need
//       to exist to test GNU's rather hubris filled claim of being
//       the "fastest" claim to see if it is justified or not.
//
// Note: Our current implementation is with UInt32 sized digits.
//       However, we've made the code resilient enough to transparently
//       contain larger "Digits" (see Digit typedef in Integer Class)
//
// Note: Because we're using a representational numbering system we
//       keep all variable length buffered data in little-endian
//       order.  This is because we always start processing
//       representational numbers at the "little end", like we were
//       taught in primary school.  ;-)
//
//       However, for the short versions where the value is kept
//       in memory right there in the data member instead of in
//       a buffer.  We keep it there in whatever form it is natural
//       to for the processor that the code is running on.
//

class Integer { // Class Integer...

  // Digit typedef
  typedef UInt32 Digit;
  
  //
  // Integer State
  //
  // Essentially, the complete number state can be represented by a
  // small number of bits, as follows...
  //
  //  ┌─┬─┬─┐
  //  │N│Z│O│
  //  │E│R│N│
  //  │G│O│E│
  //  └─┴─┴─┘
  //   │ └┬┘
  //   │  │
  //   │  └─── 0 0 = Not All-Zeros and Not All-Ones => Natural Integer
  //   │       0 1 = Not All-Zeros and Is  All-Ones => Infinity
  //   │       1 0 = Is  All-Zeros and Not All-Ones => Zero
  //   │       1 1 = Is  All-Zeros and Is  All-Ones => NaN
  //   │
  //   └────── 0   = Not Negative
  //           1   =     Negative
  //
  // So, the ZRO bit gives us -0, +0, -Infinity, +Infinity, -N, +N, and
  // -NaN, and +NaN.
  //
  // Note: All non-zero Integer States mean the number has no Digits,
  //       so the conditions (state && data) and (!state && !data) both
  //       indicate an internal error or, at least, an inconsistent state.
  // 
  
  public: enum State {
    
     StateSGN         = 0x04
    ,StateZRO         = 0x02
    ,StateONE         = 0x01
    
    ,StateNatural     = 0x00
    ,StateInfinity    = 0x01
    ,StateZero        = 0x02
    ,StateNaN         = 0x03
    
    ,StateNegNatural  = 0x04
    ,StateNegInfinity = 0x05
    ,StateNegZero     = 0x06
    ,StateNegNaN      = 0x07
    
  };

  enum ValueType {
    TypeNull,    // ◄── Null value
    TypeNone,    // ◄── Value, but it's all in State
    TypeDigit,   // ◄── UInt32 data
    TypeBuffer,  // ◄── Variable length data
    NumTypes
  };
  
  //
  // Protected members
  //

  protected: Boolean  isMutable:1;
  protected: Boolean  isConsumable:1;  // ◄── "Do MoveFrom() when operator= or copy called next"
  public:    UInt     state:3;         // State (**see above***)
  protected: UInt     type:3;          // Type  (***see above***)
  protected: UInt     reserved:24;     // ◄── Reserved for future use
  public:    union {
               Digit  digit;
               UInt32 buf;
             };

  //
  // Public Instance Methods
  //
  
  // C++ Lifecycle
  //   Implemented in Integer.cpp
  public: Integer();
  public: Integer(Integer const &that);
  public: Integer(String const &that);
  public: Integer(char   const *that);
  public: Integer(char         *that);
  public: Integer(char   val);
  public: Integer(UInt8  val);
  public: Integer(UInt16 val);
  public: Integer(UInt32 val);
  public: Integer(UInt64 val);
  public: Integer(SInt8  val);
  public: Integer(SInt16 val);
  public: Integer(SInt32 val);
  public: Integer(SInt64 val);
  public: ~Integer();
  
  // BEFE Lifecycle
  //   Implemented in Integer.cpp  
  public: Status   StartUp();
  public: Status   ShutDown();
  public: Status   Reset();
  public: Boolean  IsNull() const;
  public: Status   SetNull();
  public: Status   MoveFrom(Integer const &that);
  public: Status   CopyFrom(Integer const &that);
  public: Integer &Consumable() const; // ◄── Called mainly while returning local Integer as
                                       //       return value (implemented in IntegerOperator.cpp)
  public: String   ToString() const;
  
  // Digit operations
  //   Implemented in IntegerDigit.cpp
  public: Status Add        (Digit digit);
  public: Status Subtract   (Digit digit);
  public: Status Distance   (Digit digit);  // ◄── Distance(0) == Abs()
  public: Status Increment  ();
  public: Status Decrement  ();
  public: Status Multiply   (Digit digit);
  public: Status Divide     (Digit digit);
  public: Status Modulus    (Digit digit);
  
  // Full static operations
  //  Implemented in IntegerFull.cpp
  public: static Status Compare    (Integer const &num1, Integer const &num2, SInt    &result);
  public: static Status Add        (Integer const &num1, Integer const &num2, Integer &result);
  public: static Status Subtract   (Integer const &num1, Integer const &num2, Integer &result);
  public: static Status Difference (Integer const &num1, Integer const &num2, Integer &result);
  public: static Status Multiply   (Integer const &num1, Integer const &num2, Integer &result);
  public: static Status Divide     (Integer const &num1, Integer const &num2, Integer &result);
  
  // Operator =
  //   Implemented in IntegerOperator.cpp
  public: Integer &operator=  (Integer const &that);
  public: Integer &operator=  (char const *that);
  public: Integer &operator=  (String const &that);
  public: Integer &operator=  (char  *that);
  public: Integer &operator=  (char   that);
  public: Integer &operator=  (UInt8  that);
  public: Integer &operator=  (SInt8  that);
  public: Integer &operator=  (UInt16 that);
  public: Integer &operator=  (SInt16 that);
  public: Integer &operator=  (UInt32 that);
  public: Integer &operator=  (SInt32 that);
  public: Integer &operator=  (UInt64 that);
  public: Integer &operator=  (SInt64 that);

  // Operator +
  //   Implemented in IntegerOperator.cpp
  public: Integer operator+  (Integer const &that) const;
  public: Integer operator+  (char const *that) const;
  public: Integer operator+  (char  *that) const;
  public: Integer operator+  (char   that) const;
  public: Integer operator+  (UInt8  that) const;
  public: Integer operator+  (SInt8  that) const;
  public: Integer operator+  (UInt16 that) const;
  public: Integer operator+  (SInt16 that) const;
  public: Integer operator+  (UInt32 that) const;
  public: Integer operator+  (SInt32 that) const;
  public: Integer operator+  (UInt64 that) const;
  public: Integer operator+  (SInt64 that) const;

  // Operator -
  //   Implemented in IntegerOperator.cpp
  public: Integer operator-  (Integer const &that) const;
  public: Integer operator-  (char const *that) const;
  public: Integer operator-  (char  *that) const;
  public: Integer operator-  (char   that) const;
  public: Integer operator-  (UInt8  that) const;
  public: Integer operator-  (SInt8  that) const;
  public: Integer operator-  (UInt16 that) const;
  public: Integer operator-  (SInt16 that) const;
  public: Integer operator-  (UInt32 that) const;
  public: Integer operator-  (SInt32 that) const;
  public: Integer operator-  (UInt64 that) const;
  public: Integer operator-  (SInt64 that) const;

  // Typecasting...
  //   Implemented in IntegerOperator.cpp
  public: operator Int  ()  const;
  public: operator UInt () const;
  
}; // ...Class Integer

} // ...Namespace BEFE

#endif // ...INTEGER_H
