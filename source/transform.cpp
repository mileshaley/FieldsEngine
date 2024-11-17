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
	: m_data()
	, m_matrix(1)
	, m_dirty(true)
{}

fields_engine::transform::transform(transform_data const& data)
	: m_data(data)
	, m_matrix(1)
	, m_dirty(true)
{}

fe::mat4 const& fields_engine::transform::matrix() const {
	constexpr mat4 ident(1);
	if (m_dirty) {
		m_dirty = false;
		m_matrix = 
			glm::scale(
				glm::rotate(
					glm::rotate(
						glm::rotate(
							glm::translate(
								ident,
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
