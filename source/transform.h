/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: transform.h                                                         *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/


namespace fields_engine {

/*~-------------------------------------------------------------------------~*\
 * Transform Data Structure                                                  *
\*~-------------------------------------------------------------------------~*/

	struct transform_data {
		vec3 position;
		vec3 rotation;
		vec3 scale;
	};

/*~-------------------------------------------------------------------------~*\
 * Transform Class                                                           *
\*~-------------------------------------------------------------------------~*/

	// Lazy, only recalculates matrix when absolutely necessary
	class transform {
	public:

		transform(
			vec3 position = { 0, 0, 0 }, 
			vec3 rotation = { 0, 0, 0 },
			vec3 scale    = { 1, 1, 1 }
		);

		transform(transform_data const& data);

#ifdef EDITOR
		bool display();
#endif


		void set_dirty() const;

		mat4 const& world_matrix() const;

		void        set_position(vec3 const& new_pos);
		vec3 const& get_position() const;

		void        set_scale(vec3 const& new_scale);
		vec3 const& get_scale() const;

		void        set_rotation(vec3 const& new_rotation);
		vec3 const& get_rotation() const;

	private:
		transform_data m_data;
		transform* m_parent;
		mutable mat4 m_matrix;
		mutable bool m_dirty;
	};




} // namespace fields_engine
