/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: texture.h                                                           *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * Texture Class                                                             *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {
	class texture {
	public:
		texture(string_view filename);
		~texture();

		ivec2 get_size() const;

		u32 get_id() const;

		// User must check that new_unit_num is between 0 and 31
		void set_unit(int new_unit_num);
		int get_unit() const;

		void use() const;
		void unuse() const;

	private:
		u32 m_tex_id;
		ivec2 m_size;
		i16 m_num_channels;
		i16 m_active_unit = 0; // 0-31
	};

} // namespace fields_engine::vis
