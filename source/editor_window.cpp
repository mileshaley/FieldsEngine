/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: editor_window.cpp													 *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "editor_window.h"

#ifdef EDITOR

#include "imgui/imgui.h"

fields_engine::editor::editor_window::editor_window(
	string_view name, 
	own<window_invoker>&& function, 
	editor_icon icon
)
	: m_name(name)
	, m_str_id()
	, m_callback(move(function))
	, m_icon(icon)
	, m_open(true) 
{
	const string_view icon_view(icon);
	if (!icon_view.empty()) {
		m_str_id += icon_view;
		m_str_id += " ";
	}
	m_str_id += name;
	m_str_id += "###";
	m_str_id += name;
}

fields_engine::editor::editor_window::editor_window(editor_window&& other) noexcept
	: m_name(move(other.m_name))
	, m_str_id(move(other.m_str_id))
	, m_callback(move(other.m_callback))
	, m_icon(other.m_icon)
	, m_open(std::exchange(other.m_open, false)) 
{}

bool fields_engine::editor::editor_window::display() {
	return m_open && force_display();
}

bool fields_engine::editor::editor_window::force_display() {
	bool modif = false;
	if (m_callback) {
		if (begin_window()) {
			modif = m_callback->invoke(*this);
		}
		end_window();
	}
	return modif;
}

bool fields_engine::editor::editor_window::begin_window() {
	return ImGui::Begin(m_str_id.c_str(), &m_open);
}

void fields_engine::editor::editor_window::end_window() const {
	ImGui::End();
}

bool fields_engine::editor::editor_window::menu_item() {
	if (ImGui::MenuItem(m_str_id.c_str(), nullptr, &m_open)) {
		return true;
		/// Bring to top
	}
	return false;
}

bool fields_engine::editor::editor_window::is_open() const {
	return m_open;
}

void fields_engine::editor::editor_window::open() {
	m_open = true;
}

void fields_engine::editor::editor_window::close() {
	m_open = false;
}

bool& fields_engine::editor::editor_window::ref_open() {
	return m_open;
}

fe::editor::window_invoker const& fields_engine::editor::editor_window::get_invoker() const {
	return *m_callback;
}

void fields_engine::editor::editor_window::set_invoker(own<window_invoker>&& new_invoker) {
	m_callback = move(new_invoker);
}

fe::string const& fields_engine::editor::editor_window::get_str_id() const {
	return m_str_id;
}
#endif // EDITOR
