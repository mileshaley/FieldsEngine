/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: entity.cpp                                                          *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "entity.h"
#include "spatial_component.h"

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

fields_engine::entity::entity(string_view name) 
	: entity(name, make_unique<spatial_component>())
{
}

fields_engine::entity::entity(string_view name, unique<spatial_component>&& root_component)
	: m_name(name)
	, m_components()
	, m_root_component(nullptr)
{
	m_root_component = root_component.get();
	acquire_component(move(root_component));
}

fields_engine::entity::entity(unique<spatial_component>&& root_component)
	: m_name()
	, m_components()
	, m_root_component(nullptr)
{
	m_root_component = root_component.get();
	acquire_component(move(root_component));
}

fields_engine::entity::entity(entity const& other)
	: m_name(other.m_name)
	, m_components()
	, m_root_component(nullptr)
{
	// Clone all components from other into this entity
	m_components.reserve(other.m_components.size());
	for (auto const& comp : m_components) {
		m_components.emplace_back(move(comp->clone()));
		if (comp.get() == other.m_root_component) {
			m_root_component = reinterpret_cast<spatial_component*>(m_components.back().get());
		}
	}
}

fields_engine::entity::~entity() {
	
}

void fields_engine::entity::init() {
	for (unique_cr<component> comp : m_components) {
		comp->init();
	}
}

void fields_engine::entity::tick(float dt) {
	for (unique_cr<component> comp : m_components) {
		comp->tick(dt);
	}
}

void fields_engine::entity::draw(graphics::shader const& shader) const {

	GLint loc = shader.uniform_location("objectId");
	glUniform1i(loc, 5);
	FE_GL_VERIFY;

	for (unique_cr<component> comp : m_components) {
		comp->draw(shader);
	}
}

void fields_engine::entity::exit() {
	for (unique_cr<component> comp : m_components) {
		comp->exit();
	}
}

#ifdef EDITOR
bool fields_engine::entity::display() {
	bool modif = false;
	ImGui::Text(m_name.c_str());
	for (unique_cr<component> comp : m_components) {
		ImGui::PushID(comp.get());
		if (comp.get() == m_root_component) {
			ImGui::SeparatorText((string(comp->component_name()) + " (root)").c_str());
		} else {
			ImGui::SeparatorText(comp->component_name().data());
		}
		modif |= comp->display();
		ImGui::PopID();
	}
	
	return modif;
}

#endif // EDITOR

fe::string const& fields_engine::entity::get_name() const {
	return m_name;
}
fe::string& fields_engine::entity::ref_name() {
	return m_name;
}

fe::transform& fields_engine::entity::ref_transform() {
	return m_root_component->ref_transform();
}
fe::transform const& fields_engine::entity::ref_transform() const {
	return m_root_component->ref_transform();
}

fe::spatial_component* fields_engine::entity::get_root() {
	return m_root_component;
}

fe::spatial_component const* fields_engine::entity::get_root() const {
	return m_root_component;
}

void fields_engine::entity::acquire_component(unique<component>&& comp_to_own) {
	comp_to_own->set_owner(this);
	m_components.emplace_back(move(comp_to_own));
}

fe::component& fields_engine::entity::attach_component(unique<component>&& comp) {
	component* comp_ptr = comp.get();
	spatial_component* spatial = dynamic_cast<spatial_component*>(comp.get());
	if (spatial) {
		m_root_component->adopt_owned_component(spatial);
	}
	acquire_component(move(comp));
	return *comp_ptr;
}
