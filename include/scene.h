/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: scene.h                                                             *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "asset.h"

namespace fields_engine {
	class entity;
	class camera_component;
	namespace vis {
		class shader;
	} // namespace vis

	namespace editor {
		class editor_window;
	} // namespace editor
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Scene Class                                                               *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class scene : public asset {
		FE_REFLECT_BODY(scene);
	public:
		scene();
		virtual ~scene() override;

		virtual void read(json const& in) override;
		virtual void write(json& out) const override;

		void startup();
		void tick(float dt);
		void draw() const;
		void shutdown();

#ifdef EDITOR
		bool display_window(editor::editor_window& window);
		bool inspect_window(editor::editor_window& window);
#endif // EDITOR

		void register_camera(camera_component* cam);
		void unregister_camera(camera_component* cam);

	private:
#ifdef EDITOR
		std::vector<entity*> m_selected_entities;
#endif // EDITOR

	private: /// TODO: remove variables in this section

		vec3 m_light_pos = { 9, 9, 11 };
		vec3 m_light_color = { 3.5f, 3.5f, 3.5f };
		vec3 m_ambient_color = { 0.2f, 0.2f, 0.2f };
		own<vis::shader> m_shader;

	private:
		vec4 m_background_color = { 0.55f, 0.75f, 1.0f, 1 };

		vector<own<entity>> m_entities;
		vector<camera_component*> m_cameras;
		camera_component* m_active_camera = nullptr;
	};

} // namespace fields_engine
