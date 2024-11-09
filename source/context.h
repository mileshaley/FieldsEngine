/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: context.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

// context<application>().run();

#define GENERATE_GLOBAL_CONTEXT_FOR_TYPE(mp_type)   \
	namespace detail {             \
		template<>               \
		inline mp_type** context_ptr<mp_type>() {					\
			static mp_type* ptr;					\
			return &ptr;                             \
		}                                           \
	} // namespace (fields_engine)::detail

	//static_assert(0, "Use GENERATE_GLOBAL_CONTEXT_FOR_TYPE()");


namespace fields_engine {


	namespace detail {
		template<class T>
		inline T** context_ptr(void) {
		}
	} // namespace detail

	template<class T>
	inline FE_NODISCARD T* context() {
		return *detail::context_ptr<T>();
	}

	//template<class T>
	//void set_context_ptr(T* newCurrentContext) {
	//	*detail::context_ptr<T>() = newCurrentContext;
	//}

	using namespace fields_engine;
	template<class T>
	class unique_context {
	public:

		unique_context(std::unique_ptr<T>&& ptr) 
			: m_ptr(std::move(ptr))
		{
			T*& current = *detail::context_ptr<T>();
			if (current == nullptr) {
				current = m_ptr.get();
			}
		}

		~unique_context() {
			T*& current = *detail::context_ptr<T>();
			if (m_ptr.get() == current) {
				*detail::context_ptr<T>() = nullptr;
			}
		}

		unique_context& operator=(unique_ptr<T>&& rhs) {
			T*& current = *detail::context_ptr<T>();
			if (current == m_ptr.get() || current == nullptr) {
				current = rhs.get();
			}
			m_ptr = move(rhs);
			return *this;
		}

		void reset(T* new_ptr = nullptr) {
			*this = unique_ptr<T>(new_ptr);
		}

		void use() {
			*detail::context_ptr<T> = m_ptr.get();
		}

		FE_NODISCARD T& operator*() const {
			return *m_ptr;
		}
		FE_NODISCARD T* operator->() const {
			return m_ptr.get();
		}

		FE_NODISCARD T* get() const {
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
			T*& current = *detail::context_ptr<T>();
			if (current == nullptr) {
				current = &m_data;
			}
		}

		~local_context() {
			T*& current = *detail::context_ptr<T>();
			if (&m_data == current) {
				*detail::context_ptr<T>() = nullptr;
			}
		}

		void use() {
			*detail::context_ptr<T> = &m_data;
		}

		FE_NODISCARD T* operator->() const {
			return &m_data;
		}

		FE_NODISCARD T& get() const {
			return m_data;
		}

	private:
		T m_data;
	};

} // namespace fields_engine
