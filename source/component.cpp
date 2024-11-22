/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: component.cpp                                                       *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "component.h"
#include "entity.h"

/*~-------------------------------------------------------------------------~*\
 * Component Definitions                                                     *
\*~-------------------------------------------------------------------------~*/

fields_engine::component::component()
	: m_owner(nullptr)
	, m_parent(nullptr)
	, m_transform()
{
	m_transform.set_owner(this);
}

fields_engine::component::component(component const& other) 
	: m_owner(nullptr)
	, m_parent(nullptr)
	, m_transform(other.m_transform)
{
	m_transform.set_owner(this);
}

void fields_engine::component::dirtify_transforms() const {
	m_transform.set_only_this_dirty();
	for (component* child : m_children) {
		child->dirtify_transforms();
	}
}

#ifdef EDITOR
bool fields_engine::component::display() {
	return false;
}
#endif // EDITOR

void fields_engine::component::attach_component(unique<component>&& comp) {
	adopt_owned_component(comp.get());
	m_owner->acquire_component(move(comp));
}

void fields_engine::component::adopt_owned_component(component* new_child) {
	m_children.push_back(new_child);
	new_child->set_parent(this);
}

void fields_engine::component::set_parent(component* new_parent) {
	m_parent = new_parent;
}

fe::component* fields_engine::component::get_parent() const {
	return m_parent;
}

void fields_engine::component::set_owner(entity* new_owner) {
	m_owner = new_owner;
}

fe::entity* fields_engine::component::get_owner() const {
	return m_owner;
}
