/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: mesh_component.h                                                              *
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

		void set_texture(unique<vis::texture>&& new_texture);
		void set_normal_texture(unique<vis::texture>&& new_normal_texture);

	private:
		unique<vis::mesh> m_mesh;
		unique<vis::texture> m_texture;
		unique<vis::texture> m_normal_texture;
		vis::material m_material;
	};

} // namespace fields_engine
