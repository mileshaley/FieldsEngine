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

		virtual void read(json const& in) override;
		virtual void write(json& out) const override;

		void set_material(vis::material const* new_material) { m_material = new_material; }
		vis::material const* get_material() const { return m_material; }
		void set_mesh(vis::mesh const& new_mesh) { m_mesh = &new_mesh; }
		vis::mesh const& get_mesh() const { return *m_mesh; }


	private:
		const vis::material* m_material;
		vis::mesh const* m_mesh;

	};

} // namespace fields_engine
