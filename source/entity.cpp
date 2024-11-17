/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: entity.cpp                                                          *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "entity.h"
#include "component.h"

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

void fields_engine::entity::render() const {
	for (auto const& comp : m_components) {
		comp->render();
	}
}
