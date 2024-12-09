/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: this_type.h                                                         *
 * Desc: Provides DEF_THIS_TYPE() macro for defining alias                   *
 *       for the inhabited class as 'this_type'                              *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * File Includes                                                             *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"

/*~-------------------------------------------------------------------------~*\
 * This Type Define                                                          *
\*~-------------------------------------------------------------------------~*/

#define DEF_THIS_TYPE                                                                          \
	struct internal_this_type_key {};                                                           \
	inline auto internal_this_type_setter() -> decltype(                                         \
			::fields_engine::detail::this_type::setter<internal_this_type_key, decltype(this)>{}, \
			void()                                                                                 \
	) {}                                                                                            \
	using this_type = ::fields_engine::detail::this_type::getter<internal_this_type_key>;
	
/*~-------------------------------------------------------------------------~*\
 * This Type Implementation                                                  *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::detail::this_type {

	template<class T>
	struct getter {
		friend auto deduction_dummy(getter<T>);
	};

	template<class T, class U>
	struct setter {
		// Defined
		friend auto deduction_dummy(getter<T>) {
			return U{};
		}
	};
	
	void deduction_dummy() {}

	template<class T>
	using type = std::remove_pointer_t<decltype(deduction_dummy(getter<T>{}))>;

} // namespace fields_engine::detail::this_type
