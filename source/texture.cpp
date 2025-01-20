/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: texture.cpp                                                         *
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "texture.h"

#include "graphics.h"
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "base64/base64.hpp"
#include <filesystem>

/// TODO: remove when using proper error logger
#include <iostream>

/*~-------------------------------------------------------------------------~*\
 * Texture Definitions                                                       *
\*~-------------------------------------------------------------------------~*/

fields_engine::vis::texture::texture() 
    : m_tex_id(0)
    , m_size(-1, -1)
    , m_num_channels(0)
    , m_active_unit(0)
{
}

fields_engine::vis::texture::texture(texture&& other) noexcept
    : m_tex_id(std::exchange(other.m_tex_id, 0))
    , m_size(other.m_size)
    , m_num_channels(other.m_num_channels)
    , m_active_unit(other.m_active_unit)
{
}

fields_engine::vis::texture::texture(std::filesystem::path const& file)
    : m_tex_id(0)
    , m_size(-1, -1)
    , m_num_channels(0)
    , m_active_unit(0)
{
    int num_channels = 4;
    // Flip for OpenGL
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* image_data = stbi_load(file.string().c_str(), 
        &m_size.x, &m_size.y, &num_channels, num_channels);
    if (image_data == nullptr) {
        /// TODO: Do proper error logging
        std::cerr << "Load failed for texture \""
            << file << "\"" << std::endl;
        return;
    }
    m_num_channels = static_cast<i8>(num_channels);
    generate(image_data);
    stbi_image_free(image_data);
}

fields_engine::vis::texture::~texture() {
    glDeleteTextures(1, &m_tex_id);
    VIS_VERIFY;
}

void fields_engine::vis::texture::load(json const& in) {
    vector<u8> raw_data = base64::decode_into<vector<u8>>(in);
    int num_channels = -1;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* image_data = stbi_load_from_memory(raw_data.data(), raw_data.size(),
        &m_size.x, &m_size.y, &num_channels, 0);
    if (image_data == nullptr) {
        return;
    }
    m_num_channels = static_cast<i8>(num_channels);
    generate(image_data);
    stbi_image_free(image_data);
}

fe::ivec2 fields_engine::vis::texture::get_size() const {
    return m_size;
}

fe::u32 fields_engine::vis::texture::get_id() const {
    return m_tex_id;
}

void* fields_engine::vis::texture::get_void_ptr_id() const {
    return reinterpret_cast<void*>(static_cast<i64>(m_tex_id));
}

void fields_engine::vis::texture::set_unit(int new_unit_num) {
    m_active_unit = static_cast<i8>(new_unit_num);
}

int fields_engine::vis::texture::get_unit() const {
    return m_active_unit;
}

void fields_engine::vis::texture::use() const {
    glActiveTexture(GL_TEXTURE0 + int(m_active_unit));
    VIS_VERIFY;
    glBindTexture(GL_TEXTURE_2D, m_tex_id);
    VIS_VERIFY;
}

void fields_engine::vis::texture::unuse() const {
    glActiveTexture(GL_TEXTURE0 + int(m_active_unit));
    VIS_VERIFY;
    glBindTexture(GL_TEXTURE_2D, 0);
    VIS_VERIFY;
}

void fields_engine::vis::texture::generate(u8* image_data) {
    glGenTextures(1, &m_tex_id);
    VIS_VERIFY;
    glBindTexture(GL_TEXTURE_2D, m_tex_id);
    VIS_VERIFY;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    VIS_VERIFY;
    // Copy the image into a gl texture
    
    const int channel_format = m_num_channels == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, channel_format, m_size.x, m_size.y,
        0, channel_format, GL_UNSIGNED_BYTE, image_data);
    VIS_VERIFY;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10);
    VIS_VERIFY;
    glGenerateMipmap(GL_TEXTURE_2D);
    VIS_VERIFY;
    /// TODO: Make this a parameter/variable
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_LINEAR);
    VIS_VERIFY;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR_MIPMAP_LINEAR);
    VIS_VERIFY;
    // Horizontal wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    VIS_VERIFY;
    // Vertical wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    VIS_VERIFY;
    glBindTexture(GL_TEXTURE_2D, 0);
    VIS_VERIFY;
}
