/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             * 
 *~-------------------------------------------------------------------------~* 
 * File: component.h                                                         * 
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "polymorphic.h" // Convenience for child components

namespace fields_engine {
	class entity;
	/// TODO: remove
	namespace vis {
		class shader;
	}
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Component Class                                                           *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class component {
		FE_CLASS_BASE_BODY(component);

		component();
		component(component const& other);
		virtual ~component() {}

		
		virtual void init() {}
		virtual void tick(float dt) {}
		virtual void exit() {}

		virtual void read(json const& in) {}
		virtual void write(json& out) const {}

#ifdef EDITOR
		virtual bool display();
#endif // EDITOR

		void set_owner(entity* new_owner);
		entity* get_owner() const;

	private:
		entity* m_owner;
	};

} // namespace fields_engine
