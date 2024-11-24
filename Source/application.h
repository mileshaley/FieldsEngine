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

namespace fields_engine {
	class editor;
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
		window& ref_window();
#if EDITOR
		editor& ref_editor();
#endif // EDITOR

		ivec2 get_window_size() const;

	private:
		ivec2 m_win_size;
		local_context<window> m_window;
		local_context<input_manager> m_input_manager;
		unique_context<scene> m_scene;
#if EDITOR
		unique_context<editor> m_editor{nullptr};
#endif // EDITOR
		bool m_running;
	};
} // namespace fields_engine
