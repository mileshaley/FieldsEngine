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

namespace fields_engine {
	namespace vis {
		class texture;
	} // namespace vis

#if EDITOR
	namespace editor {
		class asset_browser;
	} // namespace editor
#endif // EDITOR

	class asset_entry;
} // namespace fields_engine

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

		asset* get_loaded_asset(string const& name, type_name const& type);

		// Returns null in the case of nonexistent/invali asset
		asset_entry* get_asset_entry(string const& name, type_name const& type);

		//asset* get_or_create_asset(string const& asset_name);

		// Add a new asset
		// Returns a pointer to the moved asset on success
		// Returns nullptr if asset with that name already exists
		//asset* add_asset(asset&& new_asset);
		
		// Add a new asset
		// Returns a pointer to the created asset on success
		// Returns nullptr if asset with that name already exists or couldn't be created
		asset_entry* add_asset(std::filesystem::path const& new_asset_path);

	private:
		unordered_map<string, asset_entry> m_assets;

#if EDITOR
		box<editor::asset_browser> m_asset_browser;
		friend class editor::asset_browser;
#endif // EDITOR
	};

} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Asset Convenience Functions                                               *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	template<typename T>
	inline T* get_asset(string const& name) {
		return static_cast<T*>(
			context<asset_manager>().get_loaded_asset(name, type_name(T::static_type_name())));
	}

	template<typename T>
	inline T* get_asset(string const& name, string_view type) {
		return static_cast<T*>(
			context<asset_manager>().get_loaded_asset(name, type_name(type)));
	}

} // namespace fields_engine
