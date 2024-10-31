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
	: programID_(glCreateProgram())
{}

void fields_engine::graphics::shader::add(const char* filename, int type) const {
    // Read a file's contents using fstream, stringstream, and string
    std::fstream file(filename);
    if (!file) { return; }
    std::stringstream ss;
    ss << file.rdbuf();
    string source = ss.str();
    // Array of one const char* to get const char**
    const char* sourcePtr[1] = { source.c_str() };

    int shaderID = glCreateShader(type);
    glAttachShader(programID_, shaderID);
    GL_CHECK;
    glShaderSource(shaderID, 1, sourcePtr, nullptr);
    GL_CHECK;
    glCompileShader(shaderID);
    GL_CHECK;

    int status = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    GL_CHECK;
    // Something went wrong, log an error
    if (status != 1) {
        int length = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        GL_CHECK;
        unique_ptr<char[]> buffer = make_unique<char[]>(size_t(length));
        glGetShaderInfoLog(shaderID, length, nullptr, buffer.get());
        GL_CHECK;
        /// TODO: use proper error logger
        std::cerr << "Addition failed for shader \""
            << filename << "\": " << buffer << std::endl;
    }
}

void fields_engine::graphics::shader::finalize() const {
    glLinkProgram(programID_);
    GL_CHECK;
    int status = 0;
    glGetProgramiv(programID_, GL_LINK_STATUS, &status);
    GL_CHECK;
    // Something went wrong, log an error
    if (status != 1) {
        int length = 0;
        glGetProgramiv(programID_, GL_INFO_LOG_LENGTH, &length);
        GL_CHECK;
        unique_ptr<char[]> buffer = make_unique<char[]>(size_t(length));
        glGetProgramInfoLog(programID_, length, nullptr, buffer.get());
        GL_CHECK;
        /// TODO: use proper error logger
        std::cerr << "Finalization failed for shader: " << buffer << std::endl;
    }
}

void fields_engine::graphics::shader::use() const {
	glUseProgram(programID_);
}

void fields_engine::graphics::shader::unuse() const {
	glUseProgram(0);
}

int fields_engine::graphics::shader::id() const {
    return programID_;
}
