#pragma once
#include <json/json.hpp>
#include <string>
#include <unordered_map>

namespace ebtask
{
	const std::string FILE_SEPARTOR = "/";	// ebtask is only for unix os
	enum class PathExistErrorAction
	{
		OVERRIDE = 1,
		COPY = 2,
		ERROR = 3
	};

	ebtask::PathExistErrorAction get_remap_error_action_value(std::string error_action_string);
	std::string get_dumb_unique_prefix();
	void save_file(std::string path, const std::string &content);
	void save_json_file(std::string path, const nlohmann::json &content);
	nlohmann::json get_json_file_content(std::string path, bool required = false);
	void delete_file(std::string path);
	void delete_folder(std::string path);
	void copy_folder(std::string source, std::string destination);
	std::unordered_map<std::string, ebtask::PathExistErrorAction> get_enum_string_path_error();
}  // namespace ebtask
