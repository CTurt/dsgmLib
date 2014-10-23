//
//  trig.h : Basic trigonometry routines.
//
//! \file trig.h
//! \author J Vijn
//! \date 20080130 - 20080210
//

/*
	dsgmLib adaptations:
	
	- Only atan2Lerp was needed, all other trigonometry functions are
	already present in libnds and were removed from this library
	
	- Upgraded the code from C++ to C
*/

#pragma once

// These things may already be present in other headers.
typedef unsigned int uint;

#ifndef countof
#define countof(array)	( sizeof(array)/sizeof(array[0]) )
#endif

#ifndef ALIGN
#define ALIGN(n)	__attribute__((aligned(n)))
#endif

// --------------------------------------------------------------------
// CONSTANTS
// --------------------------------------------------------------------

#define BRAD_PI_SHIFT 14
#define BRAD_PI (1<<BRAD_PI_SHIFT)
#define BRAD_HPI (BRAD_PI/2)
#define BRAD_2PI (BRAD_PI*2)

// --------------------------------------------------------------------
// PROTOTYPES 
// --------------------------------------------------------------------

uint atan2Lerp(int x, int y);
