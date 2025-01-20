/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: texture_import.cpp                                                  *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "texture_import.h"
#include <filesystem>
#include <fstream>
#include "base64/base64.hpp"

/*~-------------------------------------------------------------------------~*\
 * Texture Import Definitions                                                *
\*~-------------------------------------------------------------------------~*/

fe::json fields_engine::vis_texture_import(std::filesystem::path const& path) {
	std::ifstream in_stream(path, std::ios::binary);
	if (!in_stream) {
		return nullptr;
	}
	vector<u8> data_vector(std::istreambuf_iterator(in_stream), {});
	json out{};
	out["data"] = base64::encode_into<string>(data_vector.begin(), data_vector.end());
	out["upscale_filter"] = 0;
	out["downscale_filter"] = 0;
	return out;
}
