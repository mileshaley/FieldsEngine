/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: input.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "window.h"

namespace fields_engine::input {

	namespace detail {
		void initialize_callbacks(window& windowHandle);
	} // namespace detail

} // namespace fields_engine::input