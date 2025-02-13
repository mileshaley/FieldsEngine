/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset_manager.cpp                                                   *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "asset_manager.h"
#include "asset_browser.h"
#include "asset_entry.h"
#include "reflection.h"
#include <fstream>

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

fe::asset_base* fields_engine::asset_manager::get_loaded_asset(
	string const& name, type_name const& type
) {
	asset_entry* entry = get_asset_entry(name, type);
	if (entry == nullptr) { return nullptr; }

	if (entry->asset == nullptr) {
		std::ifstream in_file(entry->path);
		if (!in_file) { return nullptr; }
		json in = json::parse(in_file, nullptr, false);
		if (in.is_discarded()) { return nullptr; }
		entry->asset = make_from_type_name<asset_base>(type);
		entry->asset->read(in.at("data"));
	}
	return entry->asset.get();
}

fe::asset_entry* fields_engine::asset_manager::get_asset_entry(
	string const& name, type_name const& type
) {
	auto it = m_assets.find(name + "." + type);
	if (it == m_assets.end()) { return nullptr; } // Asset not found
	return &it->second;
}

fe::asset_entry* fields_engine::asset_manager::add_asset(std::filesystem::path const& new_asset_path) {
	if (new_asset_path.extension() != ".fea") {
		return nullptr;
	}

	string key = new_asset_path.stem().string();
	const auto [it, success] = m_assets.try_emplace(
		move(key),
		asset_entry{ nullptr, new_asset_path }
	);
	/// TODO: Determine if success is returned on element already exists, if not, this needs to change
	if (success) { return &it->second; } 
	else { return nullptr; }
}
