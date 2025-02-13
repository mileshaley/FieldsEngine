/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: error.h                                                             *
\*~-------------------------------------------------------------------------~*/

#pragma once

/// TODO: Implement our own assert and not use _STL_ASSERT

#define FE_ASSERT(cond, message) _STL_ASSERT(cond, message);
#define FE_FAILED_ASSERT(message) _STL_ASSERT(false, message);