#include "precompiled.h"
#include "application.h"

#include <iostream>
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "input.h"




#if defined(_glfw3_h_)


#define GL_CHECK check_graphics_error(__FILE__, __LINE__);

static void check_graphics_error(string_view file = "", int line = 0) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		if (file.empty()) {
			// TODO: use proper error logger
			std::cerr << "Graphical error [" << error << "]" << std::endl;
		} else {
			std::cerr << "Graphical error [" << error << "] in file " 
					  << file << "at line " << line << std::endl;
		}
		assert(false);
	}
}

static void gl_error_callback(int errorCode, const char* message) {
	std::cerr << "gl error [" << errorCode << "]: \"" << message << "\"" << std::endl;
}

fields_engine::application::application() 
	: window_{nullptr}
	, editor_{nullptr}
{}

fields_engine::application::~application() {
	
}

bool fields_engine::application::startup()
{
	glfwSetErrorCallback(gl_error_callback);
	glfwInit();
	// Use OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_.handle = glfwCreateWindow(1920, 1080, "FieldsEngine", nullptr, nullptr);

	if (!window_.handle) { 
		return false; 
	}

	glfwMakeContextCurrent(window_.handle);
	// Set vsync on
	glfwSwapInterval(1);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		return false;
	}

	editor_ = make_unique<editor>(window_);

	input::detail::initialize_callbacks(window_);
	//glfwSetWindowFocusCallback(window_, );

	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	GL_CHECK;
	glEnable(GL_DEPTH_TEST);
	GL_CHECK;

	return true;
}

bool fields_engine::application::shutdown() {
	editor_.reset();
	return true;
}

void fields_engine::application::run() {
	
	while (window_.is_running()) {
		glfwPollEvents();

		//editor_->update();

		glfwSwapBuffers(window_.handle);
	}

}

#endif

