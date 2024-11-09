#include "precompiled.h"
#include "application.h"

#include <iostream>
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "input.h"
#include "graphics.h"

//fe::application* fe::g_application = nullptr;


fields_engine::application::application() 
	: m_window{nullptr}
	, m_editor{nullptr}
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

	/// TODO: relocate
	m_shader = make_unique<graphics::shader>();
	m_shader->add("lighting.vert", GL_VERTEX_SHADER);
	m_shader->add("lighting.frag", GL_FRAGMENT_SHADER);
	glBindAttribLocation(m_shader->id(), 0, "vertex");
	glBindAttribLocation(m_shader->id(), 1, "vertexNormal");
	glBindAttribLocation(m_shader->id(), 2, "vertexTexture");
	glBindAttribLocation(m_shader->id(), 3, "vertexTangent");
	m_shader->finalize();

	///

	input::detail::initialize_callbacks(m_window);

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

		/// render logic goes here

		m_editor->update(dt);

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


