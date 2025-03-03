/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: context_ownership.h                                                 *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "context.h"



namespace fields_engine {

	namespace impl {
		template<typename T, class = void>
		class use_context {
		public:
			static inline constexpr bool has_propagator = false;

			static inline void propagate(T*) {}
			static inline void propagate_unchecked(T&) {}
		};

		template<typename T>
		class use_context<T, std::enable_if_t<std::is_same_v<decltype(&T::use_context), void(T::*)()>>> {
		public:
			static inline bool constexpr has_propagator = true;

			static inline void propagate(T* context) {
				if (context != nullptr) {
					propagate_unchecked(*context);
				}
			}
			static inline void propagate_unchecked(T& context) {
				context.use_context();
			}
		};

	} // namespace impl

/*~-------------------------------------------------------------------------~*\
 * Unique Pointer Context Ownership Class                                    *
\*~-------------------------------------------------------------------------~*/

	template<class T>
	class own_context/**/ {
	public:
		using type = impl::remove_all_t<T>;

		inline own_context(own<type>&& ptr)
			: m_ptr(move(ptr))
		{
			T*& current = impl::context_storage<type>::ptr;
			if (current == nullptr) {
				current = m_ptr.get();
				impl::context_storage<type>::initialize();
			}
		}

		inline ~own_context() {
			type*& current = impl::context_storage<type>::ptr;
			if (current == m_ptr.get()) {
				current = nullptr;
			}
		}

		inline own_context& operator=(own<type>&& rhs) {
			type*& current = impl::context_storage<type>::ptr;
			if (current == m_ptr.get()) {
				current = rhs.get();
			}
			else if (current == nullptr) {
				current = rhs.get();
				impl::context_storage<type>::initialize();
			}
			m_ptr = move(rhs);
			return *this;
		}

		inline own_context& operator=(own_context&& rhs) noexcept {
			type*& current = impl::context_storage<type>::ptr;
			if (current == m_ptr.get()) {
				current = rhs.get();
			} else if (current == nullptr) {
				current = rhs.get();
				impl::context_storage<type>::initialize();
			}
			m_ptr = move(rhs.m_ptr);
			return *this;
		}

		inline void reset(type* new_ptr = nullptr) {
			*this = own<type>(new_ptr);
		}

		inline void use() {
			impl::context_storage<type>::ptr = m_ptr.get();
			impl::use_context<T>::propagate(m_ptr.get());
		}

		FE_NODISCARD inline bool operator!() const noexcept {
			return m_ptr == nullptr;
		}

		FE_NODISCARD inline type& operator*() const noexcept(noexcept(*std::declval<type*>())) {
			return *m_ptr;
		}
		FE_NODISCARD inline type* operator->() const noexcept {
			return m_ptr.get();
		}

		FE_NODISCARD inline type* get() const noexcept {
			return m_ptr.get();
		}

	private:
		own<type> m_ptr;
	};

/*~-------------------------------------------------------------------------~*\
 * Non-Owning Pointer Context Manegement Class                               *
\*~-------------------------------------------------------------------------~*/

	template<class T>
	class manage_context {
	public:
		using type = impl::remove_all_t<T>;

		inline manage_context(type* ptr)
			: m_ptr(ptr) {
			T*& current = impl::context_storage<type>::ptr;
			if (current == nullptr) {
				current = m_ptr;
				impl::context_storage<type>::initialize();
			}
		}

		inline ~manage_context() {
			type*& current = impl::context_storage<type>::ptr;
			if (current == m_ptr) {
				current = nullptr;
			}
		}

		inline manage_context& operator=(type* rhs) {
			type*& current = impl::context_storage<type>::ptr;
			if (current == m_ptr) {
				current = rhs;
			} else if (current == nullptr) {
				current = rhs;
				impl::context_storage<type>::initialize();
			}
			m_ptr = rhs;
			return *this;
		}

		inline manage_context& operator=(manage_context&& rhs) noexcept {
			type*& current = impl::context_storage<type>::ptr;
			if (current == m_ptr) {
				current = rhs;
			} else if (current == nullptr) {
				current = rhs;
				impl::context_storage<type>::initialize();
			}
			m_ptr = rhs.m_ptr;
			return *this;
		}

		inline void use() {
			impl::context_storage<type>::ptr = m_ptr;
			impl::use_context<T>::propagate(m_ptr);
		}

		FE_NODISCARD inline bool operator!() const noexcept {
			return m_ptr == nullptr;
		}

		FE_NODISCARD inline type& operator*() const noexcept(noexcept(*std::declval<type*>())) {
			return *m_ptr;
		}
		FE_NODISCARD inline type* operator->() const noexcept {
			return m_ptr;
		}

		FE_NODISCARD inline type* get() const noexcept {
			return m_ptr;
		}

	private:
		type* m_ptr;
	};

/*~-------------------------------------------------------------------------~*\
 * Local Context Ownership Class                                             *
\*~-------------------------------------------------------------------------~*/

	template<class T>
	class local_context {
	public:
		using type = impl::remove_all_t<T>;

		template<typename... Ts>
		inline local_context(Ts&&... args)
			: m_data(std::forward<Ts>(args)...)
		{
			type*& current = impl::context_storage<type>::ptr;
			if (current == nullptr) {
				current = &m_data;
				impl::context_storage<type>::initialize();
			}
		}

		inline ~local_context() {
			type*& current = impl::context_storage<type>::ptr;
			if (current == &m_data) {
				current = nullptr;
			}
		}

		inline void use() {
			impl::context_storage<type>::ptr = &m_data;
			impl::use_context<T>::propagate_unchecked(m_data);
		}

		FE_NODISCARD inline type* operator->() noexcept {
			return &m_data;
		}

		FE_NODISCARD inline type const* operator->() const noexcept {
			return &m_data;
		}

		FE_NODISCARD inline operator type& () noexcept {
			return m_data;
		}

		FE_NODISCARD inline operator type const& () const noexcept {
			return m_data;
		}

		FE_NODISCARD inline type& get() noexcept {
			return m_data;
		}
		FE_NODISCARD inline type const& get() const noexcept {
			return m_data;
		}

	private:
		type m_data;
	};
} // namespace fields_engine
