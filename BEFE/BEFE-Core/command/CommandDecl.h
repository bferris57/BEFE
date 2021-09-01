//!befe-publish inc
//----------------------------------------------------------------------
// File: CommandDecl.h - Declarations for the CommandDecl and related Classes/Types
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef COMMANDDECL_H // COMMANDDECL_H...

#include "Primitives.h"
#include "Array.h"
#include "BcmdCommand.h"

namespace BEFE { // Namespace BEFE...

// External references
class CommandLayer;

//
// CommandHandler - Pointer to a function that handles a single non-prompting command
//

typedef Status (*CommandHandler)(CommandLayer &layer, BcmdCommand &cmd);

//
// CommandSpec - Staging point for a CommandDecl (see below) used by CommandLayers
//               to initialise a hard-coded list of command declarations
//

typedef struct {
  char const     *minExp;
  CommandHandler  handler;
  Boolean         insensitive;
} CommandSpec;

//
// CommandDecl - A single Command Declaration
//

typedef struct CommandDecl {
  
  // Members
  String         name;
  CommandHandler handler;

  // C++ Lifecycle
  CommandDecl();
  ~CommandDecl();
  CommandDecl(CommandDecl const &that);
  CommandDecl &operator = (CommandDecl const &that);

  // BEFE Lifecycle
  Status       StartUp();
  Status       ShutDown();
  Boolean      IsNull() const;
  Status       SetNull();
  Boolean      IsEmpty() const;
  Status       SetEmpty();
  Status       MoveFrom(CommandDecl const &that);
  Status       CopyFrom(CommandDecl const &that);
  Boolean      IsConsumable() const;
  CommandDecl &Consumable() const;
  CommandDecl &NotConsumable() const;
  UInt         Length() const;
  UInt         Size() const;
  
} CommandDecl;

typedef ArrayValue<CommandDecl> CommandDecls;

} // ...Namespace BEFE
#endif // ...COMMANDDECL_H
