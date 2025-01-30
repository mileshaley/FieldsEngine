/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset_manager.cpp                                                   *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "asset_manager.h"
#include "asset_loader.h"
#include "asset_browser.h"

/*~-------------------------------------------------------------------------~*\
 * Asset Manager Definitions                                                 *
\*~-------------------------------------------------------------------------~*/

fields_engine::asset_manager::asset_manager()
	: m_assets()
{
}

fields_engine::asset_manager::~asset_manager() = default;

bool fields_engine::asset_manager::startup() {
	std::filesystem::recursive_directory_iterator content_directory("assets");
	for (auto const& file : content_directory) {
		add_asset(file);
	}
#if EDITOR
	m_asset_browser = make_box<editor::asset_browser>();
#endif // EDITOR
	return true;
}

bool fields_engine::asset_manager::shutdown() {
#if EDITOR
	m_asset_browser.reset();
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

//fe::asset* fields_engine::asset_manager::add_asset(asset&& new_asset) {
//	string name(new_asset.get_name());
//	const auto [it, success] = m_assets.try_emplace(move(name), move(new_asset));
//	if (success) { return &it->second; }
//	else { return nullptr; }
//}

fe::asset* fields_engine::asset_manager::add_asset(std::filesystem::path const& new_asset_path) {
	auto ext = new_asset_path.extension();
	if (ext != ".fea") {
		return nullptr;
	}
	const auto [it, success] = m_assets.try_emplace(
		new_asset_path.stem().stem().string(), 
		new_asset_path
	);
	if (success) { return &it->second; } 
	else { return nullptr; }
}
