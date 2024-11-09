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
		unique_ptr<graphics::shader> shader_; /// TODO: remove
		fe::window window_;
		unique_context<fe::editor> editor_;
	};

	GENERATE_GLOBAL_CONTEXT_FOR_TYPE(application);

} // End namespace fields_engine

