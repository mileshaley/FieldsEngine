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
#include "shader.h"
#include "texture.h"
#include "mesh_resource.h"

namespace fields_engine {
    using namespace graphics;
} // namespace fields_engine

fields_engine::mesh::mesh()
	: spatial_component()
    , m_resource(make_unique<mesh_resource>())
    , m_texture(nullptr)
    , m_normal_texture(nullptr)
    , m_material()
{}

fields_engine::mesh::mesh(mesh const& other)
    : spatial_component(other)
    , m_resource(make_unique<mesh_resource>(*other.m_resource))
    , m_texture(nullptr)
    , m_normal_texture(nullptr)
    , m_material(other.m_material)
{
    /// TODO: Copy texture when we are using resource manager
    //if (other.m_texture) {
    //    m_texture = make_unique<texture>(other.m_texture);
    //}
    //if (other.m_normal_texture) {
    //    m_normal_texture = make_unique<texture>(other.m_texture);
    //}
}

fields_engine::mesh::~mesh() {}

void fields_engine::mesh::draw(graphics::shader const& shader) const {
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

    int has_texture = m_texture != nullptr;
    int has_normal_texture = m_normal_texture != nullptr;
    loc = shader.uniform_location("texScale");
    glUniform2fv(loc, 1, glm::value_ptr(vec2(1, 1)));
    FE_GL_VERIFY;
    loc = shader.uniform_location("texRot");
    glUniform1f(loc, 0);
    FE_GL_VERIFY;

    if (has_texture) {
        m_texture->use();
        loc = shader.uniform_location("tex");
        glUniform1i(loc, m_texture->get_unit());
    }
    loc = shader.uniform_location("hasTexture");
    glUniform1i(loc, has_texture);
    FE_GL_VERIFY;

    if (has_normal_texture) {
        m_normal_texture->use();
        loc = shader.uniform_location("norm");
        glUniform1i(loc, m_normal_texture->get_unit());
    }
    loc = shader.uniform_location("hasNormal");
    glUniform1i(loc, has_normal_texture);
    FE_GL_VERIFY;

    m_resource->draw();
}

void fields_engine::mesh::set_texture(unique<graphics::texture>&& new_texture) {
    m_texture = move(new_texture);
    if (m_texture != nullptr) {
        m_texture->set_unit(0);
    }
}

void fields_engine::mesh::set_normal_texture(unique<graphics::texture>&& new_normal_texture) {
    m_normal_texture = move(new_normal_texture);
    if (m_normal_texture != nullptr) {
        m_normal_texture->set_unit(1);
    }
}