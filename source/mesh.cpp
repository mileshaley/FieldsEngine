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
	: m_vertices()
	, m_triangles()
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
        m_vertices.size() * 4 * sizeof(float),
        glm::value_ptr(m_vertices[0]), // float ptr
        GL_STATIC_DRAW);
    FE_GL_VERIFY;
    glEnableVertexAttribArray(0);
    FE_GL_VERIFY;
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    FE_GL_VERIFY;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    FE_GL_VERIFY;

    if (!m_normals.empty()) {
        GLuint norm_buf = 0;
        glGenBuffers(1, &norm_buf);
        FE_GL_VERIFY;
        glBindBuffer(GL_ARRAY_BUFFER, norm_buf);
        FE_GL_VERIFY;
        glBufferData(GL_ARRAY_BUFFER,
            m_normals.size() * 3 * sizeof(float),
            glm::value_ptr(m_normals[0]), // float ptr
            GL_STATIC_DRAW);
        FE_GL_VERIFY;
        glEnableVertexAttribArray(1);
        FE_GL_VERIFY;
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        FE_GL_VERIFY;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        FE_GL_VERIFY;
    }

    if (!m_textures.empty()) {

        GLuint tex_buf = 0;
        glGenBuffers(1, &tex_buf);
        FE_GL_VERIFY;
        glBindBuffer(GL_ARRAY_BUFFER, tex_buf);
        FE_GL_VERIFY;
        glBufferData(GL_ARRAY_BUFFER,
            m_textures.size() * 2 * sizeof(float),
            glm::value_ptr(m_textures[0]), // float ptr
            GL_STATIC_DRAW);
        FE_GL_VERIFY;
        glEnableVertexAttribArray(2);
        FE_GL_VERIFY;
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        FE_GL_VERIFY;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        FE_GL_VERIFY;
    }

    if (!m_tangents.empty()) {
        GLuint tan_buf;
        glGenBuffers(1, &tan_buf);
        FE_GL_VERIFY;
        glBindBuffer(GL_ARRAY_BUFFER, tan_buf);
        FE_GL_VERIFY;
        glBufferData(GL_ARRAY_BUFFER,
            m_tangents.size() * 3 * sizeof(float),
            glm::value_ptr(m_tangents[0]), // float ptr 
            GL_STATIC_DRAW);
        FE_GL_VERIFY;
        glEnableVertexAttribArray(3);
        FE_GL_VERIFY;
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
        FE_GL_VERIFY;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        FE_GL_VERIFY;
    }

    // Generate triangle buffer
    GLuint tri_buf = 0;
    glGenBuffers(1, &tri_buf);
    FE_GL_VERIFY;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_buf);
    FE_GL_VERIFY;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        m_triangles.size() * 3 * sizeof(int),
        glm::value_ptr(m_triangles[0]), // int ptr 
        GL_STATIC_DRAW);
    FE_GL_VERIFY;
    glBindVertexArray(0);
    FE_GL_VERIFY;
}

void fields_engine::mesh::draw() const {
    glBindVertexArray(m_vao_id);
    FE_GL_VERIFY;
    glDrawElements(GL_TRIANGLES, unsigned(m_triangles.size() * 3), GL_UNSIGNED_INT, 0);
    FE_GL_VERIFY;
    glBindVertexArray(0);
    FE_GL_VERIFY;
}

void fields_engine::mesh::add_plane(mat4 const& tr) {
    constexpr int num_verts = 4;
    constexpr vec4 verts[num_verts]{
        { 1.0f,  1.0f, 1.0f, 1.0f}, 
        {-1.0f,  1.0f, 1.0f, 1.0f}, 
        {-1.0f, -1.0f, 1.0f, 1.0f}, 
        { 1.0f, -1.0f, 1.0f, 1.0f}
    };
    constexpr vec2 tex_coords[num_verts]{ 
        {1.0f, 1.0f},  
        {0.0f, 1.0f},  
        {0.0f, 0.0f}, 
        {1.0f, 0.0f}
    };

    const vec3 normal{tr * vec4(0, 0, 1, 0)};
    const vec3 tangent{tr * vec4(1, 0, 0, 0)};

    for (int i = 0; i < num_verts; ++i) {
        m_vertices.emplace_back(tr * verts[i]);
        m_textures.emplace_back(tex_coords[i]);
        m_normals .emplace_back(normal);
        m_tangents.emplace_back(tangent);
    }

    int n = int(m_vertices.size());
    add_tris_for_quad({ n, n + 1, n + 2, n + 3 });
}

void fields_engine::mesh::add_cube(float width) {
    constexpr float rot_90 = glm::radians(90.0f);
    constexpr vec3 i{ 1, 0, 0 };
    constexpr vec3 j{ 0, 1, 0 };
    const glm::mat4 face_mat(width * 0.5f);

    // Add 6 faces as rotations of face_mat 
    add_plane(face_mat);
    add_plane(glm::rotate(face_mat, rot_90, i));
    add_plane(glm::rotate(face_mat, -rot_90, i));
    add_plane(glm::rotate(face_mat, rot_90, j));
    add_plane(glm::rotate(face_mat, -rot_90, j));
    add_plane(glm::rotate(face_mat, 2 * rot_90, i));
}

void fields_engine::mesh::add_triangle(vec3 const& a, vec3 const& b, vec3 const& c) {
    int first = int(m_triangles.size());
    m_vertices.emplace_back(a, 1.0f);
    m_vertices.emplace_back(b, 1.0f);
    m_vertices.emplace_back(c, 1.0f);
    m_tangents.emplace_back(0, 0, 0);
    m_tangents.emplace_back(0, 0, 0);
    m_tangents.emplace_back(0, 0, 0);
    vec3 norm = glm::cross(a - b, a - c);
    m_normals.emplace_back(norm);
    m_normals.emplace_back(norm);
    m_normals.emplace_back(norm);
    m_textures.emplace_back(norm);
    m_triangles.emplace_back(first, first + 1, first + 2);
}

void fields_engine::mesh::add_tris_for_quad(ivec4 const& idcs) {
    m_triangles.emplace_back(idcs.x, idcs.y, idcs.z);
    m_triangles.emplace_back(idcs.x, idcs.z, idcs.w);
}
