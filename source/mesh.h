/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: mesh.h                                                              *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "component.h"
#include "material.h"

/*~-------------------------------------------------------------------------~*\
 * Entity Class                                                              *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class mesh : public component {
	public:

		mesh();
		mesh(mesh const& other);

		FE_GEN_COMPONENT(mesh);

		void generate();

		virtual void draw(graphics::shader const& shader) const override;
		void add_face(mat4 const& transform);
		void add_cube();

		graphics::material      & ref_material()       { return m_material; }
		graphics::material const& ref_material() const { return m_material; }

	private:
		void add_tris_for_quad(ivec4 const& indices);

		unsigned m_vao_id;
		vector<vec4> m_vertices;
		vector<vec3> m_normals;
		vector<vec2> m_textures;
		// Indices within m_vertices
		vector<ivec3> m_triangles;

		graphics::material m_material;
	};

} // namespace fields_engine
