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
	/// TODO: remove
	namespace graphics {
		class shader; 
	}
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
		void render(graphics::shader const& shader) const;

		void attach_component(unique_ptr<component>&& comp);

		transform      & ref_transform()       { return m_transform; }
		transform const& ref_transform() const { return m_transform; }
	
	private:
		transform m_transform;
		dyn_arr<unique_ptr<component>> m_components;
		component* m_root_component;
	};

} // namespace fields_engine
