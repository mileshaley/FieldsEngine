/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: asset_manager.h                                                     *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "asset.h"
#include "context.h" // For convenience functions

///#include <filesystem>

namespace fields_engine::vis {
	class texture;
} // namespace fields_engine::vis

/*~-------------------------------------------------------------------------~*\
 * Asset Manager Class                                                       *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class asset_manager {
	public:
		asset_manager();
		~asset_manager();

		bool startup();

		bool shutdown();

		// Get an asset and load it if it hasn't already been
		// Returns null in the case of nonexistent/invalid asset
		asset* get_asset(string const& asset_name);
		// Get an asset and load it if it hasn't already been
		// Returns null in the case of nonexistent/invali asset
		asset const* get_asset(string const& asset_name) const;

		//asset* get_or_create_asset(string const& asset_name);

		// Add a new asset
		// Returns a pointer to the moved asset on success
		// Returns nullptr if asset with that name already exists
		asset* add_asset(asset&& new_asset);

	private:
		unordered_map<string, asset> m_assets;
	
#if EDITOR
	public:
		bool asset_browser_window();
	private:
		void refresh_asset_browser();
		/// TODO: Find a better way to do this. Perhaps keep a working tree
		/// structure of directory and update when needed
		enum class file_type { folder, asset, other };
		struct file_entry {
			string name;
			file_type type;
		};
		vector<file_entry> m_browser_entries;
		string m_browser_current_directory = "assets";

		/// TODO: Relocate
		box<vis::texture> m_mesh_thumbnail = nullptr;
		box<vis::texture> m_material_thumbnail = nullptr;
		box<vis::texture> m_missing_thumbnail = nullptr;
		box<vis::texture> m_folder_thumbnail = nullptr;
#endif // EDITOR
	};

} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Asset Convenience Functions                                               *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	template<typename T>
	inline T* get_asset(string const& asset_name) {
		if (asset* a = context<asset_manager>().get_asset(asset_name)) {
			return a->get_data<T>();
		}
		return nullptr;
	}

} // namespace fields_engine
