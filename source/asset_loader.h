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

		/// TODO: Make not static
		static void* load_asset(
			json const& data,
			type_name const& type
		);// const;

		/// TODO: Use or remove this
		void unload_asset(
			void* loaded_asset,
			type_name const& type
		) const;

	private:
	};

} // namespace fields_engine
