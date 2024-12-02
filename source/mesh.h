/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: mesh.h                                                              *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "spatial_component.h" // Inheritance
#include "material.h"

namespace fields_engine::graphics {
	class texture;
} // namespace fields_engine::graphics

/*~-------------------------------------------------------------------------~*\
 * Entity Class                                                              *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class mesh : public spatial_component {
	public:

		mesh();
		mesh(mesh const& other);
		~mesh();

		FE_GEN_COMPONENT(mesh);

		void generate();

		virtual void draw(graphics::shader const& shader) const override;
		void add_face(mat4 const& transform);
		void add_cube();
		void add_sphere(int subdivisions);
		void add_cylinder(int sides = 16, float height = 1.0f);
		void add_pyramid(int sides = 4, float height = 1.0f);


		graphics::material      & ref_material()       { return m_material; }
		graphics::material const& ref_material() const { return m_material; }

		void set_texture(unique<graphics::texture>&& new_texture);
		void set_normal_texture(unique<graphics::texture>&& new_normal_texture);


	private:
		void sequential_tris(int first_vert_index);
		void tris_for_quad(ivec4 const& indices);
		void sequential_tris_for_quad(int first_vert_index);

		/// TODO: Put these in external managed resource, 
		/// add pointer to that resource and also material, texture, etc.
		unsigned m_vao_id;
		vector<vec4> m_vertices;
		vector<vec3> m_normals;
		vector<vec2> m_textures;
		// Indices within m_vertices
		vector<ivec3> m_triangles;

		graphics::material m_material;
		unique<graphics::texture> m_texture;
		unique<graphics::texture> m_normal_texture;

	};

} // namespace fields_engine
