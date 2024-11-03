/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: editor_window.cpp													 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "editor_window.h"
#include "imgui.h"

fields_engine::editor_window::editor_window(string_view name, callback_t const& callback, editor_icon icon)
	: name_(name)
	, strID_()
	, callback_(callback)
	, icon_(icon)
	, open_(true)
{
	string_view iconStrv(icon);
	if (!iconStrv.empty()) {
		strID_ += iconStrv;
		strID_ += " ";
	}
	strID_ += name;
	strID_ += "###";
	strID_ += name;
}

bool fields_engine::editor_window::display() {
	return open_ && force_display();
}

bool fields_engine::editor_window::force_display() {
	bool result = false;
	if (callback_) {
		if (begin_window()) {
			result = callback_();
		}
		end_window();
	}
	return result;
}

bool fields_engine::editor_window::begin_window() {
	return ImGui::Begin(strID_.c_str(), &open_);
}

void fields_engine::editor_window::end_window() const {
	ImGui::End();
}

void fields_engine::editor_window::menu_item() {
	if (ImGui::MenuItem(strID_.c_str(), nullptr, &open_)) {
		open_ = true;
		/// Bring to top
	}
}

fe::editor_window::callback_t const& fields_engine::editor_window::callback() const {
	return callback_;
}

void fields_engine::editor_window::callback(callback_t const& newCallback) {
	callback_ = newCallback;
}
