//----------------------------------------------------------------------
// File: LexToken.h - Declarations for the BEFE LexToken Struct
//----------------------------------------------------------------------
//
// This class is a <Foundation Class> for BEFE Lexical processing.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LEXTOKEN_H // LEXTOKEN_H...
#define LEXTOKEN_H

#include "Primitives.h"
#include "Array.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Struct LexToken
//

struct LexToken {

  // Enum LexTokenType
  //
  // Note: Most of these token types are based entirely on settings in 
  //       the LexLanguage used to process the LexTokens.
  //

  enum LexTokenType : Byte { // Enum LexToken.Type...

    Unknown        // 00: Unknown (initial value)
   ,EndOfInput     // 01: Input exhausted
   ,EndOfLine      // 02: Typically '\n'
   ,Error          // 03: Typically Error::None
   ,Unexpected     // 04: Character not recognised
   ,Warning        // 05: Typically Error::None
   ,WhiteSpace     // 06: Whitespace (typically 0x00..0x20)
   ,Operator       // 07: Some operator
   ,BracketLeft    // 08: Beginning of some grouping
   ,BracketRight   // 09: End of some grouping
   ,Separator      // 10: List separator
   ,Comment        // 11: Comment
   ,Name           // 12: The "Name" of something
   ,Number         // 13: Number literal
   ,DLiteral       // 14: Double-quote literal
   ,SLiteral       // 15: Single-quote literal
	 ,BLiteral       // 16: Back-quote literal
   ,MultiQuote     // 17: Multi-line String literal
	 ,NameLiteral    // 18: Befe Name literal
   ,Preprocessor   // 19: C/C++/Befe Preprocessor Directive
   ,NumBaseTypes
     
  }; // ...Enum Type
  
  // Members
  LexTokenType type;        // Token Type (see above)
  Byte         _reserved[1];// Unused (for now)
  UInt         lineCount;   // Number of '\n' in Token (typically 0)
  Status       status;      // ◄── For Error/Warning (can use instead of value)
  String       value;       // Value (populated up to 4 bytes except for Error/Warning)
  UInt         lineNo;      // Input Line Number (0..)
  UInt         colNo;       // Input Column Number (0..)
  Span         span;        // Span (File offset relative)

	// C++ Lifecycle
  //   Implemented in LexToken.cpp
	LexToken();
  LexToken(LexToken const &that);
  ~LexToken();
  LexToken &operator=(LexToken const &that);
	
	// BEFE Lifecycle
  //   Implemented in LexToken.cpp
  Status    StartUp();
  Status    ShutDown();
  Status    Clear();
  Status    Reset();
  Boolean   IsNull() const;
  Status    SetNull();
  Boolean   IsEmpty() const;
  Status    SetEmpty();
  Status    MoveFrom(LexToken const &that);
  Status    CopyFrom(LexToken const &that);
  Boolean   IsConsumable() const;
  LexToken &Consumable() const;
  LexToken &NotConsumable() const;
  UInt      Length() const;               // Return number of Chars in the LexToken
  UInt      Size() const;                 // Return number of Bytes in the LexToken
	
};

typedef ArrayValue<LexToken>        LexTokens;
typedef enum LexToken::LexTokenType LexTokenType;

//----------------------------------------------------------------------
//
// LexToken BEFE Lifecycle...
//

            Status    StartUp       (LexToken &rThis)                     ;
            Status    ShutDown      (LexToken &rThis)                     ;
            Status    Reset         (LexToken &rThis)                     ;
            Status    Clear         (LexToken &rThis)                     ;
BEFE_INLINE Boolean   IsNull        (LexToken const &rThis)               {return rThis.IsNull();}
BEFE_INLINE Status    SetNull       (LexToken &rThis)                     {return rThis.SetNull();}
BEFE_INLINE Boolean   IsEmpty       (LexToken const &rThis)               {return rThis.IsEmpty();}
BEFE_INLINE Status    SetEmpty      (LexToken &rThis)                     {return rThis.SetEmpty();}
            Status    MoveFrom      (LexToken &rThis)                     ;
            Status    CopyFrom      (LexToken &rThis)                     ;
            Boolean   IsConsumable  (LexToken const &rThis)               ;
            Char     &Consumable    (LexToken &rThis)                     ;
            Char     &NotConsumable (LexToken &rThis)                     ;
            Boolean   IsMutable     (LexToken const &rThis)               ;
            LexToken &Mutable       (LexToken &rThis)                     ;
            LexToken &Immutable     (LexToken &rThis)                     ;
            UInt      Length        (LexToken const &rThis)               ;
            UInt      Size          (LexToken const &rThis)               ;

} // ...Namespace BEFE

#endif // ...LEXTOKEN_H
