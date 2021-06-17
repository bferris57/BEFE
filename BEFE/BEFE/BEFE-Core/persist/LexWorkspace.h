//----------------------------------------------------------------------
// File: LexWorkspace.h - Declarations for the BEFE LexWorkspace Class
//----------------------------------------------------------------------
//
// This file contains the main declarations for the BEFE Lexical
// Workspace Class and related declarations.
//
// A LexWorkspace is responsible for maintaining a file to hold Lexical
// processing data structures during processing.
//
// Since Lexical processing, by nature, requires a lot of String
// manipulation and duplicate strings are almost certainly going to 
// happen, we've identified the following main types of Strings and
// each are handled differently...
//
//  o Mini-Strings (<= 4 Bytes)
//  o General Strings (any length)
//
// Mini-Strings, like LexToken Operators, Brackets, etc. are stored and
// maintained in-line in the LexTokens.  These are NOT assigned an Id.
//
// General Strings (any length) are maintained separately and each of
// them are assigned an Id (only meaningful within a single LexWorkspace)
// and are managed as a virtual array of <Id,String> entries.
//
// Various other variable length data implementations (such as Array,
// Sparse, and Map) are persistent along with General Strings.  Each of
// these are also assigned an Id (within the LexWorkspace).
//
// NOTE: Currently we don't pay too much attention, or worry ourselves
//       sick over, "garbage collection" of old Strings, Arrays, etc.
//       This is because the current intent of a LexWorkspace is to
//       provide transient management of these structures.  However, we
//       may have to beef this up in the near future if we decide to
//       make a LexWorkspace more of a long-term persistent store.
//       *** So, keep this in mind. ;-) ***
//
// A LexWorkspace file contains one Header Block and zero or more
// Data Blocks.
//
// Each Workspace File Block (except for block zero, the header) has an
// 8..10 byte LexWsBlockHead (see LexWs.h) overhead which is maintained
// in the following format...
//
//  ┌────┼────┼────┼────┼────┼────┼────┼────┼────┼────┐
//  │Type│NextBlockNo   │Levl│PrevBlockNo   │NumEntry │
//  └────┼────┼────┼────┼────┼────┼────┼────┼────┼────┘
//                        │                  │       │
//                        │                  └───┬───+
//                        │                      │
//                        │                      └─── Number of Entries in Block
//                        └────────────────────────── Index Level (0=Bottom)
//
//     Where: Type        - Type (0=Header, 1=BlobIndex, 2=Blob, etc.)
//            NextBlockNo - Next Block Number (0..numBlocks)
//            Levl        - Index Level (only used for Index blocks, 0=Bottom)
//            PrevBlockNo - Previous Block Number (0..numBlocks)
//            NumEntry    - Number of Index entries(only used for Index Blocks)
//
// File contents are stored in Blobs in the following Structure...
//
//   ┌────┼────┼────┼────┐
//   │NumLines           │
//   ├────┼────┼────┼────┤
//   │Offset             │ Line 0 Offset in Blob
//   ├────┼────┼────┼────┤
//   │Offset             │ Line 1 Offset in Blob
//   ├────┼────┼────┼────┤
//            ...
//   ├────┼────┼────┼────┤
//   │Offset             │ Line NumLines-1 Offset in Blob
//   ├────┼────┼────┼────┼────┼────┼
//   │Data│Data│Data│Data│Data│Data│... The "real" file text
//   ├────┼────┼────┼────┼────┼────┼
//
//      Where: NumLines - Number of lines in the file
//             Offset   - Offset into Data of Line 0..NumLines-1
//             Data     - File content Bytes
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
//
// NOTE: In this initial implementation, we don't manage higher level
//       index blocks. These, however, will be implemented fairly soon
//       since they're going to be needed for reasonable performance on
//       LexWorkspaceStrings.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LEXWORKSPACE_H // LEXWORKSPACE_H...
#define LEXWORKSPACE_H

#include "Primitives.h"
#include "LexWs.h"

namespace BEFE { // Namespace BEFE...

// Forward class/struct declarations
struct Lex;

//----------------------------------------------------------------------
//
// Class: LexWorkspace - A persistent Lexical Workspace
//

class LexWorkspace { // Class LexWorkspace...

  // Friends
  friend class Lex;
  friend class LexFormatOut;
  
	// C++ Lifecycle
	public: LexWorkspace();
  public: LexWorkspace(LexWorkspace const &that);
	public: ~LexWorkspace();
  public: LexWorkspace &operator=(LexWorkspace const &that);
	
	// BEFE Lifecycle
  public: Status        StartUp();
  public: Status        ShutDown();
  public: Status        Clear();
  public: Status        Reset();
  public: Boolean       IsNull() const;
  public: Status        SetNull();
  public: Boolean       IsEmpty();
  public: Status        SetEmpty();
  public: Status        MoveFrom(LexWorkspace const &that);
  public: Status        CopyFrom(LexWorkspace const &that);
	public: Boolean       IsConsumable() const;
  public: LexWorkspace &Consumable() const;
  public: LexWorkspace &NotConsumable() const;

  // Lex Management
  //   (should only be called from Lex implementations)
  protected: Boolean IsAttached() const;
  protected: Status  _Attach(Lex &lex);
  protected: Status  _Detach();
  
  // File Management
  public: Boolean IsOpen() const;
  public: Status  _Open();            // ◄── You shouldn't really need this
  public: Status  _Close();           // ◄── You shouldn't really need this
  public: Status  _FlushHead();       // ◄── You shouldn't really need this
  
  // Workspace methods
  public: Status SetFileName(String const &fileName);
  
  //
  // Content Methods
  //
  
  // Workspace File Block methods
  //   Implemented in LexWorkspace.cpp
  public: Status  NewFileBlock(UInt blockType, UInt32 &blockNo);
  public: Status  FreeFileBlock(UInt32 blockNo);

  // Workspace general Id Entry workhorse methods...
  //   Implemented in LexWorkspaceIdEntries.cpp
  public: UInt    _IdEntryNew(LexWsArrayHead &arrayHead, UInt32 &blockNo, UInt32 &entryNo, Id &newId);
  public: Status  _IdEntryFind(LexWsArrayHead &arrayHead, Id findId, UInt32 &blockNo, UInt32 &entryNo);
  
  // Workspace Blob methods
  //   Implemented in LexWorkspaceBlob.cpp
  public: UInt    BlobCount();
  public: Status  BlobNew(Id &blobId);
  public: Status  BlobDelete(Id blobId);
  public: Boolean BlobExists(Id blobId);
  public: UInt    BlobLength(Id blobId);
  public: UInt    BlobSize(Id blobId);
  public: Status  BlobAppend(Id blobId, Byte *bytes, UInt len);
  public: Status  BlobInsert(Id blobId, USpan &span, Byte *bytes);
  public: Status  BlobRemove(Id blobId, USpan &span);
  public: Bytes   BlobGet(Id blobId);
  public: Status  BlobGet(Id blobId, USpan &span, Byte *bytes);
  public: Status  BlobGet(Id blobId, Bytes &bytes);
  public: Status  _BlobAppendEmptyBlock(UInt headBlockNo, UInt headEntryNo, LexWsBlob &blobHead);
  
  // Workspace String methods
  //   Implemented in LexWorkspaceString.cpp
  public: UInt    StringCount();
  public: Status  StringNew(String const &str, Id &strId);
  public: Status  StringDelete(Id strId);
  public: Status  StringFind(Id stringId, UInt &blockNo, UInt &entryNo, LexWsString &stringHead);
  public: Boolean StringExists(Id strId);
  public: Boolean StringExists(String &that);
  public: UInt    StringLength(Id strId);
  public: UInt    StringSize(Id strId);
  public: String  StringGet(Id strId);
  public: Status  StringGet(Id strId, String &str);
  public: Status  _StringInit(Boolean needBlob);
  
  // Workspace String methods
  //   Implemented in LexWorkspaceFile.cpp
  public: UInt    FileCount();
  public: Status  FileNew(String const &fileName, Id &fileId);
  public: Status  FileDelete(Id fileId);
  public: Boolean FileExists(Id fileId);
  public: UInt    FileLength(Id fileId);
  public: UInt    FileSize(Id fileId);
  public: UInt    _FileNewFileBlob(Id cacheFileId, Id &blobId, Crc32 &crc);
  
  //
  // Protected members
  //

  protected: Boolean    isOpen:1;
  
  protected: Lex       *lex;
  protected: String     fileName;
  protected: Id         fileId;
  protected: UInt       blkSize;
  
  protected: LexWsHead  wsHead;
  protected: Boolean    wsHeadIsDirty:1;
  
}; // ...Class LexWorkspace

} // ...Namespace BEFE

#endif // ...LEXWORKSPACE_H