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

		FE_GEN_COMPONENT(spatial_component, component);

		void dirtify_transforms() const;
		
		void init_all();
		void tick_all(float dt);
		void draw_all(graphics::shader const& shader) const;
		void exit_all();

#ifdef EDITOR
		virtual bool display() override;
#endif // EDITOR

		spatial_component& attach_spatial_component(unique<spatial_component>&& comp);
		void adopt_owned_component(spatial_component* new_child);

		void set_parent(spatial_component* new_parent);
		spatial_component* get_parent() const;

		transform      & ref_transform()       { return m_transform; }
		transform const& ref_transform() const { return m_transform; }

		//void deep_resolve_clone_relations(
		//	spatial_component* original_root, 
		//	dyn_arr<unique<component>> const& original,
		//	dyn_arr<unique<component>> const& comps
		//);

		void deep_copy_into_entity(entity& other_owner) const;

	private:
		void deep_copy_into_entity_aux(entity& other_owner, spatial_component& other_parent) const;

		transform m_transform;
		spatial_component* m_parent;
		dyn_arr<spatial_component*> m_children;
	};

} // namespace fields_engine
