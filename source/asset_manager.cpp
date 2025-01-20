/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset_manager.cpp                                                   *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "asset_manager.h"
#include <filesystem>
#include "asset_loader.h"
#include "context.h"
#include "editor.h"
#include "imgui.h"
#include "string_util.h"


#include "texture.h"

/*~-------------------------------------------------------------------------~*\
 * Asset Manager Definitions                                                 *
\*~-------------------------------------------------------------------------~*/

fields_engine::asset_manager::asset_manager()
	: m_assets()
{
}

fields_engine::asset_manager::~asset_manager() = default;

bool fields_engine::asset_manager::startup() {
	std::filesystem::recursive_directory_iterator content_directory("content");
	for (auto const& file : content_directory) {
		std::filesystem::path in_path = file;
		auto ext = in_path.extension();
		if (ext != ".fea") {
			continue; 
		}
		m_assets.emplace(
			in_path.stem().stem().string(), 
			in_path
		);
	}

#if EDITOR
	context<editor>().add_window(make_box<editor_window>(
		"Asset Browser",
		std::bind(&asset_manager::asset_browser_window, this),
		ICON_FOLDER
	));
	m_missing_thumbnail = make_box<vis::texture>(string_view("engine_assets/missing_asset_thumbnail.png"));
	m_mesh_thumbnail = make_box<vis::texture>(string_view("engine_assets/mesh_asset_thumbnail.png"));
	m_material_thumbnail = make_box<vis::texture>(string_view("engine_assets/material_asset_thumbnail.png"));
	m_folder_thumbnail = make_box<vis::texture>(string_view("engine_assets/folder_thumbnail.png"));
	refresh_asset_browser();
#endif // EDITOR

	return true;
}

bool fields_engine::asset_manager::shutdown() {
#if EDITOR
	m_missing_thumbnail.reset();
	m_mesh_thumbnail.reset();
	m_material_thumbnail.reset();
	m_folder_thumbnail.reset();
#endif // EDITOR
	m_assets.clear();
	return true;
}

fe::asset* fields_engine::asset_manager::get_asset(string const& asset_name) {
	auto it = m_assets.find(asset_name);
	if (it == m_assets.end()) { return nullptr; } // Asset not found
	if (!it->second.load()) { return nullptr; } // Asset couldn't be loaded
	return &it->second;
}

fe::asset const* fields_engine::asset_manager::get_asset(string const& asset_name) const {
	auto it = m_assets.find(asset_name);
	if (it == m_assets.end()) { return nullptr; }
	return &it->second;
}

fe::asset* fields_engine::asset_manager::add_asset(asset&& new_asset) {
	string name(new_asset.get_name());
	const auto [it, success] = m_assets.try_emplace(move(name), move(new_asset));
	if (success) { return &it->second; }
	else		 { return nullptr; }
}

#if EDITOR
bool fields_engine::asset_manager::asset_browser_window() {
	constexpr ImVec2 entry_size{ 120, 156 };
	constexpr ImVec2 thumbnail_size{ 100, 100 };
	constexpr ImVec2 thumbnail_margin{
		(entry_size.x - thumbnail_size.x) / 2,
		(entry_size.x - thumbnail_size.x) / 2
	};
	constexpr float pad_between = 15;
	constexpr ImVec2 name_text_offset{
		7,
		5 + thumbnail_margin.y + thumbnail_size.y
	};
	constexpr ImVec2 type_text_offset{
		name_text_offset.x,
		name_text_offset.y + 17
	};
	constexpr float offscreen_tolerance = 0.1f;

	if (ImGui::BeginChild("content_browser_child")) {
		const ImVec2 max = ImGui::GetContentRegionMax();
		// Make outlines visible
		ImGuiStyle const& style = ImGui::GetStyle();
		ImVec4 border = style.Colors[ImGuiCol_Border];
		ImVec4 border_shadow = style.Colors[ImGuiCol_BorderShadow];
		border.w = border.z;
		border_shadow.w = border.z;
		ImGui::PushStyleColor(ImGuiCol_Border, border);
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, border_shadow);
		
		for (auto const& entry : m_browser_entries) {
			const ImVec2 cursor_pos = ImGui::GetCursorPos();
			/// TODO: this doesn't really make sense, fix it
			/// Also when fixed, update the comment on the matching PopID() call
			ImGui::PushID(&entry);

			ImVec2 window_pos = ImGui::GetWindowPos();
			if (entry.type != file_type::folder || ImGui::IsMouseHoveringRect(
					window_pos + cursor_pos, window_pos + cursor_pos + entry_size)) {
				if (ImGui::Button("", entry_size)) {
				
				}
			}

			ImGui::SetCursorPos(cursor_pos + type_text_offset);

			asset* asset = nullptr;
			if (entry.type == file_type::asset) {
				asset = &m_assets.at(entry.name);
				ImGui::TextColored(
					ImVec4(1, 1, 1, 0.65f),
					asset->get_type().c_str()
				);
			} else if (entry.type == file_type::other) {
				ImGui::TextColored(
					ImVec4(1, 1, 1, 0.65f),
					"unknown"
				);
			}

			ImGui::SetCursorPos(cursor_pos + name_text_offset);
			ImGui::Text(ellipsis_compress_middle(entry.name, 14).c_str());

			void* thumbnail = nullptr;
			if (entry.type == file_type::asset) {
				thumbnail = asset->get_thumbnail();
				if (!thumbnail) {
					if (asset->get_type() == "mesh") {
						thumbnail = m_mesh_thumbnail->get_void_ptr_id();
					}
					else if (asset->get_type() == "material") {
						thumbnail = m_material_thumbnail->get_void_ptr_id();
					}
					else {
						thumbnail = m_missing_thumbnail->get_void_ptr_id();
					}
				}
			} else if (entry.type == file_type::folder) {
				thumbnail = m_folder_thumbnail->get_void_ptr_id();
			} else if (entry.type == file_type::other) {
				thumbnail = m_missing_thumbnail->get_void_ptr_id();
			}
			ImGui::SetCursorPos(cursor_pos + thumbnail_margin);
			// For some reason ImGui textures are flipped, so we adjust uvs manually here
			ImGui::Image(thumbnail, thumbnail_size, ImVec2(0,1), ImVec2(1,0));
			// We want to check if the next item will be put offscreen
			if (cursor_pos.x + (2.0f - offscreen_tolerance) * entry_size.x + pad_between > max.x) {
				// Start a new line
				ImGui::SetCursorPosY(cursor_pos.y + entry_size.y + pad_between);
			} else {
				// Continue with next item horizontally
				ImGui::SetCursorPos(cursor_pos + ImVec2(entry_size.x + pad_between, 0));
			}
			ImGui::PopID(); // entry address
		}

		ImGui::PopStyleColor(2);
	}
	ImGui::EndChild();
	return false;
}

void fields_engine::asset_manager::refresh_asset_browser() {
	std::filesystem::directory_iterator curr_directory(m_browser_current_directory);
	m_browser_entries.clear();
	for (std::filesystem::directory_entry const& entry : curr_directory) {
		std::filesystem::path const& path = entry.path();
		if (entry.is_directory()) {
			m_browser_entries.push_back(file_entry{path.filename().string(), file_type::folder});
			continue;
		}
		if (path.extension() == ".fea") {
			auto it = m_assets.find(path.stem().stem().string());
			if (it != m_assets.end()) {
				m_browser_entries.push_back(file_entry{ it->first, file_type::asset });
				continue;
			}
		}
		m_browser_entries.push_back(file_entry{path.filename().string(), file_type::other });
	}

	std::sort(m_browser_entries.begin(), m_browser_entries.end(), 
		[](file_entry const& l, file_entry const& r) {
			return int(l.type) < int(r.type);
		});
}
#endif // EDITOR
