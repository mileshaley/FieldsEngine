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
	class camera;
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

		void startup();
		void tick(float dt);
		void draw() const;
		void shutdown();

#ifdef EDITOR
		bool display_window();
#endif // EDITOR

		void register_camera(camera* cam);
		void unregister_camera(camera* cam);

	private: /// TODO: remove

		vec3 m_light_pos = { 9, 9, 11 };
		unique<graphics::shader> m_shader;

	private:

		dyn_arr<unique<entity>> m_entities;
		dyn_arr<camera*> m_cameras;
		camera* m_active_camera;
	};

} // namespace fields_engine
