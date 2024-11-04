/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: window.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

struct GLFWwindow;

namespace fields_engine {
	class window {
	public:
		
		bool is_running() const;

		GLFWwindow* handle;
	};

} // namespace fields_engine


