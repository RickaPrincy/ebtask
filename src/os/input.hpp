#pragma once

#include <functional>
#include <string>

namespace ebtask
{
	namespace os
	{
		namespace input
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
				KeyStatus status{ KeyStatus::RELEASED };
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
			using ReaderFunction = std::function<bool(int code, KeyStatus status, int &fd, const char *devnode)>;
		}  // namespace input
	}  // namespace os

}  // namespace ebtask
