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
{

}

fields_engine::component::component(component const& other) 
	: m_owner(nullptr)
	, m_parent(nullptr)
{
}

#ifdef EDITOR
bool fields_engine::component::display() {
	return false;
}
#endif // EDITOR

void fields_engine::component::attach_component(unique<component>&& comp) {
	component* comp_ptr = comp.get();
	m_owner->attach_component(move(comp));
	m_children.push_back(comp_ptr);
	comp_ptr->set_parent(this);
	comp_ptr->ref_transform().set_parent(m_transform.as_parent());
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
