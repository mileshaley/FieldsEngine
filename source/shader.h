/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: shader.h																 *
\*~-------------------------------------------------------------------------~*/

#pragma once

namespace fields_engine::graphics {
	class shader {
	public:
		shader();
	
		void add(const char* filename, int type) const;
		void finalize() const;
	
		void use() const;
		void unuse() const;
	
		int id() const;
	private:
		int programID_;
	};

} // namespace fields_engine::graphics
