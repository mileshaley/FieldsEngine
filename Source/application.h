#pragma once

#include "editor.h"

namespace fields_engine {

	class application {
	public:
		application();
		~application();

		bool startup();
		bool shutdown();

		void run();

	private:
		window window_;
		unique_ptr<editor> editor_;
	};

} // End namespace fields_engine

