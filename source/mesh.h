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

namespace fields_engine::graphics {
	class texture;
	class mesh_resource;
} // namespace fields_engine::graphics

/*~-------------------------------------------------------------------------~*\
 * Entity Class                                                              *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	/// TODO: Rename to mesh_component

	class mesh : public spatial_component {
	public:

		mesh();
		mesh(mesh const& other);
		~mesh();

		FE_GEN_COMPONENT(mesh, spatial_component);

		virtual void draw(graphics::shader const& shader) const override;

		graphics::material      & ref_material()       { return m_material; }
		graphics::material const& get_material() const { return m_material; }
		graphics::mesh_resource      & ref_resource()		{ return *m_resource; }
		graphics::mesh_resource const& get_resource() const { return *m_resource; }

		void set_texture(unique<graphics::texture>&& new_texture);
		void set_normal_texture(unique<graphics::texture>&& new_normal_texture);

	private:
		unique<graphics::mesh_resource> m_resource;
		unique<graphics::texture> m_texture;
		unique<graphics::texture> m_normal_texture;
		graphics::material m_material;
	};

} // namespace fields_engine
