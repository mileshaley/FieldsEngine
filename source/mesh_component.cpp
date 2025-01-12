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

fields_engine::mesh_component::mesh_component()
	: spatial_component()
    , m_mesh(make_box<vis::mesh>())
    , m_material(nullptr)
{}

fields_engine::mesh_component::mesh_component(mesh_component const& other)
    : spatial_component(other)
    , m_mesh(make_box<vis::mesh>(*other.m_mesh))
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

    /// TODO: move these to physical component and standardize it for all physical components
    GLint loc = shader.uniform_location("ModelTr");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    VIS_VERIFY;
    /// ???
    loc = shader.uniform_location("NormalTr");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(inverse));
    VIS_VERIFY;

    if (m_material) {
        m_material->use(shader);
    } else {
        /// TODO: hand
    }
    m_mesh->draw();
}