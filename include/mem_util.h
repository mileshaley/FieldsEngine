/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: mem_util.h                                                          *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include <memory.h>
#include <type_traits>
#include "fields_engine_defines.h"

namespace fields_engine {

/*~-------------------------------------------------------------------------~*\
 * Own Alias & Functions                                                     *
\*~-------------------------------------------------------------------------~*/

    // Equivalent to unique_ptr, signifies ownership of a dynamically allocated object
    template<typename T>
    using own = std::unique_ptr<T>;

    // Specialized for scalar T 
    template<typename T, typename... ArgTs, std::enable_if_t<!std::is_array_v<T>, int> = 0>
    FE_NODISCARD inline own<T> make_own(ArgTs&&... args) {
        return own<T>(new T(std::forward<ArgTs>(args)...));
    }

    // Specialized for one-dimensional array of T
    template<typename T, std::enable_if_t< std::is_array_v<T> && !std::extent_v<T>, int> = 0>
    FE_NODISCARD inline own<T> make_own(const size_t size) {
        using element_type = std::remove_extent_t<T>;
        return own<T>(new element_type[size]());
    }

    // Do not allow multi-dimensional array of T
    template<typename T, typename... ArgTs, std::enable_if_t<std::extent_v<T> != 0, int> = 0>
    void make_own(ArgTs&&...) = delete;

/*~-------------------------------------------------------------------------~*\
 * Move Semantics Functions                                                  *
\*~-------------------------------------------------------------------------~*/

} // namespace fields_engine
