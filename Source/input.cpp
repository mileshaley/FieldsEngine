/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: input.cpp                                                           *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "input.h"
#include "imgui.h"
#include <iostream>
#include "window.h"
#include "context.h"

#if FE_USING_GLFW
#include "glfw/glfw3.h"
#elif FE_USING_SDL3
#include "SDL3/SDL.h"
#endif // FE_USING_SDL3



namespace fields_engine::input::detail {
	// GLFWkeyFun
	static void key_callback(GLFWwindow* win, int key, int scan_code, int action, int mods) {
		// Actions: 
		// 0 - released
		// 1 - triggered
		// 2 - repeat (no thanks)

		if (ImGui::GetIO().WantCaptureKeyboard) {
			return;
		} else if (action == GLFW_REPEAT) {
			return;
		}

		//std::cout << "key: '" << static_cast<char>(key) 
		//		  << "', scan code: " << scan_code << ", action: " 
		//		  << action << ", mods: " << mods << std::endl;

		context<input_manager>().report_key_action(key, action == 1);
	}
	// GLFWmousebuttonfun
	static void mouse_button_callback(GLFWwindow* win, int button, int action, int mods) {
		if (ImGui::GetIO().WantCaptureMouse) {
			return;
		}

	}
	// GLFWcursorposfun
	static void cursor_move_callback(GLFWwindow* win, double x, double y) {
		if (ImGui::GetIO().WantCaptureMouse) {
			return;
		}

	}
	// GLFWscrollfun
	static void scroll_callback(GLFWwindow* win, double x, double y) {
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


fields_engine::input_manager::input_manager() 
	: m_key_map()
{
}

bool fields_engine::input_manager::startup() {

	return true;
}

bool fields_engine::input_manager::shutdown() {

	return true;
}

void fields_engine::input_manager::tick(float dt) {
	for (auto it = m_key_map.begin(); it != m_key_map.end(); ++it) {
		record& rec = it->second;
		input_state& state = rec.state;
		if (state == input_state::triggered) {
			state = input_state::held;
		} else if (state == input_state::released) {
			state = input_state::none;
		}
	}
}

bool fields_engine::input_manager::is_key_held(int key_code) const {
	auto it = m_key_map.find(key_code);
	if (it != m_key_map.end()) {
		return it->second.state == input_state::held 
			|| it->second.state == input_state::triggered;
	}
	return false;
}

bool fields_engine::input_manager::was_key_triggered(int key_code) const {
	auto it = m_key_map.find(key_code);
	if (it != m_key_map.end()) {
		return it->second.state == input_state::triggered;
	}
	return false;
}

bool fields_engine::input_manager::was_key_released(int key_code) const {
	auto it = m_key_map.find(key_code);
	if (it != m_key_map.end()) {
		return it->second.state == input_state::released;
	}
	return false;
}

void fields_engine::input_manager::report_key_action(int key_code, bool triggered) {
	m_key_map[key_code].state = triggered ? input_state::triggered : input_state::released;
	/// TODO: trigger input events here
}


