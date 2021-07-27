#!/usr/bin/python
#coding=utf8
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================
global verbose0
global verbose1
global verbose2

Y = YES = Yes = yes = yep  = True
N = NO  = No  = no  = nope = False

debug       = N
verboseAll  = N # To set them ALL on  (overridden by verboseNone)
verboseNone = N # To set them ALL off (overrides verboseAll)
verbose     = N # Verbose
verbose0    = N
verbose1    = N
verbose2    = N
verbose3    = N
verbose4    = N
verbose5    = N
verbose6    = N

if verboseNone:
  verbose = verbose1 = verbose2 = verbose3 = verbose4 = verbose5 = verbose6 = False
elif verboseAll:
  verbose = verbose1 = verbose2 = verbose3 = verbose4 = verbose5 = verbose6 = True

# Specialisations...
processOpTOSdebug = N