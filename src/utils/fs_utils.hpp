#pragma once

#include <nlohmann/json.hpp>
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
	void write_json_file(std::string path, const nlohmann::json &content);
	nlohmann::json read_json_file(std::string path, bool required = false);
	void copy(std::string source, std::string destination);
	ebtask::PathExistErrorAction get_path_error_action_from_string(std::string action);
}  // namespace ebtask
