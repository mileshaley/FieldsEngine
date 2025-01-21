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
#include "window.h" // window
#include "input.h"
#include "frame_buffer.h"
#include "asset_manager.h"

namespace fields_engine {
	class editor_manager;
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
		window& ref_window();
#if EDITOR
		editor_manager& ref_editor();
#endif // EDITOR

		ivec2 get_window_size() const;
		float get_delta_time() const;

	private:
		double m_prev_time;
		float m_delta_time;
		ivec2 m_win_size;
		local_context<window> m_window;
		local_context<input_manager> m_input_manager;
		local_context<asset_manager> m_asset_manager;
		box_context<scene> m_scene;
#if EDITOR
		box_context<editor_manager> m_editor{nullptr};
#endif // EDITOR
		bool m_running;
	};
} // namespace fields_engine
