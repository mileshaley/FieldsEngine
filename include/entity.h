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
		entity();
		/// TODO: Remove name constructors
		entity(string_view name);
		entity(string_view name, own<spatial_component>&& root_component);

		entity(own<spatial_component>&& root_component);
		entity(entity const& other);

		~entity();

		void init();
		void tick(float dt);
		void draw(vis::shader const& shader) const;
		void exit();

		void read(json const& in);
		void write(json& out) const;

#ifdef EDITOR
		bool display();
		bool component_display();
#endif // EDITOR

		string const& get_name() const;
		string& ref_name();

		transformer& ref_transform();
		transformer const& ref_transform() const;

		component& attach_basic_component(own<component>&& comp);
		// Set as or attach to root
		spatial_component& attach_spatial_component(own<spatial_component>&& comp);

		void acquire_basic_component(own<component>&& comp_to_own);
		void acquire_spatial_component(own<spatial_component>&& comp_to_own);

		spatial_component const* get_root() const;
		spatial_component* get_root();
		void set_root(spatial_component* new_root);

	private:
		string m_name;
		vector<own<component>> m_basic_components;
		vector<own<spatial_component>> m_spatial_components;
		spatial_component* m_root_component;
	};

} // namespace fields_engine
