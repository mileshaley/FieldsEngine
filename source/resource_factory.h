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

/*~-------------------------------------------------------------------------~*\
 * Resource Factory Class Template                                           *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	template<typename T, typename KeyT>
	class default_resource_factory_handler {
	public:
		static inline unique<T> copy(T const& source) {
			return FE clone(source);
		}

		static inline void post_allocate(T const& resource, KeyT const& key) {

		}
	};

	// Wrapper for a map for managing a heap allocated resource
	template<
		typename T, 
		typename KeyT = string, 
		class Handler = default_resource_factory_handler<T, KeyT>
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
			Handler::post_allocate(new_item, key);
			return new_item;
		}

		inline unique<T> make_unique(KeyT const& key) {
			return Handler::copy(*get(key));
		}

		// Resets the state of the resource factory, freeing all of its resources
		// The resource factory can still be used after this
		inline void clear() {
			m_map.clear();
		}
		
	private:
		map<KeyT, T> m_map;
	};

} // namespace fields_engine
