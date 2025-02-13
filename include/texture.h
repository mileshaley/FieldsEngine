/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: texture.h                                                           *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "asset.h" // Inheritance

namespace std::filesystem {
	class path;
} // namespace std::filesystem

/*~-------------------------------------------------------------------------~*\
 * Texture Class                                                             *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::vis {



	class texture : public asset {
		FE_REFLECT_BODY(texture);
	public:
		texture();
		texture(texture&& other) noexcept;
		texture(std::filesystem::path const& file);
		virtual ~texture() override;


		virtual void read(json const& in) override;
		virtual void write(json& out) const override;

		ivec2 get_size() const;

		u32 get_id() const;
		void* get_void_ptr_id() const;

		// User must check that new_unit_num is between 0 and 31
		void set_unit(int new_unit_num);
		int get_unit() const;

		void use() const;
		void unuse() const;

		// Do not reorder
		enum class upscale_filter : i8 {
			linear,
			nearest,
		};

		// Do not reorder
		enum class downscale_filter : i8 {
			linear_mipmap_linear,
			nearest_mipmap_nearest,
			linear_mipmap_nearest,
			nearest_mipmap_linear,
			linear,
			nearest,
		};

	private:
		void generate(u8* image_data);

		u32 m_tex_id;
		ivec2 m_size;
		i8 m_num_channels;
		i8 m_active_unit = 0; // 0-31
		upscale_filter m_upscale_filter;
		downscale_filter m_downscale_filter;
	};

} // namespace fields_engine::vis
