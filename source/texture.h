/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: texture.h                                                           *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

namespace std::filesystem {
	class path;
} // namespace std::filesystem

/*~-------------------------------------------------------------------------~*\
 * Texture Class                                                             *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {
	class texture {
	public:
		texture();
		texture(texture&& other) noexcept;
		texture(std::filesystem::path const& file);
		~texture();

		void load(json const& in);

		ivec2 get_size() const;

		u32 get_id() const;
		void* get_void_ptr_id() const;

		// User must check that new_unit_num is between 0 and 31
		void set_unit(int new_unit_num);
		int get_unit() const;

		void use() const;
		void unuse() const;

	private:
		void generate(u8* image_data);

		u32 m_tex_id;
		ivec2 m_size;
		i8 m_num_channels;
		i8 m_active_unit = 0; // 0-31
	};

} // namespace fields_engine::vis
