///
//  UVcoord.h
//
//  Representation of 2D texture coordinates.
//
//  Author:  Warren R. Carithers
//  Date:    2018/11/12 12:03:27
///

#ifndef _UVCOORD_H_
#define _UVCOORD_H_

// re-use the Tuple declaration

#include "Tuple.h"

///
// Information pertaining to (U,V) texture coordinates
///

typedef Tuple UVcoord;

// mapping of (u,v) to Tuple data members
#define u    x
#define v    y

#endif
