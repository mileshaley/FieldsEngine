#pragma once

#include "editor.h"
#include "window.h"
#include "context.h"

namespace fields_engine {

	class scene;

	class application {
	public:
		application();
		~application();

		bool startup();
		bool shutdown();

		void run();

		// Reinstate this window as the primary one
		void reinstate() const;
		window& window();
		editor* editor();

		ivec2 window_size() const;

	private:

		ivec2 m_win_size;
		local_context<fe::window> m_window;
		unique_context<fe::editor> m_editor;
		unique_context<scene> m_scene;
	};

} // End namespace fields_engine

