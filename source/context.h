/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: context.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "error.h"

namespace fields_engine {
	namespace detail {
		template<typename T>
		struct context_storage {
			static inline T* ptr;

			static inline void initialize() {
#ifdef DEBUG
				initialized_src() = true;
#endif // DEBUG
			}

			static inline void verify() {
#ifdef DEBUG
				if (ptr == nullptr) {
					if (initialized_src()) {
						FE_FAILED_ASSERT("Context was once established became null at some point before accessing");
					}
					else {
						FE_FAILED_ASSERT("Context was never established, check that the type is correct");
					}
				}
#endif // DEBUG
			}
		private:
#ifdef DEBUG
			static bool& initialized_src() {
				static bool initialized = false;
				return initialized;
			}
#endif // DEBUG
		};
	} // namespace detail
	

	template<class T>
	inline FE_NODISCARD T& context() noexcept(noexcept(*std::declval<T*>())) {
		detail::context_storage<T>::verify();
		return *detail::context_storage<T>::ptr;
	}

	using namespace fields_engine;
	template<class T>
	class unique_context {
	public:
		unique_context(std::unique_ptr<T>&& ptr) 
			: m_ptr(std::move(ptr))
		{
			T*& current = detail::context_storage<T>::ptr;
			if (current == nullptr) {
				current = m_ptr.get();
				detail::context_storage<T>::initialize();
			}
		}

		~unique_context() {
			T*& current = detail::context_storage<T>::ptr;
			if (current == m_ptr.get()) {
				current = nullptr;
			}
		}

		unique_context& operator=(unique_ptr<T>&& rhs) {
			T*& current = detail::context_storage<T>::ptr;
			if (current == m_ptr.get()) {
				current = rhs.get();
			} else if (current == nullptr) {
				current = rhs.get();
				detail::context_storage<T>::initialize();
			}
			m_ptr = move(rhs);
			return *this;
		}

		inline void reset(T* new_ptr = nullptr) {
			*this = unique_ptr<T>(new_ptr);
		}

		inline void use() {
			detail::context_storage<T>::ptr = m_ptr.get();
		}

		FE_NODISCARD inline T& operator*() const noexcept(noexcept(*m_ptr)) {
			return *m_ptr;
		}
		FE_NODISCARD inline T* operator->() const noexcept {
			return m_ptr.get();
		}

		FE_NODISCARD inline T* get() const noexcept {
			return m_ptr.get();
		}

	private:
		unique_ptr<T> m_ptr;
	};

	template<class T>
	class local_context {
	public:
		
		template<typename... Ts>
		local_context(Ts&&... args) 
			: m_data(std::forward<Ts>(args)...)
		{
			T*& current = detail::context_storage<T>::ptr;
			if (current == nullptr) {
				current = &m_data;
				detail::context_storage<T>::initialize();
			}
		}

		~local_context() {
			T*& current = detail::context_storage<T>::ptr;
			if (current == &m_data) {
				current = nullptr;
			}
		}

		inline void use() {
			detail::context_storage<T>::ptr = &m_data;
		}

		FE_NODISCARD inline T* operator->() noexcept {
			return &m_data;
		}

		FE_NODISCARD inline T const* operator->() const noexcept {
			return &m_data;
		}

		FE_NODISCARD inline operator T& () noexcept {
			return m_data;
		}

		FE_NODISCARD inline operator T const& () const noexcept {
			return m_data;
		}

		FE_NODISCARD inline T& get() noexcept {
			return m_data;
		}
		FE_NODISCARD inline T const& get() const noexcept {
			return m_data;
		}

	private:
		T m_data;
	};

} // namespace fields_engine
