/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: mesh.h                                                              *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "spatial_component.h" // Inheritance
#include "material.h"

namespace fields_engine::vis {
	class texture;
	class mesh_resource;
} // namespace fields_engine::vis

/*~-------------------------------------------------------------------------~*\
 * Entity Class                                                              *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	/// TODO: Consider rename to mesh_component
	class mesh : public spatial_component {
	public:

		mesh();
		mesh(mesh const& other);
		~mesh();

		FE_GEN_COMPONENT(mesh, spatial_component);

		virtual void draw(vis::shader const& shader) const override;

		vis::material      & ref_material()       { return m_material; }
		vis::material const& get_material() const { return m_material; }
		vis::mesh_resource      & ref_resource()		{ return *m_resource; }
		vis::mesh_resource const& get_resource() const { return *m_resource; }

		void set_texture(unique<vis::texture>&& new_texture);
		void set_normal_texture(unique<vis::texture>&& new_normal_texture);

	private:
		unique<vis::mesh_resource> m_resource;
		unique<vis::texture> m_texture;
		unique<vis::texture> m_normal_texture;
		vis::material m_material;
	};

} // namespace fields_engine
