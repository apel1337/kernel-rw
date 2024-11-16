#include "Windows.h"
#include <iostream>

#include "driver/driver.h"

auto main() -> int
{
	auto status = mem::find_driver();
	if (!status)
	{
		printf("driver not found");
		std::cin.get();
	}
	std::cin.get();
	return 0;
}