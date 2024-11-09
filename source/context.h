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


namespace fields_engine {

	//static_assert(0, "Use GENERATE_GLOBAL_CONTEXT_FOR_TYPE()");

	namespace detail {
		template<class T>
		inline T** context_ptr(void) {
		}
	} // namespace detail

	template<class T>
	inline NO_DISCARD T* context() {
		return *detail::context_ptr<T>();
	}

	//template<class T>
	//void set_context_ptr(T* newCurrentContext) {
	//	*detail::context_ptr<T>() = newCurrentContext;
	//}

	using namespace fields_engine;
	template<class T>
	class context_ownership {
	public:

		context_ownership(std::unique_ptr<T>&& ptr = nullptr) 
			: ptr_(std::move(ptr))
		{
			T*& current = *detail::context_ptr<T>();
			if (current == nullptr) {
				current = ptr_.get();
			}
		}

		~context_ownership() {
			if (is_current_context()) {
				*detail::context_ptr<T>() = nullptr;
			}
		}

		context_ownership& operator=(unique_ptr<T>&& rhs) {
			if (ptr_.get() == rhs.get()) {
				return *this;
			}
			T*& current = *detail::context_ptr<T>();
			if (ptr_.get() == current || current == nullptr) {
				current = rhs.get();
				ptr_ = move(rhs);
			}
			return *this;
		}

		void reset(T* newPtr = nullptr) {
			*this = unique_ptr<T>(newPtr);
		}

		bool is_current_context() const noexcept {
			return *detail::context_ptr<T>() == ptr_.get();
		}

		void use() {
			*detail::context_ptr<T> = ptr_.get();
		}

		NO_DISCARD T& operator*() {
			return *ptr_;
		}
		NO_DISCARD T const& operator*() const {
			return *ptr_;
		}
		NO_DISCARD T* operator->() {
			return ptr_.operator->();
		}
		NO_DISCARD T const* operator->() const {
			return ptr_.operator->();
		}

		NO_DISCARD T* get() {
			return ptr_.get();
		}
		T const* get() const {
			return ptr_.get();
		}

	private:
		unique_ptr<T> ptr_;
	};

} // namespace fields_engine
