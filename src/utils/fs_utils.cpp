#include "fs_utils.hpp"

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;
static const std::unordered_map<std::string, ebtask::PathExistErrorAction> enum_error_action = {
	{ "COPY", ebtask::PathExistErrorAction::COPY },
	{ "ERROR", ebtask::PathExistErrorAction::ERROR },
	{ "OVERRIDE", ebtask::PathExistErrorAction::OVERRIDE }
};

void ebtask::write_json_file(std::string path, const nlohmann::json &text)
{
	std::ofstream file(path);
	if (file.is_open())
	{
		file << text.dump(2);
		return;
	}
	throw std::runtime_error("Cannot save " + path);
}

void ebtask::copy(std::string source, std::string destination)
{
	try
	{
		fs::copy(source, destination, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
	}
	catch (const std::exception &e)
	{
		throw std::runtime_error("Error when try to copy " + source + " to " + destination);
	}
}

std::string ebtask::get_dumb_unique_prefix()
{
	static int id = 0;
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	return std::to_string(ms) + "_" + std::to_string(id++);
}

nlohmann::json ebtask::read_json_file(std::string path, bool required)
{
	std::ifstream config_file(path);
	if (!config_file.is_open() && required)
		throw std::runtime_error(path + " was not found");

	nlohmann::json config_file_content = nlohmann::json::object({});

	try
	{
		if (config_file.is_open())
			config_file_content = nlohmann::json::parse(config_file);
	}
	catch (const nlohmann::json::exception &error)
	{
		config_file.close();
		throw std::runtime_error(path + " is not a valid json file");
	}

	config_file.close();
	return config_file_content;
}

ebtask::PathExistErrorAction ebtask::get_path_error_action_from_string(std::string action)
{
	auto error_action = enum_error_action.find(action);
	if (error_action == enum_error_action.end())
		throw std::runtime_error("Valid error_action are [ OVERRIDE | COPY | ERROR ]");
	return error_action->second;
}
