//!befe-publish inc
//----------------------------------------------------------------------
// File: Help.h - Help Strings declarations
//----------------------------------------------------------------------
//
// This file contains declarations for all help strings
//
// NOTE: We expect these to be offloaded into help files are some point
//       in the near future.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef HELP_H // HELP_H...
#define HELP_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

Status Help(String const &subject);
Status Help(String const &fileName, String const &subject);

} // ...Namespace BEFE

#endif // ...HELP_H
