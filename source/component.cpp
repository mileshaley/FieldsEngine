/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: component.cpp                                                       *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "component.h"

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
