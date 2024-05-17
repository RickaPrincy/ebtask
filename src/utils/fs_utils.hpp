#pragma once
#include <json/json.hpp>
#include <string>

namespace ebtask
{
	const std::string FILE_SEPARTOR = "/";	// ebtask is only for unix os
	enum class PathExistErrorAction
	{
		OVERRIDE = 1,
		COPY = 2,
		ERROR = 3
	};

	std::string get_dumb_unique_prefix();
	void save_file(std::string path, const std::string &content);
	void save_json_file(std::string path, const nlohmann::json &content);
	nlohmann::json get_json_file_content(std::string path, bool required = false);
	void delete_file(std::string path);
	void delete_folder(std::string path);
	void copy_folder(std::string source, std::string destination);
	ebtask::PathExistErrorAction get_path_error_action_from_string(std::string action);
}  // namespace ebtask
