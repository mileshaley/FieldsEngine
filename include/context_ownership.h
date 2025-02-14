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

/*~-------------------------------------------------------------------------~*\
 * Unique Pointer Context Ownership Class                                    *
\*~-------------------------------------------------------------------------~*/

#define TEST_CTX_REPL true ||

namespace fields_engine {

	using namespace fields_engine;
	template<class T>
	class box_context {
	public:
		using type = impl::remove_all_t<T>;

		inline box_context(box<type>&& ptr)
			: m_ptr(move(ptr))
		{
			T*& current = impl::context_storage<type>::ptr;
			if (TEST_CTX_REPL current == nullptr) {
				current = m_ptr.get();
				impl::context_storage<type>::initialize();
			}
		}

		inline ~box_context() {
			type*& current = impl::context_storage<type>::ptr;
			if (current == m_ptr.get()) {
				current = nullptr;
			}
		}

		inline box_context& operator=(box<type>&& rhs) {
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

		inline box_context& operator=(box_context&& rhs) noexcept {
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
			*this = box<type>(new_ptr);
		}

		inline void use() {
			impl::context_storage<type>::ptr = m_ptr.get();
		}

		FE_NODISCARD inline type& operator*() const noexcept(noexcept(*m_ptr)) {
			return *m_ptr;
		}
		FE_NODISCARD inline type* operator->() const noexcept {
			return m_ptr.get();
		}

		FE_NODISCARD inline type* get() const noexcept {
			return m_ptr.get();
		}

	private:
		box<type> m_ptr;
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
			if (TEST_CTX_REPL current == nullptr) {
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
