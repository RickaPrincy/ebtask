#ifndef __EBTASK_CONF_TYPES__
#define __EBTASK_CONF_TYPES__

#define EBTASK_PATH_ENV "EBTASK_PATH"

#include <functional>
#include <string>
#include <vector>

enum class KeyStatus
{
	NOT_FOUND = -1,
	RELEASED = 0,
	PRESSED = 1
};

namespace ECallBack
{
	using ReadFunction =
		std::function<bool(int code, KeyStatus status, int &fd, const char *devnode)>;
	using StartFunction = ReadFunction (*)();
}  // namespace ECallBack

class Key
{
public:
	int _code{ 1 };
	std::string _normal{}, _altgr{}, _shift{}, _capslock{};
	KeyStatus _status{ KeyStatus::RELEASED };
	Key(){};
};

class Action
{
public:
	std::vector<int> _keybinding{};
	std::string _function{}, _command{};
	Action(){};
};

class Mode
{
public:
	std::vector<Action> _actions{};
	std::vector<int> _keybinding{};
	std::string _name{""}, _type{""}, _render_command{""}, _unmount_command{""};
    bool _is_function{false}, _log{true};
	Mode(){};
};

class Config
{
public:
	std::vector<int> _stop_keybinding{};
	std::string _path;
	Config(){};
};

#endif	// !__EBTASK_CONF_TYPES__
