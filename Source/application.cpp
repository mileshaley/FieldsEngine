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
#include "graphics.h"

#include "scene.h"
#if EDITOR 
#include "editor.h"
#endif

#if FE_USING_GLFW
#include "glfw/glfw3.h"
#elif FE_USING_SDL3
#include "SDL3/SDL.h"
#endif // FE_USING_SDL3

/*~-------------------------------------------------------------------------~*\
 * Application Definitions                                                   *
\*~-------------------------------------------------------------------------~*/

fields_engine::application::application() 
	: m_window{nullptr}
	, m_scene{nullptr}
	, m_win_size{1000, 800}
	, m_running(false)
{}

// Fulfil dependencies on member destructors
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

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		return false;
	}
	
	m_window->handle = SDL_CreateWindow("FieldsEngine", m_win_size.x, m_win_size.y, 0);
	if (!m_window->handle) {
		return false;
	}

#endif // FE_USING_SDL3

	if (!m_input_manager->startup()) {
		return false;
	}

	input::detail::initialize_callbacks(m_window);
	graphics::detail::initialize();

	m_scene = make_unique<scene>();
#if EDITOR
	m_editor = make_unique<editor>(m_window.get());
#endif // EDITOR

	m_scene->startup();

	return true;
}



void fields_engine::application::run() {
	m_running = true;

	m_prev_time = glfwGetTime();
	while (m_running) {
		const double new_time = glfwGetTime();
		const float dt = float(new_time - m_prev_time);
		m_prev_time = new_time;

		m_input_manager->tick(dt);

		glfwPollEvents();

		/// update logic goes here
		glfwGetFramebufferSize(m_window->handle, &m_win_size.x, &m_win_size.y);
		glViewport(0, 0, m_win_size.x, m_win_size.y);
		FE_GL_VERIFY;
		
		m_scene->tick(dt);
		m_scene->draw();
		

		/// draw logic goes here

		m_editor->tick(dt);
#if EDITOR
		m_editor->ref_dual_frame_buffer().swap();
#elif FE_USING_GLFW
		glfwSwapBuffers(m_window->handle);
#elif FE_USING_SDL3
		/// RenderClear
#endif // FE_USING_SDL3

#if FE_USING_GLFW
m_running = !glfwWindowShouldClose(m_window->handle);
#elif FE_USING_SDL3
m_running; /// using input
#endif // FE_USING_SDL3
	}
}

bool fields_engine::application::shutdown() {
	m_scene->shutdown();
	m_editor.reset();
	m_scene.reset();
	m_input_manager->shutdown();
#if FE_USING_GLFW
	glfwDestroyWindow(m_window->handle);
	glfwTerminate();
#elif FE_USING_SDL3
	SDL_DestroyWindow(m_window->handle);
	SDL_Quit();
#endif // FE_USING_SDL3
	return true;
}

void fields_engine::application::use() const {
#if FE_USING_GLFW
	glfwMakeContextCurrent(m_window->handle);
#elif FE_USING_SDL3
	///
#endif // FE_USING_SDL3
}

fe::input_manager& fields_engine::application::ref_input_manager() {
	return m_input_manager;
}

fe::window& fields_engine::application::ref_window() {
	return m_window.get();
}

#if EDITOR
fe::editor& fields_engine::application::ref_editor() {
	return *m_editor;
}
#endif // EDITOR

fe::ivec2 fields_engine::application::get_window_size() const {
	return m_win_size;
}
