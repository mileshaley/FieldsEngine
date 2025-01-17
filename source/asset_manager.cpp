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

/*~-------------------------------------------------------------------------~*\
 * Asset Manager Definitions                                                 *
\*~-------------------------------------------------------------------------~*/

fields_engine::asset_manager::asset_manager()
	: m_assets()
{
}

bool fields_engine::asset_manager::startup() {
#if EDITOR
	context<editor>().add_window(make_box<editor_window>(
		"Content Browser",
		std::bind(&asset_manager::content_browser_window, this),
		ICON_FOLDER
	));
#endif
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
	return true;
}

bool fields_engine::asset_manager::shutdown() {
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
bool fields_engine::asset_manager::content_browser_window() {
	constexpr ImVec2 entry_size{ 80, 80 };
	constexpr ImVec2 thumbnail_size{ 60, 60 };
	constexpr ImVec2 thumbnail_margin{
		(entry_size.x - thumbnail_size.x) / 2,
		2
	};
	constexpr float pad_between = 15;
	constexpr float offscreen_tolerance = 0.1f;
	constexpr ImVec2 text_offset{
		2,
		thumbnail_margin.y * 2 + thumbnail_size.y
	};
	constexpr ImGuiSelectableFlags entry_selectable_flags
		= ImGuiSelectableFlags_AllowDoubleClick
		| ImGuiSelectableFlags_Disabled;

	if (ImGui::BeginChild("content_browser_child")) {
		const ImVec2 max = ImGui::GetContentRegionMax();

		for (auto const& asset : m_assets) {
			const ImVec2 cursor_pos = ImGui::GetCursorPos();
			ImGui::PushID(&asset.second);
			ImGui::Selectable("", false, entry_selectable_flags, entry_size);
			ImGui::SetCursorPos(cursor_pos + text_offset);
			ImGui::Text(ellipsis_compress_middle(asset.first, 10).c_str());
			if (void* texture = asset.second.get_thumbnail()) {
				ImGui::SetCursorPos(cursor_pos + thumbnail_margin);
				// For some reason ImGui textures are flipped, so we adjust uvs manually here
				ImGui::Image(texture, thumbnail_size, ImVec2(0,1), ImVec2(1,0));
			}
			// We want to check if the next item will be put offscreen
			if (cursor_pos.x + (2.0f - offscreen_tolerance) * entry_size.x + pad_between > max.x) {
				// Start a new line
				ImGui::SetCursorPosY(cursor_pos.y + entry_size.y + pad_between);
			} else {
				// Continue with next item horizontally
				ImGui::SetCursorPos(cursor_pos + ImVec2(entry_size.x + pad_between, 0));
			}
			ImGui::PopID(); // asset address
		}
	}
	ImGui::EndChild();
	return false;
}
#endif // EDITOR
