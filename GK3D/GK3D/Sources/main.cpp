#include "Application.h"

int main()
{
	auto app = std::make_shared<Application>();
	return app->run();
}
