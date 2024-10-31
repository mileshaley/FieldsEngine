#include "precompiled.h"
#include "input.h"
#include "glfw/glfw3.h"
#include <iostream>


namespace fields_engine::input::detail {
	
	// GLFWkeyFun
	static void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
		std::cout << key << scanCode << action << action << mods << std::endl;
	}
	// GLFWcursorposfun
	static void cursor_move_callback(GLFWwindow* window, double x, double y) {

	}
	// GLFWmousebuttonfun
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	}
	// GLFWscrollfun
	static void scroll_callback(GLFWwindow* window, double x, double y) {

	}


	void initialize_callbacks(window& win) {
		glfwSetKeyCallback(win.handle, key_callback);
		glfwSetCursorPosCallback(win.handle, cursor_move_callback);
		glfwSetMouseButtonCallback(win.handle, mouse_button_callback);
		glfwSetScrollCallback(win.handle, scroll_callback);
	}

} // namespace fields_engine::input::detail



