/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: vis.h																 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "glm/glm.hpp"

#ifdef DEBUG
#define VIS_VERIFY ::fields_engine::vis::check_error(__FILE__, __LINE__);
#else // ^^^ defined(DEBUG) / !defined(DEBUG) vvv
#define VIS_VERIFY
#endif // ^^^ !defined(DEBUG)

namespace fields_engine {
	class mesh;
}

namespace fields_engine::vis {

	void check_error(string_view file = "", int line = 0);
	void gl_error_callback(int error_code, const char* message);
	void resize_viewport(int width, int height);

	void reset_frame();
	void set_background_color(glm::vec4 const& color);

	namespace impl {
		void initialize();
	} // namespace impl

} // namespace fields_engine::vis
