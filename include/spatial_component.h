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

		FE_CLASS_BODY(spatial_component, component);

		virtual void draw(vis::shader const& shader) const {}

		void dirtify_transformers() const;
		
		void init_all();
		void tick_all(float dt);
		void draw_all(vis::shader const& shader) const;
		void exit_all();

		void read_all(json const& in);
		void write_all(json& out) const;

#ifdef EDITOR
		virtual bool display() override;
#endif // EDITOR

		spatial_component& attach_spatial_component(own<spatial_component>&& comp);
		void adopt_owned_component(spatial_component* new_child);

		void set_parent(spatial_component* new_parent);
		spatial_component* get_parent() const;

		transformer      & ref_transform()       { return m_transform; }
		transformer const& ref_transform() const { return m_transform; }

		void deep_copy_into_entity(entity& other_owner) const;

	private:
		void deep_copy_into_entity_aux(entity& other_owner, spatial_component& other_parent) const;

		transformer m_transform;
		spatial_component* m_parent;
		vector<spatial_component*> m_children;
	};

} // namespace fields_engine
