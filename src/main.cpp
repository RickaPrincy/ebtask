#include <TColor/TColor.hpp>
#include <rcli/option.hpp>
#include <rcli/rcli.hpp>
#include <stdexcept>
#include <string>

#include "core/actions/actions.hpp"
#include "core/ebtask_config.hpp"
#include "core/run.hpp"
#include "utils/logger.hpp"

int main(int argc, const char *argv[])
{
	rcli::App ebtask("ebtask", "Execute your command in background task easily", EBTASK_VERSION);
	rcli::InputConfig input_config = rcli::InputConfig().required(true).clean(true);

	rcli::Option error_action_option("-e,--error-action",
		"Specify what to do if file config already existed (enum: [ ERROR - COPY - OVERRIDE ])",
		"error_action");
	rcli::Option name_option("-n,--name", "Specify new name", "name");
	rcli::Option devnode_option("-i,--input-event", "Specify new input event on your os", "devnode");

	// -----------------------------------------------------------------------------------
	rcli::Command remap("remap_layout",
		"Remap a layout for your keyboard",
		[&](rcli::Command *_remap)
		{
			auto layout_name = _remap->get_option_value("name");
			auto error_action = _remap->get_option_value("error_action");
			auto devnode = _remap->get_option_value("devnode");

			if (layout_name.empty())
				layout_name = rcli::ask_input_value(input_config.text("Layout name"));

			ebtask::run(
				[&]() { return ebtask::remap_layout(layout_name + LAYOUT_CONFIG_SUFFIX, error_action); }, devnode);
		});
	remap.add_option(&name_option);
	remap.add_option(&devnode_option);
	remap.add_option(&error_action_option);

	// -----------------------------------------------------------------------------------
	rcli::Command config("init_config",
		"init a new configuration",
		[&](rcli::Command *_config)
		{
			auto error_action = _config->get_option_value("error_action");
			auto name = _config->get_option_value("name");

			if (name.empty())
				name = DEFAULT_CONFIG_FILE_NAME;

			ebtask::generate_config(name + CONFIG_SUFFIX, error_action);
		});
	config.add_option(&name_option);
	config.add_option(&error_action_option);

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
			ebtask::run([&]()
				{ return ebtask::listen_event(layout_name + LAYOUT_CONFIG_SUFFIX, config_name + CONFIG_SUFFIX); },
				devnode);
		});
	listen.add_option("-l,--layout", "Specify layout to use (only name without .layout.json)", "layout_name");
	listen.add_option("-c,--config", "Specify config to use (only name without .config.json)", "config_name");
	listen.add_option(&devnode_option);

	// -----------------------------------------------------------------------------------
	ebtask.add_subcommand(&remap);
	ebtask.add_subcommand(&config);
	ebtask.add_subcommand(&listen);

	try
	{
		ebtask.run(argc, argv);
	}
	catch (const std::runtime_error &error)
	{
		ebtask::cerr(error.what());
	}

	return EXIT_SUCCESS;
}
