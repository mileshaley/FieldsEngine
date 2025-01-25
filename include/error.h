/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: error.h                                                             *
\*~-------------------------------------------------------------------------~*/

#pragma once
#include <stdexcept>

/// TODO: STL_ASSERT here is a placeholder, change it

#define FE_ASSERT(cond, message) _STL_ASSERT(cond, message);
#define FE_FAILED_ASSERT(message) _STL_ASSERT(false, message);

namespace fields_engine {

	using std::runtime_error;

} // namespace fields_engine
