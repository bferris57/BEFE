//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: Dictionary.h - Dictionary Class declaration
//----------------------------------------------------------------------
//
// The Dictionary Class, as the name implies, implements a persistent
// Dictionary of an arbitrairily large list of Words.
//
// This first implementation has no information associated with each
// word.  Still, a Dictionary is useful for determining if a word
// exists in the dictionary.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef DICTIONARY_H // DICTIONARY_H...

#include "Primitives.h"
#include "Sort.h"
#include "Persist.h"

namespace BEFE { // Namespace BEFE...
  
//----------------------------------------------------------------------
//
// Dictionary Class
//

class Dictionary { // Class Dictionary...

  // Friends
  friend class DictSortTarg;
  
  // C++ Lifecycle
  public:          Dictionary();
  public:          Dictionary(Dictionary const &that);
  public: virtual ~Dictionary();
  public: virtual  Dictionary &operator=(Dictionary const &that);
  
  // BEFE Lifecycle
  public: virtual Status StartUp();
  public: virtual Status ShutDown();
  public: virtual Status Reset();
  public: virtual Status MoveFrom(Dictionary const &that);
  public: virtual Status CopyFrom(Dictionary const &that);
  public: virtual Status WriteTo(Persist &store);
  public: virtual Status ReadFrom(Persist &store);
  
  public: virtual UInt   Length () const;
  public: virtual UInt   Size   () const;
  
  public: virtual Dictionary &Consumable() const;
  public: virtual Dictionary &NotConsumable() const;
  public: virtual Boolean     IsConsumable() const;
  
  // More Persistence
  public: virtual Status ReadFromText(char   const *fileName);
  public: virtual Status ReadFromText(String const &fileName);
  
  // Entry query
  public: virtual Status  Get(UInt index, String &entry);
  public: virtual String  Get(UInt index);
  public: virtual Status  Find(String const &string, UInt &index);
  public: virtual Status  FindInsensitive(String const &string, UInt &index);
  public: virtual Boolean Exists(String const &string);
  public: virtual Boolean ExistsInsensitive(String const &string);
  
  //
  // Protected members
  //
  
  // Bit Members
  protected: UInt     isConsumable:1;
  protected: UInt     isSorted:1;
  protected: UInt     numWords:30;
  
  // File name
  protected: String   fileName; 
  // File contents
  protected: Buffer   contents;  
  // Word index pointers
  protected: UInts    entryOffsets;   // Entry Index Buffer
  // Sorted order
  protected: UInts    sEntryOffsets;  // Sorted Entry Index Buffer
  
  // Statistics...
  protected: UInt     minWordLen;     // Minimum word length
  protected: UInt     maxWordLen;     // Maximum word length
  protected: UInt     totWordLen;     // Total word lengths
  
}; // ...Class Dictionary

} // ...Namespace BEFE

#endif // ...DICTIONARY_H