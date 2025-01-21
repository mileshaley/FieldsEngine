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

#include <filesystem>

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

		enum class file_type : i8 { folder, asset, other };
		struct file_entry {
			std::filesystem::path path;
			asset* asset; // So we save some string manipulation and retrieval for assets
			file_type type;
			bool selected;
		};
		vector<file_entry> m_browser_entries;
		std::filesystem::path m_browser_current_directory = "assets";
		string m_address_bar_buffer;
		string m_search_bar_buffer;
		int m_prev_entry_clicked = -1; // For shift multi-select
		bool m_browser_needs_refresh = true;
		enum class address_bar_state : i8 {
			inactive = 0,
			activated,
			active,
		} m_address_bar_state = address_bar_state::inactive;

		friend struct asset_browser_callback_wrapper;

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
