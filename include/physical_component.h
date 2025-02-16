/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: physical_component.h                                                *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "spatial_component.h" // Inheritance

/*~-------------------------------------------------------------------------~*\
 * Physical Component Class                                                  *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	/// TODO: make physical_component abstract
	class physical_component : public spatial_component {
		FE_CLASS_BODY(physical_component, spatial_component);
	public: // Lifecycle
		physical_component();
		physical_component(physical_component const& other);
		~physical_component() override;

	public:


	private:

	}; // class physical_component

} // namespace fields_engine
