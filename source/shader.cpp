/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: shader.cpp														 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "shader.h"
#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "graphics.h"

fields_engine::graphics::shader::shader()
	: m_program_id(glCreateProgram())
{}

void fields_engine::graphics::shader::add(const char* filename, int type) const {
    // Read a file's contents using fstream, stringstream, and string
    std::fstream file(filename);
    if (!file) { return; }
    std::stringstream ss;
    ss << file.rdbuf();
    string source = ss.str();
    // Array of one const char* to get const char**
    const char* source_ptr[1] = { source.c_str() };

    int shader_id = glCreateShader(type);
    glAttachShader(m_program_id, shader_id);
    GL_CHECK;
    glShaderSource(shader_id, 1, source_ptr, nullptr);
    GL_CHECK;
    glCompileShader(shader_id);
    GL_CHECK;

    int status = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
    GL_CHECK;
    // Something went wrong, log an error
    if (status != 1) {
        int length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        GL_CHECK;
        unique_ptr<char[]> buffer = make_unique<char[]>(size_t(length));
        glGetShaderInfoLog(shader_id, length, nullptr, buffer.get());
        GL_CHECK;
        /// TODO: use proper error logger
        std::cerr << "Addition failed for shader \""
            << filename << "\": " << buffer << std::endl;
    }
}

void fields_engine::graphics::shader::finalize() const {
    glLinkProgram(m_program_id);
    GL_CHECK;
    int status = 0;
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &status);
    GL_CHECK;
    // Something went wrong, log an error
    if (status != 1) {
        int length = 0;
        glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &length);
        GL_CHECK;
        unique_ptr<char[]> buffer = make_unique<char[]>(size_t(length));
        glGetProgramInfoLog(m_program_id, length, nullptr, buffer.get());
        GL_CHECK;
        /// TODO: use proper error logger
        std::cerr << "Finalization failed for shader: " << buffer << std::endl;
    }
}

void fields_engine::graphics::shader::use() const {
	glUseProgram(m_program_id);
}

void fields_engine::graphics::shader::unuse() const {
	glUseProgram(0);
}

int fields_engine::graphics::shader::id() const {
    return m_program_id;
}
