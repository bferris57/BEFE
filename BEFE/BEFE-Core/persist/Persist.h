//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: Persist.h - Persistence related declarations
//----------------------------------------------------------------------
//
// This header contains Persistence related declarations.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef PERSIST_H // PERSIST_H...
#define PERSIST_H

#include "Primitives.h"
#include "Storage.h"
#include "Class.h"
#include "Stream.h"
#include "FileStream.h"

namespace BEFE { // Namespace BEFE...

//
// ValueTypes Namespace-Enum - Enumerates each Value Type
//

namespace ValueType { enum : Byte { // Namespace-Enum ValueType...
  
   ScalarNull     =0x00 // 0x00: Null value ◄─┬─ Both treated interchangably
  ,ScalarNaN      =0xFF // 0xFF: NaN  value ◄─┘ 
   
  // Scalar Value Types
  ,ScalarRaw      =0x01 // 0x01: Raw binary Bytes (<Length>,<Byte>,<Byte>...)
  ,ScalarLength         // 0x02: BEFE Length      (*** See Technical Documentation ***)
  ,ScalarString         // 0x03: String           (<Length>,<Byte>,<Byte>...0x00)
  ,ScalarBoolean        // 0x04: Boolean          (<Byte>)
  ,ScalarUInt8          // 0x05: UInt8            (<Byte>)
  ,ScalarUInt16         // 0x06: UInt16           (<MSByte>,<LSByte>)
  ,ScalarUInt24         // 0x07: ◄── Not Used
  ,ScalarUInt32         // 0x08: UInt32           (<MSByte>,<Byte>,<Byte>,<LSByte>)
  ,ScalarUInt40         // 0x09: ◄── Not Used
  ,ScalarUInt48         // 0x0A: ◄── Not Used
  ,ScalarUInt56         // 0x0B: ◄── Not Used
  ,ScalarUInt64         // 0x0C: UInt64           (<MSByte>,<Byte>,<Byte>,<Byte>,<Byte>,<Byte>,<Byte>,<LSByte>)

  ,ScalarLengths  =0x42 // 0x42: BEFE Length      (*** See Technical Documentation ***)
  ,ScalarStrings        // 0x43: Strings          (<Length>,(<String>,<String>...))
  ,ScalarBooleans       // 0x44: Booleans         (<Length>,(<Byte>,...))
  ,ScalarUInt8s         // 0x45: UInt8s           (<Length>,(<UInt8>,...))
  ,ScalarUInt16s        // 0x46: UInt16s          (<Length>,(<UInt16>,...))
  ,ScalarUInt24s        // 0x47: ◄── Not Used
  ,ScalarUInt32s        // 0x48: UInt32s          (<Length>,(<UInt16>,...))
  ,ScalarUInt40s        // 0x49: ◄── Not Used
  ,ScalarUInt48s        // 0x4A: ◄── Not Used
  ,ScalarUInt56s        // 0x4B: ◄── Not Used
  ,ScalarUInt64s        // 0x4C: UInt64s          (<Length>,(<UInt64>,...))

  ,NumScalars
    
  // Structured Value Types
  ,ScalarNamed    =0x80 // 0x80: Named Scalar Value         (0x80,<String>,<Type>,<value>)
  ,StructAnon           // 0x81: Anonymous Structured Value (0x81,<astruct>)
  ,StructNamed          // 0x82: Named Structured Value     (0x82,<String>,<struct>)
  ,StructInstance       // 0x83: Instance Structured Value  (0x83,<Class>,<struct>)
  ,SequenceAnon         // 0x84: Anonymous Sequence         (0x84,<Type>,<Length>,<entry>,<entry>...)
  ,SequenceNamed        // 0x85: Named Sequence             ──► (0x85,<Name>,<Type>,<Length>,(<entry>,...))
  ,SequenceInstance     // 0x86: Class Sequence             (0x86,<String>,(<entry>,...))
  ,NumberMapAnon        // 0x87: Anonymous Number Map       (0x87,<Length>,((<Name>,<entry>)...))
  ,NumberMapNamed       // 0x88: Named Number Map           (0x88,<Name>,<Length>,((<Name>,<entry>)...))
  ,StringMapAnon        // 0x89: Anonymous String Map       (0x89,<Length>,((<name>,<entry>)...))
  ,StringMapNamed       // 0x8A: Named String Map           (0x8A,<Name>,<Length>,((<Name>,<entry>)...))

  ,MinStructuredType = ScalarNamed
  ,MaxStructuredType = StringMapNamed

  ,EndSequence    =0xFF // 0xFF; End of Sequence
    
  ,Signature      =0xBE // 0xBE: BEFE Signature (various forms)
  
}; } // ...Namespace-Enum ValueType
  
//----------------------------------------------------------------------
//
// Class: Persist - Main Persistence class
//

class Persist { // Class Persist...
  
  //
  // Public Methods
  //
  
  // C++ Lifecycle
  //   Implemented in Persist.cpp
  public: Persist();
  public: virtual ~Persist();
  
  // BEFE Lifecycle
  //   Implemented in Persist.cpp
  public: virtual Status   StartUp();
  public: virtual Status   MoveFrom(Persist const &that);
  public: virtual Status   CopyFrom(Persist const &that);
  public: virtual Status   ShutDown();
  public: virtual Status   Reset();
  public: virtual Boolean  IsNull() const;
  public: virtual Status   SetNull();
  public: virtual Boolean  IsEmpty() const;
  public: virtual Status   SetEmpty();
  public: virtual Persist &Consumable() const;
  public: virtual Persist &NotConsumable() const;
  public: virtual Boolean  IsConsumable() const;
  
  // Persist Member manipulation
  //   Implemeneted in Persist.cpp
  public: virtual String   GetFilename() const;
  public: virtual Status   SetFileName(char const *fileName);
  public: virtual Status   SetFilename(String const &fileName);
  
  // Write Transactions
  //   Implemented in Persist.cpp
  public: virtual Status StartWrite ();
  public: virtual Status AbortWrite ();
  public: virtual Status AbortWrite (Status error);
  public: virtual Status EndWrite   ();
  public: virtual Status StartSequenceAnon  ();
  public: virtual Status StartSequenceAnon  (UInt32 seqLength);
  public: virtual Status StartSequenceNamed (char   const *name);
  public: virtual Status StartSequenceNamed (String const &name);
  public: virtual Status StartSequenceNamed (char   const *name, UInt32 seqLength);
  public: virtual Status StartSequenceNamed (String const &name, UInt32 seqLength);
  public: virtual Status StartSequenceInst  (char   const *instanceOf);
  public: virtual Status StartSequenceInst  (String const &instanceOf);
  public: virtual Status StartSequenceInst  (Class  const &instanceOf);
  public: virtual Status StartSequenceInst  (char   const *instanceOf, UInt32 seqLength);
  public: virtual Status StartSequenceInst  (String const &instanceOf, UInt32 seqLength);
  public: virtual Status StartSequenceInst  (Class  const &instanceOf, UInt32 seqLength);
  public: virtual Status AbortSequence ();
  public: virtual Status AbortSequence (Status error);
  public: virtual Status EndSequence ();
  
  // Writing Single Anonymous Values
  //   Implemented in PersistAnon.cpp
  public: virtual Status WriteRaw     (Byte         *value, UInt valueLen);
  public: virtual Status WriteLength  (UInt32        length);
  public: virtual Status WriteString  (String const &value);
  public: virtual Status WriteScalar  (Boolean       value);
  public: virtual Status WriteScalar  (UInt8         value);
  public: virtual Status WriteScalar  (UInt16        value);
  public: virtual Status WriteScalar  (UInt32        value);
  public: virtual Status WriteScalar  (UInt64        value);
  
  // Writing Multiple Anonymous Scalar Values
  //   Implemented in PersistAnon.cpp
  public: virtual Status WriteLengths (UInt32       *lengths, UInt32 length);
  public: virtual Status WriteStrings (String const *strings, UInt32 length);
  public: virtual Status WriteScalars (UInt8        *value,   UInt32 length);
  public: virtual Status WriteScalars (UInt16       *value,   UInt32 length);
  public: virtual Status WriteScalars (UInt32       *value,   UInt32 length);
  public: virtual Status WriteScalars (UInt64       *value,   UInt32 length);
  
  // Writing Single Named Scalar Values
  //   Implemented in PersistNamed.cpp
  public: virtual Status WriteRaw     (char   const *name, Byte         *value, UInt valueLen);
  public: virtual Status WriteLength  (char   const *name, UInt32        length);
  public: virtual Status WriteString  (char   const *name, String const &string);
  public: virtual Status WriteScalar  (char   const *name, Boolean       value);
  public: virtual Status WriteScalar  (char   const *name, UInt8         value);
  public: virtual Status WriteScalar  (char   const *name, UInt16        value);
  public: virtual Status WriteScalar  (char   const *name, UInt32        value);
  public: virtual Status WriteScalar  (char   const *name, UInt64        value);
  public: virtual Status WriteRaw     (String const &name, Byte         *value, UInt valueLen);
  public: virtual Status WriteLength  (String const &name, UInt32        length);
  public: virtual Status WriteString  (String const &name, String const &string);
  public: virtual Status WriteScalar  (String const &name, Boolean       value);
  public: virtual Status WriteScalar  (String const &name, UInt8         value);
  public: virtual Status WriteScalar  (String const &name, UInt16        value);
  public: virtual Status WriteScalar  (String const &name, UInt32        value);
  public: virtual Status WriteScalar  (String const &name, UInt64        value);
  
  // Writing Multiple Named Scalar Values
  //   Implemented in PersistNamed.cpp
  public: virtual Status WriteLengths (char   const *name, UInt32       *lengths, UInt32 length);
  public: virtual Status WriteStrings (char   const *name, String const *strings, UInt32 length);
  public: virtual Status WriteScalars (char   const *name, Boolean      *values,  UInt32 length);
  public: virtual Status WriteScalars (char   const *name, UInt8        *values,  UInt32 length);
  public: virtual Status WriteScalars (char   const *name, UInt16       *values,  UInt32 length);
  public: virtual Status WriteScalars (char   const *name, UInt32       *values,  UInt32 length);
  public: virtual Status WriteScalars (char   const *name, UInt64       *values,  UInt32 length);

  public: virtual Status WriteLengths (String const &name, UInt32       *lengths, UInt32 length);
  public: virtual Status WriteStrings (String const &name, String const *strings, UInt32 length);
  public: virtual Status WriteScalars (String const &name, Boolean      *values,  UInt32 length);
  public: virtual Status WriteScalars (String const &name, UInt8        *values,  UInt32 length);
  public: virtual Status WriteScalars (String const &name, UInt16       *values,  UInt32 length);
  public: virtual Status WriteScalars (String const &name, UInt32       *values,  UInt32 length);
  public: virtual Status WriteScalars (String const &name, UInt64       *values,  UInt32 length);
  
  // Writing Single Atomic Values
  //   Implemented in PersistValues.cpp
  public: virtual Status WriteAtomicRaw     (Byte         *value, UInt32 valueLen);
  public: virtual Status WriteAtomicLength  (UInt32        length);
  public: virtual Status WriteAtomicString  (String const &value);
  public: virtual Status WriteAtomic        (Boolean       value);
  public: virtual Status WriteAtomic        (UInt8         value);
  public: virtual Status WriteAtomic        (UInt16        value);
  public: virtual Status WriteAtomic        (UInt32        value);
  public: virtual Status WriteAtomic        (UInt64        value);
  
  // Writing Multiple Atomic Values
  //   Implemented in PersistValues.cpp
  public: virtual Status WriteAtomicLengths (UInt32       *lengths, UInt32 numValues);
  public: virtual Status WriteAtomicStrings (String const *strings, UInt32 numValues);  
  public: virtual Status WriteAtomics       (UInt8        *values,   UInt32 numValues);
  public: virtual Status WriteAtomics       (UInt16       *values,   UInt32 numValues);
  public: virtual Status WriteAtomics       (UInt32       *values,   UInt32 numValues);
  public: virtual Status WriteAtomics       (UInt64       *values,   UInt32 numValues);
    
  //
  // Protected Members
  //
  
  protected: FileStream stream;
  protected: UInt       storeType;
  
}; // ...Class Persist

} // ...Namespace BEFE

#endif // ...PERSIST_H