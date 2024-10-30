#pragma once


struct GLFWwindow;
namespace fields_engine {

	class application {
	public:
		application();
		~application();

		bool startup();
		bool shutdown();

		void run();

	private:
		GLFWwindow* windowHandle_;
	};

} // End namespace fields_engine

