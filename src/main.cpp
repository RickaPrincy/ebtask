#include <iostream>

#include "cli/CLI11.hpp"
#include "utils/utils.h"
#include "start.h"

int main(int argc, const char *argv[])
{
	CLI::App ebtask("Execute background easily");

	ebtask.add_subcommand("create_layout", "Remap a layout for your keyboard")
		->callback([]() { start(StartType::CREATE_LAYOUT); });
	
    ebtask.add_subcommand("launch", "Launch the ebtask application")->callback([]() { start(StartType::NORMAL); });

	CLI11_PARSE(ebtask, argc, argv);
	return EXIT_SUCCESS;
}
