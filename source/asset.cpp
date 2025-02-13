/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset.cpp                                                           *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "asset.h"
#include <filesystem>
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

fe::asset_entry& fields_engine::asset::get_asset_entry() {
	return *m_entry;
}

fe::string const& fields_engine::asset::get_name() const {
	return m_entry->get_name();
}
