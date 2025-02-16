/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: physical_component.cpp                                              *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "physical_component.h"

/*~-------------------------------------------------------------------------~*\
 * Physical Component Definitions                                            *
\*~-------------------------------------------------------------------------~*/

fields_engine::physical_component::physical_component() 
	: spatial_component()
{
}

fields_engine::physical_component::physical_component(physical_component const& other) 
	: spatial_component(other)
{
}

fields_engine::physical_component::~physical_component() = default;


