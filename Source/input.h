#pragma once

struct GLFWwindow;

namespace fields_engine::input {


	namespace detail {

		void initialize_callbacks(GLFWwindow* windowHandle);
	} // End detail
} // End fields_engine::input