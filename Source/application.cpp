/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: application.cpp                                                     *
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * File Includes                                                             *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "application.h"

#include <iostream>
#include "glad/glad.h"
#include "input.h"
#include "graphics.h"

#include "scene.h"
#if EDITOR 
#include "editor.h"
#endif

#if FE_USING_GLFW
#include "glfw/glfw3.h"
#elif FE_USING_SDL2
#include "SDL/SDL.h"
#endif

/*~-------------------------------------------------------------------------~*\
 * Application Definitions                                                   *
\*~-------------------------------------------------------------------------~*/

fields_engine::application::application() 
	: m_window{nullptr}
	, m_scene{nullptr}
	, m_win_size{1000, 800}
{}

// Avoid dependencies on composed classes' destructors
fields_engine::application::~application() = default;

bool fields_engine::application::startup() {
#if FE_USING_GLFW
	glfwSetErrorCallback(graphics::gl_error_callback);
	glfwInit();

	// Use OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 0);

	m_window->handle = glfwCreateWindow(m_win_size.x, m_win_size.y, "FieldsEngine", nullptr, nullptr);

	if (!m_window->handle) { 
		return false; 
	}

	glfwMakeContextCurrent(m_window->handle);
	// Set vsync on
	glfwSwapInterval(1);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		return false;
	}

	glfwSetFramebufferSizeCallback(m_window->handle,
		[](GLFWwindow* win, int w, int h) { graphics::resize_viewport(w, h); }
	);

	//glfwSetWindowFocusCallback(m_window, );
#elif FE_USING_SDL3


#endif // FE_USING_SDL3

	input::detail::initialize_callbacks(m_window);
	graphics::detail::initialize();

	m_scene = make_unique<fe::scene>();
#if EDITOR
	m_editor = make_unique<fe::editor>(m_window.get());
#endif // EDITOR

	m_scene->startup();

	return true;
}



void fields_engine::application::run() {
	
	while (m_window->is_running()) {
		/// TODO: use real delta time
		const float dt = 1.0f / 60.0f;
		glfwPollEvents();

		/// update logic goes here
		glfwGetFramebufferSize(m_window->handle, &m_win_size.x, &m_win_size.y);
		glViewport(0, 0, m_win_size.x, m_win_size.y);
		FE_GL_VERIFY;
		
		m_scene->tick(dt);
		m_scene->draw();
		

		/// draw logic goes here

		m_editor->tick(dt);

		glfwSwapBuffers(m_window->handle);
	}
}

bool fields_engine::application::shutdown() {
	m_scene->shutdown();
	m_editor.reset();
	m_scene.reset();
	glfwTerminate();
	return true;
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

fe::ivec2 fields_engine::application::get_window_size() const {
	return m_win_size;
}
