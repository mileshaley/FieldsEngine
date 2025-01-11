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
#include "material.h"

/*~-------------------------------------------------------------------------~*\
 * Asset Loader Definitions                                                  *
\*~-------------------------------------------------------------------------~*/

fields_engine::asset_loader::asset_loader()
{
}

fields_engine::asset_loader::~asset_loader()
{
}



void* fields_engine::asset_loader::load_asset_from_path(
	std::filesystem::path const& data_path, 
	type_name const& type
) {
	//std::ifstream data_file(data_path);
	//if (!data_file) { return nullptr; }

	if (type == "texture") {
		return new vis::texture(data_path.string());
	} else {
		
	}

	return nullptr;
}

void* fields_engine::asset_loader::load_asset_from_data(
	json const& data, 
	type_name const& type
) {
	if (type == "material") {
		return new vis::material();
	} else {

	}
	return nullptr;
}

void fields_engine::asset_loader::unload_asset(
	void* loaded_asset, 
	type_name const& type
) const {
	delete loaded_asset;
}
