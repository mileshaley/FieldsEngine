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

	private:
		unique_ptr<graphics::shader> shader_; /// TODO: remove
		window window_;
		unique_ptr<editor> editor_;
	};

} // End namespace fields_engine

