/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: transform.cpp                                                       *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "transform.h"
#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtx/transform.hpp"

/*~-------------------------------------------------------------------------~*\
 * Transform Definitions                                                     *
\*~-------------------------------------------------------------------------~*/

fields_engine::transform::transform()
	: m_data({{ 0, 0, 0 }, { 0, 0, 0 }, {1, 1, 1}})
	, m_matrix(1)
	, m_dirty(true)
	, m_parent(nullptr)
{}

fields_engine::transform::transform(transform_data const& data)
	: m_data(data)
	, m_matrix(1)
	, m_dirty(true)
	, m_parent(nullptr)
{}

fe::mat4 const& fields_engine::transform::world_matrix() const {
	constexpr mat4 ident(1);
	if (m_dirty) {
		m_dirty = false;
		m_matrix = 
			glm::scale(
				glm::rotate(
					glm::rotate(
						glm::rotate(
							glm::translate(
								(m_parent ? m_parent->world_matrix() : ident),
								m_data.position
							), m_data.rotation.x,
							(vec3&)ident[0]
						), m_data.rotation.y,
						(vec3&)ident[1]
					), m_data.rotation.z,
					(vec3&)ident[2]
				), m_data.scale
			);
	}

	return m_matrix;
}

void fields_engine::transform::set_position(fe::vec3 const& new_position) {
	m_data.position = new_position;
}

fe::vec3 const& fields_engine::transform::get_position() const {
	return m_data.position;
}

void fields_engine::transform::set_scale(fe::vec3 const& new_scale) {
	m_data.scale = new_scale;
}

fe::vec3 const& fields_engine::transform::get_scale() const {
	return m_data.scale;
}

void fields_engine::transform::set_rotation(vec3 const& new_rotation) {
	m_data.rotation = new_rotation;
}

fe::vec3 const& fields_engine::transform::get_rotation() const {
	return m_data.rotation;
}
