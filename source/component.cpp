/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: component.cpp                                                       *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "component.h"
#include "entity.h"

/*~-------------------------------------------------------------------------~*\
 * Component Definitions                                                     *
\*~-------------------------------------------------------------------------~*/

fields_engine::component::component()
	: m_owner(nullptr)
{
}

fields_engine::component::component(component const& other) 
	: m_owner(nullptr)
{
}

#ifdef EDITOR
bool fields_engine::component::display() {
	return false;
}
#endif // EDITOR

void fields_engine::component::set_owner(entity* new_owner) {
	m_owner = new_owner;
}

fe::entity* fields_engine::component::get_owner() const {
	return m_owner;
}
