#include "cli/CLI11.hpp"
#include "command_actions.h"
#include "setup.h"

int main(int argc, const char *argv[])
{
	CLI::App ebtask("Execute your command in background task easily");

	ebtask.add_subcommand("remap", "Remap a layout for your keyboard")
		->callback([]() { start(remap_layout); });

	ebtask.add_subcommand("listen", "Launch the ebtask application")
		->callback([]() { start(listen_event); });

	CLI11_PARSE(ebtask, argc, argv);
	return EXIT_SUCCESS;
}
