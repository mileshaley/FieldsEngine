/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: mesh.cpp                                                            *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "mesh.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "graphics.h"
#include "asset_manager.h"
#include "material.h"

/*~-------------------------------------------------------------------------~*\
 * Mesh Definitions                                                          *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {

    mesh::mesh()
        : m_positions()
        , m_tex_uvs()
        , m_normals()
        , m_tangents()
        , m_triangles()
        , m_prim_type(primitive_type::none)
        , m_vao_id(0)
#if EDITOR // Editor data
        , m_default_material(nullptr)
#endif // EDITOR
    {}

    mesh::mesh(mesh const& other)
        : m_positions(other.m_positions)
        , m_tex_uvs(other.m_tex_uvs)
        , m_normals(other.m_normals)
        , m_tangents(other.m_tangents)
        , m_triangles(other.m_triangles)
        , m_prim_type(other.m_prim_type)
        , m_vao_id(0)
#if EDITOR // Editor data
        , m_default_material(nullptr)
#endif // EDITOR
    {
        if (other.m_vao_id != 0) {
            generate();
        }
    }

    mesh::~mesh() {
        if (m_vao_id != 0) {
            glDeleteBuffers(1, &m_vao_id);
            VIS_VERIFY;
        }
    }

    template<glm::length_t L, typename T, glm::qualifier Q>
    static void copy_unflatten_vec_buf(json const& in, vector<vec<L, T, Q>>& out) {
        for (size_t i = 0; i < in.size(); i += L) {
            if constexpr (L == 2) {
                out.push_back(vec<L, T, Q>(in[i], in[i + 1]));
            } else if constexpr (L == 3) {
                out.push_back(vec<L, T, Q>(in[i], in[i + 1], in[i + 2]));
            } else if constexpr (L == 4) {
                out.push_back(vec<L, T, Q>(in[i], in[i + 1], in[i + 2], in[1 + 3]));
            }
        }
    }

    void mesh::read(json const& in) {
        auto prim_it = in.find("primitive");
        if (prim_it != in.end()) {
            m_prim_type = static_cast<mesh::primitive_type>((*prim_it)["type"]);
            switch (m_prim_type) {
            case mesh::primitive_type::cube:
                add_cube();
                break;
            case mesh::primitive_type::cylinder:
                add_cylinder((*prim_it)["divisions"], (*prim_it)["height"]);
                break;
            case mesh::primitive_type::pyramid:
                add_pyramid((*prim_it)["divisions"], (*prim_it)["height"]);
                break;
            case mesh::primitive_type::sphere:
                add_sphere((*prim_it)["divisions"]);
                break;
            case primitive_type::none: FE_FALLTHROUGH
            default:
                /// TODO: Handle this error properly
                FE_FAILED_ASSERT("Failed to read unknown primitive mesh type");
            }

            generate();
        } else {

            json const& in_positions = in["positions"];
            json const& in_normals = in["normals"];
            json const& in_tex_uvs = in["tex_uvs"];
            json const& in_tangents = in["tangents"]; /// Empty for now
            json const& in_triangles = in["triangles"];
            json const& in_sections = in["sections"];

            copy_unflatten_vec_buf(in_positions, m_positions);
            copy_unflatten_vec_buf(in_normals, m_normals);
            copy_unflatten_vec_buf(in_tex_uvs, m_tex_uvs);
            copy_unflatten_vec_buf(in_tangents, m_tangents);
            copy_unflatten_vec_buf(in_triangles, m_triangles);

            for (size_t i = 0; i < in_sections.size(); ++i) {
                m_sections.push_back(mesh::section{
                   in_sections[i]["first_index"],
                   in_sections[i]["index_count"],
                   in_sections[i]["material_index"]
                });
            }

            generate();
        }

#if EDITOR
        auto default_mat_it = in.find("default_material");
        if (default_mat_it != in.end()) {
            m_default_material = get_asset<material>(*default_mat_it);
        }
#endif
    }

    void mesh::write(json& out) const {
        if (m_prim_type == mesh::primitive_type::none) {
            /// TODO: Write vertices here
            return;
        }
        json& prim_json = out["primitive"];
        prim_json["type"] = static_cast<int>(m_prim_type);
        switch (m_prim_type) {
        case mesh::primitive_type::cylinder:
            out["divisions"] = m_divisions;
            out["height"] = m_height;
            break;
        case mesh::primitive_type::pyramid:
            out["divisions"] = m_divisions;
            out["height"] = m_height;
            break;
        case mesh::primitive_type::sphere:
            out["divisions"] = m_divisions;
            break;
        case primitive_type::cube:
            break;
        case primitive_type::none: FE_FALLTHROUGH // Not possible
        default:
            break;
        }
    }

    void mesh::draw() const {
        glBindVertexArray(m_vao_id);
        VIS_VERIFY;
        if (m_sections.empty()) {
            glDrawElements(GL_TRIANGLES,
                GLsizei(m_triangles.size() * 3),
                GL_UNSIGNED_INT,
                nullptr
            );
            VIS_VERIFY;
        } else {
            for (section const& section : m_sections) {
                glDrawElements(GL_TRIANGLES,
                    GLsizei(section.index_count * 3),
                    GL_UNSIGNED_INT,
                    reinterpret_cast<void*>(section.first_index * 3llu)
                );
                VIS_VERIFY;
            }
        }
      
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
            m_positions.size() * sizeof(vec3),
            glm::value_ptr(m_positions[0]), // float ptr
            GL_STATIC_DRAW);
        VIS_VERIFY;
        glEnableVertexAttribArray(0);
        VIS_VERIFY;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
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

        if (!m_tex_uvs.empty()) {

            GLuint tex_buf = 0;
            glGenBuffers(1, &tex_buf);
            VIS_VERIFY;
            glBindBuffer(GL_ARRAY_BUFFER, tex_buf);
            VIS_VERIFY;
            glBufferData(GL_ARRAY_BUFFER,
                m_tex_uvs.size() * sizeof(vec2),
                glm::value_ptr(m_tex_uvs[0]), // float ptr
                GL_STATIC_DRAW);
            VIS_VERIFY;
            glEnableVertexAttribArray(2);
            VIS_VERIFY;
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
            VIS_VERIFY;
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            VIS_VERIFY;
        }

        if (!m_tangents.empty()) {
            GLuint tan_buf = 0;
            glGenBuffers(1, &tan_buf);
            VIS_VERIFY;
            glBindBuffer(GL_ARRAY_BUFFER, tan_buf);
            VIS_VERIFY;
            glBufferData(GL_ARRAY_BUFFER,
                m_normals.size() * sizeof(vec3),
                glm::value_ptr(m_tangents[0]), // float ptr
                GL_STATIC_DRAW);
            VIS_VERIFY;
            glEnableVertexAttribArray(3);
            VIS_VERIFY;
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
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

    void mesh::reset() {
        if (m_vao_id != 0) {
            glDeleteBuffers(1, &m_vao_id);
            VIS_VERIFY;
        }
        m_vao_id = 0;
        m_positions.clear();
        m_normals.clear();
        m_tex_uvs.clear();
        m_triangles.clear();
        m_tangents.clear();
        m_sections.clear();
    }

    void mesh::add_face(mat4 const& tr) {
        constexpr int num_corners = 4;
        constexpr vec4 positions[num_corners]{
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

        // Insert num_corners amount of duplicate attributes now
        m_normals.insert(m_normals.end(), num_corners, static_cast<vec3 const&>(tr * vec4(0, 0, 1, 0)));
        m_tangents.insert(m_tangents.end(), num_corners, static_cast<vec3 const&>(tr * vec4(1, 0, 0, 0)));

        const int n = int(m_positions.size());
        for (int i = 0; i < num_corners; ++i) {
            m_positions.emplace_back(tr * positions[i]);
            m_tex_uvs.emplace_back(tex_coords[i]);
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

    void mesh::add_sphere(int divisions) {
        m_divisions = divisions;
    }

    void mesh::add_cylinder(int sides, float height) {
        m_divisions = sides;
        m_height = height;
        const float half_height = height * 0.5f;
        const vec3 top_mid_pos{ 0, 0, half_height};
        const vec3 bot_mid_pos{ 0, 0, -half_height};
        constexpr vec3 top_norm{ 0, 0, 1 };
        constexpr vec3 bot_norm{ 0, 0, -1 };

        vec3 prev_bot_pos{ 0, 0.5f, -half_height};
        // Offset by 1 (same # iterations) to make use of prev_bot_pos
        for (int i = 1; i < sides + 1; ++i) {
            constexpr float two_pi = glm::pi<float>() * 2.0f;
            const float percent = i / float(sides);
            const vec3 bot_pos{
                sin(percent * two_pi) * 0.5f,
                cos(percent * two_pi) * 0.5f,
                -half_height
            };
            vec3 top_pos = bot_pos;
            vec3 prev_top_pos = prev_bot_pos;
            top_pos.z *= -1;
            prev_top_pos.z *= -1;

            // Side quad

            int n = int(m_positions.size());

            m_positions.emplace_back(top_pos);
            m_positions.emplace_back(bot_pos);
            m_positions.emplace_back(prev_bot_pos);
            m_positions.emplace_back(prev_top_pos);

            m_tex_uvs.emplace_back(1, 1);
            m_tex_uvs.emplace_back(1, 0);
            m_tex_uvs.emplace_back(0, 0);
            m_tex_uvs.emplace_back(0, 1);

            // Vector from origin to midpoint of the outer quad is equal to the midpoint in this case
            const vec3 out_norm = (prev_bot_pos + top_pos) * 0.5f;

            m_normals.insert(m_normals.end(), 4, out_norm);

            sequential_tris_for_quad(n);

            // Top of cylinder

            n = int(m_positions.size());
            m_positions.emplace_back(top_pos);
            m_positions.emplace_back(prev_top_pos);
            m_positions.emplace_back(top_mid_pos);

            m_tex_uvs.emplace_back(top_pos + vec3{ 0.5f });
            m_tex_uvs.emplace_back(prev_top_pos + vec3{ 0.5f });
            m_tex_uvs.emplace_back(top_mid_pos + vec3{ 0.5f });

            m_normals.insert(m_normals.end(), 3, top_norm);

            sequential_tris(n);

            // Bottom of cylinder

            n = int(m_positions.size());

            m_positions.emplace_back(bot_pos);
            m_positions.emplace_back(bot_mid_pos);
            m_positions.emplace_back(prev_bot_pos);

            m_tex_uvs.emplace_back(bot_pos + vec3{ 0.5f });
            m_tex_uvs.emplace_back(bot_mid_pos + vec3{ 0.5f });
            m_tex_uvs.emplace_back(prev_bot_pos + vec3{ 0.5f });

            m_normals.insert(m_normals.end(), 3, bot_norm);

            sequential_tris(n);

            prev_bot_pos = bot_pos;
        }
    }

    void mesh::add_pyramid(int sides, float height) {
        m_divisions = sides;
        m_height = height;
        constexpr vec3 bot_norm{ 0, 0, -1 };
        const float half_height = height * 0.5f;
        const vec3 bot_mid_pos{ 0, 0, -half_height};
        const vec3 top_mid_pos{ 0, 0, half_height};

        vec3 prev_pos{ 0, 0.5f, -half_height};
        // Offset by 1 (same # iterations) to make use of prev_pos
        for (int i = 1; i < sides + 1; ++i) {
            constexpr float two_pi = glm::pi<float>() * 2.0f;
            const float percent = i / float(sides);
            const vec3 pos{
                sin(percent * two_pi) * 0.5f,
                cos(percent * two_pi) * 0.5f,
                -half_height
            };

            int n = int(m_positions.size());
            // Top triangle
            m_positions.emplace_back(pos);
            m_positions.emplace_back(prev_pos);
            m_positions.emplace_back(top_mid_pos);

            m_tex_uvs.emplace_back(pos + vec3{ 0.5f });
            m_tex_uvs.emplace_back(prev_pos + vec3{ 0.5f });
            m_tex_uvs.emplace_back(top_mid_pos + vec3{ 0.5f });
            
            //std::cout << "Pyramid [i=" << i << "/" << (sides + 1) << "]" << std::endl
            //          << "Top UV [pos]:      " << vec2(pos) << std::endl
            //          << "Top UV [prev_pos]: " << vec2(prev_pos) << std::endl
            //          << "Top UV [top_mid_pos]:  " << vec2(top_mid_pos) << std::endl;

            const vec3 top_norm = 
                glm::cross(vec3(top_mid_pos - prev_pos), vec3(pos - prev_pos));
            m_normals.insert(m_normals.end(), 3, top_norm);

            // Bottom triangle
            m_positions.emplace_back(pos);
            m_positions.emplace_back(bot_mid_pos);
            m_positions.emplace_back(prev_pos);

            m_tex_uvs.emplace_back(pos + vec3{ 0.5f });
            m_tex_uvs.emplace_back(bot_mid_pos + vec3{ 0.5f });
            m_tex_uvs.emplace_back(prev_pos + vec3{ 0.5f });

            m_normals.insert(m_normals.end(), 3, bot_norm);

            sequential_tris(n);
            sequential_tris(n + 3);

            prev_pos = pos;
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
