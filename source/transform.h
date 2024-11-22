/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: transform.h                                                         *
\*~-------------------------------------------------------------------------~*/

#pragma once

namespace fields_engine {

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

	class component;

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
			vec3 const& position = { 0, 0, 0 }, 
			vec3 const& rotation = { 0, 0, 0 },
			vec3 const& scale    = { 1, 1, 1 }
		);

		transform(transform_data const& data);
		transform(transform const& other);

#ifdef EDITOR
		bool display();
#endif

		void recalculate_matrix() const;

		// new_parent will be stored within this transform, 
		// make sure it lives for at least as long as this transform does
		void set_parent(const mat4* new_parent = &no_parent);

		// new_parent's matrix will be stored within this transform, 
		// make sure it lives for at least as long as this transform does
		void set_parent(transform const& new_parent);
		mat4 const* get_parent() const;

		void set_owner(const component* new_owner);
		const component* get_owner() const;

		void set_dirty() const;
		void set_only_this_dirty() const;

		mat4 const& world_matrix() const;

		void set_local_position(vec3 const& new_pos);
		void set_local_scale(vec3 const& new_scale);
		void set_local_rotation(vec3 const& new_rotation);

		transform_data const& get_local_transform() const;
		vec3 const& get_local_position() const;
		vec3 const& get_local_scale() const;
		vec3 const& get_local_rotation() const;

		transform_data get_world_transform() const;
		vec3 get_world_position() const;
		vec3 get_world_scale() const;
		vec3 get_world_rotation() const;

	private:
		transform_data m_data;
		const component* m_owner;
		const mat4* m_parent;
		mutable mat4 m_matrix;
		mutable bool m_dirty = true;

		bool m_invert = false;
	};

/*~-------------------------------------------------------------------------~*\
 * Transform Related Functions                                               *
\*~-------------------------------------------------------------------------~*/

	vec3 matrix_decompose_position(mat4 const& mat);
	vec3 matrix_decompose_rotation(mat4 const& mat);
	vec3 matrix_decompose_rotation(mat4 const& mat, vec3 const& scale);
	vec3 matrix_decompose_scale(mat4 const& mat);

	void matrix_decompose(mat4 const& mat, transform_data& out_data);

} // namespace fields_engine
