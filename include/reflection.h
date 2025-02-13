/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: reflection.h                                                        *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"

/*~-------------------------------------------------------------------------~*\
 * Reflection Macro                                                       *
\*~-------------------------------------------------------------------------~*/

#define FE_REFLECT_BODY(Class)                                              \
	public:																	\
		static inline string_view static_type_name() {                      \
			static constexpr string_view name( #Class );			        \
			return name;											        \
		}																	\
		static inline fe::impl::type_registerer<Class>                      \
			internal_##Class##_type_registerer{static_type_name()};         \
		virtual string_view get_type_name() const /* override */ {			\
			return static_type_name();										\
		}																	\
	private:																\

/*~-------------------------------------------------------------------------~*\
 * Reflection Functions                                                      *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	namespace impl {

		class type_record_base {
		public:
			type_record_base() {

			}
			virtual ~type_record_base() {}
			virtual void* make() = 0;
		};

		template<class T>
		class type_record : public type_record_base {
		public:
			type_record() {}
			virtual void* make() {
				if constexpr (std::is_abstract_v<T>) {
					return nullptr;
				} else {
					return new T();
				}
			}
		};


		inline std::unordered_map<string, box<type_record_base>>& get_type_records() {
			static std::unordered_map<string, box<type_record_base>> records;
			return records;
		}
	} // namespace impl

	template<class T>
	void register_type(string const& type_name) {
		auto& type_records = impl::get_type_records();
		FE_ASSERT(
			type_records.find(type_name) == type_records.end(),
			"Type multiply registered. Each type should be registered only once."
		);
		type_records.emplace(type_name, make_box<impl::type_record<T>>());
	}

	template<class T>
	box<T> make_from_type_name(type_name const& type) {
		auto& type_records = impl::get_type_records();
		return box<T>{ static_cast<T*>(type_records.at(type)->make()) };
	}

	namespace impl {
		template<class T>
		class type_registerer {
		public:
			type_registerer(string_view type_name) {
				register_type<T>(string(type_name));
			}
		};
	} // namespace impl

} // namespace fields_engine
