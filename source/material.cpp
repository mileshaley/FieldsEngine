/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: material.cpp                                                        
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "material.h"

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
