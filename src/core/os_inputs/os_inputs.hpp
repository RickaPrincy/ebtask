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
		int _code{ 1 };
		std::string _normal{}, _altgr{}, _shift{}, _capslock{};
		ebtask::KeyStatus _status{ ebtask::KeyStatus::RELEASED };
		Key() = default;

		Key &operator=(const Key &other)
		{
			if (this == &other)
				return *this;
			_code = other._code;
			_normal = other._normal;
			_altgr = other._altgr;
			_shift = other._shift;
			_capslock = other._capslock;
			_status = other._status;
			return *this;
		}
	};

	using ReaderFunction =
		std::function<bool(int code, ebtask::KeyStatus status, int &fd, const char *devnode)>;

	std::string guess_input_file();
	void read_input_file(const char *devnode, ebtask::ReaderFunction callback);

}  // namespace ebtask
