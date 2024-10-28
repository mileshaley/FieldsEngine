#include "precompiled.h"
#include "input.h"
#include "glfw/glfw3.h"



namespace fields_engine::input::detail {
	
	// GLFWkeyFun
	static void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
		
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


	void initialize_callbacks(GLFWwindow* windowHandle) {
		glfwSetKeyCallback(windowHandle, key_callback);
		glfwSetCursorPosCallback(windowHandle, cursor_move_callback);
		glfwSetMouseButtonCallback(windowHandle, mouse_button_callback);
		glfwSetScrollCallback(windowHandle, scroll_callback);
	}

} // End fields_engine::input::detail



