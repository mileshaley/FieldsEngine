#include "precompiled.h"
#include "application.h"

#include <iostream>
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "input.h"
#include "graphics.h"

#include "scene.h"

//fe::application* fe::g_application = nullptr;


fields_engine::application::application() 
	: m_window{nullptr}
	, m_editor{nullptr}
	, m_scene{nullptr}
{}

fields_engine::application::~application() {
	
}

bool fields_engine::application::startup()
{
	glfwSetErrorCallback(graphics::gl_error_callback);
	glfwInit();

	// Use OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 0);
	//const ivec2 win_size{ 1920, 1080 };
	const ivec2 win_size{1000, 800};

	m_window->handle = glfwCreateWindow(win_size.x, win_size.y, "FieldsEngine", nullptr, nullptr);

	if (!m_window->handle) { 
		return false; 
	}

	glfwMakeContextCurrent(m_window->handle);
	// Set vsync on
	glfwSwapInterval(1);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		return false;
	}

	input::detail::initialize_callbacks(m_window);

	m_scene = make_unique<fe::scene>();
	m_editor = make_unique<fe::editor>(m_window.get());
	//glfwSetWindowFocusCallback(m_window, );

	graphics::detail::initialize();

	glfwSetFramebufferSizeCallback(m_window->handle, 
		[](GLFWwindow* win, int w, int h) { graphics::resize_viewport(w, h); }
	);

	return true;
}

bool fields_engine::application::shutdown() {
	m_editor.reset();
	glfwTerminate();
	return true;
}

void fields_engine::application::run() {
	
	while (m_window->is_running()) {
		/// TODO: use real delta time
		const float dt = 1.0f / 60.0f;
		//graphics::clear_background({0.5, 0.5, 1.0, 1.0});
		glfwPollEvents();

		/// update logic goes here
		glfwGetFramebufferSize(m_window->handle, &m_win_size.x, &m_win_size.y);
		glViewport(0, 0, m_win_size.x, m_win_size.y);
		FE_GL_VERIFY;
		
		m_scene->tick(dt);
		

		/// render logic goes here

		m_editor->tick(dt);

		glfwSwapBuffers(m_window->handle);
	}
}

void fields_engine::application::reinstate() const {
	glfwMakeContextCurrent(m_window->handle);
}

fe::window& fields_engine::application::window() {
	return m_window.get();
}

fe::editor* fields_engine::application::editor() {
	return m_editor.get();
}

fe::ivec2 fields_engine::application::window_size() const {
	return m_win_size;
}
