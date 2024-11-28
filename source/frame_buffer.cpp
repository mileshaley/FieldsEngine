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

namespace fields_engine::graphics {
	frame_buffer::frame_buffer() {
		constexpr ivec2 windowSize = { 1920, 1080 };
		glGenFramebuffers(1, &m_fbo_id);
		FE_GL_VERIFY;
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
		FE_GL_VERIFY;
		glGenTextures(1, &m_tex_id);
		FE_GL_VERIFY;
		glBindTexture(GL_TEXTURE_2D, m_tex_id);
		FE_GL_VERIFY;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowSize.x, windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		FE_GL_VERIFY;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		FE_GL_VERIFY;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		FE_GL_VERIFY;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex_id, 0);
		FE_GL_VERIFY;

		glGenRenderbuffers(1, &m_rbo_id);
		FE_GL_VERIFY;
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo_id);
		FE_GL_VERIFY;
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowSize.x, windowSize.y);
		FE_GL_VERIFY;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo_id);
		FE_GL_VERIFY;
		FE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
			"Something went wrong during frame buffer creation"
		);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		FE_GL_VERIFY;
		glBindTexture(GL_TEXTURE_2D, 0);
		FE_GL_VERIFY;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		FE_GL_VERIFY;
	}

	frame_buffer::~frame_buffer() {
		glDeleteRenderbuffers(1, &m_rbo_id);
		FE_GL_VERIFY;
		glDeleteTextures(1, &m_tex_id);
		FE_GL_VERIFY;
		glDeleteFramebuffers(1, &m_fbo_id);
		FE_GL_VERIFY;
	}

	void frame_buffer::use() const {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
		FE_GL_VERIFY;
	}

	void frame_buffer::unuse() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		FE_GL_VERIFY;
	}

	unsigned frame_buffer::get_texture_id() const {
		return m_tex_id;
	}

	unsigned frame_buffer::get_frame_buffer_id() const {
		return m_fbo_id;
	}

	unsigned frame_buffer::get_render_buffer_id() const {
		return m_rbo_id;
	}

/*~-------------------------------------------------------------------------~*\
 * Dual Frame Buffer Definitions                                             *
\*~-------------------------------------------------------------------------~*/

	dual_frame_buffer::dual_frame_buffer()
		: m_fb_1()
		, m_fb_2()
		, m_1_active(true)
	{
	}

	void dual_frame_buffer::swap() {
		int draw_fbo_id;
		int read_fbo_id;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw_fbo_id);
		FE_GL_VERIFY;
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read_fbo_id);
		FE_GL_VERIFY;

		int active_fbo_id = ref_active().get_frame_buffer_id();
		int inactive_fbo_id = ref_inactive().get_frame_buffer_id();
		if (draw_fbo_id == active_fbo_id) {
			if (read_fbo_id == active_fbo_id) {
				glBindFramebuffer(GL_FRAMEBUFFER, inactive_fbo_id);
			} else {
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, inactive_fbo_id);
			}
		} else if (read_fbo_id == active_fbo_id) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, inactive_fbo_id);
		} // else do nothing (neither of our fbo were bound)
		FE_GL_VERIFY;
		m_1_active = !m_1_active;
	}

	void dual_frame_buffer::use() const {
		ref_active().use();
	}

	void dual_frame_buffer::unuse() const {
		ref_active().unuse();
	}

	unsigned dual_frame_buffer::get_texture_id() const {
		return ref_inactive().get_texture_id();
	}

	unsigned dual_frame_buffer::get_frame_buffer_id() const {
		return ref_active().get_frame_buffer_id();
	}

	unsigned dual_frame_buffer::get_render_buffer_id() const {
		return ref_active().get_render_buffer_id();
	}

	frame_buffer& dual_frame_buffer::ref_active() {
		return m_1_active ? m_fb_1 : m_fb_2;
	}

	frame_buffer const& dual_frame_buffer::ref_active() const {
		return m_1_active ? m_fb_1 : m_fb_2;
	}

	frame_buffer& dual_frame_buffer::ref_inactive() {
		return !m_1_active ? m_fb_1 : m_fb_2;
	}

	frame_buffer const& dual_frame_buffer::ref_inactive() const {
		return !m_1_active ? m_fb_1 : m_fb_2;
	}

} // namespace fields_engine::graphics
