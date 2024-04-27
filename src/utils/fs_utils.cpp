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

void ebtask::save_file(std::string path, const std::string &text)
{
	std::ofstream file(path);
	if (file.is_open())
	{
		file << text;
		return;
	}
	throw std::runtime_error("Cannot save " + path);
}

void ebtask::save_json_file(std::string path, const nlohmann::json &text)
{
	std::ofstream file(path);
	if (file.is_open())
	{
		file << text.dump(4);
		return;
	}
	throw std::runtime_error("Cannot save " + path);
}

void ebtask::delete_file(std::string path)
{
	if (!(std::remove(path.c_str()) == 0))
		throw std::runtime_error("Cannot delete " + path);
}

void ebtask::copy_folder(std::string source, std::string destination)
{
	try
	{
		fs::copy(source,
			destination,
			fs::copy_options::recursive | fs::copy_options::overwrite_existing);
	}
	catch (const std::exception &e)
	{
		throw std::runtime_error("Error when try to copy " + source + " to " + destination);
	}
}

void ebtask::delete_folder(std::string path)
{
	if (!fs::exists(path))
		return;

	try
	{
		fs::remove_all(path);
	}
	catch (const std::filesystem::filesystem_error &e)
	{
		std::string message = e.what();
		throw std::runtime_error(message);
	}
}

ebtask::PathExistErrorAction get_remap_error_action_value(std::string error_action_string)
{
	auto value = enum_error_action.find(error_action_string);
	if (value != enum_error_action.end())
	{
		return value->second;
	}
	throw std::runtime_error("Valid error_action are [ OVERRIDE | COPY | ERROR ]");
}

std::string ebtask::get_dumb_unique_prefix()
{
	static int id = 0;
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	return std::to_string(ms) + "_" + std::to_string(id++);
}
