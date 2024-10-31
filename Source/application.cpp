#include "precompiled.h"
#include "application.h"

#include <iostream>
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "input.h"
#include "graphics.h"


fields_engine::application::application() 
	: window_{nullptr}
	, editor_{nullptr}
{}

fields_engine::application::~application() {
	
}

bool fields_engine::application::startup()
{
	glfwSetErrorCallback(graphics::gl_error_callback);
	glfwInit();
	// Use OpenGL version 3.3

	glfwWindowHint(GLFW_RESIZABLE, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 0);
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

	/// TODO: remove
	shader_ = make_unique<graphics::shader>();
	shader_->add("lighting.vert", GL_VERTEX_SHADER);
	shader_->add("lighting.frag", GL_FRAGMENT_SHADER);
	glBindAttribLocation(shader_->id(), 0, "vertex");
	glBindAttribLocation(shader_->id(), 1, "vertexNormal");
	glBindAttribLocation(shader_->id(), 2, "vertexTexture");
	glBindAttribLocation(shader_->id(), 3, "vertexTangent");
	shader_->finalize();

	///

	input::detail::initialize_callbacks(window_);

	editor_ = make_unique<editor>(window_);

	//glfwSetWindowFocusCallback(window_, );

	graphics::detail::initialize();

	glfwSetFramebufferSizeCallback(window_.handle, 
		[](GLFWwindow* wind, int w, int h) { graphics::resize_viewport(w, h); }
	);

	return true;
}

bool fields_engine::application::shutdown() {
	editor_.reset();
	glfwTerminate();
	return true;
}

void fields_engine::application::run() {
	
	while (window_.is_running()) {
		glfwPollEvents();

		editor_->update();

		glfwSwapBuffers(window_.handle);
	}

}


