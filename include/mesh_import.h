/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: mesh_import.h                                                       *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

namespace std::filesystem {
	class path;
} // namespace std::filesystem

/*~-------------------------------------------------------------------------~*\
 * Mesh Import Functions                                                     *
\*~-------------------------------------------------------------------------~*/

/// TODO: put this in fields_engine::editor namespace
namespace fields_engine {

	json import_vis_mesh(std::filesystem::path const& in_path);

} // namespace fields_engine::editor
