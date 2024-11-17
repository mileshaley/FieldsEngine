/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: mesh.h                                                              *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "component.h"

namespace fields_engine {

	class mesh : public component {
	public:

		mesh();
		mesh(mesh const& other);

		virtual unique_ptr<component> clone() const override {
			return make_unique<mesh>(*this);
		}

		void generate();

		virtual void render() const override;
		void add_plane(mat4 const& transform);
		void add_cube(float width = 1.0f);

	private:
		void add_tris_for_quad(ivec4 const& indices);

		unsigned m_vao_id;
		vector<vec4> m_vertices;
		vector<vec3> m_normals;
		vector<vec2> m_textures;
		// Indices within m_vertices
		vector<ivec3> m_triangles;
	};

} // namespace fields_engine
