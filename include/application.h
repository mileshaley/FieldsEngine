/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: application.h                                                       *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/
	
#include "context_ownership.h"
#include "window_handle.h" // window_handle
#include "input.h"
#include "frame_buffer.h"
#include "asset_manager.h"

namespace fields_engine {
#ifdef EDITOR
	namespace editor {
		class editor_manager;
	} // namespace editor
#endif // EDITOR
	class scene;
	class project;
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Application Class                                                         *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	class application {
	public:
		application();
		~application();
		
		bool startup();
		void run();
		bool shutdown();

		void use_context();
		input_manager& ref_input_manager();
		window_handle& get_window_handle();
#ifdef EDITOR
		editor::editor_manager& ref_editor();
#endif // EDITOR

		ivec2 get_window_size() const;
		float get_delta_time() const;

	private:
		double m_prev_time;
		float m_delta_time;
		bool m_running;
		ivec2 m_win_size;
		local_context<window_handle> m_window;
		local_context<input_manager> m_input_manager;
		local_context<asset_manager> m_asset_manager;
		own_context<project> m_project;
		own_context<scene> m_scene;
#ifdef EDITOR
		own_context<editor::editor_manager> m_editor;
#endif // EDITOR
	};
} // namespace fields_engine
