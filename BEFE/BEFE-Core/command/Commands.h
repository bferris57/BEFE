//----------------------------------------------------------------------
// File: CommandS.h - Declarations for layerless Command Functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef COMMANDS_H // COMMANDS_H...

#include "CommandLayer.h"

namespace BEFE { // Namespace BEFE...

Status Command_Analyse(CommandLayer *layer, BcmdCommand &cmd);
Status Command_EncryptFile(CommandLayer *layer, BcmdCommand &cmd);
Status Command_DecryptFile(CommandLayer *layer, BcmdCommand &cmd);
Status Command_CreateKeyFile(CommandLayer *layer, BcmdCommand &cmd);
Status Command_UseKeyFile(CommandLayer *layer, BcmdCommand &cmd);
Status Command_Build(CommandLayer *layer, BcmdCommand &cmd);

} // ...Namespace BEFE

#endif // ...COMMANDS_H