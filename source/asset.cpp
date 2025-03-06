/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset.cpp                                                           *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "asset.h"
#include <fstream>
#include "context.h"
#include "asset_entry.h"

/*~-------------------------------------------------------------------------~*\
 * Asset Definitions                                                         *
\*~-------------------------------------------------------------------------~*/

fields_engine::asset::asset()
	: m_entry(nullptr)
{}

fields_engine::asset::~asset() = default;

void fields_engine::asset::set_asset_entry(asset_entry* entry) {
	m_entry = entry;
}

void fields_engine::asset::reload() {
	std::ifstream in_file(m_entry->get_path());
	if (!in_file) { return; }
	const json in(json::parse(in_file));
	read(in["data"]);
	in.find("data");
}

void fields_engine::asset::save() const {
	std::ofstream out_file(m_entry->get_path());
	if (!out_file) { return; }
	json out{};
	write(out["data"]);
	out_file << std::setw(4) << out << std::endl;
}

fe::asset_entry& fields_engine::asset::get_asset_entry() {
	return *m_entry;
}

fe::string const& fields_engine::asset::get_name() const {
	return m_entry->get_name();
}
