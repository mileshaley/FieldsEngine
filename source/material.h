/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: material.h                                                          *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/



/*~-------------------------------------------------------------------------~*\
 * Material Class                                                            *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {

	class material {
	public:
		material();
		material(material const& other);


		friend void from_json(json const& in, material& out);

	//private:
		float m_shininess;
		vec3 m_diffuse_color;
		vec3 m_specular_color;
	};

} // namespace fields_engine::vis
