#pragma once

#include <functional>
#include <string>

namespace ebtask
{
	enum class KeyStatus
	{
		NOT_FOUND = -1,
		RELEASED = 0,
		PRESSED = 1,
		REPEATED = 2
	};

	class Key
	{
	public:
		int code{ 1 };
		std::string normal{}, altgr{}, shift{}, capslock{};
		ebtask::KeyStatus status{ ebtask::KeyStatus::RELEASED };
		Key() = default;

		Key &operator=(const Key &other)
		{
			if (this == &other)
				return *this;
			code = other.code;
			normal = other.normal;
			altgr = other.altgr;
			shift = other.shift;
			capslock = other.capslock;
			status = other.status;
			return *this;
		}
	};

	using ReaderFunction =
		std::function<bool(int code, ebtask::KeyStatus status, int &fd, const char *devnode)>;

	std::string guess_input_file();
	void read_input_file(const char *devnode, ebtask::ReaderFunction callback);

}  // namespace ebtask
