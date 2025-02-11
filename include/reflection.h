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
				return new T();
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
	box<T> make_from_type_name(string const& type_name) {
		auto& type_records = impl::get_type_records();
		return box<T>{ static_cast<T*>(type_records.at(type_name)->make()) };
	}


	namespace impl {
		template<class T, class = void>
		class type_registerer {
		public:
			type_registerer(string_view type_name) {
				register_type<T>(string(type_name));
			}
		};

		template<class T>
		class type_registerer<T, std::void_t<decltype(T::static_type_name())>> {
		public:
			type_registerer() {
				register_type<T>(string(T::static_type_name()));
			}
		};
	} // namespace impl

} // namespace fields_engine
