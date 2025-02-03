#include <TColor/TColor.hpp>
#include <cstdlib>
#include <rcli/option.hpp>
#include <rcli/rcli.hpp>
#include <stdexcept>
#include <string>

#include "core/actions/actions.hpp"
#include "core/run.hpp"
#include "generated_config.hpp"
#include "utils/logger.hpp"

int main(int argc, const char *argv[])
{
	rcli::App ebtask("ebtask", "Execute a command anywhere you are", EBTASK_VERSION);
	rcli::InputConfig input_config = rcli::InputConfig().required(true).clean(true);

	rcli::Option error_action_option("-e,--error-action",
		"Specify what to do if file config already existed (enum: [ ERROR - COPY - OVERRIDE ])",
		"error_action");
	rcli::Option name_option("-n,--name", "Specify name of the config", "name");
	rcli::Option devnode_option("-i,--input-event", "Specify keyboard input event on your os", "devnode");

	// -----------------------------------------------------------------------------------
	rcli::Command configure_layout("configure-layout",
		"Configure your layout",
		[&](rcli::Command *_configure_layout)
		{
			auto layout_name = _configure_layout->get_option_value("name");
			auto error_action = _configure_layout->get_option_value("error_action");
			auto devnode = _configure_layout->get_option_value("devnode");

			if (layout_name.empty())
				layout_name = DEFAULT_CONFIG_FILE_NAME;

			if (devnode.empty())
				devnode = DEFAULT_DEVNODE_PATH;

			ebtask::run(
				[&]() { return ebtask::configure_layout(layout_name + LAYOUT_CONFIG_SUFFIX, error_action); }, devnode);
		});
	configure_layout.add_option(&name_option);
	configure_layout.add_option(&devnode_option);
	configure_layout.add_option(&error_action_option);

	// -----------------------------------------------------------------------------------
	rcli::Command new_config("new-config",
		"Create a new config",
		[&](rcli::Command *_new_config)
		{
			auto error_action = _new_config->get_option_value("error_action");
			auto name = _new_config->get_option_value("name");

			if (name.empty())
				name = DEFAULT_CONFIG_FILE_NAME;

			ebtask::generate_config(name + CONFIG_SUFFIX, error_action);
		});
	new_config.add_option(&name_option);
	new_config.add_option(&error_action_option);

	// -----------------------------------------------------------------------------------
	rcli::Command listen("listen",
		"Start ebtask",
		[&](rcli::Command *_listen)
		{
			auto layout_name = _listen->get_option_value("layout_name");
			auto config_name = _listen->get_option_value("config_name");
			auto devnode = _listen->get_option_value("devnode");

			if (layout_name.empty())
				layout_name = DEFAULT_CONFIG_FILE_NAME;

			if (config_name.empty())
				config_name = DEFAULT_CONFIG_FILE_NAME;

			if (devnode.empty())
				devnode = DEFAULT_DEVNODE_PATH;

			ebtask::run([&]()
				{ return ebtask::listen_event(layout_name + LAYOUT_CONFIG_SUFFIX, config_name + CONFIG_SUFFIX); },
				devnode);
		});
	listen.add_option("-l,--layout", "Specify layout to use (only name without .layout.json)", "layout_name");
	listen.add_option("-c,--config", "Specify config to use (only name without .config.json)", "config_name");
	listen.add_option(&devnode_option);

	// -----------------------------------------------------------------------------------
	ebtask.add_subcommand(&configure_layout);
	ebtask.add_subcommand(&new_config);
	ebtask.add_subcommand(&listen);

	try
	{
		ebtask.run(argc, argv);
	}
	catch (const std::runtime_error &error)
	{
		ebtask::cerr(error.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
