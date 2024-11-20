/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             * 
 *~-------------------------------------------------------------------------~* 
 * File: component.h                                                         * 
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Component Defines                                                         * 
\*~-------------------------------------------------------------------------~*/

#define FE_GEN_COMPONENT(p_subclass) \
		virtual unique<component> clone() const override {\
			return make_unique<p_subclass>(*this); \
		}

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

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
	public:
		component();
		component(component const& other);
		virtual ~component() {}
		virtual unique<component> clone() const = 0;
		
		virtual void init() {}
		virtual void tick(float dt) {}
		virtual void render(graphics::shader const& shader) const {}
		virtual void exit() {}

#ifdef EDITOR
		virtual bool display();
#endif // EDITOR

		transform      & ref_transform()       { return m_transform; }
		transform const& ref_transform() const { return m_transform; }



	private:
		entity* m_owner;
		component* m_parent;
		transform m_transform;
	};

} // namespace fields_engine
