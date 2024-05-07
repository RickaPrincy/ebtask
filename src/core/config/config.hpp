#pragma once

#include <map>
#include <string>
#include <vector>

namespace ebtask
{
	using KeyBinding = std::vector<int>;

	enum class ActionHandler
	{
		FUNCTION,
		KEYBINDING
	};

	class Action
	{
	public:
		ebtask::KeyBinding keybinding{};
		std::string function{}, command{};
		Action() = default;
	};

	class Mode
	{
	public:
		std::vector<Action> actions{};
		ebtask::KeyBinding keybinding{};
		std::string name{}, output_reader{}, input_cleaner{}, on_start{}, on_stop{};
		bool log_action{ true };
		ebtask::ActionHandler handler_type;
		Mode() = default;
	};

	class EbtaskConfig
	{
	public:
		ebtask::KeyBinding normal_mode_keybinding{};
		std::vector<Mode> modes;
		std::map<std::string /*alias*/, std::string /*value*/> alias;

		void save_config(std::string file_config_path);
		std::string get_alias_value(std::string alias_name);
		static EbtaskConfig get_config_from_config_file(std::string file_config_path);
		static EbtaskConfig generate_new_config_template();
		EbtaskConfig() = default;
	};

	std::string handle_config_file_already_exist_error(std::string file_path,
		std::string error_action);
	std::string get_config_path();
	std::string get_config_file_path(std::string file_name);
}  // namespace ebtask
