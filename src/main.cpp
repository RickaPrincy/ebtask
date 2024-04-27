#include <rcli/option.hpp>
#include <rcli/rcli.hpp>
#include <stdexcept>
#include <string>

#include "core/actions/actions.hpp"
#include "core/ebtask_config.hpp"
#include "core/run.hpp"
#include "utils/fs_utils.hpp"
#include "utils/logger.hpp"

int main(int argc, const char *argv[])
{
	rcli::App ebtask("ebtask", "Execute your command in background task easily", EBTASK_VERSION);
	rcli::InputConfig input_config = rcli::InputConfig().required(true).clean(true);

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
				ebtask::run(
					[&]() {
						return ebtask::remap_layout(
							layout_name, ebtask::get_remap_error_action_value(error_action));
					});
		});

	remap.add_option("-n,--layout-name", "Specify the layout new name", "layout_name");
	remap.add_option("-e,--error-action",
		"Specify what to do if layout already existed (enum: [ ERROR - COPY - OVERRIDE ])",
		"error_action");
	ebtask.add_subcommand(&remap);

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
