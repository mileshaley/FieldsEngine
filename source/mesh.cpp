/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             * 
 *~-------------------------------------------------------------------------~* 
 * File: mesh.cpp                                                            * 
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "mesh.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "graphics.h"
#include "shader.h"

namespace fields_engine {
    using namespace graphics;
} // namespace fields_engine

fields_engine::mesh::mesh()
	: component()
    , m_vertices()
	, m_triangles()
    , m_textures()
    , m_normals()
	, m_vao_id(0)
{}

fields_engine::mesh::mesh(mesh const& other)
    : component(other)
    , m_vertices(other.m_vertices)
    , m_triangles(other.m_triangles)
    , m_textures(other.m_textures)
    , m_normals(other.m_normals)
    , m_vao_id(0)
{
    if (other.m_vao_id != 0) {
        generate();
    }
}

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
        m_vertices.size() * sizeof(vec4),
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
            m_normals.size() * sizeof(vec3),
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
            m_textures.size() * sizeof(vec2),
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

    // Generate triangle buffer
    GLuint tri_buf = 0;
    glGenBuffers(1, &tri_buf);
    FE_GL_VERIFY;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_buf);
    FE_GL_VERIFY;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        m_triangles.size() * sizeof(ivec3),
        glm::value_ptr(m_triangles[0]), // int ptr 
        GL_STATIC_DRAW);
    FE_GL_VERIFY;
    glBindVertexArray(0);
    FE_GL_VERIFY;
}

void fields_engine::mesh::render(graphics::shader const& shader) const {
    const mat4& matrix = ref_transform().world_matrix();
    const mat4 inverse = glm::inverse(matrix);

    GLint loc = shader.uniform_location("ModelTr");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    FE_GL_VERIFY;
    /// ???
    loc = shader.uniform_location("NormalTr");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(inverse));
    FE_GL_VERIFY;





    // Material settings for shader
    loc = shader.uniform_location("diffuse");
    glUniform3fv(loc, 1, glm::value_ptr(m_material.m_diffuse_color));
    FE_GL_VERIFY;

    loc = shader.uniform_location("specular");
    glUniform3fv(loc, 1, glm::value_ptr(m_material.m_specular_color));
    FE_GL_VERIFY;
    loc = shader.uniform_location("shininess");
    glUniform1f(loc, m_material.m_shininess);
    FE_GL_VERIFY;

    // Mesh render
    glBindVertexArray(m_vao_id);
    FE_GL_VERIFY;
    glDrawElements(GL_TRIANGLES,
        GLsizei(m_triangles.size() * 3),
        GL_UNSIGNED_INT,
        0);
    FE_GL_VERIFY;
    glBindVertexArray(0);
    FE_GL_VERIFY;
}

void fields_engine::mesh::add_plane(mat4 const& tr) {
    constexpr int num_corners = 4;
    constexpr vec4 verts[num_corners]{
        { 1.0f,  1.0f, 1.0f, 1.0f}, 
        {-1.0f,  1.0f, 1.0f, 1.0f}, 
        {-1.0f, -1.0f, 1.0f, 1.0f}, 
        { 1.0f, -1.0f, 1.0f, 1.0f}
    };
    constexpr vec2 tex_coords[num_corners]{ 
        {1.0f, 1.0f},  
        {0.0f, 1.0f},  
        {0.0f, 0.0f}, 
        {1.0f, 0.0f}
    };

    const vec3 normal{tr * vec4(0, 0, 1, 0)};

    int n = int(m_vertices.size());

    for (int i = 0; i < num_corners; ++i) {
        m_vertices.emplace_back(tr * verts[i]);
        m_textures.emplace_back(tex_coords[i]);
        m_normals .emplace_back(normal);
    }

    add_tris_for_quad({n, n + 1, n + 2, n + 3 });
}

void fields_engine::mesh::add_cube() {
    constexpr float rot_90 = glm::radians(90.0f);
    constexpr vec3 i{ 1, 0, 0 };
    constexpr vec3 j{ 0, 1, 0 };

    constexpr mat4 face_mat(1);

    // Add 6 faces as rotations of face_mat
    add_plane(face_mat);
    add_plane(glm::rotate(face_mat, rot_90, i));
    add_plane(glm::rotate(face_mat, -rot_90, i));
    add_plane(glm::rotate(face_mat, rot_90, j));
    add_plane(glm::rotate(face_mat, -rot_90, j));
    add_plane(glm::rotate(face_mat, 2 * rot_90, i));
}

void fields_engine::mesh::add_tris_for_quad(ivec4 const& idcs) {
    m_triangles.emplace_back(idcs.x, idcs.y, idcs.z);
    m_triangles.emplace_back(idcs.x, idcs.z, idcs.w);
}
