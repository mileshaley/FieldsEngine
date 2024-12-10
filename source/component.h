/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             * 
 *~-------------------------------------------------------------------------~* 
 * File: component.h                                                         * 
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Component Defines                                                         * 
\*~-------------------------------------------------------------------------~*/

#define FE_GEN_COMPONENT(p_subclass, p_parclass)            \
		virtual unique<component> internal_clone() const override { \
			return make_unique<p_subclass>(*this);            \
		}                                                      \
		virtual string_view component_name() const override { \
			static constexpr string_view name( #p_subclass );    \
				return name;                                      \
		}                                                          \
		using this_type = p_subclass;                               \
		using super = p_parclass;

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
 * Generic Clone Function                                                    *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	// Clones a cloneable object at the most derived level of its class hierarchy
	// given its own type
	template<class T>
	unique<T> clone(T const& source) {
		return unique<T>(static_cast<T*>(source.internal_clone().release()));
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

		virtual unique<component> internal_clone() const = 0;

		virtual string_view component_name() const {
			static constexpr string_view name("component"); return name;
		};
		
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
