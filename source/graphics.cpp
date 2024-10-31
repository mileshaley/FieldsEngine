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

void fields_engine::graphics::gl_error_callback(int errorCode, const char* message) {
	std::cerr << "gl error [" << errorCode << "]: \"" << message << "\"" << std::endl;
}

void fields_engine::graphics::detail::initialize() {
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	GL_CHECK;
	glEnable(GL_DEPTH_TEST);
	GL_CHECK;
}
