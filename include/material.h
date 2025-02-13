/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: material.h                                                          *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "asset.h" // Inheritance

namespace fields_engine::vis {
	class texture;
	class shader;
} // namespace fields_engine::vis

/*~-------------------------------------------------------------------------~*\
 * Material Class                                                            *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {

	class material : public asset_base {
		FE_REFLECT_BODY(material);
	public:
		material();
		material(material const& other);

		virtual void read(json const& in) override;
		virtual void write(json& out) const override;

		void use(shader const& shader) const;

		void set_texture(texture const* new_texture);
		void set_normal_texture(texture const* new_normal_texture);

		float m_shininess;
		vec3 m_diffuse;
		vec3 m_specular;
	private:
		texture const* m_texture;
		texture const* m_normal_texture;
	};

} // namespace fields_engine::vis
