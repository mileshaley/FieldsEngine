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
	class mesh;
	namespace graphics {
		class shader;
	} // namespace graphics
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Scene Class                                                               *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class mesh;
	namespace graphics {
		class shader;
	} // namespace graphics

	class scene {
	public:
		scene();
		~scene();
		void update(float dt);

		bool display_window();

	private: /// TODO: remove
		float m_obj1_rot_a = 0;
		float m_obj1_rot_b = 0;
		float m_spin = 75;
		float m_tilt = -30;
		float m_front = 0.5f;
		float m_back = 5000;
		vec2 m_ratio = { 0.4f, 0.4f };
		vec3 m_cam_pos = { 0, 1, 2 };
		vec3 m_light_pos;
		mat4 m_world_view;
		//vec2 m_t = { 0, 0 };
		//float m_zoom = 25;

		unique_ptr<graphics::shader> m_shader;

		unique_ptr<mesh> m_mesh;
		//bool m_enabled[6];
	};

} // namespace fields_engine
