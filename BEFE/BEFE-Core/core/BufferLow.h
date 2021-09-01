//----------------------------------------------------------------------
// File: BufferLow.h - Declarations for the BufferLow Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BUFFERLOW_H // BUFFERLOW_H...
#define BUFFERLOW_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

// Buffer headers
struct BufHead8  {UInt8  data[4];};
struct BufHead16 {UInt16 data[4];};
struct BufHead32 {UInt32 data[4];};
struct BufHead64 {UInt64 data[4];};

union BufHeads {
  BufHead8  buf8;
  BufHead16 buf16;
  BufHead32 buf32;
  BufHead64 buf64;
};

//
// BufferLow class
//

class BufferLow { // Class BufferLow...

//
// Public static (class) methods
//

  public: static Status New          (UInt initBytes, UInt incBytes, BufferLow *&newbuf);
  public: static Status Delete       (BufferLow *&selfp);
  public: static Status Consumable   (BufferLow *&selfp);
  public: static Status NotConsumable(BufferLow *&selfp);
  public: static Status SetSize      (BufferLow *&selfp, UInt newBytes);
  public: static Status Resize       (BufferLow *&selfp, UInt newBytes);
  public: static Status CopyFrom     (BufferLow *&selfp, BufferLow *&thatp);

  // Manipulating Byte contents
  public: static Status GetBytes     (BufferLow *&selfp, UInt bindex, Byte *thebytes, UInt bufMaxSize, UInt &usedSize);
  public: static Status SetBytes     (BufferLow *&selfp, UInt bindex, Byte *thebytes, UInt bufMaxSize, UInt &usedSize);
  public: static Status AppendBytes  (BufferLow *&selfp, Byte *thebytes, UInt numBytes);
  public: static Status InsertBytes  (BufferLow *&selfp, UInt bindex, Byte *theBytes, UInt numBytes);
  public: static Status RemoveBytes  (BufferLow *&selfp, UInt bindex, UInt numBytes);
  public: static Status ReplaceBytes (BufferLow *&selfp, UInt bindef, UInt numBytes, Byte *newBytes, UInt newSize);

  //
  // Public instance methods
  //

  // Buffer info
  public: UInt    Size() const;                 // Get size in bytes
  public: UInt    Length() const;               // Get length in bytes (same thing as size for this class)
  public: Boolean IsConsumable() const;         // "Is this BufferLow consumable?"
  
  public: UInt    GetAllocBytes() const;        // Get allocated bytes
  public: UInt    GetIncrementBytes() const;    // Get byte increment count
  public: UInt    GetUsedBytes() const;         // Get number of used bytes
  public: Status  SetUsedBytes(UInt newSize);   // Set number of used bytes
  public: Byte   *GetBuf() const;               // Get pointer to the bytes (transient)

  //
  // !!!NOTE: The following has to be completely redone when we implement on
  //          big-endian processors
  //
  
  public: inline UInt GetBufSize() const {
                        return ((BufHead32 *)this)->data[0] >> 3;
                        };
                        
  public: inline void SetBufSize(UInt uint) const {
                        ((BufHead32 *)this)->data[0] = (uint << 3) | 0x04;
                        };
                        
  public: inline UInt GetIsConsumable() const {
                        return ((BufHead32 *)this)->data[1] & 0x07;
                        };
                        
  public: inline void SetIsConsumable(UInt val) const {
                        BufHead32 *head;
                        head = (BufHead32 *)this;
                        head->data[1] = (head->data[1] & 0xfffffff8) | (val & 0x07);
                        };
                        
  public: inline UInt GetInitBytes() const {
                         return ((BufHead32 *)this)->data[1] >> 3;
                         };
                         
  public: inline void SetInitBytes(UInt uint) const {
                        BufHead32 *head;
                        head = (BufHead32 *)this;
                        head->data[1] = ((uint & 0x1fffffff) << 3) | (head->data[1] & 0x07);
                        };
                        
  public: inline UInt GetIncBytes() const {
                        return ((BufHead32 *)this)->data[2] & 0x1fffffff;
                        };
                        
  public: inline void SetIncBytes(UInt uint) const {
                        BufHead32 *head;
                        head = (BufHead32 *)this;
                        head->data[2] = uint & 0x1fffffff;
                        };
                        
  public: inline UInt GetCurBytes() const {
                        return ((BufHead32 *)this)->data[3] & 0x1fffffff;
                        };
  
  public: inline void SetCurBytes(UInt uint) const {
                        BufHead32 *head;
                        head = (BufHead32 *)this;
                        head->data[3] = uint & 0x1fffffff;
                        };
                        
  //
  // Public instance members
  //
  
  public: BufHeads header;
  
}; // ...Class BufferLow
  
} // ...Namespace BEFE

#endif // ...BUFFERLOW_H