
#include "precompiled.h"
#include <iostream>
#include "imGui/imgui.h"
#include "application.h"


int main() {
	json js;
	js["twelve"] = 5;
	std::cout << js << std::endl;
	std::cout << "Hello, world!" << std::endl;

	
	{
		fe::application app;
		if (!app.startup()) {

		}

		if (!app.shutdown()) {

		}
	}

	return 0;
}