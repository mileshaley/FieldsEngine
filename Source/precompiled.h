/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: precompiled.h														 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include <vector>
#include <string>
#include "json-develop/single_include/nlohmann/json.hpp"



using json = nlohmann::json;
using string = std::string;
using string_cr = string const&;
using string_view = std::string_view;
using std::vector;



#define FIELDS_ENGINE
#define IF_CONSTEXPR if constexpr
#define CONSTEXPR constexpr
#define INLINE inline
#define NODISCARD [[nodiscard]]
#define FALLTHROUGH [[fallthrough]]

#include "common.h"

namespace fields_engine {
	using namespace common;
} // namespace fields_engine

namespace fe = fields_engine;

