//!befe-publish inc
//----------------------------------------------------------------------
// File: CommandLayer.h - Declarations for the CommandLayer Class
//----------------------------------------------------------------------
//
// The CommandLayer class, for want of a better term, implements a
// command sub-shell.  The CommandLayer Class itself does not do any
// operations other than those that are common to all sub-shells
// or "Layers".
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef COMMANDLAYER_H // COMMANDLAYER_H...
#define COMMANDLAYER_H

#include "Primitives.h"
#include "Array.h"
#include "CommandDecl.h"

namespace BEFE { // Namespace BEFE...

// External Class references...
class CommandShell;

//----------------------------------------------------------------------
//
// Class: CommandLayer - Command sub-shell processor
//
// This class is responsible for command processing and the management
// of a closely related set of input commands.
//
// Note: The CommandShell manages these layers and does the actual I/O
//       and other processing.  It is only up to each Layer implementation
//       do register and handle it's own affairs.
//

class CommandLayer {

  // Friends
  friend class CommandShell;

  // C++ Lifecycle
  public: CommandLayer();
  public: virtual ~CommandLayer();

  // BEFE Lifecycle
  public: virtual Status StartUp();
  public: virtual Status ShutDown();

  // CommandShell knowledge
  public: virtual CommandShell *GetShell();
  public: virtual Status        SetShell(CommandShell *theShell);
  public: virtual Status        SetIndent(String const &indent);
  public: virtual Status        SetHelpFile(String const &pHelpFileName);

  // Command Flow
  public: virtual Status Enter(BcmdCommand &cmd);
  public: virtual Status Execute(BcmdCommand &cmd);
  public: virtual Status Leave();

  // Header/Trailer management
  public: virtual Status DoHeader();
  public: virtual Status DoTrailer();

  // Command helper functions...
  public: virtual void Msg(char const *msg);
  public: virtual void Note(char const *msg);

  // Command handler registry
  protected: virtual Status RegisterHandlers();
  protected: virtual Status RegisterHandlers(CommandSpec const *specs);
  protected: virtual Status RegisterHandler(CommandSpec const &spec);
  protected: virtual Status RegisterHandler(CommandDecl const &decl);
  public:    virtual Status RegisterAdditionalHandler(CommandSpec const &spec);
  public:    virtual Status RegisterAdditionalHandler(CommandDecl const &decl);

  // Settings
	public:    virtual String GetPrompt() const;
  public:    virtual Status SetPrompt(String const &str);
  
  //
  // Public members
  //

  public: String        name;        // CommandLayer Name (may be a MinExp Spec)
  public: CommandShell *shell;       // CommandShell we're registered with
  public: String        prompt;      // We tell the CommandShell this
  public: String        indent;      // CommandShell sets this
  public: String        helpFileName;// Help file name
  public: Status        returnCode;  // ***I forget why we want this***

  //
  // Protected members
  //

  // Command handlers
  protected: Boolean      inheritCommands; // true="Inherits commands from higher layers"
  protected: CommandDecls commands;        // Array of distinct Commands it handles

};

//----------------------------------------------------------------------
//
// Handy BEFE declarations
//

typedef ArrayValue<CommandLayer> CommandLayers;

} // ...Namespace BEFE

#endif // ...COMMANDLAYER_H
