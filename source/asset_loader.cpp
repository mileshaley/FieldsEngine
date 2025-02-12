/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset_loader.cpp                                                    *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "asset_loader.h"
#include <filesystem>
#include <fstream>

/// TODO: Remove
#include "texture.h"
#include "material.h"
#include "mesh.h"

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
	json const& data, 
	type_name const& type
) {
	if (type == "material") {
		vis::material* mat = new vis::material();
		mat->read(data);
		return mat;
	} else if (type == "mesh") {
		vis::mesh* mesh = new vis::mesh();
		mesh->read(data);
		return mesh;
	} else if (type == "texture") {
		vis::texture* texture = new vis::texture();
		texture->read(data);
		return texture;
	}
	return nullptr;
}

void fields_engine::asset_loader::unload_asset(
	void* loaded_asset, 
	type_name const& type
) const {
	delete loaded_asset;
}
