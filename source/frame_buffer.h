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
		frame_buffer();
		~frame_buffer();

		void use() const;
		void unuse() const;

		unsigned get_texture_id();

	private:
		unsigned m_fbo_id;
		unsigned m_rbo_id;
		unsigned m_tex_id;
	};

} // namespace fields_engine::graphics
