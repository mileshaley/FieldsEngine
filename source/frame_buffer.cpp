/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: frame_buffer.cpp                                                    *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "frame_buffer.h"
#include "graphics.h"
#include "glad/glad.h"
#include "error.h"

/*~-------------------------------------------------------------------------~*\
 * Frame Buffer Definitions                                                  *
\*~-------------------------------------------------------------------------~*/

fields_engine::graphics::frame_buffer::frame_buffer() {
	glGenFramebuffers(1, &m_fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
	constexpr ivec2 windowSize = {1920, 1080};
	glGenTextures(1, &m_tex_id);
	glBindTexture(GL_TEXTURE_2D, m_tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowSize.x, windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex_id, 0);

	glGenRenderbuffers(1, &m_rbo_id);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowSize.x, windowSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo_id);

	FE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
		"Something went wrong during frame buffer creation"
	);
	
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

fields_engine::graphics::frame_buffer::~frame_buffer() {
	glDeleteRenderbuffers(1, &m_rbo_id);
	FE_GL_VERIFY;
	glDeleteTextures(1, &m_tex_id);
	FE_GL_VERIFY;
	glDeleteFramebuffers(1, &m_fbo_id);
	FE_GL_VERIFY; 
}

void fields_engine::graphics::frame_buffer::use() const {
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
	FE_GL_VERIFY;
}

void fields_engine::graphics::frame_buffer::unuse() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	FE_GL_VERIFY;
}

unsigned fields_engine::graphics::frame_buffer::get_texture_id() {
	return m_tex_id;
}
