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

/*~-------------------------------------------------------------------------~*\
 * Asset Manager Class                                                       *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class asset_manager {
	public:
		asset_manager();
		~asset_manager() = default;



		bool startup();

		bool shutdown();

		// Get an asset and load it if it hasn't already been
		// Returns null in the case of nonexistent/invalid asset
		asset* get_asset(string const& asset_name);
		// Get an asset and load it if it hasn't already been
		// Returns null in the case of nonexistent/invali asset
		asset const* get_asset(string const& asset_name) const;

#if EDITOR
		bool content_browser_window();
#endif // EDITOR

	private:
		unordered_map<string, asset> m_assets;
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
