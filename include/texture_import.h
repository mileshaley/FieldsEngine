/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: texture_import.h                                                    *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

namespace std::filesystem {
	class path;
} // namespace std::filesystem

/*~-------------------------------------------------------------------------~*\
 * Texture Import Functions                                                  *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	json vis_texture_import(std::filesystem::path const& path);

} // namespace fields_engine
