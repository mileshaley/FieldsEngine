/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: polymorphic.h                                                       *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * Polymorphism Defines                                                      *
\*~-------------------------------------------------------------------------~*/

// Use within every polymorphic class's body
#define FE_CLASS_BODY(p_class, p_super)                                 \
	private: /* Specific to this class */                               \
		using this_type = p_class;                                      \
		using super = p_super;                                          \
		static inline string_view static_type_name() {                  \
			static constexpr string_view name( #p_class );              \
			return name;                                                \
		}                                                               \
	public: /* Common virtual functions */                              \
		virtual string_view get_type_name() const override {            \
			return static_type_name();                                  \
		}                                                               \
		virtual p_class* internal_clone() const override {              \
			return new p_class(*this);                                  \
		}                                                               \
	public: /* Macro should only be used in public areas of body */     \

#define FE_DEFINING_CLASS_BODY(p_class)                                 \
	private: /* Specific to this class */                               \
		using this_type = p_class;                                      \
		static inline string_view static_type_name() {                  \
			static constexpr string_view name( #p_class );              \
			return name;                                                \
		}                                                               \
	public: /* Define common virtual functions */                       \
		using defining_base = p_class;                                  \
				virtual p_class* internal_clone() const {               \
			return new p_class(*this);                                  \
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
