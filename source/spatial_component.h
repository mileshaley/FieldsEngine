/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             * 
 *~-------------------------------------------------------------------------~* 
 * File: spatial_component.h                                                 * 
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "component.h" // Inheritance
#include "transform.h"

/*~-------------------------------------------------------------------------~*\
 * Component Class                                                           *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class spatial_component : public component {
	public:
		spatial_component();
		spatial_component(spatial_component const& other);

		FE_GEN_COMPONENT(spatial_component);

		void dirtify_transforms() const;


#ifdef EDITOR
		virtual bool display() override;
#endif // EDITOR

		spatial_component& attach_component(unique<spatial_component>&& comp);
		void adopt_owned_component(spatial_component* new_child);

		void set_parent(spatial_component* new_parent);
		spatial_component* get_parent() const;

		transform      & ref_transform()       { return m_transform; }
		transform const& ref_transform() const { return m_transform; }

	private:
		transform m_transform;
		spatial_component* m_parent;
		dyn_arr<spatial_component*> m_children;
	};

} // namespace fields_engine
