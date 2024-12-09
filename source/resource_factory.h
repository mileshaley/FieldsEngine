/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: resource_factory.h                                                  *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include <unordered_map>

/*~-------------------------------------------------------------------------~*\
 * Resource Factory Class Template                                           *
\*~-------------------------------------------------------------------------~*/

//namespace fields_engine {
//
//	template<typename T, typename ID_Type = string, template<ID_Type> class Allocator>
//	class resource_factory {
//	public:
//		resource_factory();
//
//		T* get(ID_Type const& hash_source);
//		unique<T> make_unique(ID_Type const& hash_source);
//		T* find(ID_Type const& hash_source);
//
//		void clear();
//		
//	private:
//		map m_map;
//	};
//
//
//	template<typename T, typename ID_Type>
//	inline resource_factory<T, ID_Type>::resource_factory() 
//		: m_map()
//	{
//	}
//
//	template<typename T, typename ID_Type>
//	inline T* resource_factory<T, ID_Type>::get(ID_Type const& hash_source) {
//		auto it = m_map.find(hash_source);
//		if (it != m_map.end()) {
//			return *it;
//		}
//		
//
//
//		return nullptr;
//	}
//
//	template<typename T, typename ID_Type>
//	inline unique<T> resource_factory<T, ID_Type>::make_unique(ID_Type const& hash_source) {
//		return 
//	}
//
//	template<typename T, typename ID_Type>
//	inline T* resource_factory<T, ID_Type>::find(ID_Type const& hash_source) {
//		return ;
//	}
//
//	template<typename T, typename ID_Type>
//	inline void resource_factory<T, ID_Type>::clear() {
//
//	}
//
//} // namespace fields_engine
