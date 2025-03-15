/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: mesh_component.h                                                    *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "physical_component.h" // Inheritance
#include "material.h"

namespace fields_engine::vis {
	class texture;
	class mesh;
} // namespace fields_engine::vis

/*~-------------------------------------------------------------------------~*\
 * Entity Class                                                              *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	class mesh_component : public physical_component {
	public:
		FE_CLASS_BODY(mesh_component, spatial_component);

		mesh_component();
		mesh_component(mesh_component const& other);
		~mesh_component();

		virtual void draw(vis::shader const& shader) const override;

		virtual void read(json const& in) override;
		virtual void write(json& out) const override;

#ifdef EDITOR
		virtual bool display() override;
#endif // EDITOR

		void set_material(vis::material const* new_material) { m_material = new_material; }
		vis::material const* get_material() const { return m_material; }
		void set_mesh(vis::mesh const& new_mesh) { m_mesh = &new_mesh; }
		vis::mesh const& get_mesh() const { return *m_mesh; }

	private:
		const vis::mesh* m_mesh;
		const vis::material* m_material;
	};

} // namespace fields_engine
