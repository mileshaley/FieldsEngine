/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset.cpp                                                           *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "asset.h"
#include <filesystem>
#include <fstream>
#include "asset_loader.h"
#include "context.h"


#include <iostream>
#include "texture.h"

/*~-------------------------------------------------------------------------~*\
 * Asset Definitions                                                         *
\*~-------------------------------------------------------------------------~*/

fields_engine::asset::asset(std::filesystem::path const& info_path)
	: m_data(nullptr)
	, m_name()
	, m_type()
	, m_data_source()
	, m_valid(false)
	, m_loaded(false)
{
	/// TODO: Add error reporting/logging to this function
	std::ifstream info_file(info_path);
	if (!info_file) { return; }
	json info = json::parse(info_file, nullptr, false);
	// json will be in discarded state if parse failure occurs with exceptions disabled
	if (info.is_discarded()) { return; }
	auto data_it = info.find("data");
	if (data_it == info.end()) { return; }
	m_data_source = move(*data_it);
	// Wait until we know all data is present
	m_valid = true;
	m_type = move(string(info_path.stem().extension().string().c_str() + 1));
	m_name = move(info_path.stem().stem().string());
}

fields_engine::asset::asset(asset&& other) noexcept
	: m_data(std::exchange(other.m_data, nullptr))
	, m_name(move(other.m_name))
	, m_type(move(other.m_type))
	, m_data_source(move(other.m_data_source))
	, m_valid(std::exchange(other.m_valid, false))
	, m_loaded(other.m_loaded)
{
}

fields_engine::asset::~asset() {
	delete m_data; // Delete is valid on nullptr
}

bool fields_engine::asset::load() {
	if (m_loaded) { return true; }
	if (!m_valid) { return false; }
	m_data = asset_loader::load_asset(m_data_source, m_type);
	m_loaded = m_data != nullptr;;
	return m_loaded;
}

bool fields_engine::asset::reload() {
	/// TODO: implement this
	return false;
}

bool fields_engine::asset::unload() {
	if (!m_loaded) { return true; }
	delete m_data;
	m_loaded = false;
	return true;
}

bool fields_engine::asset::is_loaded() const {
	return m_loaded;
}

fe::string const& fields_engine::asset::get_name() const {
	return m_name;
}

fe::type_name const& fields_engine::asset::get_type() const {
	return m_type;
}


#if EDITOR
void* fields_engine::asset::get_thumbnail() const {
	/// TODO: Put this elsewhere
	if (m_type == "texture") {
		if (vis::texture const* tex = get_data<vis::texture>()) {
			return reinterpret_cast<void*>(static_cast<intptr_t>(tex->get_id()));
		}
	}
	return nullptr;
}
#endif // EDITOR
