/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: input.cpp                                                           *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "input.h"
#include "imgui/imgui.h"
#include <iostream>
#include "window_handle.h"
#include "context.h"
#include "application.h"

#if EDITOR
#include "editor_manager.h"
#endif // EDITOR

#if FE_USING_GLFW
#include "glfw/include/GLFW/glfw3.h"
#elif FE_USING_SDL3
#include "SDL3/SDL.h"
#endif // FE_USING_SDL3


static constexpr int mouse_code_offset = GLFW_KEY_LAST + 1;

namespace fields_engine::input::impl {
	// GLFWkeyFun
	static void key_callback(GLFWwindow* win, int key, int scan_code, int action, int mods) {
		(void)scan_code;
		(void)mods;

		// Actions: 
		// 0 - released
		// 1 - triggered
		// 2 - repeat (no thanks)
		if (action == GLFW_REPEAT) {
			return;
		}
		application& app = context<application>();
#if EDITOR
		if (app.ref_editor().is_capturing_keyboard()) {
			return;
		}
#endif // EDITOR
		app.ref_input_manager().report_key_action(key, action == 1);
		//std::cout << "key: '" << static_cast<char>(key) 
		//		  << "', scan code: " << scan_code << ", action: " 
		//		  << action << ", mods: " << mods << std::endl;
	}

	// GLFWmousebuttonfun
	static void mouse_button_callback(GLFWwindow* win, int button, int action, int mods) {
		application& app = context<application>();
#if EDITOR
		if (app.ref_editor().is_capturing_mouse()) {
			return;
		}
#endif // EDITOR
		app.ref_input_manager().report_mouse_action(button, action == 1);
	}

	// GLFWcursorposfun
	static void cursor_pos_callback(GLFWwindow* win, double x, double y) {
		application& app = context<application>();
#if EDITOR
		if (app.ref_editor().is_capturing_mouse()) {
			return;
		}
#endif // EDITOR
		app.ref_input_manager().report_mouse_pos({x, y});
	}
	// GLFWscrollfun
	static void scroll_callback(GLFWwindow* win, double x, double y) {
		application& app = context<application>();
#if EDITOR
		if (app.ref_editor().is_capturing_mouse()) {
			return;
		}
#endif // EDITOR
		app.ref_input_manager().report_mouse_scroll({ x, y });
	}

	void initialize_callbacks(window_handle& win) {
		glfwSetKeyCallback(win.handle, key_callback);
		glfwSetMouseButtonCallback(win.handle, mouse_button_callback);
		glfwSetCursorPosCallback(win.handle, cursor_pos_callback);
		glfwSetScrollCallback(win.handle, scroll_callback);
	}
} // namespace fields_engine::input::detail


fields_engine::input_manager::input_manager()
	: m_button_records()
	, m_last_mouse_pos(0, 0)
	, m_mouse_pos(0, 0)
	, m_mouse_scroll(0, 0)
{
}

bool fields_engine::input_manager::startup() {

	return true;
}

bool fields_engine::input_manager::shutdown() {

	return true;
}

void fields_engine::input_manager::tick(float dt) {
	for (auto it = m_button_records.begin(); it != m_button_records.end(); ++it) {
		record& rec = it->second;
		input_state& state = rec.state;
		if (state == input_state::triggered) {
			state = input_state::held;
		} else if (state == input_state::released) {
			state = input_state::none;
		}
	}
	m_last_mouse_pos = m_mouse_pos;
	m_mouse_scroll = vec2{ 0, 0 };
}

bool fields_engine::input_manager::is_button_held(int key_code, bool on_mouse) const {
	auto it = m_button_records.find(key_code + (on_mouse ? mouse_code_offset : 0));
	if (it != m_button_records.end()) {
		return it->second.state == input_state::held 
			|| it->second.state == input_state::triggered;
	}
	return false;
}

bool fields_engine::input_manager::was_button_triggered(int key_code, bool on_mouse) const {
	auto it = m_button_records.find(key_code + (on_mouse ? mouse_code_offset : 0));
	if (it != m_button_records.end()) {
		return it->second.state == input_state::triggered;
	}
	return false;
}

bool fields_engine::input_manager::was_button_released(int key_code, bool on_mouse) const {
	auto it = m_button_records.find(key_code + (on_mouse ? mouse_code_offset : 0));
	if (it != m_button_records.end()) {
		return it->second.state == input_state::released;
	}
	return false;
}

bool fields_engine::input_manager::did_mouse_move() const {
	return m_mouse_pos != m_last_mouse_pos;
}

fe::vec2 fields_engine::input_manager::get_delta_mouse_move() const {
	return m_mouse_pos - m_last_mouse_pos;
}

bool fields_engine::input_manager::did_mouse_scroll() const {
	return m_mouse_scroll != vec2(0, 0);
}

fe::vec2 fields_engine::input_manager::get_delta_mouse_scroll() const {
	return m_mouse_scroll;
}

void fields_engine::input_manager::report_key_action(int key_code, bool triggered) {
	m_button_records[key_code].state = triggered ? input_state::triggered : input_state::released;
	/// TODO: trigger input events here
}

void fields_engine::input_manager::report_mouse_action(int button_code, bool triggered) {
	m_button_records[button_code + mouse_code_offset].state = triggered ? input_state::triggered : input_state::released;
	/// TODO: trigger input events here
}

void fields_engine::input_manager::report_mouse_pos(vec2 delta) {
	m_mouse_pos = delta;
}

void fields_engine::input_manager::report_mouse_scroll(vec2 delta) {
	m_mouse_scroll = delta;
}
