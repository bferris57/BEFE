//!befe-publish inc
//----------------------------------------------------------------------
// File: CommandLayer_BEFE.h - Declarations for the CommandLayer_BEFE Layer Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef COMMANDLAYER_BEFE_H // COMMANDLAYER_BEFE_H...
#define COMMANDLAYER_BEFE_H

#include "Primitives.h"
#include "Sequence.h"
#include "CommandLayer.h"
#include "CommandLayer_OS.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class: CommandLayer_BEFE - BEFE Main Command Processor
//

class CommandLayer_BEFE : public CommandLayer { // Class CommandLayer_BEFE...

  // C++ Lifecycle
  public: CommandLayer_BEFE();

  // BEFE Lifecycle
  public: virtual Status StartUp();

  // Command handler registry
  public: virtual Status RegisterHandlers();

}; // ...Class CommandLayer_BEFE

//----------------------------------------------------------------------
//
// CommandLayer_BEFE Command handlers...
//

Status CommandLayer_BEFE_Exit        (CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_BEFE_Finish      (CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_BEFE_Help        (CommandLayer *layer, BcmdCommand &cmd);

Status CommandLayer_BEFE_Switches    (CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_BEFE_DateTime    (CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_BEFE_Set         (CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_BEFE_RefreshEnv  (CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_BEFE_ShowArgs    (CommandLayer *layer, BcmdCommand &cmd);

Status CommandLayer_BEFE_Run         (CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_BEFE_Build       (CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_BEFE_UUID        (CommandLayer *layer, BcmdCommand &cmd);

Status CommandLayer_BEFE_Tests       (CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_BEFE_OS          (CommandLayer *layer, BcmdCommand &cmd);

Status CommandLayer_BEFE_MemoryStats (CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_BEFE_StreamStats (CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_BEFE_Statistics  (CommandLayer *layer, BcmdCommand &cmd);

Status CommandLayer_BEFE_Echo        (CommandLayer *layer, BcmdCommand &cmd);
Status CommandLayer_BEFE_ClearScreen (CommandLayer *layer, BcmdCommand &cmd);

} // ...Namespace BEFE

#endif // ...COMMANDLAYER_BEFE_H
