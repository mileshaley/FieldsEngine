#pragma once

#include <vector>
#include <string>
#include "json-develop/single_include/nlohmann/json.hpp"

#define FIELDS_ENGINE
#define IF_CONSTEXPR if constexpr
#define CONSTEXPR constexpr
#define INLINE inline
#define NODISCARD [[nodiscard]]
#define FALLTHROUGH [[fallthrough]]

using json = nlohmann::json;
using string = std::string;
using string_view = std::string_view;

namespace fields_engine {}
namespace fe = fields_engine;