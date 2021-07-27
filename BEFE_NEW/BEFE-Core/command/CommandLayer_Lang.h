//!befe-publish inc
//----------------------------------------------------------------------
// File: CommandLayer_Lang.h - Declarations for the CommandLayer_Lang Class
//----------------------------------------------------------------------
//
// This class is a Command handler for new generic Lexical Analysis
// command execution we're currently working on to replace the old
// Lex stuff.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef COMMANDLAYER_LANG_H // COMMANDLAYER_LANG_H...
#define COMMANDLAYER_LANG_H

#include "Primitives.h"
#include "CommandLayer.h"
#include "Lex.h"
#include "LexWorkspace.h"
#include "LexLanguage.h"
#include "LexStream.h"
#include "Match.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// CommandLayer_Lang - BEFE-Lex CommmandLayer_Lang Class...
//

class CommandLayer_Lang : public CommandLayer {

  // C++ Lifecycle
  public: CommandLayer_Lang();
  public: virtual ~CommandLayer_Lang();

  // BEFE Lifecycle
  public: virtual Status StartUp();
  public: virtual Status ShutDown();

  // Single command execution
  public: virtual Status Enter(BcmdCommand &cmd);

  // Command handler registry
  public: virtual Status RegisterHandlers();

  // Header/Trailer management
  //public: virtual Status DoHeader();
  //public: virtual Status DoTrailer();

  // Settings
  public: virtual Status SetPrompt(String const &str);
  
  // Internal Methods
  public: Status _InitLex();
  
  //
  // Protected members
  //

  public: Lex          lex;
  public: LexWorkspace ws;
  public: LexLanguage  curLang;   // Current Language (for parsing)  
  public: LexStream    lexStream;
  public: Match        match;
  
};

//----------------------------------------------------------------------
//
// CommandWorkspace Command Handlers...
//

Status CommandLayer_Lang_Help      (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_EnterLang (CommandLayer_Lang *layer, BcmdCommand &cmd);

Status CommandLayer_Lang_Show      (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_New       (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_Open      (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_Close     (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_Display   (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_Block     (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_CurLang   (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_Lex       (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_Expr      (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_MatchType (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_Pattern   (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_Match     (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_MinExp    (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_Test      (CommandLayer_Lang *layer, BcmdCommand &cmd);
Status CommandLayer_Lang_Unknown   (CommandLayer_Lang *layer, BcmdCommand &cmd);

} // ...Namespace BEFE

#endif // ...COMMANDLAYER_LANG_H
