/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: graphics.h																 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#define GL_CHECK ::fields_engine::graphics::check_graphics_error(__FILE__, __LINE__);


namespace fields_engine::graphics {

	static void check_graphics_error(string_view file = "", int line = 0);
	static void gl_error_callback(int errorCode, const char* message);

	namespace detail {
		static void initialize();
	}

} // namespace fields_engine::graphics
