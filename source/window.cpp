/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: window.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "window.h"
#include "glfw/glfw3.h"

bool fields_engine::window::is_running() const {
	return !glfwWindowShouldClose(handle);
}
