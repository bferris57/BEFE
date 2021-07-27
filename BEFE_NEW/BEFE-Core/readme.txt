#-----------------------------------------------------------------------
#-----------------------------------------------------------------------
#
# File: readme.txt - Description of this directory and its contents
#
# This is intended to provide a quick overview of what's in this
# directory, how it's structured, and what it's for.
#
#-----------------------------------------------------------------------
#Copyright (c) 2011, Bruce Ferris (UK), All rights reserved
#-----------------------------------------------------------------------
#-----------------------------------------------------------------------

This directory contains all of Bruce Ferris's current C/C++ code.

It is currently checked in Assembla's SVN service for user "bferris57"
under the project "bferris", and directory "c++" (Bruce's Java code is
under the "bferris" directory there and is described elsewhere.

Directory: C++
--------------

This is the high level directory for all the source code.  It contains
the following...

  readme.txt  - This file
  main.cpp    - "Main program" for testing purposes
  trythis.cpp - Temporary and volatile non-essential place to try things

  _tempcode   - Directory for temporary or incomplete code
  array       - Directory of array related source
  core        - Directory of core, foundation, classes - Everything hinges on this
  file        - Directory of File related source
  format      - Directory of formatting classes and functions
  number      - Directory of Number data types
  platform    - Directory of port specific code (see below)
  repo        - Directory of repository speciic code (early stages)
  seq         - Directory of sequence related code (foundation classes)
  set         - Directory of Set related source (early stages)
  string      - Directory of String class related source
  test        - Directory of self-test classes
  utils       - Directory of utility classes
  word        - Directory of word/dictionary related source (semi-obsolete)

  Directory: C++/platform
  -------------------
  
  This directory contains port/platform specific source.  It contains
  the following...
  
    nokiaQt - Nokia Qt platform specific source
    win32   - Microsoft 32-bit Windows platform specific source
  	win64   - Microsoft 64-bit Windows platform specific source (none yet)

Build Notes
-----------

Nothing much more except...

  1) Always include c++, and all sub directories so all headers are
     available without directory names in the include statement.
	 
  2) Still have to come up with ideas on port specific building, am currently
     only doing win32 so it hasn't been a bother yet.
