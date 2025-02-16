/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: entity.cpp                                                          *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
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

fields_engine::entity::entity() 
	: m_name()
	, m_basic_components()
	, m_spatial_components()
	, m_root_component(nullptr)
{
}

fields_engine::entity::entity(string_view name)
	: entity(name, make_own<spatial_component>())
{
}

fields_engine::entity::entity(string_view name, own<spatial_component>&& root_component)
	: m_name(name)
	, m_basic_components()
	, m_spatial_components()
	, m_root_component(nullptr)
{
	attach_spatial_component(move(root_component));
}

fields_engine::entity::entity(own<spatial_component>&& root_component)
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
	for (own<component> const& comp : m_basic_components) {
		comp->init();
	}
	m_root_component->init_all();
}

void fields_engine::entity::tick(float dt) {
	for (own<component> const& comp : m_basic_components) {
		comp->tick(dt);
	}
	m_root_component->tick_all(dt);
}

void fields_engine::entity::draw(vis::shader const& shader) const {
	/// TODO: Remove
	GLint loc = shader.uniform_location("objectId");
	glUniform1i(loc, 5);
	VIS_VERIFY;

	//for (own<component> const& comp : m_basic_components) {
	//	comp->draw(shader);
	//}
	m_root_component->draw_all(shader);
}

void fields_engine::entity::exit() {
	// Reverse order of init()
	m_root_component->exit_all();
	for (auto it = m_basic_components.rbegin(); it != m_basic_components.rend(); ++it) {
		(*it)->exit();
	}
}

void fields_engine::entity::read(json const& in) {
	m_name = in.at("name");
	auto root_it = in.find("root");
	FE_ASSERT(root_it != in.end(), "Root components are required for all entities");
	json const& in_root = *root_it;
	attach_spatial_component(make_from_type_name<spatial_component>(in_root["type"]));
	m_root_component->read_all(in_root);
	auto basic_it = in.find("basic_components");
	if (basic_it != in.end()) {
		for (json in_basic : *basic_it) {
			own<component> basic =
				make_from_type_name<spatial_component>(in_basic["type"]);
			component* p_basic = basic.get();
			attach_basic_component(move(basic));
			p_basic->read(in_basic);
		}
	}
	
}

void fields_engine::entity::write(json& out) const {
	out["name"] = m_name;
	json& out_root = out["root"];
	out_root["type"] = m_root_component->get_type_name();
	m_root_component->write_all(out_root);
	if (!m_basic_components.empty()) {
		json& out_basics = out["basic_components"] = json::array();
		for (own<component> const& basic : m_basic_components) {
			json& out_basic = out_basics.emplace_back();
			out_basic["type"] = basic->get_type_name();
			basic->write(out_basic);
		}
	}
}

#ifdef EDITOR
bool fields_engine::entity::display() {
	bool modif = false;
	ImGui::Text(m_name.c_str());

	for (own<spatial_component> const& comp : m_spatial_components) {
		ImGui::PushID(comp.get());
		if (comp.get() == m_root_component) {
			ImGui::SeparatorText((string(comp->get_type_name()) + " (root)").c_str());
		} else {
			ImGui::SeparatorText(comp->get_type_name().data());
		}
		modif |= comp->display();
		ImGui::PopID();
	}
	for (own<component> const& comp : m_basic_components) {
		ImGui::PushID(comp.get());
		ImGui::SeparatorText(comp->get_type_name().data());
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

fe::transformer& fields_engine::entity::ref_transform() {
	return m_root_component->ref_transform();
}
fe::transformer const& fields_engine::entity::ref_transform() const {
	return m_root_component->ref_transform();
}

fe::spatial_component* fields_engine::entity::get_root() {
	return m_root_component;
}

fe::spatial_component const* fields_engine::entity::get_root() const {
	return m_root_component;
}

fe::component& fields_engine::entity::attach_basic_component(own<component>&& comp) {
	component* comp_ptr = comp.get();
	acquire_basic_component(move(comp));
	return *comp_ptr;
}

fe::spatial_component& fields_engine::entity::attach_spatial_component(own<spatial_component>&& comp) {
	spatial_component* comp_ptr = comp.get();
	acquire_spatial_component(move(comp));
	if (m_root_component) {
		m_root_component->adopt_owned_component(comp_ptr);
	} else {
		m_root_component = comp_ptr;
	}
	return *comp_ptr;
}

void fields_engine::entity::acquire_basic_component(own<component>&& comp_to_own) {
	comp_to_own->set_owner(this);
	m_basic_components.emplace_back(move(comp_to_own));
}

void fields_engine::entity::acquire_spatial_component(own<spatial_component>&& comp_to_own) {
	comp_to_own->set_owner(this);
	m_spatial_components.emplace_back(move(comp_to_own));
}

void fields_engine::entity::set_root(spatial_component* new_root) {
	m_root_component = new_root;
}
