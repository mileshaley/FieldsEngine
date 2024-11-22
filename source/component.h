/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             * 
 *~-------------------------------------------------------------------------~* 
 * File: component.h                                                         * 
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Component Defines                                                         * 
\*~-------------------------------------------------------------------------~*/

#define FE_GEN_COMPONENT(p_subclass)                      \
		virtual unique<component> clone() const override { \
			return make_unique<p_subclass>(*this);          \
		}                                                    \
		virtual string_view component_name() const override { \
			static constexpr string_view name( #p_subclass );  \
				return name;                                    \
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
		virtual unique<component> clone() const {
			return make_unique<component>();
		};

		virtual string_view component_name() const {
			static constexpr string_view name("component");
			return name;
		}
		
		virtual void init() {}
		virtual void tick(float dt) {}
		virtual void render(graphics::shader const& shader) const {}
		virtual void exit() {}

		void dirtify_transforms() const;


#ifdef EDITOR
		virtual bool display();
#endif // EDITOR

		void attach_component(unique<component>&& comp);
		void adopt_owned_component(component* new_child);


		void set_parent(component* new_parent);
		component* get_parent() const;

		void set_owner(entity* new_owner);
		entity* get_owner() const;

		transform      & ref_transform()       { return m_transform; }
		transform const& ref_transform() const { return m_transform; }


	private:
		transform m_transform;
		entity* m_owner;
		component* m_parent;
		dyn_arr<component*> m_children;
	};

} // namespace fields_engine
