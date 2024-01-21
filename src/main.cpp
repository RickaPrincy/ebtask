#include <iostream>

#include "cli/CLI11.hpp"
#include "config.h"
#include "keymap.h"
#include "start.h"

int main(int argc, const char *argv[])
{
	Config ebtask_config = get_config();
	CLI::App ebtask("Execute background easily");

	ebtask.add_subcommand("create_layout", "Remap a layout for your keyboard")
		->callback([&ebtask_config]() { create_layout(ebtask_config._path); });

	ebtask.add_subcommand("start", "launch the ebtask application")->callback([]() { start(); });

	CLI11_PARSE(ebtask, argc, argv);
	return EXIT_SUCCESS;
}
