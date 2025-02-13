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

fields_engine::asset::asset()
	: m_name()
	, m_type()
{
}

fields_engine::asset::~asset() = default;

void fields_engine::asset::set_name(string const& name) {
	m_name = name;
}

void fields_engine::asset::set_type(type_name const& type) {
	m_type = type;
}

fe::string const& fields_engine::asset::get_name() const {
	return m_name;
}

fe::type_name const& fields_engine::asset::get_type() const {
	return m_type;
}
