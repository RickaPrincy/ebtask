#pragma once

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
		ebtask::KeyBinding _keybinding{};
		std::string _function{}, _command{}, _output_reader;
		Action() = default;
	};

	class Mode
	{
	public:
		std::vector<Action> _actions{};
		ebtask::KeyBinding _keybinding{};
		std::string _name{}, _output_reader{}, _input_cleaner{}, _on_start{}, _on_stop{};
		bool _log_action{ true };
		ebtask::ActionHandler _hanlder_type;
		Mode() = default;
	};

	class EbtaskConfig
	{
	public:
		ebtask::KeyBinding _normal_mode_keybinding{};
		std::vector<Mode> _modes;

		void save_config(std::string file_config_path);
		static EbtaskConfig from_config_file(std::string file_config_path);
		static EbtaskConfig generate_config_template();
		EbtaskConfig() = default;
	};

	std::string handle_config_file_already_exist_error(std::string file_path,
		std::string error_action);
	std::string get_config_path();
	std::string get_config_file_path(std::string file_name);
}  // namespace ebtask
