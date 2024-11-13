/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: mesh.cpp                                                            *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "mesh.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "graphics.h"

namespace fields_engine {
    using namespace graphics;
} // namespace fields_engine

fields_engine::mesh::mesh()
	: m_verts()
	, m_tris()
	, m_vao_id(0)
{}

void fields_engine::mesh::generate() {
    // Give this mesh a vertex object array
    glGenVertexArrays(1, &m_vao_id);
    glBindVertexArray(m_vao_id);
    
    // Generate vertex buffer
    GLuint vert_buf = 0;
    glGenBuffers(1, &vert_buf);
    FE_GL_VERIFY;
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
    FE_GL_VERIFY;
    glBufferData(GL_ARRAY_BUFFER,
        m_verts.size() * 4 *sizeof(float),
        glm::value_ptr(m_verts[0]), // float ptr
        GL_STATIC_DRAW);
    FE_GL_VERIFY;
    glEnableVertexAttribArray(0);
    FE_GL_VERIFY;
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    FE_GL_VERIFY;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    FE_GL_VERIFY;

    // Generate triangle buffer
    GLuint tri_buf = 0;
    glGenBuffers(1, &tri_buf);
    FE_GL_VERIFY;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_buf);
    FE_GL_VERIFY;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        m_tris.size() * 3 * sizeof(int),
        glm::value_ptr(m_tris[0]), // int ptr 
        GL_STATIC_DRAW);
    FE_GL_VERIFY;
    glBindVertexArray(0);
    FE_GL_VERIFY;
}

void fields_engine::mesh::draw() const {
    glBindVertexArray(m_vao_id);
    FE_GL_VERIFY;
    glDrawElements(GL_TRIANGLES, unsigned(m_tris.size() * 3), GL_UNSIGNED_INT, 0);
    FE_GL_VERIFY;
    glBindVertexArray(0);
    FE_GL_VERIFY;
}

void fields_engine::mesh::add_triangle(vec3 const& a, vec3 const& b, vec3 const& c) {
    int first = int(m_tris.size());
    m_verts.emplace_back(vec4{ a, 1 });
    m_verts.emplace_back(vec4{ b, 1 });
    m_verts.emplace_back(vec4{ c, 1 });
    m_tris.emplace_back(first, first + 1, first + 2);
}
