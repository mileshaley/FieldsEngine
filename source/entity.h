/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: entity.h                                                            *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "transform.h"

namespace fields_engine {
	class component;
	class spatial_component;
	/// TODO: remove
	namespace graphics {
		class shader; 
	}
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Entity Class                                                              *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class entity {
	public:
		/// TODO: Remove name constructors
		entity(string_view name);
		entity(string_view name, unique<spatial_component>&& root_component);

		entity(unique<spatial_component>&& root_component);
		entity(entity const& other);

		~entity();

		void init();
		void tick(float dt);
		void draw(graphics::shader const& shader) const;
		void exit();

#ifdef EDITOR
		bool display();
#endif // EDITOR

		transform& ref_transform();
		transform const& ref_transform() const;

		spatial_component* get_root();
		spatial_component const* get_root() const;

		void acquire_component(unique<component>&& comp_to_own);
		// Attach to root
		component& attach_component(unique<component>&& comp);

	private:
		string m_name;
		dyn_arr<unique<component>> m_components;
		spatial_component* m_root_component;
	};

} // namespace fields_engine
