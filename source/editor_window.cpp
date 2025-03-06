/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: editor_window.cpp													 *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "editor_window.h"

#ifdef EDITOR

#include "imgui/imgui.h"
#include <fstream>

/*~-------------------------------------------------------------------------~*\
 * Editor Window Definitions                                                 *
\*~-------------------------------------------------------------------------~*/

fields_engine::editor::editor_window::editor_window(
	own<window_invoker>&& function, 
	string_view id_name, 
	string_view icon, 
	string_view display_name
) 
	: m_id_name(id_name)
	, m_display_name(display_name)
	, m_full_name()
	, m_invoker(move(function))
	, m_icon(icon)
	, m_open(true)
{
	recalculate_full_name();
}

fields_engine::editor::editor_window::editor_window(editor_window&& other) noexcept
	: m_id_name(move(other.m_id_name))
	, m_full_name(move(other.m_full_name))
	, m_invoker(move(other.m_invoker))
	, m_icon(other.m_icon)
	, m_open(std::exchange(other.m_open, false)) 
{}

void fields_engine::editor::editor_window::read() {
	std::ifstream in_file((std::filesystem::path("user_data") / "editor" / "window" / m_id_name).concat(".json"));
	if (!in_file) { return; }
	const json in(json::parse(in_file, nullptr, false));
	if (in.is_discarded()) { return; }
	read(in);
}

void fields_engine::editor::editor_window::write() const {
	const std::filesystem::path path 
		= (std::filesystem::path("user_data") / "editor" / "window" / m_id_name).concat(".json");
	// Ensure the directories exist
	std::filesystem::create_directories(path.parent_path());
	std::ofstream out_file(path);
	if (!out_file) { return; }
	json out;
	write(out);
	out_file << std::setw(4) << out << std::endl;
}

void fields_engine::editor::editor_window::read(json const& in) {
	TRY_JSON_READ(m_open, in, "open");
}

void fields_engine::editor::editor_window::write(json& out) const {
	out["open"] = m_open;
}

bool fields_engine::editor::editor_window::display() {
	return m_open && force_display();
}

bool fields_engine::editor::editor_window::force_display() {
	bool modif = false;
	if (m_invoker) {
		if (begin_window()) {
			//ImGui::PushID(m_function_id.c_str());
			modif = m_invoker->invoke(*this);
			//ImGui::PopID();
		}
		end_window();
	}
	return modif;
}

bool fields_engine::editor::editor_window::begin_window() {
	return ImGui::Begin(m_full_name.c_str(), &m_open);
}

void fields_engine::editor::editor_window::end_window() const {
	ImGui::End();
}

bool fields_engine::editor::editor_window::menu_item() {
	if (ImGui::MenuItem(m_full_name.c_str(), nullptr, &m_open)) {
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

bool& fields_engine::editor::editor_window::get_open_flag() {
	return m_open;
}

fe::editor::window_invoker const& fields_engine::editor::editor_window::get_invoker() const {
	return *m_invoker;
}

void fields_engine::editor::editor_window::set_invoker(own<window_invoker>&& new_invoker) {
	m_invoker = move(new_invoker);
	//m_function_id = m_invoker->get_function_id();
}

fe::string const& fields_engine::editor::editor_window::get_full_name() const {
	return m_full_name;
}

fe::string const& fields_engine::editor::editor_window::get_id_name() const {
	return m_id_name;
}

fe::string const& fields_engine::editor::editor_window::get_display_name() const {
	return m_display_name;
}

void fields_engine::editor::editor_window::set_icon(string_view new_icon) {
	m_icon = new_icon;
	recalculate_full_name();
}

void fields_engine::editor::editor_window::set_display_name(string_view new_display_name) {
	m_display_name = new_display_name;
	recalculate_full_name();
}

void fields_engine::editor::editor_window::recalculate_full_name() {
	m_full_name.clear();
	if (!m_icon.empty()) {
		m_full_name += m_icon;
		m_full_name += " ";
	}
	m_full_name += m_display_name;
	m_full_name += "###";
	m_full_name += m_id_name;
}

#endif // EDITOR
