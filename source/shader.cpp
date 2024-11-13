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
    FE_GL_VERIFY;
    glShaderSource(shader_id, 1, source_ptr, nullptr);
    FE_GL_VERIFY;
    glCompileShader(shader_id);
    FE_GL_VERIFY;

    int status = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
    FE_GL_VERIFY;
    // Something went wrong, log an error
    if (status != 1) {
        int length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        FE_GL_VERIFY;
        unique_ptr<char[]> buffer = make_unique<char[]>(size_t(length));
        glGetShaderInfoLog(shader_id, length, nullptr, buffer.get());
        FE_GL_VERIFY;
        /// TODO: use proper error logger
        std::cerr << "Addition failed for shader \""
            << filename << "\": " << buffer << std::endl;
    }
}

void fields_engine::graphics::shader::finalize() const {
    glLinkProgram(m_program_id);
    FE_GL_VERIFY;
    int status = 0;
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &status);
    FE_GL_VERIFY;
    // Something went wrong, log an error
    if (status != 1) {
        int length = 0;
        glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &length);
        FE_GL_VERIFY;
        unique_ptr<char[]> buffer = make_unique<char[]>(size_t(length));
        glGetProgramInfoLog(m_program_id, length, nullptr, buffer.get());
        FE_GL_VERIFY;
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

int fields_engine::graphics::shader::uniform_location(const char* name) const {
    int loc = glGetUniformLocation(m_program_id, name);
    FE_GL_VERIFY;
    return loc;
}

int fields_engine::graphics::shader::id() const {
    return m_program_id;
}
