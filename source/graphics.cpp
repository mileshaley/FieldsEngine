/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: graphics.cpp                                                        *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "graphics.h"
#include "glad/glad.h"
#include "shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "mesh.h"
#include "error.h"

void fields_engine::vis::check_error(string_view file, int line) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		if (file.empty()) {
			// TODO: use proper error logger
			std::cerr << "Graphical error [" << error << "]" << std::endl;
		}
		else {
			std::cerr << "Graphical error [" << error << "] in file "
				<< file << " at line " << line << std::endl;
		}
		/// TODO: Please fix
		FE_FAILED_ASSERT(":(");
	}
}

void fields_engine::vis::gl_error_callback(int error_code, const char* message) {
	std::cerr << "Graphical error (callback) [" << error_code << "]: \"" << message << "\"" << std::endl;
}

void fields_engine::vis::resize_viewport(int width, int height) {
	glViewport(0, 0, width, height);
}

void fields_engine::vis::reset_frame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	VIS_VERIFY;
}

void fields_engine::vis::set_background_color(glm::vec4 const& color) {
	glClearColor(color.r, color.g, color.b, color.a);
	VIS_VERIFY;
}

void fields_engine::vis::impl::initialize() {
	glEnable(GL_DEPTH_TEST);
	/// TODO: Fix all of our meshes so we can do face culling
	//glEnable(GL_CULL_FACE);

	VIS_VERIFY;
	set_background_color({ 1.5f, 0.5f, 1.0f, 1.0f });
	reset_frame();
}
