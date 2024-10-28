
#include <iostream>
#include "imGui/imgui.h"
#include "json-develop/single_include/nlohmann/json.hpp"

using Json = nlohmann::json;

int main() {
	Json json;
	json["twelve"] = 5;
	std::cout << json << std::endl;
	std::cout << "Hello, world!" << std::endl;
	return 0;
}