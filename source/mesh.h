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

		void add_triangle(vec3 const& a, vec3 const& b, vec3 const& c);

	private:
		unsigned m_vao_id;
		vector<vec4> m_verts;
		// Indices within m_verts
		vector<ivec3> m_tris;
	};

} // namespace fields_engine
