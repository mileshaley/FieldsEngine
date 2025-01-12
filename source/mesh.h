/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: mesh.h                                                              *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {
	class material;
} // namespace fields_engine::vis

/*~-------------------------------------------------------------------------~*\
 * Mesh Class                                                                *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {

	class mesh {
	public:
		enum class primitive_type {
			none,
			cube,
			cylinder,
			pyramid,
			sphere
		};

		mesh();
		mesh(mesh const& other);
		~mesh();

		void draw() const;

		void generate();
		void reset();

		void add_face(mat4 const& transform);
		void add_cube();
		void add_sphere(int subdivisions);
		void add_cylinder(int sides = 16, float height = 1.0f);
		void add_pyramid(int sides = 4, float height = 1.0f);


		friend void from_json(json const& in, mesh& out);
		friend void to_json(json& out, mesh const& in);

#if EDITOR // Editor data
		material* m_default_material;
#endif // EDITOR

	private:
		void sequential_tris(int first_vert_index);
		void tris_for_quad(ivec4 const& indices);
		void sequential_tris_for_quad(int first_vert_index);


	private: /// TODO: Remove variables in this section
		int m_divisions;
		int m_height;

	private:
		primitive_type m_prim_type;
		unsigned m_vao_id;
		vector<vec4> m_vertices;
		vector<vec2> m_tex_uvs;
		vector<vec3> m_normals;
		vector<vec3> m_tangents;
		// Indices within vertices, normals, tangents, and texture uvs
		vector<ivec3> m_triangles;

	}; // class mesh

} // namespace fields_engine::vis
