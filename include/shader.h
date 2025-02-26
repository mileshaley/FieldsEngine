/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: shader.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

namespace fields_engine::vis {
	class shader {
	public:
		shader();
	
		void add(std::filesystem::path const& filename, int type) const;
		void finalize() const;
	
		void use() const;
		void unuse() const;

		int uniform_location(const char* name) const;
		int id() const;
	private:
		int m_program_id;
	};

} // namespace fields_engine::vis
