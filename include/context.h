/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: context.h                                                           *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "error.h"

/*~-------------------------------------------------------------------------~*\
 * Context Storage & Access                                                  *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	namespace detail {
		template<typename T>
		struct context_storage {
			static inline T* ptr;

			static inline void initialize() {
#ifdef _DEBUG
				initialized_src() = true;
#endif // DEBUG
			}

			static inline void verify() {
#ifdef _DEBUG
				if (ptr == nullptr) {
					if (initialized_src()) {
						FE_FAILED_ASSERT("Context was once established became null at some point before accessing");
					}
					else {
						FE_FAILED_ASSERT("Context was never established, check that the type is correct");
					}
				}
#endif // _DEBUG
			}
		private:
#ifdef _DEBUG
			static bool& initialized_src() {
				static bool initialized = false;
				return initialized;
			}
#endif // _DEBUG
		};

		/// TODO: Relocate
		template<typename T>
		using remove_all_t = 
			std::remove_all_extents_t<
			std::remove_pointer_t<
			std::remove_reference_t<
			std::remove_cv_t<T>>>>;
	} // namespace detail

	template<class T>
	FE_NODISCARD inline detail::remove_all_t<T>& context() noexcept(noexcept(*std::declval<detail::remove_all_t<T>*>())) {
		detail::context_storage<detail::remove_all_t<T>>::verify();
		return *detail::context_storage<detail::remove_all_t<T>>::ptr;
	}

	template<class T>
	FE_NODISCARD inline detail::remove_all_t<T>* context_unchecked() noexcept(noexcept(*std::declval<detail::remove_all_t<T>*>())) {
		return detail::context_storage<detail::remove_all_t<T>>::ptr;
	}

	template<class T>
	FE_NODISCARD inline bool context_active() noexcept {
		return detail::context_storage<detail::remove_all_t<T>>::ptr != nullptr;
	}
} // namespace fields_engine
