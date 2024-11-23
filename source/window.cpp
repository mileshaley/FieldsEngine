/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: window.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "window.h"

#include "glfw/glfw3.h"

fields_engine::window::window(GLFWwindow* win_handle)
	: handle(win_handle)
{}

bool fields_engine::window::is_running() const {
	return !glfwWindowShouldClose(handle);
}
