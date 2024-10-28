#pragma once

#include <vector>
#include <string>
#include "json-develop/single_include/nlohmann/json.hpp"

using json = nlohmann::json;
using string = std::string;
using string_view = std::string_view;

namespace fe = fields_engine;