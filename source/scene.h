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
	class camera_component;
	namespace vis {
		class shader;
	} // namespace vis
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

		void register_camera(camera_component* cam);
		void unregister_camera(camera_component* cam);

	private: /// TODO: remove variables in this section

		vec3 m_light_pos = { 9, 9, 11 };
		vec3 m_light_color = { 3.5f, 3.5f, 3.5f };
		vec3 m_ambient_color = { 0.2f, 0.2f, 0.2f };
		box<vis::shader> m_shader;

	private:
		// { 0.1f, 0.1f, 0.1f, 1 } // Gray
		vec4 m_background_color = { 0.55f, 0.75f, 1.0f, 1 };

		dyn_arr<box<entity>> m_entities;
		dyn_arr<camera_component*> m_cameras;
		camera_component* m_active_camera;
	};

} // namespace fields_engine
