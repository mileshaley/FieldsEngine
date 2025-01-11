/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: material.cpp                                                        
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "material.h"

#include "serial_conversion.h"

/*~-------------------------------------------------------------------------~*\
 * Material Definitions                                                      *
\*~-------------------------------------------------------------------------~*/

fields_engine::vis::material::material() 
	: m_diffuse_color(1.0f, 1.0f, 1.0f)
	, m_specular_color(0.5f, 0.5f, 0.5f)
	, m_shininess(1.0f)
{
}

fields_engine::vis::material::material(material const& other)
	: m_diffuse_color(other.m_diffuse_color)
	, m_specular_color(other.m_specular_color)
	, m_shininess(other.m_shininess)
{
}

/*~-------------------------------------------------------------------------~*\
 * Material Friend Definitions                                               *
\*~-------------------------------------------------------------------------~*/

void fields_engine::vis::from_json(json const& in, material& out) {
	TRY_JSON_READ(out.m_diffuse_color, in, "diffuse");
	TRY_JSON_READ(out.m_specular_color, in, "specular");
	TRY_JSON_READ(out.m_shininess, in, "shininess");

}
