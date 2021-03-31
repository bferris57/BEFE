//!befe-publish inc
//----------------------------------------------------------------------
// File: CommandShell.h - Declarations for the CommandShell Class
//----------------------------------------------------------------------
//
// This class is a <Foundation Class> for command line parsing and
// execution.
//
// The distinction between this and Command is that CommandShell is that
// CommandShell is responsible for retrieving and dispatching input
// lines to the different Command handlers, whereas Command is
// responsible execution of a Command Line.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef COMMANDSHELL_H // COMMANDSHELL_H...
#define COMMANDSHELL_H

#include "Primitives.h"
#include "FileStream.h"
#include "CommandLayer.h"
#include "Arrays.h"
#include "Bcmd.h"

// This is nasty but that's what we have to do at the moment because
// we haven't quite genericised the xterm Class...
#ifdef BEFE_PortLinux
# define BEFE_DOXTERM
# include "Linux32xterm.h"
#endif
#ifdef BEFE_PortWindows
# define BEFE_DOXTERM
# include "Win32xterm.h"
#endif
// ...End of Nasty

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
// 
// Class CommandShell - A Command Shell itself
//

class CommandShell { // Class CommandShell...

  // Friend classes
  friend class Command;
  friend class CommandOS;
  
  // C++ Lifecycle
  public: CommandShell() {StartUp();};
  public: ~CommandShell() {ShutDown();};

  // BEFE Lifecycle
  public: Status        StartUp();
  public: Status        ShutDown();
  public: Status        Reset();
  public: Boolean       IsNull() const;
  public: Status        SetNull();
  public: Boolean       IsConsumable() const;
  public: CommandShell &Consumable() const;
  public: CommandShell &NotConsumable() const;
  
  // State query...
  public: Boolean IsInteractive();
  public: Boolean IsExiting();

  // List of known Command Processors (CommandLayer &)...
  public: Status        RegisterLayer(CommandLayer &layer);
  public: CommandLayer *FindLayer(String const &layerName);
  public: Status        FindLayer(String const &layerName, CommandLayer *&theLayer);
  
  // Command Shell stack maintenance
  public: Status PushLayer(String const &layerName, BcmdCommand &cmd);
  public: Status ExitTopLayer();
  public: Status ExitAllLayers();

  // Command Shell Input stack maintenance
  public: Status PushInputFile(String const &fileName);
  public: Status PushInputString(String const &string);

  // Input processing...
  public: Status MainLoop(BcmdCommand &cmd);
  public: Status ExecuteScript(String const &fileName, BcmdCommand &cmd);
  public: Status ExecuteString(String const &string);
  public: Status ExecuteCommand(Bcmd &bcmd, BcmdCommand &cmd);
  public: Status ExecuteCommand(BcmdCommand &cmd);
  
  //
  // Public Members
  //

  public: String  prompt;
  
  //
  // Private Members
  //

  public:    NamedUInts  layers;        // ◄── The UInt is Actually a (CommandLayer *)
  protected: UInts       stack;         // Stack of (CommandLayer *)
  protected: UInt        popCount;
  protected: Bcmds       inputs;        // Bcmd inputs

}; // ...Class CommandShell

//----------------------------------------------------------------------
//
// CommandShell BEFE Lifecycle...
//

            Status        StartUp       (CommandShell &rThis)                     ;
            Status        ShutDown      (CommandShell &rThis)                     ;
            Status        Reset         (CommandShell &rThis)                     ;
BEFE_INLINE Boolean       IsNull        (CommandShell const &rThis)               {return rThis.IsNull();}
BEFE_INLINE Status        SetNull       (CommandShell &rThis)                     {return rThis.SetNull();}
            Status        MoveFrom      (CommandShell &rThis, UInt8 const &rThat) ;
            Status        CopyFrom      (CommandShell &rThis, UInt8 const &rThat) ;
            Boolean       IsConsumable  (CommandShell const &rThis)               ;
            CommandShell &Consumable    (CommandShell &rThis)                     ;
            CommandShell &NotConsumable (CommandShell &rThis)                     ;
            UInt          Length        (CommandShell const &rThis)               ;
            UInt          Size          (CommandShell const &rThis)               ;
            

} // ...Namespace BEFE

#endif // ...COMMANDSHELL_H