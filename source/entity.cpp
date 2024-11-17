/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: entity.cpp                                                          *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "entity.h"
#include "component.h"

/// TODO: remove
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"
#include "graphics.h"

/*~-------------------------------------------------------------------------~*\
 * Entity Definitions                                                        *
\*~-------------------------------------------------------------------------~*/

fields_engine::entity::entity() 
	: m_transform()
	, m_components()
	, m_root_component(nullptr)
{}

fields_engine::entity::entity(entity const& other)
	: m_transform(other.m_transform)
	, m_components()
	, m_root_component(nullptr)
{
	// Clone all components from other into this entity
	m_components.reserve(other.m_components.size());
	for (auto const& comp : m_components) {
		m_components.emplace_back(move(comp->clone()));
		if (comp.get() == other.m_root_component) {
			m_root_component = m_components.back().get();
		}
	}
}

fields_engine::entity::~entity() {

}

void fields_engine::entity::tick(float dt) {
	for (auto const& comp : m_components) {
		comp->tick(dt);
	}
}

void fields_engine::entity::render(graphics::shader const& shader) const {
	const vec3 diffuse_color{ 1.0f, 0.5f, 0.2f };
	const vec3 specular_color{ 0.5f, 0.5f, 0.1f };
	const float shininess = 3.0f;
	const mat4& matrix = m_transform.world_matrix();
	const mat4 inverse = glm::inverse(matrix);

	GLint loc = shader.uniform_location("diffuse");
	glUniform3fv(loc, 1, glm::value_ptr(diffuse_color));
	FE_GL_VERIFY;

	loc = shader.uniform_location("specular");
	glUniform3fv(loc, 1, glm::value_ptr(specular_color));
	FE_GL_VERIFY;
	loc = shader.uniform_location("shininess");
	glUniform1f(loc, shininess);
	FE_GL_VERIFY;
	loc = shader.uniform_location("objectId");
	glUniform1i(loc, 5);
	FE_GL_VERIFY;


	loc = shader.uniform_location("ModelTr");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
	FE_GL_VERIFY;
	/// ???
	loc = shader.uniform_location("NormalTr");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(inverse));
	FE_GL_VERIFY;


	loc = shader.uniform_location("texScale");
	glUniform2fv(loc, 1, glm::value_ptr(vec2(1, 1)));
	FE_GL_VERIFY;
	loc = shader.uniform_location("texRot");
	glUniform1f(loc, 1);
	FE_GL_VERIFY;
	loc = shader.uniform_location("hasTexture");
	glUniform1i(loc, 0);
	FE_GL_VERIFY;
	loc = shader.uniform_location("hasNormal");
	glUniform1i(loc, 0);
	FE_GL_VERIFY;

	for (auto const& comp : m_components) {
		comp->render();
	}
}

void fields_engine::entity::attach_component(unique_ptr<component>&& comp) {
	m_components.emplace_back(move(comp));
}
