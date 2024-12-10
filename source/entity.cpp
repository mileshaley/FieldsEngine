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
	, m_basic_components()
	, m_spatial_components()
	, m_root_component(nullptr)
{
	attach_spatial_component(move(root_component));
}

fields_engine::entity::entity(unique<spatial_component>&& root_component)
	: m_name()
	, m_basic_components()
	, m_spatial_components()
	, m_root_component(nullptr)
{
	attach_spatial_component(move(root_component));
}

fields_engine::entity::entity(entity const& other)
	: m_name(other.m_name)
	, m_basic_components()
	, m_spatial_components()
	, m_root_component(nullptr)
{
	// Clone all components from other into this entity
	m_basic_components.reserve(other.m_basic_components.size());
	m_spatial_components.reserve(other.m_spatial_components.size());

	for (auto const& comp : other.m_basic_components) {
		acquire_basic_component(clone(*comp));
	}

	other.m_root_component->deep_copy_into_entity(*this);
}

fields_engine::entity::~entity() {
}

void fields_engine::entity::init() {
	for (unique<component> const& comp : m_basic_components) {
		comp->init();
	}
	m_root_component->init_all();
}

void fields_engine::entity::tick(float dt) {
	for (unique<component> const& comp : m_basic_components) {
		comp->tick(dt);
	}
	m_root_component->tick_all(dt);
}

void fields_engine::entity::draw(graphics::shader const& shader) const {
	/// TODO: Remove
	GLint loc = shader.uniform_location("objectId");
	glUniform1i(loc, 5);
	FE_GL_VERIFY;

	for (unique<component> const& comp : m_basic_components) {
		comp->draw(shader);
	}
	m_root_component->draw_all(shader);
}

void fields_engine::entity::exit() {
	// Reverse order of init()
	m_root_component->exit_all();
	for (auto it = m_basic_components.rbegin(); it != m_basic_components.rend(); ++it) {
		(*it)->exit();
	}
}

#ifdef EDITOR
bool fields_engine::entity::display() {
	bool modif = false;
	ImGui::Text(m_name.c_str());

	for (unique<component> const& comp : m_basic_components) {
		ImGui::PushID(comp.get());
		if (comp.get() == m_root_component) {
			ImGui::SeparatorText((string(comp->component_name()) + " (root)").c_str());
		} else {
			ImGui::SeparatorText(comp->component_name().data());
		}
		modif |= comp->display();
		ImGui::PopID();
	}
	// Update basic components first
	for (unique<component> const& comp : m_basic_components) {
		ImGui::PushID(comp.get());
		ImGui::SeparatorText(comp->component_name().data());
		modif |= comp->display();
		ImGui::PopID();
	}
	return modif;
}

bool fields_engine::entity::component_display() {
	bool modif = false;

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

fe::component& fields_engine::entity::attach_basic_component(unique<component>&& comp) {
	component* comp_ptr = comp.get();
	acquire_basic_component(move(comp));
	return *comp_ptr;
}

fe::spatial_component& fields_engine::entity::attach_spatial_component(unique<spatial_component>&& comp) {
	spatial_component* comp_ptr = comp.get();
	acquire_spatial_component(move(comp));
	if (m_root_component) {
		m_root_component->adopt_owned_component(comp_ptr);
	} else {
		m_root_component = comp_ptr;
	}
	return *comp_ptr;
}

void fields_engine::entity::acquire_basic_component(unique<component>&& comp_to_own) {
	comp_to_own->set_owner(this);
	m_basic_components.emplace_back(move(comp_to_own));
}

void fields_engine::entity::acquire_spatial_component(unique<spatial_component>&& comp_to_own) {
	comp_to_own->set_owner(this);
	m_spatial_components.emplace_back(move(comp_to_own));
}

void fields_engine::entity::set_root(spatial_component* new_root) {
	m_root_component = new_root;
}
