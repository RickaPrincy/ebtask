#include <rcli/option.hpp>
#include <rcli/rcli.hpp>
#include <stdexcept>
#include <string>

#include "core/actions/actions.hpp"
#include "core/config/config.hpp"
#include "core/ebtask_config.hpp"
#include "core/run.hpp"
#include "utils/logger.hpp"

int main(int argc, const char *argv[])
{
	rcli::App ebtask("ebtask", "Execute your command in background task easily", EBTASK_VERSION);
	rcli::InputConfig input_config = rcli::InputConfig().required(true).clean(true);
	rcli::Option error_action_option("-e,--error-action",
		"Specify what to do if layout already existed (enum: [ ERROR - COPY - OVERRIDE ])",
		"error_action");
	rcli::Option name_option("-n,--name", "Specify new name", "name");

	// -----------------------------------------------------------------------------------
	rcli::Command remap("remap_layout",
		"Remap a layout for your keyboard",
		[&](rcli::Command *_remap)
		{
			auto layout_name = _remap->get_option_value("layout_name");
			const auto error_action = _remap->get_option_value("error_action");

			if (layout_name.empty())
				layout_name = rcli::ask_input_value(input_config.text("Layout name"));

			if (error_action.empty())
				ebtask::run([&]() { return ebtask::remap_layout(layout_name); });
			else
				ebtask::run([&]() { return ebtask::remap_layout(layout_name, error_action); });
		});
	remap.add_option(&error_action_option);
	remap.add_option(&name_option);

	// -----------------------------------------------------------------------------------
	rcli::Command config("init-config",
		"init a new configuration",
		[&](rcli::Command *_config)
		{
			auto error_action = _config->get_option_value("error_action");
			auto name = _config->get_option_value("name");

			if (name.empty())
				name = DEFAULT_CONFIG_FILE_NAME;
			ebtask::EbtaskConfig new_config = ebtask::EbtaskConfig::generate_config_template();
			new_config.save_config(name, error_action);
			ebtask::log(ebtask::get_config_file_path(name) + " was genereated !!");
		});
	config.add_option(&error_action_option);
	config.add_option(&name_option);

	// -----------------------------------------------------------------------------------
	ebtask.add_subcommand(&remap);
	ebtask.add_subcommand(&config);
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
