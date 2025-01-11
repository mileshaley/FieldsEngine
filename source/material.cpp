/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: material.cpp                                                        
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "material.h"

#include "serial_conversion.h"
#include "shader.h"
#include "texture.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "graphics.h"

/*~-------------------------------------------------------------------------~*\
 * Material Definitions                                                      *
\*~-------------------------------------------------------------------------~*/

fields_engine::vis::material::material() 
	: m_diffuse_color(1.0f, 1.0f, 1.0f)
	, m_specular_color(0.5f, 0.5f, 0.5f)
	, m_shininess(1.0f)
	, m_texture(nullptr)
	, m_normal_texture(nullptr)
{
}

fields_engine::vis::material::material(material const& other)
	: m_diffuse_color(other.m_diffuse_color)
	, m_specular_color(other.m_specular_color)
	, m_shininess(other.m_shininess)
	, m_texture(other.m_texture)
	, m_normal_texture(other.m_normal_texture)
{
}

void fields_engine::vis::material::use(shader const& shader) const {
    // Material settings for shader
    int loc = shader.uniform_location("diffuse");
    glUniform3fv(loc, 1, glm::value_ptr(m_diffuse_color));
    VIS_VERIFY;

    loc = shader.uniform_location("specular");
    glUniform3fv(loc, 1, glm::value_ptr(m_specular_color));
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

/*~-------------------------------------------------------------------------~*\
 * Material Friend Definitions                                               *
\*~-------------------------------------------------------------------------~*/

void fields_engine::vis::from_json(json const& in, material& out) {
	TRY_JSON_READ(out.m_diffuse_color, in, "diffuse");
	TRY_JSON_READ(out.m_specular_color, in, "specular");
	TRY_JSON_READ(out.m_shininess, in, "shininess");

}
