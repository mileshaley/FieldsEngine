/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             * 
 *~-------------------------------------------------------------------------~* 
 * File: component.h                                                         * 
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Component Defines                                                         * 
\*~-------------------------------------------------------------------------~*/

#define FE_GEN_COMPONENT(p_subclass, p_parclass) FE_CLASS_BODY(p_subclass, p_parclass)

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "polymorphic.h"
#include "transform.h"

namespace fields_engine {
	class entity;
	/// TODO: remove
	namespace graphics {
		class shader;
	}
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Component Class                                                           *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class component {
		FE_DEFINING_CLASS_BODY(component);

		component();
		component(component const& other);
		virtual ~component() {}

		
		virtual void init() {}
		virtual void tick(float dt) {}
		virtual void draw(graphics::shader const& shader) const {}
		virtual void exit() {}

#ifdef EDITOR
		virtual bool display();
#endif // EDITOR

		void set_owner(entity* new_owner);
		entity* get_owner() const;

	private:
		entity* m_owner;
	};

} // namespace fields_engine
