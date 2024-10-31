/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: graphics.h																 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "glm/glm.hpp"

#define GL_CHECK ::fields_engine::graphics::check_graphics_error(__FILE__, __LINE__);


namespace fields_engine::graphics {

	void check_graphics_error(string_view file = "", int line = 0);
	void gl_error_callback(int errorCode, const char* message);
	void resize_viewport(int width, int height);

	void clear_background();
	void clear_background(glm::vec4 const& color);

	namespace detail {
		void initialize();
	}

} // namespace fields_engine::graphics
