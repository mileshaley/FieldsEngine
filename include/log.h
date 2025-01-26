/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: log.h																 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include <ostream>

namespace fields_engine {

	class logger /*: std::ostream*/ {
		//logger& operator<< ()
		//void print() const;
	};

	extern logger log;
} // namespace fields_engine
