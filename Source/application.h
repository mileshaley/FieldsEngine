#pragma once

#include "editor.h"
#include "shader.h"
#include "window.h"
#include "context.h"

namespace fields_engine {

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

	private:
		unique_ptr<graphics::shader> m_shader; /// TODO: remove
		fe::window m_window;
		unique_context<fe::editor> m_editor;
	};

} // End namespace fields_engine

