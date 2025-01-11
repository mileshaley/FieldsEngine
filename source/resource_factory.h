/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: resource_factory.h                                                  *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include <unordered_map>
#include "polymorphic.h" // clone()
#include "assert.h"

/*~-------------------------------------------------------------------------~*\
 * Resource Factory Class Template                                           *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	template<typename T>
	struct default_resource_factory_copier {
		static inline box<T> copy(T const& source) {
			return FE clone(source);
		}
	};

	//template<typename T, typename KeyT>
	//struct default_resource_factory_handler {
	//public:
	//	static inline void post_allocate(T& resource, KeyT const& key) {
	//
	//	}
	//};

	// Wrapper for a map for managing a heap allocated resource
	template<
		typename T,
		typename KeyT = string,
		class Copier = default_resource_factory_copier<T>
	> class resource_factory {
	public:
		inline resource_factory()
			: m_map()
		{}

		// Returns a pointer to an existing item at key.
		// If item doesn't exist, it will not be created and nullptr will be returned
		inline T const* get_existing(KeyT const& key) const {
			auto it = m_map.find(key);
			if (it != m_map.end()) {
				return *it;
			}
			return nullptr;
		}

		// Returns a reference to the item at key.
		// If item doesn't exist, it will be created
		inline T const& get(KeyT const& key) {
			auto it = m_map.find(key);
			if (it != m_map.end()) {
				return *it;
			}

			T& new_item = m_map.emplace(key);
			return new_item;
		}

		// Makes a duplicate of an existing item at key.
		//
		inline T& duplicate(KeyT const& key, KeyT const& duplicated_item_key) {
			FE_ASSERT(key != duplicated_item_key);
			auto it = m_map.find(key);
			if (it != m_map.end()) {
				T& duplicated_item = m_map.emplace(key, move(Copier::Copy(*it)));
				return duplicated_item;
			}
			return m_map.emplace(duplicated_item_key);
		}

		inline box<T> make_unique_copy(KeyT const& key) {
			return Copier::copy(*get(key));
		}

		// Resets the state of the resource factory, freeing all of its resources
		// The resource factory can still be used after this
		inline void clear() {
			m_map.clear();
		}
		
	private:
		unordered_map<KeyT, T> m_map;
	};

} // namespace fields_engine
