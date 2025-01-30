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

namespace fields_engine::vis {
	class texture;
} // namespace fields_engine::vis

#if EDITOR
namespace fields_engine::editor {
	class asset_browser;
} // namespace fields_engine:editor
#endif // EDITOR

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
		//asset* add_asset(asset&& new_asset);
		
		// Add a new asset
		// Returns a pointer to the created asset on success
		// Returns nullptr if asset with that name already exists or couldn't be created
		asset* add_asset(std::filesystem::path const& new_asset_path);

	private:
		unordered_map<string, asset> m_assets;

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
	inline T* get_asset(string const& asset_name) {
		if (asset* a = context<asset_manager>().get_asset(asset_name)) {
			return a->get_data<T>();
		}
		return nullptr;
	}

} // namespace fields_engine
