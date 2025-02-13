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

/*~-------------------------------------------------------------------------~*\
 * Asset Definitions                                                         *
\*~-------------------------------------------------------------------------~*/

fields_engine::asset_base::asset_base()
	: m_name()
	, m_type()
{
}

fields_engine::asset_base::~asset_base() = default;

void fields_engine::asset_base::set_name(string const& name) {
	m_name = name;
}

void fields_engine::asset_base::set_type(type_name const& type) {
	m_type = type;
}

fe::string const& fields_engine::asset_base::get_name() const {
	return m_name;
}

fe::type_name const& fields_engine::asset_base::get_type() const {
	return m_type;
}
