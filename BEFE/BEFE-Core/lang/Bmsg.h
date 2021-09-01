//----------------------------------------------------------------------
// File: Bmsg.h - Declarations for the BEFE Language ".bmsg"
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BMSG_H // BMSG_H...
#define BMSG_H

#include "Primitives.h"
#include "Lex.h"
#include "LexToken.h"
#include "LexStream.h"
#include "LexLanguage.h"
#include "Blang.h"
#include "Message.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// BmsgToken - A single Bmsg Lexical Token
//

struct BmsgToken { // Struct BMsgToken...

  // Enum BmsgTokenType...
  enum BmsgTokenType : Byte { // Enum BmsgTokenType...
  
     TypeUnknown
    ,TypeError            // 1:  Error
    ,TypeWarning          // 2:  Warning
    ,TypeBeginEllipsis    // 3:  Named Start Ellipsis (xxx...)
    ,TypeEndEllipsis      // 4:  Named End Ellipsis (...xxx)
    ,TypeName             // 5:  Name of something
    ,TypeNumber           // 6:  Will need validating though
    ,TypeEquals           // 7:  "="
    ,TypeRange            // 8:  ".."
    ,TypeSeparator        // 9:  ","
    ,TypeString           // 10: "..." and '...'
    
    ,NumTypes

    ,TypeEndOfStream = 0xFF
    
  }; // ...Enum BmsgTokenType

  // Members
  Byte   type;
  UInt   lineNo;
  UInt   colNo;
  String value;
  
  // C++ Lifecycle
	BEFE_inline BmsgToken()                                 {StartUp();}
  BEFE_inline BmsgToken(BmsgToken const &that)            {StartUp(); CopyFrom(that);}
	BEFE_inline ~BmsgToken()                                {ShutDown();}
  BEFE_inline BmsgToken &operator=(BmsgToken const &that) {CopyFrom(that); return *this;}
  
  // BEFE Lifecycle
  Status     StartUp();
  Status     ShutDown();
  Status     Reset();
  Status     MoveFrom(BmsgToken const &that);
  Status     CopyFrom(BmsgToken const &that);
  Boolean    IsNull() const;
  Status     SetNull();
  Boolean    IsEmpty() const;
  Status     SetEmpty();
  UInt       Length();
  UInt       Size();
	Boolean    IsConsumable() const;
  BmsgToken &Consumable() const;
  BmsgToken &NotConsumable() const;

}; // ...Struct BMsgToken

typedef ArrayValue<struct BmsgToken> BmsgTokens;

//----------------------------------------------------------------------
//
// Struct BmsgLine - A .bmsg Input Line of tokens
//
// Note: Because we want to be a bit generous in .bmsg, we allow C++
//       style "/*...\n...\n...*/" comments AND we also allow Python
//       style "trailing \" for line continuation (but we haven't gone
//       so far, yet, as allowing Python '''...\n...\n...''' style
//       string literals, BUT WE MAY DO!)
//
//       So, a BmsgLine STARTS at a given lineNo, but has a lineSpan
//       which typically is lineNo:lineNo+1 but can actually be multiple
//       file lines.
//
//       This may appear weird but we're trying it out in expectation
//       for future stuff in .befe and .btyp languages because we
//       wrote .bmsg BEFORE really getting the other ones plugged
//       properly throughout BEFE-0.4 (The "Language/Modelling Release")
//

struct BmsgLine { // Struct BmsgLine...

  // Members
  Boolean     isConsumable:1;
  UInt        lineNo;       // This Line Number (0..numLines-1)
  UInt        numTokens;    // ◄── Denormalised number of BmsgTokens (tokens.Length())
  BmsgToken  *token0;       // ◄── Denormalised pointer to first BmsgToken in Line
  BmsgTokens  tokens;
  
  // C++ Lifecycle
	BEFE_inline BmsgLine()                                {StartUp();}
  BEFE_inline BmsgLine(BmsgLine const &that)            {StartUp(); CopyFrom(that);}
	BEFE_inline ~BmsgLine()                               {ShutDown();}
  BEFE_inline BmsgLine &operator=(BmsgLine const &that) {CopyFrom(that); return *this;}
  
  // BEFE Lifecycle
  Status   StartUp();
  Status   ShutDown();
  Status   Reset();
  Status   MoveFrom(BmsgLine const &that);
  Status   CopyFrom(BmsgLine const &that);
  Boolean  IsNull() const;
  Status   SetNull();
  Boolean  IsEmpty() const;
  Status   SetEmpty();
  UInt     Length() const;
  UInt     Size() const;
	Boolean  IsConsumable() const;
  BmsgLine &Consumable() const;
  BmsgLine &NotConsumable() const;

#ifdef DEBUG
  Strings   Format();
  void      Display();
  void      Display(String indent);
#endif
  
}; // ...struct BmsgLine

//----------------------------------------------------------------------
//
// Class Bmsg - The .bmsg Language Class
//

class Bmsg : public Blang { // Class Bmsg...

  //
  // Methods
  //
  
  // C++ Lifecycle
	public: BEFE_inline Bmsg()                            {StartUp();}
  public: BEFE_inline Bmsg(Bmsg const &that)            {StartUp(); CopyFrom(that);}
	public: BEFE_inline ~Bmsg()                           {ShutDown();}
  public: BEFE_inline Bmsg &operator=(Bmsg const &that) {CopyFrom(that); return *this;}
  
  // BEFE Lifecycle
  public: Status   StartUp();
  public: Status   ShutDown();
  public: Status   Reset();
  public: Status   MoveFrom(Bmsg const &that);
  public: Status   CopyFrom(Bmsg const &that);
  public: Boolean  IsNull() const;
  public: Status   SetNull();
  public: Boolean  IsEmpty() const;
  public: Status   SetEmpty();
  public: UInt     Length() const;
  public: UInt     Size() const;
  
  // Operational Methods
  //
  // Note: The "Get..." methods give you a copy of the results while
  //       the "Take..." methods give you THE results and clears the originals.
  //
  //       So "Get..." uses CopyFrom and "Take..." uses MoveFrom.

  public: Status   ProcessFile(String const &pFileName);
  
  public: Status   GetCounts    (UInt &rangeCount,
                                 UInt &msgCount,
                                 UInt &errorCount,
                                 UInt &warningCount);
                                 
  public: Status   GetErrors    (BmsgTokens &theErrors);
  public: Status   TakeErrors   (BmsgTokens &theErrors);

  public: Status   GetWarnings  (BmsgTokens &theWarnings);
  public: Status   TakeWarnings (BmsgTokens &theWarnings);
  
  public: Status   GetResults   (MessageRanges &theRngArray,
                                 Messages      &theMsgArray,
                                 MessageIds    &theIdIndexes,
                                 MessageNames  &theNameIndexes);

  public: Status   TakeResults  (MessageRanges &theRngArray,
                                 Messages      &theMsgArray,
                                 MessageIds    &theIdIndexes,
                                 MessageNames  &theNameIndexes);  

  // Low Level Under-the-cover Methods
  protected: Status   _StartParsing();
  protected: Status   _StopParsing();
  protected: Status   _NextBmsgToken(BmsgToken &token);
  
  // High Level Under-the-cover Methods
  protected: Status   _NextBmsgLine(BmsgLine &line);
  protected: Status   _ProcessLines(Boolean doit);
  
  //
  // Members
  //
  
  // Messages and MessagesRanges we're building...
  protected: MessageRanges ranges;
  protected: Messages      messages;
  protected: MessageIds    msgIdIndexes;
  protected: MessageNames  msgNameIndexes;
  protected: BmsgTokens    errors;         // ◄── Errors
  protected: BmsgTokens    warnings;       // ◄── Warnings
  protected: UInt          numRanges;      // ◄── Total MessageRanges delivered so far
  protected: UInt          numMessages;    // ◄── Total Messages delivered so far
  protected: UInt          numWarnings;    // ◄── Total Warnings delivered so far
  protected: UInt          numErrors;      // ◄── Total Errors delivered so far
  
  // Internal: BmsgLine processing
  protected: BmsgToken     nextBmsgToken;  // ◄── One BmsgToken "look ahead" for line processing
  
}; // ...Class Bmsg

} // ...Namespace BEFE

#endif // ...BMSG_H
