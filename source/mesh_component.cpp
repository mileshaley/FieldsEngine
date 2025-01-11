/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             * 
 *~-------------------------------------------------------------------------~* 
 * File: mesh_component.cpp                                                  * 
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "mesh_component.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "graphics.h"
#include "shader.h"
#include "texture.h"
#include "mesh.h"

namespace fields_engine {
    using namespace vis;
} // namespace fields_engine

fields_engine::mesh_component::mesh_component()
	: spatial_component()
    , m_mesh(make_box<mesh>())
    , m_texture(nullptr)
    , m_normal_texture(nullptr)
    , m_material()
{}

fields_engine::mesh_component::mesh_component(mesh_component const& other)
    : spatial_component(other)
    , m_mesh(make_box<mesh>(*other.m_mesh))
    , m_texture(other.m_texture)
    , m_normal_texture(other.m_normal_texture)
    , m_material(other.m_material)
{
    /// TODO: Copy texture when we are using resource manager
    //if (other.m_texture) {
    //    m_texture = make_box<texture>(other.m_texture);
    //}
    //if (other.m_normal_texture) {
    //    m_normal_texture = make_box<texture>(other.m_texture);
    //}
}

fields_engine::mesh_component::~mesh_component() {}

void fields_engine::mesh_component::draw(vis::shader const& shader) const {
    const mat4& matrix = ref_transform().world_matrix();
    const mat4 inverse = glm::inverse(matrix);

    GLint loc = shader.uniform_location("ModelTr");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    VIS_VERIFY;
    /// ???
    loc = shader.uniform_location("NormalTr");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(inverse));
    VIS_VERIFY;

    // Material settings for shader
    loc = shader.uniform_location("diffuse");
    glUniform3fv(loc, 1, glm::value_ptr(m_material.m_diffuse_color));
    VIS_VERIFY;

    loc = shader.uniform_location("specular");
    glUniform3fv(loc, 1, glm::value_ptr(m_material.m_specular_color));
    VIS_VERIFY;
    loc = shader.uniform_location("shininess");
    glUniform1f(loc, m_material.m_shininess);
    VIS_VERIFY;

    loc = shader.uniform_location("texScale");
    glUniform2fv(loc, 1, glm::value_ptr(vec2(1, 1)));
    VIS_VERIFY;
    loc = shader.uniform_location("texRot");
    glUniform1f(loc, 0.0f);
    VIS_VERIFY;

    if (m_texture) {
        m_texture->use();
        int loc = shader.uniform_location("tex");
        glUniform1i(loc, 0);
    }

    if (m_normal_texture) {
        m_normal_texture->use();
        int loc = shader.uniform_location("norm");
        glUniform1i(loc, 1);
    }

    loc = shader.uniform_location("hasTexture");
    glUniform1i(loc, int(m_texture != nullptr));
    VIS_VERIFY;

    loc = shader.uniform_location("hasNormal");
    glUniform1i(loc, int(m_normal_texture != nullptr));
    VIS_VERIFY;

    m_mesh->draw();
}

void fields_engine::mesh_component::set_texture(vis::texture const* new_texture) {
    m_texture = new_texture;
}

void fields_engine::mesh_component::set_normal_texture(vis::texture const* new_normal_texture) {
    m_normal_texture = new_normal_texture;
}