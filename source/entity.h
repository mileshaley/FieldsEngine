/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: entity.h                                                            *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "transform.h"

namespace fields_engine {
	class component;
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Entity Class                                                              *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class entity {
	public:
		entity();
		entity(entity const& other);

		~entity();

		void tick(float dt);
		void render() const;
	
	private:
		transform m_transform;
		dyn_arr<unique_ptr<component>> m_components;
		component* m_root_component;
	};

} // namespace fields_engine
