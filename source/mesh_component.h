/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: mesh_component.h                                                    *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "spatial_component.h" // Inheritance
#include "material.h"

namespace fields_engine::vis {
	class texture;
	class mesh;
} // namespace fields_engine::vis

/*~-------------------------------------------------------------------------~*\
 * Entity Class                                                              *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	class mesh_component : public spatial_component {
	public:
		mesh_component();
		mesh_component(mesh_component const& other);
		~mesh_component();

		FE_GEN_COMPONENT(mesh_component, spatial_component);

		virtual void draw(vis::shader const& shader) const override;

		vis::material      & ref_material()       { return m_material; }
		vis::material const& get_material() const { return m_material; }
		vis::mesh      & ref_mesh()		{ return *m_mesh; }
		vis::mesh const& get_mesh() const { return *m_mesh; }

		void set_texture(box<vis::texture>&& new_texture);
		void set_normal_texture(box<vis::texture>&& new_normal_texture);

	private:
		/// TODO: Make material and mesh both managed resource ptrs
		/// TODO: Allow for multiple material slots per mesh
		vis::material m_material;
		box<vis::mesh> m_mesh;
		box<vis::texture> m_texture;
		box<vis::texture> m_normal_texture;
	};

} // namespace fields_engine
