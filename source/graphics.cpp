/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: graphics.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "graphics.h"
#include "glad/glad.h"
#include <iostream>

void fields_engine::graphics::check_graphics_error(string_view file, int line) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		if (file.empty()) {
			// TODO: use proper error logger
			std::cerr << "Graphical error [" << error << "]" << std::endl;
		}
		else {
			std::cerr << "Graphical error [" << error << "] in file "
				<< file << "at line " << line << std::endl;
		}
		assert(false);
	}
}

void fields_engine::graphics::gl_error_callback(int error_code, const char* message) {
	std::cerr << "Graphical error [" << error_code << "]: \"" << message << "\"" << std::endl;
}

void fields_engine::graphics::resize_viewport(int width, int height) {
	glViewport(0, 0, width, height);
}

void fields_engine::graphics::clear_background() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GL_CHECK;
}

void fields_engine::graphics::clear_background(glm::vec4 const& color) {
	glClearColor(color.r, color.g, color.b, color.a);
	GL_CHECK;
	clear_background();
}

void fields_engine::graphics::detail::initialize() {
	glEnable(GL_DEPTH_TEST);
	GL_CHECK;
	clear_background({ 1.5f, 0.5f, 1.0f, 1.0f });
}
