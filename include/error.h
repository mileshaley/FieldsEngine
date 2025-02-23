/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: error.h                                                             *
\*~-------------------------------------------------------------------------~*/

#pragma once

/// TODO: Make our own assert
#include <cassert>

#ifdef _DEBUG
#define FE_ASSERT(cond, message) assert(cond && message)
#define FE_FAILED_ASSERT(message) assert(!message)
#else // ^^^ defined(_DEBUG) / !defined(_DEBUG) vvv
#define FE_ASSERT(cond, message)
#define FE_FAILED_ASSERT(message)
#endif // !defined(_DEBUG)