/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: shader.cpp                                                          *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "shader.h"
#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "graphics.h"

fields_engine::vis::shader::shader()
	: m_program_id(glCreateProgram())
{}

void fields_engine::vis::shader::add(const char* filename, int type) const {
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
    VIS_VERIFY;
    glShaderSource(shader_id, 1, source_ptr, nullptr);
    VIS_VERIFY;
    glCompileShader(shader_id);
    VIS_VERIFY;

    int status = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
    VIS_VERIFY;
    // Something went wrong, log an error
    if (status != 1) {
        int length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        VIS_VERIFY;
        own<char[]> buffer = make_own<char[]>(size_t(length));
        glGetShaderInfoLog(shader_id, length, nullptr, buffer.get());
        VIS_VERIFY;
        /// TODO: use proper error logger
        std::cerr << "Addition failed for shader \""
            << filename << "\": " << string(buffer.get(), length) << std::endl;
    }
}

void fields_engine::vis::shader::finalize() const {
    glLinkProgram(m_program_id);
    VIS_VERIFY;
    int status = 0;
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &status);
    VIS_VERIFY;
    // Something went wrong, log an error
    if (status != 1) {
        int length = 0;
        glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &length);
        VIS_VERIFY;
        own<char[]> buffer = make_own<char[]>(size_t(length));
        glGetProgramInfoLog(m_program_id, length, nullptr, buffer.get());
        VIS_VERIFY;
        /// TODO: use proper error logger
        std::cerr << "Finalization failed for shader: " 
            << string(buffer.get(), length) << std::endl;
    }
}

void fields_engine::vis::shader::use() const {
	glUseProgram(m_program_id);
}

void fields_engine::vis::shader::unuse() const {
	glUseProgram(0);
}

int fields_engine::vis::shader::uniform_location(const char* name) const {
    int loc = glGetUniformLocation(m_program_id, name);
    VIS_VERIFY;
    return loc;
}

int fields_engine::vis::shader::id() const {
    return m_program_id;
}
