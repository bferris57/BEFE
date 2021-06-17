//!befe-publish inc
//----------------------------------------------------------------------
// File: Coord.h - Primitive Coord and related data type declarations
//----------------------------------------------------------------------
//
// This header provides the header for various commonly implemented
// Integer dimensional coordinates.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef COORD_H // COORD_H...
#define COORD_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Dimensional Coordinate structs
//

struct Coord1d {
  Int x;
};

struct Coord2d {
	BEFE_inline Coord2d()               { x = y = NaN; }
	BEFE_inline Coord2d(Int ix, Int iy) {x = ix; y = iy;}
  Int x;
  Int y;
};

struct Coord3d {
  Int x;
  Int y;
  Int z;
};

struct Coord4d {
  Int x;
  Int y;
  Int z;
  Int t;  // <-- Einstein's time in "Space Time"!!!!
};

//----------------------------------------------------------------------
//
// Common Typedefs
//

typedef Int     Index;
typedef UInt    Subscript;
typedef Coord2d Coord;

} // ...Namespace BEFE

#endif // ...COORD_H