/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: mesh.h                                                              *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * Mesh Class                                                                *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {

	class mesh {
	public:
		mesh();
		mesh(mesh const& other);
		~mesh();

		void draw() const;

		void generate();

		void add_face(mat4 const& transform);
		void add_cube();
		void add_sphere(int subdivisions);
		void add_cylinder(int sides = 16, float height = 1.0f);
		void add_pyramid(int sides = 4, float height = 1.0f);

	private:
		void sequential_tris(int first_vert_index);
		void tris_for_quad(ivec4 const& indices);
		void sequential_tris_for_quad(int first_vert_index);

		unsigned m_vao_id;
		vector<vec4> m_vertices;
		vector<vec2> m_tex_uvs;
		vector<vec3> m_normals;
		vector<vec3> m_tangents;
		// Indices within vertices, normals, tangents, and texture uvs
		vector<ivec3> m_triangles;
	}; // class mesh

} // namespace fields_engine::vis
