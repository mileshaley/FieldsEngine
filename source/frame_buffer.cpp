/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: frame_buffer.cpp                                                    *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "frame_buffer.h"
#include "graphics.h"
#include "glad/glad.h"

/*~-------------------------------------------------------------------------~*\
 * Frame Buffer Definitions                                                  *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {
	frame_buffer::frame_buffer(ivec2 size) 
		: m_size(size)
	{
		create();
	}

	frame_buffer::~frame_buffer() {
		destroy();
	}

	void frame_buffer::viewport() {
		glViewport(0, 0, m_size.x, m_size.y);
		VIS_VERIFY;
	}

	void frame_buffer::use() const {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
		VIS_VERIFY;
	}

	void frame_buffer::unuse() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		VIS_VERIFY;
	}

	void frame_buffer::resize(ivec2 new_size) {
		destroy();
		m_size = new_size;
		create();
		//usage_status usage = get_usage_status();
		//if (usage == usage_status::unused) {
		//	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
		//	VIS_VERIFY;
		//}
		//m_size = new_size;
		//glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
		//glViewport(0, 0, new_size.x, new_size.y);
		//
		//glBindTexture(GL_TEXTURE_2D, m_tex_id);
		//VIS_VERIFY;
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, new_size.x, new_size.y, 
		//	0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		//VIS_VERIFY;
		//glBindTexture(GL_TEXTURE_2D, 0);
		//VIS_VERIFY;
		//
		//glBindRenderbuffer(GL_RENDERBUFFER, m_rbo_id);
		//VIS_VERIFY;
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 
		//	new_size.x, new_size.y);
		//VIS_VERIFY;
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//VIS_VERIFY;
		//
		//
		//FE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
		//	"Something went wrong during frame buffer resize"
		//);
		////if (usage == usage_status::unused) {
		////	glBindFramebuffer(GL_FRAMEBUFFER, 0);
		////	VIS_VERIFY;
		////}
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	frame_buffer::usage_status frame_buffer::get_usage_status() const {
		int draw_fbo_id;
		int read_fbo_id;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw_fbo_id);
		VIS_VERIFY;
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read_fbo_id);
		VIS_VERIFY;

		return usage_status{
			int(draw_fbo_id == m_fbo_id) | (int(read_fbo_id == m_fbo_id) << 1)
		};
	}

	void frame_buffer::create() {
		glGenFramebuffers(1, &m_fbo_id);
		VIS_VERIFY;
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
		VIS_VERIFY;
		glGenTextures(1, &m_tex_id);
		VIS_VERIFY;
		glBindTexture(GL_TEXTURE_2D, m_tex_id);
		VIS_VERIFY;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_size.x, m_size.y,
			0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		VIS_VERIFY;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		VIS_VERIFY;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		VIS_VERIFY;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex_id, 0);
		VIS_VERIFY;

		glGenRenderbuffers(1, &m_rbo_id);
		VIS_VERIFY;
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo_id);
		VIS_VERIFY;
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
			m_size.x, m_size.y);
		VIS_VERIFY;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo_id);
		VIS_VERIFY;
		FE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
			"Something went wrong during frame buffer creation"
		);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		VIS_VERIFY;
		glBindTexture(GL_TEXTURE_2D, 0);
		VIS_VERIFY;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		VIS_VERIFY;
	}

	void frame_buffer::destroy() {
		glDeleteRenderbuffers(1, &m_rbo_id);
		VIS_VERIFY;
		glDeleteTextures(1, &m_tex_id);
		VIS_VERIFY;
		glDeleteFramebuffers(1, &m_fbo_id);
		VIS_VERIFY;
		//m_rbo_id = m_tex_id = m_fbo_id = 0;
	}


/*~-------------------------------------------------------------------------~*\
 * Dual Frame Buffer Definitions                                             *
\*~-------------------------------------------------------------------------~*/

	dual_frame_buffer::dual_frame_buffer(ivec2 size)
		: m_fb_1(size)
		, m_fb_2(size)
		, m_1_active(true)
	{
	}

	void dual_frame_buffer::swap() {
		frame_buffer::usage_status usage = ref_active().get_usage_status();

		int inactive_fbo_id = ref_inactive().get_frame_buffer_id();
		if (usage == frame_buffer::usage_status::used) {
			glBindFramebuffer(GL_FRAMEBUFFER, inactive_fbo_id);
		} else if (usage == frame_buffer::usage_status::used_draw_only) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, inactive_fbo_id);
		} else if (usage == frame_buffer::usage_status::used_read_only) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, inactive_fbo_id);
		} // else do nothing (neither of our fbo were bound)
		VIS_VERIFY;
		m_1_active = !m_1_active;
	}

	void dual_frame_buffer::viewport() {
		m_fb_1.viewport();
	}

	void dual_frame_buffer::use() const {
		ref_active().use();
	}

	void dual_frame_buffer::unuse() const {
		ref_active().unuse();
	}

	void dual_frame_buffer::resize(ivec2 new_size) {
		m_fb_1.resize(new_size);
		m_fb_2.resize(new_size);
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

} // namespace fields_engine::vis
