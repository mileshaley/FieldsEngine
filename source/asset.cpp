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

/*~-------------------------------------------------------------------------~*\
 * Asset Definitions                                                         *
\*~-------------------------------------------------------------------------~*/

fields_engine::asset::asset(std::filesystem::path const& info_path)
	: m_name()
	, m_type()
	, m_data(nullptr)
	, m_valid(false)
	, m_loaded(false)
{
	std::ifstream info_file(info_path);
	if (!info_file) { return; }
	/// TODO: Add error reporting/logging to this
	json info = json::parse(info_file, nullptr, false);
	if (info.is_discarded()) { return; }
	//auto name_it = info.find("name");
	//if (name_it == info.end()) { return; }
	auto type_it = info.find("type");
	if (type_it == info.end()) { return; }
	auto data_path_it = info.find("data_path");
	if (data_path_it == info.end()) { return; }
	/// TODO: check if the data path extension is supported with function

	// Wait until we know all data is present to take it
	//m_name = move(name_it->get<string>());
	m_name = info_path.stem().string();
	m_type = move(type_it->get<string>());
	m_data_path = move(data_path_it->get<string>());
	m_valid = true;
}

fields_engine::asset::asset(asset&& other) noexcept 
	: m_name(move(other.m_name))
	, m_type(move(other.m_type))
	, m_data_path(move(other.m_data_path))
	, m_data(other.m_data)
	, m_valid(other.m_valid)
	, m_loaded(other.m_loaded)
{
}

fields_engine::asset::~asset() {
	delete m_data; // Delete is valid on nullptr
}

bool fields_engine::asset::load() {
	if (m_loaded) { return true; }
	if (m_data_path.empty()) { return false; }
	m_data = asset_loader::load_asset(m_data_path, m_type);
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
