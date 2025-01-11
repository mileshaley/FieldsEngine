/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: material.h                                                          *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {
	class texture;
	class shader;
} // namespace fields_engine::vis

/*~-------------------------------------------------------------------------~*\
 * Material Class                                                            *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {

	class material {
	public:
		material();
		material(material const& other);

		void use(shader const& shader) const;

		friend void from_json(json const& in, material& out);

		void set_texture(texture const* new_texture);
		void set_normal_texture(texture const* new_normal_texture);

	//private:
		float m_shininess;
		vec3 m_diffuse_color;
		vec3 m_specular_color;
	private:
		texture const* m_texture;
		texture const* m_normal_texture;
	};

} // namespace fields_engine::vis
