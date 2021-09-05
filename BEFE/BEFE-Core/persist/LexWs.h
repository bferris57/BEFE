//----------------------------------------------------------------------
// File: LexWs.h - Declarations of the BEFE LexWs Low Level Data
//----------------------------------------------------------------------
//
// This file contains the declarations for Low Level Data Types.
//
// The "Ws Data Types" are basically header Structs within a Workspace
// file instead of "the real data".
//
// So, for example, a LexWsHead Struct contains the LexWorkspace file
// header, and a LexWsBlob is the header used to support the actual
// LexWorkspace.Blob... methods and indicates a single Blob with an
// Id.  And a LexWsString is used to support the LexWorkspace.String...
// methods and indicates a single Stringwith an Id.
//
// Each LexWorkspace file contains zero or more Blobs, Strings,
// Files, etc.  Each of the Structs here can be thought of as an element
// in the "Array of Blobs", "Array of Strings", etc. and, since they
// are fixed size, are the low level entries in each Array where the
// actual contents of each Blob, String, etc. are stored separately
// in various forms in an way appropriate for efficient access.
//
// NOTE: This is different from the LexWorkspace class itself because
//       the declarations here are for managing the low level block
//       and data structures in the file, whereas a LexWorkspace is more
//       of a higher level operational abstraction layer.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LEXWS_H // LEXWS_H...
#define LEXWS_H

#include "Primitives.h"
#include "Cache.h"
#include "Crc32.h"

namespace BEFE { // Namespace BEFE...

// Ids...
typedef Id LexWsBlobId;
typedef Id LexWsFileId;
typedef Id LexWsStringId;

namespace LexWs { //Namespace LexWs...

  // Block Types...
  enum BlockType : Byte {

     BlockTypeFree              // 0x00 - Free
     
    ,BlockTypeBlob              // 0x01 - LexWsBlob Entry
    ,BlockTypeString            // 0x02 - LexWsString Entry
    ,BlockTypeFile              // 0x03 - LexWsFile Entry
    ,BlockTypeBlobData          // 0x04 - Blob Data
    ,NumBlockTypes
    
    ,BlockTypeNull = 0xFF       // 0xFF - Null
    
  };
  
} // ...Namespace LexWs

//----------------------------------------------------------------------
//
// Struct: LexWsArrayHead - One of these for each Array in a Persistent
//                          Lexical Workspace
//
// LexWsArrays are persisent Arrays of fixed sized Entries.  However,
// each Entry may indicate Ids of variable length members that are
// managed separately from the array using a different storage
// mechanism.
//
// Typically, however, each entry in the array can be assigne an Id.
// As such, the array keeps track of the highest allocate Id so far but
// does not itself directly manage the searching for Ids, as the Ids
// may become Sparse through Time.
//
// Note: This is implemented in LexWsArrayHead.cpp
//
// Note: Instances of these are generally sitting in the LexWsHead
//       at various places but there's nothing stopping us from
//       putting them in just about anywhere in any kind of block
//       we wish;
//

struct LexWsArrayHead { // Struct LexWsArrayHead...

	// C++ Lifecycle
	LexWsArrayHead();
  LexWsArrayHead(LexWsArrayHead const &that);
	~LexWsArrayHead();
  LexWsArrayHead &operator=(LexWsArrayHead const &that);
	
	// BEFE Lifecycle
  Status     StartUp();
  Status     ShutDown();
  Status     Clear();
  Status     Reset();
  Boolean    IsNull() const;
  Status     SetNull();
  Boolean    IsEmpty();
  Status     SetEmpty();
  Status     MoveFrom(LexWsArrayHead const &that);
  Status     CopyFrom(LexWsArrayHead const &that);

  // Other BEFE functionality
  String     ToString() const;
  Strings    ToStrings() const;
  
  // Persistence
  UInt       PerBlock() const;
  UInt       PersistSize() const;
  Status     SetEntryPersistSize(UInt numBytes);
  Status     Write(UInt fileId, UInt blockNo, UInt entryNo);
  Status     Read(UInt fileId, UInt blockNo, UInt entryNo);
  
  //
  // Members
  //

  Byte   blockType;         // Block Type (see enum BlockType above)
  Id     nameId;            // StringId of name
  Id     hiId;              // High Id so far
  UShort entrySize;         // Low Entry size (Bytes)
  UShort entriesPerBlock;   // Number of entries per Low Array Block
  UInt32 numEntries;        // Number of Entries
  UInt32 fstLowBlockNo;     // First Low Block Number
  UInt32 lstLowBlockNo;     // Last Low Block Number
  UInt32 topBlockNo;        // Top Level Index Block Number
  
}; // ...Struct LexWsArrayHead

//----------------------------------------------------------------------
//
// LexWsArrayHead General BEFE Functions
//

BEFE_INLINE Boolean IsNull(LexWsArrayHead const &that)    {return that.IsNull();}
BEFE_INLINE Status  SetNull(LexWsArrayHead &that)         {return that.SetNull();}
BEFE_INLINE Strings ToStrings(LexWsArrayHead const &that) {return that.ToStrings();}

//----------------------------------------------------------------------
//
// Struct: LexWsHead - A persistent Lexical Workspace Header
//
// Notes: Implemented in LexWsHead.cpp
//

struct LexWsHead {

	// C++ Lifecycle
	LexWsHead();
  LexWsHead(LexWsHead const &that);
	~LexWsHead();
  LexWsHead &operator=(LexWsHead const &that);
	
	// BEFE Lifecycle
  Status     StartUp();
  Status     ShutDown();
  Status     Clear();
  Status     Reset();
  Boolean    IsNull() const;
  Status     SetNull();
  Boolean    IsEmpty();
  Status     SetEmpty();
  Status     MoveFrom(LexWsHead const &that);
  Status     CopyFrom(LexWsHead const &that);
	Boolean    IsConsumable() const;
  LexWsHead &Consumable() const;
  LexWsHead &NotConsumable() const;

  // Other BEFE functionality
  String     ToString() const;
  Strings    ToStrings() const;
  
  // Persistence
  UInt       PersistSize() const;
  Status     Write(Id fileId);
  Status     Read(Id fileId);
  
  //
  // Members
  //

  Id     workspaceId;
  UInt32 blkSize;
  Time   timeCreated;
  Time   timeUpdated;

  // Free Blocks  
  UInt32 fstFreeBlock;
  UInt32 lstFreeBlock;
  
  // Array Headers
  LexWsArrayHead  blobs;
  LexWsArrayHead  files;
  LexWsArrayHead  strings;  
  LexWsBlobId     stringsBlobId;
  
};

//----------------------------------------------------------------------
//
// LexWsHead General BEFE Functions
//

//BEFE_INLINE Boolean IsNull(LexWsHead const &that)    {return that.IsNull();}
//BEFE_INLINE Status  SetNull(LexWsHead &that)         {return that.SetNull();}
BEFE_INLINE Strings ToStrings(LexWsHead const &that) {return that.ToStrings();}

//----------------------------------------------------------------------
//
// Struct: LexWsBlockHead - A persistent Lexical Workspace Block Header
//
// Notes: Implemented in LexWsBlockHead.cpp
//

struct LexWsBlockHead { // Struct LexWsBlockHead...

  // Class (static) methods
  static Status  ValidateBlockType(UInt vType);
  static Boolean IsValidBlockType(UInt vType);
  static UInt    PersistSize(UInt type);
    
	// C++ Lifecycle
	LexWsBlockHead();
  LexWsBlockHead(LexWsBlockHead const &that);
	~LexWsBlockHead();
  LexWsBlockHead &operator=(LexWsBlockHead const &that);
	
	// BEFE Lifecycle
  Status     StartUp();
  Status     ShutDown();
  Status     Clear();
  Status     Reset();
  Boolean    IsNull() const;
  Status     SetNull();
  Boolean    IsEmpty();
  Status     SetEmpty();
  Status     MoveFrom(LexWsBlockHead const &that);
  Status     CopyFrom(LexWsBlockHead const &that);

  // Other BEFE functionality
  String     ToString() const;
  Strings    ToStrings() const;
  
  // Persistence
  Boolean    IsIndexBlock() const;
  UInt       PersistSize() const;
  Status     Write(Id fileId, UInt32 blockNo);
  Status     Read(Id fileId, UInt32 blockNo);
  Status     _SwapBytes();
  
  //
  // Members
  //

  Byte   type;
  UInt32 nextBlockNo;
  Byte   level;
  UInt32 prevBlockNo;
  UInt16 numEntries;
  
}; // ...Struct LexWsBlockHead

//----------------------------------------------------------------------
//
// LexWsBlockHead General BEFE Functions
//

//BEFE_INLINE Boolean IsNull(LexWsBlockHead const &that)    {return that.IsNull();}
//BEFE_INLINE Status  SetNull(LexWsBlockHead &that)         {return that.SetNull();}
BEFE_INLINE Strings ToStrings(LexWsBlockHead const &that) {return that.ToStrings();}

//----------------------------------------------------------------------
//
// Struct: LexWsBlob - One of these per Lexical Workspace Blob
//
// There's one of these for each allocated LexWsBlobId
//
// NOTE: Blobs are basically for storing a contiguous array of arbitrary
//       Byte values.  Each Blob is assigned a LexWsBlobId and may contain
//       zero or more Blob Blocks.
//
//       There's a Blob for each non-zero length LexWsFileId and there's
//       also a single Blob containing all the active LexWsStringIds.
//
//       Then, there's a Blob for each Low Level fixed size data structure...
//
//         o LexWsBlobs
//         o LexWsFiles
//         o LexWsStrings
//         o LexWsFileTokens
//
//       At some later date, there will be multi-layer indexes built up for
//       each fixed size data structures for faster random access.
//

struct LexWsBlob {
  
  // Members
  LexWsBlobId id;
  UInt32      len;
  UInt32      fstLowBlockNo;
  UInt32      lstLowBlockNo;
  UInt32      topBlockNo;
  
  // Class (static) methods
  static UInt PerBlock();
  static UInt PersistSize();
  static UInt EntryPersistSize();
  static UInt DataBytesPerBlock();
  
	// C++ Lifecycle
	LexWsBlob();
  LexWsBlob(LexWsBlob const &that);
	~LexWsBlob();
  LexWsBlob &operator=(LexWsBlob const &that);
	
	// BEFE Lifecycle
  Status     StartUp();
  Status     ShutDown();
  Status     Clear();
  Status     Reset();
  Boolean    IsNull() const;
  Status     SetNull();
  Boolean    IsEmpty();
  Status     SetEmpty();
  Status     MoveFrom(LexWsBlob const &that);
  Status     CopyFrom(LexWsBlob const &that);
	Boolean    IsConsumable() const;
  LexWsBlob &Consumable() const;
  LexWsBlob &NotConsumable() const;

  // Persistence
  Status     Write(Id fileId, UInt blockNo, UInt entryNo);
  Status     Read(Id fileId, UInt blockNo, UInt entryNo);
  
};

//----------------------------------------------------------------------
//
// Struct: LexWsString - One of these per Lexical Workspace String
//
// There's one of these for each allocated LexWsStringId.
//
// NOTE: LexWsStrings are managed by two blobs...
//
//         1) Blob for LexWsString headers (below)
//         2) Blob for String len/values (see LexWsHead)
//

struct LexWsString {

  // Members  
  LexWsStringId id;
  USpan         blobSpan;
  
  // Class (static) methods
  static UInt PerBlock();
  static UInt PersistSize();
  
	// C++ Lifecycle
	LexWsString();
  LexWsString(LexWsString const &that);
	~LexWsString();
  LexWsString &operator=(LexWsString const &that);
	
	// BEFE Lifecycle
  Status       StartUp();
  Status       ShutDown();
  Status       Clear();
  Status       Reset();
  Boolean      IsNull() const;
  Status       SetNull();
  Boolean      IsEmpty();
  Status       SetEmpty();
  Status       MoveFrom(LexWsString const &that);
  Status       CopyFrom(LexWsString const &that);
	Boolean      IsConsumable() const;
  LexWsString &Consumable() const;
  LexWsString &NotConsumable() const;

  // Persistence
  Status     Write(Id fileId, UInt blockNo, UInt entryNo);
  Status     Read(Id fileId, UInt blockNo, UInt entryNo);

};


//----------------------------------------------------------------------
//
// Struct: LexWsFile - One of these per Lexical Workspace Input File
//
// There's one of these for each allocated LexWsFileId
//
// NOTE: LexWsFiles are managed by various blobs...
//
//         1) Blob for LexWsFile headers (below)
//         2) Blob for each LexWsFile

struct LexWsFile {
  
  LexWsFileId   id;
  LexWsStringId name;
  LexWsBlobId   text;
  LexWsStringId lang;
  LexWsBlobId   tokens;
  Crc32         crc;
  Time          snapTime;

  // Class (static) methods
  static UInt PerBlock();
  static UInt PersistSize();
  
	// C++ Lifecycle
	LexWsFile();
  LexWsFile(LexWsFile const &that);
	~LexWsFile();
  LexWsFile &operator=(LexWsFile const &that);
	
	// BEFE Lifecycle
  Status     StartUp();
  Status     ShutDown();
  Status     Clear();
  Status     Reset();
  Boolean    IsNull() const;
  Status     SetNull();
  Boolean    IsEmpty();
  Status     SetEmpty();
  Status     MoveFrom(LexWsFile const &that);
  Status     CopyFrom(LexWsFile const &that);
	Boolean    IsConsumable() const;
  LexWsFile &Consumable() const;
  LexWsFile &NotConsumable() const;

  // Persistence
  Status     Write(Id fileId, UInt blockNo, UInt entryNo);
  Status     Read(Id fileId, UInt blockNo, UInt entryNo);
  
};

} // ...Namespace BEFE

#endif // ...LEXWS_H
