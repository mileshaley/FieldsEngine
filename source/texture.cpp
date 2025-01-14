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

/// TODO: remove when using proper error logger
#include <iostream>

/*~-------------------------------------------------------------------------~*\
 * Texture Definitions                                                       *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {
    texture::texture(string_view filename) 
        : m_tex_id(0)
        , m_size(-1, -1)
        , m_num_channels(0)
        , m_active_unit(0)
    {
        int num_channels = 4;
        // Flip for OpenGL
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* image_data = stbi_load(filename.data(), &m_size.x, &m_size.y, 
            &num_channels, num_channels);
        if (image_data == nullptr) {
            /// TODO: use proper error logger
            std::cerr << "Load failed for texture \""
                << filename << "\"" << std::endl;
            return;
        }
        m_num_channels = num_channels;

        glGenTextures(1, &m_tex_id);
        VIS_VERIFY;
        glBindTexture(GL_TEXTURE_2D, m_tex_id);
        VIS_VERIFY;
        // Copy the image into a gl texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y,
            0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
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
        stbi_image_free(image_data);
    }
    
    texture::~texture() {
        glDeleteTextures(1, &m_tex_id);
        VIS_VERIFY;
    }

    ivec2 texture::get_size() const {
        return m_size;
    }

    void texture::set_unit(int new_unit_num) {
        m_active_unit = new_unit_num;
    }

    int texture::get_unit() const {
        return m_active_unit;
    }

    void texture::use() const {
        glActiveTexture(GL_TEXTURE0 + int(m_active_unit));
        glBindTexture(GL_TEXTURE_2D, m_tex_id);
        VIS_VERIFY;
    }

    void texture::unuse() const {
        glActiveTexture(GL_TEXTURE0 + int(m_active_unit));
        glBindTexture(GL_TEXTURE_2D, 0);
        VIS_VERIFY;
    }


} // namespace fields_engine::vis

