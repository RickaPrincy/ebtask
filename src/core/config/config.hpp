#pragma once

#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace ebtask
{
	namespace config
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
			KeyBinding keybinding{};
			std::string function{}, command{};
			Action() = default;
		};

		class Mode
		{
		public:
			std::vector<Action> actions{};
			KeyBinding keybinding{};
			std::string name{}, on_start{}, on_stop{};
			bool log_action{ true };
			ActionHandler handler_type{ ActionHandler::KEYBINDING };
			Mode() = default;
		};

		class EbtaskConfig
		{
		public:
			KeyBinding normal_mode_keybinding{};
			std::vector<Mode> modes{};
			std::map<std::string /*alias*/, std::string /*value*/> alias{}, specific_alias{};

			nlohmann::json to_json();
			void save(std::string path);
			std::string replace_all_alias(std::string command);

			EbtaskConfig() = default;
			EbtaskConfig(std::string file_config_path);
			EbtaskConfig& operator=(const EbtaskConfig& other);

			static EbtaskConfig read_ebtask_config_file(std::string file_config_path);
			static EbtaskConfig generate_new_config_template();
			static std::string handle_config_file_already_exist_error(std::string file_path, std::string error_action);
			static std::string get_config_base_path();
			static std::string append_config_base_path(std::string file_name);

		private:
			EbtaskConfig(bool generate) { };
		};

	}  // namespace config
}  // namespace ebtask
