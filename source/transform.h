/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: transform.h                                                         *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/


namespace fields_engine {

/*~-------------------------------------------------------------------------~*\
 * Transform Data Structure                                                  *
\*~-------------------------------------------------------------------------~*/

	struct transform_data {
		vec3 position;
		vec3 scale;
		vec3 rotation;
	};

/*~-------------------------------------------------------------------------~*\
 * Transform Class                                                           *
\*~-------------------------------------------------------------------------~*/

	// Lazy, only recalculates matrix when absolutely necessary
	class transform {
	public:

		transform();
		transform(transform_data const& data);

		mat4 const& matrix() const;
		
	private:
		transform_data m_data;
		mutable mat4 m_matrix;
		mutable bool m_dirty;
	};




} // namespace fields_engine
