/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: asset_entry.h                                                       *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

namespace std::filesystem {
	class path;
} // namespace std::filesystem

namespace fields_engine {
	class asset;
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Asset Entry Class                                                         *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class asset_entry {
	public:
		box<asset> asset;
		std::filesystem::path path;

#if EDITOR
		void* get_thumbnail() const;
#endif // EDITOR

	}; // class asset_entry

} // namespace fields_engine
