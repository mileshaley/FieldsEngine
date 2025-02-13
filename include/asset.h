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

/*~-------------------------------------------------------------------------~*\
 * Asset Class                                                               *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class asset {
		FE_REFLECT_BODY(asset);
	public:
		asset();
		virtual ~asset();

		virtual void read(json const& in) = 0;
		virtual void write(json& out) const = 0;

		void set_name(string const& name);
		void set_type(type_name const& type);

		string const& get_name() const;
		type_name const& get_type() const;

	private:
		string m_name;
		type_name m_type;
	};

} // namespace fields_engine

