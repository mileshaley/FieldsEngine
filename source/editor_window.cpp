/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: editor_window.cpp													 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "editor_window.h"
#include "imgui.h"

fields_engine::editor_window::editor_window(string_view name, callback_fn callback, editor_icon icon)
	: name_(name)
	, strID_(name)
	, callback_(callback)
	, icon_(icon)
{
	string_view iconStrv(icon);
	strID_ += "###";
	if (!iconStrv.empty()) {
		strID_ += iconStrv;
		strID_ += " ";
	}
	strID_ += name;
}

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
	return ImGui::Begin(strID_.c_str());
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
