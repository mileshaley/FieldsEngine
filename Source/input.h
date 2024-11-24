/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: input.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "window.h"

namespace fields_engine {



	namespace input::detail {
		void initialize_callbacks(window& win);
	} // namespace detail

} // namespace fields_engine::input