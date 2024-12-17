/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: graphics.h																 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "glm/glm.hpp"

#ifdef DEBUG
#define FE_GL_VERIFY ::fields_engine::graphics::check_graphics_error(__FILE__, __LINE__);
#else // ^^^ defined(DEBUG) / !defined(DEBUG) vvv
#define FE_GL_VERIFY
#endif // ^^^ !defined(DEBUG)


namespace fields_engine {
	class mesh;
}

namespace fields_engine::graphics {

	void check_graphics_error(string_view file = "", int line = 0);
	void gl_error_callback(int error_code, const char* message);
	void resize_viewport(int width, int height);

	void clear_background();
	void clear_background(glm::vec4 const& color);

	namespace impl {
		void initialize();
	} // namespace impl

} // namespace fields_engine::graphics
