/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: editor_window.cpp													 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "editor_window.h"
#include "imgui.h"

fields_engine::editor_window::editor_window(string_view name, callback_t const& callback, editor_icon icon)
	: m_name(name)
	, m_str_id()
	, m_callback(callback)
	, m_icon(icon)
	, m_open(true)
{
	string_view icon_strv(icon);
	if (!icon_strv.empty()) {
		m_str_id += icon_strv;
		m_str_id += " ";
	}
	m_str_id += name;
	m_str_id += "###";
	m_str_id += name;
}

fields_engine::editor_window::editor_window(editor_window const& other)
	: m_name(other.m_name)
	, m_str_id(other.m_str_id)
	, m_callback(other.m_callback)
	, m_icon(other.m_icon)
	, m_open(other.m_open)
{}

bool fields_engine::editor_window::display() {
	return m_open && force_display();
}

bool fields_engine::editor_window::force_display() {
	bool modif = false;
	if (m_callback) {
		if (begin_window()) {
			modif = m_callback();
		}
		end_window();
	}
	return modif;
}

bool fields_engine::editor_window::begin_window() {
	return ImGui::Begin(m_str_id.c_str(), &m_open);
}

void fields_engine::editor_window::end_window() const {
	ImGui::End();
}

bool fields_engine::editor_window::menu_item() {
	if (ImGui::MenuItem(m_str_id.c_str(), nullptr, &m_open)) {
		return true;
		/// Bring to top
	}
	return false;
}

bool fields_engine::editor_window::is_open() const {
	return m_open;
}

void fields_engine::editor_window::open() {
	m_open = true;
}

void fields_engine::editor_window::close() {
	m_open = false;
}

bool& fields_engine::editor_window::ref_open() {
	return m_open;
}

fe::editor_window::callback_t const& fields_engine::editor_window::get_callback() const {
	return m_callback;
}

void fields_engine::editor_window::set_callback(callback_t const& new_callback) {
	m_callback = new_callback;
}

fe::string const& fields_engine::editor_window::get_str_id() const {
	return m_str_id;
}
