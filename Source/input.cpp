/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: input.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "input.h"
#include "glfw/glfw3.h"
#include <iostream>
#include "imgui.h"

namespace fields_engine::input::detail {
	// GLFWkeyFun
	static void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
		// actions: 
		// 0 - released
		// 1 - triggered
		// 2 - repeat (no thanks)

		if (ImGui::GetIO().WantCaptureKeyboard) {
			return;
		} else if (action == GLFW_REPEAT) {
			return;
		}

		std::cout << "key: '" << static_cast<char>(key) 
				  << "', scan code: " << scanCode << ", action: " 
				  << action << ", mods: " << mods << std::endl;
	}
	// GLFWmousebuttonfun
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		if (ImGui::GetIO().WantCaptureMouse) {
			return;
		}

	}
	// GLFWcursorposfun
	static void cursor_move_callback(GLFWwindow* window, double x, double y) {
		if (ImGui::GetIO().WantCaptureMouse) {
			return;
		}

	}
	// GLFWscrollfun
	static void scroll_callback(GLFWwindow* window, double x, double y) {
		if (ImGui::GetIO().WantCaptureMouse) {
			return;
		}

	}


	void initialize_callbacks(window& win) {
		glfwSetKeyCallback(win.handle, key_callback);
		glfwSetMouseButtonCallback(win.handle, mouse_button_callback);
		glfwSetCursorPosCallback(win.handle, cursor_move_callback);
		glfwSetScrollCallback(win.handle, scroll_callback);
	}

} // namespace fields_engine::input::detail



