/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: editor_window.cpp													 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "editor_window.h"
#include "imgui.h"

fields_engine::editor_window::editor_window(string_view name, callback_fn callback) 
	: name_(name)
	, callback_(callback)
{}

bool fields_engine::editor_window::display() const {
	bool result = false;
	if (callback_) {
		if (begin_window()) {
			result = callback_();
		}
		end_window();
	}
	return result;
}

bool fields_engine::editor_window::begin_window() const {
	return ImGui::Begin(name_.c_str());
}

void fields_engine::editor_window::end_window() const {
	ImGui::End();
}

fe::editor_window::callback_fn fields_engine::editor_window::callback() const {
	return callback_;
}

void fields_engine::editor_window::callback(callback_fn newCallback) {
	callback_ = newCallback;
}
