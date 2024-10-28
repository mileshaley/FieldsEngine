#include "precompiled.h"
#include "application.h"

#include <iostream>
#include "glad/glad.h"
#include "imGui/imgui.h"
#include "imGui/imgui_impl_glfw.h"
#include "imGui/imgui_impl_opengl3.h"

#include "input.h"

#ifdef _glfw3_h_

static void gl_error_callback(int errorCode, const char* message) {
	std::cerr << "gl error [" << errorCode << "]: \"" << message << "\"" << std::endl;
}

fields_engine::application::application() 
	: windowHandle_(nullptr)
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

	windowHandle_ = glfwCreateWindow(1920, 1080, "FieldsEngine", nullptr, nullptr);

	if (!windowHandle_) { 
		return false; 
	}

	glfwMakeContextCurrent(windowHandle_);
	// Set vsync on
	glfwSwapInterval(1);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		return false;
	}

	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(windowHandle_, true);
	ImGui_ImplOpenGL3_Init();

	input::detail::initialize_callbacks(windowHandle_);
	//glfwSetWindowFocusCallback(windowHandle_, );


	return true;
}

bool fields_engine::application::shutdown() {
	return true;
}

void fields_engine::application::run() {

}

#endif

