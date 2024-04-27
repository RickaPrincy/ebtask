#pragma once

#include <functional>
#include <string>

namespace ebtask
{
	enum class KeyStatus
	{
		RELEASED = 0,
		PRESSED = 1,
		REPEATED = 2
	};

	class Key
	{
	public:
		int _code{ 1 };
		std::string _normal{}, _altgr{}, _shift{}, _capslock{};
		ebtask::KeyStatus _status{ ebtask::KeyStatus::RELEASED };
		Key() = default;
	};

	using ReaderFunction =
		std::function<void(int code, ebtask::KeyStatus status, int &fd, const char *devnode)>;

	std::string guess_input_file();
	void read_input_file(const char *devnode, ebtask::ReaderFunction callback);

}  // namespace ebtask
