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
	namespace editor {
		class editor_manager;
	} // namespace editor
	class scene;
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

		// Use this window as the primary one
		void use() const;
		input_manager& ref_input_manager();
		window_handle& get_window_handle();
#if EDITOR
		editor::editor_manager& ref_editor();
#endif // EDITOR

		ivec2 get_window_size() const;
		float get_delta_time() const;

	private:
		double m_prev_time;
		float m_delta_time;
		ivec2 m_win_size;
		local_context<window_handle> m_window;
		local_context<input_manager> m_input_manager;
		local_context<asset_manager> m_asset_manager;
		own_context<scene> m_scene;
#if EDITOR
		own_context<editor::editor_manager> m_editor{nullptr};
#endif // EDITOR
		bool m_running;
	};
} // namespace fields_engine
