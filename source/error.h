/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: error.h                                                             *
\*~-------------------------------------------------------------------------~*/

#pragma once
#include <stdexcept>

// TODO: )STL_VERIFY here is a placeholder, change it

#define FE_ASSERT(cond, message) _STL_VERIFY(cond, message);
#define FE_FAILED_ASSERT(message) _STL_VERIFY(false, message);

namespace fields_engine {

	using std::runtime_error;

} // namespace fields_engine
