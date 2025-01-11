/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: asset_loader.h                                                      *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

namespace std::filesystem {
	class path;
} // namespace std::filesystem

/*~-------------------------------------------------------------------------~*\
 * Asset Loader Class                                                        *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class asset_loader {
	public:
		asset_loader();
		~asset_loader();

		template<class T, class LoadingFn>
		bool register_type(type_name const& type, LoadingFn const& loader) {
			/// TODO: Implement
			return false;
		}

		void* load_asset(
			std::filesystem::path const& data_path, 
			string const& type_name
		) const;

	private:
	};

} // namespace fields_engine
