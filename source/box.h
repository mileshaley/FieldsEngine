/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: box.h                                                               *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include <memory.h>
#include <type_traits>

/*~-------------------------------------------------------------------------~*\
 * Box Functions                                                             *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
    template<typename T>
    using box = std::unique_ptr<T>;

    // Specialized for scalar T 
    template <typename T, typename... ArgTs, std::enable_if_t<!std::is_array_v<T>, int> = 0>
    [[nodiscard]] inline box<T> make_box(ArgTs&&... args) {
        return box<T>(new T(std::forward<ArgTs>(args)...));
    }

    // Specialized for one-dimensional array of T
    template <typename T, std::enable_if_t<std::is_array_v<T> and std::extent_v<T> == 0, int> = 0>
    [[nodiscard]] inline box<T> make_box(const size_t size) {
        using element_type = std::remove_extent_t<T>;
        return box<T>(new element_type[size]());
    }

    // Do not allow multi-dimensional array of T
    template <typename T, typename... ArgTs, std::enable_if_t<std::extent_v<T> != 0, int> = 0>
    void make_box(ArgTs&&...) = delete;

	

} // namespace fields_engine
