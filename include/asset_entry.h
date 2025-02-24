/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: asset_entry.h                                                       *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#if EDITOR
#include "texture.h" // Asset thumbnail
#endif // EDITOR

namespace fields_engine {
	class asset;
} // namespace fields_engine


/*~-------------------------------------------------------------------------~*\
 * Asset Entry Class                                                         *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class asset_entry {
	public:
		explicit asset_entry(std::filesystem::path const& path);
		//asset_entry(asset_entry const& other);
		~asset_entry();

		void set_path(std::filesystem::path const& path);

		bool load();

		bool is_loaded() const;
		asset* get_load_asset();
		asset* get_asset_unchecked();

		string const& get_str_id() const;
		string const& get_name() const;
		string_view get_type() const;

#if EDITOR
		vis::texture const& get_thumbnail() const;
#endif // EDITOR

	private:
		own<asset> m_asset;
#if EDITOR
		/// TODO: Make this (or textures in general) less wasteful
		vis::texture m_thumbnail;
#endif // EDITOR
		std::filesystem::path m_path;
		// Stores the composite string id with the form name.type 
		// as seen in the asset path
		string m_str_id;
		// We save a duplicate copy of the name so it can be null terminated
		string m_name;
		// Save the position of the . separator in the str id 
		// so the type substr can be extracted easily 
		int m_str_id_separator_offset;

	}; // class asset_entry
} // namespace fields_engine
