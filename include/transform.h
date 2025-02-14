/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: transform.h                                                         *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "glm/gtc/quaternion.hpp"

namespace fields_engine {
	class spatial_component;
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Transform Data                                                            *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	struct transform {
		vec3 position;
		vec3 scale;
		quat rotation;
	};

	void from_json(json const& in, transform& out);
	void to_json(json& out, transform const& in);

/*~-------------------------------------------------------------------------~*\
 * Transformer Class                                                         *
\*~-------------------------------------------------------------------------~*/

	// Lazy, only recalculates matrix when necessary
	class transformer {
		static constexpr mat4 identity{1};
	public:
		transformer(
			vec3 const& position = { 0, 0, 0 }, 
			vec3 const& scale    = { 1, 1, 1 },
			vec3 const& rotation = { 0, 0, 0,}
		);

		transformer(transform const& data);
		transformer(transformer const& other);

		void read(json const& in);
		void write(json& out) const;

#ifdef EDITOR
		bool display();
#endif

		void recalculate_matrix() const;

		void set_owner(const spatial_component* new_owner);
		const spatial_component* get_owner() const;

		void set_dirty() const;
		void set_only_this_dirty() const;

		mat4 const& world_matrix() const;

		void set_local_position(vec3 const& new_pos);
		void set_local_scale(vec3 const& new_scale);
		void set_local_rotation(vec3 const& new_rotation);
		void set_local_rotation(quat const& new_rotation);

		transform get_world_transform() const;
		vec3 get_world_position() const;
		vec3 get_world_scale() const;
		quat get_world_rotation() const;

		transform const& get_local_transform() const;
		vec3 const& get_local_position() const;
		vec3 const& get_local_scale() const;
		quat const& get_local_rotation() const;

		vec3 get_local_forward_vector() const;
		vec3 get_local_right_vector() const;
		vec3 get_local_up_vector() const;
	private:
		transform m_data;
		mutable bool m_dirty = true;
		mutable mat4 m_matrix;
		const spatial_component* m_owner;
	};

/*~-------------------------------------------------------------------------~*\
 * Transform Related Functions                                               *
\*~-------------------------------------------------------------------------~*/

	vec3 matrix_decompose_position(mat4 const& mat);
	vec3 matrix_decompose_rotation(mat4 const& mat);
	vec3 matrix_decompose_rotation(mat4 const& mat, vec3 const& scale);
	vec3 matrix_decompose_scale(mat4 const& mat);

	void matrix_decompose(mat4 const& mat, transform& out_data);

} // namespace fields_engine
