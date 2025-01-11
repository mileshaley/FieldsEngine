/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset_loader.cpp                                                    *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "asset_loader.h"
#include <filesystem>
#include <fstream>

/// TODO: Remove
#include "texture.h"

/*~-------------------------------------------------------------------------~*\
 * Asset Loader Definitions                                                  *
\*~-------------------------------------------------------------------------~*/

fields_engine::asset_loader::asset_loader()
{
}

fields_engine::asset_loader::~asset_loader()
{
}

void* fields_engine::asset_loader::load_asset(
	std::filesystem::path const& data_path, 
	string const& type
) const {
	//std::ifstream data_file(data_path);
	//if (!data_file) { return nullptr; }

	if (type == "texture") {
		return new vis::texture(data_path.string());
	} else {
		
	}

	return nullptr;
}
