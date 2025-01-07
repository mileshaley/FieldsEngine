/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: mesh_resource_resource.cpp                                          *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "mesh_resource.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "graphics.h"

/*~-------------------------------------------------------------------------~*\
 * Mesh Resource Definitions                                                 *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {

    mesh::mesh()
        : m_vertices()
        , m_triangles()
        , m_textures()
        , m_normals()
        , m_vao_id(0)
    {}

    mesh::mesh(mesh const& other)
        : m_vertices(other.m_vertices)
        , m_triangles(other.m_triangles)
        , m_textures(other.m_textures)
        , m_normals(other.m_normals)
        , m_vao_id(0)
    {
        if (other.m_vao_id != 0) {
            generate();
        }
    }

    mesh::~mesh() {
        glDeleteBuffers(1, &m_vao_id);
        VIS_VERIFY;
    }

    void mesh::draw() const {
        glBindVertexArray(m_vao_id);
        VIS_VERIFY;
        glDrawElements(GL_TRIANGLES,
            GLsizei(m_triangles.size() * 3),
            GL_UNSIGNED_INT,
            0);
        VIS_VERIFY;
        glBindVertexArray(0);
        VIS_VERIFY;
    }

    void mesh::generate() {
        // Give this mesh_component a vertex object array
        glGenVertexArrays(1, &m_vao_id);
        glBindVertexArray(m_vao_id);
        // Generate vertex buffer
        GLuint vert_buf = 0;
        glGenBuffers(1, &vert_buf);
        VIS_VERIFY;
        glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
        VIS_VERIFY;
        glBufferData(GL_ARRAY_BUFFER,
            m_vertices.size() * sizeof(vec4),
            glm::value_ptr(m_vertices[0]), // float ptr
            GL_STATIC_DRAW);
        VIS_VERIFY;
        glEnableVertexAttribArray(0);
        VIS_VERIFY;
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        VIS_VERIFY;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        VIS_VERIFY;

        if (!m_normals.empty()) {
            GLuint norm_buf = 0;
            glGenBuffers(1, &norm_buf);
            VIS_VERIFY;
            glBindBuffer(GL_ARRAY_BUFFER, norm_buf);
            VIS_VERIFY;
            glBufferData(GL_ARRAY_BUFFER,
                m_normals.size() * sizeof(vec3),
                glm::value_ptr(m_normals[0]), // float ptr
                GL_STATIC_DRAW);
            VIS_VERIFY;
            glEnableVertexAttribArray(1);
            VIS_VERIFY;
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            VIS_VERIFY;
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            VIS_VERIFY;
        }

        if (!m_textures.empty()) {

            GLuint tex_buf = 0;
            glGenBuffers(1, &tex_buf);
            VIS_VERIFY;
            glBindBuffer(GL_ARRAY_BUFFER, tex_buf);
            VIS_VERIFY;
            glBufferData(GL_ARRAY_BUFFER,
                m_textures.size() * sizeof(vec2),
                glm::value_ptr(m_textures[0]), // float ptr
                GL_STATIC_DRAW);
            VIS_VERIFY;
            glEnableVertexAttribArray(2);
            VIS_VERIFY;
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
            VIS_VERIFY;
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            VIS_VERIFY;
        }

        // Generate triangle buffer
        GLuint tri_buf = 0;
        glGenBuffers(1, &tri_buf);
        VIS_VERIFY;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_buf);
        VIS_VERIFY;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            m_triangles.size() * sizeof(ivec3),
            glm::value_ptr(m_triangles[0]), // int ptr 
            GL_STATIC_DRAW);
        VIS_VERIFY;
        glBindVertexArray(0);
        VIS_VERIFY;
    }

    void mesh::add_face(mat4 const& tr) {
        constexpr int num_corners = 4;
        constexpr vec4 verts[num_corners]{
            { 0.5f,  0.5f, 0.5f, 1.0f},
            {-0.5f,  0.5f, 0.5f, 1.0f},
            {-0.5f, -0.5f, 0.5f, 1.0f},
            { 0.5f, -0.5f, 0.5f, 1.0f}
        };
        constexpr vec2 tex_coords[num_corners]{
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            {1.0f, 0.0f}
        };

        const vec3 normal{ tr * vec4(0, 0, 1, 0) };

        const int n = int(m_vertices.size());

        for (int i = 0; i < num_corners; ++i) {
            m_vertices.emplace_back(tr * verts[i]);
            m_textures.emplace_back(tex_coords[i]);
            m_normals.emplace_back(normal);
        }

        sequential_tris_for_quad(n);
    }

    void mesh::add_cube() {
        constexpr float rot_90 = glm::radians(90.0f);
        constexpr vec3 i{ 1, 0, 0 };
        constexpr vec3 j{ 0, 1, 0 };

        constexpr mat4 face_mat(1);

        // Add 6 faces as rotations of face_mat
        add_face(face_mat);
        add_face(glm::rotate(face_mat, rot_90, i));
        add_face(glm::rotate(face_mat, -rot_90, i));
        add_face(glm::rotate(face_mat, rot_90, j));
        add_face(glm::rotate(face_mat, -rot_90, j));
        add_face(glm::rotate(face_mat, 2 * rot_90, i));
    }

    void mesh::add_sphere(int subdivisions) {

    }

    void mesh::add_cylinder(int sides, float height) {
        const float half_height = height * 0.5f;
        const vec4 top_mid_vert{ 0, 0, half_height, 1 };
        const vec4 bot_mid_vert{ 0, 0, -half_height, 1 };
        constexpr vec3 top_norm{ 0, 0, -1 };
        constexpr vec3 bot_norm{ 0, 0, -1 };

        vec4 prev_bot_vert{ 0, 0.5f, -half_height, 1 };
        // Offset by 1 (same # iterations) to make use of prev_bot_vert
        for (int i = 1; i < sides + 1; ++i) {
            constexpr float two_pi = glm::pi<float>() * 2.0f;
            const float percent = i / float(sides);
            const vec4 bot_vert{
                sin(percent * two_pi) * 0.5f,
                cos(percent * two_pi) * 0.5f,
                -half_height,
                1
            };
            vec4 top_vert = bot_vert;
            vec4 prev_top_vert = prev_bot_vert;
            top_vert.z *= -1;
            prev_top_vert.z *= -1;

            // Side quad

            int n = int(m_vertices.size());

            m_vertices.emplace_back(top_vert);
            m_vertices.emplace_back(prev_top_vert);
            m_vertices.emplace_back(prev_bot_vert);
            m_vertices.emplace_back(bot_vert);

            m_textures.emplace_back(1, 1);
            m_textures.emplace_back(0, 1);
            m_textures.emplace_back(0, 0);
            m_textures.emplace_back(1, 0);

            // Vector from origin to midpoint of the outer quad is equal to the midpoint in this case
            const vec3 out_norm = (prev_bot_vert + top_vert) * 0.5f;

            m_normals.emplace_back(out_norm);
            m_normals.emplace_back(out_norm);
            m_normals.emplace_back(out_norm);
            m_normals.emplace_back(out_norm);

            sequential_tris_for_quad(n);

            // Top of cylinder

            n = int(m_vertices.size());
            m_vertices.emplace_back(top_mid_vert);
            m_vertices.emplace_back(prev_top_vert);
            m_vertices.emplace_back(top_vert);

            m_textures.emplace_back(1, 0);
            m_textures.emplace_back(0, 1);
            m_textures.emplace_back(0, 0);

            m_normals.emplace_back(top_norm);
            m_normals.emplace_back(top_norm);
            m_normals.emplace_back(top_norm);

            sequential_tris(n);

            // Bottom of cylinder

            n = int(m_vertices.size());

            m_vertices.emplace_back(bot_mid_vert);
            m_vertices.emplace_back(prev_bot_vert);
            m_vertices.emplace_back(bot_vert);

            m_textures.emplace_back(1, 0);
            m_textures.emplace_back(0, 1);
            m_textures.emplace_back(0, 0);

            m_normals.emplace_back(bot_norm);
            m_normals.emplace_back(bot_norm);
            m_normals.emplace_back(bot_norm);

            sequential_tris(n);

            prev_bot_vert = bot_vert;
        }
    }

    void mesh::add_pyramid(int sides, float height) {
        constexpr vec3 bot_norm{ 0, 0, -1 };
        const float half_height = height * 0.5f;
        const vec4 bot_middle_vert{ 0, 0, -half_height, 1 };
        const vec4 tip_vert{ 0, 0, half_height, 1 };

        vec4 prev_vert{ 0, 0.5f, -half_height, 1 };
        // Offset by 1 (same # iterations) to make use of prev_vert
        for (int i = 1; i < sides + 1; ++i) {
            constexpr float two_pi = glm::pi<float>() * 2.0f;
            const float percent = i / float(sides);
            const vec4 vert{
                sin(percent * two_pi) * 0.5f,
                cos(percent * two_pi) * 0.5f,
                -half_height,
                1
            };

            int n = int(m_vertices.size());
            m_vertices.emplace_back(tip_vert);
            m_vertices.emplace_back(prev_vert);
            m_vertices.emplace_back(vert);

            m_vertices.emplace_back(vert);
            m_vertices.emplace_back(prev_vert);
            m_vertices.emplace_back(bot_middle_vert);

            m_textures.emplace_back(1, 1);
            m_textures.emplace_back(0, 1);
            m_textures.emplace_back(1, 0);

            m_textures.emplace_back(1, 0);
            m_textures.emplace_back(0, 1);
            m_textures.emplace_back(0, 0);

            const vec3 top_norm = glm::cross(vec3(tip_vert - prev_vert), vec3(vert - prev_vert));
            m_normals.emplace_back(top_norm);
            m_normals.emplace_back(top_norm);
            m_normals.emplace_back(top_norm);

            m_normals.emplace_back(bot_norm);
            m_normals.emplace_back(bot_norm);
            m_normals.emplace_back(bot_norm);

            sequential_tris(n);
            sequential_tris(n + 3);

            prev_vert = vert;
        }
    }

    void mesh::sequential_tris(int i) {
        m_triangles.emplace_back(i, i + 1, i + 2);

    }

    void mesh::tris_for_quad(ivec4 const& indices) {
        m_triangles.emplace_back(indices.x, indices.y, indices.z);
        m_triangles.emplace_back(indices.x, indices.z, indices.w);
    }

    void mesh::sequential_tris_for_quad(int i) {
        tris_for_quad({ i, i + 1, i + 2, i + 3 });
    }

} // namespace fields_engine::vis
