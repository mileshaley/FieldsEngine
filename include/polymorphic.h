/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: polymorphic.h                                                       *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "reflection.h"

/*~-------------------------------------------------------------------------~*\
 * Polymorphism Defines                                                      *
\*~-------------------------------------------------------------------------~*/

// Use within every polymorphic class's body
#define FE_CLASS_BODY(Class, Super)                                     \
	private: /* Specific to this class */                               \
		using this_type = Class;                                        \
		using super = Super;                                            \
		static inline string_view static_type_name() {                  \
			static constexpr string_view name( #Class );                \
			return name;                                                \
		}                                                               \
	static inline fe::impl::type_registerer<Class> Class##type_registerer_{static_type_name()}; \
	public: /* Common virtual functions */                              \
		virtual string_view get_type_name() const override {            \
			return static_type_name();                                  \
		}                                                               \
		virtual Class* internal_clone() const override {                \
			return new Class(*this);                                    \
		}                                                               \
	public: /* Macro should only be used in public areas of body */     \
																	    
#define FE_DEFINING_CLASS_BODY(Class)                                   \
	private: /* Specific to this class */                               \
		using this_type = Class;                                        \
		static inline string_view static_type_name() {                  \
			static constexpr string_view name( #Class );                \
			return name;                                                \
		}                                                               \
	public: /* Define common virtual functions */                       \
		using defining_base = Class;                                    \
				virtual Class* internal_clone() const {                 \
			return new Class(*this);                                    \
		}                                                               \
		virtual string_view get_type_name() const {                     \
			return static_type_name();                                  \
		}                                                               \
	public: /* Macro should only be used in public areas of body */     \

/*~-------------------------------------------------------------------------~*\
 * Polymorphism Functions                                                    *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	namespace impl {
		// Primary template. Does a simple copy
		template<typename T, class = void>
		struct clone_helper {
			static inline box<T> clone(T const& source) {
				return make_box<T>(source);
			}
		};
		// Specialized for classes with non-static internal_clone() function
		template<typename T>
		struct clone_helper<T, std::void_t<decltype(std::declval<T>().internal_clone())>> {
			static inline box<T> clone(T const& source) {
				return box<T>(static_cast<T*>(source.internal_clone()));
			}
		};
	} // namespace impl

	// Virtually clones a generic cloneable object as its real type and returns it
	// as a box<T> given that T <= the real type
	template<class T>
	inline box<T> clone(T const& source) {
		return impl::clone_helper<T>::clone(source);
	}


} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Polymorphism Templates                                                    *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	/// TODO: Make this get a pack of all classes in order in the hierarchy
	namespace impl {
		template<class T, class = void>
		struct deepest_super {
			using type = T;
		};
		template<class T>
		struct deepest_super<T, std::void_t<typename T::super>> {
			using type = typename deepest_super<typename T::super>::type;
		};
		template<class T>
		using deepest_super_t = typename impl::deepest_super<T>::type;
	}

} // namespace fields_engine
