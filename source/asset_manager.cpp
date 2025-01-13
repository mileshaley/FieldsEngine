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

#include <iostream>

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
	for (auto const& file : std::filesystem::directory_iterator("content")) {
		std::filesystem::path in_path = file;
		auto ext = in_path.extension();
		if (ext != ".fea") {
			/// TODO: Use proper error logger
			std::cerr << "Incorrect asset extension for \"" 
					  << in_path << "\"." << std::endl;
			continue; 
		}
		m_assets.emplace(in_path.stem().string(), in_path);
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

#if EDITOR
bool fields_engine::asset_manager::content_browser_window() {
	for (auto const& asset : m_assets) {
		ImGui::Text("%s (%s)", asset.first.c_str(), asset.second.get_type().c_str());
	}
	return false;
}
#endif // EDITOR
