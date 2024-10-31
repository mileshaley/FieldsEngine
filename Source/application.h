#pragma once

#include "editor.h"
#include "shader.h"


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

		nullable_ptr<editor> editor();

	private:
		unique_ptr<graphics::shader> shader_; /// TODO: remove
		window window_;
		unique_ptr<fe::editor> editor_;
	};

	extern application* g_application;

} // End namespace fields_engine

