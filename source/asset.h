/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: asset.h                                                             *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

namespace std::filesystem {
	class path;
} // namespace std::filesystem

/*~-------------------------------------------------------------------------~*\
 * Asset Class                                                               *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class asset {
	public:
		asset() = delete;
		asset(asset const&) = delete;
		asset& operator=(asset const&) = delete;
		asset& operator=(asset&&) = delete;

		asset(asset&& other) noexcept;
		asset(std::filesystem::path const& info_path);

		~asset();

		// Returns false if the asset is still unloaded
		bool load();
		bool reload();
		bool unload();

		bool is_loaded() const;
		string const& get_name() const;
		type_name const& get_type() const;

		template<typename T>
		T* get_data() {
			return reinterpret_cast<T*>(m_data);
		}

		template<typename T>
		T const* get_data() const {
			return reinterpret_cast<T*>(m_data);
		}

	private:
		/// TODO: Use a safer, more C++-like mechanism to store data
		void* m_data;
		string m_name;
		type_name m_type;
		// May be a path to data file or the data itself
		json m_data_source;
		bool m_data_source_is_path;
		bool m_valid;
		bool m_loaded;
	};

} // namespace fields_engine

