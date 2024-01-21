#include <iostream>

#include "cli/CLI11.hpp"
#include "command_actions.h"
#include "start.h"
#include "utils/utils.h"

int main(int argc, const char *argv[])
{
	CLI::App ebtask("Execute background easily");

	ebtask.add_subcommand("create_layout", "Remap a layout for your keyboard")
		->callback(
			[]()
			{
				ebtask::log("Creating layout started, type on key to set it values");
				ebtask::log("-------------------------------------------------------");
				start(create_layout);
			});

	ebtask.add_subcommand("launch", "Launch the ebtask application")
		->callback([]() { start(listen); });

	CLI11_PARSE(ebtask, argc, argv);
	return EXIT_SUCCESS;
}
