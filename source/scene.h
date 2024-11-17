/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: scene.h                                                             *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	class entity;
	namespace graphics {
		class shader;
	} // namespace graphics
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Scene Class                                                               *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class scene {
	public:
		scene();
		~scene();
		void tick(float dt);

		bool display_window();

	private: /// TODO: remove
		float m_obj1_rot_a = 0;
		float m_obj1_rot_b = 0;
		float m_spin = 75;
		float m_tilt = -30;
		float m_front = 0.5f;
		float m_back = 5000;
		vec3 m_cam_pos = { 0, 1, 2 };
		vec3 m_light_pos;
		mat4 m_world_view;

		unique_ptr<graphics::shader> m_shader;
		dyn_arr<unique_ptr<entity>> m_entities;
	};

} // namespace fields_engine
