#ifndef __DEFS__
#define __DEFS__

#include "flags.h"

#if PRINTDOM
static const bool PRINT_DOM = true;
#else
static const bool PRINT_DOM = false;
#endif

#if PRINTMERGE
static const bool PRINT_MERGE = true;
#else
static const bool PRINT_MERGE = false;
#endif

#if PRINTLOOPS
static const bool PRINT_LOOPS = true;
#else
static const bool PRINT_LOOPS = false;
#endif

#if PRINTPRE
static const bool PRINT_PRE = true;
#else
static const bool PRINT_PRE = false;
#endif

#if PRINTMOVING
static const bool PRINT_MOVING = true;
#else
static const bool PRINT_MOVING = false;
#endif

#endif /* __DEFS__ */
