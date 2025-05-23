/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: material.cpp                                                        
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "material.h"

#include "serial_conversion.h"
#include "shader.h"
#include "texture.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "graphics.h"
#include "context.h"
#include "asset_manager.h"

/*~-------------------------------------------------------------------------~*\
 * Material Definitions                                                      *
\*~-------------------------------------------------------------------------~*/

fields_engine::vis::material::material() 
	: m_diffuse(1.0f, 1.0f, 1.0f)
	, m_specular(0.5f, 0.5f, 0.5f)
	, m_shininess(1.0f)
	, m_texture(nullptr)
	, m_normal_texture(nullptr)
{
}

fields_engine::vis::material::material(material const& other)
	: m_diffuse(other.m_diffuse)
	, m_specular(other.m_specular)
	, m_shininess(other.m_shininess)
	, m_texture(other.m_texture)
	, m_normal_texture(other.m_normal_texture)
{
}

void fields_engine::vis::material::read(json const& in) {
    try_read_json(m_diffuse, in, "diffuse");
    try_read_json(m_specular, in, "specular");
    try_read_json(m_shininess, in, "shininess");

    auto tex_it = in.find("texture");
    if (tex_it != in.end()) {
        set_texture(get_asset<vis::texture>(*tex_it));
    }

    auto norm_tex_it = in.find("normal_texture");
    if (norm_tex_it != in.end()) {
        set_normal_texture(get_asset<vis::texture>(*norm_tex_it));

    }
}

void fields_engine::vis::material::write(json& out) const {
    out["diffuse"] = m_diffuse;
    out["specular"] = m_specular;
    out["shininess"] = m_shininess;
    if (m_texture) {
        /// TODO: Implement this
        //out["texture"] = *in.m_texture;
        //out["normal_texture"] = *in.m_texture;
    }
}

void fields_engine::vis::material::use(shader const& shader) const {
    // Material settings for shader
    int loc = shader.uniform_location("diffuse");
    glUniform3fv(loc, 1, glm::value_ptr(m_diffuse));
    VIS_VERIFY;

    loc = shader.uniform_location("specular");
    glUniform3fv(loc, 1, glm::value_ptr(m_specular));
    VIS_VERIFY;
    loc = shader.uniform_location("shininess");
    glUniform1f(loc, m_shininess);
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
}

void fields_engine::vis::material::set_texture(texture const* new_texture) {
	m_texture = new_texture;
}

void fields_engine::vis::material::set_normal_texture(texture const* new_normal_texture) {
	m_normal_texture = new_normal_texture;
}
