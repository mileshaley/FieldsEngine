/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: frame_buffer.h                                                      *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * Frame Buffer Class                                                        *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::graphics {

	class frame_buffer {
	public:
		enum class usage_status : int {
			unused = 0,
			used_draw_only = 1,
			used_read_only = 2,
			used = 3,
		};

		frame_buffer(ivec2 size = { 1920, 1080 });
		~frame_buffer();

		void viewport();
		void use() const;
		void unuse() const;

		void resize(ivec2 new_size);

		unsigned get_texture_id() const;
		unsigned get_frame_buffer_id() const;
		unsigned get_render_buffer_id() const;

		usage_status get_usage_status() const;

	private:
		void create();
		void destroy();

		unsigned m_fbo_id;
		unsigned m_rbo_id;
		unsigned m_tex_id;
		vec<2, i16> m_size;
	};

/*~-------------------------------------------------------------------------~*\
 * Frame Buffer Class                                                        *
\*~-------------------------------------------------------------------------~*/

	class dual_frame_buffer {
	public:
		dual_frame_buffer(ivec2 size = {1920, 1080});

		void swap();

		void viewport();
		void use() const;
		void unuse() const;

		void resize(ivec2 new_size);

		// Gets the inactive texture's id
		unsigned get_texture_id() const;
		// Gets the active texture's id
		unsigned get_frame_buffer_id() const;
		// Gets the active texture's id
		unsigned get_render_buffer_id() const;

	private:
		frame_buffer& ref_active();
		frame_buffer const& ref_active() const;
		frame_buffer& ref_inactive();
		frame_buffer const& ref_inactive() const;

		frame_buffer m_fb_1;
		frame_buffer m_fb_2;
		bool m_1_active;
	};

} // namespace fields_engine::graphics
