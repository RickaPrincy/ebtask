#pragma once

#include <string>
#include <vector>

#include "../../utils/fs_utils.hpp"
#include "../ebtask_config.hpp"

namespace ebtask
{
	class Action
	{
	public:
		std::vector<int> _keybinding{};
		std::string _function{}, _command{};
		Action() = default;
	};

	class Mode
	{
	public:
		std::vector<Action> _actions{};
		std::vector<int> _keybinding{};
		std::string _name{}, _type{};
		Mode() = default;
	};

	class EbtaskConfig
	{
	public:
		void read_config(std::string config_name = DEFAULT_CONFIG_FILE_NAME,
			ebtask::PathExistErrorAction error_action = ebtask::PathExistErrorAction::ERROR);
	};
	std::string get_config_path();
	std::string get_config_file_path(std::string file_name);
}  // namespace ebtask
