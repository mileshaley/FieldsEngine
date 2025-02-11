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
	namespace vis {
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
		entity(string_view name, box<spatial_component>&& root_component);

		entity(box<spatial_component>&& root_component);
		entity(entity const& other);

		~entity();

		void init();
		void tick(float dt);
		void draw(vis::shader const& shader) const;
		void exit();

		void read(json const& in);
		void write(json& out) const;

#ifdef EDITOR
		//static bool display_window();
		bool display();
		bool component_display();
#endif // EDITOR

		string const& get_name() const;
		string& ref_name();

		transform& ref_transform();
		transform const& ref_transform() const;

		component& attach_basic_component(box<component>&& comp);
		// Set as or attach to root
		spatial_component& attach_spatial_component(box<spatial_component>&& comp);

		void acquire_basic_component(box<component>&& comp_to_own);
		void acquire_spatial_component(box<spatial_component>&& comp_to_own);

		spatial_component const* get_root() const;
		spatial_component* get_root();
		void set_root(spatial_component* new_root);

	private:
		vector<box<component>> m_basic_components;
		vector<box<spatial_component>> m_spatial_components;
		spatial_component* m_root_component;
		string m_name;
	};

} // namespace fields_engine
