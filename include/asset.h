/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: asset.h                                                             *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "reflection.h" // FE_REFLECT_BODY

namespace fields_engine {
	class asset_entry;
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Asset Class                                                               *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class asset {
		FE_REFLECT_BODY(asset);
	public:
		asset();
		virtual ~asset();

		void set_asset_entry(asset_entry* entry);

		virtual void read(json const& in) = 0;
		virtual void write(json& out) const = 0;

		void reload();
		void save() const;


		asset_entry& get_asset_entry();
		string const& get_name() const;

	private:
		asset_entry* m_entry;
	};

} // namespace fields_engine

