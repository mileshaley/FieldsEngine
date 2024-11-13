/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: mesh.h                                                              *
\*~-------------------------------------------------------------------------~*/

#pragma once

namespace fields_engine {

	class mesh {
	public:
		
		mesh();
		void generate();

		void draw() const;
		void add_plane(mat4 const& transform);
		void add_cube(float width = 1.0f);

		void add_triangle(vec3 const& a, vec3 const& b, vec3 const& c);

	private:
		void add_tris_for_quad(ivec4 const& indices);

		unsigned m_vao_id;
		vector<vec4> m_vertices;
		vector<vec3> m_normals;
		vector<vec3> m_tangents;
		vector<vec2> m_textures;
		// Indices within m_vertices
		vector<ivec3> m_triangles;
	};

} // namespace fields_engine
