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

	// Lazy, only recalculates matrix when necessary
	class transform {
	public:
		static constexpr mat4 no_parent{1};

		transform(
			vec3 position = { 0, 0, 0 }, 
			vec3 rotation = { 0, 0, 0 },
			vec3 scale    = { 1, 1, 1 }
		);

		transform(transform_data const& data);

#ifdef EDITOR
		bool display();
#endif

		// new_parent will be stored within this transform, 
		// make sure it lives for at least as long as this transform does
		void set_parent(const mat4* new_parent = &no_parent);

		// new_parent's matrix will be stored within this transform, 
		// make sure it lives for at least as long as this transform does
		void set_parent(transform const& new_parent);

		const mat4* as_parent() const;

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
		const mat4* m_parent;
		mutable mat4 m_matrix;
		mutable bool m_dirty;
	};




} // namespace fields_engine
