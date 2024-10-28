#pragma once

#include "glfw/glfw3.h"

namespace fields_engine {

	class application {
	public:
		application();
		~application();

		bool startup();
		bool shutdown();
		//friend bool InitializeGLFW(application& app);


	private:
		GLFWwindow* windowHandle_;
	};

} // End namespace fields_engine

