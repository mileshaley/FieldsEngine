/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: scene.h                                                             *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "transform.h"

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


#ifdef EDITOR
		bool display_window();
#endif // EDITOR
	private: /// TODO: remove
		transform m_cam_transform{
			{ -7, -14, -23 },
			{ 90, 180,  40 },
			{  1,   1,   1 }
		};

		vec3 m_light_pos = { 9, 9, 11 };
		float m_back = 5000;
		float m_front = 0.5f;

		unique_ptr<graphics::shader> m_shader;
		dyn_arr<unique_ptr<entity>> m_entities;
	};

} // namespace fields_engine
