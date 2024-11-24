/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: spatial_component.cpp                                               *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "spatial_component.h"
#include "entity.h"

/*~-------------------------------------------------------------------------~*\
 * Component Definitions                                                     *
\*~-------------------------------------------------------------------------~*/

fields_engine::spatial_component::spatial_component()
	: component()
	, m_transform()
{
	m_transform.set_owner(this);
}

fields_engine::spatial_component::spatial_component(spatial_component const& other)
	: component(other)
	, m_transform(other.m_transform)
{
	m_transform.set_owner(this);
}

void fields_engine::spatial_component::dirtify_transforms() const {
	m_transform.set_only_this_dirty();
	for (spatial_component* child : m_children) {
		child->dirtify_transforms();
	}
}

#ifdef EDITOR
bool fields_engine::spatial_component::display() {
	return m_transform.display();
}
#endif // EDITOR



fe::spatial_component& fields_engine::spatial_component::attach_component(unique<spatial_component>&& comp) {
	spatial_component* comp_ptr = comp.get();
	adopt_owned_component(comp.get());
	get_owner()->acquire_component(move(comp));
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
