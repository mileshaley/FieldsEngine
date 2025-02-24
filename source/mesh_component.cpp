/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             * 
 *~-------------------------------------------------------------------------~* 
 * File: mesh_component.cpp                                                  * 
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * Includes                                                                  *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "mesh_component.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "graphics.h"
#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "asset_manager.h"

/*~-------------------------------------------------------------------------~*\
 * Mesh Component Method Definitions                                         *
\*~-------------------------------------------------------------------------~*/

fields_engine::mesh_component::mesh_component()
	: physical_component()
    , m_mesh(nullptr)
    , m_material(nullptr)
{}

fields_engine::mesh_component::mesh_component(mesh_component const& other)
    : physical_component(other)
    , m_mesh(other.m_mesh)
    , m_material(other.m_material)
{}

fields_engine::mesh_component::~mesh_component() = default;

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
        /// TODO: hand what????
        /// TODO: hand
    }

    /// TODO: remove the possibility for m_mesh to be null
    if (m_mesh) {
        m_mesh->draw();
    }
}

void fields_engine::mesh_component::read(json const& in) {
    auto mesh_it = in.find("mesh");
    if (mesh_it != in.end()) {
        m_mesh = get_asset<vis::mesh>(*mesh_it);
    }

    auto material_it = in.find("material");
    if (material_it != in.end()) {
        m_material = get_asset<vis::material>(*material_it);
    }
}

void fields_engine::mesh_component::write(json& out) const {
    if (m_mesh) {
        out["mesh"] = m_mesh->get_name();
    }

    if (m_material) {
        out["material"] = m_material->get_name();
    }
}