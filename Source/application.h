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
		bool m_running;
		ivec2 m_win_size;
		local_context<window> m_window;
		unique_context<scene> m_scene;
#if EDITOR
		unique_context<editor> m_editor{nullptr};
#endif // EDITOR
	};
} // namespace fields_engine
