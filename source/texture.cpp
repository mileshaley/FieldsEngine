/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: texture.cpp                                                         *
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "texture.h"

#include "graphics.h"
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
// Deal with compiler specific pragmas here
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#include "stb/stb_image.h"
#pragma GCC diagnostic pop
#else // ^^^ GNUC / !GNUC vvv
#include "stb/stb_image.h"
#endif // !GNUC

#include "base64/base64.hpp"

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
    , m_upscale_filter()
    , m_downscale_filter()
{
}

fields_engine::vis::texture::texture(texture&& other) noexcept
    : m_tex_id(std::exchange(other.m_tex_id, 0))
    , m_size(other.m_size)
    , m_num_channels(other.m_num_channels)
    , m_active_unit(other.m_active_unit)
    , m_upscale_filter(other.m_upscale_filter)
    , m_downscale_filter(other.m_downscale_filter)
{
}

fields_engine::vis::texture::texture(std::filesystem::path const& file)
    : m_tex_id(0)
    , m_size(-1, -1)
    , m_num_channels(0)
    , m_active_unit(0)
    , m_upscale_filter()
    , m_downscale_filter()
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

void fields_engine::vis::texture::read(json const& in) {
    vector<u8> raw_data = base64::decode_into<vector<u8>>(in.at("raw").get<string>());
    int num_channels = -1;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* image_data = stbi_load_from_memory(raw_data.data(), int(raw_data.size()),
        &m_size.x, &m_size.y, &num_channels, 0);
    if (image_data == nullptr) {
        return;
    }
    m_num_channels = static_cast<i8>(num_channels);
    m_upscale_filter = in["upscale_filter"];
    m_downscale_filter = in["downscale_filter"];
    generate(image_data);
    stbi_image_free(image_data);
}

void fields_engine::vis::texture::write(json& out) const {
    /// TODO: Implement. This probably can wait because editing textures in engine is low prio
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

namespace fields_engine::vis {
    static int upscale_filter_to_gl(texture::upscale_filter filter) {
        if (filter == decltype(filter)::linear) {
            return GL_LINEAR;
        } else if (filter == decltype(filter)::nearest) {
            return GL_NEAREST;
        }
        return -1;
    }

    static int downscale_filter_to_gl(texture::downscale_filter filter) {
        using enum_t = texture::downscale_filter;
        switch (filter) {
        case enum_t::linear_mipmap_linear: return GL_LINEAR_MIPMAP_LINEAR;
        case enum_t::nearest_mipmap_nearest: return GL_NEAREST_MIPMAP_NEAREST;
        case enum_t::linear_mipmap_nearest: return GL_LINEAR_MIPMAP_NEAREST;
        case enum_t::nearest_mipmap_linear: return GL_NEAREST_MIPMAP_LINEAR;
        case enum_t::linear: return GL_LINEAR;
        case enum_t::nearest: return GL_NEAREST;
        default: return -1;
        }
    }
} // namespace fields_engine::vis


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
    
    // Upscale
    glTexParameteri(GL_TEXTURE_2D, 
        GL_TEXTURE_MAG_FILTER, upscale_filter_to_gl(m_upscale_filter));
    VIS_VERIFY;
    // Downscale
    glTexParameteri(GL_TEXTURE_2D, 
        GL_TEXTURE_MIN_FILTER, downscale_filter_to_gl(m_downscale_filter));
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
