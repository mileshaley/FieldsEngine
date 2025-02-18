/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: spatial_component.cpp                                               *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "spatial_component.h"
#include "entity.h"

/*~-------------------------------------------------------------------------~*\
 * Spatial Component Lifecycle                                               *
\*~-------------------------------------------------------------------------~*/

fields_engine::spatial_component::spatial_component()
	: component()
	, m_transform()
	, m_parent(nullptr)
	, m_children()
{
	m_transform.set_owner(this);
}

fields_engine::spatial_component::spatial_component(spatial_component const& other)
	: component(other)
	, m_transform(other.m_transform)
	, m_parent(nullptr)
	, m_children()
{
	m_transform.set_owner(this);
}

/*~-------------------------------------------------------------------------~*\
 * Spatial Component Definitions                                             *
\*~-------------------------------------------------------------------------~*/

void fields_engine::spatial_component::dirtify_transformers() const {
	m_transform.set_only_this_dirty();
	for (spatial_component* child : m_children) {
		child->dirtify_transformers();
	}
}

void fields_engine::spatial_component::init_all() {
	init(); // Virtual
	for (spatial_component* child : m_children) {
		child->init_all();
	}
}

void fields_engine::spatial_component::tick_all(float dt) {
	tick(dt); // Virtual
	for (spatial_component* child : m_children) {
		child->tick_all(dt);
	}
}

void fields_engine::spatial_component::draw_all(vis::shader const& shader) const {
	draw(shader); // Virtual
	for (spatial_component const* child : m_children) {
		child->draw_all(shader);
	}
}

void fields_engine::spatial_component::exit_all() {
	// Reverse order of all operations in init_all()
	for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
		(*it)->exit_all();
	}
	exit(); // Virtual
}

void fields_engine::spatial_component::read_all(json const& in) {
	m_transform.read(in.at("transform"));
	read(in); // Virtual
	auto children_it = in.find("children");
	if (children_it != in.end()) {
		json const& in_children = *children_it;

		for (json const& in_child : in_children) {
			spatial_component& child = attach_spatial_component(
				make_from_type_name<spatial_component>(in_child.at("type")));
			child.read_all(in_child);
		}
	}
}

void fields_engine::spatial_component::write_all(json& out) const {
	write(out); // Virtual
	m_transform.write(out["transform"]);

	if (!m_children.empty()) {
		json& out_children = out["children"];
		for (spatial_component const* child : m_children) {
			json& out_child = out_children.emplace_back();
			out_child["type"] = child->get_type_name();
			child->write_all(out_child);
		}
	}
}

#ifdef EDITOR
bool fields_engine::spatial_component::display() {
	return m_transform.display();
}
#endif // EDITOR

fe::spatial_component& fields_engine::spatial_component::attach_spatial_component(own<spatial_component>&& comp) {
	spatial_component* comp_ptr = comp.get();
	adopt_owned_component(comp.get());
	get_owner()->acquire_spatial_component(move(comp));
	return *comp_ptr;
}

void fields_engine::spatial_component::adopt_owned_component(spatial_component* new_child) {
	m_children.push_back(new_child);
	new_child->set_parent(this);
}

void fields_engine::spatial_component::set_parent(spatial_component* new_parent) {
	m_parent = new_parent;
}

fe::spatial_component* fields_engine::spatial_component::get_parent() const {
	return m_parent;
}

void fields_engine::spatial_component::deep_copy_into_entity(entity& other_owner) const {
	own<spatial_component> new_root = clone(*this);
	// Relies on attach's ability to either set as root or attach to root	
	spatial_component& root =
		other_owner.attach_spatial_component(move(new_root));
	for (spatial_component* child : m_children) {
		child->deep_copy_into_entity_aux(other_owner, root);
	}
}

void fields_engine::spatial_component::deep_copy_into_entity_aux(entity& other_owner, spatial_component& other_parent) const {
	own<spatial_component> comp = clone(*this);
	spatial_component* comp_ptr = comp.get();
	other_owner.acquire_spatial_component(move(comp));
	other_parent.adopt_owned_component(comp_ptr);
	for (spatial_component* child : m_children) {
		child->deep_copy_into_entity_aux(other_owner, *comp_ptr);
	}
}
