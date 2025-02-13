/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset_entry.cpp                                                     *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "asset_entry.h"
#include <fstream>

/*~-------------------------------------------------------------------------~*\
 * Asset Entry Definitions                                                   *
\*~-------------------------------------------------------------------------~*/

fields_engine::asset_entry::asset_entry(std::filesystem::path const& path)
	: m_asset()
#if EDITOR
	, m_thumbnail()
#endif // EDITOR
	, m_path()
	, m_str_id()
	, m_name()
	, m_str_id_separator_offset(0)
{
	set_path(path);
}

fields_engine::asset_entry::~asset_entry() = default;

void fields_engine::asset_entry::set_path(std::filesystem::path const& path) {
	m_path = path;
	m_str_id = m_path.stem().string();
	const size_t separator_offset = m_str_id.rfind('.');
	FE_ASSERT(separator_offset != string::npos, "Asset file missing name & type separator");
	m_str_id_separator_offset = static_cast<int>(separator_offset);
	m_name = m_str_id.substr(0, m_str_id_separator_offset);
}

bool fields_engine::asset_entry::load() {
	if (!std::filesystem::exists(m_path)) {
		return false;
	}
	std::ifstream in_file(m_path);
	if (!in_file) { return false; }
	json in = json::parse(in_file, nullptr, false);
	if (in.is_discarded()) { return false; }

	m_asset = make_from_type_name<asset>(string(get_type()));
	if (!m_asset) { return false; }
	m_asset->set_asset_entry(this);
	m_asset->read(in.at("data"));

	return true;
}

bool fields_engine::asset_entry::is_loaded() const {
	return m_asset != nullptr;
}

fe::asset* fields_engine::asset_entry::get_load_asset() {
	if (!is_loaded() && !load()) {
		return nullptr;
	}
	return m_asset.get();
}

fe::asset* fields_engine::asset_entry::get_asset_unchecked() {
	return m_asset.get();
}

fe::string const& fields_engine::asset_entry::get_str_id() const {
	return m_str_id;
}

fe::string const& fields_engine::asset_entry::get_name() const {
	return m_name;
}

fe::string_view fields_engine::asset_entry::get_type() const {
	const size_t offset = m_str_id_separator_offset + 1llu;
	return string_view(m_str_id.c_str() + offset, m_str_id.size() - offset);
}

#if EDITOR
fe::vis::texture const& fields_engine::asset_entry::get_thumbnail() const {
	return m_thumbnail;
}
#endif // EDITOR
